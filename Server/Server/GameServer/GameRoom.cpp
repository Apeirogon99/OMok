#include "pch.h"
#include "GameRoom.h"
#include "ClientPacketHandler.h"

GameRoom GRoom;

void GameRoom::Enter(shared_ptr<GameUser> user)
{
	lock_guard<mutex> lockguard(_mutex);
	if (false == IsStartGame())
	{
		_users[user->_userId] = user;
		StartGame();
	}
}

void GameRoom::Leave(shared_ptr<GameUser> user)
{
	lock_guard<mutex> lockguard(_mutex);
	_users.erase(user->_userId);
}

void GameRoom::Broadcast(shared_ptr<BYTE*> sendBuffer)
{
	lock_guard<mutex> lockguard(_mutex);
	for (auto& user : _users)
	{
		user.second->_userSession->Send(sendBuffer);
	}
}

void GameRoom::InitGame()
{
	//0		= 흑
	//1		= 백
	//-1	= 빈공간

	for (int i = 0; i < MAX_BOARD; ++i)
	{
		memset(_board[i],-1, sizeof(int) * MAX_BOARD);
	}
}

bool GameRoom::StartGame()
{
	if (_users.size() == 2)
	{
		InitGame();

		_startGame.store(true);

		for (auto iter = _users.begin(); iter != _users.end(); iter++)
		{
			Protocol::S_ENTER_GMAE pkt;
			pkt.set_success(true);
			pkt.set_playerid(iter->second->_userId);

			auto newSendBuf = ClientPacketHandler::SerializePacket(pkt, PKT_S_ENTER_GMAE);
			Broadcast(newSendBuf);
		}
	}
	

	return true;
}

uint16 GameRoom::JudgeRules(const int64 playerId, const int64 posX, const int64 posY, const bool team)
{
	//오차범위
	if (posX > MAX_BOARD || posX < 0 || posY > MAX_BOARD || posY < 0)
		return OMokRule::ERROR_RANGE;

	//이미 누가 있는가확인
	if (this->_board[posY][posX] != -1)
		return OMokRule::ERROR_HAVE;

	//방금 놓은 수의 결과

	return OMokRule::NOT_HAPPEND;
}
