#pragma once
#include "GameUser.h"
#include "UserSession.h"

class GameRoom
{
public:
	void Enter(shared_ptr<GameUser> user);
	void Leave(shared_ptr<GameUser> user);
	void Broadcast(BYTE* sendBuffer);

private:
	map<int64, shared_ptr<GameUser>> _users;
	mutex _mutex;
};

extern GameRoom GRoom;

