#include "component/Component.h"
#include "entity/EntityDB.h"
#include "utility/Dynamic_bitset.h"
#include "boost/dynamic_bitset.hpp"
#include <iostream>


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
	//std::cout << "test start" << std::endl;
 //   auto setup_start = std::chrono::high_resolution_clock::now();
 //   int nentities = 1000000;
	//se::EntityDB db;

 //   for (size_t i = 0; i < nentities; i++) {
 //       auto entity = db.createEntity();

 //       db.addComponent(entity, PositionComponent{}, DirectionComponent{});

 //       if (i % 2 != 0) {
 //           db.addComponent(entity, ComflabulationComponent{});
 //       }
 //   }
 //   float dt = 1.0 / 60;

 //   db.addSystem([dt](PositionComponent& position, DirectionComponent& direction) {
 //       position.x += direction.x * dt;
 //       position.y += direction.y * dt;
 //   });
 //   db.addSystem([](ComflabulationComponent& comflab) {
 //       comflab.thingy *= 1.000001f;
 //       comflab.mingy = !comflab.mingy;
 //       comflab.dingy++;
 //   });

 //   auto setup_finish = std::chrono::high_resolution_clock::now();

 //   auto sys_start = std::chrono::high_resolution_clock::now();
 //   db.runAllSystems();
 //   auto sys_finish = std::chrono::high_resolution_clock::now();

 //   std::chrono::duration<double> setup_time = setup_finish - setup_start;
 //   std::chrono::duration<double> sys_time = sys_finish - sys_start;
 //   std::cout << "test end" << std::endl;
 //   std::cout << "setup time (s) : " << setup_time.count() << std::endl;
 //   std::cout << "update time (s) : " << sys_time.count() << std::endl;

    se::dynamic_bitset bitset;
    bitset.set(2);
    bitset.set(7);
    bitset.set(22);
    bitset.set(40);

    se::dynamic_bitset bitset2;
    bitset2.set(2);
    bitset2.set(7);
    bitset2.set(22);

    std::cout << bitset.to_string() << std::endl;
    std::cout << bitset2.to_string() << std::endl;

    if (bitset2.is_proper_subset_of(bitset2))
        std::cout << "test";

	return 0;
}
