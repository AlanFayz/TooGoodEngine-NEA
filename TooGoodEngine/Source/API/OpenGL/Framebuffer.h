#pragma once

#include "External/glad/glad.h"
#include "Texture2D.h"

#include <vector>

namespace TooGoodEngine {
	namespace OpenGL {

		struct FramebufferInfo
		{
			std::vector<Ref<Texture2D>> ColorAttachments;
			Ref<Texture2D> DepthAttachment; //if needed will make this DepthStencil 
		};

		class Framebuffer
		{
		public:
			Framebuffer(const FramebufferInfo& info);
			Framebuffer() = default;
			~Framebuffer();

			Framebuffer(const Framebuffer& other) = delete;
			Framebuffer& operator=(const Framebuffer& other) = delete;

			Framebuffer(Framebuffer&& other) noexcept;
			Framebuffer& operator=(Framebuffer&& other) noexcept;

			inline const uint32_t GetHandle() const { return m_FramebufferHandle; }

		private:
			uint32_t m_FramebufferHandle = 0;
		};

	}

}