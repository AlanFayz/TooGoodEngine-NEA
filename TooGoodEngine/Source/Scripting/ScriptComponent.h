#pragma once

#include "PythonBase.h"

#include <corecrt.h>
#include <functional>

namespace TooGoodEngine {
	
	//May contain more information in the future
	struct ScriptData
	{
		PyObject* PyOnCreate;
		PyObject* PyOnUpdate;
	};

	//container for a python script
	class ScriptComponent
	{
	public:
		ScriptComponent(const ScriptData& data);
		ScriptComponent() = default;
		~ScriptComponent();

		void Create(const ScriptData& data);
		void OnCreate();
		void OnUpdate(double delta);

	private:
		void _PrintDebugInfo();

	private:
		std::function<void()>       m_OnScriptCreate;
		std::function<void(double)> m_OnScriptUpdate;

		PyObject* m_PyOnScriptCreate = nullptr;
		PyObject* m_PyOnScriptUpdate = nullptr;
	};
}