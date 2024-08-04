#include "Command.h"

namespace TooGoodEngine {
	namespace OpenGL {

		void Command::DrawArrays(VertexArray* vertexArray, DrawMode mode, uint32_t first, uint32_t count)
		{
			TGE_VERIFY(vertexArray, "vertex array was nullptr");
			vertexArray->Bind();
			glDrawArrays((GLenum)mode, (GLint)first, (GLsizei)count);
		}
		void Command::DrawElements(VertexArray* vertexArray, DrawMode mode, uint32_t count)
		{
			TGE_VERIFY(vertexArray, "vertex array was nullptr");
			vertexArray->Bind();
			glDrawElements((GLenum)mode, (GLsizei)count, GL_UNSIGNED_INT, nullptr);
		}
		void Command::DrawElementsInstanced(VertexArray* vertexArray, DrawMode mode, uint32_t count, uint32_t instanceCount)
		{
			TGE_VERIFY(vertexArray, "vertex array was nullptr");
			vertexArray->Bind();
			glDrawElementsInstanced((GLenum)mode, (GLsizei)count, GL_UNSIGNED_INT, nullptr, (GLsizei)instanceCount);
		}
	}
}