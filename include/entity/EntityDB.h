#pragma once

#include <algorithm>
#include <chrono>
#include <concepts>
#include <execution>
#include <memory>
#include <optional>
#include <vector>
#include <utility>
#include <ranges>
#include <range/v3/all.hpp>
#include <boost/dynamic_bitset.hpp>
#include "component/Component.h"
#include "Entity.h"

namespace se {

	// EntityDB에서 만든 EntityID를 다른 EntityDB 인스턴스에서 쓰는 경우는 없겠지?
	// 거기에 대한 대비책도 있어야 하나??
	class EntityDB final {
	public:
		//struct EntityID final {
		//	std::size_t id;
		//	friend class EntityDB;
		//private:
		//	std::size_t tm;
		//};


		// 실제 id != 해당 ComponentVector에서의 인덱스
		// 컴포넌트 제거를 가장 끝에 있는 컴포넌트와 삭제할 컴포넌트의 위치를 바꾸는 것으로 구현할 생각임
		// 그리고 나서 last_component를 1 줄이는 거지
		// 정렬될 필요는 없고 메모리 재할당을 최소화 하기 위함
		// 그에 따라 id에 표기를 위한 id에 더해 실제 인덱스를 담을 변수를 넣어야 할지 생각해야 함
		// EntityID 또한 같음
		// 혹은 version 정보를 넣어서 재활용을 계속 하던가
		// 리스트면 위 방법이 괜찮겠는데 벡터니까 안쓰는 오브젝트용 벡터를 하나 더 두는게 나을 듯
		struct ComponentID final {
			std::size_t id{};

			ComponentID() = default;

			explicit ComponentID(std::size_t id) : id(id) {};

			auto operator==(ComponentID&) -> bool;
		};

		auto createEntity() -> Entity& {
			Entity entity;
			entity.id.id = nextEid();
			if (!recycleEntities.empty())
			{
				entity.id.index = recycleEntities.back();
				recycleEntities.pop_back();
			}
			else //여기서 emplace_back이 낫나? 아니면 그냥 다 초기화시켜버리고 push_back을 하는게낫나
			{
				entity.id.index = entities.size();
				//entities.emplace_back();
			}
			entity.mask.clear();
			entities.push_back(std::move(entity));
			return entities.back();
		}

		auto removeEntity(Entity& e) -> void
		{
			const auto index = e.id.index;
			if (entities[index].id.index != e.id.index)
				return;

			// do something?

			++entities[index].id.index;
			entities[index].mask.clear();
			recycleEntities.push_back(index);
		}

		template<component Component>
		auto addComponent(Entity& e, Component&& c) {
			using component_type = std::decay_t<Component>;
			getComponentVector<component_type>()->push_back(e.id, std::forward<component_type>(c));
			addMask<component_type>(e.mask);
		}

//		template<component Component, component... Components>
//		auto addComponent(Entity::EntityID eid, Component&& c, Components && ...cs) {
//			addComponent(eid, c);
//			ᅟaddComponent(eid, cs...);
//		}

		template <component Component>
		auto removeComponent(Entity& e) {
			//비트마스크 없애기
			removeMask<Component>(e.mask);

			//맵가서 없애기
			auto cid = getComponentVector<Component>()->findComponentID(e);
			if (cid)
			{
				for (auto iter : getComponentVector<Component>()->EntityComponentMap)
				{
					if (iter.second == *cid)
						getComponentVector<Component>()->EntityComponentMap.remove(iter);
				}
			}
		}

		template<component Component>
		auto getComponent(Entity::EntityID eid) -> std::conditional_t<std::is_pointer_v<Component>, Component, void> {
		    if constexpr (std::is_pointer_v<Component>) {
		        using PureComponent = std::remove_pointer_t<Component>;
				
				if (entities[eid.index].id.index != eid.index)
					return nullptr;

                auto cid = getComponentVector<PureComponent>()->findComponentID(eid);
                if (cid)
                    return getComponentVector<PureComponent>()->getComponent(*cid);
                else
                    return nullptr;
		    }
		}

        // TODO : Callable 타입 제한 둬야함
        template<typename Callable>
        auto addSystem(Callable callable){
            systems.push_back(std::make_unique<System<Callable>>(callable, this));
        }

        auto runSystems() -> void;

    private:
        // ---- Utils ----
        // TODO : 우측값 받아서 값 리턴하게 수정?
        template<component ... Cs>
        static auto addMask(boost::dynamic_bitset<>& mask) -> typename std::enable_if<(sizeof...(Cs) == 0)>::type {}

        template<component C, component ... Cs>
        static auto addMask(boost::dynamic_bitset<>& mask) -> void {
            auto i = getGroupId<C>();
            if (mask.size() <= i)
                mask.resize(i + 1);
            mask.set(i);
            addMask<Cs ...>(mask);
        }

        template<component C>
        static auto removeMask(boost::dynamic_bitset<>& mask) -> void {
            auto i = getGroupId<C>();
            if (mask.size() <= i)
                mask.resize(i + 1);
            mask.reset(i);
        }

