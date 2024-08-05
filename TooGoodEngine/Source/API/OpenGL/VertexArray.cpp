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
		VertexArray::VertexArray(VertexArray&& other) noexcept
			: m_VertexArrayHandle(other.m_VertexArrayHandle), m_CurrentOffset(other.m_CurrentOffset)
		{
			other.m_VertexArrayHandle = 0;
			other.m_CurrentOffset = 0;
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
		void VertexArray::AttachVertexBuffer(Buffer* buffer, uint32_t bindingIndex, size_t offset, size_t stride)
		{
			TGE_VERIFY(buffer, "buffer was nullptr");

			glVertexArrayVertexBuffer(m_VertexArrayHandle,
				(GLuint)bindingIndex,
				(GLuint)buffer->GetHandle(), 
				(GLintptr)offset,
				(GLsizei)stride);
		}
	
		void VertexArray::AttachIndexBuffer(Buffer* buffer)
		{
			TGE_VERIFY(buffer, "buffer was nullptr");
			glVertexArrayElementBuffer(m_VertexArrayHandle, buffer->GetHandle());
		}

		void VertexArray::AttachVertexInput(Buffer* buffer, Program* program, const VertexInputMap& map)
		{
			TGE_VERIFY(buffer && program, "buffer or program was nullptr");

			size_t totalStride = 0;

			for (const auto& [name, input] : map)
				totalStride += GetSizeFromType(input.Type);
			
			size_t startingOffset = m_CurrentOffset;

			for (const auto& [name, input] : map)
			{
				program->Use();
				GLint index = glGetAttribLocation(program->GetHandle(), name.c_str());

				if (input.Type == VertexType::Matrix4x4)
				{
					GLuint numColumns = GetNumberOfElements(input.Type); 
					GLuint baseType = GetBaseTypeFromVertexType(input.Type); 

					for (GLuint i = 0; i < numColumns; ++i) 
					{
						GLint columnIndex = index + i;

						glVertexArrayAttribFormat(
							m_VertexArrayHandle,
							columnIndex,
							GetNumberOfElements(VertexType::Vector4),
							baseType,
							GL_FALSE,
							m_CurrentOffset + sizeof(float) * 4 * i); 

						glEnableVertexArrayAttrib(m_VertexArrayHandle, columnIndex);
						glVertexArrayAttribBinding(m_VertexArrayHandle, columnIndex, (GLuint)m_CurrentBufferIndex);

						if (input.Instanced)
							glVertexArrayBindingDivisor(m_VertexArrayHandle, columnIndex, 1);
					}

					m_CurrentOffset += sizeof(float) * 4 * numColumns; 

					if (input.Instanced)
						glVertexArrayBindingDivisor(m_VertexArrayHandle, index, 1);

					continue;
				}
				
				glVertexArrayAttribFormat(
					m_VertexArrayHandle,
					index,
					GetNumberOfElements(input.Type),
					GetBaseTypeFromVertexType(input.Type),
					GL_FALSE, m_CurrentOffset);

				glEnableVertexArrayAttrib(m_VertexArrayHandle, index);
				glVertexArrayAttribBinding(m_VertexArrayHandle, index, (GLuint)m_CurrentBufferIndex);

				if (input.Instanced)
					glVertexArrayBindingDivisor(m_VertexArrayHandle, index, 1);

				m_CurrentOffset += (uint32_t)GetSizeFromType(input.Type);
			}

			AttachVertexBuffer(buffer, m_CurrentBufferIndex, startingOffset, totalStride);

			m_CurrentBufferIndex += 1;
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
				case VertexType::Matrix4x4: return GL_FLOAT;
				case VertexType::Int:		return GL_INT;
				case VertexType::Uint:	    return GL_UNSIGNED_INT;
				case VertexType::None:
				default:					return GL_NONE;
			}
		}

		constexpr GLint VertexArray::GetNumberOfElements(VertexType type)
		{
			switch (type)
			{
			case VertexType::Vector2:	return 2;
			case VertexType::Vector3:	return 3;
			case VertexType::Vector4:	return 4;
			case VertexType::Matrix4x4: return 4;
			case VertexType::Int:		
			case VertexType::Uint:	    
			case VertexType::Float:		return 1;
			case VertexType::None:
			default:					return GL_NONE;
			}
		}
	}
}