#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace TooGoodEngine {
	namespace OpenGL {

		enum class DrawMode
		{
			None = 0,
			Triangle = GL_TRIANGLES,
			Point = GL_POINTS,
			TriangleFan = GL_TRIANGLE_FAN,
			TriangleStrip = GL_TRIANGLE_STRIP
		};

		//abstracts the underlying gpu command and takes in program/vertexarray instead of calling bind 
		class Command
		{
		public:
			static void DrawArrays(Program* program, VertexArray* vertexArray, DrawMode mode, uint32_t first, uint32_t count);
			static void DrawElements(Program* program, VertexArray* vertexArray, DrawMode mode, uint32_t count);
			static void DrawElementsInstanced(Program* program, VertexArray* vertexArray, DrawMode mode, uint32_t count, uint32_t instanceCount);
			static void ClearColor(const glm::vec4& color);
			static void ClearDepth();
			static void SetViewport(uint32_t width, uint32_t height);
		};

	}
}