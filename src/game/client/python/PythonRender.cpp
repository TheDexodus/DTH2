//
// Created by danii on 09.01.2025.
//

#include "PythonRender.h"

#include "game/client/gameclient.h"
#include "game/client/ui.h"
#include "game/client/ui_rect.h"
#include "engine/storage.h"
#include <algorithm>
#include <cmath>
#include <vector>

PythonRender::PythonRender() = default;

static float DegreeToRad(float Degree)
{
	return Degree * 3.14159265358979323846f / 180.0f;
}

static vec2 RotatePointAroundCenter(vec2 Point, vec2 Center, float Degree)
{
	float Rad = DegreeToRad(Degree);
	float CosV = std::cos(Rad);
	float SinV = std::sin(Rad);

	vec2 Relative = Point - Center;
	return vec2(
		Center.x + Relative.x * CosV - Relative.y * SinV,
		Center.y + Relative.x * SinV + Relative.y * CosV
	);
}

void PythonRender::OnRender()
{
	// if(Client()->State() != IClient::STATE_ONLINE && Client()->State() != IClient::STATE_DEMOPLAYBACK)
	// 	return;

	const CUIRect Screen = *Ui()->Screen();
	Ui()->MapScreen();

	Graphics()->BlendNormal();

	Graphics()->TextureClear();
	Graphics()->QuadsBegin();

	for (auto & circlesPair : this->circles)
	{
		for (auto & circlePair : circlesPair.second)
		{
			PythonCircle circle = circlePair.second;
			Graphics()->SetColor(circle.getColorR(), circle.getColorG(), circle.getColorB(), circle.getColorA());
			Graphics()->DrawCircle(circle.position.x, circle.position.y, circle.radius, 64);
		}
	}

	Graphics()->QuadsEnd();

	for(auto &rectsPair : this->rects)
	{
		for(auto &rectPair : rectsPair.second)
		{
			int objectId = rectPair.first;
			PythonRect rect = rectPair.second;
			if(!rect.filled)
			{
				continue;
			}

			float x = std::min(rect.start.x, rect.end.x);
			float y = std::min(rect.start.y, rect.end.y);
			float w = std::fabs(rect.end.x - rect.start.x);
			float h = std::fabs(rect.end.y - rect.start.y);
			float degree = this->objectRotations[rectsPair.first][objectId];
			if(std::fabs(degree) < 0.001f)
			{
				Graphics()->DrawRect(x, y, w, h, ColorRGBA(rect.getColorR(), rect.getColorG(), rect.getColorB(), rect.getColorA()), IGraphics::CORNER_NONE, 0.0f);
			}
			else
			{
				vec2 center(x + w / 2.0f, y + h / 2.0f);
				vec2 p0 = RotatePointAroundCenter(vec2(x, y), center, degree);
				vec2 p1 = RotatePointAroundCenter(vec2(x + w, y), center, degree);
				vec2 p2 = RotatePointAroundCenter(vec2(x + w, y + h), center, degree);
				vec2 p3 = RotatePointAroundCenter(vec2(x, y + h), center, degree);

				Graphics()->TextureClear();
				Graphics()->QuadsBegin();
				Graphics()->SetColor(rect.getColorR(), rect.getColorG(), rect.getColorB(), rect.getColorA());
				IGraphics::CFreeformItem item(p0.x, p0.y, p1.x, p1.y, p3.x, p3.y, p2.x, p2.y);
				Graphics()->QuadsDrawFreeform(&item, 1);
				Graphics()->QuadsEnd();
			}
		}
	}

	for(auto &roundedRectsPair : this->roundedRects)
	{
		for(auto &roundedRectPair : roundedRectsPair.second)
		{
			int objectId = roundedRectPair.first;
			PythonRoundedRect roundedRect = roundedRectPair.second;
			if(!roundedRect.filled)
			{
				continue;
			}

			float x = std::min(roundedRect.start.x, roundedRect.end.x);
			float y = std::min(roundedRect.start.y, roundedRect.end.y);
			float w = std::fabs(roundedRect.end.x - roundedRect.start.x);
			float h = std::fabs(roundedRect.end.y - roundedRect.start.y);
			float radius = std::max(0.0f, std::min(roundedRect.borderRadius, std::min(w * 0.5f, h * 0.5f)));
			float degree = this->objectRotations[roundedRectsPair.first][objectId];
			if(std::fabs(degree) < 0.001f)
			{
				Graphics()->DrawRect(x, y, w, h, ColorRGBA(roundedRect.getColorR(), roundedRect.getColorG(), roundedRect.getColorB(), roundedRect.getColorA()), IGraphics::CORNER_ALL, radius);
			}
			else
			{
				// Keep rounded corners while rotating: fill a rounded-rect polygon.
				vec2 center(x + w / 2.0f, y + h / 2.0f);
				std::vector<vec2> boundary;
				boundary.reserve(40);
				const int arcSegments = 8;
				const float pi = 3.14159265358979323846f;

				auto AppendPoint = [&boundary](float px, float py)
				{
					if(boundary.empty() || std::fabs(boundary.back().x - px) > 0.001f || std::fabs(boundary.back().y - py) > 0.001f)
					{
						boundary.emplace_back(px, py);
					}
				};

				if(radius <= 0.001f)
				{
					AppendPoint(x, y);
					AppendPoint(x + w, y);
					AppendPoint(x + w, y + h);
					AppendPoint(x, y + h);
				}
				else
				{
					AppendPoint(x + radius, y);
					AppendPoint(x + w - radius, y);

					for(int i = 0; i <= arcSegments; i++)
					{
						float t = static_cast<float>(i) / static_cast<float>(arcSegments);
						float a = -pi * 0.5f + t * (pi * 0.5f);
						AppendPoint(x + w - radius + std::cos(a) * radius, y + radius + std::sin(a) * radius);
					}

					AppendPoint(x + w, y + h - radius);
					for(int i = 0; i <= arcSegments; i++)
					{
						float t = static_cast<float>(i) / static_cast<float>(arcSegments);
						float a = 0.0f + t * (pi * 0.5f);
						AppendPoint(x + w - radius + std::cos(a) * radius, y + h - radius + std::sin(a) * radius);
					}

					AppendPoint(x + radius, y + h);
					for(int i = 0; i <= arcSegments; i++)
					{
						float t = static_cast<float>(i) / static_cast<float>(arcSegments);
						float a = pi * 0.5f + t * (pi * 0.5f);
						AppendPoint(x + radius + std::cos(a) * radius, y + h - radius + std::sin(a) * radius);
					}

					AppendPoint(x, y + radius);
					for(int i = 0; i <= arcSegments; i++)
					{
						float t = static_cast<float>(i) / static_cast<float>(arcSegments);
						float a = pi + t * (pi * 0.5f);
						AppendPoint(x + radius + std::cos(a) * radius, y + radius + std::sin(a) * radius);
					}
				}

				std::vector<IGraphics::CFreeformItem> triangles;
				triangles.reserve(boundary.size());
				for(size_t i = 0; i < boundary.size(); i++)
				{
					vec2 p0 = RotatePointAroundCenter(boundary[i], center, degree);
					vec2 p1 = RotatePointAroundCenter(boundary[(i + 1) % boundary.size()], center, degree);
					vec2 c = RotatePointAroundCenter(center, center, degree);
					triangles.emplace_back(c.x, c.y, p0.x, p0.y, p1.x, p1.y, p1.x, p1.y);
				}

				if(!triangles.empty())
				{
					Graphics()->TextureClear();
					Graphics()->QuadsBegin();
					Graphics()->SetColor(roundedRect.getColorR(), roundedRect.getColorG(), roundedRect.getColorB(), roundedRect.getColorA());
					Graphics()->QuadsDrawFreeform(triangles.data(), static_cast<int>(triangles.size()));
					Graphics()->QuadsEnd();
				}
			}
		}
	}

	Graphics()->LinesBegin();
	for (auto & linesPair : this->lines)
	{
		for (auto & linePair : linesPair.second)
		{
			int objectId = linePair.first;
			PythonLine line = linePair.second;
			float degree = this->objectRotations[linesPair.first][objectId];
			vec2 from = line.from;
			vec2 to = line.to;
			if(std::fabs(degree) >= 0.001f)
			{
				vec2 center = (from + to) / 2.0f;
				from = RotatePointAroundCenter(from, center, degree);
				to = RotatePointAroundCenter(to, center, degree);
			}

			Graphics()->SetColor(line.getColorR(), line.getColorG(), line.getColorB(), line.getColorA());
			IGraphics::CLineItem lineItems[1];
			lineItems[0] = IGraphics::CLineItem(from.x, from.y, to.x, to.y);
			Graphics()->LinesDraw(lineItems, 1);
		}
	}

	for(auto &rectsPair : this->rects)
	{
		for(auto &rectPair : rectsPair.second)
		{
			int objectId = rectPair.first;
			PythonRect rect = rectPair.second;
			if(rect.filled)
			{
				continue;
			}

			float degree = this->objectRotations[rectsPair.first][objectId];
			vec2 p0(rect.start.x, rect.start.y);
			vec2 p1(rect.end.x, rect.start.y);
			vec2 p2(rect.end.x, rect.end.y);
			vec2 p3(rect.start.x, rect.end.y);
			if(std::fabs(degree) >= 0.001f)
			{
				vec2 center((rect.start.x + rect.end.x) / 2.0f, (rect.start.y + rect.end.y) / 2.0f);
				p0 = RotatePointAroundCenter(p0, center, degree);
				p1 = RotatePointAroundCenter(p1, center, degree);
				p2 = RotatePointAroundCenter(p2, center, degree);
				p3 = RotatePointAroundCenter(p3, center, degree);
			}

			Graphics()->SetColor(rect.getColorR(), rect.getColorG(), rect.getColorB(), rect.getColorA());
			IGraphics::CLineItem lineItems[4];
			lineItems[0] = IGraphics::CLineItem(p0.x, p0.y, p1.x, p1.y);
			lineItems[1] = IGraphics::CLineItem(p1.x, p1.y, p2.x, p2.y);
			lineItems[2] = IGraphics::CLineItem(p2.x, p2.y, p3.x, p3.y);
			lineItems[3] = IGraphics::CLineItem(p3.x, p3.y, p0.x, p0.y);
			Graphics()->LinesDraw(lineItems, 4);
		}
	}

	for(auto &roundedRectsPair : this->roundedRects)
	{
		for(auto &roundedRectPair : roundedRectsPair.second)
		{
			int objectId = roundedRectPair.first;
			PythonRoundedRect roundedRect = roundedRectPair.second;
			if(roundedRect.filled)
			{
				continue;
			}

			Graphics()->SetColor(roundedRect.getColorR(), roundedRect.getColorG(), roundedRect.getColorB(), roundedRect.getColorA());
			float x = std::min(roundedRect.start.x, roundedRect.end.x);
			float y = std::min(roundedRect.start.y, roundedRect.end.y);
			float w = std::fabs(roundedRect.end.x - roundedRect.start.x);
			float h = std::fabs(roundedRect.end.y - roundedRect.start.y);
			float radius = std::max(0.0f, std::min(roundedRect.borderRadius, std::min(w * 0.5f, h * 0.5f)));
			float degree = this->objectRotations[roundedRectsPair.first][objectId];
			vec2 center(x + w / 2.0f, y + h / 2.0f);

			std::vector<IGraphics::CLineItem> lineItems;
			lineItems.reserve(40);

			if(radius <= 0.0f)
			{
				lineItems.emplace_back(x, y, x + w, y);
				lineItems.emplace_back(x + w, y, x + w, y + h);
				lineItems.emplace_back(x + w, y + h, x, y + h);
				lineItems.emplace_back(x, y + h, x, y);
			}
			else
			{
				lineItems.emplace_back(x + radius, y, x + w - radius, y);
				lineItems.emplace_back(x + w, y + radius, x + w, y + h - radius);
				lineItems.emplace_back(x + w - radius, y + h, x + radius, y + h);
				lineItems.emplace_back(x, y + h - radius, x, y + radius);

				const float pi = 3.14159265358979323846f;
				const int segments = 8;
				auto AddArc = [&lineItems, segments](float centerX, float centerY, float arcRadius, float startAngle, float endAngle)
				{
					for(int i = 0; i < segments; i++)
					{
						float t0 = static_cast<float>(i) / static_cast<float>(segments);
						float t1 = static_cast<float>(i + 1) / static_cast<float>(segments);
						float a0 = startAngle + (endAngle - startAngle) * t0;
						float a1 = startAngle + (endAngle - startAngle) * t1;
						lineItems.emplace_back(centerX + std::cos(a0) * arcRadius, centerY + std::sin(a0) * arcRadius, centerX + std::cos(a1) * arcRadius, centerY + std::sin(a1) * arcRadius);
					}
				};

				AddArc(x + radius, y + radius, radius, pi, pi * 1.5f);
				AddArc(x + w - radius, y + radius, radius, pi * 1.5f, pi * 2.0f);
				AddArc(x + w - radius, y + h - radius, radius, 0.0f, pi * 0.5f);
				AddArc(x + radius, y + h - radius, radius, pi * 0.5f, pi);
			}

			if(!lineItems.empty())
			{
				if(std::fabs(degree) >= 0.001f)
				{
					for(auto &line : lineItems)
					{
						vec2 from = RotatePointAroundCenter(vec2(line.m_X0, line.m_Y0), center, degree);
						vec2 to = RotatePointAroundCenter(vec2(line.m_X1, line.m_Y1), center, degree);
						line.m_X0 = from.x;
						line.m_Y0 = from.y;
						line.m_X1 = to.x;
						line.m_Y1 = to.y;
					}
				}
				Graphics()->LinesDraw(lineItems.data(), static_cast<int>(lineItems.size()));
			}
		}
	}

	Graphics()->LinesEnd();

	for(auto &spritesPair : this->sprites)
	{
		for(auto &spritePair : spritesPair.second)
		{
			int objectId = spritePair.first;
			PythonSprite sprite = spritePair.second;
			if(!sprite.texture.IsValid())
			{
				continue;
			}

			Graphics()->TextureSet(sprite.texture);
			Graphics()->QuadsBegin();
			Graphics()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			float degree = this->objectRotations[spritesPair.first][objectId];
			Graphics()->QuadsSetRotation(DegreeToRad(degree));
			IGraphics::CQuadItem item(sprite.position.x + sprite.size / 2.0f, sprite.position.y + sprite.size / 2.0f, sprite.size, sprite.size);
			Graphics()->QuadsDraw(&item, 1);
			Graphics()->QuadsSetRotation(0.0f);
			Graphics()->QuadsEnd();
		}
	}

	for (auto & textsPair : this->texts)
	{
		for (auto & textPair : textsPair.second)
		{
			PythonText text = textPair.second;
			TextRender()->TextColor(text.getColorR(), text.getColorG(), text.getColorB(), text.getColorA());
			TextRender()->Text(text.position.x, text.position.y, text.fontSize, text.text.c_str());
		}
	}

	if(GameClient()->pythonController.showMenuCursor && !GameClient()->m_Menus.IsActive())
	{
		RenderTools()->RenderCursor(GameClient()->pythonController.GetScriptCursorPos(), 24.0f);
	}

	TextRender()->TextColor(1.0, 1.0, 1.0, 1.0);
	Graphics()->WrapClamp();
}

