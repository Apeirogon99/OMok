#pragma once
#include "UserSession.h"

class UserSession;

class UserSessionManager
{
	void Add(shared_ptr<UserSession> session);
	void Remove(shared_ptr<UserSession> session);
	void Broadcast(BYTE* sendBuffer);

public:
	mutex _mutex;
	set<shared_ptr<UserSession>> _userSessions;
};

extern UserSessionManager GUserSessionManger;
