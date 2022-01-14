#include "pch.h"
#include "SimpleClient.h"

SimpleClient::SimpleClient()
{
	ZeroMemory(recvBuffer, sizeof(recvBuffer));
}

SimpleClient::~SimpleClient()
{
	::closesocket(ClientSocket);


	::WSACleanup();
}

bool SimpleClient::ClientInit()
{
	WSADATA wsaData;
	WORD ver = MAKEWORD(2, 2);

	if (0 != WSAStartup(ver, &wsaData))
		return false;

	ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == ClientSocket)
		return false;

	u_long on = 1;
	if (::ioctlsocket(ClientSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(9000);

	while (true)
	{
		if (::connect(ClientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			if (::WSAGetLastError() == WSAEISCONN)
				break;
			return false;
			break;
		}
	}

	return true;
}

bool SimpleClient::ClientStart()
{
	if (false == ClientInit())
		return false;

	ZeroMemory(&recvBuffer, BUFSIZE);

	while (true)
	{

		SendThread();

		RecvThread();

		this_thread::sleep_for(1000ms);
	}

	//sendThread = std::thread([this]() { SendThread(); });
	//recvThread = std::thread([this]() { RecvThread(); });

	//sendThread.join();
	//recvThread.join();


	return false;
}

void SimpleClient::RecvThread()
{

	WSABUF recvwsaBuf;
	recvwsaBuf.buf = reinterpret_cast<char*>(&recvBuffer[writePos]);
	recvwsaBuf.len = static_cast<ULONG>(4096 - writePos);

	DWORD numOfBytes = 0;
	DWORD flag = 0;

	WSAEVENT wsaEvent = ::WSACreateEvent();
	WSAOVERLAPPED overlapped = {};
	overlapped.hEvent = wsaEvent;

	if (SOCKET_ERROR == ::WSARecv(ClientSocket, &recvwsaBuf, 1, &numOfBytes, &flag, &overlapped, nullptr))
	{
		auto error = WSAGetLastError();
		if (error == WSA_IO_PENDING)
		{
			// Pending
			::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
			::WSAGetOverlappedResult(ClientSocket, &overlapped, &numOfBytes, FALSE, &flag);
		}
		else
		{
			cout << "Clinet Recv Error : " << error << endl;
		}
	}

	cout << "Recv Data ! Len = " << numOfBytes << endl;

	writePos += numOfBytes;
	readPos += TestRecvPacket(&recvBuffer[readPos], numOfBytes);

	if (writePos == readPos)
		writePos = readPos = 0;
	
}

void SimpleClient::SendThread()
{

	WSAEVENT wsaEvent = ::WSACreateEvent();
	WSAOVERLAPPED overlapped = {};
	overlapped.hEvent = wsaEvent;

	auto buffer = TestSendPacket();

	WSABUF snedwsaBuf;
	snedwsaBuf.buf = reinterpret_cast<char*>(*buffer.get());
	snedwsaBuf.len = static_cast<LONG>(_msize(*buffer.get()) / sizeof(BYTE));

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	if (::WSASend(ClientSocket, &snedwsaBuf, 1, &numOfBytes, flags, &overlapped, nullptr) == SOCKET_ERROR)
	{
		if (::WSAGetLastError() == WSA_IO_PENDING)
		{
			// Pending
			::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
			::WSAGetOverlappedResult(ClientSocket, &overlapped, &numOfBytes, FALSE, &flags);
		}
		else
		{
			printf("%d", WSAGetLastError());
		}
	}

	cout << "Send Data ! Len = " << numOfBytes << endl;
}

shared_ptr<BYTE*> SimpleClient::TestSendPacket()
{
	Protocol::C_LOGIN pkt;
	pkt.set_id(1);

	const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
	const uint16 packetSize = dataSize + sizeof(PacketHeader);

	shared_ptr<BYTE*> PacketSendBuffer = make_shared<BYTE*>(new BYTE[packetSize]);

	PacketHeader* header = reinterpret_cast<PacketHeader*>(*PacketSendBuffer.get());
	header->id = PKT_C_LOGIN;
	header->size = packetSize;

	if (false == pkt.SerializeToArray(&header[1], dataSize))
	{
		printf("can't SerializeToArray");
	}

	//TestParsePaketandPrint<Protocol::C_LOGIN>(Print_C_LOGIN, *PacketSendBuffer.get(), packetSize);

	return PacketSendBuffer;
}

int32 SimpleClient::TestRecvPacket(BYTE* buffer, int32 len)
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

		TestParsePaketandPrint<Protocol::S_LOGIN>(Print_S_LOGIN, &buffer[processLen], len);

		processLen += header.size;
	}

	return processLen;
}

template<typename ProtoType, typename ProtoFunc>
bool SimpleClient::TestParsePaketandPrint(ProtoFunc func, BYTE* buffer, int32 len)
{
	//TEMP : Only Login Pakcet
	ProtoType pkt;
	if (false == pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)))
	{
		printf("can't ParsePacket");
		return false;
	}

	return func(pkt);
}
