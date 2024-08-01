#pragma once

#include "Core/Base.h"
#include "PythonBase.h"

namespace TooGoodEngine {

	namespace PythonBindings {

		static PyObject* LogMsg(PyObject* self, PyObject* args)
		{
			const char* val;
			if (!PyArg_ParseTuple(args, "s", &val))
				return nullptr;

			TGE_LOG_INFO(val);
			return PyLong_FromUnsignedLongLong(1);
		}

		static PyObject* LogWarn(PyObject* self, PyObject* args)
		{
			const char* val;
			if (!PyArg_ParseTuple(args, "s", &val))
				return nullptr;


			TGE_LOG_WARNING(val);
			return PyLong_FromUnsignedLongLong(1);
		}

		static PyObject* LogError(PyObject* self, PyObject* args)
		{
			const char* val;
			if (!PyArg_ParseTuple(args, "s", &val))
				return nullptr;

			TGE_LOG_ERROR(val);
			return PyLong_FromUnsignedLongLong(1);
		}
	}
}