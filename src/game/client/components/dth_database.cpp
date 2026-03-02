//
// Created by danii on 23.02.2025.
//

#include "dth_database.h"

#include "game/client/gameclient.h"

#include <engine/shared/json.h>

void DTHDatabase::Reset()
{
	players.clear();
	members.clear();
}

void DTHDatabase::UpdateInfo()
{
	if (!GameClient()->user.isAuthorized())
	{
		return;
	}

	if (!this->isUpdating)
	{
		this->getWarPlayersRequest = HttpGet("https://backend.dth.dexodus.ru/api/players?pagination=false");
		this->getWarPlayersRequest->LogProgress(HTTPLOG::FAILURE);
		Http()->Run(this->getWarPlayersRequest);

		this->getUsersRequest = HttpGet("https://backend.dth.dexodus.ru/api/users?pagination=false");
		this->getUsersRequest->LogProgress(HTTPLOG::FAILURE);
		Http()->Run(this->getUsersRequest);

		this->getRatingsRequest = HttpGet("https://backend.dth.dexodus.ru/ratings");
		this->getRatingsRequest->LogProgress(HTTPLOG::FAILURE);
		Http()->Run(this->getRatingsRequest);

		this->getTasksRequest = HttpGet("https://backend.dth.dexodus.ru/tasks");
		this->getTasksRequest->LogProgress(HTTPLOG::FAILURE);
		Http()->Run(this->getTasksRequest);

		this->isUpdating = true;
		return;
	}

	if (this->getWarPlayersRequest != nullptr)
	{
		if (this->getWarPlayersRequest->Done())
		{
			json_value* resultJson = this->getWarPlayersRequest->ResultJson();
			this->getWarPlayersRequest = nullptr;

			if (resultJson)
			{
				const json_value &Json = *resultJson;
				const json_value& members = Json["hydra:member"];

				for (size_t i = 0; i < members.u.array.length; i++)
				{
					const json_value& entry = *members.u.array.values[i];
					DTHPlayer player;
					player.name = json_string_get(&entry["playerName"]);
					player.status = json_string_get(&entry["status"]);
					this->players.push_back(player);
				}
			}
		}
	}
	else if (this->getUsersRequest != nullptr)
	{
		if (this->getUsersRequest->Done())
		{
			json_value* resultJson = this->getUsersRequest->ResultJson();
			this->getUsersRequest = nullptr;

			if (resultJson)
			{
				const json_value &Json = *resultJson;
				const json_value& members = Json["hydra:member"];

				for (size_t i = 0; i < members.u.array.length; i++)
				{
					const json_value& entry = *members.u.array.values[i];
					DTHMember member;
					member.name = json_string_get(&entry["email"]);
					member.role = json_string_get(&entry["clanRole"]);

					if (entry["skin"].type == json_object)
					{
						member.skin.name = std::string(json_string_get(&entry["skin"]["skinName"]));
						member.skin.skinColor = json_int_get(&entry["skin"]["skinColor"]);
						member.skin.footsColor = json_int_get(&entry["skin"]["footsColor"]);
						member.skin.customColors = json_boolean_get(&entry["skin"]["customColors"]);
					}

					this->members.push_back(member);
				}
			}
		}
	}
	else if (this->getRatingsRequest != nullptr)
	{
		if (this->getRatingsRequest->Done())
		{
			json_value* ResultJson = this->getRatingsRequest->ResultJson();
			this->getRatingsRequest = nullptr;

			if (ResultJson)
			{
				const json_value &Json = *ResultJson;

				for (size_t i = 0; i < Json.u.array.length; i++)
				{
					const json_value& Entry = *Json.u.array.values[i];
					DTHRating Rating;
					Rating.name = json_string_get(&Entry["name"]);
					Rating.value = json_string_get(&Entry["value"]);
					Rating.title = json_string_get(&Entry["title"]);
					Rating.from = json_int_get(&Entry["from"]);
					Rating.to = json_int_get(&Entry["to"]);

					this->ratings.push_back(Rating);
				}
			}
		}
	}
	else if (this->getTasksRequest != nullptr)
	{
		if (this->getTasksRequest->Done())
		{
			json_value* ResultJson = this->getTasksRequest->ResultJson();
			this->getTasksRequest = nullptr;

			if (ResultJson)
			{
				const json_value &Json = *ResultJson;

				for (size_t i = 0; i < Json.u.array.length; i++)
				{
					const json_value& Entry = *Json.u.array.values[i];
					DTHTask Task;
					Task.description = json_string_get(&Entry["description"]);
					Task.reward = json_int_get(&Entry["reward"]);

					this->tasks.push_back(Task);
				}
			}
		}
	}
	else
	{
		this->isUpdating = false;
	}
}

