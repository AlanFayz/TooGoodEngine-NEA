#include "GeometryInstanceBuffer.h"

#include <utility>

namespace TooGoodEngine {

	GeometryInstanceBuffer::GeometryInstanceBuffer(const GeometryInstanceBufferInfo& info)
		: m_CurrentInstanceBuffer(0), m_InstanceCount(0), m_DefaultMaterialIndex(info.DefaultMaterialIndex), 
		  m_IndexCount((uint32_t)info.IndexDataSize)
	{
		TGE_VERIFY(info.Program && info.VertexData && info.Indices, "program, vertex or index data was nullptr");

		{
			OpenGL::BufferInfo bufferInfo{};
			bufferInfo.Capacity = info.VertexDataSize * sizeof(Vertex);
			bufferInfo.Data = info.VertexData;

			m_GeometryBuffer = OpenGL::Buffer(bufferInfo);
		}

		{
			OpenGL::BufferInfo bufferInfo{};
			bufferInfo.Capacity = 10 * sizeof(Instance); //default capacity
			bufferInfo.Masks = OpenGL::BufferOptionMapCoherient     |
							   OpenGL::BufferOptionMapPersistent    |
							   OpenGL::BufferOptionMapWrite;

			for (size_t i = 0; i < 3; i++)
				m_InstanceBuffers[i] = OpenGL::Buffer(bufferInfo);

			for (size_t i = 0; i < 3; i++)
				m_MappedData[i] = (Instance*)m_InstanceBuffers[i].MapRange(bufferInfo.Masks);
		}

		{
			OpenGL::BufferInfo bufferInfo{};
			bufferInfo.Capacity = info.IndexDataSize * sizeof(uint32_t);
			bufferInfo.Data = info.Indices;

			m_IndexBuffer = OpenGL::Buffer(bufferInfo);
		}

		m_VertexArray.AttachIndexBuffer(&m_IndexBuffer);
		
		{
			OpenGL::VertexInputMap map
			{ {"Position",    {OpenGL::VertexType::Vector3, false}}, 
			  {"Normal",      {OpenGL::VertexType::Vector3, false}}, 
			  {"TextureCoord",{OpenGL::VertexType::Vector2, false}} };
		
			m_VertexArray.AttachVertexInput(&m_GeometryBuffer, info.Program, map);
		}

	}

	GeometryInstanceBuffer::GeometryInstanceBuffer(GeometryInstanceBuffer&& other) noexcept
	{
		m_VertexArray    = std::move(other.m_VertexArray);
		m_GeometryBuffer = std::move(other.m_GeometryBuffer);
		m_IndexBuffer    = std::move(other.m_IndexBuffer);

		for (size_t i = 0; i < 3; i++)
		{
			m_InstanceBuffers[i] = std::move(other.m_InstanceBuffers[i]);
			m_MappedData[i] = other.m_MappedData[i];
		}

		m_CurrentInstanceBuffer = other.m_CurrentInstanceBuffer;
		m_InstanceCount = other.m_InstanceCount;
		m_DefaultMaterialIndex = other.m_DefaultMaterialIndex;
		m_IndexCount = other.m_IndexCount;
	}

	GeometryInstanceBuffer& GeometryInstanceBuffer::operator=(GeometryInstanceBuffer&& other) noexcept
	{
		m_VertexArray = std::move(other.m_VertexArray);
		m_GeometryBuffer = std::move(other.m_GeometryBuffer);
		m_IndexBuffer = std::move(other.m_IndexBuffer);

		for (size_t i = 0; i < 3; i++)
		{
			m_InstanceBuffers[i] = std::move(other.m_InstanceBuffers[i]);
			m_MappedData[i] = other.m_MappedData[i];
		}

		m_CurrentInstanceBuffer = other.m_CurrentInstanceBuffer;
		m_InstanceCount = other.m_InstanceCount;
		m_DefaultMaterialIndex = other.m_DefaultMaterialIndex;
		m_IndexCount = other.m_IndexCount;

		return *this;
	}


	void GeometryInstanceBuffer::Push(const Instance& instance)
	{
		if (m_InstanceCount * sizeof(Instance) >= m_InstanceBuffers[m_CurrentInstanceBuffer].GetCapacity())
		{
			m_InstanceBuffers[m_CurrentInstanceBuffer].Unmap();
			m_InstanceBuffers[m_CurrentInstanceBuffer].Resize(m_InstanceBuffers[m_CurrentInstanceBuffer].GetCapacity() * 2);
			m_MappedData[m_CurrentInstanceBuffer] = (Instance*)m_InstanceBuffers[m_CurrentInstanceBuffer].MapRange(
				OpenGL::BufferOptionMapCoherient  |
				OpenGL::BufferOptionMapPersistent |
				OpenGL::BufferOptionMapWrite     
			);
		}

		m_MappedData[m_CurrentInstanceBuffer][m_InstanceCount++] = instance;
	}

	void GeometryInstanceBuffer::BeginBatch(uint32_t instanceStorageBufferIndex)
	{
		m_InstanceBuffers[m_CurrentInstanceBuffer].BindBase(instanceStorageBufferIndex, OpenGL::BufferTypeShaderStorage);
	}

	void GeometryInstanceBuffer::EndBatch()
	{
		m_CurrentInstanceBuffer = (m_CurrentInstanceBuffer + 1) % 3;
		m_InstanceCount = 0;
	}

}