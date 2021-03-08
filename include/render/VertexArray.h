#pragma once
#include "glad/glad.h"

namespace se
{
	class VertexArray
	{
	public:
		VertexArray() = delete;
		~VertexArray();
		VertexArray(const float* vertices, const unsigned int* indices);
		VertexArray(const float* vertices);
	
	
	
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
	};
}