//
// Created by danii on 12.02.2025.
//
#include "menus_dth_shared.h"

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


