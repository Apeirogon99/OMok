#include "pch.h"
#include "GameUser.h"

GameUser::GameUser(weak_ptr<UserSession> userSession, uint64 userId)
{
	_userSession = userSession;
	_userId = userId;
}

GameUser::~GameUser()
{
}
