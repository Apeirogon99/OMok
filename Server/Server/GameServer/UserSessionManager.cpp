#include "pch.h"
#include "UserSessionManager.h"

UserSessionManager GUserSessionManger;

void UserSessionManager::Add(shared_ptr<UserSession> session)
{
	lock_guard<mutex> lockguard(_mutex);
	_userSessions.insert(session);
}

void UserSessionManager::Remove(shared_ptr<UserSession> session)
{
	lock_guard<mutex> lockguard(_mutex);
	_userSessions.erase(session);
}

void UserSessionManager::Broadcast(shared_ptr<BYTE*> sendBuffer)
{
	lock_guard<mutex> lockguard(_mutex);
	for (shared_ptr<UserSession> session : _userSessions)
	{
		session->Send(sendBuffer);
	}
}

bool UserSessionManager::FindUser(shared_ptr<UserSession> session)
{
	auto iter = _userSessions.find(session);
	return iter != _userSessions.end();
}