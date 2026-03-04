//
// Created by danii on 12.02.2025.
//
#include "menus_dth_shared.h"

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

void CMenus::DTHSetModalOpen(bool Open)
{
	m_DthPlayersModalOpen = Open;
}

bool CMenus::DTHRenderModal(CUIRect Screen, CUIRect &Popup, float WidthMarginRatio, float HeightMarginRatio, float InnerMargin, float Rounding)
{
	if(!m_DthPlayersModalOpen)
		return false;

	Ui()->SetEnabled(true);

	// Pseudo blur: layered translucent wash instead of heavy real blur pass.
	Screen.Draw(ColorRGBA(0.0f, 0.0f, 0.0f, 0.18f), IGraphics::CORNER_ALL, 0.0f);
	Screen.Draw(ColorRGBA(1.0f, 1.0f, 1.0f, 0.05f), IGraphics::CORNER_ALL, 0.0f);
	Screen.Draw(ColorRGBA(0.0f, 0.0f, 0.0f, 0.18f), IGraphics::CORNER_ALL, 0.0f);

	Popup = Screen;
	Popup.VMargin(Screen.w * WidthMarginRatio, &Popup);
	Popup.HMargin(Screen.h * HeightMarginRatio, &Popup);
	Popup.Draw(ms_ColorTabbarActiveOutgame, IGraphics::CORNER_ALL, Rounding);
	Popup.Margin(InnerMargin, &Popup);
	return true;
}


