#include "Command.h"

namespace TooGoodEngine {
	namespace OpenGL {

		void Command::DrawArrays(Program* program, VertexArray* vertexArray, DrawMode mode, uint32_t first, uint32_t count)
		{
			TGE_VERIFY(vertexArray && program, "vertex array or program was nullptr");
			program->Use();
			vertexArray->Bind();
			glDrawArrays((GLenum)mode, (GLint)first, (GLsizei)count);
		}
		void Command::DrawElements(Program* program, VertexArray* vertexArray, DrawMode mode, uint32_t count)
		{
			TGE_VERIFY(vertexArray && program, "vertex array or program was nullptr");
			program->Use();
			vertexArray->Bind();
			glDrawElements((GLenum)mode, (GLsizei)count, GL_UNSIGNED_INT, nullptr);
		}
		void Command::DrawElementsInstanced(Program* program, VertexArray* vertexArray, DrawMode mode, uint32_t count, uint32_t instanceCount)
		{
			TGE_VERIFY(vertexArray && program, "vertex array or program was nullptr");
			program->Use();
			vertexArray->Bind();
			glDrawElementsInstanced((GLenum)mode, (GLsizei)count, GL_UNSIGNED_INT, nullptr, (GLsizei)instanceCount);
		}
		void Command::ClearColor(const glm::vec4& color)
		{
			glClearColor(color[0], color[1], color[2], color[3]);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		void Command::SetViewport(uint32_t width, uint32_t height)
		{
			glViewport(0, 0, (GLsizei)width, (GLsizei)height);
		}
	}
}