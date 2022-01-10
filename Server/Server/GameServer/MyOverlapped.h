#pragma once

enum class OverlappedType : uint8
{
	NONE,
	ACCEPT,
	CONNECT,
	DISCONNECT,
	SEND,
	RECV,
};

class MyOverlapped : public OVERLAPPED
{
public:
	MyOverlapped(OverlappedType type);

	void Init();

public:
	OverlappedType _type = OverlappedType::NONE;
	shared_ptr <class MyCompltionKey > _compltionKey = nullptr;
};

class AcceptOverlapped : public MyOverlapped
{
public:
	AcceptOverlapped() : MyOverlapped(OverlappedType::ACCEPT) { }

	//shared_ptr<MyCompltionKey>	_compltionKey = nullptr;
};

class ConnectOverlapped : public MyOverlapped
{
public:
	ConnectOverlapped() : MyOverlapped(OverlappedType::CONNECT) { }
};

class DisConnectOverlapped : public MyOverlapped
{
public:
	DisConnectOverlapped() : MyOverlapped(OverlappedType::DISCONNECT) { }
};

class RecvOverlapped : public MyOverlapped
{
public:
	RecvOverlapped() : MyOverlapped(OverlappedType::RECV) { }
};

class SendOverlapped : public MyOverlapped
{
public:
	SendOverlapped() : MyOverlapped(OverlappedType::SEND) { }
};