int PythonRender::DrawCircle(vec2 position, float radius, unsigned int color)
{
	int objectId = this->objectOffset[this->scriptId]++;
	this->circles[this->scriptId][objectId] = PythonCircle(position, radius, color);
	this->objectRotations[this->scriptId][objectId] = 0.0f;
	return objectId;
}

int PythonRender::DrawLine(vec2 from, vec2 to, unsigned int color)
{
	int objectId = this->objectOffset[this->scriptId]++;
	this->lines[this->scriptId][objectId] = PythonLine(from, to, color);
	this->objectRotations[this->scriptId][objectId] = 0.0f;
	return objectId;
}

int PythonRender::DrawRect(vec2 start, vec2 end, bool filled, unsigned int color)
{
	int objectId = this->objectOffset[this->scriptId]++;
	this->rects[this->scriptId][objectId] = PythonRect(start, end, filled, color);
	this->objectRotations[this->scriptId][objectId] = 0.0f;
	return objectId;
}

int PythonRender::DrawRoundedRect(vec2 start, vec2 end, bool filled, float borderRadius, unsigned int color)
{
	int objectId = this->objectOffset[this->scriptId]++;
	this->roundedRects[this->scriptId][objectId] = PythonRoundedRect(start, end, filled, borderRadius, color);
	this->objectRotations[this->scriptId][objectId] = 0.0f;
	return objectId;
}

