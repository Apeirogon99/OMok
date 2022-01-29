// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameState.h"
#include <OMok/Server/PacketHandle.h>

ABattleGameState::ABattleGameState()
{

}

ABattleGameState::~ABattleGameState()
{
	
}

void ABattleGameState::BeginPlay()
{
	Super::BeginPlay();


	TSubclassOf<AActor> Board_Actor;
	UGameplayStatics::GetActorOfClass(GetWorld(), Board_Actor);
	Omak_Board = Cast<ABoard>(Board_Actor);

}
