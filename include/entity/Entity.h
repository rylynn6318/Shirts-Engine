#pragma once
#include "EntityDB.h"
#include <boost/dynamic_bitset.hpp>

namespace se
{
    struct Entity
    {
        EntityDB::EntityID ID;
        boost::dynamic_bitset<> ComponentBitmask;

        auto operator==(const EntityDB::EntityID& eid) {
            return ID.id == eid.id;
        }
    };
} // namespace se
