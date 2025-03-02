//
// Created by danii on 12.02.2025.
//
#include <base/log.h>
#include <base/math.h>
#include <base/system.h>

#include <engine/graphics.h>
#include <engine/shared/config.h>
#include <engine/shared/linereader.h>
#include <engine/shared/localization.h>
#include <engine/shared/protocol7.h>
#include <engine/storage.h>
#include <engine/textrender.h>
#include <engine/updater.h>

#include <game/generated/protocol.h>

#include <game/client/animstate.h>
#include <game/client/components/chat.h>
#include <game/client/components/menu_background.h>
#include <game/client/components/sounds.h>
#include <game/client/gameclient.h>
#include <game/client/render.h>
#include <game/client/skin.h>
#include <game/client/ui.h>
#include <game/client/ui_listbox.h>
#include <game/client/ui_scrollregion.h>
#include <game/localization.h>

#include "binds.h"
#include "countryflags.h"
#include "menus.h"
#include "skins.h"

#include <SDL_clipboard.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

void CMenus::RenderDTH(CUIRect MainView)
{
	CUIRect Button, TabBar, RestartBar;
	MainView.VSplitRight(120.0f, &MainView, &TabBar);

	MainView.Draw(ms_ColorTabbarActive, IGraphics::CORNER_B, 10.0f);
	MainView.Margin(20.0f, &MainView);

	TabBar.HSplitTop(50.0f, &Button, &TabBar);
	Button.Draw(ms_ColorTabbarActive, IGraphics::CORNER_BR, 10.0f);

	const char *apTabs[] = {
		Localize("Профиль"),
		Localize("Соклановцы"),
		Localize("Задания"),
		"",
		Localize("Python"),
		Localize("Репозиторий"),
		"",
		Localize("Уроки"),
	};
	const uint8_t CountTabs = 8;
	static CButtonContainer s_aTabButtons[CountTabs];
	static uint8_t s_SelectedTab = 0;

	for(int i = 0; i < CountTabs; i++)
	{
		if (str_comp(apTabs[i], "") == 0)
		{
			TabBar.HSplitTop(26.0f, nullptr, &TabBar);
			continue;
		}

		TabBar.HSplitTop(10.0f, nullptr, &TabBar);
		TabBar.HSplitTop(26.0f, &Button, &TabBar);
		if(DoButton_MenuTab(&s_aTabButtons[i], apTabs[i], i == s_SelectedTab, &Button, IGraphics::CORNER_R))
			s_SelectedTab = i;
	}

	if (s_SelectedTab == 0)
	{
		RenderDTHProfile(MainView);
	}

	if (s_SelectedTab == 1)
	{
		RenderDTHMembers(MainView);
	}

	if (s_SelectedTab == 2)
	{
		RenderDTHTasks(MainView);
	}

	if (s_SelectedTab == 4)
	{
		RenderDTHPython(MainView);
	}

	if (s_SelectedTab == 5)
	{
		RenderDTHRepository(MainView);
	}

	if (s_SelectedTab == 7)
	{
		RenderDTHLessons(MainView);
	}
}

