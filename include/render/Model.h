#pragma once
#include <vector>
#include <string>
#include "render/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace se
{
	//unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

	class Model
	{
	public:
		Model() = delete;
		~Model();
		Model(std::string const& path, bool gamma = false);
		auto draw(Shader& shader)->void;

	public:
		std::vector<Texture> textures_loaded;
		std::vector<Mesh> meshes;
		std::string directory;
		bool gammaCorrection;

	private:
		auto loadModel(std::string const& path) -> void;
		auto processNode(aiNode* node, const aiScene* scene) -> void;
		auto processMesh(aiMesh* mesh, const aiScene* scene)->Mesh;
		auto loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)->std::vector<Texture>;
	};

}