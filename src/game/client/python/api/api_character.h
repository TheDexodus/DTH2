//
// Created by danii on 18.09.2023.
//

#ifndef DDNET_API_CHARACTER_H
#define DDNET_API_CHARACTER_H

#include "Python.h"
#include "game/client/gameclient.h"
#include "api.h"
#include "api_vector2.h"
#include "api_tee.h"

using namespace std;

struct Character {
	PyObject_HEAD;

	int player_flags;
	int health;
	int armor;
	int ammo_count;
	int weapon;
	int emote;
	int attack_tick;

	bool is_active;
	Vector2 position;
};

extern PyTypeObject CharacterType;

static PyMethodDef Character_methods[] = {
	{NULL}  /* Sentinel */
};

static int Character_init(Character *self, PyObject *args, PyObject *kwds)
{
	int id;

	if (!PyArg_ParseTuple(args, "i", &id))
		return -1;

	if (id < 0 || id > 63) {
		PyErr_SetString(PyExc_TypeError, "Expected int argument, with value between 0-63");

		return -1;
	}

	auto snapCharacter = PythonAPI_GameClient->m_Snap.m_aCharacters[id];

	self->player_flags = snapCharacter.m_Cur.m_PlayerFlags;
	self->health = snapCharacter.m_Cur.m_Health;
	self->armor = snapCharacter.m_Cur.m_Armor;
	self->ammo_count = snapCharacter.m_Cur.m_AmmoCount;
	self->weapon = snapCharacter.m_Cur.m_Weapon;
	self->emote = snapCharacter.m_Cur.m_Emote;
	self->attack_tick = snapCharacter.m_Cur.m_AttackTick;

	self->is_active = snapCharacter.m_Active;
	self->position = snapCharacter.m_Position;

	return 0;
}

static void Character_dealloc(Character* self)
{
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* Character_getplayer_flags(Character* self, void* closure)
{
	return Py_BuildValue("i", self->player_flags);
}

static PyObject* Character_gethealth(Character* self, void* closure)
{
	return Py_BuildValue("i", self->health);
}

static PyObject* Character_getarmor(Character* self, void* closure)
{
	return Py_BuildValue("i", self->armor);
}

static PyObject* Character_getammo_count(Character* self, void* closure)
{
	return Py_BuildValue("i", self->ammo_count);
}

static PyObject* Character_getweapon(Character* self, void* closure)
{
	return Py_BuildValue("i", self->weapon);
}

static PyObject* Character_getemote(Character* self, void* closure)
{
	return Py_BuildValue("i", self->emote);
}

static PyObject* Character_getattack_tick(Character* self, void* closure)
{
	return Py_BuildValue("i", self->attack_tick);
}

static PyObject* Character_getis_active(Character* self, void* closure)
{
	return Py_BuildValue("b", self->is_active);
}

static PyObject* Character_getposition(Character* self, void* closure)
{
	Vector2 *vector = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	vector->x = self->position.x;
	vector->y = self->position.y;

	return (PyObject *) vector;
}

static PyGetSetDef Character_getseters[] = {
	{"player_flags", (getter) Character_getplayer_flags, NULL, "player_flags (integer)", NULL},
	{"health", (getter) Character_gethealth, NULL, "health (integer)", NULL},
	{"armor", (getter) Character_getarmor, NULL, "armor (integer)", NULL},
	{"ammo_count", (getter) Character_getammo_count, NULL, "ammo_count (string)", NULL},
	{"weapon", (getter) Character_getweapon, NULL, "weapon (string)", NULL},
	{"emote", (getter) Character_getemote, NULL, "emote (integer)", NULL},
	{"attack_tick", (getter) Character_getattack_tick, NULL, "attack_tick (string)", NULL},
	{"is_active", (getter) Character_getis_active, NULL, "is_active (bool)", NULL},
	{"position", (getter) Character_getposition, NULL, "position (Vector2)", NULL},
	{NULL}  /* Sentinel */
};

inline PyTypeObject CharacterType = {
	{ PyObject_HEAD_INIT(NULL) 0, },
	"API.Character",                /* tp_name */
	sizeof(Character),              /* tp_basicsize */
	0,                            /* tp_itemsize */
	(destructor)Character_dealloc,  /* tp_dealloc */
	0,                            /* tp_print */
	0,                            /* tp_getattr */
	0,                            /* tp_setattr */
	0,                            /* tp_compare */
	0,                            /* tp_repr */
	0,               /* tp_as_number */
	0,                            /* tp_as_sequence */
	0,                            /* tp_as_mapping */
	0,                            /* tp_hash */
	0,                            /* tp_call */
	0,                            /* tp_str */
	0,                            /* tp_getattro */
	0,                            /* tp_setattro */
	0,                            /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,           /* tp_flags */
	"Character",           /* tp_doc */
	0,                            /* tp_traverse */
	0,                            /* tp_clear */
	0,                            /* tp_richcompare */
	0,                            /* tp_weaklistoffset */
	0,                            /* tp_iter */
	0,                            /* tp_iternext */
	Character_methods,              /* tp_methods */
	0,                            /* tp_members */
	Character_getseters,                            /* tp_getset */
	0,                            /* tp_base */
	0,                            /* tp_dict */
	0,                            /* tp_descr_get */
	0,                            /* tp_descr_set */
	0,                            /* tp_dictoffset */
	(initproc)Character_init,      /* tp_init */
	0,                            /* tp_alloc */
	PyType_GenericNew,            /* tp_new */
};

#endif // DDNET_API_CHARACTER_H

