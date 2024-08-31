#pragma once

#include "Core/SparseSet.h"
#include "GeometryInstanceBuffer.h"

namespace TooGoodEngine {

	class GeometryStorage
	{
	public:
		GeometryStorage() = default;
		~GeometryStorage() = default;

		GeometryID Create(const GeometryInstanceBufferInfo& info);

		GeometryInstanceBuffer& Get(GeometryID id);
		
		void Remove(GeometryID id);
		void PushInstance(GeometryID id, const Instance& instanceData);

		auto begin() { return m_Storage.begin(); }
		auto end()   { return m_Storage.end(); }

	private:
		SparseSet<GeometryInstanceBuffer> m_Storage;
		size_t m_Count = 0;
	};

}