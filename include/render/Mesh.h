#pragma once
#include <vector>
#include <string>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "shader/Shader.h"

namespace se
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct Texture 
	{
		unsigned int textureID;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		Mesh() = delete;
		~Mesh();
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		auto draw(Shader shader)->void;

	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

	private:
		GLuint VAO;
		GLuint verticesVBO;
		GLuint EBO;
		GLuint bonesVBO;

		auto setUpMesh()->void;
	};
}