int PythonRender::DrawText(vec2 position, float fontSize, std::string text, unsigned int color)
{
	int objectId = this->objectOffset[this->scriptId]++;
	this->texts[this->scriptId][objectId] = PythonText(position, fontSize, text, color);
	this->objectRotations[this->scriptId][objectId] = 0.0f;
	return objectId;
}

int PythonRender::DrawSprite(vec2 position, const std::string &spritePath, float size)
{
	IGraphics::CTextureHandle texture;
	auto textureIt = this->spriteTextures.find(spritePath);
	if(textureIt != this->spriteTextures.end())
	{
		texture = textureIt->second;
	}
	else
	{
		texture = Graphics()->LoadTexture(spritePath.c_str(), IStorage::TYPE_ALL);
		this->spriteTextures[spritePath] = texture;
	}

	int objectId = this->objectOffset[this->scriptId]++;
	this->sprites[this->scriptId][objectId] = PythonSprite(position, size, texture);
	this->objectRotations[this->scriptId][objectId] = 0.0f;
	return objectId;
}

bool PythonRender::MoveDrawObject(int objectId, vec2 newPosition)
{
	if(this->circles[scriptId].find(objectId) != this->circles[scriptId].end())
	{
		this->circles[scriptId][objectId].position = newPosition;
		return true;
	}
	else if(this->lines[scriptId].find(objectId) != this->lines[scriptId].end())
	{
		PythonLine &line = this->lines[scriptId][objectId];
		vec2 offset = newPosition - line.from;
		line.from = newPosition;
		line.to += offset;
		return true;
	}
	else if(this->texts[scriptId].find(objectId) != this->texts[scriptId].end())
	{
		this->texts[scriptId][objectId].position = newPosition;
		return true;
	}
	else if(this->rects[scriptId].find(objectId) != this->rects[scriptId].end())
	{
		PythonRect &rect = this->rects[scriptId][objectId];
		vec2 offset = newPosition - rect.start;
		rect.start = newPosition;
		rect.end += offset;
		return true;
	}
	else if(this->roundedRects[scriptId].find(objectId) != this->roundedRects[scriptId].end())
	{
		PythonRoundedRect &roundedRect = this->roundedRects[scriptId][objectId];
		vec2 offset = newPosition - roundedRect.start;
		roundedRect.start = newPosition;
		roundedRect.end += offset;
		return true;
	}
	else if(this->sprites[scriptId].find(objectId) != this->sprites[scriptId].end())
	{
		this->sprites[scriptId][objectId].position = newPosition;
		return true;
	}

	return false;
}

