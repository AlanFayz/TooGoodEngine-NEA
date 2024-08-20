#pragma once

#include "PythonBase.h"
#include "Core/UUID.h"

#include <corecrt.h>
#include <functional>

namespace TooGoodEngine {
	
	//May contain more information in the future
	struct ScriptData
	{
		PyObject* PyOnCreate;
		PyObject* PyOnUpdate;
	};

	class ScriptComponent
	{
	public:
		ScriptComponent(const ScriptData& data);
		ScriptComponent() = default;
		~ScriptComponent();

		void Create(const ScriptData& data);
		void OnCreate() const;
		void OnUpdate(double delta) const;

		inline void SetHandle(UUID other) { m_AssetHandle = other; }
		inline const UUID GetHandle() const { return m_AssetHandle; }

	private:
		void _PrintDebugInfo();

	private:
		std::function<void()>       m_OnScriptCreate;
		std::function<void(double)> m_OnScriptUpdate;

		PyObject* m_PyOnScriptCreate = nullptr;
		PyObject* m_PyOnScriptUpdate = nullptr;

		UUID m_AssetHandle; //for serialization
	};
}