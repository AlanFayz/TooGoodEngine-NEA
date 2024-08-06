#pragma once

#include "Renderer/Renderer.h"

namespace TooGoodEngine {

	struct MeshComponent
	{
		MeshComponent() : ID(g_NullID), PathToSource("") {};
		MeshComponent(GeometryID id, const std::filesystem::path& path) : ID(id), PathToSource(path.string()) {};
		~MeshComponent() = default;

		MeshComponent(const MeshComponent& other)
			: ID(other.ID), PathToSource(other.PathToSource)
		{}

		GeometryID ID;
		std::string PathToSource;
	};

}