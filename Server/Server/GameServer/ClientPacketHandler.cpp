#include "pch.h"
#include "ClientPacketHandler.h"
#include "UserSessionManager.h"
#include "GameRoom.h"
#include "Lobby.h"
#include "GameUser.h"

bool Handle_C_LOGIN(Protocol::C_LOGIN& dpkt,shared_ptr<MyCompltionKey> compltionKey)
{
    cout << "Handle_C_LOGIN - id : "<< dpkt.id() << " password : " << dpkt.password() << endl;

    static atomic<uint64> idGenerator = 1;

    shared_ptr<UserSession> userSession = static_pointer_cast<UserSession>(compltionKey);

    if (true == GLobby.FindUser(userSession))
        return false;

    GLobby.Enter(userSession);
    
    Protocol::S_LOGIN pkt;
    pkt.set_success(true);
    pkt.set_playerid(idGenerator++);
    pkt.set_nickname(dpkt.id());
    compltionKey->Send(ClientPacketHandler::SerializePacket(pkt, PKT_S_LOGIN));

    //cout << GLobby.GetUserSessionCount() << endl;

    return true;
}

bool Handle_C_ENTER_GMAE(Protocol::C_ENTER_GAME& dpkt, shared_ptr<MyCompltionKey> compltionKey)
{
    shared_ptr<UserSession> userSession = static_pointer_cast<UserSession>(compltionKey);

    return true;
}

bool Handle_C_MATCHING_GAME(Protocol::C_MATCHING_GAME& dpkt, shared_ptr<MyCompltionKey> compltionKey)
{
    //매칭잡힌 플레이어가 수락을 하였는지 확인하는 곳
    //TEMP : 그냥 바로 잡아줌
    shared_ptr<UserSession> userSession = static_pointer_cast<UserSession>(compltionKey);

    GRoom.Enter(userSession->GetUser());
    GLobby.Leave(userSession);

    //TEMP
    Protocol::S_MATCHING_GAME pkt;
    pkt.set_delaytime(1);
    pkt.set_success(true);

    auto newSendBuf = ClientPacketHandler::SerializePacket(pkt, PKT_S_MATCHING_GAME);

    compltionKey->Send(newSendBuf);

    if (GRoom.StartGame())
    {
        Protocol::S_ENTER_GMAE pktg;
        pkt.set_delaytime(1);
        pkt.set_success(true);
        auto newSendBufg = ClientPacketHandler::SerializePacket(pktg, PKT_S_ENTER_GMAE);

        GRoom.Broadcast(newSendBufg);
    }

    return true;
}

bool Handle_C_LEAVE_GAME(Protocol::C_LEAVE_GAME& dpkt, shared_ptr<MyCompltionKey> compltionKey)
{
    shared_ptr<UserSession> userSession = static_pointer_cast<UserSession>(compltionKey);
    GRoom.Leave(userSession->GetUser());

    Protocol::S_LEAVE_GAME pkt;
    pkt.set_success(true);

    auto newSendBuf = ClientPacketHandler::SerializePacket(pkt, PKT_S_LEAVE_GAME);

    compltionKey->Send(newSendBuf);


    return true;
}

bool Handle_C_NEXT_TURN(Protocol::C_NEXT_TURN& dpkt, shared_ptr<MyCompltionKey> compltionKey)
{
    auto posX = dpkt.posx();
    auto posY = dpkt.posy();
    auto playerId = dpkt.playerid();
    auto team = dpkt.team();

    auto result = GRoom.JudgeRules(playerId, posX, posY, team);

    Protocol::S_NEXT_TURN pkt;
    pkt.set_posx(posX);
    pkt.set_posx(posY);
    pkt.set_playerid(playerId);
    pkt.set_team(team);
    pkt.set_result(result);

    auto newSendBuf = ClientPacketHandler::SerializePacket(pkt, PKT_S_ENTER_GMAE);

    GRoom.Broadcast(newSendBuf);

    return true;
}

bool Handle_C_CHAT_LOBBY(Protocol::C_CHAT_LOBBY& dpkt, shared_ptr<MyCompltionKey> compltionKey)
{
    shared_ptr<UserSession> userSession = static_pointer_cast<UserSession>(compltionKey);

    if (false == GLobby.FindUser(userSession))
        return false;

    Protocol::S_CHAT_LOBBY pkt;
    pkt.set_playerid(dpkt.playerid());
    pkt.set_playernickname(dpkt.playernickname());
    pkt.set_message(dpkt.message());

    cout << dpkt.message() << endl;

    auto newSendBuf = ClientPacketHandler::SerializePacket(pkt, PKT_S_CHAT_LOOBY);

    GLobby.Broadcast(newSendBuf);

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
    case PKT_C_MATCHING_GAME:
        parsePacket<Protocol::C_MATCHING_GAME>(Handle_C_MATCHING_GAME, compltionKey, buffer, header.size);
        break;
    case PKT_C_LEAVE_GAME:
        parsePacket<Protocol::C_LEAVE_GAME>(Handle_C_LEAVE_GAME, compltionKey, buffer, header.size);
        break;
    case PKT_C_NEXT_TURN:
        parsePacket<Protocol::C_NEXT_TURN>(Handle_C_NEXT_TURN, compltionKey, buffer, header.size);
        break;
    case PKT_C_CHAT_LOOBY:
        parsePacket<Protocol::C_CHAT_LOBBY>(Handle_C_CHAT_LOBBY, compltionKey, buffer, header.size);
        break;
    default:
        printf("EXIT PKT CODE");
        break;
    }
}