        // ---- Component Vector ----
        struct ComponentVectorBase {
		    virtual ~ComponentVectorBase() = default;
		};

		template<component Component>
		class ComponentVector final : public ComponentVectorBase {
		public:
			auto push_back(Entity::EntityID eID, Component&& c)
			{
				//맨첨에 gcc랑 msvc 둘다 capacity=0
				// components.reserve(components.size() + 1);
				components.push_back(c);

				ComponentID cid{components.size() - 1};

				// EntityComponentMap.emplace_back( eID, cid );
                EntityComponentMap2.emplace(std::make_pair(eID, cid));
			}

//			auto findEntityID(ComponentID cid) -> std::optional<Entity::EntityID>
//			{
//				auto eid = std::find_if(EntityComponentMap.cbegin(), EntityComponentMap.cend(),
//					[&cid](std::pair<Entity::EntityID, ComponentID> pair) {
//						return pair.second == cid;
//					}
//				);
//				if (eid != EntityComponentMap.cend())
//					return (*eid).first;
//				else
//					return std::nullopt;
//			}

			auto findComponentID(Entity::EntityID eid)->std::optional<ComponentID>
			{
//				auto cid = std::find_if(EntityComponentMap.cbegin(), EntityComponentMap.cend(),
//					[&eid](std::pair<Entity::EntityID, ComponentID> pair) {
//						return pair.first == eid;
//					}
//				);
//				if (cid != EntityComponentMap.cend())
//					return (*cid).second;
//				else
//					return std::nullopt;
                if (EntityComponentMap2.find(eid) != EntityComponentMap2.end())
                    return ComponentID{EntityComponentMap2[eid].id};
                else
                    return std::nullopt;
			}

			auto getComponent(ComponentID cid) {
			    return &components[cid.id];
			}

		private:
			std::vector<Component> components;
			std::unordered_map<Entity::EntityID, ComponentID> EntityComponentMap2;
			//std::vector<std::pair<Entity::EntityID, ComponentID>> EntityComponentMap;
			//std::vector<EntityID> component_to_entity;
			//std::vector<ComponentID> entity_to_component;
		};

        // ---- System Traits ----
        template<component ... ArgsOfCallable>
        struct system_traits_args {
            system_traits_args() {
                mask.clear();
                addMask<ArgsOfCallable...>(mask);
            }

            boost::dynamic_bitset<> mask;

            // TODO : 아래 내용 생각해볼것
            // 여기는 mask 정보만 들고 있고 실제 함수 실행은 타입의 mask(guid)로 getComponent 하는 함수를 만들어서
            // System에 배치하는게 좀 더 직관적인가?
            template<typename Callable>
            auto apply(EntityDB* db, Callable func, Entity& e) {
                return std::forward<Callable>(func)(db->getComponent<ArgsOfCallable>(e.id)...);
            }
        private:
//            struct static_bitset_constructor {
//                static_bitset_constructor(){
//                    addMask<ArgsOfCallable ...>(system_traits_args::mask);
//                }
//            };
//
//            static static_bitset_constructor c;
        };

        template<typename Callable>
	    struct system_traits : public system_traits<decltype(&std::decay_t<Callable>::operator())> {};

        template<typename R, component ... Components>
        struct system_traits<R (&)(Components...)> : public system_traits_args<Components...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...)> : public system_traits_args<Components...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...) &> : public system_traits_args<Components...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...) &&> : public system_traits_args<Components...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...) const> : public system_traits_args<Components...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...) const &> : public system_traits_args<Components...> {};

        // ---- System ----
        struct SystemBase {
            virtual ~SystemBase() = default;
            virtual auto update(Entity&) -> void = 0;
        };

        template<typename Callable>
        class System : public SystemBase {
        public:
            System(Callable callable, EntityDB* db) : callback(callable), db(db) {};

            auto update(Entity& e) -> void override {
                auto traits = EntityDB::system_traits<Callable>{};
				traits.mask.resize(e.mask.size());
				if(traits.mask.is_proper_subset_of(e.mask) || traits.mask == e.mask)
                    traits.apply(db, callback, e);
            }

        private:
            EntityDB *db;
            Callable callback;
        };

        // ---- Member var and func ----
        std::vector<Entity> entities;
        std::vector<Entity::EntityID::index_type> recycleEntities; //재활용될 엔티티들;
        std::vector<std::unique_ptr<ComponentVectorBase>> component_vectors;
        std::vector<std::unique_ptr<SystemBase>> systems;

        std::size_t eid_counter = 0;

        auto nextEid() -> std::size_t;

        template<component C>
		auto getComponentVector() -> ComponentVector<C>* {
			auto group_id = getGroupId<C>();

			if (component_vectors.size() <= group_id) {
				component_vectors.resize(group_id + 1);
                component_vectors[group_id] = std::make_unique<ComponentVector<C>>();
			}

			return static_cast<ComponentVector<C>*>(component_vectors[group_id].get());
		}
    };
} // namespace se