void CMenus::RenderDTHProfile(CUIRect MainView)
{
	int CurrentRatingValue = GameClient()->user.userData.rating;
	int PrevRatingValue = 0;
	int NextRatingValue = 666;

	for (const DTHRating& Rating: GameClient()->dthDatabase.ratings)
	{
		if (CurrentRatingValue >= Rating.from && CurrentRatingValue <= Rating.to)
		{
			PrevRatingValue = Rating.from;
			NextRatingValue = Rating.to;
			break;
		}
	}

	float RatingProgressValue = 1.0f - (float) (NextRatingValue - CurrentRatingValue) / (NextRatingValue - PrevRatingValue);

	CTeeRenderInfo TeeRenderInfo;
	TeeRenderInfo.Apply(m_pClient->m_Skins.Find(g_Config.m_ClPlayerSkin));
	TeeRenderInfo.ApplyColors(g_Config.m_ClPlayerUseCustomColor, g_Config.m_ClPlayerColorBody, g_Config.m_ClPlayerColorFeet);
	TeeRenderInfo.m_Size = 64.0f;
	RenderDTHPlayer(MainView, TeeRenderInfo, true, true, Client()->PlayerName(), GameClient()->user.userData.getClanNameTitle());

	// Render Registration Date
	CUIRect RegistrationDate;
	MainView.VSplitLeft(48.0f, nullptr, &MainView);
	MainView.HSplitTop(10.0f, &RegistrationDate, &MainView);
	Ui()->DoLabel(&RegistrationDate, (Localize("Дата поступления в клан: ") + GameClient()->user.userData.createdAt).c_str(), 16.0f, TEXTALIGN_ML);

	// Render Rating
	string PrevRatingString = to_string(PrevRatingValue);
	string NextRatingString = to_string(NextRatingValue);
	string CurrentRatingString = to_string(CurrentRatingValue);
	CUIRect RatingProgressBar, RatingLine, RatingFrom, RatingTo, RatingCurrent, AfterRatingLine, MyRating;
	MainView.HSplitTop(20.0f, nullptr, &MainView);
	MainView.HSplitTop(16.0f, &MyRating, &MainView);
	MainView.HSplitTop(12.0f, nullptr, &MainView);
	MainView.HSplitTop(16.0f, &RatingLine, &MainView);
	MainView.HSplitTop(4.0f, &RatingProgressBar, &MainView);
	MainView.HSplitTop(4.0f, nullptr, &MainView);
	MainView.HSplitTop(16.0f, &AfterRatingLine, &MainView);
	RatingLine.VSplitLeft(TextRender()->TextWidth(16.0f, PrevRatingString.c_str()), &RatingFrom, &RatingLine);
	RatingLine.VSplitRight(TextRender()->TextWidth(16.0f, NextRatingString.c_str()), &RatingLine, &RatingTo);

	float CurrentProgressOffset = AfterRatingLine.w * RatingProgressValue - (TextRender()->TextWidth(16.0f, to_string(CurrentRatingValue).c_str())) / 2;
	float CurrentProgressStringLength = TextRender()->TextWidth(16.0f, to_string(CurrentRatingValue).c_str());

	AfterRatingLine.VSplitLeft(max(0.0f, min(CurrentProgressOffset, AfterRatingLine.w - CurrentProgressStringLength)), nullptr, &RatingCurrent);
	RatingCurrent.VSplitLeft(CurrentProgressStringLength, &RatingCurrent, nullptr);
	Ui()->RenderProgressBar(RatingProgressBar, RatingProgressValue);
	Ui()->DoLabel(&MyRating, Localize("Мой рейтинг:"), 16.0f, TEXTALIGN_MIDDLE);
	Ui()->DoLabel(&RatingFrom, PrevRatingString.c_str(), 16.0f, TEXTALIGN_MIDDLE);
	Ui()->DoLabel(&RatingTo, NextRatingString.c_str(), 16.0f, TEXTALIGN_MIDDLE);
	Ui()->DoLabel(&RatingCurrent, CurrentRatingString.c_str(), 16.0f, TEXTALIGN_MIDDLE);
}

