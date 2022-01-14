// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePlayerState.h"

ABattlePlayerState::ABattlePlayerState()
{
	UE_LOG(LogTemp, Warning, TEXT("Create PlayerState"));
}

ABattlePlayerState::~ABattlePlayerState()
{

}

void ABattlePlayerState::setPlayerInfo(bool team, int64 gameId)
{
	this->_team = team;
	this->_gameId = gameId;
}

