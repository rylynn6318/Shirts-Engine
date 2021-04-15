#pragma once

#include "Component.h"
#include "glm/glm.hpp"

namespace se {
    struct TransformComponent : Component<TransformComponent> {
        glm::vec3 position;
        glm::mat4 rotation;
        glm::vec3 scale;

        TransformComponent(glm::vec3 position, glm::mat4 rotation, float scale)
                : position(position), rotation(rotation), scale(scale) {}
    };
}