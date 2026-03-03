//
// Created by danii on 18.09.2023.
//

#ifndef DDNET_API_Chat_H
#define DDNET_API_Chat_H

#include "api.h"

// ============ API.Chat Module ============ //
static PyObject* API_Chat_sendLocalMessage(PyObject* self, PyObject* args) {
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

	PythonAPI_GameClient->m_Chat.Echo(pMessage);
	Py_DECREF(pString);
	Py_RETURN_NONE;
}

static PyObject* API_Chat_sendMessage(PyObject* self, PyObject* args) {
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

	PythonAPI_GameClient->m_Chat.SendChat(0, pMessage);
	Py_DECREF(pString);
	Py_RETURN_NONE;
}

static PyMethodDef API_ChatMethods[] = {
	{"send_local_message", API_Chat_sendLocalMessage, METH_VARARGS, "send_local_message(message)"},
	{"send_message", API_Chat_sendMessage, METH_VARARGS, "send_message(message)"},
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
