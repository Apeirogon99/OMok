#pragma once
#include "MyOverlapped.h"

class MyCompltionKey : public enable_shared_from_this<MyCompltionKey>
{
public:
	MyCompltionKey();
	virtual ~MyCompltionKey();

	void Send(shared_ptr<BYTE*> sendBuffer);

	SOCKET GetSocket() const;
	HANDLE GetHandle() const;
	int32 GetKeyIndex() const;
	bool IsConnect() const;

	void SetKeyIndex(int32 keyIndex);
	void SetAddress(SOCKADDR_IN clientAddr);

public:
	void CompletionIo(MyOverlapped* overlapped, int32 numOfBytes);
	void ProcessConnect();
	void ProcessDisConnect();
	void ProcessSend(int32 numOfBytes);
	void ProcessRecv(int32 numOfBytes);

	void RegisterConnect();
	void RegisterDisConnect();
	void RegisterSend();
	void RegisterRecv();

public:
	virtual void		OnConnected() { return; }
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) { return; }
	virtual void		OnDisconnected() { return; }

	//TEMP
	void Init(SOCKET socket);
	BYTE tempBuf[4096];

	//RECVBUF TEMP
public:
	BYTE recvBuffer[4096];
	int32 readPos = 0;
	int32 writePos = 0;
	void CleanBuf();

public:
	//char sendBuffer[BUFSIZE];
	int32 snedWrite = 0;
	queue<shared_ptr<BYTE*>> _sendQueue;
	atomic<bool> _RegisterSend = false;

public:
	SOCKET _clientSocket = INVALID_SOCKET;
	SOCKADDR_IN _clientAddr = {};
	int32 _keyIndex = 0;
	atomic<bool> _connect = false;

	//AcceptOverlapped acceptOverlapped;
	ConnectOverlapped connectOverlapped;
	DisConnectOverlapped disConnectOverlapped;
	RecvOverlapped recvOverlapped;
	SendOverlapped sendOverlapped;

	mutex _mutex;

	LPFN_DISCONNECTEX DisconnectEx;
};