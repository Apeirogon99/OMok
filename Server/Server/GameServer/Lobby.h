#pragma once
#include "GameUser.h"
#include "UserSession.h"

class Lobby
{
public:
	void Enter(shared_ptr<UserSession> session);
	void Leave(shared_ptr<UserSession> session);
	void Broadcast(shared_ptr<BYTE*> sendBuffer);
	bool FindUser(shared_ptr<UserSession> session);
	int64 GetUserSessionCount() { return _userSessions.size(); }

private:
	set<shared_ptr<UserSession>> _userSessions;
	mutex _mutex;
};

extern Lobby GLobby;