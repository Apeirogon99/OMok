#pragma once

#define BUFSIZE 4096

//���ڹ����� ���� Ŭ���̾�Ʈ
class SimpleClient
{
public:

	bool ClientInit(uint16 port, const char* ip);
	bool ConnectStart();

	void RecvThread();
	void SendThread();

private:
	SOCKET ClientSocket = INVALID_SOCKET;
	thread recvThread;
	thread sendThread;
	bool IsrecvThreadRun = true;
	bool IssendThreadRun = true;

	//char sendBuffer[BUFSIZE];
	char recvBuffer[BUFSIZE];
	atomic<int32> sendPos;
	atomic<int32> readPos;
};

