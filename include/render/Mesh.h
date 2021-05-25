#pragma once
#include <vector>
#include <string>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "assimp/Importer.hpp"
#include "shader/Shader.h"

namespace se
{
	const int NUM_BONES_PER_VERTEX = 4;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		//glm::vec3 tangent;
		//glm::vec3 bitangent;
	};

	struct Texture 
	{
		unsigned int ID;
		std::string type;
		std::string path;
	};

	struct VertexBoneData
	{
		unsigned int ids[NUM_BONES_PER_VERTEX];   // we have 4 bone ids for EACH vertex & 4 weights for EACH vertex
		float weights[NUM_BONES_PER_VERTEX];

		VertexBoneData()
		{
			memset(ids, 0, sizeof(ids));    // init all values in array = 0
			memset(weights, 0, sizeof(weights));
		}

		void addBoneData(unsigned int bone_id, float weight);
	};

	struct BoneMatrix
	{
		aiMatrix4x4 offsetMatrix;
		aiMatrix4x4 finalWorldTransform;
	};

	class Mesh
	{
	public:
		Mesh();
		~Mesh();
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		Mesh(std::vector<Vertex> vertices, 
			std::vector<unsigned int> indices, 
			std::vector<Texture> textures,
			std::vector<VertexBoneData> boneWeight
		);
		auto draw(Shader shader)->void;

	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		std::vector<VertexBoneData> boneWeight;

	private:
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		GLuint bonesVBO;

		auto setUpMesh()->void;
	};
}