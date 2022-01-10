#pragma once
#include "Protocol.pb.h"

enum 
{
	PKT_C_LOGIN = 1,
	PKT_S_LOGIN = 2,

	PKT_C_ENTER_GAME = 3,
	PKT_S_ENTER_GMAE = 4,

	PKT_S_MATCHING_GAME = 5,

	PKT_C_LEAVE_GAME = 6,
	PKT_S_LEAVE_GAME = 7,
};

bool Handle_S_LOGIN();
bool Handle_S_ENTER_GMAE();
bool Handle_S_LEAVE_GAME();

class ServerPacketHandler
{
public:
	static void Init()
	{

	}
	static bool HandlePacket()
	{


	}
	
private:

};

