// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <OMok/Framework/BattleGameMode.h>
#include <OMok/Framework/BattleGameState.h>

struct PacketHeader
{
	uint16 id;
	uint16 size;
};

enum PacketId : uint32
{
	PKT_C_LOGIN = 1000,

};

bool Handle_C_LOGIN(BYTE* buffer, int32 len);


//static BYTE* MakeSendBuffer(Protocol::S_LOGIN& pkt) { return EncodePacket(pkt, PKT_S_LOGIN); }

class OMOK_API PacketHandle
{
public:
	PacketHandle();
	~PacketHandle();

	static void Init(UWorld* world);

	static int32 RecviveData(TArray<uint8> data);
	static void DecodePacket(BYTE* buffer, PacketHeader header);
	static void EncodePacket();

	template<typename PacketType>
	static PacketType parsePacket(BYTE* buffer, int32 len)
	{
		PacketType pkt;
		//TODO : pkt parse
		return pkt;
	}

private:
	static ABattleGameMode* _gameMode;
	//static ABattleGameState* _gameState;
};
