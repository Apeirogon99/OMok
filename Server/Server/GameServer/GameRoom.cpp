#include "pch.h"
#include "GameRoom.h"

GameRoom GRoom;

void GameRoom::Enter(shared_ptr<GameUser> user)
{
	lock_guard<mutex> lockguard(_mutex);
	_users[user->_userId] = user;
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
		memset(_board[i], -1, sizeof(int) * MAX_BOARD);
	}
}

bool GameRoom::StartGame()
{
	lock_guard<mutex> lockguard(_mutex);
	if (_users.size() == 1)
	{
		InitGame();

		printf("GameStart\n");

		_startGame.store(true);


		/*Protocol::S_MATCHING_GAME pkt;
		pkt.set_success(true);
		auto newSendBuf = ClientPacketHandler::SerializePacket(pkt, PKT_S_MATCHING_GAME);*/

		//Broadcast(newSendBuf);
		
	}


	return true;
}

Protocol::BoardGameResult GameRoom::JudgeRules(const int64 playerId, const int64 posX, const int64 posY, const bool team)
{
	lock_guard<mutex> lockguard(_mutex);
	//오차범위
	if (posX > MAX_BOARD || posX < 0 || posY > MAX_BOARD || posY < 0)
		return Protocol::BoardGameResult::ERROR_RANGE;

	//이미 누가 있는가확인
	if (this->_board[posY][posX] != -1)
		return Protocol::BoardGameResult::ERROR_HAVE;

	//방금 놓은 수의 결과

	return Protocol::BoardGameResult::NOT_HAPPEND;
}