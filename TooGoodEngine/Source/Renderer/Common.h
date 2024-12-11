#pragma once

#include "API/OpenGL/Framebuffer.h"

namespace TooGoodEngine {

	struct CommonRendererData
	{
		bool Initialized = false;
		OpenGL::Framebuffer FinalImageFramebuffer;
		Ref<OpenGL::Texture2D> FinalImage;
		Ref<OpenGL::Texture2D> DepthTexture;
	};

	class Common
	{
	public:
		static void Init(uint32_t viewportWidth, uint32_t viewportHeight);

		static void OnWindowResize(uint32_t viewportWidth, uint32_t viewportHeight);

		static bool IsInitialized();
		static OpenGL::Framebuffer&   GetFramebuffer();
		static Ref<OpenGL::Texture2D> GetTexture();
		static Ref<OpenGL::Texture2D> GetDepth();
	};

}