#pragma once
#include "Protocol.pb.h"
#include "UserSession.h"

struct PacketHeader
{
	uint16 id;
	uint16 size;
};

enum : uint16
{
	PKT_C_LOGIN = 1,
	PKT_S_LOGIN = 2,

	PKT_C_ENTER_GAME = 3,
	PKT_S_ENTER_GMAE = 4,

	PKT_S_MATCHING_GAME = 5,

	PKT_C_LEAVE_GAME = 6,
	PKT_S_LEAVE_GAME = 7,
};

bool Handle_C_LOGIN(shared_ptr<MyCompltionKey> compltionKey, BYTE* buffer, uint16 len);
bool Handle_C_ENTER_GMAE(shared_ptr<MyCompltionKey> compltionKey, BYTE* buffer, uint16 len);
bool Handle_C_LEAVE_GAME(shared_ptr<MyCompltionKey> compltionKey, BYTE* buffer, uint16 len);

class ClientPacketHandler
{
public:

	static int32 cheackPacket(shared_ptr<MyCompltionKey>, BYTE* buffer, int32 len);
	static void HandlePacket(shared_ptr<MyCompltionKey>, BYTE* buffer, PacketHeader header);
};

