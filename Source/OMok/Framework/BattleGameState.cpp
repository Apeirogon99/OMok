// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameState.h"
#include <OMok/Server/PacketHandle.h>

ABattleGameState::ABattleGameState()
{

}

ABattleGameState::~ABattleGameState()
{
	delete _networkCore;
}

void ABattleGameState::BeginPlay()
{
	Super::BeginPlay();

	_networkCore = new NetworkCore();
	if (_networkCore)
	{
		UE_LOG(LogTemp, Warning, TEXT("Create GameState and NetworkCore"));
	}
}
