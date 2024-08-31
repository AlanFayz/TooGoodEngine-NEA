#include "Buffer.h"

namespace TooGoodEngine {
	namespace OpenGL {



		Buffer::Buffer(const BufferInfo& info)
			: m_Capacity(info.Capacity), m_Flags(info.Masks)
		{
			Allocate(info);
		}

		Buffer::~Buffer()
		{
			Release();
		}

		Buffer::Buffer(const Buffer& other)
		{
			if (other.m_Capacity == 0 || other.m_BufferHandle == g_NullBufferID)
			{
				Release();
				return;
			}

			OpenGL::BufferInfo info{};
			info.Capacity = other.m_Capacity;
			info.Masks = other.m_Flags;
			info.Data = nullptr;

			Release();
			Allocate(info);
			Copy(other);
		}

		Buffer& Buffer::operator=(const Buffer& other)
		{
			OpenGL::BufferInfo info{};
			info.Capacity = other.m_Capacity;
			info.Masks = other.m_Flags;
			info.Data = nullptr;

			Release();
			Allocate(info);
			Copy(other);

			return *this;
		}

		Buffer::Buffer(Buffer&& other) noexcept
			: m_BufferHandle(other.m_BufferHandle), 
			  m_Capacity(other.m_Capacity), 
			  m_Flags(other.m_Flags), 
			  m_Mapped(other.m_Mapped) 
		{
			other.m_BufferHandle = g_NullBufferID;
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

				other.m_BufferHandle = g_NullBufferID;
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
		void Buffer::FlushMapRange()
		{
			glFlushMappedNamedBufferRange(m_BufferHandle, 0, (GLsizeiptr)m_Capacity);
		}
		void Buffer::Unmap()
		{
			TGE_VERIFY(m_Mapped, "not mapped");
			m_Mapped = false;
			glUnmapNamedBuffer(m_BufferHandle);
		}
		void Buffer::BindBase(uint32_t index, BufferType type) const
		{
			glBindBufferBase((GLenum)type, index, m_BufferHandle);
		}
		void Buffer::BindRange(uint32_t index, BufferType type, size_t size) const
		{
			glBindBufferRange((GLenum)type, (GLuint)index, m_BufferHandle, 0, (GLsizeiptr)size);
		}
		void Buffer::Allocate(const BufferInfo& info)
		{
			TGE_VERIFY(info.Capacity > 0, "capacity cannot be 0");

			glCreateBuffers(1, &m_BufferHandle);
			glNamedBufferStorage(m_BufferHandle, info.Capacity, info.Data, info.Masks);

			m_Capacity = info.Capacity;
			m_Flags = info.Masks;
		}
		void Buffer::Release()
		{
			if (m_BufferHandle)
			{
				if (m_Mapped)
					Unmap();

				glDeleteBuffers(1, &m_BufferHandle);
				m_BufferHandle = g_NullBufferID;
			}
		}
	}
}