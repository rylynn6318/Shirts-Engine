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
		// addComponent를 비롯해 대부분 컴포넌트의 포인터는 허용되지 않음
		// 예외적으로 컴포넌트의 포인터를 신경써야 하는게 시스템과 시스템에서 필요한 unsafeGetComponent()임

		struct ComponentID final {
			std::size_t id{};
			std::size_t recycleCounter = 0;
			ComponentID() = default;

			explicit ComponentID(std::size_t id) : id(id) {};

			auto operator==(ComponentID&) -> bool;
		};

		auto createEntity() -> Entity::ID {
			if (recycleEntities.empty())
			{
				// create new entity
				entities.emplace_back(Entity{ Entity::ID{entities.size()} });
				return entities.back().id;
			}
			else
			{
				// recycle entity
				auto entity_to_recycle = recycleEntities.back();
				recycleEntities.pop_back();
				entity_to_recycle.recycle_counter++;

				return entity_to_recycle;
			}
		}

		// TODO : move 해버린다던가 해서 해당 eid 소멸시켜서 더 이상 접근 못하게 하면 좋을듯
		auto destroyEntity(Entity::ID eid) -> void
		{
			recycleEntities.push_back(eid);

			auto i = entities[eid.index].mask.find_first();

			while (i != boost::dynamic_bitset<>::npos) {
				component_vectors[i].get()->remove(eid);
				i = entities[eid.index].mask.find_next(i);
			}
			entities[eid.index].mask.clear();
		}

		template<component Component>
		auto addComponent(Entity::ID eid, Component&& c) {
			using component_type = std::decay_t<Component>;
			getComponentVector<component_type>()->pushBack(eid, std::forward<component_type>(c));
			addMask<component_type>(entities[eid.index].mask);
		}

		template<component Component, component... Components>
		auto addComponent(Entity::ID eid, Component&& c, Components && ... cs) {
			addComponent(eid, std::forward<Component>(c));
			addComponent(eid, std::forward<Components>(cs)...);
		}

		template <component Component>
		auto removeComponent(Entity::ID eid) {
			component_vectors[getGroupId<Component>()].get()->remove(eid);
			removeMask<Component>(entities[eid.index].mask);
		}

		// TODO : Callable 타입 제한 둬야함
		template<typename Callable>
		auto addSystem(Callable callable) {
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

			virtual auto remove(Entity::ID eid) -> void = 0;
		};

		template<component Component>
		class ComponentVector final : public ComponentVectorBase {
		public:
			using PureComponent = std::conditional_t<std::is_pointer_v<Component>, std::remove_pointer_t<Component>, std::decay_t<Component>>;

			auto pushBack(Entity::ID eid, Component&& c)
			{
				ComponentID cid;
				if (recycleComponents.empty())
				{
					cid_to_eid_map.push_back(eid);
					components.push_back(c);
					cid = ComponentID{ components.size() - 1 };
				}
				else
				{
					cid = recycleComponents.back();
					recycleComponents.pop_back();
					components[cid.id] = c;
				}

				eid_to_cid_map.emplace(eid, cid);
			}

			auto remove(Entity::ID eid) -> void override {
				auto iter = eid_to_cid_map.find(eid);
				if (iter != eid_to_cid_map.end())
				{
					if (iter->first.recycle_counter != eid.recycle_counter)
						return;

					auto cid = iter->second;
					cid.recycleCounter++;
					recycleComponents.push_back(cid);

					eid_to_cid_map.erase(iter);
				}
			}

			// TODO : 삭제된 엔티티의 컴포넌트와 존재하지 않는 cid를 파라메터로 넘긴 경우의 예외처리 필요함
			auto findEntityID(ComponentID cid) -> Entity::ID
			{
				return cid_to_eid_map[cid.id];
			}

			auto findComponentID(Entity::ID eid) -> std::optional<ComponentID>
			{
				auto cid = eid_to_cid_map.find(eid);
				if (cid != eid_to_cid_map.end())
					return cid->second;
				else
					return std::nullopt;
			}

			// TODO : 존재하지 않는 cid를 파라메터로 넘긴 경우의 예외처리 필요함
			auto getComponentPtr(ComponentID cid) -> PureComponent* {
				return &components[cid.id];
			}

			auto getComponentRef(ComponentID cid) -> PureComponent& {
				return components[cid.id];
			}

		private:
			std::vector<PureComponent> components;
			std::unordered_map<Entity::ID, ComponentID> eid_to_cid_map;
			std::vector<Entity::ID> cid_to_eid_map;
			std::vector<ComponentID> recycleComponents;
		};

		// ---- System Traits ----

		// 시스템에 파라메터로 올 수 있는 것 : se::Component<>를 상속받은 객체의 포인터, 레퍼런스. const 가능
		// 근데 decay_t를 하는 이유 : 어짜피 Callable 정의할때 const 박았으면 함수 내에서 값 수정 못하니 굳이 const 처리를 할 필요 없음.
		// 레퍼런스나 값은? 굳이 copy 하려는 사람은 없을거임. 무적권 레퍼런스 쓴다고 가정.
		// 컴포넌트 배열을 decay_t 하면 Component*가 나오겠지만 배열은 컨셉 단계에서 컷 되니 ㄱㅊ
		// 결국 실제로 처리해야 하는건 포인터 경우와 레퍼런스 경우, 마스크는 순수 타입으로 만들어 주고 나머지는 unsafeGetComponent()에서 따로 처리 해야함
		template<component ... ArgsOfCallable>
		struct system_traits_args {
			system_traits_args() {
				mask.clear();
				addMask<ArgsOfCallable...>(mask);
			}

			boost::dynamic_bitset<> mask;

			// TODO : 아래 내용 생각해볼것
			// 여기는 mask 정보만 들고 있고 실제 함수 실행은 타입의 mask(guid)로 unsafeGetComponent 하는 함수를 만들어서
			// System에 배치하는게 좀 더 직관적인가?
			template<typename Callable>
			auto apply(EntityDB* db, Callable func, Entity& e) {
				return std::forward<Callable>(func)(db->unsafeGetComponent<ArgsOfCallable>(e.id)...);
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
		struct system_traits<R(*)(Components...)> : public system_traits_args<std::decay_t<Components>...> {};

		template<typename Callable, typename R, component ... Components>
		struct system_traits<R(Callable::*)(Components...)> : public system_traits_args<std::decay_t<Components>...> {};

		template<typename Callable, typename R, component ... Components>
		struct system_traits<R(Callable::*)(Components...)&> : public system_traits_args<std::decay_t<Components>...> {};

		template<typename Callable, typename R, component ... Components>
		struct system_traits<R(Callable::*)(Components...)&&> : public system_traits_args<std::decay_t<Components>...> {};

		template<typename Callable, typename R, component ... Components>
		struct system_traits<R(Callable::*)(Components...) const> : public system_traits_args<std::decay_t<Components>...> {};

		template<typename Callable, typename R, component ... Components>
		struct system_traits<R(Callable::*)(Components...) const&> : public system_traits_args<std::decay_t<Components>...> {};

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
				if (e.mask.size() > traits.mask.size())
					traits.mask.resize(e.mask.size());
				else if (e.mask.size() < traits.mask.size())
					e.mask.resize(traits.mask.size());

				if (traits.mask.is_proper_subset_of(e.mask) || traits.mask == e.mask)
					traits.apply(db, callback, e);
			}

		private:
			EntityDB* db;
			Callable callback;
			EntityDB::system_traits<Callable> traits{};
		};

		// ---- Member var and func ----
		std::vector<Entity> entities;
		std::vector<Entity::ID> recycleEntities; //재활용될 엔티티들;
		std::vector<std::unique_ptr<ComponentVectorBase>> component_vectors;
		std::vector<std::unique_ptr<SystemBase>> systems;

		template<component C>
		auto getComponentVector() -> ComponentVector<C>* {
			auto group_id = getGroupId<C>();

			if (component_vectors.size() <= group_id) {
				component_vectors.resize(group_id + 1);
				component_vectors[group_id] = std::make_unique<ComponentVector<C>>();
			}

			return static_cast<ComponentVector<C>*>(component_vectors[group_id].get());
		}

		// Component : System에서 이 함수 호출할때 Component는 Component의 포인터 혹은 decay_t가 적용된 순수 타입임.
		// 그래서 포인터로 왔을 경우는 그대로 포인터로 넘겨주고, 그 외엔 레퍼런스로 리턴.
		// 또한 시스템에서 호출했을 경우엔 이미 마스크로 해당 컴포넌트가 있는 엔티티만 파라메터로 오니 null 예외 처리 필요 없음
		// TODO : 사용자가 그냥 호출 했을 경우엔 문제 있으니 사용자용 optional<Component>와 시스템용 예외처리 없는 함수 2개 만들어야 할듯
		template<component Component>
		auto unsafeGetComponent(Entity::ID eid) -> std::conditional_t<std::is_pointer_v<Component>, Component, Component&> {
			auto com_vec = getComponentVector<Component>();
			auto cid = com_vec->findComponentID(eid);

			if constexpr (std::is_pointer_v<Component>) {
				return com_vec->getComponentPtr(*cid);
			}
			else {
				return com_vec->getComponentRef(*cid);
			}
		}
	};
}// namespace se
