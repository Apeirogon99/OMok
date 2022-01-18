#include "pch.h"
#include "MyCompltionKey.h"

MyCompltionKey::MyCompltionKey()
{
    _clientSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

MyCompltionKey::~MyCompltionKey()
{
    ::closesocket(_clientSocket);
}

void MyCompltionKey::Send(shared_ptr<BYTE*> sendBuffer)
{
    cout << "Send data data" << endl;
    if (IsConnect() == false)
        return;

    bool registerSend = false;

    {
        //TODO : Lock_guard
        ::lock_guard<::mutex> guard(_mutex);

        _sendQueue.push(sendBuffer);

        if (_RegisterSend.exchange(true) == false)
            registerSend = true;
    }

    cout << "Buf Sucess Push : " << registerSend << endl;

    if(registerSend)
         RegisterSend();
}

SOCKET MyCompltionKey::GetSocket() const
{
    return _clientSocket;
}

HANDLE MyCompltionKey::GetHandle() const
{
    return reinterpret_cast<HANDLE>(_clientSocket);
}

int32 MyCompltionKey::GetKeyIndex() const
{
    return this->_keyIndex;
}

bool MyCompltionKey::IsConnect() const
{
    return _connect;
}

void MyCompltionKey::SetKeyIndex(int32 keyIndex)
{
    if (keyIndex < 0)
    {
        printf("worng index");
        return;
    }

    this->_keyIndex = keyIndex;
}

void MyCompltionKey::SetAddress(SOCKADDR_IN clientAddr)
{
    this->_clientAddr = clientAddr;
}

void MyCompltionKey::CompletionIo(MyOverlapped* overlapped, int32 numOfBytes)
{
    switch (overlapped->_type)
    {
    case OverlappedType::CONNECT:
        ProcessConnect();
        break;
    case OverlappedType::DISCONNECT:
        ProcessDisConnect();
        break;
    case OverlappedType::SEND:
        ProcessSend(numOfBytes);
        break;
    case OverlappedType::RECV:
        ProcessRecv(numOfBytes);
        break;
    default:
        printf("CompletionIo Error");
        break;
    }
}

void MyCompltionKey::ProcessConnect()
{
    connectOverlapped._compltionKey = nullptr;

    _connect.store(true);

    OnConnected();

    RegisterRecv();
}

void MyCompltionKey::ProcessDisConnect()
{
    disConnectOverlapped._compltionKey = nullptr;

    OnDisconnected();
}

void MyCompltionKey::ProcessSend(int32 numOfBytes)
{
    //TEMP
    cout << "SDNIENIFNE : " << numOfBytes << endl;

    sendOverlapped._compltionKey = nullptr;

    if (numOfBytes == 0)
    {
        RegisterDisConnect();
        return;
    }

    OnSend(numOfBytes);

    {
        ::lock_guard<::mutex> guard(_mutex);

        if (_sendQueue.empty())
            _RegisterSend.store(false);
        else
            RegisterSend();
    }

}

void MyCompltionKey::ProcessRecv(int32 numOfBytes)
{
    recvOverlapped._compltionKey = nullptr;

    if (numOfBytes == 0)
    {
        printf("ProcessRecv error %d", WSAGetLastError());
        RegisterDisConnect();
        return;
    }

    if (writePos + numOfBytes > 4096 - writePos)
    {
        printf("ProcessRecv error %d", WSAGetLastError());
        RegisterDisConnect();
        return;
    }

    writePos += numOfBytes;

    int32 dataSize = writePos - readPos;
    int32 processLen = OnRecv(&recvBuffer[readPos], dataSize);
    if (processLen < 0)
    {
        printf("OnRecv error %d", WSAGetLastError());
        RegisterDisConnect();
        return;
    }

    if (dataSize < processLen)
    {
        printf("OnRecv error %d", WSAGetLastError());
        RegisterDisConnect();
        return;
    }

    readPos += processLen;

    CleanBuf();

    RegisterRecv();

}

void MyCompltionKey::RegisterConnect()
{
    if (IsConnect() == false)
        return;

    //SetReuseAddress

    //Bind AnyAdresss

    connectOverlapped.Init();
    connectOverlapped._compltionKey = shared_from_this();

    //DWORD numOfBytes = 0;
    //SOCKADDR_IN sockAddr = _clientAddr;
    //if (false == SocketUtils::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), nullptr, 0, &numOfBytes, &_connectEvent))
    //{
    //    int32 errorCode = ::WSAGetLastError();
    //    if (errorCode != WSA_IO_PENDING)
    //    {
    //        _connectEvent.owner = nullptr; // RELEASE_REF
    //        return false;
    //    }
    //}

    //return true;
}

void MyCompltionKey::RegisterDisConnect()
{
    if (_connect.exchange(false) == false)
        return;

    return;

    disConnectOverlapped.Init();
    disConnectOverlapped._compltionKey = shared_from_this();

    if (false == DisconnectEx(_clientSocket, &disConnectOverlapped, TF_REUSE_SOCKET, 0))
    {
        int32 errorCode = ::WSAGetLastError();
        if (errorCode != WSA_IO_PENDING)
        {
            disConnectOverlapped._compltionKey = nullptr; // RELEASE_REF
            return;
        }
    }

    return;
}

void MyCompltionKey::RegisterSend()
{
    if (false == IsConnect())
        return;

    sendOverlapped.Init();
    sendOverlapped._compltionKey = shared_from_this();

    //TODO : 한번에 같이 보내기
    vector<WSABUF> wsaBufs;
    while (_sendQueue.empty() == false)
    {
        ::lock_guard<::mutex> guard(_mutex);

        //TEMP
        shared_ptr<BYTE*> temp = _sendQueue.front();
        WSABUF wsaBuf;
        wsaBuf.buf = reinterpret_cast<char*>(*temp.get());
        wsaBuf.len = static_cast<LONG>(_msize(*temp.get())/sizeof(BYTE));
        wsaBufs.push_back(wsaBuf);
        _sendQueue.pop();
    }

    //cout << "Bufsize : " << wsaBufs.size() << endl;
    //for (auto index : wsaBufs)
    //{
    //    cout << "BufLen : " << index.len << endl;
    //}

    DWORD Bytes = 0;
    if (SOCKET_ERROR == ::WSASend(
        _clientSocket,
        wsaBufs.data(),
        static_cast<DWORD>(wsaBufs.size()),
        &Bytes,
        0,
        &sendOverlapped,
        nullptr))
    {
        int32 errorCode = ::WSAGetLastError();
        if (errorCode != WSA_IO_PENDING)
        {
            printf("WSASend Error : %d\n", errorCode);
            sendOverlapped._compltionKey = nullptr;
            _RegisterSend.store(false);
        }
    }

    wsaBufs.clear();
}

void MyCompltionKey::RegisterRecv()
{
    if (false == IsConnect())
        return;

    recvOverlapped.Init();
    recvOverlapped._compltionKey = shared_from_this();

    WSABUF wsaBuf;
    wsaBuf.buf = reinterpret_cast<char*>(&recvBuffer[writePos]);
    wsaBuf.len = static_cast<ULONG>(4096 - writePos);
    DWORD numOfBytes = 0;
    DWORD flags = 0;

    if (SOCKET_ERROR == ::WSARecv(_clientSocket, &wsaBuf, 1, OUT & numOfBytes, OUT & flags, &recvOverlapped, nullptr))
    {
        int32 errorCode = ::WSAGetLastError();
        if (errorCode != WSA_IO_PENDING)
        {
            printf("WSARecv Error : %d\n", errorCode);
            recvOverlapped._compltionKey = nullptr; // RELEASE_REF
        }
    }
}

void MyCompltionKey::Init(SOCKET socket)
{
    _clientSocket = socket;

    LINGER option;
    option.l_onoff = 0;
    option.l_linger = 0;
    setsockopt(_clientSocket, SOL_SOCKET, SO_LINGER, reinterpret_cast<char*>(&option), sizeof(option));
}

void MyCompltionKey::CleanBuf()
{

    if (readPos - writePos == 0)
    {
        readPos = 0;
        writePos = 0;
    }
}
