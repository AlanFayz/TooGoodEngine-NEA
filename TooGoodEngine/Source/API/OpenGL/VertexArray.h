#pragma once

#include "Buffer.h"
#include "Program.h"

#include <map>

namespace TooGoodEngine {
	namespace OpenGL {

		enum class VertexType
		{
			None = 0, Float, Vector2, Vector3, Vector4, 
			Matrix4x4, Int, Uint //will add more when needed
		};

		struct VertexInput
		{
			VertexType Type;
			bool Instanced;
		};
		
		using VertexInputMap = std::map<std::string, VertexInput>;

		class VertexArray
		{
		public:
			VertexArray();
			~VertexArray();

			VertexArray(const VertexArray& other);
			VertexArray(VertexArray&& other) noexcept;

			VertexArray& operator=(const VertexArray& other);
			VertexArray& operator=(VertexArray&& other) noexcept;

			void AttachVertexBuffer(const Buffer& buffer, uint32_t bindingIndex, size_t offset, size_t stride);
			void AttachVertexBuffer(const Ref<Buffer>& buffer, uint32_t bindingIndex, size_t offset, size_t stride);

			void AttachIndexBuffer(const Buffer& buffer);
			void AttachIndexBuffer(const Ref<Buffer>& buffer);

			void AttachVertexInput(const Buffer& buffer, const Program& program, const VertexInputMap& map);
			void AttachVertexInput(const Ref<Buffer>& buffer, const Ref<Program>& program, const VertexInputMap& map);

		private:
			constexpr size_t GetSizeFromType(VertexType type);
			constexpr GLenum GetBaseTypeFromVertexType(VertexType type);

		private:
			uint32_t m_VertexArrayHandle = 0;
			uint32_t m_CurrentOffset = 0;
			uint32_t m_CurrentBufferIndex = 0;
		};

	}
}