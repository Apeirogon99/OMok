#include "pch.h"
#include "ClientPacketHandler.h"
#include "GameRoom.h"
#include "GameUser.h"

bool Handle_C_LOGIN(shared_ptr<MyCompltionKey> compltionKey, BYTE* buffer, uint16 len)
{
    //TEMPLEATE
    Protocol::C_LOGIN dPkt;
    if (false == dPkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)))
        return false;

    static atomic<int32> idGenerator = 1;

    Protocol::S_LOGIN pkt;

    pkt.set_success(true);
    pkt.set_playerid(idGenerator++);

    const int64 dataSize = pkt.ByteSizeLong();
    const int64 packetSize = dataSize +sizeof(PacketHeader);

    //TEMPLEATE
    BYTE* newSendBuf = new BYTE[packetSize];
    PacketHeader* header = reinterpret_cast<PacketHeader*>(newSendBuf);
    header->id = PKT_S_LOGIN;
    header->size = static_cast<uint16>(packetSize);

    pkt.SerializeToArray(&header[1], static_cast<int32>(dataSize));

    compltionKey->Send(newSendBuf);

    delete[] newSendBuf;

    //GRoom.Enter();

    return true;
}

bool Handle_C_ENTER_GMAE(shared_ptr<MyCompltionKey> compltionKey, BYTE* buffer, uint16 len)
{
    return false;
}

bool Handle_C_LEAVE_GAME(shared_ptr<MyCompltionKey> compltionKey, BYTE* buffer, uint16 len)
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

        if (dataSize < header.size)
            break;

        HandlePacket(compltionKey, &buffer[processLen], header);

        processLen += header.size;
    }

    return processLen;
}

void ClientPacketHandler::HandlePacket(shared_ptr<MyCompltionKey> compltionKey, BYTE* buffer, PacketHeader header)
{
    //cout << "id : " << header.id << " size : " << header.size << endl;

    switch (header.id)
    {
    case PKT_C_LOGIN:
        Handle_C_LOGIN(compltionKey, buffer, header.size);
        break;
    case PKT_C_ENTER_GAME:
        Handle_C_ENTER_GMAE(compltionKey, buffer, header.size);
        break;
    case PKT_C_LEAVE_GAME:
        Handle_C_LEAVE_GAME(compltionKey, buffer, header.size);
        break;
    default:
        printf("EXIT PKT CODE");
        break;
    }
}
