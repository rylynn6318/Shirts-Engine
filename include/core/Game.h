#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader/Shader.h"
#include "render/VertexArray.h"
#include "render/Texture.h"
#include "render/Camera.h"
#include "render/Model.h"

namespace se
{
	//TODO : 메모리 해제는 나중에 구조가 정해지고 나서 바꿔야함, 텍스쳐같은건 나중에 컴포넌트로 빼야함
	//TODO : 카메라 분리 생각해야함
	class Game
	{
	public:
		Game();
		~Game();

		auto init()->bool;
		auto run()->void;
		auto terminate()->void;

	public:


	private:
		GLFWwindow* window;
		Shader shader;
		VertexArray* vao;
		Texture texture;
		Model* model;

		void processInput(GLFWwindow* window);
		void load(); //test for load shader, vertex
		bool isRunning = true;
	};
}