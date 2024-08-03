#include "VertexArray.h"


namespace TooGoodEngine {
	namespace OpenGL {

		VertexArray::VertexArray()
			: m_VertexArrayHandle(0), m_CurrentOffset(0)
		{
			glCreateVertexArrays(1, &m_VertexArrayHandle);
		}
		VertexArray::~VertexArray()
		{
			if (m_VertexArrayHandle)
				glDeleteVertexArrays(1, &m_VertexArrayHandle);

			m_VertexArrayHandle = 0;
			m_CurrentOffset = 0;
		}
		VertexArray::VertexArray(const VertexArray& other)
			: m_VertexArrayHandle(other.m_VertexArrayHandle), m_CurrentOffset(other.m_CurrentOffset)
		{
		}
		VertexArray::VertexArray(VertexArray&& other) noexcept
			: m_VertexArrayHandle(other.m_VertexArrayHandle), m_CurrentOffset(other.m_CurrentOffset)
		{
			other.m_VertexArrayHandle = 0;
			other.m_CurrentOffset = 0;
		}
		VertexArray& VertexArray::operator=(const VertexArray& other)
		{
			if (this != &other)
			{
				m_VertexArrayHandle = other.m_VertexArrayHandle;
				m_CurrentOffset = other.m_CurrentOffset;
			}
			
			return *this;
		}
		VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
		{
			if (this != &other)
			{
				m_VertexArrayHandle = other.m_VertexArrayHandle;
				m_CurrentOffset = other.m_CurrentOffset;

				other.m_VertexArrayHandle = 0;
				other.m_CurrentOffset = 0;
			}

			return *this;
		}
		void VertexArray::AttachVertexBuffer(const Buffer& buffer, uint32_t bindingIndex, size_t offset, size_t stride)
		{
			glVertexArrayVertexBuffer(m_VertexArrayHandle,
				(GLuint)bindingIndex,
				(GLuint)buffer.GetHandle(), 
				(GLintptr)offset, 
				(GLsizei)stride);
		}
		void VertexArray::AttachVertexBuffer(const Ref<Buffer>& buffer, uint32_t bindingIndex, size_t offset, size_t stride)
		{
			TGE_VERIFY(buffer, "buffer was nullptr");
			AttachVertexBuffer((*buffer), bindingIndex, offset, stride);
		}
		void VertexArray::AttachIndexBuffer(const Buffer& buffer)
		{
			glVertexArrayElementBuffer(m_VertexArrayHandle, buffer.GetHandle());
		}

		void VertexArray::AttachIndexBuffer(const Ref<Buffer>& buffer)
		{
			TGE_VERIFY(buffer, "buffer was nullptr");
			AttachIndexBuffer(*buffer);
		}

		void VertexArray::AttachVertexInput(const Buffer& buffer, const Program& program, const VertexInputMap& map)
		{
			size_t totalStride = 0;

			for (const auto& [name, input] : map)
				totalStride += GetSizeFromType(input.Type);
#
			for (const auto& [name, input] : map)
			{
				GLuint index = glGetAttribLocation(program.GetHandle(), name.c_str()); 

				glVertexArrayAttribFormat(
					m_VertexArrayHandle,
					index,
					(GLint)GetSizeFromType(input.Type),
					GetBaseTypeFromVertexType(input.Type),
					GL_FALSE, m_CurrentOffset);

				glEnableVertexArrayAttrib(m_VertexArrayHandle, index);
				glVertexArrayAttribBinding(m_VertexArrayHandle, index, (GLuint)m_CurrentBufferIndex);

				if (input.Instanced)
					glVertexArrayBindingDivisor(m_VertexArrayHandle, index, 1);

				m_CurrentOffset += GetSizeFromType(input.Type);
			}

			AttachVertexBuffer(buffer, m_CurrentBufferIndex, 0, totalStride);

			m_CurrentBufferIndex += 1;
		}


		void VertexArray::AttachVertexInput(const Ref<Buffer>& buffer, const Ref<Program>& program, const VertexInputMap& map)
		{
			TGE_VERIFY(buffer, "buffer was nullptr");
			AttachVertexInput(*buffer, *program, map);
		}

		constexpr size_t VertexArray::GetSizeFromType(VertexType type)
		{
			switch (type)
			{
				case VertexType::Float:     return sizeof(float);
				case VertexType::Vector2:   return sizeof(float) * 2;
				case VertexType::Vector3:   return sizeof(float) * 3;
				case VertexType::Vector4:   return sizeof(float) * 4;
				case VertexType::Matrix4x4: return sizeof(float) * 16;
				case VertexType::Int:		return sizeof(int);
				case VertexType::Uint:	    return sizeof(uint32_t);
				case VertexType::None:
				default:					return 0;
			}
		}
		constexpr GLenum VertexArray::GetBaseTypeFromVertexType(VertexType type)
		{
			switch (type)
			{
				case VertexType::Float:     
				case VertexType::Vector2:   
				case VertexType::Vector3:   
				case VertexType::Vector4:	return GL_FLOAT;
				case VertexType::Matrix4x4: return GL_FLOAT_MAT4;
				case VertexType::Int:		return GL_INT;
				case VertexType::Uint:	    return GL_UNSIGNED_INT;
				case VertexType::None:
				default:					return GL_NONE;
			}
		}
	}
}