// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketHandle.h"

ABattleGameMode* PacketHandle::_gameMode = nullptr;

PacketHandle::PacketHandle()
{
}

PacketHandle::~PacketHandle()
{
}

void PacketHandle::Init(UWorld* world)
{
    if (world->GetWorld())
    {
        _gameMode = Cast<ABattleGameMode>(world->GetWorld()->GetAuthGameMode());
    }

    if (_gameMode)
    {
        UE_LOG(LogTemp, Warning, TEXT("PacketHandle Ready"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PacketHandle not Ready"));
    }
}

int32 PacketHandle::RecviveData(BYTE* data, int32 len)
{
    int32 processLen = 0;
    int32 dataSize = len;

    while (true)
    {
        dataSize -= processLen;

        if (dataSize < sizeof(PacketHeader))
            break;

        //TEMP
        PacketHeader header = *(reinterpret_cast<PacketHeader*>(&data[processLen]));

        UE_LOG(LogTemp, Warning, TEXT("ID : %d, SIZE : %d"), header.id,header.size);
        
        if (dataSize < header.size)
            break;
        
        DecodePacket(&data[processLen], header);

        processLen += header.size;
    }

    return processLen;
}

void PacketHandle::DecodePacket(BYTE* buffer, PacketHeader header)
{
    switch (header.id)
    {
    case PKT_S_LOGIN:
        parsePacket<Protocol::S_LOGIN>(Handle_S_LOGIN,buffer, header.size);
        break;
    case PKT_S_ENTER_GAME:
        parsePacket<Protocol::S_ENTER_GMAE>(Handle_S_ENTER_GAME, buffer, header.size);
        break;
    case PKT_S_LEAVE_GAME:
        parsePacket<Protocol::S_LEAVE_GAME>(Handle_S_LEAVE_GAME, buffer, header.size);
        break;
    default:
        break;
    }
}

bool Handle_S_LOGIN(Protocol::S_LOGIN& pkt)
{
    UE_LOG(LogTemp, Warning, TEXT("Handle_C_LOGIN - id : %d"),pkt.playerid());
    return true;
}

bool Handle_S_ENTER_GAME(Protocol::S_ENTER_GMAE& pkt)
{
    UE_LOG(LogTemp, Warning, TEXT("Handle_C_ENTER_GAME"));
    
    return true;
}

bool Handle_S_LEAVE_GAME(Protocol::S_LEAVE_GAME& pkt)
{
    UE_LOG(LogTemp, Warning, TEXT("Handle_C_LEAVE_GAME"));
    
    return true;
}
