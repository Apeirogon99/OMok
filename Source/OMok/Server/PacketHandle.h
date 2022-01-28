// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <OMok/Framework/BattleGameMode.h>
#include <OMok/Framework/BattleGameState.h>
#include <OMok/Framework/BattlePlayerState.h>
#include <OMok/Framework/UserController.h>
#include <OMok/ProtoFiles/Protocol.pb.h>
#include <OMok/OMokGameInstance.h>

struct PacketHeader
{
	uint16 id;
	uint16 size;
};

enum PacketId : uint16
{
	PKT_C_LOGIN = 1,
	PKT_S_LOGIN = 2,

	PKT_C_ENTER_GAME = 3,
	PKT_S_ENTER_GAME = 4,

	PKT_C_MATCHING_GAME = 5,
	PKT_S_MATCHING_GAME = 6,

	PKT_C_LEAVE_GAME = 7,
	PKT_S_LEAVE_GAME = 8,

	PKT_C_NEXT_TURN = 9,
	PKT_S_NEXT_TURN = 10,

	PKT_C_CHAT_LOOBY = 11,
	PKT_S_CHAT_LOOBY = 12,
};

bool Handle_S_LOGIN(Protocol::S_LOGIN& dpkt);
bool Handle_S_ENTER_GAME(Protocol::S_ENTER_GMAE& dpkt);
bool Handle_S_LEAVE_GAME(Protocol::S_LEAVE_GAME& dpkt);
bool Handle_S_NEXT_TURN(Protocol::S_NEXT_TURN& dpkt);
bool Handle_S_MATCHING_GAME(Protocol::S_MATCHING_GAME& dpkt);
bool Handle_S_CHAT_LOBBY(Protocol::S_CHAT_LOBBY& dpkt);

class OMOK_API PacketHandle
{
public:
	PacketHandle();
	~PacketHandle();

	static void Init(UWorld* world)
	{
		if (world->GetWorld())
			_gameMode = Cast<ABattleGameMode>(world->GetWorld()->GetAuthGameMode());

		if (_gameMode)
			UE_LOG(LogTemp, Warning, TEXT("PacketHandle Ready"));
	}

	static int32 RecviveData(BYTE* data, int32 len);
	static void DecodePacket(BYTE* buffer, PacketHeader header);


	static TArray<BYTE> MakeSendBuffer(Protocol::C_LOGIN& pkt) { return EncodePacket(pkt, PacketId::PKT_C_LOGIN); }
	static TArray<BYTE> MakeSendBuffer(Protocol::C_ENTER_GAME& pkt) { return EncodePacket(pkt, PacketId::PKT_C_ENTER_GAME); }
	static TArray<BYTE> MakeSendBuffer(Protocol::C_LEAVE_GAME& pkt) { return EncodePacket(pkt, PacketId::PKT_C_LEAVE_GAME); }
	static TArray<BYTE> MakeSendBuffer(Protocol::C_NEXT_TURN& pkt) { return EncodePacket(pkt, PacketId::PKT_C_NEXT_TURN); }
	static TArray<BYTE> MakeSendBuffer(Protocol::C_MATCHING_GAME& pkt) { return EncodePacket(pkt, PacketId::PKT_C_MATCHING_GAME); }
	static TArray<BYTE> MakeSendBuffer(Protocol::C_CHAT_LOBBY& pkt) { return EncodePacket(pkt, PacketId::PKT_C_CHAT_LOOBY); }

	template<typename ProtoType>
	static TArray<BYTE> EncodePacket(ProtoType& pkt, PacketId id)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		TArray<BYTE> sendBuffer;
		sendBuffer.Init(0, packetSize);

		PacketHeader* header = reinterpret_cast<PacketHeader*>(&sendBuffer[0]);
		header->size = packetSize;
		header->id = id;

		if (false == pkt.SerializeToArray(&header[1], dataSize))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed EncodePacket"));
			sendBuffer.Empty();
		}

		//TEMP
		SendPacketBuffer(sendBuffer);

		return sendBuffer;
	}

	template<typename ProtoType, typename ProcessFunc>
	static bool parsePacket(ProcessFunc func, BYTE* buffer, int32 len)
	{
		ProtoType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed parsePacket"));
			return false;
		}

		return func(pkt);
	}

	static bool SendPacketBuffer(TArray<BYTE> buffer)
	{
		if(_gameMode->_networkCore->IsConnect())
			_gameMode->_networkCore->SendData(buffer);

		return true;
	}

public:
	static ABattleGameMode* _gameMode;
	//static ABattleGameState* _gameState;
};