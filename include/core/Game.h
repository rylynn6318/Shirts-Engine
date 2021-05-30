#pragma once

#include <chrono>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "entity/EntityDB.h"
#include "render/Renderer.h"

using namespace std::chrono_literals;

namespace se {
    //TODO : 다 분리해야함
    class Game {
    public:
        static Game &Instance() {
            static Game instance;
            return instance;
        }

        Game(const Game &) = delete;

        Game(Game &&) = delete;

        Game &operator=(const Game &) = delete;

        Game &operator=(Game &&) = delete;

        auto init() -> bool;

        auto run(EntityDB &db, Renderer&) -> void;

        auto terminate() -> void;

        [[nodiscard]]
        static auto deltaTime() {
            return delta_time;
        }

        constexpr static auto MS_PER_UPDATE = 16ms;

    private:
        Game() = default;

        ~Game() = default;

        static std::chrono::milliseconds delta_time;

        bool isRunning = true;
    };
}