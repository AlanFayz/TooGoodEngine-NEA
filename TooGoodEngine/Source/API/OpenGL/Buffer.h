#pragma once

#include "External/glad/glad.h"

#include "Core/Base.h"

#include <inttypes.h>

namespace TooGoodEngine {
	namespace OpenGL {

		enum BufferOptions : uint32_t
		{
			BufferOptionNone = 0,
			BufferOptionDynamicStorage  = GL_DYNAMIC_STORAGE_BIT,
			BufferOptionMapWrite        = GL_MAP_WRITE_BIT,
			BufferOptionMapCoherient    = GL_MAP_COHERENT_BIT,
			BufferOptionMapPersistent   = GL_MAP_PERSISTENT_BIT,
			BufferOptionUnsynchronized  = GL_MAP_UNSYNCHRONIZED_BIT,
			BufferOptionInvalidate      = GL_MAP_INVALIDATE_BUFFER_BIT
		};


		enum BufferMapOptions : uint32_t
		{
			BufferMapNone = 0,
			BufferMapWrite        = GL_WRITE_ONLY,
			BufferMapRead		  = GL_READ_ONLY,
			BufferMapReadAndWrite = GL_READ_WRITE
		};

		enum BufferType : uint32_t
		{
			BufferTypeNone = 0,
			BufferTypeArray         = GL_ARRAY_BUFFER,
			BufferTypeIndex         = GL_ELEMENT_ARRAY_BUFFER,
			BufferTypeUniform       = GL_UNIFORM_BUFFER,
			BufferTypeShaderStorage = GL_SHADER_STORAGE_BUFFER
		};

		struct BufferInfo
		{
			void* Data;
			size_t Capacity;
			uint32_t Masks;
		};

		class Buffer
		{
		public:
			Buffer() = default;
			Buffer(const BufferInfo& info);
			~Buffer();

			Buffer(const Buffer& other);
			Buffer(Buffer&& other) noexcept;

			Buffer& operator=(const Buffer& other);
			Buffer& operator=(Buffer&& other) noexcept;

			inline const uint32_t GetHandle()   const { return m_BufferHandle; }
			inline const size_t   GetCapacity() const { return m_Capacity; }

			void Resize(size_t newCapacity);
			void SetData(size_t capacity, void* data);
			
			void Copy(const Ref<Buffer>& other);
			void Copy(const Buffer& other);

			void* Map(uint32_t mapOptions);
			void* MapRange(uint32_t bufferOptions);
			void  Unmap();
			 
			void BindBase(uint32_t index, BufferType type);
			
		private:
			uint32_t m_BufferHandle = 0;
			size_t   m_Capacity;

			uint32_t m_Flags;
			bool m_Mapped;
		};

	}
}