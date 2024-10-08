#pragma once

#include "API/OpenGL/Texture2D.h"
#include "Asset.h"

namespace TooGoodEngine {

	//simply a wrapper around a texture but includes the asset API.
	class Image : public Asset
	{
	public:
		Image() = default;
		Image(const OpenGL::Texture2DInfo& info); 
		~Image() = default;

		inline OpenGL::Texture2D& GetTexture() { return m_Texture; }

		virtual const AssetType GetAssetType() const { return AssetType::Image;  }

		static Ref<Image> LoadImageAssetFromFile(const std::filesystem::path& path);

	private:
		OpenGL::Texture2D m_Texture;
	};

}