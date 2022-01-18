#include "pch.h"
#include "UserSession.h"
#include "UserSessionManager.h"
#include "ClientPacketHandler.h"

UserSession::~UserSession()
{
}

void UserSession::OnConnected()
{
	//printf("OnConnected UserSessionn", len);
	GUserSessionManger.Add(static_pointer_cast<UserSession>(shared_from_this()));
}

void UserSession::OnDisconnected()
{
	//printf("OnDisconnected UserSessionn", len);
	GUserSessionManger.Remove(static_pointer_cast<UserSession>(shared_from_this()));
}

int32 UserSession::OnRecv(BYTE* buffer, int32 len)
{
	shared_ptr<MyCompltionKey> compltionKey = static_pointer_cast<MyCompltionKey>(shared_from_this());
	return ClientPacketHandler::cheackPacket(compltionKey, buffer, len);
}

void UserSession::OnSend(int32 len)
{
	//printf("OnSend : %d\n", len);
}

bool UserSession::CreateUser(shared_ptr<UserSession> userSession, uint64 userId)
{
	if (_GameUser)
	{
		return false;
	}
	else
	{
		_GameUser = make_shared<GameUser>(userSession, userId);
		return true;
	}
}