bool PythonRender::GetDrawObjectPosition(int objectId, vec2 &position) const
{
	auto circlesScriptIt = this->circles.find(scriptId);
	if(circlesScriptIt != this->circles.end())
	{
		auto circleIt = circlesScriptIt->second.find(objectId);
		if(circleIt != circlesScriptIt->second.end())
		{
			position = circleIt->second.position;
			return true;
		}
	}

	auto linesScriptIt = this->lines.find(scriptId);
	if(linesScriptIt != this->lines.end())
	{
		auto lineIt = linesScriptIt->second.find(objectId);
		if(lineIt != linesScriptIt->second.end())
		{
			position = lineIt->second.from;
			return true;
		}
	}

	auto textsScriptIt = this->texts.find(scriptId);
	if(textsScriptIt != this->texts.end())
	{
		auto textIt = textsScriptIt->second.find(objectId);
		if(textIt != textsScriptIt->second.end())
		{
			position = textIt->second.position;
			return true;
		}
	}

	auto rectsScriptIt = this->rects.find(scriptId);
	if(rectsScriptIt != this->rects.end())
	{
		auto rectIt = rectsScriptIt->second.find(objectId);
		if(rectIt != rectsScriptIt->second.end())
		{
			position = rectIt->second.start;
			return true;
		}
	}

	auto roundedRectsScriptIt = this->roundedRects.find(scriptId);
	if(roundedRectsScriptIt != this->roundedRects.end())
	{
		auto roundedRectIt = roundedRectsScriptIt->second.find(objectId);
		if(roundedRectIt != roundedRectsScriptIt->second.end())
		{
			position = roundedRectIt->second.start;
			return true;
		}
	}

	auto spritesScriptIt = this->sprites.find(scriptId);
	if(spritesScriptIt != this->sprites.end())
	{
		auto spriteIt = spritesScriptIt->second.find(objectId);
		if(spriteIt != spritesScriptIt->second.end())
		{
			position = spriteIt->second.position;
			return true;
		}
	}

	return false;
}

