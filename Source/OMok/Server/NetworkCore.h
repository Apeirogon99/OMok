// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Networking/Public/Networking.h>
#include <Sockets/Public/Sockets.h>
#include <Sockets/Public/SocketSubsystem.h>
#include <HAL/Runnable.h>
#include "PacketHandle.h"

/**
 * NetworkCore
 */
class OMOK_API NetworkCore : public FRunnable
{
public:
	NetworkCore(const FString& socketName = "ClientSocket", const FString& ip = "127.0.0.1", uint16 port = 9000);
	virtual ~NetworkCore();
public:

	bool Connect();
	bool StillConnect();
	void DisConnect();
	void SendData(TArray<uint8> data);
	void RecvData();

private:
	void SendToData();
	int32 RecviveData(TArray<uint8> data);

public:
	bool GetConnect();
	FSocket* GetSocket() { return _clientSocket; }
	//FIPv4Endpoint GetEndpoint{ return _serverAddr; }

public:
	bool Init() override;
	uint32 Run() override;
	void Stop() override;


public:
	FRunnableThread* _frunnableThread = nullptr;
	bool _runThread = false;

private:
	FSocket* _clientSocket = nullptr;
	FIPv4Endpoint _serverAddr = {};
	FString _socketName = "";
	bool _connect = false;
	TAtomic<bool> _registerSend = false;
	int32 _newRecvBufferSize = 0;
	int32 _newSendBufferSize = 0;

	TQueue<TArray<uint8>, EQueueMode::Spsc>  _sendQueue;
	TArray<uint8>  _recvBuffer;
	int32 _writePos = 0;
	int32 _readPos = 0;

	FCriticalSection _lock;
};

//extern NetworkCore GNetworkCore;
