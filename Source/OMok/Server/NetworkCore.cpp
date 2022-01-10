// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkCore.h"

//NetworkCore GNetworkCore;

#pragma region Main Thread Code

NetworkCore::NetworkCore(const FString& socketName, const FString& ip, uint16 port)
{
	//TEMP : LoopBack 127.0.01
	FIPv4Address::Parse(ip, _serverAddr.Address);
	_serverAddr.Port = port;

	this->_socketName = socketName;

	_frunnableThread = FRunnableThread::Create(this, TEXT("ServerThread"));
}

NetworkCore::~NetworkCore()
{
	if (_frunnableThread)
	{
		_frunnableThread->Kill();
		delete _frunnableThread;
	}

	if (_clientSocket)
	{
		_clientSocket->Close();
	}

	_clientSocket = nullptr;
	_frunnableThread = nullptr;
}

#pragma endregion

bool NetworkCore::Connect()
{
	if (_connect)
	{
		return true;
	}

	_clientSocket = ::ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, _socketName, false);

	if (!_clientSocket)
	{
		return false;
	}

	//TEMP
	int32 bufSize = 16384;
	_clientSocket->SetReceiveBufferSize(bufSize, _newRecvBufferSize);
	_clientSocket->SetSendBufferSize(bufSize, _newSendBufferSize);
	_recvBuffer.Init(0, 16384);

	TSharedRef<FInternetAddr> internetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	internetAddr->SetIp(_serverAddr.Address.Value);
	internetAddr->SetPort(_serverAddr.Port);

	_connect = _clientSocket->Connect(*internetAddr);
	if (_connect)
	{
		UE_LOG(LogTemp, Warning, TEXT("connect to server"));
		}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("can't connect to server"));
		_runThread = false;
		return false;
	}

	return true;
}

bool NetworkCore::StillConnect()
{
	_clientSocket->SetNonBlocking(true);
	int32 Bytes;
	BYTE EmptyData;
	if (_clientSocket->Recv(&EmptyData, 1, Bytes, ESocketReceiveFlags::Peek))
	{
		return false;
	}
	_clientSocket->SetNonBlocking(false);
	return true;
}

void NetworkCore::DisConnect()
{
	if (_clientSocket)
	{
		_clientSocket->Close();
	}

	_clientSocket = nullptr;
}

void NetworkCore::SendData(TArray<uint8> data)
{
	bool registerSend = false;

	{
		FScopeLock lockguard(&_lock);

		_sendQueue.Enqueue(data);

		if (_registerSend.Exchange(true) == false)
			registerSend = true;
	}

	if (registerSend)
	{
		SendToData();
	}
}

void NetworkCore::RecvData()
{

	if (false == GetConnect())
		return;

	uint32 pendingDataSize;
	while (_clientSocket->HasPendingData(pendingDataSize))
	{
		int32 Bytes = 0;
		if (false == _clientSocket->Recv(&_recvBuffer[_writePos], pendingDataSize, Bytes))
		{
			UE_LOG(LogTemp, Warning, TEXT("can't recv to server"));
			Stop();
		}

		_writePos = Bytes;
		_readPos = PacketHandle::RecviveData(_recvBuffer);

		if (_writePos == _readPos)
			_writePos = _readPos = 0;
	}

}

void NetworkCore::SendToData()
{
	if (_sendQueue.IsEmpty())
		return;

	TArray<uint8> data;
	while (_sendQueue.Dequeue(data))
	{
		int32 Bytes = 0;
		if (false == _clientSocket->Send(data.GetData(), data.Num(), Bytes))
		{
			UE_LOG(LogTemp, Warning, TEXT("can't send to server"));
			Stop();
		}
	}

	_registerSend.Store(false);
}

int32 NetworkCore::RecviveData(TArray<uint8> data)
{
	return 0;
}

bool NetworkCore::GetConnect()
{
	ESocketConnectionState connectState = _clientSocket->GetConnectionState();
	if (connectState != ESocketConnectionState::SCS_Connected)
		return false;
	return true;
}

bool NetworkCore::Init()
{
	if (_frunnableThread)
	{
		UE_LOG(LogTemp, Warning, TEXT("cant't initialized FRunnableThread"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("initialized FRunnableThread"));
	_runThread = true;

	return true;
}

uint32 NetworkCore::Run()
{
	while (_runThread)
	{

		FDateTime timeBeginningOfTick = FDateTime::UtcNow();

		if (!Connect())
		{
			continue;
		}

		if (nullptr == _clientSocket)
		{
			Stop();
		}

		//if (false == StillConnect())
		//{
		//	Stop();
		//}

		//TODO : FIX
		RecvData();

		float TimeBetweenTicks = 0.008f;
		FDateTime timeEndOfTick = FDateTime::UtcNow();
		FTimespan tickDuration = timeEndOfTick - timeBeginningOfTick;
		float secondsThisTickTook = tickDuration.GetTotalSeconds();
		float timeToSleep = TimeBetweenTicks - secondsThisTickTook;
		if (timeToSleep > 0.f)
		{
			FPlatformProcess::Sleep(timeToSleep);
		}
		
	}

	return 0;
}

void NetworkCore::Stop()
{
	_runThread = false;
	DisConnect();
}
