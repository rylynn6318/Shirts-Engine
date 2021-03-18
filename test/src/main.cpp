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

struct test : public se::Component<test>
{

};

int main()
{
	std::cout << "test start" << std::endl;

	se::EntityDB db;

	auto hero = db.createEntity();
	auto pos = Position{ 3,4 };
	auto p = HealthPoint{ 99 };
	db.addComponent(hero, Position{ 3,4 });
	db.addComponent(hero, HealthPoint{ 99 });
	db.addComponent(hero, test{});
	db.addSystem([](Position* p, HealthPoint* h) {
		std::cout << "Pos : " << p->x << ", " << p->y << std::endl;
		std::cout << "health : " << h->hp << std::endl;
		});
	db.runSystems();

	std::cout << "test end" << std::endl;

	return 0;
}