bool PythonRender::SetDrawObjectRotation(int objectId, float degree)
{
	if(this->circles[scriptId].find(objectId) == this->circles[scriptId].end() &&
		this->lines[scriptId].find(objectId) == this->lines[scriptId].end() &&
		this->rects[scriptId].find(objectId) == this->rects[scriptId].end() &&
		this->roundedRects[scriptId].find(objectId) == this->roundedRects[scriptId].end() &&
		this->texts[scriptId].find(objectId) == this->texts[scriptId].end() &&
		this->sprites[scriptId].find(objectId) == this->sprites[scriptId].end())
	{
		return false;
	}

	this->objectRotations[scriptId][objectId] = degree;
	return true;
}

bool PythonRender::GetDrawObjectRotation(int objectId, float &degree) const
{
	if(this->circles.find(scriptId) != this->circles.end() && this->circles.at(scriptId).find(objectId) != this->circles.at(scriptId).end())
	{
		auto it = this->objectRotations.find(scriptId);
		degree = (it != this->objectRotations.end() && it->second.find(objectId) != it->second.end()) ? it->second.at(objectId) : 0.0f;
		return true;
	}
	if(this->lines.find(scriptId) != this->lines.end() && this->lines.at(scriptId).find(objectId) != this->lines.at(scriptId).end())
	{
		auto it = this->objectRotations.find(scriptId);
		degree = (it != this->objectRotations.end() && it->second.find(objectId) != it->second.end()) ? it->second.at(objectId) : 0.0f;
		return true;
	}
	if(this->rects.find(scriptId) != this->rects.end() && this->rects.at(scriptId).find(objectId) != this->rects.at(scriptId).end())
	{
		auto it = this->objectRotations.find(scriptId);
		degree = (it != this->objectRotations.end() && it->second.find(objectId) != it->second.end()) ? it->second.at(objectId) : 0.0f;
		return true;
	}
	if(this->roundedRects.find(scriptId) != this->roundedRects.end() && this->roundedRects.at(scriptId).find(objectId) != this->roundedRects.at(scriptId).end())
	{
		auto it = this->objectRotations.find(scriptId);
		degree = (it != this->objectRotations.end() && it->second.find(objectId) != it->second.end()) ? it->second.at(objectId) : 0.0f;
		return true;
	}
	if(this->texts.find(scriptId) != this->texts.end() && this->texts.at(scriptId).find(objectId) != this->texts.at(scriptId).end())
	{
		auto it = this->objectRotations.find(scriptId);
		degree = (it != this->objectRotations.end() && it->second.find(objectId) != it->second.end()) ? it->second.at(objectId) : 0.0f;
		return true;
	}
	if(this->sprites.find(scriptId) != this->sprites.end() && this->sprites.at(scriptId).find(objectId) != this->sprites.at(scriptId).end())
	{
		auto it = this->objectRotations.find(scriptId);
		degree = (it != this->objectRotations.end() && it->second.find(objectId) != it->second.end()) ? it->second.at(objectId) : 0.0f;
		return true;
	}

	return false;
}

