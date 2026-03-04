//
// Created by danii on 12.02.2025.
//
#include "menus_dth_shared.h"

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


