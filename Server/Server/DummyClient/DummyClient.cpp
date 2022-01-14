#include"pch.h"
#include "SimpleClient.h"
#include "Protocol.pb.h"

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

int main()
{
	SimpleClient* sc = new SimpleClient();
	sc->ClientStart();
}