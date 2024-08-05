#pragma once

#include "Alignment.h"
#include "Material.h"

#include <vector>

namespace TooGoodEngine {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TextureCoordinate;
	};

	struct Instance
	{
		TGE_SCALAR_ALIGNMENT uint32_t MaterialIndex;
		TGE_MATRIX_4X4_ALIGNMENT glm::mat4 Transform;
	};

	//represents the geometry info for a single mesh
	struct Geometry
	{
		std::vector<Vertex>     Vertices;
		std::vector<uint32_t>   Indices; //will have a material soon
	};

	struct PerFrameData
	{
		glm::mat4 ViewProjection;
	};
}