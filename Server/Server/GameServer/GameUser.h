#pragma once
#include "UserSession.h"

class UserSession;

class GameUser
{
public:
	GameUser(weak_ptr<UserSession> userSession, uint64 userId);
	~GameUser();

public:
	weak_ptr<UserSession> _userSession;
	uint64 _userId;
};

