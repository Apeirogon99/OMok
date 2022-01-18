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

	PKT_C_MATCHING_GAME = 5,
	PKT_S_MATCHING_GAME = 6,

	PKT_C_LEAVE_GAME = 7,
	PKT_S_LEAVE_GAME = 8,

	PKT_C_NEXT_TURN = 9,
	PKT_S_NEXT_TURN = 10,

	PKT_C_CHAT_LOOBY = 11,
	PKT_S_CHAT_LOOBY = 12,
};

bool Handle_C_LOGIN(Protocol::C_LOGIN& dpkt, shared_ptr<MyCompltionKey> compltionKey);
bool Handle_C_ENTER_GMAE(Protocol::C_ENTER_GAME& dpkt, 
shared_ptr<MyCompltionKey> compltionKey);
bool Handle_C_MATCHING_GAME(Protocol::C_MATCHING_GAME& dpkt, shared_ptr<MyCompltionKey> compltionKey);
bool Handle_C_LEAVE_GAME(Protocol::C_LEAVE_GAME& dpkt, shared_ptr<MyCompltionKey> compltionKey);
bool Handle_C_NEXT_TURN(Protocol::C_NEXT_TURN& dpkt, shared_ptr<MyCompltionKey> compltionKey);
bool Handle_C_CHAT_LOBBY(Protocol::C_CHAT_LOBBY& dpkt, shared_ptr<MyCompltionKey> compltionKey);

class ClientPacketHandler
{
public:

	static int32 cheackPacket(shared_ptr<MyCompltionKey>, BYTE* buffer, int32 len);
	static void HandlePacket(shared_ptr<MyCompltionKey>, BYTE* buffer, PacketHeader header);


public:
	template<typename ProtoType, typename ProtoFunc>
	static bool parsePacket(ProtoFunc func, shared_ptr<MyCompltionKey> compltionKey, BYTE* buffer, uint16 len)
	{
		ProtoType pkt;
		if (false == pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)))
		{
			return false;
		}

		return func(pkt, compltionKey);
	}

	template<typename ProtoType>
	static shared_ptr<BYTE*> SerializePacket(ProtoType& pkt, uint16 id)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = static_cast<uint16>(dataSize + sizeof(PacketHeader));

		shared_ptr<BYTE*> newSendBuf = make_shared<BYTE*>(new BYTE[packetSize]);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(*newSendBuf.get());
		header->id = id;
		header->size = packetSize;

		if (false == pkt.SerializeToArray(&header[1], dataSize))
		{
			printf("can't SerializeToArray");
		}
			
		//TEST TEMP
		//Protocol::S_LOGIN dpkt;
		//if (false == dpkt.ParseFromArray(*newSendBuf.get() + sizeof(PacketHeader), packetSize - sizeof(PacketHeader)))
		//{
		//	printf("can't ParseFromArray");
		//}
		//else
		//{
		//	cout << dpkt.playerid() << ", " << dpkt.success() << endl;
		//}



		return newSendBuf;
	}
};

