#include "entity/Entity.h"

auto se::Entity::ID::operator==(se::Entity::ID &eid) -> bool {
    return this->index == eid.index && this->recycle_counter == eid.recycle_counter;
}

auto se::Entity::ID::operator==(se::Entity::ID const & eid) const -> bool {
    return this->index == eid.index && this->recycle_counter == eid.recycle_counter;
}

auto se::Entity::ID::operator==(se::Entity &e) -> bool {
    return *this == e.id;
}

bool se::Entity::operator==(se::Entity &e) {
    return this->id == e.id;
}

bool se::Entity::operator==(se::Entity::ID &eid) {
    return this->id == eid;
}