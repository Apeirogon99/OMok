#include "pch.h"
#include "MyIocpServer.h"

MyIocpServer::MyIocpServer()
{
}

MyIocpServer::~MyIocpServer()
{
	CloseIocpServer();
}

bool MyIocpServer::StartIocpServer()
{
	if (false == ServerInit())					return false;

	if (false == CreateSocket(_ListenSocket))	return false;

	if (false == SetSocketOpt())				return false;

	if (false == SetAcceptEX())					return false;

	if (false == Bind())						return false;

	if (false == Listen())						return false;

	if (false == CreateHandle())				return false;

	if (false == BindCompltionPort(_ListenSocket)) return false;

	if (false == CreateClient(10))				return false;

	if (false == CreateWorkThreads())			return false;


	printf("Server Start\n");
	return true;
}

bool MyIocpServer::CloseIocpServer()
{
	::CloseHandle(_iocpHandle);

	::closesocket(_ListenSocket);

	::WSACleanup();

	for (auto& index : _WorkThreads)
	{
		index.join();
	}

	return true;
}

bool MyIocpServer::CreateSocket(SOCKET& OUT socket)
{
	socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (socket == INVALID_SOCKET)
		return false;

	printf("CreateSocket\n");

	return true;
}

bool MyIocpServer::CreateHandle()
{
	_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (NULL == _iocpHandle)
		return false;

	printf("Create Handle\n");
	return true;
}

bool MyIocpServer::SetSocketOpt()
{
	LINGER option;
	option.l_onoff = 0;
	option.l_linger = 0;
	if (SOCKET_ERROR == setsockopt(_ListenSocket, SOL_SOCKET, SO_LINGER, reinterpret_cast<char*>(&option), sizeof(option)))
		return false;

	bool flag = false;
	if (SOCKET_ERROR == setsockopt(_ListenSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&flag), sizeof(flag)))
		return false;




	printf("SetSocketOpt\n");
	return true;
}

bool MyIocpServer::ServerInit()
{
	WSADATA wsaData;
	WORD ver = MAKEWORD(2, 2);

	if (0 != ::WSAStartup(ver, &wsaData))
		return false;

	printf("ServerInit\n");

	return true;
}

bool MyIocpServer::Bind()
{
	//TEMP
	const uint16 port = 9000;

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (SOCKET_ERROR == ::bind(_ListenSocket, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)))
		return false;

	printf("Bind\n");
	return true;
}

bool MyIocpServer::Listen()
{
	if (SOCKET_ERROR == listen(_ListenSocket, SOMAXCONN))
	{
		return false;
	}

	printf("Listen\n");
	return true;
}

bool MyIocpServer::WorkThread(DWORD timeout)
{
	if (timeout < 0)
		return false;

	bool Success = false;
	DWORD numOfBytes = 0;
	ULONG_PTR compltionKey = 0;
	MyOverlapped* iocpOverlapped = nullptr;

	while (true)
	{
		Success = ::GetQueuedCompletionStatus(_iocpHandle,&numOfBytes,&compltionKey,reinterpret_cast<LPOVERLAPPED*>(&iocpOverlapped),timeout);

		if (Success)
		{
			if (iocpOverlapped->_type == OverlappedType::ACCEPT)
			{
				printf("Acccccept\n");
				AcceptOverlapped* acceptEvent = static_cast<AcceptOverlapped*>(iocpOverlapped);
				ProcessAccept(acceptEvent);
			}
			else
			{
				iocpOverlapped->_compltionKey->CompletionIo(iocpOverlapped, numOfBytes);
			}
		}
		else
		{
			auto errorCode = GetLastError();
			printf("WorkThread Error : %d\n", errorCode);
			WorkThread();
		}

	}

	return true;
}

bool MyIocpServer::BindCompltionPort(SOCKET socket)
{
	if (NULL == ::CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket),_iocpHandle,0,0))
	{
		return false;
	}

	printf("Socket %d : BindGetQueueCompltionPort\n",static_cast<int32>(socket));
	return true;
}

