//
// Created by danii on 12.02.2025.
//
#include "menus_dth_shared.h"

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

static std::vector<std::string> split (const std::string &s, char delim) {
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
