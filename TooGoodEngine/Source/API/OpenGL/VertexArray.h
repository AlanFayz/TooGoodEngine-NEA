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
		
		using VertexInputMap = std::vector<std::pair<std::string, VertexInput>>;

		//holds a collection of buffers which are made available 
		//in shader pipelines.
		class VertexArray
		{
		public:
			VertexArray();
			~VertexArray();

			VertexArray(const VertexArray& other) = delete;
			VertexArray& operator=(const VertexArray& other) = delete;

			VertexArray(VertexArray&& other) noexcept;
			VertexArray& operator=(VertexArray&& other) noexcept;

			void AttachVertexBuffer(Buffer* buffer, uint32_t bindingIndex, size_t offset, size_t stride);
			void AttachIndexBuffer(Buffer* buffer);
			void AttachVertexInput(Buffer* buffer, Program* program, const VertexInputMap& map);

			inline void Bind() { glBindVertexArray(m_VertexArrayHandle); }

		private:
			constexpr size_t GetSizeFromType(VertexType type);
			constexpr GLenum GetBaseTypeFromVertexType(VertexType type);
			constexpr GLint  GetNumberOfElements(VertexType type);

		private:
			uint32_t m_VertexArrayHandle = 0;
			uint32_t m_CurrentOffset = 0;
			uint32_t m_CurrentBufferIndex = 0;
		};

	}
}