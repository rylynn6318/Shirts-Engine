#include <render/CameraComponent.h>
#include "core/Game.h"

//static std::chrono::milliseconds delta_time{};

auto se::Game::init() -> bool {

    return true;
}

auto se::Game::run(se::EntityDB &db, Renderer& r) -> void {
    auto previous = std::chrono::system_clock::now();
    auto lag = 0ms;
    while (!glfwWindowShouldClose(r.window)) {
        auto current = std::chrono::system_clock::now();
        auto elasped = current - previous;
        previous = current;
        lag += std::chrono::duration_cast<std::chrono::milliseconds>(elasped);

        processInput(r.window);

        for (int i = 0, max_loop_time = 5; lag >= MS_PER_UPDATE && i < max_loop_time; ++i) {
            db.runAllSystems();
            lag -= MS_PER_UPDATE;
        }

        // todo : remove this
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        r.drawSkybox();
        //r.drawTextures();

        db.render();

        glfwSwapBuffers(r.window);
        glfwPollEvents();
    }
}

auto se::Game::terminate() -> void {
}

//auto se::Game::deltaTime() -> std::chrono::milliseconds {
//    return delta_time;
//}
