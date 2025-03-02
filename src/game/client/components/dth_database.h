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
		if (role == "OWN")
		{
			return std::string("Создатель");
		}
		if (role == "A+")
		{
			return std::string("Администратор +");
		}
		if (role == "ADM")
		{
			return std::string("Администратор");
		}
		if (role == "M+")
		{
			return std::string("Модератор");
		}
		if (role == "MOD")
		{
			return std::string("Модератор +");
		}
		if (role == "ELT")
		{
			return std::string("Элита");
		}
		if (role == "VET")
		{
			return std::string("Ветеран");
		}
		if (role == "MBR")
		{
			return std::string("Участник");
		}
		if (role == "G")
		{
			return std::string("Гость");
		}

		return role;
	}
};

struct DTHRating
{
	std::string name;
	std::string value;
	std::string title;
	int from;
	int to;
};

struct DTHTask
{
	std::string description;
	int reward;
};

struct DTHPythonScript
{
	std::string filename;
	std::string name;
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
	std::vector<DTHRating> ratings;
	std::vector<DTHTask> tasks;

private:
	std::shared_ptr<CHttpRequest> getWarPlayersRequest;
	std::shared_ptr<CHttpRequest> getUsersRequest;
	std::shared_ptr<CHttpRequest> getRatingsRequest;
	std::shared_ptr<CHttpRequest> getTasksRequest;
	std::shared_ptr<CHttpRequest> getPythonScriptsRequest;
	std::vector<std::shared_ptr<CHttpRequest>> requests;
	bool isUpdating = false;
	float lastUpdateTime = 0.0f;
};



#endif //DTH_DATABASE_H
