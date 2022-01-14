#pragma once
#include "GameUser.h"
#include "UserSession.h"

class GameRoom
{
public:
	void Enter(shared_ptr<GameUser> user);
	void Leave(shared_ptr<GameUser> user);
	void Broadcast(shared_ptr<BYTE*> sendBuffer);

	bool IsStartGame() { return _startGame; }

private:
	map<int64, shared_ptr<GameUser>> _users;
	mutex _mutex;
	atomic<bool> _startGame = false;
};

extern GameRoom GRoom;

