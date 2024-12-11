#include "Common.h"

namespace TooGoodEngine {

	static CommonRendererData s_Data;

	void Common::Init(uint32_t viewportWidth, uint32_t viewportHeight)
	{
		s_Data.Initialized = true;
		OnWindowResize(viewportWidth, viewportHeight);
	}

	void Common::OnWindowResize(uint32_t viewportWidth, uint32_t viewportHeight)
	{
		{
			OpenGL::Texture2DInfo info{};
			info.Type = OpenGL::Texture2DType::Texture;
			info.Format = OpenGL::Texture2DFormat::RGBA32F;
			info.Width = viewportWidth;
			info.Height = viewportHeight;

			info.MipMapLevels = 1;
			info.Paramaters[OpenGL::TextureParamater::MinFilter] = OpenGL::TextureParamaterOption::Linear;

			info.Paramaters[OpenGL::TextureParamater::MagFilter] = OpenGL::TextureParamaterOption::Linear;
			info.Paramaters[OpenGL::TextureParamater::WrapModeS] = OpenGL::TextureParamaterOption::ClampToEdge;
			info.Paramaters[OpenGL::TextureParamater::WrapModeT] = OpenGL::TextureParamaterOption::ClampToEdge;

			s_Data.FinalImage.reset();
			s_Data.FinalImage = CreateRef<OpenGL::Texture2D>(info);
		}

		{
			OpenGL::Texture2DInfo info{};
			info.Type   = OpenGL::Texture2DType::DepthTexture;
			info.Format = OpenGL::Texture2DFormat::DEPTH_32F;
			info.Width  = viewportWidth;
			info.Height = viewportHeight;

			info.Paramaters[OpenGL::TextureParamater::MinFilter] = OpenGL::TextureParamaterOption::Linear;
			info.Paramaters[OpenGL::TextureParamater::MagFilter] = OpenGL::TextureParamaterOption::Linear;
			info.Paramaters[OpenGL::TextureParamater::WrapModeS] = OpenGL::TextureParamaterOption::ClampToBorder;
			info.Paramaters[OpenGL::TextureParamater::WrapModeT] = OpenGL::TextureParamaterOption::ClampToBorder;

			s_Data.DepthTexture.reset();
			s_Data.DepthTexture = CreateRef<OpenGL::Texture2D>(info);
		}

		{
			OpenGL::FramebufferInfo info{};
			info.ColorAttachments.push_back(s_Data.FinalImage.get());
			info.DepthAttachment = s_Data.DepthTexture.get();

			s_Data.FinalImageFramebuffer = OpenGL::Framebuffer(info);
		}
	}

	bool Common::IsInitialized()
	{
		return s_Data.Initialized;
	}

	OpenGL::Framebuffer& Common::GetFramebuffer()
	{
		return s_Data.FinalImageFramebuffer;
	}

	Ref<OpenGL::Texture2D> Common::GetTexture()
	{
		return s_Data.FinalImage;
	}

	Ref<OpenGL::Texture2D> Common::GetDepth()
	{
		return s_Data.DepthTexture;
	}

}
