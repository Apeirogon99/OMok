#include "pch.h"
#include "ClientPacketHandler.h"
#include "UserSessionManager.h"
#include "GameRoom.h"
#include "GameUser.h"

bool Handle_C_LOGIN(Protocol::C_LOGIN& dpkt,shared_ptr<MyCompltionKey> compltionKey)
{
    //cout << "SOCKET : " << compltionKey->GetSocket() << ", Handle_C_LOGIN : " << dpkt.id() << endl;

    //TEMPLEATE
    static atomic<int32> idGenerator = 1;

    Protocol::S_LOGIN pkt;

    pkt.set_success(true);
    pkt.set_playerid(idGenerator++);


    //TEMPLEATE
    auto newSendBuf = ClientPacketHandler::SerializePacket(pkt, PKT_S_LOGIN);

    compltionKey->Send(newSendBuf);

    return true;
}

bool Handle_C_ENTER_GMAE(Protocol::C_ENTER_GAME& dpkt, shared_ptr<MyCompltionKey> compltionKey)
{




    return false;
}

bool Handle_C_LEAVE_GAME(Protocol::C_LEAVE_GAME& dpkt, shared_ptr<MyCompltionKey> compltionKey)
{
    return false;
}

int32 ClientPacketHandler::cheackPacket(shared_ptr<MyCompltionKey> compltionKey, BYTE* buffer, int32 len)
{
    int32 processLen = 0;
    while (true)
    {
        int32 dataSize = len - processLen;
        if (dataSize < sizeof(PacketHeader))
            break;
    
        PacketHeader header;
        memcpy(&header, &buffer[processLen], sizeof(PacketHeader));

        //cout << header.id << ", " << header.size << endl;

        if (dataSize < header.size)
            break;

        HandlePacket(compltionKey, &buffer[processLen], header);

        processLen += header.size;
    }

    return processLen;
}

void ClientPacketHandler::HandlePacket(shared_ptr<MyCompltionKey> compltionKey, BYTE* buffer, PacketHeader header)
{
    cout << compltionKey->GetSocket() << ", " << header.id << ", " << header.size << endl;

    switch (header.id)
    {
    case PKT_C_LOGIN:
        parsePacket<Protocol::C_LOGIN>(Handle_C_LOGIN,compltionKey, buffer, header.size);
        break;
    case PKT_C_ENTER_GAME:
        parsePacket<Protocol::C_ENTER_GAME>(Handle_C_ENTER_GMAE, compltionKey, buffer, header.size);
        break;
    case PKT_C_LEAVE_GAME:
        parsePacket<Protocol::C_LEAVE_GAME>(Handle_C_LEAVE_GAME, compltionKey, buffer, header.size);
        break;
    default:
        printf("EXIT PKT CODE");
        break;
    }
}