void CMenus::RenderDTHPlayer(CUIRect &MainView, CTeeRenderInfo TeeRenderInfo, bool withBlink, bool withWatchToCursor, std::string name, std::string role)
{
	string PlayerNameText = name;
	string PlayerClanStatusText = "[" + role + "]";

	CUIRect PlayerLabel, PlayerName, PlayerClanStatus, PlayerTee;
	MainView.HSplitTop(TeeRenderInfo.m_Size * 0.75, &PlayerLabel, &MainView);

	PlayerLabel.VSplitLeft(TeeRenderInfo.m_Size * 0.75, &PlayerTee, &PlayerLabel);
	PlayerLabel.VSplitLeft(TextRender()->TextWidth(24.0f, PlayerNameText.c_str(), PlayerNameText.size()) + 8.0f, &PlayerName, &PlayerClanStatus);
	Ui()->DoLabel(&PlayerName, PlayerNameText.c_str(), 24.0f, TEXTALIGN_ML);
	Ui()->DoLabel(&PlayerClanStatus, PlayerClanStatusText.c_str(), 16.0f, TEXTALIGN_ML);

	static float s_LastBlinkTime = Client()->LocalTime() + 0.5f;
	static float s_CancelBlinkTime = Client()->LocalTime() + 0.5f;
	static int emote = 0;

	if (s_LastBlinkTime < Client()->LocalTime() && withBlink)
	{
		emote = EMOTE_BLINK;
		s_LastBlinkTime = Client()->LocalTime() + (float(rand() % 1000) / 1000.0f) + 2.0f;
		s_CancelBlinkTime = Client()->LocalTime() + (float(rand() % 100) / 1000.0f) + 0.1f;
	}

	if (s_CancelBlinkTime < Client()->LocalTime() && withBlink)
	{
		emote = 0;
	}

	RenderTools()->RenderTee(CAnimState::GetIdle(), &TeeRenderInfo, emote, withWatchToCursor ? normalize(Ui()->MousePos() - PlayerTee.Center()) : vec2(1.0f, 0.0f), PlayerTee.Center());
}

void CMenus::RenderDTHMembers(CUIRect MainView)
{
	const int countPlayers = GameClient()->dthDatabase.members.size();
	static CButtonContainer* s_ListBoxButtonContainers = new CButtonContainer[countPlayers];
	static CListBox s_ListBox;
	s_ListBox.DoStart(48.0f, countPlayers, 1, 1, -1, &MainView, true);
	int id = 0;

	for (DTHMember member : GameClient()->dthDatabase.members)
	{
		const CListboxItem Item = s_ListBox.DoNextItem(&s_ListBoxButtonContainers[id]);
		CUIRect ItemRect = Item.m_Rect;
		if (Item.m_Visible)
		{
			CTeeRenderInfo TeeRenderInfo;
			TeeRenderInfo.Apply(m_pClient->m_Skins.Find(member.skin.name.c_str()));
			TeeRenderInfo.ApplyColors(member.skin.customColors, member.skin.skinColor, member.skin.footsColor);
			TeeRenderInfo.m_Size = 64.0f;
			RenderDTHPlayer(ItemRect, TeeRenderInfo, true, false, member.name, member.getClanNameTitle());
		}

		id++;
	}

	s_ListBox.DoEnd();
}

