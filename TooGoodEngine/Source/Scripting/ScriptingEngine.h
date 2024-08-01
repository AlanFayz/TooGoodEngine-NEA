#pragma once

#include "Bindings.h"
#include "ScriptComponent.h"

#include <array>
#include <filesystem>

namespace TooGoodEngine {

	struct ScriptingEngineData
	{
		static constexpr size_t NumberOfMethods = 3;

		bool Initialized = false;
		PyObject*   EngineModule = nullptr;
		PyModuleDef EngineModuleDefinition{};

		std::array<PyMethodDef, NumberOfMethods> MethodDefinitions{};
	};

	class ScriptingEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static ScriptData ExtractScript(const std::filesystem::path& path);
	};
}