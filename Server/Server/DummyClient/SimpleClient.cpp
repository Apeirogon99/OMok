#include "pch.h"
#include "SimpleClient.h"

bool SimpleClient::ClientInit(uint16 port, const char* ip)
{
	WSADATA wsaData;
	WORD ver = MAKEWORD(2, 2);

	if (0 != WSAStartup(ver, &wsaData))
		return false;

	ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == ClientSocket)
		return false;

	printf("���� �ʱ�ȭ\n");

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &serverAddr.sin_addr);

	if(SOCKET_ERROR == connect(ClientSocket,(SOCKADDR*)&serverAddr,sizeof(serverAddr)))
		return false;

	

	printf("���� ����\n");

	return true;
}

bool SimpleClient::ConnectStart()
{
	sendThread = std::thread([this]() { SendThread(); });
	recvThread = std::thread([this]() { RecvThread(); });

	recvThread.join();
	sendThread.join();

	closesocket(ClientSocket);
	WSACleanup();

	return false;
}

void SimpleClient::RecvThread()
{
	int32 recvLen;

	while (true)
	{
		recvLen = recv(ClientSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (recvLen == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			break;
		}
		else if (recvLen == 0)
			break;

		recvBuffer[recvLen] = '\0';
		printf("[����] bytes : %d , msg : %s\n", recvLen, recvBuffer);
		this_thread::sleep_for(1s);
	}
	
}

void SimpleClient::SendThread()
{
	size_t len = 0;
	int32 retval = 0;

	char sendBuffer[BUFSIZE] = "Hello";

	while (true)
	{
		/*
		printf("\n�Է� >>");
		if (fgets(sendBuffer, BUFSIZE, stdin) == NULL)
			break;

		len = strlen(sendBuffer);
		if (sendBuffer[len - 1] == '\n')
			sendBuffer[len - 1] = '\0';
		if (strlen(sendBuffer) == 0)
			break;
		*/

		retval = send(ClientSocket, sendBuffer, static_cast<int>(strlen(sendBuffer)), 0);
		if (retval == SOCKET_ERROR)
			break;
		
		//Ȯ�ο�
		printf("[�۽�] bytes : %d , msg : %s\n", retval, sendBuffer);
		this_thread::sleep_for(1s);
	}
}
