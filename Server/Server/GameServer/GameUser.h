#pragma once
#include "UserSession.h"

class UserSession;

class GameUser
{
public:
	GameUser(shared_ptr<UserSession> userSession, uint64 userId);
	~GameUser();

public:
	shared_ptr<UserSession> _userSession;
	uint64 _userId;
};

