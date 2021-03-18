#pragma once

#include <boost/dynamic_bitset.hpp>

namespace se {
    struct Entity {
        struct EntityID {
            friend class EntityDB;
            friend class Entity;
            friend struct std::hash<EntityID>;

            std::size_t id;

            auto operator==(EntityID &) -> bool;
            auto operator==(EntityID const &) const -> bool;
            auto operator==(Entity &) -> bool;
        private:

            std::size_t index;

            EntityID() = default;
        };

        EntityID id{};

        auto operator==(Entity &) -> bool;
        auto operator==(EntityID &) -> bool;

    private:
        friend class EntityDB;

        boost::dynamic_bitset<> mask{};

        Entity() = default;
    };
} // namespace se

namespace std {
    template<>
    struct hash<se::Entity::EntityID> {
        size_t operator()(const se::Entity::EntityID& eid) const {
            hash<size_t> hash_func;

            return (hash_func(eid.id)) ^ eid.index;
        }
    };
}
