#pragma once

#include "Storage/MaterialStorage.h"

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
		glm::mat4 Transform;
		uint32_t MaterialIndex;
		glm::uvec3 NotUsed;
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


	struct GeometryUniformBuffer
	{
		int  PointLightSize;
		int  DirectionalLightSize;
		int  HasCubeMap;
		int  NotUsed;
		glm::vec3 CameraPosition;
		float NumberOfMipMaps;
		glm::mat4 ViewProjection;
	};

	struct BloomUniformBuffer
	{
		int   SampleOption;
		int   SourceMip;
		float Threshold;
		float Intensity;
		float FilterRadius;
	};
}