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
}

void CMenus::RenderDTHProfile(CUIRect MainView)
{
	static float timer = Client()->LocalTime();
	static int direction = 1;
	static int CurrentRatingValue = 900;
	int PrevRatingValue = 600;
	int NextRatingValue = 1200;

	if (timer < Client()->LocalTime())
	{
		CurrentRatingValue = CurrentRatingValue + direction;
		timer = Client()->LocalTime() + 0.01f;

		if (CurrentRatingValue >= NextRatingValue || CurrentRatingValue <= PrevRatingValue)
		{
			direction *= -1;
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
	MainView.VSplitLeft(32.0f, nullptr, &MainView);
	MainView.HSplitTop(10.0f, &RegistrationDate, &MainView);
	Ui()->DoLabel(&RegistrationDate, (Localize("Дата поступления в клан: ") + string("23.09.2002 (300 дней)")).c_str(), 16.0f, TEXTALIGN_ML);

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
	MainView.HSplitTop(32.0f, &PlayerLabel, &MainView);

	PlayerLabel.VSplitLeft(32.0f, &PlayerTee, &PlayerLabel);
	PlayerLabel.VSplitLeft(TextRender()->TextWidth(24.0f, PlayerNameText.c_str(), PlayerNameText.size()) + 4.0f, &PlayerName, &PlayerClanStatus);
	Ui()->DoLabel(&PlayerLabel, PlayerNameText.c_str(), 24.0f, TEXTALIGN_TL);
	Ui()->DoLabel(&PlayerClanStatus, PlayerClanStatusText.c_str(), 16.0f, TEXTALIGN_TL);

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

	RenderTools()->RenderTee(CAnimState::GetIdle(), &TeeRenderInfo, emote, withWatchToCursor ? normalize(Ui()->MousePos() - PlayerTee.TopLeft()) : vec2(1.0f, 0.0f), PlayerTee.TopLeft() + vec2(8.0f, 8.0f));
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
			ItemRect.VSplitLeft(20.0f, nullptr, &ItemRect);
			ItemRect.HSplitTop(20.0f, nullptr, &ItemRect);

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