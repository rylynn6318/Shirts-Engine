#include "entity/Entity.h"

auto se::Entity::EntityID::operator==(se::Entity::EntityID &eid) -> bool {
    return this->id == eid.id && this->index == eid.index;
}

auto se::Entity::EntityID::operator==(se::Entity &e) -> bool {
    return *this == e.id;
}

bool se::Entity::operator==(se::Entity &e) {
    return this->id == e.id;
}

bool se::Entity::operator==(se::Entity::EntityID &eid) {
    return this->id == eid;
}