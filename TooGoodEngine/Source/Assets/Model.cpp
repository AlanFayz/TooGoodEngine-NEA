#include "Model.h"
#include "Project/Project.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


namespace TooGoodEngine {

	static std::unordered_map<std::string, UUID> s_CachedImages; //Asset id will be to images in this case

	Model::Model(const std::vector<Geometry>& geometry)
		: m_GeometryData(geometry)
	{
	}
	Ref<Model> Model::LoadModelAssetFromFile(const std::filesystem::path& path)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate      |
																aiProcess_FlipUVs          |
																aiProcess_GenSmoothNormals |
																aiProcess_JoinIdenticalVertices);

		if (!scene)
		{
			TGE_LOG_ERROR("failed to import ", path, " ", importer.GetErrorString());
			return nullptr;
		}

		std::vector<Geometry> geometry;

		_ProcessNode(geometry, scene->mRootNode, scene);

		return CreateRef<Model>(geometry);
	}
	void Model::_ProcessNode(std::vector<Geometry>& geometry, const aiNode* node, const aiScene* scene)
	{
		for (size_t i = 0; i < node->mNumChildren; i++)
			_ProcessNode(geometry, node->mChildren[i], scene);

		for (size_t i = 0; i < node->mNumMeshes; i++)
			_ProcessMesh(geometry, scene->mMeshes[node->mMeshes[i]], scene);
	}
	void Model::_ProcessMesh(std::vector<Geometry>& geometry, const aiMesh* mesh, const aiScene* scene)
	{
		geometry.push_back({});
		auto& currentMesh = geometry.back();

		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex Vertex{};
			memcpy(&Vertex.Position, &mesh->mVertices[i], sizeof(glm::vec3));
			memcpy(&Vertex.Normal,   &mesh->mNormals[i],  sizeof(glm::vec3));

			Vertex.TextureCoordinate = { 0.0f, 0.0f };

			if (mesh->mTextureCoords[0] != nullptr)
				memcpy(&Vertex.TextureCoordinate, &mesh->mTextureCoords[0][i], sizeof(glm::vec2));

			currentMesh.Vertices.push_back(Vertex);
		}

		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			for (size_t j = 0; j < mesh->mFaces[i].mNumIndices; j++)
				currentMesh.Indices.push_back(mesh->mFaces[i].mIndices[j]);
		}

		currentMesh.Material = _ConstructMaterialInfoFromAiMaterial(scene->mMaterials[mesh->mMaterialIndex]);
	}
	MaterialInfo Model::_ConstructMaterialInfoFromAiMaterial(const aiMaterial* material)
	{
		MaterialInfo info{};
		
		info.AmbientTexture = _LoadImageFromAiMaterial(material, aiTextureType_AMBIENT, 0);

		if (!info.AmbientTexture)
			material->Get(AI_MATKEY_COLOR_AMBIENT, info.Ambient);

		info.AlbedoTexture = _LoadImageFromAiMaterial(material, aiTextureType_DIFFUSE, 0);
		info.AlbedoFactor  = 1.0f;

		if (!info.AlbedoTexture)
			material->Get(AI_MATKEY_COLOR_DIFFUSE, info.Emission);

		info.EmissionTexture = _LoadImageFromAiMaterial(material, aiTextureType_EMISSION_COLOR, 0);
		info.EmissionFactor = material->Get(AI_MATKEY_EMISSIVE_INTENSITY, info.EmissionFactor);

		if (!info.EmissionTexture)
			material->Get(AI_MATKEY_COLOR_EMISSIVE, info.Emission);
		

		info.MetallicTexture = _LoadImageFromAiMaterial(material, aiTextureType_METALNESS, 0);
		info.MetallicFactor = 1.0f;

		if (!info.MetallicFactor)
			material->Get(AI_MATKEY_METALLIC_FACTOR, info.Metallic);
		

		info.RoughnessTexture = _LoadImageFromAiMaterial(material, aiTextureType_DIFFUSE_ROUGHNESS, 0);

		if (!info.RoughnessTexture)
			material->Get(AI_MATKEY_ROUGHNESS_FACTOR, info.Roughness);


		return info;
	}
	Ref<Image> Model::_LoadImageFromAiMaterial(const aiMaterial* material, aiTextureType type, uint32_t index)
	{
		aiString path{};

		if (material->GetTexture(type, index, &path) == AI_SUCCESS)
		{
			std::filesystem::path filePath = path.C_Str();
			std::string strPath = filePath.string();

			Ref<Image> image = g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Image>(filePath);

			if (!image)
				return nullptr;

			if (s_CachedImages.count(strPath) == 0)
				s_CachedImages[strPath] = image->GetAssetID();

			return image;
		}

		return nullptr;
	}
}