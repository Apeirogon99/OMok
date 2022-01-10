#include "pch.h"
#include "MYOverlapped.h"

MyOverlapped::MyOverlapped(OverlappedType type) : _type(type)
{
	Init();
}

void MyOverlapped::Init()
{
	OVERLAPPED::hEvent = 0;
	OVERLAPPED::Internal = 0;
	OVERLAPPED::InternalHigh = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;
}