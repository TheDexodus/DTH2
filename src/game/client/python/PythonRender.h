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

struct PythonSprite
{
	vec2 position;
	float size;
	IGraphics::CTextureHandle texture;
	PythonSprite() = default;
	PythonSprite(vec2 position, float size, IGraphics::CTextureHandle texture)
	{
		this->position = position;
		this->size = size;
		this->texture = texture;
	}
};

struct PythonRect
{
	vec2 start;
	vec2 end;
	bool filled;
	unsigned int color;
	PythonRect() = default;
	PythonRect(vec2 start, vec2 end, bool filled, unsigned int color)
	{
		this->start = start;
		this->end = end;
		this->filled = filled;
		this->color = color;
	}
	float getColorR() { return (color >> 24 & 0xFF) / 255.0f; }
	float getColorG() { return (color >> 16 & 0xFF) / 255.0f; }
	float getColorB() { return (color >> 8 & 0xFF) / 255.0f; }
	float getColorA() { return (color & 0xFF) / 255.0f; }
};

struct PythonRoundedRect
{
	vec2 start;
	vec2 end;
	bool filled;
	float borderRadius;
	unsigned int color;
	PythonRoundedRect() = default;
	PythonRoundedRect(vec2 start, vec2 end, bool filled, float borderRadius, unsigned int color)
	{
		this->start = start;
		this->end = end;
		this->filled = filled;
		this->borderRadius = borderRadius;
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
	int DrawRect(vec2 start, vec2 end, bool filled, unsigned int color);
	int DrawRoundedRect(vec2 start, vec2 end, bool filled, float borderRadius, unsigned int color);
	int DrawText(vec2 position, float fontSize, std::string text, unsigned int color);
	int DrawSprite(vec2 position, const std::string &spritePath, float size);
	bool MoveDrawObject(int objectId, vec2 newPosition);
	bool GetDrawObjectPosition(int objectId, vec2 &position) const;
	bool SetDrawObjectRotation(int objectId, float degree);
	bool GetDrawObjectRotation(int objectId, float &degree) const;
	void RemoveDrawObject(int objectId);
	void ResetScriptObjects(std::string scriptId);

	void SetScriptRender(std::string scriptId) { this->scriptId = scriptId; };
	const std::string &GetScriptRender() const { return scriptId; }

	virtual int Sizeof() const override { return sizeof(*this); }

private:
	std::map<std::string, std::map<int, PythonCircle>> circles;
	std::map<std::string, std::map<int, PythonLine>> lines;
	std::map<std::string, std::map<int, PythonRect>> rects;
	std::map<std::string, std::map<int, PythonRoundedRect>> roundedRects;
	std::map<std::string, std::map<int, PythonText>> texts;
	std::map<std::string, std::map<int, PythonSprite>> sprites;
	std::map<std::string, IGraphics::CTextureHandle> spriteTextures;
	std::map<std::string, std::map<int, float>> objectRotations;
	std::map<std::string, int> objectOffset;
	std::string scriptId = "";
};



#endif //PYTHONRENDER_H
