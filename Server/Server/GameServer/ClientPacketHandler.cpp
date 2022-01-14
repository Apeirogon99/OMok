#include "pch.h"
#include "ClientPacketHandler.h"
#include "UserSessionManager.h"
#include "GameRoom.h"
#include "GameUser.h"

bool Handle_C_LOGIN(Protocol::C_LOGIN& dpkt,shared_ptr<MyCompltionKey> compltionKey)
{
    static atomic<uint64> idGenerator = 1;

    //TODO : DB
    
    //TODO : 
    //shared_ptr<UserSession> userSession = static_pointer_cast<UserSession>(compltionKey);
    //if (false == GUserSessionManger.FindUser(userSession))
    //{
    //    GUserSessionManger.Add(userSession);
    //    userSession->CreateUser(userSession, idGenerator);
    //}
    //else
    //{
    //    printf("[Handle_C_LOGIN] : have user");
    //    return false;
    //}

    Protocol::S_LOGIN pkt;
    pkt.set_success(true);
    pkt.set_playerid(idGenerator++);

    auto newSendBuf = ClientPacketHandler::SerializePacket(pkt, PKT_S_LOGIN);
    
    compltionKey->Send(newSendBuf);

    return true;
}

bool Handle_C_ENTER_GMAE(Protocol::C_ENTER_GAME& dpkt, shared_ptr<MyCompltionKey> compltionKey)
{
    //TEMP
    shared_ptr<UserSession> userSession = static_pointer_cast<UserSession>(compltionKey);
    auto playerId = dpkt.playerid();

    auto player = userSession->GetUser();
    GRoom.Enter(player);

    if (false == GRoom.IsStartGame())
    {
        GRoom.StartGame();
    }

    //만약에 이걸보내면 그사람의 이름이나 정보등이 들어가야할듯?
    //지금은 playerId를 보내서 식별정도로만 해두자
    //나중에는 방있는지 확인하고 있으면은 매치잡아주기

    return true;
}

bool Handle_C_LEAVE_GAME(Protocol::C_LEAVE_GAME& dpkt, shared_ptr<MyCompltionKey> compltionKey)
{
    return false;
}

bool Handle_C_NEXT_TURN(Protocol::C_NEXT_TURN& dpkt, shared_ptr<MyCompltionKey> compltionKey)
{
    auto posX = dpkt.posx();
    auto posY = dpkt.posy();
    auto playerId = dpkt.playerid();
    auto team = dpkt.team();

    //Room에서 판정해줌
    auto result = GRoom.JudgeRules(playerId, posX, posY, team);

    Protocol::S_NEXT_TURN pkt;
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_playerid(playerId);
    pkt.set_team(team);
    pkt.set_result(result);

    auto newSendBuf = ClientPacketHandler::SerializePacket(pkt, PKT_S_ENTER_GMAE);

    GRoom.Broadcast(newSendBuf);

    return true;
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
