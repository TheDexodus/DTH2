//
// Created by Esquad on 06.09.2023.
//

#include <engine/graphics.h>
#include <engine/keys.h>
#include <engine/serverbrowser.h>
#include <engine/textrender.h>

#include <engine/client/updater.h>
#include <engine/shared/config.h>

#include <game/client/gameclient.h>
#include <game/client/ui.h>

#include <game/generated/client_data.h>
#include <game/localization.h>

#include "menus.h"

#include <chrono>

using namespace std::chrono_literals;

void CMenus::RenderLoginMenu(CUIRect MainView)
{
	if(GameClient()->user.isAuthorized())
		return;

	static int init = 0;
	static int rememberMe = 0;
	static bool ShowInvalidCredentials = false;
	static bool ShowNotLatestVersion = false;

	static std::string errlogin = "";
	static std::string errpass = "";

	if (init == 0) {
		init = 1;
		auto credentials = GameClient()->user.getCredentials();
		strcpy(m_Login, credentials.first.c_str());
		strcpy(m_Pass, credentials.second.c_str());

		if (strlen(m_Login) > 0 && strlen(m_Pass) > 0) {
			rememberMe = 1;
		}
	}

	CUIRect Button, LoginButton, AbortButton, PassLine, LoginLine, Label;
	CUIRect Box;
	static CButtonContainer s_Login, s_Abort;

	const float VMargin = MainView.w / 2 - 265.0f;

	MainView.VMargin(VMargin, &Box);

	Box.HSplitTop(30.0f, &Button, &Box);
	Box.Draw(ms_ColorTabbarActiveOutgame, IGraphics::CORNER_ALL, 10.0f);
	Ui()->DoLabel(&Button, Localize("You need login to use this client."), 23.0f, TEXTALIGN_CENTER);
	Box.HSplitBottom(35.0f, &Box, &Button);
	Box.HSplitBottom(35.0f, &Button, &Box);

	Box.VSplitMid(&LoginButton, &AbortButton, Box.h/2);

	LoginButton.VSplitLeft(50.0f, &Button, &LoginButton);
	AbortButton.VSplitRight(50.0f, &AbortButton, &Button);

	if (GameClient()->user.isLoginLoading())
	{
		bool loggedIn = GameClient()->user.login(string(m_Login), string(m_Pass));

		if (!GameClient()->user.isLoginLoading())
		{
			if (loggedIn && rememberMe) {
				GameClient()->user.saveCredentials(string(m_Login), string(m_Pass));
			}

			if (!rememberMe) {
				GameClient()->user.eraseCredentials();
			}

			if (!loggedIn) {
				// show "Invalid Credentials"
				ShowInvalidCredentials = true;
				errlogin = m_Login;
				errpass = m_Pass;
			}
			else if (!GameClient()->user.isLatestVersion())
			{
				ShowNotLatestVersion = true;
			}
		}
	}

	if(DoButton_Menu(&s_Login, Localize("Log in"), 0, &LoginButton, BUTTONFLAG_ALL, nullptr, IGraphics::CORNER_ALL, 5.0f, 0.f) && !GameClient()->user.isLoginLoading())
	{
		GameClient()->user.login(string(m_Login), string(m_Pass));
	}

	if(DoButton_Menu(&s_Abort, Localize("Quit"), 0, &AbortButton, BUTTONFLAG_ALL, nullptr, IGraphics::CORNER_ALL, 5.0f, 0.f))
		Client()->Quit();

	CUIRect LoginBox;

	MainView.VMargin(VMargin, &LoginBox);

	if(ShowInvalidCredentials)
	{
		CUIRect InvalidCredsLabel;
		LoginBox.HSplitTop(100.0f, &LoginBox, &InvalidCredsLabel);
		Ui()->DoLabel(&InvalidCredsLabel, Localize("Invalid credentials"), 24.0f, TEXTALIGN_CENTER);

		if(errlogin != m_Login || errpass != m_Pass)
		{
			ShowInvalidCredentials = false;
		}
	}

	if (ShowNotLatestVersion)
	{
		CUIRect InvalidCredsLabel;
		LoginBox.HSplitTop(100.0f, &LoginBox, &InvalidCredsLabel);
		Ui()->DoLabel(&InvalidCredsLabel, Localize("Not latest version of DTH Client"), 24.0f, TEXTALIGN_CENTER);
	}

	if (GameClient()->user.isLoginLoading())
	{
		CUIRect LoginLoadingLabel;
		LoginBox.HSplitTop(100.0f, &LoginBox, &LoginLoadingLabel);
		Ui()->DoLabel(&LoginLoadingLabel, Localize("Is logging"), 24.0f, TEXTALIGN_CENTER);
	}

	LoginBox.HSplitTop(LoginBox.w/3.0f, 0, &LoginBox);

	LoginBox.HSplitTop(17.0f, &Label, &LoginBox);
	Ui()->DoLabel(&Label, Localize("Login"), 24.0f, TEXTALIGN_CENTER);
	LoginBox.HSplitTop(24.0f, &LoginLine, &LoginBox);
	LoginBox.HSplitTop(24.0f, &LoginLine, &LoginBox);

	LoginLine.HSplitTop(50.0f, &LoginLine, 0);
	LoginLine.HSplitBottom(50.0f, 0, &LoginLine);

	LoginLine.VSplitLeft(50.0f, 0, &LoginLine);
	LoginLine.VSplitRight(50.0f, &LoginLine, 0);


	LoginBox.HSplitTop(50.0f, &PassLine, &LoginBox);
	LoginBox.HSplitTop(17.0f, &Label, &LoginBox);
	Ui()->DoLabel(&Label, Localize("Password"), 24.0f, TEXTALIGN_CENTER);
	LoginBox.HSplitTop(24.0f, &PassLine, &LoginBox);
	LoginBox.HSplitTop(7.0f, &PassLine, &LoginBox);

	PassLine.HSplitTop(50.0f, &PassLine, 0);
	PassLine.HSplitBottom(50.0f, 0, &PassLine);

	PassLine.VSplitLeft(50.0f, 0, &PassLine);
	PassLine.VSplitRight(50.0f, &PassLine, 0);

	LoginBox.HSplitTop(80.0f, &LoginBox, &Button);

	Button.HSplitTop(20.0f, &Button, 0);
	Button.HSplitBottom(20.0f, 0, &Button);

	Button.VSplitLeft(50.0f, 0, &Button);
	Button.VSplitRight(50.0f, &Button, 0);

	if(DoButton_CheckBox(&rememberMe, Localize("Remember me"), rememberMe, &Button))
		rememberMe ^= 1;


	m_LogInLogin.SetBuffer(m_Login, sizeof(m_Login));
	Ui()->DoClearableEditBox(&m_LogInLogin, &LoginLine, 18.0f);

	m_LogInPassword.SetBuffer(m_Pass, sizeof(m_Pass));
	m_LogInPassword.SetHidden(true);
	Ui()->DoClearableEditBox(&m_LogInPassword, &PassLine, 12.0f);
}