bool MyIocpServer::SetAcceptEX()
{
	//ACCEPT EX
	GUID guid = WSAID_ACCEPTEX;
	DWORD Bytes = 0;

	if (SOCKET_ERROR == WSAIoctl(_ListenSocket,SIO_GET_EXTENSION_FUNCTION_POINTER,&guid, sizeof(guid),reinterpret_cast<LPVOID*>(&AcceptEx),sizeof(&AcceptEx),&Bytes,NULL,NULL))
	{
		return false;
	}

	return true;
}

bool MyIocpServer::CreateWorkThreads()
{
	int32 maxThreadCount = GetThreadCount();

	if (0 == maxThreadCount)
		return false;

	for (int i = 0; i < maxThreadCount; ++i)
	{
		_WorkThreads.push_back(thread([=]()
			{
				WorkThread();
			}));
	}

	printf("Create WorkThread : %d\n",maxThreadCount);
	return true;
}

bool MyIocpServer::CreateClient(int32 maxClient)
{
	for (int32 i = 0; i < maxClient; i++)
	{
		AcceptOverlapped* newAceept = new AcceptOverlapped();
		_acceptOverlappeds.push_back(newAceept);
		RegisterAccept(newAceept);
	}
	return true;
}

int32 MyIocpServer::GetThreadCount()
{
	SYSTEM_INFO info;
	int32 maxThreadCount = 0;

	GetSystemInfo(&info);
	//maxThreadCount = info.dwNumberOfProcessors * 2;
	maxThreadCount = info.dwNumberOfProcessors;

	return maxThreadCount;
}

void MyIocpServer::ProcessAccept(AcceptOverlapped* newAceept)
{
	shared_ptr<MyCompltionKey> compltionKey = newAceept->_compltionKey;

	if (true == compltionKey->IsConnect())
		return;

	if (SOCKET_ERROR == setsockopt(compltionKey->GetSocket(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<char*>(&_ListenSocket), sizeof(_ListenSocket)))
	{
		printf("setsockopt : %d", WSAGetLastError());
		
		compltionKey.reset();

		return;
	}

	SOCKADDR_IN clientAddr;
	int32 clientAddrLen = sizeof(clientAddr);
	if (SOCKET_ERROR == ::getpeername(compltionKey->GetSocket(), reinterpret_cast<SOCKADDR*>(&clientAddr), &clientAddrLen))
	{
		printf("getpeername : %d", WSAGetLastError());

		compltionKey.reset();

		return;
	}

	char buffer[100];
	uint16 port = ntohs(clientAddr.sin_port);
	int32 socketNum = static_cast<int32>(compltionKey->GetSocket());
	inet_ntop(AF_INET, &clientAddr.sin_addr, buffer, sizeof(buffer));
	printf("[clientAccept : %d] IP : %s, PORT : %d\n", socketNum, buffer, port);

	compltionKey->SetAddress(clientAddr);
	compltionKey->ProcessConnect();
	//RegisterAccept(newAceept);
}

void MyIocpServer::RegisterAccept(AcceptOverlapped* newAceept)
{
	shared_ptr<MyCompltionKey> compltionKey = make_shared<UserSession>();

	newAceept->Init();
	newAceept->_compltionKey = nullptr;

	newAceept->_compltionKey = compltionKey;

	if (false == BindCompltionPort(compltionKey->GetSocket()))
	{
		printf("faild bind compltionPort Socket");
		return;
	}

	if (INVALID_SOCKET == compltionKey->GetSocket())
	{
		printf("non Socket");
		return;
	}

	bool Success = false;
	DWORD bytesReceived = 0;
	Success = AcceptEx(_ListenSocket,compltionKey->GetSocket(),compltionKey->recvBuffer,0,sizeof(SOCKADDR_IN) + 16,sizeof(SOCKADDR_IN) + 16,&bytesReceived,static_cast<LPOVERLAPPED>(newAceept));

	if (false == Success)
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			printf("AcceptEx ErrorCode : %d\n", errorCode);
			RegisterAccept(newAceept);
		}
	}
}
