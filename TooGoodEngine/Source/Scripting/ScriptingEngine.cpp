#include "ScriptingEngine.h"

namespace TooGoodEngine {

	static ScriptingEngineData s_Data;

	void ScriptingEngine::Init()
	{
		Py_Initialize();

		std::filesystem::path currentDirectory = std::filesystem::current_path();

		PyObject* sysModule = PyImport_ImportModule("sys");
		PyObject* sysPath   = PyObject_GetAttrString(sysModule, "path");

		std::string stringCurrentDirectory = currentDirectory.string();
		PyList_Append(sysPath, PyUnicode_FromString(stringCurrentDirectory.c_str()));

		Py_XDECREF(sysPath);
		Py_XDECREF(sysModule);

		s_Data.MethodDefinitions[0] = { "log",         PythonBindings::LogMsg,   METH_VARARGS,   "prints to console"};
		s_Data.MethodDefinitions[1] = { "log_error",   PythonBindings::LogError, METH_VARARGS,   "prints error to console" };
		s_Data.MethodDefinitions[2] = { "log_warning", PythonBindings::LogWarn,  METH_VARARGS,   "prints warning to console" };

		s_Data.EngineModuleDefinition.m_base    = PyModuleDef_HEAD_INIT;
		s_Data.EngineModuleDefinition.m_name    = "TooGoodEngine";
		s_Data.EngineModuleDefinition.m_doc     = nullptr;
		s_Data.EngineModuleDefinition.m_size    = -1;
		s_Data.EngineModuleDefinition.m_methods = s_Data.MethodDefinitions.data();

		s_Data.EngineModule = PyModule_Create(&s_Data.EngineModuleDefinition);
		
		PyObject* moduleDict = PyImport_GetModuleDict();
		PyDict_SetItemString(moduleDict, "TooGoodEngine", s_Data.EngineModule);
		Py_XDECREF(moduleDict);

		s_Data.Initialized = true;

	}


	void ScriptingEngine::Shutdown()
	{
		Py_XDECREF(s_Data.EngineModule);
		Py_Finalize();
	}
	ScriptData ScriptingEngine::ExtractScript(const std::filesystem::path& path)
	{
		ScriptData data{};

		std::filesystem::path stem = path.stem();
		std::string name = stem.string();

		const char* p = name.c_str();
		PyObject* module = PyImport_ImportModule(p);

		if (!module)
		{
			TGE_LOG_ERROR("failed to load module ", path);
			PyErr_Print();
			return data;
		}

		PyObject* pyOnCreate = PyObject_GetAttrString(module, "on_create");
		PyObject* pyOnUpdate = PyObject_GetAttrString(module, "on_update");

		//ownership is passed onto the user (Script Component)
		if (pyOnCreate && PyCallable_Check(pyOnCreate))
			data.PyOnCreate = pyOnCreate;
		else
			PyErr_Print();

		if (pyOnUpdate && PyCallable_Check(pyOnUpdate))
			data.PyOnUpdate = pyOnUpdate;
		else
			PyErr_Print();

		Py_XDECREF(module);
		return data;
	}
	ScriptData ScriptingEngine::CreateModuleFromData(const char* name, const char* data)
	{
		ScriptData scriptData{};

		//TODO:

		return scriptData;
	}
}