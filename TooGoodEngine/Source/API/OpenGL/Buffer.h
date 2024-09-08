#pragma once

#include "External/glad/glad.h"

#include "Core/Base.h"

#include <inttypes.h>

namespace TooGoodEngine {
	namespace OpenGL {

		//enums instead of enum class so we can use the OR operator
		//without creating a bunch of operator overloads

		enum BufferOptions : uint32_t
		{
			BufferOptionNone = 0,
			BufferOptionDynamicStorage   = GL_DYNAMIC_STORAGE_BIT,
			BufferOptionMapWrite		 = GL_MAP_WRITE_BIT,
			BufferOptionMapCoherient	 = GL_MAP_COHERENT_BIT,
			BufferOptionMapPersistent	 = GL_MAP_PERSISTENT_BIT,
			BufferOptionUnsynchronized   = GL_MAP_UNSYNCHRONIZED_BIT,
			BufferOptionInvalidate		 = GL_MAP_INVALIDATE_BUFFER_BIT,
			BufferOptionMapFlushExplicit = GL_MAP_FLUSH_EXPLICIT_BIT
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
			const void* Data;
			size_t Capacity;
			uint32_t Masks;
		};

		//makes debugging easier as buffer ids can be 0, 1, 2. 
		//if it is initialized to a massive number its very obvious in the debugger
		inline constexpr uint32_t g_NullBufferID = std::numeric_limits<uint32_t>::max();

		//a class which manages a block of memory on the GPU, allocating, resizing, modifying etc...
		class Buffer
		{
		public:
			Buffer() = default;
			Buffer(const BufferInfo& info);
			~Buffer();

			Buffer(const Buffer& other);
			Buffer& operator=(const Buffer& other);

			Buffer(Buffer&& other) noexcept;
			Buffer& operator=(Buffer&& other) noexcept;

			inline const uint32_t GetHandle()   const { return m_BufferHandle; }
			inline const size_t   GetCapacity() const { return m_Capacity; }

			void Resize(size_t newCapacity);
			void SetData(size_t capacity, const void* data);

			void Copy(const Ref<Buffer>& other);
			void Copy(const Buffer& other);

			void* Map(uint32_t mapOptions);
			void* MapRange(uint32_t bufferOptions);
			void  FlushMapRange();
			void  Unmap();
			 
			void BindBase(uint32_t index, BufferType type) const;
			void BindRange(uint32_t index, BufferType type, size_t size) const;

			void Allocate(const BufferInfo& info);
			void Release();
			
			inline bool Valid() const { return m_BufferHandle != g_NullBufferID; }
			inline bool Mapped() const { return m_Mapped; }

		private:
			uint32_t m_BufferHandle = g_NullBufferID;
			size_t   m_Capacity = 0;

			uint32_t m_Flags = 0;
			bool m_Mapped = false;
		};

	}
}