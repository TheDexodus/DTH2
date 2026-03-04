//
// Created by danii on 12.02.2025.
//
#include "menus_dth_shared.h"

static std::vector<std::string> DthSplitString(const std::string &s, char delim)
{
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while(getline(ss, item, delim))
	{
		result.push_back(item);
	}

	return result;
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
			Ui()->DoLabel(&ItemRect, DthSplitString(Task.description, '\n')[0].c_str(), 16.0f, TEXTALIGN_ML);
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


