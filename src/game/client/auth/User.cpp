//
// Created by danii on 14.09.2023.
//

#include "User.h"

#include "engine/engine.h"
#include "engine/shared/http.h"
#include "game/client/gameclient.h"
#include <engine/shared/json.h>
#include <filesystem>
#include <fstream>

bool User::login(string login, string password)
{
	if (loginRequest == nullptr)
	{
		string body = "{\"email\": \"" + login + "\", \"password\": \"" + password + "\"}";

		string url = string(BACKEND_URL) + BACKEND_LOGIN_ACTION;
		loginRequest = HttpPostJson(url.c_str(), body.c_str());
		loginRequest->LogProgress(HTTPLOG::FAILURE);
		Http()->Run(loginRequest);
		return false;
	}

	if (loginRequest != nullptr && !loginRequest->Done()) {
		return false;
	}

	json_value* resultJson = loginRequest->ResultJson();

	if (!resultJson) {
		loginRequest = nullptr;
		return false;
	}

	const json_value &Json = *resultJson;
	const json_value &TokenString = Json["token"];

	this->token = json_string_get(&TokenString);

	return this->requestUserData();
}

bool User::isAuthorized()
{
	return this->token.size() > 0;
}

string encrypt(string str, char key) {
	for (int i = 0; i < str.size(); i++) {
		str[i] ^= key;
	}

	return str;
}

string decrypt(string str, char key) {
	for (int i = 0; i < str.size(); i++) {
		str[i] ^= key;
	}

	return str;
}

void User::saveCredentials(string login, string password)
{
	ofstream authFile("auth.cfg");

	if (authFile.is_open()) {
		authFile << encrypt(login, 'l') << '\n' << encrypt(password, 'p') << '\n';
		authFile.close();
	}
}

void User::eraseCredentials()
{
	const std::filesystem::path file_path{"auth.cfg"};

	try
	{
		std::filesystem::remove(file_path);
	}
	catch(const std::filesystem::filesystem_error &e)
	{
	}
}

bool User::isLoginLoading()
{
	return loginRequest != nullptr;
}

pair<string, string> User::getCredentials()
{
	ifstream authFile("auth.cfg");
	string login, password;

	if (authFile.is_open()) {
		getline(authFile, login);
		getline(authFile, password);
		authFile.close();
	}

	return pair<string, string>(decrypt(login, 'l'), decrypt(password, 'p'));
}

void User::logout()
{
	this->eraseCredentials();
	this->token = "";
}

bool User::requestUserData()
{
	if (gettingUserRequest == nullptr)
	{
		string url = string(BACKEND_URL) + BACKEND_ME_ACTION;
		gettingUserRequest = HttpGet(url.c_str());
		string authHeader = "Bearer " + this->token;
		gettingUserRequest->HeaderString("Authorization", authHeader.c_str());
		gettingUserRequest->LogProgress(HTTPLOG::FAILURE);
		Http()->Run(gettingUserRequest);
		return false;
	}

	if (gettingUserRequest != nullptr && !gettingUserRequest->Done())
	{
		return false;
	}

	json_value* resultJson = gettingUserRequest->ResultJson();
	loginRequest = nullptr;
	gettingUserRequest = nullptr;

	if (!resultJson) {
		return false;
	}

	const json_value &Json = *resultJson;
	this->userData.clanName = json_string_get(&Json["clanName"]);

	return true;
}
