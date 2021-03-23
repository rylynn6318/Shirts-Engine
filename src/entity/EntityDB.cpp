#include "entity/EntityDB.h"

auto se::EntityDB::runAllSystems() -> void {
    for (auto &s : systems) {
        runSystem(s);
    }
}

auto se::EntityDB::runSystem(std::unique_ptr<SystemBase> &system) -> void {
    std::for_each(std::execution::par_unseq, entities.begin(), entities.end(), [&system](Entity &e) {
        system->update(e);
    });
//        for(auto& e : entities) {
//            s->update(e);
//        }
}
