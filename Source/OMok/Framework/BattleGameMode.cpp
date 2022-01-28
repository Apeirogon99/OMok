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

	_networkCore = new NetworkCore();
	if (_networkCore)
	{
		UE_LOG(LogTemp, Warning, TEXT("Create GameState and NetworkCore"));
		}

	PacketHandle::Init(GetWorld());
}

void ABattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	auto player = Cast<AUserController>(NewPlayer);
	player->ShowLobbyWidget();
	player->SetShowMouseCursor(true);
}
