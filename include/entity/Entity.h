#pragma once

#include <boost/dynamic_bitset.hpp>
#include "EntityDB.h"

namespace se
{
    // 엔티티가 특정 컴포넌트를 가지는지 확인할 가장 효율적인 방법은 bitset을 쓰는 것
    // bitset을 어디에 저장할지 정해야 함
    struct Entity
    {
        EntityDB::EntityID id;
        boost::dynamic_bitset<> mask;
        /* data */
    };
} // namespace se
