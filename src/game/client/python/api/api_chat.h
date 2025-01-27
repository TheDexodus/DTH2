//
// Created by danii on 18.09.2023.
//

#ifndef DDNET_API_Chat_H
#define DDNET_API_Chat_H

#include "api.h"

// ============ API.Chat Module ============ //
static PyObject* API_Chat_sendLocalMessage(PyObject* self, PyObject* args) {
	char* message;
	PyArg_ParseTuple(args, "s", &message);

	PythonAPI_GameClient->m_Chat.Echo(message);
	Py_RETURN_NONE;
}

static PyMethodDef API_ChatMethods[] = {
	{"sendLocalMessage", API_Chat_sendLocalMessage, METH_VARARGS, "Prints a local message in chat"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef API_ChatModule = {
	PyModuleDef_HEAD_INIT,
	"API.Chat",
	NULL,
	-1,
	API_ChatMethods
};

PyMODINIT_FUNC PyInit_API_Chat(void) {
	PyObject* module = PyModule_Create(&API_ChatModule);

	return module;
}

#endif // DDNET_API_Chat_H
