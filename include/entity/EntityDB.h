#pragma once

#include <chrono>
#include <concepts>
#include <execution>
#include <memory>
#include <vector>
#include <boost/dynamic_bitset.hpp>
#include "component/Component.h"
#include "system/System.h"

namespace se {
    class Entity;

    // EntityDB에서 만든 EntityID를 다른 EntityDB 인스턴스에서 쓰는 경우는 없겠지?
    // 거기에 대한 대비책도 있어야 하나??
    class EntityDB final {
    public:
        struct EntityID final {
            std::size_t id;
        private:
            std::size_t index;
        };

        // 실제 id != 해당 ComponentVector에서의 인덱스
        // 컴포넌트 제거를 가장 끝에 있는 컴포넌트와 삭제할 컴포넌트의 위치를 바꾸는 것으로 구현할 생각임
        // 그리고 나서 last_component를 1 줄이는 거지
        // 정렬될 필요는 없고 메모리 재할당을 최소화 하기 위함
        // 그에 따라 id에 표기를 위한 id에 더해 실제 인덱스를 담을 변수를 넣어야 할지 생각해야 함
        // EntityID 또한 같음
        // 혹은 version 정보를 넣어서 재활용을 계속 하던가
        // 리스트면 위 방법이 괜찮겠는데 벡터니까 안쓰는 오브젝트용 벡터를 하나 더 두는게 나을 듯
        struct ComponentID final {
            std::size_t id;
        private:
            std::size_t index;
        };

        auto createEntity() {

        }

        auto addComponent(EntityID id, component auto c) {
            getComponentVector<typeof(c)>()->push_back(c);
        }

        auto addComponent(EntityID id, component auto ... cs) {
            (addComponent(id, cs), ...);
        }


        // TODO : Callable 타입 제한 둬야함
        template<typename Callable>
        auto addSystem(Callable callable){
            systems.push_back(std::make_unique<System>(callable));
        }

    private:
        // ---- Utils ----
        // TODO : 우측값 받아서 값 리턴하게 수정?
        template<component C>
        static auto addMask(boost::dynamic_bitset<>& mask) {
            mask[groupId<C>()] = 1;
        }

        template<component C, component ... Cs>
        static auto addMask(boost::dynamic_bitset<>& mask) {
            addMask<C>(mask);
            addMask<Cs ...>(mask);
        }


        // ---- Component Vector ----
        struct ComponentVectorBase {};

        template<component Component>
        class ComponentVector final : ComponentVectorBase {
        public:
            auto push_back(Component c) {

            }

        private:
            std::vector<Component> components;
            // index(key) : ComponentID
            // value : EntityID
            std::vector<EntityID> component_to_entity;
            // index(key) : EntityID
            // value : ComponentID
            std::vector<ComponentID> entity_to_component;
            // map 쓰는 것 보다 이렇게 index를 키로 쓰는 vector 두개 쓰는게 성능상 나을 것 같음.
        };

        // ---- System Traits ----
        template<component ... ArgsOfCallable>
        struct system_traits_args {
            static boost::dynamic_bitset<> const mask;

            template<typename Callable>
            static auto apply(Callable func, EntityID id) {
                return std::forward<Callable>(func)(getComponent<ArgsOfCallable>(id)...);
            }
        private:
            struct static_bitset_constructor {
                static_bitset_constructor(){
                    addMask<ArgsOfCallable ...>(system_traits_args::mask);
                }
            };

            static static_bitset_constructor c;
        };

        template<typename R, component ... Components>
        struct system_traits<R (*)(Components...)> : system_traits_args<std::decay_t<Components>...> {};

        template<typename R, component ... Components>
        struct system_traits<R (&)(Components...)> : system_traits_args<std::decay_t<Components>...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...)> : system_traits_args<std::decay_t<Components>...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...) &> : system_traits_args<std::decay_t<Components>...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...) &&> : system_traits_args<std::decay_t<Components>...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...) const> : system_traits_args<std::decay_t<Components>...> {};

        template<typename Callable, typename R, component ... Components>
        struct system_traits<R (Callable::*)(Components...) const &> : system_traits_args<std::decay_t<Components>...> {};

        // ---- System ----
        struct SystemBase {
            virtual auto update(EntityID) = 0;
        };

        template<typename Callable>
        class System : SystemBase {
        public:
            explicit System(Callable callable) : callback(callable) {};

            auto update(EntityID id) override {
                Signiture::apply(callback, id);
            }

        private:
            using Signiture = system_traits<decltype(&std::decay_t<Callable>::operator())>;

            Callable callback;
        };

        // ---- Member var and func ----
        std::vector<Entity> entities;
        std::vector<std::unique_ptr<ComponentVectorBase>> component_vectors;
        std::vector<std::unique_ptr<SystemBase>> systems;

        template<component C>
        auto getComponentVector() -> ComponentVector<C>* {
            auto group_id = C::groupId();

            if (component_vectors.size() < group_id) {
                component_vectors.resize(group_id + 1);
            }

            if (!component_vectors[group_id]) {
                component_vectors[group_id] = std::make_unique<ComponentVector<C>>();
            }

            return static_cast<ComponentVector<C>*>(component_vectors[group_id].get());
        }

        template<component C>
        auto getComponent(EntityID id){

        }

        auto runSystems() {
            for(auto s : systems) {
                std::for_each(std::execution::par_unseq, entities.begin(), entities.end(), [](Entity e){
                    *s->update(e.id);
                });
            }
        }
    };
} // namespace se
