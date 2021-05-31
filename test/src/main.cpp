#include <iostream>
#include <component/TextureComponent.h>
#include <random>
#include "component/Component.h"
#include "entity/EntityDB.h"
#include "render/Renderer.h"
#include "core/Game.h"

struct PositionComponent : public se::Component<PositionComponent> {
    float x = 0.0f;
    float y = 0.0f;
};

struct DirectionComponent : public se::Component<DirectionComponent> {
    float x = 0.0f;
    float y = 0.0f;
};

struct ComflabulationComponent : public se::Component<ComflabulationComponent> {
    float thingy = 0.0;
    int dingy = 0;
    bool mingy = false;
    std::string stringy;
};

int main() {
//	std::cout << "test start" << std::endl;
//    auto setup_start = std::chrono::high_resolution_clock::now();
//    int nentities = 20'000'000;
//	se::EntityDB db;
//
//    for (size_t i = 0; i < nentities; i++) {
//        auto entity = db.createEntity();
//
//        db.addComponent(entity, PositionComponent{}, DirectionComponent{});
//
//        if (i % 2 != 0) {
//            db.addComponent(entity, ComflabulationComponent{});
//        }
//    }
//    float dt = 1.0 / 60;
//
//    db.addSystem([dt](PositionComponent& position, DirectionComponent& direction) {
//        position.x += direction.x * dt;
//        position.y += direction.y * dt;
//    });
//    db.addSystem([](ComflabulationComponent& comflab) {
//        comflab.thingy *= 1.000001f;
//        comflab.mingy = !comflab.mingy;
//        comflab.dingy++;
//    });
//
//    auto setup_finish = std::chrono::high_resolution_clock::now();
//
//    auto sys_start = std::chrono::high_resolution_clock::now();
//    db.runAllSystems();
//    auto sys_finish = std::chrono::high_resolution_clock::now();
//
//    std::chrono::duration<double> setup_time = setup_finish - setup_start;
//    std::chrono::duration<double> sys_time = sys_finish - sys_start;
//    std::cout << "test end" << std::endl;
//    std::cout << "setup time (s) : " << setup_time.count() << std::endl;
//    std::cout << "update time (s) : " << sys_time.count() << std::endl;

    se::Renderer renderer;
    renderer.init(1200, 800);

    se::StaticModel staticModel;
    staticModel.loadModel("../resource/model/GrassBlock/Grass_Block.obj");
    // staticModel.loadModel("../resource/model/backpack/backpack.obj");

    auto pepe_texture = se::loadTexture("../resource/image/pepe_thumsup.png");

    se::EntityDB db;
    for (int i = 0; i < 10; ++i) {
        auto e = db.createEntity();
        db.addComponent(e, se::TransformComponent{{(float)(i * 10), 0.0f, 0.0f}, {}, {}}, se::TextureComponent{pepe_texture});
    }

    db.addSystem([](se::TransformComponent &trans) {
        std::cout << trans.position.x << std::endl;
    });

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(-10, 10);
    db.addSystem([&dis, &gen](se::TransformComponent &trans) {
        trans.position.y += dis(gen)/100.0;
    });

    renderer.drawTextures(db);

    se::Game::Instance().init();
    se::Game::Instance().run(db, renderer);

    return 0;
}
