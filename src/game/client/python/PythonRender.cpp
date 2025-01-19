//
// Created by danii on 09.01.2025.
//

#include "PythonRender.h"

#include "game/client/gameclient.h"
#include "game/client/ui.h"
#include "game/client/ui_rect.h"

PythonRender::PythonRender() = default;

void PythonRender::OnRender()
{
	// if(Client()->State() != IClient::STATE_ONLINE && Client()->State() != IClient::STATE_DEMOPLAYBACK)
	// 	return;

	const CUIRect Screen = *Ui()->Screen();
	const vec2 ScreenCenter = Screen.Center();
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
	Graphics()->LinesBegin();
	for (auto & linesPair : this->lines)
	{
		for (auto & linePair : linesPair.second)
		{
			PythonLine line = linePair.second;
			Graphics()->SetColor(line.getColorR(), line.getColorG(), line.getColorB(), line.getColorA());
			IGraphics::CLineItem lineItems[1];
			lineItems[0] = IGraphics::CLineItem(line.from.x, line.from.y, line.to.x, line.to.y);
			Graphics()->LinesDraw(lineItems, 1);
		}
	}

	Graphics()->LinesEnd();
	Graphics()->WrapClamp();
}

int PythonRender::DrawCircle(vec2 position, float radius, unsigned int color)
{
	this->circles[this->scriptId][this->objectOffset[this->scriptId]++] = PythonCircle(position, radius, color);

	return this->objectOffset[this->scriptId] - 1;
}

int PythonRender::DrawLine(vec2 from, vec2 to, unsigned int color)
{
	this->lines[this->scriptId][this->objectOffset[this->scriptId]++] = PythonLine(from, to, color);

	return this->objectOffset[this->scriptId] - 1;
}

void PythonRender::RemoveDrawObject(int objectId)
{
	if (this->circles[scriptId].find(objectId) != this->circles[scriptId].end())
	{
		this->circles[scriptId].erase(objectId);
	}
	else if (this->lines[scriptId].find(objectId) != this->lines[scriptId].end())
	{
		this->lines[scriptId].erase(objectId);
	}
}

void PythonRender::ResetScriptObjects(std::string scriptId)
{
	this->circles[scriptId] = std::map<int, PythonCircle>();
	this->lines[scriptId] = std::map<int, PythonLine>();
}