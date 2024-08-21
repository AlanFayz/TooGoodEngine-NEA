#pragma once

#include "Core/Base.h"
#include "PythonBase.h"

namespace TooGoodEngine {

	class PythonBindings
	{
	public:

		//
		// ---- user api ----
		//
		static PyObject* LogMsg(PyObject* self, PyObject* args);
		static PyObject* LogWarn(PyObject* self, PyObject* args);
		static PyObject* LogError(PyObject* self, PyObject* args);

		static PyObject* IsKeyPressed(PyObject* self, PyObject* args);
		static PyObject* IsKeyReleased(PyObject* self, PyObject* args);
		static PyObject* IsKeyDown(PyObject* self, PyObject* args);

		static PyObject* IsMouseButtonPressed(PyObject* self, PyObject* args);
		static PyObject* IsMouseButtonReleased(PyObject* self, PyObject* args);
		static PyObject* IsMouseButtonDown(PyObject* self, PyObject* args);

		static PyObject* GetMouseCoordinates(PyObject* self, PyObject* args);
		static PyObject* GetScrollWheel(PyObject* self, PyObject* args);
		
		static PyObject* DisableCursor(PyObject* self, PyObject* args);
		static PyObject* EnableCursor(PyObject* self, PyObject* args);

		// 
		// ---- internal ----
		//

		static void      InternalCleanEntity(PyObject* capsule);
		static PyObject* InternalCreateEntity(PyObject* self, PyObject* args);
		static PyObject* InternalGetEntityByName(PyObject* self, PyObject* args);
		static PyObject* InternalGetEntityName(PyObject* self, PyObject* args);
		static PyObject* InternalGetEntityID(PyObject* self, PyObject* args);
		static PyObject* InternalAddComponentToEntity(PyObject* self, PyObject* args);
		static PyObject* InternalRemoveComponentFromEntity(PyObject* self, PyObject* args);
		static PyObject* InternalRemoveEntityFromTree(PyObject* self, PyObject* args);
		static PyObject* InternalGetComponentFromEntity(PyObject* self, PyObject* args);

		static void      InternalCleanTransform(PyObject* capsule);
		static PyObject* InternalTranslateTransform(PyObject* self, PyObject* args);
		static PyObject* InternalRotateTransform(PyObject* self, PyObject* args);
		static PyObject* InternalScaleTransform(PyObject* self, PyObject* args);
		static PyObject* InternalGetTranslationFromTransform(PyObject* self, PyObject* args);
		static PyObject* InternalGetRotationFromTransform(PyObject* self, PyObject* args);
		static PyObject* InternalGetScaleFromTransform(PyObject* self, PyObject* args);

		static void		 InternalCleanMaterial(PyObject* capsule);
		static PyObject* InternalUpdateMaterialAttribute(PyObject* self, PyObject* args);
		static PyObject* InternalGetMaterialAttribute(PyObject* self, PyObject* args);
		//static PyObject* InternalUpdateMaterialTexture(PyObject* self, PyObject* args); TODO once bindings for images have been added

		static void		 InternalCleanPointLight(PyObject* capsule);
		static PyObject* InternalTranslatePointLight(PyObject* self, PyObject* args);
		static PyObject* InternalUpdatePointLightColor(PyObject* self, PyObject* args);
		static PyObject* InternalUpdatePointLightIntensity(PyObject* self, PyObject* args);
		static PyObject* InternalUpdatePointLightRadius(PyObject* self, PyObject* args);

		static void      InternalCleanDirectionalLight(PyObject* capsule);
		static PyObject* InternalUpdateDirectionalLightDirection(PyObject* self, PyObject* args);
		static PyObject* InternalUpdateDirectionalLightColor(PyObject* self, PyObject* args);
		static PyObject* InternalUpdateDirectionalLightIntensity(PyObject* self, PyObject* args);

		static void      InternalCleanOrthographicCameraController(PyObject* capsule);
		static PyObject* InternalCreateOrthographicCameraController(PyObject* self, PyObject* args);
		static PyObject* InternalSetBottomOrthographicCameraController(PyObject* self, PyObject* args);
		static PyObject* InternalSetTopOrthographicCameraController(PyObject* self, PyObject* args);
		static PyObject* InternalSetLeftOrthographicCameraController(PyObject* self, PyObject* args);
		static PyObject* InternalSetRightOrthographicCameraController(PyObject* self, PyObject* args);
		static PyObject* InternalUpdatePositionOrthographicCameraController(PyObject* self, PyObject* args);
		static PyObject* InternalUpdateUpOrthographicCameraController(PyObject* self, PyObject* args);
		static PyObject* InternalUpdateFrontOrthographicCameraController(PyObject* self, PyObject* args);

	};	


}