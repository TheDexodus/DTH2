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
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

static std::string FormatDthDate(const std::string &DateTime)
{
	const bool DigitsOnly = !DateTime.empty() && std::all_of(DateTime.begin(), DateTime.end(), [](unsigned char c) { return std::isdigit(c) != 0; });
	if(DigitsOnly)
	{
		long long Timestamp = std::strtoll(DateTime.c_str(), nullptr, 10);
		if(DateTime.size() >= 13)
			Timestamp /= 1000;

		if(Timestamp > 0)
		{
			char aDate[32];
			str_timestamp_ex((time_t)Timestamp, aDate, sizeof(aDate), "%d.%m.%Y");
			return aDate;
		}
	}

	if(DateTime.size() >= 10 &&
		DateTime[4] == '-' && DateTime[7] == '-' &&
		std::isdigit((unsigned char)DateTime[0]) && std::isdigit((unsigned char)DateTime[1]) &&
		std::isdigit((unsigned char)DateTime[2]) && std::isdigit((unsigned char)DateTime[3]) &&
		std::isdigit((unsigned char)DateTime[5]) && std::isdigit((unsigned char)DateTime[6]) &&
		std::isdigit((unsigned char)DateTime[8]) && std::isdigit((unsigned char)DateTime[9]))
	{
		return DateTime.substr(8, 2) + "." + DateTime.substr(5, 2) + "." + DateTime.substr(0, 4);
	}

	const size_t TPos = DateTime.find('T');
	if(TPos != std::string::npos)
		return DateTime.substr(0, TPos);

	return DateTime;
}

static std::string DthPlayerStatusLabel(const std::string &Status)
{
	if(str_comp_nocase(Status.c_str(), "war") == 0)
		return Localize("Враждебный игрок");
	if(str_comp_nocase(Status.c_str(), "peace") == 0)
		return Localize("Мирный игрок");
	return Status;
}

static ColorRGBA DthPlayerStatusColor(const std::string &Status)
{
	if(str_comp_nocase(Status.c_str(), "war") == 0)
		return ColorRGBA(1.0f, 0.35f, 0.35f, 1.0f);
	if(str_comp_nocase(Status.c_str(), "peace") == 0)
		return ColorRGBA(1.0f, 0.85f, 0.20f, 1.0f);
	return ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f);
}

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
		Localize("Игроки"),
		"",
		Localize("Python"),
		Localize("Репозиторий"),
		"",
		Localize("Уроки"),
	};
	const uint8_t CountTabs = 9;
	static CButtonContainer s_aTabButtons[CountTabs];
	static uint8_t s_SelectedTab = 0;

	if(m_DthPlayersModalOpen)
		Ui()->SetEnabled(false);

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

	if(m_DthPlayersModalOpen)
		Ui()->SetEnabled(true);

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

	if (s_SelectedTab == 3)
	{
		RenderDTHPlayers(MainView);
	}

	if (s_SelectedTab == 5)
	{
		RenderDTHPython(MainView);
	}

	if (s_SelectedTab == 6)
	{
		RenderDTHRepository(MainView);
	}

	if (s_SelectedTab == 8)
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
	const std::string RawCreatedAt = GameClient()->user.userData.createdAt;
	const std::string FormattedCreatedAt = FormatDthDate(RawCreatedAt);
	static std::string s_LastLoggedCreatedAt;
	if(s_LastLoggedCreatedAt != RawCreatedAt)
	{
		dbg_msg("dth/profile", "createdAt raw='%s' formatted='%s'", RawCreatedAt.c_str(), FormattedCreatedAt.c_str());
		s_LastLoggedCreatedAt = RawCreatedAt;
	}
	Ui()->DoLabel(&RegistrationDate, (Localize("Дата поступления в клан: ") + FormattedCreatedAt).c_str(), 16.0f, TEXTALIGN_ML);

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

