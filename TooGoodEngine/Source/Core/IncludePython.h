#pragma once

#ifdef DEBUG
	#undef DEBUG
		#include <Python.h>
	#define DEBUG
#else 
	#include <Python.h>
#endif