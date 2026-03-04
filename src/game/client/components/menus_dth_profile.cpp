//
// Created by danii on 12.02.2025.
//
#include "menus_dth_shared.h"

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


