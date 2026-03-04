//
// Created by danii on 12.02.2025.
//
#include "menus_dth_shared.h"

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

	DTHSetModalOpen(s_ShowAddPlayerPopup);
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

	CUIRect Popup;
	if(DTHRenderModal(Screen, Popup))
	{
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

	DTHSetModalOpen(s_ShowAddPlayerPopup);
	Ui()->SetEnabled(true);
}

