#include"pch.h"
#include "SimpleClient.h"
#include "ServerPacketHandler.h"
#include "Protocol.pb.h"

struct PacketHeader
{
	uint16 id;
	uint16 size;
};

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}



int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
		return 0;

	u_long on = 1;
	if (::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(9000);

	// Connect
	while (true)
	{
		if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			// ���� ����߾�� �ߴµ�... �ʰ� ����ŷ���� �϶��?
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			// �̹� ����� ���¶�� break
			if (::WSAGetLastError() == WSAEISCONN)
				break;
			// Error
			break;
		}
	}

	cout << "Connected to Server!" << endl;

	Protocol::C_LOGIN pkt;
	pkt.set_id(1);

	const int64 dataSize = pkt.ByteSizeLong();
	const int64 packetSize = dataSize + sizeof(PacketHeader);
	const int32 packHeaderSize = static_cast<int32>(sizeof(PacketHeader));
	
	BYTE* PacketSendBuffer = new BYTE[packetSize];

	PacketHeader* header = reinterpret_cast<PacketHeader*>(PacketSendBuffer);
	header->id = PKT_C_LOGIN;
	header->size = static_cast<uint16>(packetSize);

	pkt.SerializeToArray(&header[1], static_cast<uint16>(packetSize));

	//char sendBuffer[100] = "Hello World";
	WSAEVENT wsaEvent = ::WSACreateEvent();
	WSAOVERLAPPED overlapped = {};
	overlapped.hEvent = wsaEvent;

	// Send
	for(int i =0; i < 5; ++i)
	{
		//WSABUF wsaBuf;
		//wsaBuf.buf = sendBuffer;
		//wsaBuf.len = static_cast<ULONG>(strlen(sendBuffer));

		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(PacketSendBuffer);
		wsaBuf.len = static_cast<LONG>(packetSize);

		DWORD sendLen = 0;
		DWORD flags = 0;
		if (::WSASend(clientSocket, &wsaBuf, 1, &sendLen, flags, &overlapped, nullptr) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSA_IO_PENDING)
			{
				// Pending
				::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
				::WSAGetOverlappedResult(clientSocket, &overlapped, &sendLen, FALSE, &flags);
			}
			else
			{
				printf("%d", WSAGetLastError());
				// ��¥ ���� �ִ� ��Ȳ
				break;
			}
		}

		cout << "Send Data ! Len = " << sendLen << endl;


		/*PacketHeader header = *(reinterpret_cast<PacketHeader*>(&wsaBuf.buf[0]));
		cout << header.id << " " << header.size << endl;*/

		/*Protocol::C_LOGIN dpkt;
		dpkt.ParseFromArray(wsaBuf.buf + sizeof(PacketHeader), wsaBuf.len - sizeof(PacketHeader));

		cout << "id : " << dpkt.id() << endl;

		this_thread::sleep_for(1s);*/

		//RECV

		char recvBuf[4096];
		ZeroMemory(recvBuf, sizeof(recvBuf));

		WSABUF recvwsaBuf;
		recvwsaBuf.buf = recvBuf;
		recvwsaBuf.len = sizeof(recvBuf);

		DWORD numOfBytes = 0;
		DWORD flag = 0;

		WSAEVENT recvwsaEvent = ::WSACreateEvent();
		WSAOVERLAPPED recvoverlapped = {};
		recvoverlapped.hEvent = recvwsaEvent;

		if (SOCKET_ERROR == ::WSARecv(clientSocket, &recvwsaBuf, 1, &numOfBytes, &flag, &recvoverlapped, nullptr))
		{
			cout << "WSARecv Error : " << WSAGetLastError() << endl;
		}
		
		cout << "Recv Data ! Len = " << numOfBytes << endl;

		if (numOfBytes > sizeof(PacketHeader))
		{
			PacketHeader header = *(reinterpret_cast<PacketHeader*>(&wsaBuf.buf[0]));

			cout << "header id : " << header.id << endl;
			cout << "header size : " << header.size << endl;

			if (header.size < numOfBytes)
			{
				Protocol::S_LOGIN spkt;
				spkt.ParseFromArray(recvwsaBuf.buf + sizeof(PacketHeader), recvwsaBuf.len - sizeof(PacketHeader));

				cout << "PKT sucssess : " << spkt.success() << endl;
				cout << "PKT playId : " << spkt.playerid() << endl;
			}
		}
	}

	cout << "done" << endl;
	
	getchar();

	// ���� ���ҽ� ��ȯ
	::closesocket(clientSocket);

	// ���� ����
	::WSACleanup();
}