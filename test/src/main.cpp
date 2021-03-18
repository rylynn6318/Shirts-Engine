#include <iostream>
#include "component/Component.h"
#include "entity/EntityDB.h"

struct Position : public se::Component<Position> {
	int x, y;

	Position(int x, int y) : x(x), y(y) {};
};

struct HealthPoint : public se::Component<HealthPoint> {
	int hp;

	explicit HealthPoint(int hp) : hp(hp) {};
};

struct ManaPoint : public se::Component<ManaPoint> {
    int mp;

    explicit ManaPoint(int mp) : mp(mp) {};
};

struct Name : public se::Component<Name>
{
    std::string name;

    explicit Name(std::string&& name) : name(name) {};
};

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
	std::cout << "test start" << std::endl;
    int nentities = 1000000;
	se::EntityDB db;

//	auto hero = db.createEntity();
//	auto pos = Position{ 3,4 };
//	auto p = HealthPoint{ 99 };
//	db.addComponent(hero, Position{ 3,4 });
//	db.addComponent(hero, HealthPoint{ 99 });
//    db.addComponent(hero, ManaPoint{ 20 });
//	db.addComponent(hero, Name{"Hero"});
//
//    auto monster = db.createEntity();
//    db.addComponent(monster, Position{ 5,5 });
//    db.addComponent(monster, HealthPoint{ 10 }, Name{"Monster"});

    for (size_t i = 0; i < nentities; i++) {
        auto entity = db.createEntity();

        db.addComponent(entity, PositionComponent{});
        db.addComponent(entity, DirectionComponent{});

        if (i % 2 != 0) {
            db.addComponent(entity, ComflabulationComponent{});
        }
    }
    float dt = 1.0 / 60;
//	db.addSystem([](Position* p, HealthPoint* h, Name* n) {
//	    std::cout << "Name : " << n->name << std::endl;
//		std::cout << "\tPos : " << p->x << ", " << p->y << std::endl;
//		std::cout << "\thealth : " << h->hp << std::endl;
//		});
//
//    db.addSystem([](ManaPoint* m, Name* n) {
//        std::cout << "Name : " << n->name << std::endl;
//        std::cout << "\tmana : " << m->mp << std::endl;
//    });
    db.addSystem([dt](PositionComponent* position, DirectionComponent* direction) {
        position->x += direction->x * dt;
        position->y += direction->y * dt;
    });
    db.addSystem([](DirectionComponent* direction) {
        direction->x++;
//        comflab->thingy *= 1.000001f;
//        comflab->mingy = !comflab->mingy;
//        comflab->dingy++;
    });

    auto start = std::chrono::high_resolution_clock::now();
	db.runSystems();
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "test end" << std::endl;
    std::cout << "time(s) : " << elapsed.count() << std::endl;

	return 0;
}
