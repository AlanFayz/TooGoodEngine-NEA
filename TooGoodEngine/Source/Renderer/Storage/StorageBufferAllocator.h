#pragma once

#include "API/OpenGL/Buffer.h"
#include <unordered_map>

namespace TooGoodEngine {

	template<typename Type>
	class StorageBufferAllocator
	{
	public:
		using value_type = Type;

	public:
		StorageBufferAllocator() = default;
		~StorageBufferAllocator() = default;

		void deallocate(const Type* ptr, const size_t count)
		{
			if (s_Buffers.contains((const void*)ptr))
			{
				s_Buffers[(const void*)ptr]->Release();
				s_Buffers.erase((const void*)ptr);
			}
		}

		Type* allocate(const size_t count)
		{
			OpenGL::BufferInfo info{};
			info.Capacity = sizeof(Type) * count;
			info.Data = nullptr;
			info.Masks = OpenGL::BufferOptionMapCoherient  |
						 OpenGL::BufferOptionMapPersistent |
						 OpenGL::BufferOptionMapWrite;

			Ref<OpenGL::Buffer> buffer = CreateRef<OpenGL::Buffer>(info);
			Type* ptr = (Type*)buffer->MapRange(info.Masks);

			s_Buffers[(const void*)ptr] = buffer;
			return ptr;
		}

		inline void SubmitBuffer(const Type* ptr, uint32_t index) const
		{
			if(s_Buffers.contains((const void*)ptr))
				s_Buffers[(const void*)ptr]->BindBase(index, OpenGL::BufferTypeShaderStorage);
		}

		template<typename Other>
		struct rebind_alloc
		{
			using type = StorageBufferAllocator<Other>;
		};

	private:
		static inline std::unordered_map<const void*, Ref<OpenGL::Buffer>> s_Buffers;

	};
}