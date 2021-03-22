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
        // 사용자가 Entity::ID를 얻을 수 있는 유일한 함수
        auto createEntity() -> Entity::ID {
            if (recycleEntities.empty()) {
                entities.emplace_back(Entity{Entity::ID{entities.size()}});
                return entities.back().id;
            } else {
                auto entity_to_recycle = recycleEntities.back();
                recycleEntities.pop_back();

                return entity_to_recycle;
            }
        }

        // TODO : 파괴된 엔티티를 다시 쓰지 못하도록 일단 move를 통해 받게 했는데 더 좋은 아이디어가 있을까
        auto destroyEntity(Entity::ID&& eid) -> void {
            recycleEntities.push_back(eid);
            auto e = entities[eid.index];

            auto i = e.mask.find_first();
            while (i != boost::dynamic_bitset<>::npos) {
                component_vectors[i].get()->remove(eid);
                i = e.mask.find_next(i);
            }
            e.mask.clear();
            e.id.recycle_counter++;
        }

        template<component Component>
        auto addComponent(Entity::ID eid, Component &&c) {
            getComponentVector<Component>()->pushBack(eid, std::forward<Component>(c));
            addMask<Component>(entities[eid.index].mask);
        }

        template<component Component, component... Components>
        auto addComponent(Entity::ID eid, Component &&c, Components &&... cs) {
            addComponent(eid, std::forward<Component>(c));
            addComponent(eid, std::forward<Components>(cs)...);
        }

        template<component Component>
        auto removeComponent(Entity::ID eid) {
            component_vectors[getGroupId<Component>()].get()->remove(eid);
            removeMask<Component>(entities[eid.index].mask);
        }

        template<typename Callable>
        auto addSystem(Callable callable) {
            systems.push_back(std::make_unique<System < Callable>>(callable, this));
        }

        // TODO : 시스템 한번만 즉시 실행, 혹은 특정 시점(init() 등)에 실행할 수 있는 함수 하나 필요함
        template<typename Callable>
        auto visit(Callable callable) {
            auto system = System(callable, this);
            std::for_each(std::execution::par_unseq, entities.begin(), entities.end(), [&system](Entity &e) {
                system->update(e);
            });
        }

        auto runAllSystems() -> void;

    private:
        // ---- Utils ----
        template<component ... Cs>
        static auto addMask(boost::dynamic_bitset<> &mask) -> typename std::enable_if<(sizeof...(Cs) == 0)>::type {}

        template<component C, component ... Cs>
        static auto addMask(boost::dynamic_bitset<> &mask) -> void {
            auto i = getGroupId<C>();
            if (mask.size() <= i)
                mask.resize(i + 1);
            mask.set(i);
            addMask<Cs ...>(mask);
        }

        template<component C>
        static auto removeMask(boost::dynamic_bitset<> &mask) -> void {
            auto i = getGroupId<C>();
            if (mask.size() <= i)
                mask.resize(i + 1);
            mask.reset(i);
        }

        // ---- Inner Class, Struct ----

        // -------- Component Vector --------
        struct ComponentVectorBase {
            virtual ~ComponentVectorBase() = default;

            // destroyEntity() 때문에 필요함
            virtual auto remove(Entity::ID eid) -> void = 0;
        };

        // ComponentVector 중
        // private : 진짜 내부적으로만 사용.
        // public : EntityDB 에서만 사용. 사용자에겐 노출되지 않음.
        template<component Component>
        class ComponentVector final : public ComponentVectorBase {
            struct ComponentID final {
                std::size_t id{};
                std::size_t recycleCounter = 0;

                ComponentID() = default;

                explicit ComponentID(std::size_t id) : id(id) {};

                auto operator==(ComponentID & cid) -> bool {
                    return this->id == cid.id;
                }
            };

        public:
            using PureComponent = std::conditional_t<std::is_pointer_v<Component>, std::remove_pointer_t<Component>, std::decay_t<Component>>;

            // TODO : 컴포넌트 재활용 로직 추가해야함
            auto pushBack(Entity::ID eid, Component &&c) {
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

            auto findComponentID(Entity::ID eid) -> std::optional<ComponentID> {
                auto cid = eid_to_cid_map.find(eid);
                if (cid != eid_to_cid_map.end())
                    return cid->second;
                else
                    return std::nullopt;
            }

            // EntityDB::unsafeGetComponent()에서 사용함
            // 해당 eid에 대해 마스크 등으로 검증이 되었을 경우에만 사용할 것
            auto unsafeGetComponentPtr(Entity::ID eid) -> PureComponent * {
                return &components[unsafeFindComponentID(eid).id];
            }

            // EntityDB::unsafeGetComponent()에서 사용함
            // 해당 eid에 대해 마스크 등으로 검증이 되었을 경우에만 사용할 것
            auto unsafeGetComponentRef(Entity::ID eid) -> PureComponent & {
                return components[unsafeFindComponentID(eid).id];
            }

        private:
            std::vector<PureComponent> components;
            std::unordered_map<Entity::ID, ComponentID> eid_to_cid_map;
            std::vector<Entity::ID> cid_to_eid_map;
            std::vector<ComponentID> recycleComponents;

            // mask 확인 해서 이미 존재가 확실한 경우에만 사용할 것
            auto unsafeFindComponentID(Entity::ID eid) -> ComponentID  {
                return eid_to_cid_map[eid];
            }

            // TODO : 삭제된 엔티티의 컴포넌트, 혹은 존재하지 않는 cid를 파라메터로 넘긴 경우의 예외처리 필요함
            auto unsafeFindEntityID(ComponentID cid) -> Entity::ID {
                return cid_to_eid_map[cid.id];
            }
        };

        // -------- System Traits --------

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
            auto apply(EntityDB *db, Callable func, Entity &e) {
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
        struct system_traits<R (*)(Components...)>
                : public system_traits_args<std::decay_t<Components>...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...)>
                : public system_traits_args<std::decay_t<Components>...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...) &>
                : public system_traits_args<std::decay_t<Components>...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...) &&>
                : public system_traits_args<std::decay_t<Components>...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...) const>
                : public system_traits_args<std::decay_t<Components>...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...) const &>
                : public system_traits_args<std::decay_t<Components>...> {};

        // -------- System --------
        struct SystemBase {
            virtual ~SystemBase() = default;

            virtual auto update(Entity &) -> void = 0;
        };

        template<typename Callable>
        class System : public SystemBase {
        public:
            System(Callable callable, EntityDB *db) : callback(callable), db(db) {};

            auto update(Entity &e) -> void override {
                if (e.mask.size() > traits.mask.size())
                    traits.mask.resize(e.mask.size());
                else if (e.mask.size() < traits.mask.size())
                    e.mask.resize(traits.mask.size());

                if (traits.mask.is_proper_subset_of(e.mask) || traits.mask == e.mask)
                    traits.apply(db, callback, e);
            }

        private:
            EntityDB *db;
            Callable callback;
            EntityDB::system_traits<Callable> traits{};
        };

        // ---- Member var and func ----
        std::vector<Entity> entities;
        std::vector<Entity::ID> recycleEntities; //재활용될 엔티티들;
        std::vector<std::unique_ptr<ComponentVectorBase>> component_vectors;
        std::vector<std::unique_ptr<SystemBase>> systems;

        template<component C>
        auto getComponentVector() -> ComponentVector<C> * {
            auto group_id = getGroupId<C>();

            if (component_vectors.size() <= group_id) {
                component_vectors.resize(group_id + 1);
                component_vectors[group_id] = std::make_unique<ComponentVector<C>>();
            }

            return static_cast<ComponentVector<C> *>(component_vectors[group_id].get());
        }

        // C : System에서 이 함수 호출할때 Component는 Component의 포인터 혹은 decay_t가 적용된 순수 타입임.
        // 그래서 포인터로 왔을 경우는 그대로 포인터로 넘겨주고, 그 외엔 레퍼런스로 리턴.
        // 또한 시스템에서 호출했을 경우엔 이미 마스크로 해당 컴포넌트가 있는 엔티티만 파라메터로 오니 null 예외 처리 필요 없음
        // TODO : 사용자가 그냥 호출 했을 경우엔 문제 있으니 사용자용 optional<Component>와 시스템용 예외처리 없는 함수 2개 만들어야 할듯
        template<component C>
        auto unsafeGetComponent(Entity::ID eid) -> std::conditional_t<std::is_pointer_v<C>, C, C &> {
            auto com_vec = getComponentVector<C>();

            if constexpr (std::is_pointer_v<C>) {
                return com_vec->unsafeGetComponentPtr(eid);
            } else {
                return com_vec->unsafeGetComponentRef(eid);
            }
        }

        auto runSystem(std::unique_ptr<SystemBase>& system) -> void;
    };
} // namespace se
