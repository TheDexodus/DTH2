//
// Created by danii on 14.09.2023.
//

#ifndef DDNET_USER_H
#define DDNET_USER_H

#include "engine/client.h"
#include "engine/shared/http.h"
#include "game/client/component.h"

#define BACKEND_URL "https://backend.dth.dexodus.ru/"
#define BACKEND_LOGIN_ACTION "authentication_token"
#define BACKEND_ME_ACTION "me"

using namespace std;

class UserData
{
public:
	string clanName = "DTH";
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
	pair<string, string> getCredentials();
	UserData userData;
private:
	string token = "";
	bool requestUserData();
	std::shared_ptr<CHttpRequest> loginRequest = nullptr;
	std::shared_ptr<CHttpRequest> gettingUserRequest = nullptr;
};

#endif // DDNET_USER_H
