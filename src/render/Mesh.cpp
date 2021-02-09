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
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		//else if (name == "texture_normal")
		//    number = std::to_string(normalNr++); // transfer unsigned int to stream
		//else if (name == "texture_height")
		//    number = std::to_string(heightNr++); // transfer unsigned int to stream

		glBindTexture(GL_TEXTURE_2D, textures[i].textureID);
		glUniform1i(glGetUniformLocation(shader.getShaderPrgram(), ("material." + name + number).c_str()), i);
		
	}

	// draw mesh
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
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &verticesVBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &bonesVBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bonesVBO);
	glBufferData(GL_ARRAY_BUFFER, boneWeight.size() * sizeof(boneWeight[0]), &boneWeight[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// set the vertex attribute pointers
	// vertex Positions
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//// vertex tangent
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	//// vertex bitangent
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glBindBuffer(GL_ARRAY_BUFFER, bonesVBO);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (GLvoid*)0); // for INT Ipointer
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (GLvoid*)offsetof(VertexBoneData, weights));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);

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