void CMenus::RenderDTHPython(CUIRect MainView)
{
	CUIRect ListView, Button, BottomBar, ScriptBox;

	static int s_PythonSelectedScript = -1;
	static int s_ActiveScriptException = -1;

	if(true)
	{
		if(NeedToggle)
		{
			DTHRefreshPythonScripts();
			NeedToggle = false;
		}

		if(s_ActiveScriptException >= 0)
		{
			static CButtonContainer sCloseButton;
			CUIRect CloseButton;
			MainView.HSplitBottom(20.0f, &MainView, &CloseButton);
			if(DoButton_Menu(&sCloseButton, Localize("Close"), 0, &CloseButton, BUTTONFLAG_ALL, nullptr, IGraphics::CORNER_ALL))
			{
				s_ActiveScriptException = -1;
			}
			else
			{
				MainView.HSplitTop(10.0f, 0, &MainView);
				RenderDTHPythonExceptions(MainView, GameClient()->pythonScripts[s_ActiveScriptException]);
				return;
			}
		}
	}
	else
	{
		s_PythonSelectedScript = -1;
		s_ActiveScriptException = -1;
		NeedToggle = true;
	}

	MainView.VSplitLeft(MainView.w/2.0f, &ListView, &ScriptBox);
	ListView.HSplitBottom(60.0f + 5.0f, &ListView, &BottomBar);
	ListView.HSplitTop(20.0f, &Button, &ListView);

	// if(DoButton_CheckBox(&g_Config.m_DTHPython, "Enable Python Scripts", g_Config.m_DTHPython, &Button))
	// {
	// 	g_Config.m_DTHPython ^= 1;
	//
	// 	if (!g_Config.m_DTHPython) {
	//
	// 	}
	// }

	// if(!g_Config.m_DTHPython)
	// 	return;

	ListView.HSplitTop(13.0f, &MainView, &ListView); //Отступ
	//Кнопка обновления
	{
		CUIRect RefreshButton;
		static CButtonContainer s_RefreshButton;

		ListView.Draw(ms_ColorTabbarActiveOutgame, IGraphics::CORNER_ALL, 10.0f);

		Button.VSplitRight(100.0f, &Button, &RefreshButton);
		if(DoButton_Menu(&s_RefreshButton, Localize("Refresh"), 0, &RefreshButton))
		{
			DTHRefreshPythonScripts();
		}
	}
	static int ShowOnlyActive = 0;

	static CListBox listBox;
	static int ActiveScripts = 0, NumListedFiles = 0;
	CUIRect ListBox = ListView;
	listBox.DoStart(50.0f, NumListedFiles, 1, 1, -1, &ListBox, true, 0, true);


	{
		const int ScriptMaxCount = 512;

		static CButtonContainer pItemID[ScriptMaxCount];
		static CButtonContainer pIDButtonToggleScript[ScriptMaxCount];
		static CButtonContainer pIDButtonAutoLoad[ScriptMaxCount];

		int i = 0;

		for(auto &PythonScript : GameClient()->pythonScripts)
		{

			const CListboxItem Item = listBox.DoNextItem(&pItemID[i]);

			NumListedFiles++;

			if(Item.m_Visible)
			{
				CUIRect Label, Buttons;

				CUIRect Row = Item.m_Rect;

				Row.HMargin(7.0f, &Row);
				Row.HSplitTop(5.0f, 0, &Label);
				Ui()->DoLabel(&Label, PythonScript->name.c_str(), 24.0f, TEXTALIGN_LEFT);

				Row.VSplitRight(Row.h, &Row, &Button);
				if(Ui()->MouseInside(&Row) && Input()->KeyPress(KEY_MOUSE_1))
					s_PythonSelectedScript = i;

				if(i == s_PythonSelectedScript)
					Row.Draw(vec4(0.7f,0.7f,0.7f,0.5f), IGraphics::CORNER_L, 10.0f);

				if(DoButton_Menu(&pIDButtonToggleScript[i], GameClient()->pythonController.isExecutedScript(PythonScript) ? "Deactivate" : "Activate", 0, &Button,  BUTTONFLAG_ALL, nullptr, IGraphics::CORNER_R, 5.0f, 0.0f, vec4(0.0f, 0.0f, 0.0f, 0.5f)))
				{
					bool toggle = GameClient()->pythonController.isExecutedScript(PythonScript);

					if(!toggle)
						GameClient()->pythonController.StartExecuteScript(PythonScript);
					else if(toggle)
						GameClient()->pythonController.StopExecuteScript(PythonScript);
				}

				Row.VSplitRight(Row.h/2.0f, &Row, &Buttons);
				Buttons.HSplitMid(&Buttons, &Button);

				if(DoButton_CheckBox(&pIDButtonAutoLoad[i], "", GameClient()->pythonController.isScriptAutoloading(PythonScript), &Button))
				{
					bool toggle = m_pClient->pythonController.isScriptAutoloading(PythonScript);

					if(!toggle)
						this->m_pClient->pythonController.AutoloadAdd(PythonScript);
					else if(toggle)
						this->m_pClient->pythonController.AutoloadRemove(PythonScript);
				}
			}

			if(!this->m_pClient->pythonController.isExecutedScript(PythonScript) && this->m_pClient->pythonController.isScriptAutoloading(PythonScript))
			{
				if(NeedToggle)
				{
					this->m_pClient->pythonController.StartExecuteScript(PythonScript);
					NeedToggle = false;
					DTHRefreshPythonScripts();
				}
			}

			i++;
		}

		if(NumListedFiles == 0)
		{
			CUIRect Lbl;

			ListView.HSplitBottom(ListView.h/2+15.0f, 0, &Lbl);
			Ui()->DoLabel(&Lbl, "No Python scripts available. Try to click Refresh.", 20.0f, TEXTALIGN_CENTER);
		}

	}
	listBox.DoEnd();

	//Правое меню
	{
		if(s_PythonSelectedScript > -1 && s_PythonSelectedScript < GameClient()->pythonScripts.size())
		{
			static CButtonContainer s_ToggleButton, s_RefreshButton, s_ExceptionButton;
			CUIRect ToggleButton, RefreshButton, ExceptionsButton, Buttons, ExceptionBox;

			ScriptBox.VSplitLeft(15.0f, 0, &ScriptBox);
			ScriptBox.HSplitBottom(15.0f, &ScriptBox, 0);
			ScriptBox.Draw(vec4(1,1,1,0.25f), IGraphics::CORNER_ALL, 5.5f);

			PythonScript *PS = GameClient()->pythonScripts[s_PythonSelectedScript];
			bool isexecuted = m_pClient->pythonController.isExecutedScript(PS);

			ScriptBox.HSplitBottom(50.0f, &ScriptBox, &Buttons);

			Buttons.HSplitBottom(25.0f, &ScriptBox, &Buttons);
			Buttons.VSplitLeft(ScriptBox.w/NUMOF_BUTTONS, &ToggleButton, &Buttons);
			Buttons.VSplitLeft(ScriptBox.w/NUMOF_BUTTONS, &RefreshButton, &Buttons);
			Buttons.VSplitLeft(ScriptBox.w/NUMOF_BUTTONS, &ExceptionsButton, &Buttons);

			if(DoButton_Menu(&s_ToggleButton, isexecuted ? "Deactivate" : "Activate", 0, &ToggleButton, BUTTONFLAG_ALL, nullptr, IGraphics::CORNER_L))
			{
				if(isexecuted)
					m_pClient->pythonController.StopExecuteScript(PS);
				else if(!isexecuted)
					m_pClient->pythonController.StartExecuteScript(PS);
			}
			if(DoButton_Menu(&s_RefreshButton, "Refresh", 0, &RefreshButton, BUTTONFLAG_ALL, nullptr, IGraphics::CORNER_NONE))
			{
				DTHRefreshPythonScripts();
			}

			char aBuf[64];
			str_format(aBuf, sizeof(aBuf), Localize("Exceptions (%i)"), PS->fileExceptions.size());

			if(DoButton_Menu(&s_ExceptionButton, aBuf, 0, &ExceptionsButton, BUTTONFLAG_ALL, nullptr, IGraphics::CORNER_R))
			{
				static float aScrollValue= 1.0f;
				s_ActiveScriptException = s_PythonSelectedScript;
			}

		}
	}
}

