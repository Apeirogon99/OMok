#pragma once
#include "GameRoom.h"

class GameRoom;

class GameRoomManager
{
public:
	//ROOM
	void Add(shared_ptr<GameRoom> gameRoom);
	void Remove(shared_ptr<GameRoom> gameRoom);
	void Broadcast(shared_ptr<BYTE*> sendBuffer);
	shared_ptr<GameRoom> FindEmptyGameRoom();
	
public:
	//USER
	void WaitGameStart(shared_ptr<GameUser> gameUser);

public:
	mutex _mutex;
	map<int32,shared_ptr<GameRoom>> _GameRooms;
	atomic<int32> _GameRoomsCount = 0;

	queue<shared_ptr<GameUser>> _GameUsers;

	//TEMP
	int32 _maxEnterGameUser = 2; 
};

extern GameRoomManager GGameRoomManager;
