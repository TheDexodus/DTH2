//
// Created by danii on 09.01.2025.
//

#ifndef PYTHONRENDER_H
#define PYTHONRENDER_H

#include "engine/client.h"
#include "game/client/component.h"
#include <map>
#include <utility>

struct PythonCircle
{
	vec2 position;
	float radius;
	unsigned int color;
	PythonCircle() = default;
	PythonCircle(vec2 position, float radius, unsigned int color)
	{
		this->position = position;
		this->radius = radius;
		this->color = color;
	}
	float getColorR() { return (color >> 24 & 0xFF) / 255.0f; }
	float getColorG() { return (color >> 16 & 0xFF) / 255.0f; }
	float getColorB() { return (color >> 8 & 0xFF) / 255.0f; }
	float getColorA() { return (color & 0xFF) / 255.0f; }
};

struct PythonLine
{
	vec2 from;
	vec2 to;
	unsigned int color;
	PythonLine() = default;
	PythonLine(vec2 from, vec2 to, unsigned int color)
	{
		this->from = from;
		this->to = to;
		this->color = color;
	}
	float getColorR() { return (color >> 24 & 0xFF) / 255.0f; }
	float getColorG() { return (color >> 16 & 0xFF) / 255.0f; }
	float getColorB() { return (color >> 8 & 0xFF) / 255.0f; }
	float getColorA() { return (color & 0xFF) / 255.0f; }
};

struct PythonText
{
	vec2 position;
	float fontSize;
	std::string text;
	unsigned int color;
	PythonText() = default;
	PythonText(vec2 position, float fontSize, std::string text, unsigned int color)
	{
		this->position = position;
		this->fontSize = fontSize;
		this->text = std::move(text);
		this->color = color;
	}
	float getColorR() { return (color >> 24 & 0xFF) / 255.0f; }
	float getColorG() { return (color >> 16 & 0xFF) / 255.0f; }
	float getColorB() { return (color >> 8 & 0xFF) / 255.0f; }
	float getColorA() { return (color & 0xFF) / 255.0f; }
};

class PythonRender : public CComponent
{
public:
	PythonRender();

	void OnRender();

	int DrawCircle(vec2 position, float radius, unsigned int color);
	int DrawLine(vec2 from, vec2 to, unsigned int color);
	int DrawText(vec2 position, float fontSize, std::string text, unsigned int color);
	void RemoveDrawObject(int objectId);
	void ResetScriptObjects(std::string scriptId);

	void SetScriptRender(std::string scriptId) { this->scriptId = scriptId; };

	virtual int Sizeof() const override { return sizeof(*this); }

private:
	std::map<std::string, std::map<int, PythonCircle>> circles;
	std::map<std::string, std::map<int, PythonLine>> lines;
	std::map<std::string, std::map<int, PythonText>> texts;
	std::map<std::string, int> objectOffset;
	std::string scriptId = "";
};



#endif //PYTHONRENDER_H
