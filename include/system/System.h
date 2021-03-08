#ifndef SHIRTS_ENGINE_SYSTEM_H
#define SHIRTS_ENGINE_SYSTEM_H
#pragma once

#include <concepts>
#include "component/Component.h"

namespace se {
    struct SystemBase {
        virtual auto update() -> void = 0;
    };

    template<typename Callable, component... ArgsOfCallable>
    class System : SystemBase {
    public:
        System(Callable callable){

        }

        auto update() -> void override {
            system();
        }

        auto tick(ArgsOfCallable... args) {
            system(args...);
        }

    private:
        Callable system;
    };
}

#endif //SHIRTS_ENGINE_SYSTEM_H
