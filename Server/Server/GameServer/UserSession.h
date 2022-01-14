#pragma once
#include "MyCompltionKey.h"

class UserSession : public MyCompltionKey
{
public:
	virtual ~UserSession();

	virtual void		OnConnected() override;
	virtual void		OnDisconnected() override;
	virtual int32		OnRecv(BYTE* buffer, int32 len) override;
	virtual void		OnSend(int32 len) override;

public:

};

