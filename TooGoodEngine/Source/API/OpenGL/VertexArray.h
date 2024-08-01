#pragma once

#include "Buffer.h"

namespace TooGoodEngine {
	namespace OpenGL {
		
		struct VertexArrayInfo
		{

		};

		class VertexArray
		{
		public:
			VertexArray(const VertexArrayInfo& info);
			VertexArray() = default;
			~VertexArray();

			VertexArray(const VertexArray& other);
			VertexArray(VertexArray&& other);

			VertexArray& operator=(const VertexArray& other);
			VertexArray& operator=(VertexArray&& other);


		private:
			uint32_t m_VertexArrayHandle = 0;
		};
	}
}