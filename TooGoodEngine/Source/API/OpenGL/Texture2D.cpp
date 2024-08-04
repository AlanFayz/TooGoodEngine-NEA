#include "Texture2D.h"

namespace TooGoodEngine {
	namespace OpenGL {

		Texture2D::Texture2D(const Texture2DInfo& info)
			: m_Format(info.Format), m_Type(info.Type), m_Width(info.Width), m_Height(info.Height)
		{
			glCreateTextures(GetTarget(info.Type), 1, &m_TextureHandle);
			glBindTexture(GetTarget(info.Type), m_TextureHandle);

			for (const auto& [paramater, option] : info.Paramaters)
				glTextureParameteri(m_TextureHandle, GetTextureParamater(paramater), GetTextureParamaterOption(option));
			
			if (info.Type == Texture2DType::Multisample)
			{
				glTextureStorage2DMultisample(
					m_TextureHandle,
					(GLsizei)info.NumberOfSamples,
					GetInternalFormat(info.Format),
					(GLsizei)info.Width, (GLsizei)info.Height,
					GL_TRUE);

				return;
			}

			glTextureStorage2D(
				m_TextureHandle, 
				(GLsizei)info.MipMapLevels, 
				GetInternalFormat(info.Format), 
				(GLsizei)info.Width, (GLsizei)info.Height);

			if (info.Data)
			{
				glTextureSubImage2D(
					m_TextureHandle,
					0, 0, 0,
					(GLsizei)info.Width,
					(GLsizei)info.Height,
					GetDataType(info.Format),
					GetTarget(info.Type), info.Data);
			}

			glGenerateMipmap(GetTarget(info.Type));
			m_TextureAddress = glGetTextureHandleARB(m_TextureHandle);
		}

		Texture2D::~Texture2D()
		{
			if (m_TextureHandle)
				glDeleteTextures(1, &m_TextureHandle);
		

			m_TextureHandle = 0;
			m_TextureAddress = 0;
		}

		Texture2D::Texture2D(const Texture2D& other)
			: m_TextureHandle(other.m_TextureHandle), m_TextureAddress(other.m_TextureAddress), 
			  m_Format(other.m_Format), m_Type(other.m_Type), m_Width(other.m_Width), m_Height(other.m_Height)
		{
		}

		Texture2D::Texture2D(Texture2D&& other) noexcept
			: m_TextureHandle(other.m_TextureHandle), m_TextureAddress(other.m_TextureAddress),
			m_Format(other.m_Format), m_Type(other.m_Type), m_Width(other.m_Width), m_Height(other.m_Height)
		{
			other.m_TextureHandle = 0;
			other.m_TextureAddress = 0;
		}
		Texture2D& Texture2D::operator=(const Texture2D& other)
		{
			if (this != &other)
			{
				m_TextureHandle = other.m_TextureHandle;
				m_TextureAddress = other.m_TextureAddress;
				m_Format = other.m_Format;
				m_Type = other.m_Type;
				m_Width = other.m_Width;
				m_Height = other.m_Height;
			}
			return *this;
		}
		Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
		{
			if (this != &other)
			{
				m_TextureHandle = other.m_TextureHandle;
				m_TextureAddress = other.m_TextureAddress;
				m_Format = other.m_Format;
				m_Type = other.m_Type;
				m_Width = other.m_Width;
				m_Height = other.m_Height;

				other.m_TextureHandle = 0;
				other.m_TextureAddress = 0;
			}

			return *this;
		}
		void Texture2D::SetData(void* data)
		{
			TGE_VERIFY(data, "data was nullptr");

			glBindTexture(GetTarget(m_Type), m_TextureHandle);
			glTextureSubImage2D(
				m_TextureHandle,
				0, 0, 0,
				(GLsizei)m_Width,
				(GLsizei)m_Height,
				GetDataType(m_Format),
				GetTarget(m_Type), data);
			
		}
		constexpr GLenum Texture2D::GetInternalFormat(Texture2DFormat format)
		{
			switch (format)
			{
				case Texture2DFormat::RGBA16F:		return GL_RGBA16F;
				case Texture2DFormat::RGBA32F:		return GL_RGBA32F;
				case Texture2DFormat::RGBA8:		return GL_RGBA8;
				case Texture2DFormat::RGBA:			return GL_RGBA;
				case Texture2DFormat::DEPTH_32F:	return GL_DEPTH_COMPONENT32F;
				case Texture2DFormat::RGB16F:		return GL_RGB16F;
				case Texture2DFormat::RGB32F:		return GL_RGB32F;
				case Texture2DFormat::RGB8:		    return GL_RGB8;
				case Texture2DFormat::RGB:			return GL_RGB;
				case Texture2DFormat::None:
				default:							return GL_NONE;
			}

		}
		constexpr GLenum Texture2D::GetComponentFormat(Texture2DFormat format)
		{
			switch (format)
			{
				case Texture2DFormat::RGBA16F:		
				case Texture2DFormat::RGBA32F:		
				case Texture2DFormat::RGBA8:		
				case Texture2DFormat::RGBA:			return GL_RGBA;
				case Texture2DFormat::RGB16F:		
				case Texture2DFormat::RGB32F:		
				case Texture2DFormat::RGB8:		    
				case Texture2DFormat::RGB:			return GL_RGB;
				case Texture2DFormat::DEPTH_32F:	return GL_DEPTH_COMPONENT;
				case Texture2DFormat::None:
				default:							return GL_NONE;
			}
		}
		constexpr GLenum Texture2D::GetDataType(Texture2DFormat format)
		{
			switch (format)
			{
				case Texture2DFormat::RGBA16F:		
				case Texture2DFormat::RGBA32F:
				case Texture2DFormat::RGB16F:
				case Texture2DFormat::RGB32F:	
				case Texture2DFormat::DEPTH_32F: return GL_FLOAT;
				case Texture2DFormat::RGBA8:
				case Texture2DFormat::RGBA:			
				case Texture2DFormat::RGB8:
				case Texture2DFormat::RGB:		 return GL_UNSIGNED_BYTE;
				case Texture2DFormat::None:
				default:						 return GL_NONE;
			}
		}
		constexpr GLenum Texture2D::GetTarget(Texture2DType type)
		{
			switch (type)
			{
				case Texture2DType::Texture:		    return GL_TEXTURE_2D;
				case Texture2DType::Multisample:		return GL_TEXTURE_2D_MULTISAMPLE;
				case Texture2DType::DepthTexture:		return GL_TEXTURE_DEPTH;
				case Texture2DType::None:
				default:								return GL_NONE;
			}
		}
		constexpr GLenum Texture2D::GetTextureParamater(TextureParamater paramater)
		{
			switch (paramater)
			{
				case TextureParamater::MinFilter:	return GL_TEXTURE_MIN_FILTER;
				case TextureParamater::MagFilter:	return GL_TEXTURE_MAG_FILTER;
				case TextureParamater::WrapModeS:   return GL_TEXTURE_WRAP_S;
				case TextureParamater::WrapModeT:	return GL_TEXTURE_WRAP_T;
				case TextureParamater::None:
				default:							return GL_NONE;
			}
		}
		constexpr GLenum Texture2D::GetTextureParamaterOption(TextureParamaterOption option)
		{
			switch (option)
			{
				case TextureParamaterOption::Nearest:		return GL_NEAREST;
				case TextureParamaterOption::Linear:		return GL_LINEAR;
				case TextureParamaterOption::ClampToBorder: return GL_CLAMP_TO_BORDER;
				case TextureParamaterOption::None:
				default:								return GL_NONE;
			}
		}
	}
}