#pragma once

#include <boost/dynamic_bitset.hpp>
#include "utility/Dynamic_bitset.h"

namespace se {
    struct Entity final {
        struct ID final {
            friend class EntityDB;
            friend class Entity;
            friend struct std::hash<ID>;

            auto operator==(ID &) -> bool;
            auto operator==(ID const &) const -> bool;
            auto operator==(Entity &) -> bool;

            ID& operator=(ID const &) = delete;
            ID& operator=(ID &&) = delete;
            ID(ID const &) = default;
            ID(ID&&) = default;
        private:
            std::size_t recycle_counter = 0;
            std::size_t const index;

            explicit ID(std::size_t index) : index(index) {};
        };

        friend class EntityDB;

        auto operator==(Entity &) -> bool;
        auto operator==(ID &) -> bool;
        Entity(const Entity&) = default;
        Entity& operator=(const Entity&) = default;

    private:
        ID id;
        //boost::dynamic_bitset<> mask{};
        dynamic_bitset mask{};

        explicit Entity(ID id) : id(id) {};
    };
} // namespace se

namespace std {
    template<>
    struct hash<se::Entity::ID> {
        auto operator()(se::Entity::ID const & eid) const {
            hash<size_t> hash_func;

            return (hash_func(eid.index)); // ^ eid.recycle_counter;
        }
    };
}
