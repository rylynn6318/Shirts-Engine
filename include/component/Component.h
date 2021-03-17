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
    struct NotComponent{
        static_assert(true, "컴포넌트가 아닌 객체 감지");
    };

    template<typename T>
    concept component = std::derived_from<std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<T>>>, Component<std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<T>>>>>;

    template<typename Callable, typename ... T>
    concept tmp_concept_system = std::invocable<Callable, std::enable_if_t<component<T>, T>...>;

    template<component C>
    static auto getGroupId() {
        return Component<std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<C>>>>::groupId();
    }

} // namespace se
