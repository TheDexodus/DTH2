//
// Created by danii on 18.09.2023.
//

#ifndef DDNET_API_CONSOLE_H
#define DDNET_API_CONSOLE_H

#include "api.h"
#include "api_vector2.h"

// ============ API.Console Module ============ //
static PyObject* API_Console_debug(PyObject* self, PyObject* args) {
	PyObject *pValue = nullptr;
	if(!PyArg_ParseTuple(args, "O", &pValue))
		return NULL;

	PyObject *pString = PyObject_Str(pValue);
	if(pString == nullptr)
		return NULL;

	const char *pMessage = PyUnicode_AsUTF8(pString);
	if(pMessage == nullptr)
	{
		Py_DECREF(pString);
		return NULL;
	}

	dbg_msg("Python Script", "%s", pMessage);
	Py_DECREF(pString);
	Py_RETURN_NONE;
}

static PyMethodDef API_ConsoleMethods[] = {
	{"debug", API_Console_debug, METH_VARARGS, "Prints a debug message"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef API_ConsoleModule = {
	PyModuleDef_HEAD_INIT,
	"API.Console",
	NULL,
	-1,
	API_ConsoleMethods
};

PyMODINIT_FUNC PyInit_API_Console(void) {
	PyObject* module = PyModule_Create(&API_ConsoleModule);

	return module;
}

#endif // DDNET_API_CONSOLE_H
