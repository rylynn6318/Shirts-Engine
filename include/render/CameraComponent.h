#pragma once

#include <chrono>
#include "component/TransformComponent.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace se {
    enum class CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
    };

    //defalut value
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 0.01f;
    const float SENSITIVITY = 0.1f;
    const float FOV = 45.0f;

    class CameraComponent : public Component<CameraComponent> {
    public:
        ~CameraComponent();

        explicit CameraComponent(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                                 float yaw = YAW, float pitch = PITCH);

        CameraComponent(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        [[nodiscard]]
        auto getViewMatrix() const -> glm::mat4;

        [[nodiscard]]
        auto getProjectionMatrix() const -> glm::mat4;

        auto processKeyboard(CameraMovement direction, std::chrono::milliseconds deltaTime) -> void;

        auto processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) -> void;

        auto processMouseScroll(float yoffset) -> void;

        auto activeThisCamera() -> void;

        auto activeCamera() -> CameraComponent&;

    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        float Yaw;
        float Pitch;

        float movementSpeed;
        float mouseSensitivity;
        float fov;

    private:
        // Todo : 전역적인 처리에 active_camera 사용하게 변경, position 컴포넌트 요구하는것 어떻게 구현할지 생각
        static CameraComponent *active_camera;
        static CameraComponent *last_camera;

        glm::mat4 projection = glm::perspective(glm::radians(FOV), (float) 1200 / (float) 800, 0.1f, 1000.0f);
        // TransformComponent& postion;

        auto updateCameraVectors() -> void;
    };

}