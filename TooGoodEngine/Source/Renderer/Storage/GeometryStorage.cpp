#include "GeometryStorage.h"

namespace TooGoodEngine {

	GeometryID GeometryStorage::Create(const GeometryInstanceBufferInfo& info)
	{
		GeometryID index = m_Count;
		
		m_Storage.Emplace(index, info);
		m_Count++;

		return index;
	}

	GeometryInstanceBuffer& GeometryStorage::Get(GeometryID id)
	{
		static GeometryInstanceBuffer s_NullInstanceBuffer;
		
		if (m_Storage.Contains(id))
			return m_Storage.Get(id);

		TGE_LOG_WARNING("returning null buffer ", id);
		return s_NullInstanceBuffer;
	}

	void GeometryStorage::Remove(GeometryID id)
	{
		m_Storage.Remove(id);
	}

	void GeometryStorage::PushInstance(GeometryID id, const Instance& instanceData)
	{
		if (!m_Storage.Contains(id))
			return;

		auto& buffer = m_Storage.Get(id);
		buffer.Push(instanceData);
	}

}