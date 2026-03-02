//
// Created by danii on 14.09.2023.
//

#ifndef DDNET_USER_H
#define DDNET_USER_H

#include "engine/client.h"
#include "engine/shared/http.h"
#include "game/client/component.h"

#define BACKEND_URL "https://backend.dth.dexodus.ru/"
#define BACKEND_LOGIN_ACTION "login/authentication_token"
#define BACKEND_ME_ACTION "me"
#define DTH_CLIENT_VERSION "1.1.0"

using namespace std;

class UserData
{
public:
	string clanName = "DTH";
	string latestClientVersion = "";
	int rating = 0;
	string createdAt = "";
	std::vector<std::string> pythonBlacklistIp;

	string getClanNameTitle()
	{
		if (clanName == "OWN")
		{
			return string("Создатель");
		}
		if (clanName == "A+")
		{
			return string("Администратор +");
		}
		if (clanName == "ADM")
		{
			return string("Администратор");
		}
		if (clanName == "M+")
		{
			return string("Модератор");
		}
		if (clanName == "MOD")
		{
			return string("Модератор +");
		}
		if (clanName == "ELT")
		{
			return string("Элита");
		}
		if (clanName == "VET")
		{
			return string("Ветеран");
		}
		if (clanName == "MBR")
		{
			return string("Участник");
		}
		if (clanName == "G")
		{
			return string("Гость");
		}

		return clanName;
	}
};

class User : public CComponent
{
public:
	virtual int Sizeof() const override { return sizeof(*this); }

	bool isAuthorized();
	bool login(string login, string password);
	void logout();

	void saveCredentials(string login, string password);
	void eraseCredentials();
	bool isLoginLoading();
	bool getClientVersion();
	bool getPythonBlacklist();
	pair<string, string> getCredentials();
	bool isLatestVersion();
	UserData userData;
	string token = "";
private:
	bool requestUserData();
	std::shared_ptr<CHttpRequest> loginRequest = nullptr;
	std::shared_ptr<CHttpRequest> gettingUserRequest = nullptr;
	std::shared_ptr<CHttpRequest> gettingLatestClientVersionRequest = nullptr;
	std::shared_ptr<CHttpRequest> gettingPythonBlacklist = nullptr;
};

#endif // DDNET_USER_H
