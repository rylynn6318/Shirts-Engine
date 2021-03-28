#pragma once
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include "render/Mesh.h"

namespace se
{
	class StaticModel
	{
	public:
		StaticModel() {}
		~StaticModel() = default;
		std::vector<Texture> texturesLoaded;	
		std::vector<Mesh>  meshes;
		std::string directory;
		bool gammaCorrection;

		StaticModel(const std::string& path, bool gamma);
		auto draw(Shader& shader)->void;

	//private:
		auto loadModel(const std::string& path)->void;
		auto processNode(aiNode* node, const aiScene* scene)->void;
		auto processMesh(aiMesh* mesh, const aiScene* scene)->Mesh;
		auto loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)->std::vector<Texture>;

	};
}