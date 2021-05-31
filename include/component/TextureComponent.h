//
// Created by sm on 21. 5. 31..
//

#ifndef SHIRTS_ENGINE_TEXTURECOMPONENT_H
#define SHIRTS_ENGINE_TEXTURECOMPONENT_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Component.h"

namespace se {
    struct TextureComponent : Component<TextureComponent> {
        GLuint vao{};
        GLuint texture;

        explicit TextureComponent(GLuint texture) : texture(texture) {}
    };
}

#endif //SHIRTS_ENGINE_TEXTURECOMPONENT_H
