#pragma once
#include "UserSession.h"

class UserSession;

class UserSessionManager
{
public:
	void Add(shared_ptr<UserSession> session);
	void Remove(shared_ptr<UserSession> session);
	void Broadcast(shared_ptr<BYTE*> sendBuffer);
	bool FindUser(shared_ptr<UserSession> session);

public:
	mutex _mutex;
	set<shared_ptr<UserSession>> _userSessions;
};

extern UserSessionManager GUserSessionManger;
