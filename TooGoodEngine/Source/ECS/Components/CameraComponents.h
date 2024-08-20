#pragma once

#include "Math/OrthographicCamera.h"
#include "Math/PerspectiveCamera.h"

namespace TooGoodEngine {

	//scene will search through ortho and perspective camera components
	//to find a camera component that should be "in use"
	//note if you set  multiple camera components InUse then the first one it finds
	//will be the one it uses

	struct OrthographicCameraComponent
	{
		OrthographicCamera Camera;
		bool InUse = true; 
		OrthographicCameraData data;
	};

	struct PerspectiveCameraComponent
	{
		PerspectiveCamera Camera;
		bool InUse = true;
		PerspectiveCameraData data;
	};

}