#pragma once

#include <boost/dynamic_bitset.hpp>

namespace se {
    struct Entity {
        struct EntityID {
            std::size_t id;

            auto operator==(EntityID &) -> bool;
            auto operator==(Entity &) -> bool;

        private:
            friend class EntityDB;
            friend class Entity;

            std::size_t index;

            EntityID() = default;
        };

        EntityID id{};

        auto operator==(Entity &) -> bool;
        auto operator==(EntityID &) -> bool;

    private:
        friend class EntityDB;

        boost::dynamic_bitset<> mask;

        Entity() = default;
    };
} // namespace se
