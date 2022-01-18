#include "pch.h"
#include "Lobby.h"

Lobby GLobby;

void Lobby::Enter(shared_ptr<UserSession> session)
{
	lock_guard<mutex> lockguard(_mutex);
	_userSessions.insert(session);
}

void Lobby::Leave(shared_ptr<UserSession> session)
{
	lock_guard<mutex> lockguard(_mutex);
	_userSessions.erase(session);
}

void Lobby::Broadcast(shared_ptr<BYTE*> sendBuffer)
{
	lock_guard<mutex> lockguard(_mutex);
	for (shared_ptr<UserSession> session : _userSessions)
	{
		session->Send(sendBuffer);
	}
}

bool Lobby::FindUser(shared_ptr<UserSession> session)
{
	auto iter = _userSessions.find(session);
	return iter != _userSessions.end();
}