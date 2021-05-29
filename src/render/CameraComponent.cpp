#include "render/CameraComponent.h"

se::CameraComponent::CameraComponent(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
        Front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV),
        Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch), Right(glm::normalize(glm::cross(Front, WorldUp))),
        Up(glm::normalize(glm::cross(Right, Front))) {
    updateCameraVectors();
}


se::CameraComponent::CameraComponent(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : CameraComponent(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch) {
}

auto se::CameraComponent::getViewMatrix() const -> glm::mat4 {
    return glm::lookAt(Position, Position + Front, Up);
}

auto se::CameraComponent::processKeyboard(CameraMovement direction, std::chrono::duration<int64_t, std::ratio<1, 1000>> deltaTime) -> void {
    float velocity = movementSpeed * deltaTime.count();
    if (direction == CameraMovement::FORWARD)
        Position += Front * velocity;
    if (direction == CameraMovement::BACKWARD)
        Position -= Front * velocity;
    if (direction == CameraMovement::LEFT)
        Position -= Right * velocity;
    if (direction == CameraMovement::RIGHT)
        Position += Right * velocity;
}

auto se::CameraComponent::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) -> void {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

auto se::CameraComponent::processMouseScroll(float yoffset) -> void {
    fov -= (float) yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

auto se::CameraComponent::updateCameraVectors() -> void {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

auto se::CameraComponent::getProjectionMatrix() const -> glm::mat4 {
    return projection;
}

auto se::CameraComponent::activeThisCamera() -> void {
    last_camera = active_camera;
    active_camera = this;
}

auto se::CameraComponent::activeCamera() -> se::CameraComponent & {
    if (active_camera == nullptr)
        active_camera = this;
    return *active_camera;
}

se::CameraComponent::~CameraComponent() {
    if (active_camera == this)
        active_camera = last_camera;
}
