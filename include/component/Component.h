#pragma once

#include <concepts>
#include <type_traits>

namespace se {
    // 아래 함수 getGroupId() 외에서 사용하면 안됨
    struct GroupIdCounter {
        static std::size_t id_counter;
    };

    template<typename ComponentType>
    struct Component {
        // EntityDB의 component_vectors 벡터의 인덱스(키)로 사용됨.
        static inline auto groupId() {
            static std::size_t const groupId = GroupIdCounter::id_counter++;
            return groupId;
        }
    };

    template<typename T>
    concept component = std::derived_from<std::remove_pointer_t<T>, Component<std::remove_pointer_t<T>>> ||
                        std::derived_from<std::decay_t<T>, Component<std::decay_t<T>>>;

    template<component C>
    static auto getGroupId() {
        if constexpr (std::is_pointer_v<C>)
            return std::remove_pointer_t<C>::groupId();
        else
            return Component<std::decay_t<C>>::groupId();
    }

} // namespace se
