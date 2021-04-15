#include <iostream>
#include "render/Mesh.h"
#include "glm/gtc/matrix_transform.hpp"

se::Mesh::Mesh()
{

}

se::Mesh::~Mesh()
{

}

se::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setUpMesh();
}

se::Mesh::Mesh(std::vector<Vertex> vertices,
	std::vector<unsigned int> indices,
	std::vector<Texture> textures,
	std::vector<VertexBoneData> boneWeight)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->boneWeight = boneWeight;

	setUpMesh();
}


auto se::Mesh::draw(Shader shader)->void
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); 
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
		    number = std::to_string(normalNr++);
		else if (name == "texture_height")
		    number = std::to_string(heightNr++); 

		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
		glUniform1i(glGetUniformLocation(shader.getShader(), ("material." + name + number).c_str()), i);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

auto se::Mesh::setUpMesh()->void
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

auto se::VertexBoneData::addBoneData(unsigned int boneID, float weight)->void
{
	for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++)
	{
		if (weights[i] == 0.0)
		{
			ids[i] = boneID;
			weights[i] = weight;
			return;
		}
	}
}