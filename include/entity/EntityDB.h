#pragma once

#include <chrono>
#include <memory>
#include <vector>
#include <utility>
#include <algorithm>
#include <ranges>
#include <range/v3/all.hpp>
#include "component/Component.h"
#include "system/System.h"

namespace se {

	struct Entity;
	// EntityDB에서 만든 EntityID를 다른 EntityDB 인스턴스에서 쓰는 경우는 없겠지?
	// 거기에 대한 대비책도 있어야 하나??
	class EntityDB final {
	public:
		struct EntityID final {
			std::size_t id;
			friend class EntityDB;
		private:
			std::size_t tm;
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

		static std::size_t eid;
		static auto nextEid()
		{
			return eid++;
		}

		auto createEntity() {
			Entity entity;
			if (!recycleEntities.empty())
			{
				entity.ID.id = recycleEntities.back().id;
				entity.ID.tm = ++(recycleEntities.back().tm);
				recycleEntities.pop_back();
			}
			else
			{
				entity.ID.id = nextEid();
			}
			entity.ComponentBitmask.clear();
			entities.push_back(entity);
			return entity.ID;
		}

		template<component Component>
		auto addComponent(EntityID eid, Component&& c) {
			using component_type = std::decay_t<Component>;
			getComponentVector<Component>()->push_back(eid, std::forward<Component>(c));
			auto entity = std::ranges::find(entities, eid);
			if (entity != std::ranges::end(entities))
				entity.ComponentBitmask[groupId<component_type>()] = 1;
		}
		
		template<component... Component>
		auto addComponent(EntityID eID, Component... cs) {
			(addComponent(eID, cs), ...);
		}

		template <component Component>
		auto removeComponent(EntityID eid) {
			using component_type = std::decay_t<Component>;
			auto entity = std::ranges::find(entities, eid);
			if (entity != std::ranges::end(entities))
				entity.ComponentBitmask[groupId<component_type>()] = 0;

			//map에서 삭제?
		}

		template<component Component>
		auto getComponent(EntityID eid)->std::optional<Component&> {
			auto cid = getComponentVector()->findComponentID(eid);
			if (cid.value())
				return getComponentVector()->components[cid.id];
			else
				return std::nullopt;
		}

		// 보류
		template<typename F>
		auto addSystem(F callable) {

		}

	private:
		class ComponentVectorBase {};

		template<component Component>
		class ComponentVector final : ComponentVectorBase {
		public:
			auto push_back(EntityID eID, Component&& c)
			{
				//맨첨에 gcc랑 msvc 둘다 capacity=0
				components.reserve(components.size() + 1);
				components.push_back(c);

				auto cID = components.size();

				EntityComponentMap.push_back({ eID.id, cID });
			}

			auto findEntityID(std::size_t cid) -> std::optional<EntityID>
			{
				auto eid = std::find_if(EntityComponentMap.cbegin(), EntityComponentMap.cend(),
					[&cid](std::pair<EntityID, std::size_t> pair) {
						return pair.second == cid;
					}
				);
				if (eid != EntityComponentMap.cend())
					return (*eid).first;
				else
					return std::nullopt;
			}
			 
			auto findComponentID(EntityID eid) -> std::optional<std::size_t>
			{
				auto cid = std::find_if(EntityComponentMap.cbegin(), EntityComponentMap.cend(),
					[&eid](std::pair<EntityID, std::size_t> pair) {
						return pair.first == eid;
					}
				);
				if (cid != EntityComponentMap.cend())
					return (*cid).second;
				else
					return std::nullopt;
			}

		private:
			std::vector<Component> components;
			std::vector<std::pair<EntityID, std::size_t>> EntityComponentMap;
			//std::vector<EntityID> component_to_entity;
			//std::vector<ComponentID> entity_to_component;
		};

		std::vector<Entity> entities;
		std::vector<EntityID> recycleEntities; //재활용될 엔티티들;

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
