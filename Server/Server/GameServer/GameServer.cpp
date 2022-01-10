// GameServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "MyIocpServer.h"

int main()
{
    //IOCPSERVER -> Session -> PacketSession -> GameSession(key) -> Player -> IO



    MyIocpServer iocpServer;
    iocpServer.StartIocpServer();

    getchar();

    iocpServer.CloseIocpServer();

    return 0;
}