std::vector<std::string> split (const std::string &s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss (s);
	std::string item;

	while(getline(ss, item, delim))
	{
		result.push_back(item);
	}

	return result;
}

void CMenus::RenderDTHPythonExceptions(CUIRect MainView, PythonScript *PS)
{
	static CListBox listBox;

	char Buf[256];
	char Bottom[32];
	char Title[256];

	str_format(Title, sizeof(Title), Localize("Script exceptions thrown by '%s'"), PS->name.c_str()); // TODO: Localize all this stuffs

	listBox.DoStart(15.0f, PS->fileExceptions.size(), 1, 1, -1, &MainView, true, IGraphics::CORNER_ALL, true);
	CButtonContainer s_ExcButtonIDs[101];

	for(int i = 0; i < PS->fileExceptions.size(); i++)
	{
		std::vector<std::string> exceptionLines = split(PS->fileExceptions[i], '\n');

		for(int line = 0; line < exceptionLines.size(); line++)
		{
			CListboxItem Item = listBox.DoNextItem(s_ExcButtonIDs, false, 0);
			if(!Item.m_Visible)
				continue;

			if(Ui()->MouseInside(&Item.m_Rect) && Input()->KeyPress(KEY_MOUSE_1))
				SDL_SetClipboardText(PS->fileExceptions[i].c_str());

			CUIRect ExceptionLine;
			Item.m_Rect.VSplitLeft(35.0f, &ExceptionLine, &Item.m_Rect);

			if(line == 0)
			{
				str_format(Buf, sizeof(Buf), "#%i", i + 1);
				Ui()->DoLabel(&ExceptionLine, Buf, 15.f, TEXTALIGN_LEFT);
			}
			else
			{
				Ui()->DoLabel(&ExceptionLine, "", 15.f, TEXTALIGN_LEFT);
			}

			Item.m_Rect.VSplitLeft(10.0f, 0, &Item.m_Rect);
			str_format(Buf, sizeof(Buf), "%s", exceptionLines[line].c_str());
			Ui()->DoLabel(&Item.m_Rect, Buf, 15.f, TEXTALIGN_LEFT);
		}
	}
	listBox.DoEnd();
}

