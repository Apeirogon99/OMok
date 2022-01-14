#pragma once
#include "Protocol.pb.h"

#define BUFSIZE 4096

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


//���ڹ����� ���� Ŭ���̾�Ʈ
class SimpleClient
{
public:
	SimpleClient();
	virtual ~SimpleClient();
	bool ClientStart();
private:
	bool ClientInit();
	void RecvThread();
	void SendThread();
	shared_ptr<BYTE*> TestSendPacket();
	int32 TestRecvPacket(BYTE* buffer, int32 len);

	template<typename ProtoType, typename ProtoFunc>
	static bool TestParsePaketandPrint(ProtoFunc func, BYTE* buffer, int32 len);

	static bool Print_C_LOGIN(Protocol::C_LOGIN& pkt)
	{
		cout << pkt.id() << endl;
		return true;
	}

	static bool Print_S_LOGIN(Protocol::S_LOGIN& pkt)
	{
		cout << pkt.playerid() << ", " << pkt.success() << endl;
		return true;
	}

private:
	SOCKET ClientSocket = INVALID_SOCKET;
	thread recvThread;
	thread sendThread;
	bool IsrecvThreadRun = true;
	bool IssendThreadRun = true;

	//char sendBuffer[BUFSIZE];

	BYTE recvBuffer[4096];
	int32 writePos = 0;
	int32 readPos = 0;
};

