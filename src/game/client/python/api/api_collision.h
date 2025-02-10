//
// Created by danii on 18.09.2023.
//

#ifndef DDNET_API_COLLISION_H
#define DDNET_API_COLLISION_H

#include "api.h"
#include "api_vector2.h"

// ============ API.Collision Module ============ //
static PyObject* API_Collision_IntersectLine(PyObject* self, PyObject* args) {
	Vector2 *position0;
	Vector2 *position1;

	if (!PyArg_ParseTuple(args, "O!O!", &Vector2Type, &position0, &Vector2Type, &position1))
		return NULL;

	vec2 position0Vec2 = vec2((float) position0->x, (float) position0->y);
	vec2 position1Vec2 = vec2((float) position1->x, (float) position1->y);

	vec2 outCollisionVec2;
	vec2 outBeforeCollisionVec2;

	int tileId = PythonAPI_GameClient->Collision()->IntersectLine(position0Vec2, position1Vec2, &outCollisionVec2, &outBeforeCollisionVec2);

	Vector2 *outCollision = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	outCollision->x = outCollisionVec2.x;
	outCollision->y = outCollisionVec2.y;

	Vector2 *outBeforeCollision = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	outBeforeCollision->x = outBeforeCollisionVec2.x;
	outBeforeCollision->y = outBeforeCollisionVec2.y;

	return Py_BuildValue("iOO", tileId, outCollision, outBeforeCollision);
}
static PyObject* API_Collision_IntersectLineTeleHook(PyObject* self, PyObject* args) {
	Vector2 *position0;
	Vector2 *position1;

	if (!PyArg_ParseTuple(args, "O!O!", &Vector2Type, &position0, &Vector2Type, &position1))
		return NULL;

	vec2 position0Vec2 = vec2((float) position0->x, (float) position0->y);
	vec2 position1Vec2 = vec2((float) position1->x, (float) position1->y);

	vec2 outCollisionVec2;
	vec2 outBeforeCollisionVec2;

	int tileId = PythonAPI_GameClient->Collision()->IntersectLineTeleHook(position0Vec2, position1Vec2, &outCollisionVec2, &outBeforeCollisionVec2);

	Vector2 *outCollision = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	outCollision->x = outCollisionVec2.x;
	outCollision->y = outCollisionVec2.y;

	Vector2 *outBeforeCollision = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	outBeforeCollision->x = outBeforeCollisionVec2.x;
	outBeforeCollision->y = outBeforeCollisionVec2.y;

	return Py_BuildValue("iOO", tileId, outCollision, outBeforeCollision);
}

static PyObject* API_Collision_GetTile(PyObject* self, PyObject* args)
{
	Vector2 *position;

	if (!PyArg_ParseTuple(args, "O!", &Vector2Type, &position))
		return NULL;

	return Py_BuildValue("i", PythonAPI_GameClient->Collision()->GetTileIndex(PythonAPI_GameClient->Collision()->GetPureMapIndex(position->x, position->y)));
}

static PyObject* API_Collision_GetMapSize(PyObject* self, PyObject* args)
{
	Vector2* mapSize = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	mapSize->x = PythonAPI_GameClient->Collision()->GetWidth();
	mapSize->y = PythonAPI_GameClient->Collision()->GetHeight();

	return (PyObject*) mapSize;
}

static PyMethodDef API_CollisionMethods[] = {
	{"intersectLine", API_Collision_IntersectLine, METH_VARARGS, "Intersect Line"},
	{"intersectLineTeleHook", API_Collision_IntersectLineTeleHook, METH_VARARGS, "Intersect Line Tele Hook"},
	{"getTile", API_Collision_GetTile, METH_VARARGS, "Get Tile"},
	{"getMapSize", API_Collision_GetMapSize, METH_VARARGS, "Return Vector2 that is map size"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef API_CollisionModule = {
	PyModuleDef_HEAD_INIT,
	"API.Collision",
	NULL,
	-1,
	API_CollisionMethods
};

PyMODINIT_FUNC PyInit_API_Collision(void) {
	PyObject* module = PyModule_Create(&API_CollisionModule);

	return module;
}

#endif // DDNET_API_COLLISION_H
