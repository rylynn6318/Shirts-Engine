#include <iostream>
#include "component/Component.h"
#include "entity/EntityDB.h"
#include "render/Renderer.h"

struct PositionComponent : public se::Component<PositionComponent> {
    float x = 0.0f;
    float y = 0.0f;
};

struct DirectionComponent : public se::Component<DirectionComponent>{
    float x = 0.0f;
    float y = 0.0f;
};

struct ComflabulationComponent : public se::Component<ComflabulationComponent>{
    float thingy = 0.0;
    int dingy = 0;
    bool mingy = false;
    std::string stringy;
};

int main()
{
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
    renderer.init(1200,800);
    while (true)
    {
        renderer.draw();
    }

	return 0;
}
