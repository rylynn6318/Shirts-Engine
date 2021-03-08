#pragma once

#include <concepts>

namespace se {
    // 아래 함수 groupId() 외에서 사용하면 안됨
    struct GroupIdCounter {
        static std::size_t counter;
    };

    template<typename ComponentType>
    struct Component {
        // EntityDB의 component_vectors 벡터의 인덱스(키)로 사용됨.
        static inline auto groupId() {
            static std::size_t const groupId = GroupIdCounter::counter++;
            return groupId;
        }
    };

    template<typename T>
    concept component = std::derived_from<T, Component<T>>;

    template<component C>
    static auto groupId() {
        return std::remove_const<C>::type::groupId();
    }
} // namespace se
