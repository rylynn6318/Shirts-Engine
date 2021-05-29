#include "core/Game.h"

auto se::Game::init() -> bool {

    return true;
}

auto se::Game::run(se::EntityDB &db) -> void {
    auto previous = std::chrono::system_clock::now();
    auto lag = 0ms;
    while (isRunning) {
        auto current = std::chrono::system_clock::now();
        auto elasped = current - previous;
        previous = current;
        lag += std::chrono::duration_cast<std::chrono::milliseconds>(elasped);

        // processInput(window);

        for (int i = 0, max_loop_time = 5; lag >= MS_PER_UPDATE && i < max_loop_time; ++i) {
            db.runAllSystems();
            lag -= MS_PER_UPDATE;
        }

        db.render();
    }
}

auto se::Game::terminate() -> void {
}