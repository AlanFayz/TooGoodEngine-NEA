#include "Framebuffer.h"

namespace TooGoodEngine {
	namespace OpenGL {

		Framebuffer::Framebuffer(const FramebufferInfo& info)
		{
			glCreateFramebuffers(1, &m_FramebufferHandle);

			uint32_t index = 0;
			for (const auto& colorAttachment : info.ColorAttachments)
			{
				glNamedFramebufferTexture(
					m_FramebufferHandle,
					GL_COLOR_ATTACHMENT0 + (GLenum)index,
					colorAttachment->GetHandle(), 0);
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
	}
}