void CMenus::RenderDTHRepository(CUIRect MainView)
{
}

void CMenus::RenderDTHTasks(CUIRect MainView)
{
	static int s_SelectedTask = -1;
	const int countTasks = GameClient()->dthDatabase.tasks.size();
	static CButtonContainer *s_ListBoxButtonContainers = new CButtonContainer[countTasks];
	static CListBox s_ListBox;

	CUIRect TasksList;
	MainView.VSplitLeft(MainView.w / 2, &TasksList, &MainView);
	MainView.VSplitLeft(8.0f, nullptr, &MainView);
	TasksList.VSplitRight(8.0f, &TasksList, nullptr);

	s_ListBox.DoStart(24.0f, countTasks, 1, 1, s_SelectedTask, &TasksList, true);
	int id = 0;

	for(const DTHTask &Task : GameClient()->dthDatabase.tasks)
	{
		const CListboxItem Item = s_ListBox.DoNextItem(&s_ListBoxButtonContainers[id]);
		CUIRect ItemRect = Item.m_Rect;
		if(Item.m_Visible)
		{
			Ui()->DoLabel(&ItemRect, split(Task.description, '\n')[0].c_str(), 16.0f, TEXTALIGN_ML);
		}

		id++;
	}

	s_SelectedTask = s_ListBox.DoEnd();

	if(s_SelectedTask >= 0)
	{
		Ui()->DoLabel(&MainView, GameClient()->dthDatabase.tasks[s_SelectedTask].description.c_str(), 16.0f, TEXTALIGN_TL);
		Ui()->DoLabel(&MainView, ("За выполнение задание рейтинг увеличится на: " + to_string(GameClient()->dthDatabase.tasks[s_SelectedTask].reward)).c_str(), 16.0f, TEXTALIGN_BL);
	}
}

void CMenus::RenderDTHLessons(CUIRect MainView)
{
		Ui()->DoLabel(&MainView, "В разработке...", 16.0f, TEXTALIGN_TL);
}

void CMenus::DTHRefreshPythonScripts()
{
	for(auto &pythonScript : GameClient()->pythonScripts)
	{
		m_pClient->pythonController.StopExecuteScript(pythonScript);
	}

	GameClient()->pythonScripts = this->scriptsScanner.scan();

	for(auto &pythonScript : GameClient()->pythonScripts)
	{
		if(m_pClient->pythonController.isScriptAutoloading(pythonScript))
			m_pClient->pythonController.StartExecuteScript(pythonScript);
	}
}

