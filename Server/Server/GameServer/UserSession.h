#pragma once
#include "MyCompltionKey.h"
#include "GameUser.h"

class GameUser;

class UserSession : public MyCompltionKey
{
public:
	virtual ~UserSession();

	virtual void			OnConnected() override;
	virtual void			OnDisconnected() override;
	virtual int32			OnRecv(BYTE* buffer, int32 len) override;
	virtual void			OnSend(int32 len) override;

	void					CreateUser(shared_ptr<UserSession> userSession, uint64 userId);
	shared_ptr<GameUser>	GetUser() { return _GameUser; }

public:
	shared_ptr<GameUser>	_GameUser;
};

