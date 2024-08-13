#include "Framebuffer.h"

namespace TooGoodEngine {
	namespace OpenGL {

		Framebuffer::Framebuffer(const FramebufferInfo& info)
		{
			glCreateFramebuffers(1, &m_FramebufferHandle);

			std::vector<GLenum> drawBuffers;
			uint32_t index = 0;
			for (const auto& colorAttachment : info.ColorAttachments)
			{
				glNamedFramebufferTexture(
					m_FramebufferHandle,
					GL_COLOR_ATTACHMENT0 + (GLenum)index,
					colorAttachment->GetHandle(), 0);

				drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + (GLenum)index++);
			}

			if (info.DepthAttachment)
			{
				glNamedFramebufferTexture(
					m_FramebufferHandle,
					GL_DEPTH_ATTACHMENT,
					info.DepthAttachment->GetHandle(),
					0);
			}

			GLenum status = glCheckNamedFramebufferStatus(m_FramebufferHandle, GL_FRAMEBUFFER);
			TGE_VERIFY(status == GL_FRAMEBUFFER_COMPLETE, "framebuffer incomplete with code ", status);
		
			
			glNamedFramebufferDrawBuffers(m_FramebufferHandle, (GLsizei)drawBuffers.size(), drawBuffers.data());
		}

		Framebuffer::~Framebuffer()
		{
			if (m_FramebufferHandle)
				glDeleteFramebuffers(1, &m_FramebufferHandle);

			m_FramebufferHandle = 0;
		}

		Framebuffer::Framebuffer(Framebuffer&& other) noexcept
			: m_FramebufferHandle(other.m_FramebufferHandle)
		{
			other.m_FramebufferHandle = 0;
		}

		Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
		{
			if (this != &other)
			{
				m_FramebufferHandle = other.m_FramebufferHandle;
				other.m_FramebufferHandle = 0;
			}

			return *this;
		}

		void Framebuffer::BlitColorAttachment(const BlitInfo& info)
		{
			TGE_VERIFY(info.Destination && 
					  info.Source && 
					  info.SourceTexture && 
					  info.DestinationTexture, "destination or source was nullptr");

			TGE_VERIFY(info.DestinationWidth > 0 &&
					   info.DestinationHeight > 0 &&
					   info.SourceWidth > 0 &&
					   info.SourceHeight > 0, "all dimensions have to be bigger than 0");

			glNamedFramebufferTextureLayer(info.Source->GetHandle(), GL_COLOR_ATTACHMENT0 + (GLenum)info.SourceIndex,
				info.SourceTexture->GetHandle(), (GLint)info.SourceMipLevel, (GLint)info.SourceLayer);

			glNamedFramebufferTextureLayer(info.Destination->GetHandle(), GL_COLOR_ATTACHMENT0 + (GLenum)info.DestinationIndex,
				info.DestinationTexture->GetHandle(), (GLint)info.DestinationMipLevel, (GLint)info.DestinationLayer);
			
			glBlitNamedFramebuffer(
				(GLuint)info.Source->GetHandle(), (GLuint)info.Destination->GetHandle(),
				0, 0, (GLint)info.SourceWidth, (GLint)info.SourceHeight,
				0, 0, (GLint)info.DestinationWidth, (GLint)info.DestinationHeight,
				GL_COLOR_BUFFER_BIT, GL_LINEAR);
					
		}

		void Framebuffer::BlitDepthAttachment(const BlitInfo& info)
		{
			TGE_VERIFY(info.Destination && info.Source, "destination or source was nullptr");
			TGE_VERIFY(info.DestinationWidth > 0 &&
				info.DestinationHeight > 0 &&
				info.SourceWidth > 0 &&
				info.SourceHeight > 0, "all dimensions have to be bigger than 0");

			glBlitNamedFramebuffer(
				(GLuint)info.Source->GetHandle(), (GLuint)info.Destination->GetHandle(),
				0, 0, (GLint)info.SourceWidth, (GLint)info.SourceHeight,
				0, 0, (GLint)info.DestinationWidth, (GLint)info.DestinationHeight,
				GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		}
		
	}
}