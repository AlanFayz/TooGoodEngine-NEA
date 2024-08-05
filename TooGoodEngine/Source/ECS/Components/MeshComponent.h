#pragma once

#include "Renderer/Renderer.h"

namespace TooGoodEngine {

	struct MeshComponent
	{
		MeshComponent() = default;
		MeshComponent(GeometryID id) : ID(id) {};
		~MeshComponent() = default;

		GeometryID ID;
	};

}