#include "pch.h"
#include "GameRoomManager.h"

GameRoomManager GGameRoomManager;

void GameRoomManager::Add(shared_ptr<GameRoom> gameRoom)
{
    ::lock_guard<mutex> lg(_mutex);
    _GameRooms.insert(pair<int32, shared_ptr<GameRoom>>(_GameRoomsCount++, gameRoom));
}

void GameRoomManager::Remove(shared_ptr<GameRoom> gameRoom)
{
    map<int32, shared_ptr<GameRoom>> ::iterator iter;
    for (iter = _GameRooms.begin(); iter != _GameRooms.end(); ++iter)
    {
        _GameRooms.erase(iter++);
    }
}

void GameRoomManager::Broadcast(shared_ptr<BYTE*> sendBuffer)
{
    for (auto room : _GameRooms)
    {
        if (true == room.second->IsStartGame())
        {
            room.second->Broadcast(sendBuffer);
        }
    }
}

shared_ptr<GameRoom> GameRoomManager::FindEmptyGameRoom()
{
    map<int32, shared_ptr<GameRoom>> ::iterator iter;
    for (iter = _GameRooms.begin(); iter != _GameRooms.end(); ++iter)
    {
        if (false == iter->second->IsStartGame())
            return iter->second;
    }

    return nullptr;
}

void GameRoomManager::WaitGameStart(shared_ptr<GameUser> gameUser)
{
    _GameUsers.push(gameUser);
    if (_GameUsers.size() > _maxEnterGameUser)
    {
        for (int i = 0; i < _GameUsers.size(); ++i)
        {
            FindEmptyGameRoom()->Enter(_GameUsers.front());

            //TODO : MAKE_SENDBUFFER
            //gameUser->_userSession->Send();

            _GameUsers.pop();
        }
    }
}
