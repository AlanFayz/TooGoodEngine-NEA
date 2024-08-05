#include "Buffer.h"

namespace TooGoodEngine {
	namespace OpenGL {

		Buffer::Buffer(const BufferInfo& info)
			: m_BufferHandle(0), m_Capacity(info.Capacity), m_Flags(info.Masks), m_Mapped(false)
		{
			TGE_VERIFY(m_Capacity > 0, "capacity cannot be 0");

			glCreateBuffers(1, &m_BufferHandle);
			glNamedBufferStorage(m_BufferHandle, m_Capacity, info.Data, info.Masks);
		}

		Buffer::~Buffer()
		{
			if (m_BufferHandle)
			{
				if (m_Mapped)
					Unmap();

				glDeleteBuffers(1, &m_BufferHandle);
				m_BufferHandle = 0;
			}
		}

		Buffer::Buffer(Buffer&& other) noexcept
			: m_BufferHandle(other.m_BufferHandle), 
			  m_Capacity(other.m_Capacity), 
			  m_Flags(other.m_Flags), 
			  m_Mapped(other.m_Mapped) 
		{
			other.m_BufferHandle = 0;
			other.m_Capacity = 0;
			other.m_Flags = BufferOptionNone; 
			other.m_Mapped = false;
		}

		Buffer& Buffer::operator=(Buffer&& other) noexcept
		{
			if (this != &other)
			{
				m_BufferHandle = other.m_BufferHandle;
				m_Capacity = other.m_Capacity;
				m_Flags = other.m_Flags;
				m_Mapped = other.m_Mapped;

				other.m_BufferHandle = 0;
				other.m_Capacity = 0;
				other.m_Flags = BufferOptionNone;
				other.m_Mapped = false;
			}

			return *this;
		}

		void Buffer::Resize(size_t newCapacity)
		{
			TGE_VERIFY(newCapacity > 0, "new capacity cannot be bigger than 0");
			TGE_VERIFY(!m_Mapped, "buffer is current mapped");

			uint32_t newBuffer = 0;
			glCreateBuffers(1, &newBuffer);
			glNamedBufferStorage(newBuffer, newCapacity, nullptr, m_Flags);
			glCopyNamedBufferSubData(m_BufferHandle, newBuffer, 0, 0, (GLsizeiptr)std::min(m_Capacity, newCapacity));

			glDeleteBuffers(1, &m_BufferHandle);

			m_BufferHandle = newBuffer;
			m_Capacity = newCapacity;
		}

		void Buffer::SetData(size_t capacity, const void* data)
		{
			if (capacity > m_Capacity)
			{
				Resize(capacity);
				TGE_LOG_WARNING("resizing buffer to bigger capacity");
			}

			glNamedBufferSubData(m_BufferHandle, 0, (GLsizeiptr)capacity, data);
		}

		void Buffer::Copy(const Ref<Buffer>& other)
		{
			glCopyNamedBufferSubData(other->GetHandle(),
				m_BufferHandle, 0, 0, (GLsizeiptr)std::min(other->GetCapacity(), m_Capacity));
		}

		void Buffer::Copy(const Buffer& other)
		{
			glCopyNamedBufferSubData(other.GetHandle(),
				m_BufferHandle, 0, 0, (GLsizeiptr)std::min(other.GetCapacity(), m_Capacity));
		}

		void* Buffer::Map(uint32_t mapOptions)
		{
			TGE_VERIFY(!m_Mapped, "already mapped");
			m_Mapped = true;
			return glMapNamedBuffer(m_BufferHandle, mapOptions);
		}
		void* Buffer::MapRange(uint32_t bufferOptions)
		{
			TGE_VERIFY(!m_Mapped, "already mapped");
			m_Mapped = true;
			return glMapNamedBufferRange(m_BufferHandle, 0, m_Capacity, bufferOptions);
		}
		void Buffer::Unmap()
		{
			TGE_VERIFY(m_Mapped, "not mapped");
			m_Mapped = false;
			glUnmapNamedBuffer(m_BufferHandle);
		}
		void Buffer::BindBase(uint32_t index, BufferType type)
		{
			glBindBufferBase((GLenum)type, index, m_BufferHandle);
		}
	}
}