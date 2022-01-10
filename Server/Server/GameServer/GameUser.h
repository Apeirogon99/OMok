#pragma once
#include "UserSession.h"

class GameUser
{

public:
	shared_ptr<UserSession> _userSession = nullptr;
	int64 _userId;
};

