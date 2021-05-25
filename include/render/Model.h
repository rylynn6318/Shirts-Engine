#pragma once
#include <vector>
#include <string>
#include <map>
#include "render/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace se
{
	const int MAX_BONES = 100;
	unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma);

	class Model
	{
	public:
		Model();
		~Model();
		void initShaders(GLuint shader_program);
		auto loadStaticModel(std::string const& path) -> void;
		auto loadSkeletalodel(std::string const& path) -> void;
		auto draw(Shader& shader)->void;
		auto skeletalDraw(Shader& shader)->void;
		aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend);

	public:
		std::vector<Texture> textures_loaded;
		std::vector<Mesh> meshes;
		std::string directory;
		bool gammaCorrection;

	//private:
		Assimp::Importer importer;
		const aiScene* scene;
		GLuint boneLocation[MAX_BONES];

		auto processStaticNode(aiNode* node, const aiScene* scene) -> void;
		auto processSkeletalNode(aiNode* node, const aiScene* scene) -> void;
		auto processStaticMesh(aiMesh* mesh, const aiScene* scene)->Mesh;
		auto processSkeletalMesh(aiMesh* mesh, const aiScene* scene)->Mesh;
		auto loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)->std::vector<Texture>;

		aiMatrix4x4 globalInverseTransform;
		
		float ticksPerSecond = 0.0f;

		unsigned int findPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
		unsigned int findRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
		unsigned int findScaling(float p_animation_time, const aiNodeAnim* p_node_anim);
		aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
		aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
		aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim);

		auto findNodeAnim(const aiAnimation* p_animation, const std::string p_node_name)->const aiNodeAnim*;
		std::map<std::string, unsigned int> boneMap; 
		unsigned int boneNums= 0;
		std::vector<BoneMatrix> boneMatrix;

		auto boneTransform(double time, std::vector<aiMatrix4x4>& transforms)->void;
		auto readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform)->void;

		glm::quat rotate_head_xz = glm::quat(cosf(glm::radians(0.0f)), sinf(glm::radians(0.0f)) * glm::vec3(1.0f, 0.0f, 0.0f));
	};

}