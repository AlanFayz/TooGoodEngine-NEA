#pragma once

#include "Asset.h"

#include "API/OpenGL/Texture2D.h"
#include "API/OpenGL/Program.h"

namespace TooGoodEngine {

	class EnviormentMap : public Asset
	{
	public:
		EnviormentMap() = default;
		~EnviormentMap() = default;

		inline OpenGL::Texture2D& GetTexture() { return m_Texture; }
		inline OpenGL::Texture2D& GetIrradianceMap() { return m_IrradianceMap; }

		virtual const AssetType GetAssetType() const { return AssetType::EnviormentMap; }
		static  const AssetType GetStaticAssetType() { return AssetType::EnviormentMap; }

		//this will be taken in as an equirectanglar image (.hdr)
		static Ref<EnviormentMap> LoadEnviromentMapAssetFromFile(const std::filesystem::path& path);

	private:
		static void _RenderCube(OpenGL::Program* program);

	private:
		OpenGL::Texture2D m_Texture;
		OpenGL::Texture2D m_IrradianceMap;
	};

}