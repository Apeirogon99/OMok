// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "BattleGameState.h"
#include "BattlePlayerState.h"
#include "UserController.h"
#include "UserPawn.h"
#include <OMok/Server/PacketHandle.h>

ABattleGameMode::ABattleGameMode()
{
	PlayerControllerClass = AUserController::StaticClass();
	GameStateClass = ABattleGameState::StaticClass();
	PlayerStateClass = APlayerState::StaticClass();
	DefaultPawnClass = AUserPawn::StaticClass();
}

ABattleGameMode::~ABattleGameMode()
{
}

void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	PacketHandle::Init(GetWorld());
}
