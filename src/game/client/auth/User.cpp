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
		string body = "{\"login\": \"" + login + "\", \"password\": \"" + password + "\"}";

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
	return !this->token.empty() && loginRequest == nullptr && gettingUserRequest == nullptr && gettingLatestClientVersionRequest == nullptr && gettingPythonBlacklist == nullptr && isLatestVersion();
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
	return loginRequest != nullptr || gettingUserRequest != nullptr || gettingLatestClientVersionRequest != nullptr || gettingPythonBlacklist != nullptr;
}

bool User::getClientVersion()
{
	if (gettingLatestClientVersionRequest == nullptr)
	{
		string url = string(BACKEND_URL) + "clients/latest-version";
		gettingLatestClientVersionRequest = HttpGet(url.c_str());
		string authHeader = "Bearer " + this->token;
		// gettingLatestClientVersionRequest->HeaderString("Authorization", authHeader.c_str());
		gettingLatestClientVersionRequest->LogProgress(HTTPLOG::FAILURE);
		Http()->Run(gettingLatestClientVersionRequest);
		return false;
	}

	if (gettingLatestClientVersionRequest != nullptr && !gettingLatestClientVersionRequest->Done())
	{
		return false;
	}

	json_value* resultJson = gettingLatestClientVersionRequest->ResultJson();

	if (!resultJson) {
		return false;
	}

	const json_value &Json = *resultJson;
	this->userData.latestClientVersion = json_string_get(&Json["version"]);

	return this->getPythonBlacklist();
}

bool User::getPythonBlacklist()
{
	if (gettingPythonBlacklist == nullptr)
	{
		string url = string(BACKEND_URL) + "api/server_with_python_ignores?pagination=false";
		gettingPythonBlacklist = HttpGet(url.c_str());
		gettingPythonBlacklist->LogProgress(HTTPLOG::FAILURE);
		Http()->Run(gettingPythonBlacklist);
		return false;
	}

	if (gettingPythonBlacklist != nullptr && !gettingPythonBlacklist->Done())
	{
		return false;
	}

	json_value* resultJson = gettingPythonBlacklist->ResultJson();

	if (!resultJson) {
		return false;
	}

	loginRequest = nullptr;
	gettingUserRequest = nullptr;
	gettingLatestClientVersionRequest = nullptr;
	gettingPythonBlacklist = nullptr;

	const json_value &Json = *resultJson;
	const json_value& members = Json["hydra:member"];

	if (members.type != json_array) {
		return false;
	}

	for (size_t i = 0; i < members.u.array.length; i++) {
		const json_value& entry = *members.u.array.values[i];
		if (entry["ipAddress"].type == json_string) {
			this->userData.pythonBlacklistIp.push_back(json_string_get(&entry["ipAddress"]));
		}
	}

	return true;
}

pair<string, string> User::getCredentials()
{
	ifstream authFile("auth.cfg");
	string login, password;

	if(authFile.is_open())
	{
		getline(authFile, login);
		getline(authFile, password);
		authFile.close();
	}

	return pair<string, string>(decrypt(login, 'l'), decrypt(password, 'p'));
}

bool User::isLatestVersion()
{
	return this->userData.latestClientVersion == DTH_CLIENT_VERSION;
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

	if (!resultJson) {
		return false;
	}

	const json_value &Json = *resultJson;
	this->userData.clanName = json_string_get(&Json["clanName"]);
	this->userData.createdAt = json_string_get(&Json["createdAt"]);
	this->userData.rating = json_int_get(&Json["rating"]);
	if (this->gettingLatestClientVersionRequest == nullptr && this->gettingPythonBlacklist == nullptr)
	{
		GameClient()->dthDatabase.UpdateInfo();
		GameClient()->dthDatabase.SendSkinInfo();
	}

	return getClientVersion();
}
