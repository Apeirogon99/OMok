#pragma once
#include "GameUser.h"
#include "UserSession.h"
#include "ClientPacketHandler.h"

enum OMokRule : uint16
{
	NONE = 0,
	ERROR_RANGE = 1,
	ERROR_HAVE = 2,
	NOT_HAPPEND = 3,
	WINNER = 4,
};

class GameRoom
{
private:
	//TEMP
	enum
	{
		MAX_BOARD = 15,
	};


public:
	void Enter(shared_ptr<GameUser> user);
	void Leave(shared_ptr<GameUser> user);
	void Broadcast(shared_ptr<BYTE*> sendBuffer);
	bool IsStartGame() { return _startGame; }

public:
	void	InitGame();
	bool	StartGame();
	Protocol::BoardGameResult	JudgeRules(const int64 playerId, const int64 posX, const int64 posY, const bool team);

private:
	map<int64, shared_ptr<GameUser>> _users;
	mutex _mutex;
	atomic<bool> _startGame = false;
	int64 _board[MAX_BOARD][MAX_BOARD];
};

extern GameRoom GRoom;

