#include "VertexArray.h"

namespace TooGoodEngine {
	namespace OpenGL {

		VertexArray::VertexArray(const VertexArrayInfo& info)
			: m_VertexArrayHandle(0)
		{
			glCreateVertexArrays(1, &m_VertexArrayHandle);
		}
		VertexArray::~VertexArray()
		{
			if (m_VertexArrayHandle)
				glDeleteVertexArrays(1, &m_VertexArrayHandle);
		}
	}
}