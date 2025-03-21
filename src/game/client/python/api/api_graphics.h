//
// Created by danii on 09.01.2025.
//

#ifndef DDNET_API_GRAPHICS_H
#define DDNET_API_GRAPHICS_H

#include "api.h"
#include "api_vector2.h"

static PyObject* API_Graphics_drawCircle(PyObject* self, PyObject* args)
{
	Vector2 *position;
	PyObject *pyCircleRadius = nullptr;
	unsigned int color = 0xffffffff;

	if (!PyArg_ParseTuple(args, "O!O|I", &Vector2Type, &position, &pyCircleRadius, &color))
		return NULL;

	if (pyCircleRadius == nullptr || !PyFloat_Check(pyCircleRadius)) {
		PyErr_SetString(PyExc_TypeError, "`circleRadius` must be a float");
		return NULL;
	}

	return Py_BuildValue("i", PythonAPI_GameClient->pythonRender.DrawCircle(position->toVec2(), PyFloat_AsDouble(pyCircleRadius), color));
}

static PyObject* API_Graphics_drawLine(PyObject* self, PyObject* args)
{
	Vector2 *from;
	Vector2 *to;
	unsigned int color = 0xffffffff;

	if (!PyArg_ParseTuple(args, "O!O!|I", &Vector2Type, &from, &Vector2Type, &to, &color))
		return NULL;

	return Py_BuildValue("i", PythonAPI_GameClient->pythonRender.DrawLine(from->toVec2(), to->toVec2(), color));
}

static PyObject* API_Graphics_drawText(PyObject* self, PyObject* args)
{
	Vector2 *pos;
	float fontWeight;
	char* text;
	unsigned int color = 0xffffffff;

	if (!PyArg_ParseTuple(args, "O!fs|I", &Vector2Type, &pos, &fontWeight, &text, &color))
		return NULL;

	return Py_BuildValue("i", PythonAPI_GameClient->pythonRender.DrawText(pos->toVec2(), fontWeight, string(text), color));
}

static PyObject* API_Graphics_removeDrawObject(PyObject* self, PyObject* args)
{
	int objectId;

	if (!PyArg_ParseTuple(args, "i", &objectId))
		return NULL;

	PythonAPI_GameClient->pythonRender.RemoveDrawObject(objectId);

	Py_RETURN_NONE;
}

static PyObject* API_Graphics_rgba(PyObject* self, PyObject* args)
{
	int red, green, blue, alpha = 255;

	if (!PyArg_ParseTuple(args, "iii|i", &red, &green, &blue, &alpha))
		return NULL;

	if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255 || alpha < 0 || alpha > 255) {
		PyErr_SetString(PyExc_ValueError, "Color components must be in the range 0-255");
		return NULL;
	}

	unsigned int rgba = (red << 24) | (green << 16) | (blue << 8) | alpha;

	return Py_BuildValue("I", rgba);
}

static PyObject* API_Graphics_getScreenSize(PyObject* self, PyObject* args)
{
	Vector2 *screenSize = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	screenSize->x = PythonAPI_GameClient->Ui()->Screen()->w;
	screenSize->y = PythonAPI_GameClient->Ui()->Screen()->h;

	return reinterpret_cast<PyObject *>(screenSize);
}

static PyObject* API_Graphics_mapWorldPositionToUI(PyObject* self, PyObject* args)
{
	Vector2 *worldPosition;

	if (!PyArg_ParseTuple(args, "O!", &Vector2Type, &worldPosition))
		return NULL;

	// return Graphics.GetScreenCenter() + ((position - Game.Camera:GetCenter()) / Game.Camera:GetZoom() * 0.75)
	vec2 screenCenter(PythonAPI_GameClient->Ui()->Screen()->w / 2, PythonAPI_GameClient->Ui()->Screen()->h / 2);
	vec2 diff = worldPosition->toVec2() - PythonAPI_GameClient->m_Camera.m_Center;
	vec2 uiPosition = screenCenter + diff / PythonAPI_GameClient->m_Camera.m_Zoom *  0.75;
	Vector2 *result = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	result->x = uiPosition.x;
	result->y = uiPosition.y;
	return reinterpret_cast<PyObject *>(result);
}

static PyObject* API_Graphics_mapUIPositionToWorld(PyObject* self, PyObject* args)
{
	Vector2 *worldPosition;

	if (!PyArg_ParseTuple(args, "O!", &Vector2Type, &worldPosition))
		return NULL;

	// return Game.Camera:GetCenter() + (position - Graphics.GetScreenCenter()) * 1.25 * Game.Camera:GetZoom()
	vec2 screenCenter(PythonAPI_GameClient->Ui()->Screen()->w / 2, PythonAPI_GameClient->Ui()->Screen()->h / 2);
	vec2 diff = worldPosition->toVec2() - screenCenter;
	vec2 uiPosition = PythonAPI_GameClient->m_Camera.m_Center + diff * PythonAPI_GameClient->m_Camera.m_Zoom *  1.25;
	Vector2 *result = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	result->x = uiPosition.x;
	result->y = uiPosition.y;
	return reinterpret_cast<PyObject *>(result);
}

static PyObject* API_Graphics_getTextWidth(PyObject* self, PyObject* args)
{
	float fontWeight;
	char* text;

	if (!PyArg_ParseTuple(args, "fs", &fontWeight, &text))
		return NULL;

	return Py_BuildValue("f", PythonAPI_GameClient->TextRender()->TextWidth(fontWeight, text));
}

static PyMethodDef API_GraphicsMethods[] = {
	{"drawCircle", API_Graphics_drawCircle, METH_VARARGS, "Return Draw Object id"},
	{"drawLine", API_Graphics_drawLine, METH_VARARGS, "Return Draw Object id"},
	{"drawText", API_Graphics_drawText, METH_VARARGS, "Return Draw Object id"},
	{"rgba", API_Graphics_rgba, METH_VARARGS, "Return color from RGBA"},
	{"removeDrawObject", API_Graphics_removeDrawObject, METH_VARARGS, "RemoveDrawObject(arg: ObjectId)"},
	{"getScreenSize", API_Graphics_getScreenSize, METH_VARARGS, "getScreenSize"},
	{"mapWorldPositionToUI", API_Graphics_mapWorldPositionToUI, METH_VARARGS, "Mapping world position to UI position"},
	{"mapUIPositionToWorld", API_Graphics_mapUIPositionToWorld, METH_VARARGS, "Mapping UI position to world position"},
	{"getTextWidth", API_Graphics_getTextWidth, METH_VARARGS, "Get text width"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef API_GraphicsModule = {
	PyModuleDef_HEAD_INIT,
	"API.Graphics",
	NULL,
	-1,
	API_GraphicsMethods
};

PyMODINIT_FUNC PyInit_API_Graphics(void)
{
	return PyModule_Create(&API_GraphicsModule);
}

#endif //DDNET_API_GRAPHICS_H
