#pragma once
#include <boost/dynamic_bitset.hpp>

namespace se
{
    struct Entity
    {
        struct EntityID {
            std::size_t id;
        
            std::size_t tm;
        };
        EntityID ID;
        boost::dynamic_bitset<> ComponentBitmask;
    };
} // namespace se
