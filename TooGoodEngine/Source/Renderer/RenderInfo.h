#pragma once

#include "Alignment.h"
#include "Material.h"

#include <vector>

namespace TooGoodEngine {

	using GeometryID = size_t;
	using MaterialID = size_t;

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
		std::vector<uint32_t>   Indices; 

		MaterialInfo Material;
	};

	struct PerFrameData
	{
		glm::mat4 ViewProjection;
	};

	struct PointLight
	{
		glm::vec4 PositionAndIntensity;
		glm::vec4 ColorAndRadius;
	};

	struct DirectionalLight
	{
		glm::vec4 DirectionAndIntensity;
		glm::vec4 Color;
	};

	struct ModelInfo
	{
		GeometryID ID;
		size_t Size;
		std::vector<MaterialID> CustomMaterials; //can leave blank and will use default. 
	};


	
}