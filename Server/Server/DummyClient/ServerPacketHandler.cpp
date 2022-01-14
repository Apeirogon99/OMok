#include "pch.h"
#include "ServerPacketHandler.h"

bool Handle_S_LOGIN()
{
    Protocol::C_LOGIN pkt;
    
    //���̵�
    pkt.set_id(1);

    const int64 dataSize = pkt.ByteSizeLong();
    const int64 packetSize = dataSize;// +sizeof(PacketHeader);

   // char* newSendBuf = new char(packetSize);
    //PacketHeader* header;
    //header.id;
    //header.size;


    //memcpy(&newSendBuf, static_cast<char*>(header), sizeof(PacketHeader));
    //pkt.SerializeToArray(&newSendBuf[sizeof(PacketHeader)], packetSize);

    //session->onSend(newSendbuf);

    return true;
}

bool Handle_S_ENTER_GMAE()
{
    return false;
}

bool Handle_S_LEAVE_GAME()
{
    return false;
}
