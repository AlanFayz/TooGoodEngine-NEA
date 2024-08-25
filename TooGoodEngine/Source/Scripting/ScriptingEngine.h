#pragma once

#include "Bindings.h"
#include "ScriptComponent.h"

#include <array>
#include <filesystem>

namespace TooGoodEngine {

	struct ScriptingEngineData
	{
		static constexpr size_t MethodCount = 53;
		static constexpr const char* Delimiter = ".";
		static constexpr size_t DelimiterSize = 1;

		bool Initialized = false;
		PyObject*   EngineModule = nullptr;
		PyModuleDef EngineModuleDefinition;
		std::filesystem::path ScriptDirectory;

		std::array<PyMethodDef, MethodCount> MethodDefinitions{};
	};

	class ScriptingEngine
	{
	public:
		static void Init(const std::filesystem::path& path);
		static void Shutdown();

		static ScriptData ExtractScript(const std::filesystem::path& path);
		static ScriptData CreateModuleFromData(const char* name, const char* data);
	};
}