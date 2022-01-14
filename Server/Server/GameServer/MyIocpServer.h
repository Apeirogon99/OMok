#pragma once
#include "UserSession.h"
#include "MyCompltionKey.h"
#include "MyOverlapped.h"

class MyIocpServer
{
public:
	MyIocpServer();
	virtual ~MyIocpServer();

public:
	bool StartIocpServer();
	bool CloseIocpServer();

private:
	bool CreateSocket(SOCKET& OUT socket);
	bool CreateHandle();
	bool SetSocketOpt();
	bool ServerInit();
	bool Bind();
	bool Listen();

private:
	bool WorkThread(DWORD timeout = INFINITE);
	bool BindCompltionPort(SOCKET socket);
	bool SetAcceptEX();
	bool CreateWorkThreads();
	bool CreateClient(int32 maxClient = 100);
	int32 GetThreadCount();

private:
	void ProcessAccept(AcceptOverlapped* newAceept);
	void RegisterAccept(AcceptOverlapped* newAceept);

private:
	atomic<int32> _CompltionKeysCount = 0;
	vector<shared_ptr<MyCompltionKey>> _compltionKeys;
	vector<AcceptOverlapped*> _acceptOverlappeds;
	vector<thread> _WorkThreads;

	//TEMP
	HANDLE _iocpHandle = nullptr;
	SOCKET _ListenSocket = INVALID_SOCKET;
	//AcceptOverlapped acceptOverlapped;
	LPFN_ACCEPTEX AcceptEx;
};

