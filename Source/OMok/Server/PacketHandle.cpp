// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketHandle.h"

ABattleGameMode* PacketHandle::_gameMode = nullptr;

PacketHandle::PacketHandle()
{
}

PacketHandle::~PacketHandle()
{
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
    case PKT_S_NEXT_TURN:
        parsePacket<Protocol::S_NEXT_TURN>(Handle_S_NEXT_TURN, buffer, header.size);
        break;
    case PKT_S_MATCHING_GAME:
        parsePacket<Protocol::S_MATCHING_GAME>(Handle_S_MATCHING_GAME, buffer, header.size);
        break;
    case PKT_S_CHAT_LOOBY:
        parsePacket<Protocol::S_CHAT_LOBBY>(Handle_S_CHAT_LOBBY, buffer, header.size);
        break;

    default:
        break;
    }
}

bool Handle_S_LOGIN(Protocol::S_LOGIN& dpkt)
{
    UE_LOG(LogTemp, Warning, TEXT("Handle_C_LOGIN"));

    UOMokGameInstance* gameInstance = PacketHandle::_gameMode->GetGameInstance<UOMokGameInstance>();
    if (gameInstance)
    {
        gameInstance->_nickName = dpkt.nickname().c_str();
        gameInstance->_playerId = dpkt.playerid();
    }

    return true;
}

bool Handle_S_ENTER_GAME(Protocol::S_ENTER_GMAE& dpkt)
{
    UE_LOG(LogTemp, Warning, TEXT("Handle_C_ENTER_GAME"));
    
    return true;
}

bool Handle_S_LEAVE_GAME(Protocol::S_LEAVE_GAME& dpkt)
{
    UE_LOG(LogTemp, Warning, TEXT("Handle_C_LEAVE_GAME"));
    
    return true;
}

bool Handle_S_NEXT_TURN(Protocol::S_NEXT_TURN& dpkt)
{
    UE_LOG(LogTemp, Warning, TEXT("Handle_S_NEXT_TURN"));
    
    return false;
}

bool Handle_S_MATCHING_GAME(Protocol::S_MATCHING_GAME& dpkt)
{
    UE_LOG(LogTemp, Warning, TEXT("Handle_S_MATCHING_GAME"));
    return true;
}

bool Handle_S_CHAT_LOBBY(Protocol::S_CHAT_LOBBY& dpkt)
{
    //CHAT
    UE_LOG(LogTemp, Warning, TEXT("Handle_S_CHAT_LOBBY"));
    
    FText nickname = FText::FromString(dpkt.playernickname().c_str());
    FText message = FText::FromString(dpkt.message().c_str());

    UE_LOG(LogTemp, Warning, TEXT("name : %s, message : %s"),*nickname.ToString(), *message.ToString());
    
    AUserController* UC = Cast<AUserController>(PacketHandle::_gameMode->GetWorld()->GetFirstPlayerController());
    if (UC)
    {
         UC->_Lobby->Update_chatBox(nickname, message);
    }
    
    return true;
}
