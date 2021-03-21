#include "entity/EntityDB.h"

auto se::EntityDB::runSystems() -> void {
    for(auto& s : systems) {
        std::for_each(std::execution::par_unseq, entities.begin(), entities.end(), [&s](Entity& e){
            s->update(e);
        });
//        for(auto& e : entities) {
//            s->update(e);
//        }
    }
}