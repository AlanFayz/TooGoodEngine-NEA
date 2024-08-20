#include "ScriptComponent.h"

#include "Core/Base.h"

#include <sstream>

namespace TooGoodEngine {

	ScriptComponent::ScriptComponent(const ScriptData& data)
		: m_PyOnScriptCreate(nullptr), m_PyOnScriptUpdate(nullptr)
	{
		Create(data);
	}

	ScriptComponent::~ScriptComponent()
	{
		Py_XDECREF(m_PyOnScriptCreate);
		Py_XDECREF(m_PyOnScriptUpdate);
	}

	ScriptComponent::ScriptComponent(const ScriptComponent& other)
		: m_PyOnScriptCreate(other.m_PyOnScriptCreate), m_PyOnScriptUpdate(other.m_PyOnScriptUpdate),
		m_OnScriptCreate(other.m_OnScriptCreate), m_OnScriptUpdate(other.m_OnScriptUpdate), 
		m_AssetHandle(other.m_AssetHandle)
	{
		m_OnScriptCreate = [this]()
			{
				PyObject* success = PyObject_CallObject(m_PyOnScriptCreate, nullptr);

				if (!success)
					_PrintDebugInfo();
			};

		m_OnScriptUpdate = [this](double delta)
			{
				PyObject* args = PyTuple_New(1);
				PyObject* pyFloat = PyFloat_FromDouble(delta);

				PyTuple_SetItem(args, 0, pyFloat);

				PyObject* success = PyObject_CallObject(m_PyOnScriptUpdate, args);

				if (!success)
					_PrintDebugInfo();

				Py_XDECREF(args);
				Py_XDECREF(pyFloat);
			};


		Py_XINCREF(m_PyOnScriptCreate);
		Py_XINCREF(m_PyOnScriptUpdate);
	}

	ScriptComponent& ScriptComponent::operator=(const ScriptComponent& other)
	{
		if (this != &other)
		{
			m_PyOnScriptCreate = other.m_PyOnScriptCreate;
			m_PyOnScriptUpdate = other.m_PyOnScriptUpdate;
		
			m_OnScriptCreate = [this]()
				{
					PyObject* success = PyObject_CallObject(m_PyOnScriptCreate, nullptr);

					if (!success)
						_PrintDebugInfo();
				};

			m_OnScriptUpdate = [this](double delta)
				{
					PyObject* args = PyTuple_New(1);
					PyObject* pyFloat = PyFloat_FromDouble(delta);

					PyTuple_SetItem(args, 0, pyFloat);

					PyObject* success = PyObject_CallObject(m_PyOnScriptUpdate, args);

					if (!success)
						_PrintDebugInfo();

					Py_XDECREF(args);
					Py_XDECREF(pyFloat);
				};

			m_AssetHandle = other.m_AssetHandle;

			Py_XINCREF(m_PyOnScriptCreate);
			Py_XINCREF(m_PyOnScriptUpdate);
		}

		return *this;
	}


	void ScriptComponent::Create(const ScriptData& data)
	{
		m_PyOnScriptCreate = data.PyOnCreate;
		m_PyOnScriptUpdate = data.PyOnUpdate;

		TGE_VERIFY(m_PyOnScriptCreate && m_PyOnScriptUpdate, "scripts must be valid");

		m_OnScriptCreate = [this]()
			{
				PyObject* success = PyObject_CallObject(m_PyOnScriptCreate, nullptr);

				if (!success)
					_PrintDebugInfo();
			};

		m_OnScriptUpdate = [this](double delta)
			{
				PyObject* args    = PyTuple_New(1);
				PyObject* pyFloat = PyFloat_FromDouble(delta);

				PyTuple_SetItem(args, 0, pyFloat);

				PyObject* success = PyObject_CallObject(m_PyOnScriptUpdate, args);

				if (!success)
					_PrintDebugInfo();

				Py_XDECREF(args);
				Py_XDECREF(pyFloat);
			};

	}

	void ScriptComponent::OnCreate() const
	{
		m_OnScriptCreate();
	}

	void ScriptComponent::OnUpdate(double delta) const
	{
		m_OnScriptUpdate(delta);
	}

	void ScriptComponent::_PrintDebugInfo()
	{
		PyErr_Print(); //provides more useful information than below currently

		//PyObject* type      = nullptr; 
		//PyObject* value     = nullptr;
		//PyObject* traceback = nullptr;

		//PyErr_Fetch(&type, &value, &traceback);
		//
		//std::string errorMessage;

		//if (type)
		//{
		//	PyObject* pyString = PyObject_Str(type);
		//	const char* string = PyUnicode_AsUTF8(pyString);

		//	errorMessage += "Type: " + std::string(string) + "\n";

		//	Py_XDECREF(pyString);
		//}

		//if (value)
		//{
		//	PyObject* pyString = PyObject_Str(value);
		//	const char* string = PyUnicode_AsUTF8(pyString);

		//	errorMessage += "Value: " + std::string(string) + "\n";

		//	Py_XDECREF(pyString);
		//}

		//if (traceback)
		//{
		//	PyObject* pyString = PyObject_Str(traceback);
		//	const char* string = PyUnicode_AsUTF8(pyString);

		//	errorMessage += "Traceback: " + std::string(string);

		//	Py_XDECREF(pyString);
		//}

		//if (!errorMessage.empty())
		//	TGE_LOG_ERROR(errorMessage);

		//Py_XDECREF(type);
		//Py_XDECREF(value);
		//Py_XDECREF(traceback);

		//PyErr_Clear();
	}
}