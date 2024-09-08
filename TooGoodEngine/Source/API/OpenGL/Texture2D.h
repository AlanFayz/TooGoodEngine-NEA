#pragma once

#include "Core/Base.h"
#include "External/glad/glad.h"

#include <map>

namespace TooGoodEngine {
	namespace OpenGL {

		enum class Texture2DType
		{
			None = 0,
			Texture,
			Multisample,
			DepthTexture, 
			CubeMap
		};

		enum class Texture2DFormat
		{
			None = 0, RGBA16F, RGBA32F, RGBA8, RGBA, DEPTH_32F,
			RGB16F, RGB32F, RGB8, RGB
		};

		enum class TextureParamater
		{
			None = 0, MinFilter, MagFilter, WrapModeS, WrapModeT, WrapModeR
		};

		enum class TextureParamaterOption
		{
			None = 0, Nearest, Linear,MipMapLinear, ClampToBorder, ClampToEdge
		};

		struct Texture2DInfo
		{
			Texture2DType   Type;
			Texture2DFormat Format;
			uint32_t Width, Height;
			uint32_t MipMapLevels = 1;
			uint32_t NumberOfSamples = 1;
			void* Data = nullptr;

			void* CubeMapData[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

			std::map<TextureParamater, TextureParamaterOption> Paramaters;
		};

		//similar to a buffer but is optimized for images specifically and gives extra benifits such as
		//the ability to render directly to them and provides samplers.
		class Texture2D
		{
		public:
			Texture2D() = default;
			Texture2D(const Texture2DInfo& info);
			~Texture2D();

			Texture2D(const Texture2D& other) = delete;
			Texture2D& operator=(const Texture2D& other) = delete;

			Texture2D(Texture2D&& other) noexcept;
			Texture2D& operator=(Texture2D&& other) noexcept;
			
			void SetData(void* data);

			void MakeResident();
			void MakeNonResident();

			void GenerateMipmaps() const;
			void Bind(uint32_t textureUnit) const;
			void BindImage(uint32_t textureUnit, 
						   uint32_t level, 
						   uint32_t layer, bool layered);

			inline const uint32_t GetHandle()  const { return m_TextureHandle; }
			inline const GLuint64 GetAddress() const { return m_TextureAddress; }
			inline const Texture2DType GetType() const { return m_Type; }
			inline const Texture2DFormat GetFormat() const { return m_Format; }
			inline const uint32_t GetWidth() const { return m_Width; }
			inline const uint32_t GetHeight() const { return m_Height; }

		private:
			constexpr GLenum GetInternalFormat(Texture2DFormat format);
			constexpr GLenum GetComponentFormat(Texture2DFormat format);
			constexpr GLenum GetDataType(Texture2DFormat format);
			constexpr GLenum GetTarget(Texture2DType type);
			constexpr GLenum GetTextureParamater(TextureParamater paramater);
			constexpr GLenum GetTextureParamaterOption(TextureParamaterOption option);

		private:
			uint32_t m_TextureHandle = 0;
			GLuint64 m_TextureAddress = 0;

			Texture2DType   m_Type;
			Texture2DFormat m_Format;

			uint32_t m_Width, m_Height;
			bool m_Resident = false;
		};
	}
}