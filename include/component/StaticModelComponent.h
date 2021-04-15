#pragma once

#ifndef SHIRTS_ENGINE_STATICMODELCOMPONENT_H
#define SHIRTS_ENGINE_STATICMODELCOMPONENT_H

#include "Component.h"
#include "render/StaticModel.h"

namespace se {
    struct StaticModelComponent : Component<StaticModelComponent> {
        StaticModel *model;

        explicit StaticModelComponent(StaticModel *m) : model(m) {}
    };
}

#endif //SHIRTS_ENGINE_STATICMODELCOMPONENT_H
