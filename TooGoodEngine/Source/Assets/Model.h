#pragma once

#include "Asset.h"
#include "Renderer/RenderInfo.h"

//assimp 
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum   aiTextureType;

namespace TooGoodEngine {

	//a collection of geometries (simpler shapes)
	class Model : public Asset
	{
	public:
		Model() = default;
		Model(const std::vector<Geometry>& geometry);
		~Model() = default;

		inline void SetInfo(const ModelInfo& info) { m_Info = info; }
		inline const ModelInfo& GetInfo() const { return m_Info; }
		inline const std::vector<Geometry>& GetData() const { return m_GeometryData; }

		virtual const AssetType GetAssetType() const { return AssetType::Model; };

		static Ref<Model> LoadModelAssetFromFile(const std::filesystem::path& path);

	private:
		static void _ProcessNode(std::vector<Geometry>& geometry, const aiNode* node, const aiScene* scene);
		static void _ProcessMesh(std::vector<Geometry>& geometry, const aiMesh* mesh, const aiScene* scene);

		static MaterialInfo _ConstructMaterialInfoFromAiMaterial(const aiMaterial* material);
		static Ref<Image>   _LoadImageFromAiMaterial(const aiMaterial* material, aiTextureType type, uint32_t index);
	private:
		std::vector<Geometry> m_GeometryData;
		ModelInfo m_Info;

		static inline std::filesystem::path s_CurrentRelativePath;
	};

}