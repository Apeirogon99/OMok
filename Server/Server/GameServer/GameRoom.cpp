#include "pch.h"
#include "GameRoom.h"

GameRoom GRoom;

void GameRoom::Enter(shared_ptr<GameUser> user)
{
	lock_guard<mutex> lockguard(_mutex);
	_users[user->_userId] = user;
}

void GameRoom::Leave(shared_ptr<GameUser> user)
{
	lock_guard<mutex> lockguard(_mutex);
	_users.erase(user->_userId);
}

void GameRoom::Broadcast(shared_ptr<BYTE*> sendBuffer)
{
	lock_guard<mutex> lockguard(_mutex);
	for (auto& user : _users)
	{
		user.second->_userSession->Send(sendBuffer);
	}
}
