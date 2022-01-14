// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <OMok/Framework/BattleGameMode.h>
#include <OMok/Framework/BattleGameState.h>
#include <OMok/Framework/BattlePlayerState.h>
#include <OMok/Framework/UserController.h>
#include <OMok/Framework/UserPawn.h>
#include <OMok/ProtoFiles/Protocol.pb.h>

//class void PacketHandle::EncodePacket;

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

	PKT_S_MATCHING_GAME = 5,

	PKT_C_LEAVE_GAME = 6,
	PKT_S_LEAVE_GAME = 7,
};

bool Handle_S_LOGIN(Protocol::S_LOGIN& dpkt);
bool Handle_S_ENTER_GAME(Protocol::S_ENTER_GMAE& dpkt);
bool Handle_S_LEAVE_GAME(Protocol::S_LEAVE_GAME& dpkt);
bool Handle_S_NEXT_TURN(Protocol::S_NEXT_TURN& dpkt);

class OMOK_API PacketHandle
{
public:
	PacketHandle();
	~PacketHandle();

	static void Init(UWorld* world);

	static int32 RecviveData(BYTE* data, int32 len);
	static void DecodePacket(BYTE* buffer, PacketHeader header);


	static TArray<BYTE> MakeSendBuffer(Protocol::C_LOGIN& pkt) { return EncodePacket(pkt, PacketId::PKT_C_LOGIN); }
	static TArray<BYTE> MakeSendBuffer(Protocol::C_ENTER_GAME& pkt) { return EncodePacket(pkt, PacketId::PKT_C_ENTER_GAME); }
	static TArray<BYTE> MakeSendBuffer(Protocol::C_LEAVE_GAME& pkt) { return EncodePacket(pkt, PacketId::PKT_C_LEAVE_GAME); }

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

public:
	static ABattleGameMode* _gameMode;
};