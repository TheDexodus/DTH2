#ifndef DDNET_API_CLAN_H
#define DDNET_API_CLAN_H

#include "api.h"

// ============ API.Clan Module ============ //
static PyObject *API_Clan_getMembers(PyObject *self, PyObject *args)
{
	(void)self;
	(void)args;

	const auto &Members = PythonAPI_GameClient->dthDatabase.members;
	PyObject *pTypesModule = PyImport_ImportModule("types");
	if(!pTypesModule)
	{
		return NULL;
	}

	PyObject *pSimpleNamespaceType = PyObject_GetAttrString(pTypesModule, "SimpleNamespace");
	Py_DECREF(pTypesModule);
	if(!pSimpleNamespaceType)
	{
		return NULL;
	}

	PyObject *pEmptyArgs = PyTuple_New(0);
	if(!pEmptyArgs)
	{
		Py_DECREF(pSimpleNamespaceType);
		return NULL;
	}

	PyObject *pMembersList = PyList_New((Py_ssize_t)Members.size());
	if(!pMembersList)
	{
		Py_DECREF(pEmptyArgs);
		Py_DECREF(pSimpleNamespaceType);
		return NULL;
	}

	for(size_t i = 0; i < Members.size(); i++)
	{
		const auto &Member = Members[i];

		PyObject *pSkinKwargs = PyDict_New();
		PyObject *pMemberKwargs = PyDict_New();

		PyObject *pName = PyUnicode_FromString(Member.name.c_str());
		PyObject *pRole = PyUnicode_FromString(Member.role.c_str());

		PyObject *pSkinName = PyUnicode_FromString(Member.skin.name.c_str());
		PyObject *pSkinColor = PyLong_FromLong(Member.skin.skinColor);
		PyObject *pFootsColor = PyLong_FromLong(Member.skin.footsColor);
		PyObject *pCustomColors = PyBool_FromLong(Member.skin.customColors ? 1 : 0);

		PyDict_SetItemString(pSkinKwargs, "name", pSkinName);
		PyDict_SetItemString(pSkinKwargs, "skin_color", pSkinColor);
		PyDict_SetItemString(pSkinKwargs, "foots_color", pFootsColor);
		PyDict_SetItemString(pSkinKwargs, "custom_colors", pCustomColors);

		Py_DECREF(pSkinName);
		Py_DECREF(pSkinColor);
		Py_DECREF(pFootsColor);
		Py_DECREF(pCustomColors);

		PyObject *pSkinObject = PyObject_Call(pSimpleNamespaceType, pEmptyArgs, pSkinKwargs);
		Py_DECREF(pSkinKwargs);
		if(!pSkinObject)
		{
			Py_DECREF(pName);
			Py_DECREF(pRole);
			Py_DECREF(pMemberKwargs);
			Py_DECREF(pMembersList);
			Py_DECREF(pEmptyArgs);
			Py_DECREF(pSimpleNamespaceType);
			return NULL;
		}

		PyDict_SetItemString(pMemberKwargs, "name", pName);
		PyDict_SetItemString(pMemberKwargs, "role", pRole);
		PyDict_SetItemString(pMemberKwargs, "skin", pSkinObject);

		Py_DECREF(pName);
		Py_DECREF(pRole);
		Py_DECREF(pSkinObject);

		PyObject *pMemberObject = PyObject_Call(pSimpleNamespaceType, pEmptyArgs, pMemberKwargs);
		Py_DECREF(pMemberKwargs);
		if(!pMemberObject)
		{
			Py_DECREF(pMembersList);
			Py_DECREF(pEmptyArgs);
			Py_DECREF(pSimpleNamespaceType);
			return NULL;
		}

		PyList_SET_ITEM(pMembersList, (Py_ssize_t)i, pMemberObject);
	}

	Py_DECREF(pEmptyArgs);
	Py_DECREF(pSimpleNamespaceType);
	return pMembersList;
}

static PyObject *API_Clan_getPlayers(PyObject *self, PyObject *args)
{
	(void)self;
	(void)args;

	const auto &Players = PythonAPI_GameClient->dthDatabase.players;
	PyObject *pTypesModule = PyImport_ImportModule("types");
	if(!pTypesModule)
	{
		return NULL;
	}

	PyObject *pSimpleNamespaceType = PyObject_GetAttrString(pTypesModule, "SimpleNamespace");
	Py_DECREF(pTypesModule);
	if(!pSimpleNamespaceType)
	{
		return NULL;
	}

	PyObject *pEmptyArgs = PyTuple_New(0);
	if(!pEmptyArgs)
	{
		Py_DECREF(pSimpleNamespaceType);
		return NULL;
	}

	PyObject *pPlayersList = PyList_New((Py_ssize_t)Players.size());
	if(!pPlayersList)
	{
		Py_DECREF(pEmptyArgs);
		Py_DECREF(pSimpleNamespaceType);
		return NULL;
	}

	for(size_t i = 0; i < Players.size(); i++)
	{
		const auto &Player = Players[i];
		PyObject *pPlayerKwargs = PyDict_New();

		PyObject *pName = PyUnicode_FromString(Player.name.c_str());
		PyObject *pStatus = PyUnicode_FromString(Player.status.c_str());

		PyDict_SetItemString(pPlayerKwargs, "name", pName);
		PyDict_SetItemString(pPlayerKwargs, "status", pStatus);

		Py_DECREF(pName);
		Py_DECREF(pStatus);

		PyObject *pPlayerObject = PyObject_Call(pSimpleNamespaceType, pEmptyArgs, pPlayerKwargs);
		Py_DECREF(pPlayerKwargs);
		if(!pPlayerObject)
		{
			Py_DECREF(pPlayersList);
			Py_DECREF(pEmptyArgs);
			Py_DECREF(pSimpleNamespaceType);
			return NULL;
		}

		PyList_SET_ITEM(pPlayersList, (Py_ssize_t)i, pPlayerObject);
	}

	Py_DECREF(pEmptyArgs);
	Py_DECREF(pSimpleNamespaceType);
	return pPlayersList;
}

static PyMethodDef API_ClanMethods[] = {
	{"get_members", API_Clan_getMembers, METH_NOARGS, "get_members() -> list"},
	{"get_players", API_Clan_getPlayers, METH_NOARGS, "get_players() -> list"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef API_ClanModule = {
	PyModuleDef_HEAD_INIT,
	"API.Clan",
	NULL,
	-1,
	API_ClanMethods
};

PyMODINIT_FUNC PyInit_API_Clan(void)
{
	return PyModule_Create(&API_ClanModule);
}

#endif // DDNET_API_CLAN_H

