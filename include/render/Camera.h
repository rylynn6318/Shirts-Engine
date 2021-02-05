#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace se
{
	enum class CameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
	};

	//defalut value
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;
	
	class Camera
	{
	public:
		Camera() = delete;
		~Camera();
		//벡터 
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		//스칼라
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

		auto getViewMatrix()->glm::mat4;
		auto processKeyboard(CameraMovement direction, float deltaTime)->void;
		auto processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)->void;
		auto processMouseScroll(float yoffset)->void;

	public:
		// 카메라 속성
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		// 오일러 각
		float Yaw;
		float Pitch;
		// 카메라 옵션
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

	private:
		auto updateCameraVectors()->void;
	};	

}