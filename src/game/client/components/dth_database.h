//
// Created by danii on 23.02.2025.
//

#ifndef DTH_DATABASE_H
#define DTH_DATABASE_H

#include "game/client/component.h"
#include "engine/shared/http.h"
#include <vector>

struct DTHPlayer {
	std::string name;
	std::string status;
};

struct DTHSkin
{
	std::string name;
	int skinColor;
	int footsColor;
	bool customColors;
};

struct DTHMember
{
	std::string name;
	std::string role;
	DTHSkin skin;

	DTHMember()
	{
		skin.name = std::string("default");
		skin.skinColor = 0;
		skin.footsColor = 0;
		skin.customColors = false;
	}

	std::string getClanNameTitle()
	{
		if (role == "O")
		{
			return std::string("Создатель");
		}
		else if (role == "A+")
		{
			return std::string("Администратор +");
		}
		else if (role == "A")
		{
			return std::string("Администратор");
		}
		else if (role == "M+")
		{
			return std::string("Модератор");
		}
		else if (role == "M")
		{
			return std::string("Модератор +");
		}
		else if (role == "G")
		{
			return std::string("Новобранец");
		}

		return role;
	}
};

class DTHDatabase : public CComponent {
public:
	void Reset();
	void UpdateInfo();
	void OnUpdate();
	void SendKillInfo(std::string killedPlayerName);
	void SendConnectInfo(std::string address);
	void SendDisconnectInfo();
	void SendSkinInfo();
	virtual int Sizeof() const override { return sizeof(*this); }

	bool IsPlayerNameWar(std::string name);
	bool IsPlayerNamePeace(std::string name);
	bool IsClanMember(std::string name);

	std::vector<DTHPlayer> players;
	std::vector<DTHMember> members;

private:
	std::shared_ptr<CHttpRequest> getWarPlayersRequest;
	std::shared_ptr<CHttpRequest> getUsersRequest;
	std::vector<std::shared_ptr<CHttpRequest>> requests;
	bool isUpdating = false;
	float lastUpdateTime = 0.0f;
};



#endif //DTH_DATABASE_H
