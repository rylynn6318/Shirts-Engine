#pragma once

#include <chrono>
#include <memory>
#include <vector>
#include "Entity.h"
#include "component/Component.h"
#include "system/System.h"

namespace se {
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

        template<component Component>
        auto addComponent(EntityID id, Component&& c) {
            //size_t componentID;
            //componentID = getNextComponentNum();
            getComponentVector<Component>()->push_back(c); 

            //TODO : ComponentID 구조체구현이랑 getComponent구현
            //push_back에 넣어놓기
            getComponentVector<Component>()->component_to_entity[id.id] = componentID;
            getComponentVector<Component>()->entity_to_component[componentID] = id.id;
        }

        auto addComponent(EntityID id, component ... cs) {
            (addComponent(id, cs), ...);
        }

        // 보류
        template<typename F>
        auto addSystem(F callable){

        }

    private:
        class ComponentVectorBase {};

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

        std::vector<Entity> entities;
        std::vector<std::unique_ptr<ComponentVectorBase>> component_vectors;

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
    };
} // namespace se
