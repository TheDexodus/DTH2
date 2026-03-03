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

static PyObject* API_Graphics_drawRect(PyObject* self, PyObject* args)
{
	Vector2 *positionStart;
	Vector2 *positionEnd;
	int filled;
	unsigned int color = 0xffffffff;

	if(!PyArg_ParseTuple(args, "O!O!p|I", &Vector2Type, &positionStart, &Vector2Type, &positionEnd, &filled, &color))
		return NULL;

	return Py_BuildValue("i", PythonAPI_GameClient->pythonRender.DrawRect(positionStart->toVec2(), positionEnd->toVec2(), filled != 0, color));
}

static PyObject* API_Graphics_drawRoundedRect(PyObject* self, PyObject* args)
{
	Vector2 *positionStart;
	Vector2 *positionEnd;
	int filled;
	float borderRadius;
	unsigned int color = 0xffffffff;

	if(!PyArg_ParseTuple(args, "O!O!pf|I", &Vector2Type, &positionStart, &Vector2Type, &positionEnd, &filled, &borderRadius, &color))
		return NULL;

	if(borderRadius < 0.0f)
	{
		PyErr_SetString(PyExc_ValueError, "`borderRadius` must be >= 0");
		return NULL;
	}

	return Py_BuildValue("i", PythonAPI_GameClient->pythonRender.DrawRoundedRect(positionStart->toVec2(), positionEnd->toVec2(), filled != 0, borderRadius, color));
}

static PyObject* API_Graphics_drawSprite(PyObject* self, PyObject* args)
{
	Vector2 *position;
	char *spritePath;
	float size;

	if(!PyArg_ParseTuple(args, "O!sf", &Vector2Type, &position, &spritePath, &size))
		return NULL;

	return Py_BuildValue("i", PythonAPI_GameClient->pythonRender.DrawSprite(position->toVec2(), string(spritePath), size));
}

static PyObject* API_Graphics_removeDrawObject(PyObject* self, PyObject* args)
{
	int objectId;

	if (!PyArg_ParseTuple(args, "i", &objectId))
		return NULL;

	PythonAPI_GameClient->pythonRender.RemoveDrawObject(objectId);

	Py_RETURN_NONE;
}

static PyObject* API_Graphics_moveDrawObject(PyObject* self, PyObject* args)
{
	int objectId;
	Vector2 *newCoordinate;

	if(!PyArg_ParseTuple(args, "iO!", &objectId, &Vector2Type, &newCoordinate))
		return NULL;

	PythonAPI_GameClient->pythonRender.MoveDrawObject(objectId, newCoordinate->toVec2());

	Py_RETURN_NONE;
}

static PyObject* API_Graphics_getDrawObjectPosition(PyObject* self, PyObject* args)
{
	int objectId;
	if(!PyArg_ParseTuple(args, "i", &objectId))
		return NULL;

	vec2 position;
	if(!PythonAPI_GameClient->pythonRender.GetDrawObjectPosition(objectId, position))
	{
		Py_RETURN_NONE;
	}

	Vector2 *result = (Vector2 *)PyObject_New(Vector2, &Vector2Type);
	result->x = position.x;
	result->y = position.y;
	return reinterpret_cast<PyObject *>(result);
}

static PyObject* API_Graphics_setDrawObjectRotation(PyObject* self, PyObject* args)
{
	int objectId;
	float degree;
	if(!PyArg_ParseTuple(args, "if", &objectId, &degree))
		return NULL;

	if(!PythonAPI_GameClient->pythonRender.SetDrawObjectRotation(objectId, degree))
	{
		PyErr_SetString(PyExc_ValueError, "Draw object not found");
		return NULL;
	}

	Py_RETURN_NONE;
}

static PyObject* API_Graphics_getDrawObjectRotation(PyObject* self, PyObject* args)
{
	int objectId;
	if(!PyArg_ParseTuple(args, "i", &objectId))
		return NULL;

	float degree = 0.0f;
	if(!PythonAPI_GameClient->pythonRender.GetDrawObjectRotation(objectId, degree))
	{
		PyErr_SetString(PyExc_ValueError, "Draw object not found");
		return NULL;
	}

	return PyFloat_FromDouble((double)degree);
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

static PyObject* API_Graphics_isMenuDisplayed(PyObject* self, PyObject* args)
{
	return PyBool_FromLong(PythonAPI_GameClient->m_Menus.IsActive() ? 1 : 0);
}

static PyMethodDef API_GraphicsMethods[] = {
	{"draw_circle", API_Graphics_drawCircle, METH_VARARGS, "draw_circle(position, radius, color=0xFFFFFFFF) -> object_id"},
	{"draw_line", API_Graphics_drawLine, METH_VARARGS, "draw_line(from_position, to_position, color=0xFFFFFFFF) -> object_id"},
	{"draw_text", API_Graphics_drawText, METH_VARARGS, "draw_text(pos, font_weight, text, color=0xFFFFFFFF) -> object_id"},
	{"get_rgba", API_Graphics_rgba, METH_VARARGS, "get_rgba(red, green, blue, alpha=255) -> int"},
	{"draw_rect", API_Graphics_drawRect, METH_VARARGS, "draw_rect(position_start, position_end, filled, color=0xFFFFFFFF) -> object_id"},
	{"draw_rounded_rect", API_Graphics_drawRoundedRect, METH_VARARGS, "draw_rounded_rect(position_start, position_end, filled, border_radius, color=0xFFFFFFFF) -> object_id"},
	{"draw_sprite", API_Graphics_drawSprite, METH_VARARGS, "draw_sprite(position, sprite_path, size) -> object_id"},
	{"remove_draw_object", API_Graphics_removeDrawObject, METH_VARARGS, "remove_draw_object(object_id)"},
	{"move_draw_object", API_Graphics_moveDrawObject, METH_VARARGS, "move_draw_object(object_id, new_coordinate)"},
	{"get_draw_object_position", API_Graphics_getDrawObjectPosition, METH_VARARGS, "get_draw_object_position(object_id) -> Vector2 | None"},
	{"set_draw_object_rotation", API_Graphics_setDrawObjectRotation, METH_VARARGS, "set_draw_object_rotation(object_id, degree)"},
	{"get_draw_object_rotation", API_Graphics_getDrawObjectRotation, METH_VARARGS, "get_draw_object_rotation(object_id) -> degree"},
	{"get_screen_size", API_Graphics_getScreenSize, METH_VARARGS, "get_screen_size() -> Vector2"},
	{"map_world_position_to_ui", API_Graphics_mapWorldPositionToUI, METH_VARARGS, "map_world_position_to_ui(world_position) -> Vector2"},
	{"map_ui_position_to_world", API_Graphics_mapUIPositionToWorld, METH_VARARGS, "map_ui_position_to_world(ui_position) -> Vector2"},
	{"get_text_width", API_Graphics_getTextWidth, METH_VARARGS, "get_text_width(font_weight, text) -> float"},
	{"is_menu_displayed", API_Graphics_isMenuDisplayed, METH_NOARGS, "is_menu_displayed() -> bool"},
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
