// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BattlePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class OMOK_API ABattlePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ABattlePlayerState();
	~ABattlePlayerState();
	
public:
	bool _team;
	int64 _gameId;
	FString _nickName;

};