void PythonRender::RemoveDrawObject(int objectId)
{
	if (this->circles[scriptId].find(objectId) != this->circles[scriptId].end())
	{
		this->circles[scriptId].erase(objectId);
		this->objectRotations[scriptId].erase(objectId);
	}
	else if (this->lines[scriptId].find(objectId) != this->lines[scriptId].end())
	{
		this->lines[scriptId].erase(objectId);
		this->objectRotations[scriptId].erase(objectId);
	}
	else if (this->texts[scriptId].find(objectId) != this->texts[scriptId].end())
	{
		this->texts[scriptId].erase(objectId);
		this->objectRotations[scriptId].erase(objectId);
	}
	else if (this->rects[scriptId].find(objectId) != this->rects[scriptId].end())
	{
		this->rects[scriptId].erase(objectId);
		this->objectRotations[scriptId].erase(objectId);
	}
	else if (this->roundedRects[scriptId].find(objectId) != this->roundedRects[scriptId].end())
	{
		this->roundedRects[scriptId].erase(objectId);
		this->objectRotations[scriptId].erase(objectId);
	}
	else if (this->sprites[scriptId].find(objectId) != this->sprites[scriptId].end())
	{
		this->sprites[scriptId].erase(objectId);
		this->objectRotations[scriptId].erase(objectId);
	}
}

void PythonRender::ResetScriptObjects(std::string scriptId)
{
	this->circles[scriptId] = std::map<int, PythonCircle>();
	this->lines[scriptId] = std::map<int, PythonLine>();
	this->rects[scriptId] = std::map<int, PythonRect>();
	this->roundedRects[scriptId] = std::map<int, PythonRoundedRect>();
	this->texts[scriptId] = std::map<int, PythonText>();
	this->sprites[scriptId] = std::map<int, PythonSprite>();
	this->objectRotations[scriptId] = std::map<int, float>();
}
