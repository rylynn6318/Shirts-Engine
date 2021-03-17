#include <iostream>
#include "../../include/component/Component.h"
#include "../../include/entity/EntityDB.h"

struct Position : public se::Component<Position> {
    int x, y;

    Position(int x, int y) : x(x), y(y) {};
};

struct HealthPoint : public se::Component<HealthPoint> {
    int hp;

    explicit HealthPoint(int hp) : hp(hp) {};
};

int main()
{
	std::cout << "test start" << std::endl;

	se::EntityDB db;

	auto hero = db.createEntity();
	db.addComponent(hero, Position{10, 10});
    db.addComponent(hero, HealthPoint{10});
	db.addSystem([](Position* p){
	    std::cout << "hero: (" << p->x << ", " << p->y << ")" << std::endl;
	});
	db.runSystems();

    std::cout << "test end" << std::endl;

	return 0;
}
