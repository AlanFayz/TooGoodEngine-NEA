#pragma once

#include "External/glad/glad.h"
#include "Texture2D.h"

#include <vector>

namespace TooGoodEngine {
	namespace OpenGL {

		//never going to use render buffers 
		//so not going to bother adding an option
		//for them

		struct FramebufferInfo
		{
			std::vector<Texture2D*> ColorAttachments; 
			Texture2D* DepthAttachment = nullptr; //if needed will make this DepthStencil 
		};

		//this is a class which holds textures/rendering attachments (only textures are used)
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
			inline const void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle); }
			inline const void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

			struct BlitInfo
			{
				Framebuffer* Source;
				Framebuffer* Destination;
				Texture2D* SourceTexture;
				Texture2D* DestinationTexture;
				uint32_t SourceWidth, SourceHeight;
				uint32_t DestinationWidth, DestinationHeight;
				uint32_t SourceIndex, DestinationIndex;
				uint32_t SourceLayer, DestinationLayer;
				uint32_t SourceMipLevel, DestinationMipLevel;
			};

			//blit copies an attachemnt from one framebuffer to another.
			static void BlitColorAttachment(const BlitInfo& info); 

			//note, source and destination index are obselete here
			static void BlitDepthAttachment(const BlitInfo& info);

		private:
			uint32_t m_FramebufferHandle = 0;
		};

	}

}