void DTHDatabase::OnUpdate()
{
	if(this->isUpdating)
	{
		UpdateInfo();
	}
	else if(this->lastUpdateTime <= GameClient()->Client()->LocalTime() && GameClient()->user.isAuthorized())
	{
		this->lastUpdateTime = GameClient()->Client()->LocalTime() + 120.0f;
		Reset();
		UpdateInfo();
	}
}

void DTHDatabase::SendKillInfo(std::string killedPlayerName)
{
	CServerInfo CurrentServerInfo;
	Client()->GetServerInfo(&CurrentServerInfo);
	dbg_msg("KILL SEND", killedPlayerName.c_str());
	shared_ptr<CHttpRequest> request = HttpGet(("https://backend.dth.dexodus.ru/kill/" + killedPlayerName).c_str());
	string authHeader = "Bearer " + GameClient()->user.token;
	request->HeaderString("Authorization", authHeader.c_str());
	request->LogProgress(HTTPLOG::FAILURE);
	Http()->Run(request);
}

void DTHDatabase::SendConnectInfo(std::string address)
{
	CServerInfo CurrentServerInfo;
	Client()->GetServerInfo(&CurrentServerInfo);
	shared_ptr<CHttpRequest> request = HttpGet(("https://backend.dth.dexodus.ru/connect/" + address).c_str());
	string authHeader = "Bearer " + GameClient()->user.token;
	request->HeaderString("Authorization", authHeader.c_str());
	request->LogProgress(HTTPLOG::FAILURE);
	Http()->Run(request);
}

void DTHDatabase::SendDisconnectInfo()
{
	shared_ptr<CHttpRequest> request = HttpGet("https://backend.dth.dexodus.ru/disconnect");
	string authHeader = "Bearer " + GameClient()->user.token;
	request->HeaderString("Authorization", authHeader.c_str());
	request->LogProgress(HTTPLOG::FAILURE);
	Http()->Run(request);
}

void DTHDatabase::SendSkinInfo()
{
	const CSkin* skin = m_pClient->m_Skins.Find(g_Config.m_ClPlayerSkin);
	shared_ptr<CHttpRequest> request = HttpGet((std::string("https://backend.dth.dexodus.ru/change-skin/") + skin->GetName() + "/" + to_string(g_Config.m_ClPlayerColorBody) + "/" + to_string(g_Config.m_ClPlayerColorFeet) + "/" + (g_Config.m_ClPlayerUseCustomColor ? "1" : "0")).c_str());
	string authHeader = "Bearer " + GameClient()->user.token;
	request->HeaderString("Authorization", authHeader.c_str());
	request->LogProgress(HTTPLOG::FAILURE);
	Http()->Run(request);
}

bool DTHDatabase::IsPlayerNameWar(std::string name)
{
	for (DTHPlayer player : this->players)
	{
		if (player.name == name) {
			return player.status == "war";
		}
	}

	return false;
}

bool DTHDatabase::IsPlayerNamePeace(std::string name)
{
	for (DTHPlayer player : this->players)
	{
		if (player.name == name) {
			return player.status == "peace";
		}
	}

	return false;
}

bool DTHDatabase::IsClanMember(std::string name)
{
	for (DTHMember member : this->members)
	{
		if (member.name == name) {
			return true;
		}
	}

	return false;
}