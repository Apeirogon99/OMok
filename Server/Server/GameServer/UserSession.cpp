#include "pch.h"
#include "UserSession.h"
#include "ClientPacketHandler.h"

void UserSession::OnConnected()
{

}

void UserSession::OnDisconnected()
{

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
