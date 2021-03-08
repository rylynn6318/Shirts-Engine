#include "render/Camera.h"

se::Camera::~Camera()
{

}
 
se::Camera::Camera(
	glm::vec3 position,
	glm::vec3 up,
	float yaw, float pitch) :
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}


se::Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

auto se::Camera::getViewMatrix()->glm::mat4
{
	return glm::lookAt(Position, Position + Front, Up);
}

auto se::Camera::processKeyboard(CameraMovement direction, float deltaTime)->void
{
	float velocity = movementSpeed * deltaTime;
	if (direction == CameraMovement::FORWARD)
		Position += Front * velocity;
	if (direction == CameraMovement::BACKWARD)
		Position -= Front * velocity;
	if (direction == CameraMovement::LEFT)
		Position -= Right * velocity;
	if (direction == CameraMovement::RIGHT)
		Position += Right * velocity;
}

auto se::Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)->void
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	
	updateCameraVectors();
}

auto se::Camera::processMouseScroll(float yoffset)->void
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

auto se::Camera::updateCameraVectors()->void
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}