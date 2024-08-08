#include "Image.h"

#include "External/stb/stb_image.h"

namespace TooGoodEngine {
	Image::Image(const OpenGL::Texture2DInfo& info)
		: m_Texture(info)
	{
	}
	Ref<Image> Image::LoadImageAssetFromFile(const std::filesystem::path& path)
	{
		const int desiredChannels = 4;
		int channels = 0;
		stbi_set_flip_vertically_on_load(true);

		uint8_t* imageData = nullptr;
		int width = 0, height = 0;

		imageData = stbi_load(path.string().c_str(), &width, &height, &channels, desiredChannels);

		const char* failure = stbi_failure_reason();
		if (failure)
		{
			TGE_LOG_ERROR("failed to load image ", path, " because ", failure);
			return nullptr;
		}

		OpenGL::Texture2DInfo info{};
		info.Type = OpenGL::Texture2DType::Texture;
		info.Format = OpenGL::Texture2DFormat::RGBA8;
		info.Width = (uint32_t)width;
		info.Height = (uint32_t)height;
		info.Data = imageData;

		info.Paramaters[OpenGL::TextureParamater::MinFilter] = OpenGL::TextureParamaterOption::Linear;
		info.Paramaters[OpenGL::TextureParamater::MagFilter] = OpenGL::TextureParamaterOption::Linear;

		Ref<Image> image = CreateRef<Image>(info);

		if (imageData)
			stbi_image_free(imageData);

		return image;
	}

}