void CMenus::RenderDTHPlayers(CUIRect MainView)
{
	enum
	{
		STATUS_ALL = 0,
		STATUS_WAR,
		STATUS_PEACE,
	};

	CUIRect Screen = *Ui()->Screen();

	static CLineInputBuffered<64> s_SearchInput;
	static CLineInputBuffered<64> s_NewPlayerInput;
	static int s_StatusFilter = STATUS_ALL;
	static int s_SelectedPlayer = -1;
	static bool s_ShowAddPlayerPopup = false;
	static int s_NewPlayerStatus = STATUS_PEACE;
	static std::string s_ActionStatus;
	static float s_ActionStatusUntil = 0.0f;

	m_DthPlayersModalOpen = s_ShowAddPlayerPopup;
	if(m_DthPlayersModalOpen)
		Ui()->SetEnabled(false);

	CUIRect Header, Search, FilterRow, ListView, DetailsView, StatusRow, AddButton;
	MainView.HSplitTop(28.0f, &Header, &MainView);
	MainView.HSplitTop(6.0f, nullptr, &MainView);
	MainView.HSplitTop(30.0f, &Search, &MainView);
	MainView.HSplitTop(6.0f, nullptr, &MainView);
	MainView.HSplitTop(24.0f, &FilterRow, &MainView);
	MainView.HSplitTop(6.0f, nullptr, &MainView);
	MainView.HSplitBottom(20.0f, &MainView, &StatusRow);
	MainView.VSplitLeft(MainView.w * 0.6f, &ListView, &DetailsView);
	DetailsView.VSplitLeft(8.0f, nullptr, &DetailsView);

	Header.VSplitRight(160.0f, &Header, &AddButton);
	Ui()->DoLabel(&Header, Localize("Игроки"), 18.0f, TEXTALIGN_ML);
	static CButtonContainer s_AddPlayerButton;
	if(DoButton_Menu(&s_AddPlayerButton, Localize("Добавить игрока"), 0, &AddButton))
	{
		s_ShowAddPlayerPopup = true;
		s_NewPlayerInput.Clear();
	}

	Search.Draw(ms_ColorTabbarActiveOutgame, IGraphics::CORNER_ALL, 6.0f);
	Search.VMargin(8.0f, &Search);
	s_SearchInput.SetEmptyText(Localize("Поиск по нику"));
	Ui()->DoClearableEditBox(&s_SearchInput, &Search, 14.0f);

	CUIRect FilterAll, FilterWar, FilterPeace;
	FilterRow.VSplitLeft(110.0f, &FilterAll, &FilterRow);
	FilterRow.VSplitLeft(10.0f, nullptr, &FilterRow);
	FilterRow.VSplitLeft(110.0f, &FilterWar, &FilterRow);
	FilterRow.VSplitLeft(10.0f, nullptr, &FilterRow);
	FilterRow.VSplitLeft(110.0f, &FilterPeace, &FilterRow);
	static CButtonContainer s_FilterAll, s_FilterWar, s_FilterPeace;
	if(DoButton_MenuTab(&s_FilterAll, Localize("Все"), s_StatusFilter == STATUS_ALL, &FilterAll, IGraphics::CORNER_ALL))
		s_StatusFilter = STATUS_ALL;
	if(DoButton_MenuTab(&s_FilterWar, Localize("Враждебные"), s_StatusFilter == STATUS_WAR, &FilterWar, IGraphics::CORNER_ALL))
		s_StatusFilter = STATUS_WAR;
	if(DoButton_MenuTab(&s_FilterPeace, Localize("Мирные"), s_StatusFilter == STATUS_PEACE, &FilterPeace, IGraphics::CORNER_ALL))
		s_StatusFilter = STATUS_PEACE;

	std::vector<int> vFilteredPlayers;
	vFilteredPlayers.reserve(GameClient()->dthDatabase.players.size());
	const char *pSearch = s_SearchInput.GetString();
	for(int i = 0; i < (int)GameClient()->dthDatabase.players.size(); i++)
	{
		const DTHPlayer &Player = GameClient()->dthDatabase.players[i];
		const bool MatchStatus = s_StatusFilter == STATUS_ALL ||
			(s_StatusFilter == STATUS_WAR && str_comp_nocase(Player.status.c_str(), "war") == 0) ||
			(s_StatusFilter == STATUS_PEACE && str_comp_nocase(Player.status.c_str(), "peace") == 0);
		const bool MatchSearch = pSearch[0] == '\0' || str_find_nocase(Player.name.c_str(), pSearch) != nullptr;
		if(MatchStatus && MatchSearch)
			vFilteredPlayers.push_back(i);
	}

	if(!vFilteredPlayers.empty() && std::find(vFilteredPlayers.begin(), vFilteredPlayers.end(), s_SelectedPlayer) == vFilteredPlayers.end())
		s_SelectedPlayer = vFilteredPlayers.front();
	if(vFilteredPlayers.empty())
		s_SelectedPlayer = -1;

	static CListBox s_PlayerList;
	static CButtonContainer s_aPlayerItemButtons[2048];

	int SelectedVisible = -1;
	if(s_SelectedPlayer >= 0)
	{
		auto It = std::find(vFilteredPlayers.begin(), vFilteredPlayers.end(), s_SelectedPlayer);
		if(It != vFilteredPlayers.end())
			SelectedVisible = (int)std::distance(vFilteredPlayers.begin(), It);
	}

	const int RenderCount = minimum((int)vFilteredPlayers.size(), (int)std::size(s_aPlayerItemButtons));
	s_PlayerList.DoStart(34.0f, RenderCount, 1, 1, SelectedVisible, &ListView, true);
	for(int VisibleIndex = 0; VisibleIndex < RenderCount; VisibleIndex++)
	{
		const int PlayerIndex = vFilteredPlayers[VisibleIndex];
		const DTHPlayer &Player = GameClient()->dthDatabase.players[PlayerIndex];
		const CListboxItem Item = s_PlayerList.DoNextItem(&s_aPlayerItemButtons[VisibleIndex]);
		if(!Item.m_Visible)
			continue;

		CUIRect Row = Item.m_Rect, NameRect, StatusRect;
		Row.VMargin(6.0f, &Row);
		Row.VSplitRight(80.0f, &NameRect, &StatusRect);
		Ui()->DoLabel(&NameRect, Player.name.c_str(), 14.0f, TEXTALIGN_ML);
		TextRender()->TextColor(DthPlayerStatusColor(Player.status));
		Ui()->DoLabel(&StatusRect, DthPlayerStatusLabel(Player.status).c_str(), 13.0f, TEXTALIGN_MR);
		TextRender()->TextColor(TextRender()->DefaultTextColor());
	}
	SelectedVisible = s_PlayerList.DoEnd();
	if(SelectedVisible >= 0 && SelectedVisible < RenderCount)
		s_SelectedPlayer = vFilteredPlayers[SelectedVisible];

	DetailsView.Draw(ms_ColorTabbarActiveOutgame, IGraphics::CORNER_ALL, 6.0f);
	DetailsView.Margin(10.0f, &DetailsView);
	if(s_SelectedPlayer >= 0 && s_SelectedPlayer < (int)GameClient()->dthDatabase.players.size())
	{
		const DTHPlayer &Player = GameClient()->dthDatabase.players[s_SelectedPlayer];
		CUIRect Name, Status, DeleteRow, DeleteButton;
		DetailsView.HSplitTop(26.0f, &Name, &DetailsView);
		DetailsView.HSplitTop(20.0f, &Status, &DetailsView);
		DetailsView.HSplitTop(8.0f, nullptr, &DetailsView);
		DetailsView.HSplitTop(26.0f, &DeleteRow, &DetailsView);
		DeleteRow.VSplitRight(130.0f, nullptr, &DeleteButton);
		Ui()->DoLabel(&Name, Player.name.c_str(), 18.0f, TEXTALIGN_ML);
		CUIRect StatusLabelRect, StatusValueRect;
		const char *pStatusPrefix = Localize("Статус: ");
		const float PrefixWidth = TextRender()->TextWidth(14.0f, pStatusPrefix);
		Status.VSplitLeft(PrefixWidth, &StatusLabelRect, &StatusValueRect);
		Ui()->DoLabel(&StatusLabelRect, pStatusPrefix, 14.0f, TEXTALIGN_ML);
		TextRender()->TextColor(DthPlayerStatusColor(Player.status));
		Ui()->DoLabel(&StatusValueRect, DthPlayerStatusLabel(Player.status).c_str(), 14.0f, TEXTALIGN_ML);
		TextRender()->TextColor(TextRender()->DefaultTextColor());
		static CButtonContainer s_DeletePlayerCardButton;
		if(DoButton_Menu(&s_DeletePlayerCardButton, Localize("Удалить"), 0, &DeleteButton))
		{
			s_ActionStatus = Localize("Удаление игрока пока не реализовано");
			s_ActionStatusUntil = Client()->LocalTime() + 2.5f;
		}
	}
	else
	{
		Ui()->DoLabel(&DetailsView, Localize("Нет игроков по текущим фильтрам"), 14.0f, TEXTALIGN_MC);
	}

	if(s_ActionStatusUntil > Client()->LocalTime())
		Ui()->DoLabel(&StatusRow, s_ActionStatus.c_str(), 13.0f, TEXTALIGN_ML);
	else
	{
		char aInfo[128];
		str_format(aInfo, sizeof(aInfo), Localize("Найдено игроков: %d"), (int)vFilteredPlayers.size());
		Ui()->DoLabel(&StatusRow, aInfo, 13.0f, TEXTALIGN_ML);
	}

	if(m_DthPlayersModalOpen)
	{
		Ui()->SetEnabled(true);
		// Pseudo blur: layered translucent wash instead of heavy real blur pass.
		Screen.Draw(ColorRGBA(0.0f, 0.0f, 0.0f, 0.18f), IGraphics::CORNER_ALL, 0.0f);
		Screen.Draw(ColorRGBA(1.0f, 1.0f, 1.0f, 0.05f), IGraphics::CORNER_ALL, 0.0f);
		Screen.Draw(ColorRGBA(0.0f, 0.0f, 0.0f, 0.18f), IGraphics::CORNER_ALL, 0.0f);

		CUIRect Popup = Screen;
		Popup.VMargin(Screen.w * 0.36f, &Popup);
		Popup.HMargin(Screen.h * 0.34f, &Popup);
		Popup.Draw(ms_ColorTabbarActiveOutgame, IGraphics::CORNER_ALL, 8.0f);
		Popup.Margin(12.0f, &Popup);

		CUIRect Title, NameLabel, NameInput, StatusLabel, StatusButtons, Buttons, CancelButton, SubmitButton;
		Popup.HSplitTop(24.0f, &Title, &Popup);
		Popup.HSplitTop(8.0f, nullptr, &Popup);
		Popup.HSplitTop(18.0f, &NameLabel, &Popup);
		Popup.HSplitTop(26.0f, &NameInput, &Popup);
		Popup.HSplitTop(8.0f, nullptr, &Popup);
		Popup.HSplitTop(18.0f, &StatusLabel, &Popup);
		Popup.HSplitTop(26.0f, &StatusButtons, &Popup);
		Popup.HSplitBottom(28.0f, &Popup, &Buttons);
		Buttons.VSplitMid(&CancelButton, &SubmitButton, 12.0f);

		Ui()->DoLabel(&Title, Localize("Добавление игрока"), 17.0f, TEXTALIGN_ML);
		Ui()->DoLabel(&NameLabel, Localize("Ник"), 13.0f, TEXTALIGN_ML);
		s_NewPlayerInput.SetEmptyText(Localize("Введите ник игрока"));
		Ui()->DoClearableEditBox(&s_NewPlayerInput, &NameInput, 13.0f);
		Ui()->DoLabel(&StatusLabel, Localize("Статус"), 13.0f, TEXTALIGN_ML);

		CUIRect PeaceButton, WarButton;
		StatusButtons.VSplitLeft(StatusButtons.w * 0.5f - 6.0f, &PeaceButton, &StatusButtons);
		StatusButtons.VSplitLeft(12.0f, nullptr, &StatusButtons);
		WarButton = StatusButtons;
		static CButtonContainer s_PeaceButton, s_WarButton, s_CancelAddPlayer, s_SubmitAddPlayer;
		if(DoButton_MenuTab(&s_PeaceButton, Localize("Мирный игрок"), s_NewPlayerStatus == STATUS_PEACE, &PeaceButton, IGraphics::CORNER_ALL))
			s_NewPlayerStatus = STATUS_PEACE;
		if(DoButton_MenuTab(&s_WarButton, Localize("Враждебный игрок"), s_NewPlayerStatus == STATUS_WAR, &WarButton, IGraphics::CORNER_ALL))
			s_NewPlayerStatus = STATUS_WAR;

		if(DoButton_Menu(&s_CancelAddPlayer, Localize("Отмена"), 0, &CancelButton))
		{
			s_ShowAddPlayerPopup = false;
		}

		if(DoButton_Menu(&s_SubmitAddPlayer, Localize("Добавить"), 0, &SubmitButton))
		{
			const std::string Name = s_NewPlayerInput.GetString();
			const std::string Status = s_NewPlayerStatus == STATUS_WAR ? "war" : "peace";
			if(GameClient()->dthDatabase.AddPlayer(Name, Status))
			{
				s_ActionStatus = Localize("Запрос на добавление игрока отправлен");
				s_ActionStatusUntil = Client()->LocalTime() + 2.5f;
				s_ShowAddPlayerPopup = false;
				s_NewPlayerInput.Clear();
				GameClient()->dthDatabase.RefreshNow();
			}
			else
			{
				s_ActionStatus = Localize("Ошибка запроса. Проверьте ник и авторизацию.");
				s_ActionStatusUntil = Client()->LocalTime() + 2.5f;
			}
		}
	}

	m_DthPlayersModalOpen = s_ShowAddPlayerPopup;
	Ui()->SetEnabled(true);
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
	struct SRepoScript
	{
		const char *m_pName;
		const char *m_pFileName;
		const char *m_pAuthor;
		const char *m_pTags;
		const char *m_pUpdatedAt;
		const char *m_pDescription;
		const char *m_pUrl;
		int m_Stars;
		int m_Installs;
		bool m_Verified;
	};

	static const std::array<SRepoScript, 8> s_aRepoScripts = {{
		{"Auto Reply", "auto_reply.py", "dexodus", "chat,automation", "2026-02-14", "Reply rules and smart auto-responses for common server events.", "https://github.com/dth-scripts/auto-reply", 41, 220, true},
		{"Aim Trainer Overlay", "aim_trainer_overlay.py", "sk1llz", "hud,training", "2026-01-28", "Visual helper overlay for hook timing and crosshair discipline.", "https://github.com/dth-scripts/aim-trainer-overlay", 68, 344, true},
		{"Clan Radar", "clan_radar.py", "danii", "clan,ui", "2026-02-21", "Highlights clanmates nearby and adds compact minimap markers.", "https://github.com/dth-scripts/clan-radar", 55, 291, true},
		{"Task Tracker", "task_tracker.py", "atlas", "tasks,ui", "2026-02-10", "Shows task progress in-game with optional HUD widget.", "https://github.com/dth-scripts/task-tracker", 37, 178, false},
		{"Demo Auto Save", "demo_auto_save.py", "mori", "demo,automation", "2025-12-19", "Stores demos by map and date with configurable naming presets.", "https://github.com/dth-scripts/demo-auto-save", 24, 133, false},
		{"War Feed", "war_feed.py", "kappa", "war,notifications", "2026-02-03", "Displays war/peace status feed and recent opponent activity.", "https://github.com/dth-scripts/war-feed", 47, 205, true},
		{"Ping Guard", "ping_guard.py", "volt", "network,stability", "2025-11-11", "Warns about unstable latency and recommends anti-ping profile.", "https://github.com/dth-scripts/ping-guard", 18, 96, false},
		{"Skin Switcher Plus", "skin_switcher_plus.py", "elon", "skins,quality-of-life", "2026-01-07", "Quickly switches preset skins based on map and server tags.", "https://github.com/dth-scripts/skin-switcher-plus", 33, 149, false},
	}};

	static CLineInputBuffered<64> s_SearchInput;
	static int s_SelectedRepo = 0;
	static int s_OnlyVerified = 0;
	static int s_OnlyInstalled = 0;
	static std::string s_ActionStatus;
	static float s_ActionStatusUntil = 0.0f;

	CUIRect Header, Content, LeftPanel, RightPanel, SearchBar, Filters, Stats;
	MainView.HSplitTop(28.0f, &Header, &MainView);
	MainView.HSplitTop(6.0f, nullptr, &MainView);
	MainView.HSplitTop(32.0f, &SearchBar, &MainView);
	MainView.HSplitTop(6.0f, nullptr, &MainView);
	MainView.HSplitTop(24.0f, &Filters, &MainView);
	MainView.HSplitTop(8.0f, nullptr, &MainView);
	MainView.HSplitTop(20.0f, &Stats, &MainView);
	MainView.HSplitTop(8.0f, nullptr, &MainView);
	Content = MainView;

	Ui()->DoLabel(&Header, Localize("Python Script Repository"), 20.0f, TEXTALIGN_ML);

	SearchBar.Draw(ms_ColorTabbarActiveOutgame, IGraphics::CORNER_ALL, 6.0f);
	SearchBar.VMargin(8.0f, &SearchBar);
	s_SearchInput.SetEmptyText(Localize("Search by name, tag, author"));
	Ui()->DoClearableEditBox(&s_SearchInput, &SearchBar, 14.0f);

	CUIRect VerifiedFilter, InstalledFilter, RefreshButton;
	Filters.VSplitLeft(220.0f, &VerifiedFilter, &Filters);
	Filters.VSplitLeft(220.0f, &InstalledFilter, &Filters);
	Filters.VSplitRight(120.0f, &Filters, &RefreshButton);
	if(DoButton_CheckBox(&s_OnlyVerified, Localize("Verified only"), s_OnlyVerified, &VerifiedFilter))
		s_OnlyVerified ^= 1;
	if(DoButton_CheckBox(&s_OnlyInstalled, Localize("Installed only"), s_OnlyInstalled, &InstalledFilter))
		s_OnlyInstalled ^= 1;
	static CButtonContainer s_RefreshRepo;
	if(DoButton_Menu(&s_RefreshRepo, Localize("Refresh"), 0, &RefreshButton))
	{
		DTHRefreshPythonScripts();
		s_ActionStatus = Localize("Local scripts list refreshed");
		s_ActionStatusUntil = Client()->LocalTime() + 2.5f;
	}

	Content.VSplitLeft(Content.w * 0.58f, &LeftPanel, &RightPanel);
	RightPanel.VSplitLeft(8.0f, nullptr, &RightPanel);

	std::vector<int> vFiltered;
	vFiltered.reserve(s_aRepoScripts.size());
	const char *pFilter = s_SearchInput.GetString();
	for(int i = 0; i < (int)s_aRepoScripts.size(); i++)
	{
		const SRepoScript &Script = s_aRepoScripts[i];
		bool Installed = false;
		for(const auto *pLocalScript : GameClient()->pythonScripts)
		{
			if(str_find_nocase(pLocalScript->name.c_str(), Script.m_pFileName) != nullptr ||
				str_find_nocase(pLocalScript->filepath.c_str(), Script.m_pFileName) != nullptr)
			{
				Installed = true;
				break;
			}
		}

		const bool MatchFilter = pFilter[0] == '\0' ||
			str_find_nocase(Script.m_pName, pFilter) != nullptr ||
			str_find_nocase(Script.m_pAuthor, pFilter) != nullptr ||
			str_find_nocase(Script.m_pTags, pFilter) != nullptr;
		const bool MatchVerified = !s_OnlyVerified || Script.m_Verified;
		const bool MatchInstalled = !s_OnlyInstalled || Installed;
		if(MatchFilter && MatchVerified && MatchInstalled)
			vFiltered.push_back(i);
	}

	int InstalledCount = 0;
	for(const auto &Script : s_aRepoScripts)
	{
		for(const auto *pLocalScript : GameClient()->pythonScripts)
		{
			if(str_find_nocase(pLocalScript->name.c_str(), Script.m_pFileName) != nullptr ||
				str_find_nocase(pLocalScript->filepath.c_str(), Script.m_pFileName) != nullptr)
			{
				InstalledCount++;
				break;
			}
		}
	}

	char aStats[256];
	str_format(aStats, sizeof(aStats), Localize("Shown: %d   Installed: %d/%d"), (int)vFiltered.size(), InstalledCount, (int)s_aRepoScripts.size());
	Ui()->DoLabel(&Stats, aStats, 14.0f, TEXTALIGN_ML);

	if(!vFiltered.empty() && std::find(vFiltered.begin(), vFiltered.end(), s_SelectedRepo) == vFiltered.end())
		s_SelectedRepo = vFiltered.front();
	if(vFiltered.empty())
		s_SelectedRepo = -1;

	static CListBox s_RepoList;
	static CButtonContainer s_aRepoButtons[32];
	int SelectedVisibleIndex = -1;
	if(s_SelectedRepo >= 0)
	{
		auto It = std::find(vFiltered.begin(), vFiltered.end(), s_SelectedRepo);
		if(It != vFiltered.end())
			SelectedVisibleIndex = (int)std::distance(vFiltered.begin(), It);
	}

	s_RepoList.DoStart(56.0f, (int)vFiltered.size(), 1, 1, SelectedVisibleIndex, &LeftPanel, true);
	for(const int RepoIndex : vFiltered)
	{
		const CListboxItem Item = s_RepoList.DoNextItem(&s_aRepoButtons[RepoIndex]);
		if(!Item.m_Visible)
			continue;

		const SRepoScript &Script = s_aRepoScripts[RepoIndex];
		bool Installed = false;
		for(const auto *pLocalScript : GameClient()->pythonScripts)
		{
			if(str_find_nocase(pLocalScript->name.c_str(), Script.m_pFileName) != nullptr ||
				str_find_nocase(pLocalScript->filepath.c_str(), Script.m_pFileName) != nullptr)
			{
				Installed = true;
				break;
			}
		}

		CUIRect Row = Item.m_Rect, Top, Bottom, InstalledBadge;
		Row.VMargin(8.0f, &Row);
		Row.HSplitTop(24.0f, &Top, &Bottom);
		Top.VSplitRight(110.0f, &Top, &InstalledBadge);

		std::string Title = std::string(Script.m_pName) + (Script.m_Verified ? "  [verified]" : "");
		Ui()->DoLabel(&Top, Title.c_str(), 16.0f, TEXTALIGN_ML);
		Ui()->DoLabel(&Bottom, Script.m_pTags, 13.0f, TEXTALIGN_ML);
		Ui()->DoLabel(&InstalledBadge, Installed ? Localize("Installed") : Localize("Not installed"), 13.0f, TEXTALIGN_MR);
	}

	SelectedVisibleIndex = s_RepoList.DoEnd();
	if(SelectedVisibleIndex >= 0 && SelectedVisibleIndex < (int)vFiltered.size())
		s_SelectedRepo = vFiltered[SelectedVisibleIndex];

	RightPanel.Draw(ms_ColorTabbarActiveOutgame, IGraphics::CORNER_ALL, 8.0f);
	RightPanel.Margin(10.0f, &RightPanel);

	if(s_SelectedRepo < 0 || s_SelectedRepo >= (int)s_aRepoScripts.size())
	{
		Ui()->DoLabel(&RightPanel, Localize("No scripts found by current filters"), 16.0f, TEXTALIGN_MC);
		return;
	}

	const SRepoScript &SelectedScript = s_aRepoScripts[s_SelectedRepo];
	bool SelectedInstalled = false;
	for(const auto *pLocalScript : GameClient()->pythonScripts)
	{
		if(str_find_nocase(pLocalScript->name.c_str(), SelectedScript.m_pFileName) != nullptr ||
			str_find_nocase(pLocalScript->filepath.c_str(), SelectedScript.m_pFileName) != nullptr)
		{
			SelectedInstalled = true;
			break;
		}
	}

	CUIRect Title, Meta, Description, ButtonsRow, StatusRow;
	RightPanel.HSplitTop(26.0f, &Title, &RightPanel);
	RightPanel.HSplitTop(20.0f, &Meta, &RightPanel);
	RightPanel.HSplitTop(8.0f, nullptr, &RightPanel);
	RightPanel.HSplitTop(80.0f, &Description, &RightPanel);
	RightPanel.HSplitTop(12.0f, nullptr, &RightPanel);
	RightPanel.HSplitTop(28.0f, &ButtonsRow, &RightPanel);
	RightPanel.HSplitTop(8.0f, nullptr, &RightPanel);
	RightPanel.HSplitTop(20.0f, &StatusRow, &RightPanel);

	Ui()->DoLabel(&Title, SelectedScript.m_pName, 20.0f, TEXTALIGN_ML);
	char aMeta[256];
	str_format(aMeta, sizeof(aMeta), "Author: %s | Stars: %d | Installs: %d | Updated: %s", SelectedScript.m_pAuthor, SelectedScript.m_Stars, SelectedScript.m_Installs, SelectedScript.m_pUpdatedAt);
	Ui()->DoLabel(&Meta, aMeta, 14.0f, TEXTALIGN_ML);
	Ui()->DoLabel(&Description, SelectedScript.m_pDescription, 14.0f, TEXTALIGN_TL);

	CUIRect InstallButton, UpdateButton, OpenButton;
	ButtonsRow.VSplitLeft(120.0f, &InstallButton, &ButtonsRow);
	ButtonsRow.VSplitLeft(10.0f, nullptr, &ButtonsRow);
	ButtonsRow.VSplitLeft(120.0f, &UpdateButton, &ButtonsRow);
	ButtonsRow.VSplitLeft(10.0f, nullptr, &ButtonsRow);
	OpenButton = ButtonsRow;

	static CButtonContainer s_InstallButton, s_UpdateButton, s_OpenButton;
	if(DoButton_Menu(&s_InstallButton, SelectedInstalled ? Localize("Reinstall") : Localize("Install"), 0, &InstallButton))
	{
		DTHRefreshPythonScripts();
		s_ActionStatus = Localize("Repository integration is in progress. Local scripts list was refreshed.");
		s_ActionStatusUntil = Client()->LocalTime() + 3.0f;
	}
	if(DoButton_Menu(&s_UpdateButton, Localize("Update"), 0, &UpdateButton))
	{
		DTHRefreshPythonScripts();
		s_ActionStatus = Localize("Repository integration is in progress. Local scripts list was refreshed.");
		s_ActionStatusUntil = Client()->LocalTime() + 3.0f;
	}
	if(DoButton_Menu(&s_OpenButton, Localize("Open URL"), 0, &OpenButton))
	{
		Client()->ViewLink(SelectedScript.m_pUrl);
	}

	if(s_ActionStatusUntil > Client()->LocalTime())
		Ui()->DoLabel(&StatusRow, s_ActionStatus.c_str(), 13.0f, TEXTALIGN_ML);
	else
		Ui()->DoLabel(&StatusRow, SelectedInstalled ? Localize("Status: installed locally") : Localize("Status: not installed locally"), 13.0f, TEXTALIGN_ML);
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




