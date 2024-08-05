#pragma once

#include "API/OpenGL/VertexArray.h"
#include "API/OpenGL/Program.h"

#include "RenderInfo.h"

#include <glm/glm.hpp>
#include <vector>

namespace TooGoodEngine {

	struct GeometryInstanceBufferInfo
	{
		OpenGL::Program* Program;

		const Vertex* VertexData;
		size_t VertexDataSize;

		const uint32_t* Indices;
		size_t IndexDataSize;

		uint32_t DefaultMaterialIndex = 0;
	};

	class GeometryInstanceBuffer
	{
	public:
		GeometryInstanceBuffer() = default;
		~GeometryInstanceBuffer() = default;

		GeometryInstanceBuffer(const GeometryInstanceBufferInfo& info);
		GeometryInstanceBuffer(GeometryInstanceBuffer&& other) noexcept;

		void Push(const Instance& instance);
		void BeginBatch(uint32_t instanceStorageBufferIndex);
		void EndBatch();

		inline const uint32_t GetInstanceCount() const { return m_InstanceCount; }
		inline const uint32_t GetIndexCount() const { return m_IndexCount; }
		inline const uint32_t GetDefaultMaterialIndex() const { return m_DefaultMaterialIndex; }
		
		inline OpenGL::VertexArray* GetVertexArrayPointer() { return &m_VertexArray; }

	private:
		OpenGL::VertexArray m_VertexArray;
		OpenGL::Buffer  m_GeometryBuffer;
		OpenGL::Buffer  m_IndexBuffer;

		Instance* m_MappedData[3];
		OpenGL::Buffer m_InstanceBuffers[3];
		uint32_t m_CurrentInstanceBuffer;

		uint32_t m_InstanceCount;
		uint32_t m_DefaultMaterialIndex;
		uint32_t m_IndexCount;
	};

}