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

struct ManaPoint : public se::Component<ManaPoint> {
    int mp;

    explicit ManaPoint(int mp) : mp(mp) {};
};

struct Name : public se::Component<Name>
{
    std::string name;

    explicit Name(std::string&& name) : name(name) {};
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
    db.addComponent(hero, ManaPoint{ 20 });
	db.addComponent(hero, Name{"Hero"});

    auto monster = db.createEntity();
    db.addComponent(monster, Position{ 5,5 });
    db.addComponent(monster, HealthPoint{ 10 });
    db.addComponent(monster, Name{"Monster"});

	db.addSystem([](Position* p, HealthPoint* h, Name* n) {
	    std::cout << "Name : " << n->name << std::endl;
		std::cout << "\tPos : " << p->x << ", " << p->y << std::endl;
		std::cout << "\thealth : " << h->hp << std::endl;
		});

    db.addSystem([](ManaPoint* m, Name* n) {
        std::cout << "Name : " << n->name << std::endl;
        std::cout << "\tmana : " << m->mp << std::endl;
    });

	db.runSystems();

	std::cout << "test end" << std::endl;

	return 0;
}
