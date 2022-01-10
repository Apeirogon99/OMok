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

int32 PacketHandle::RecviveData(TArray<uint8> data)
{
    int32 processLen = 0;

    while (true)
    {
        if (data.Num() < sizeof(PacketHeader))
            break;

        //TEMP
        PacketHeader header = *(reinterpret_cast<PacketHeader*>(&data[processLen]));
        
        if (data.Num() < header.size)
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
    case PKT_C_LOGIN:
        Handle_C_LOGIN(buffer, header.size);
        break;
    default:
        break;
    }
}

void PacketHandle::EncodePacket()
{
    //const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
    //const uint16 packetSize = dataSize + sizeof(PacketHeader);

    //SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);
    //PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());

    //header->size = packetSize;
    //header->id = pktId;
    //ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
    //sendBuffer->Close(packetSize);

    //return sendBuffer;
}

bool Handle_C_LOGIN(BYTE* buffer, int32 len)
{
    //Protocol::S_LOGIN = parsePacket<Protocol::S_LOGIN>(buffer,len);
    return false;
}
