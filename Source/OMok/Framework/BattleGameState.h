// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include <OMok/Server/NetworkCore.h>
#include "BattleGameState.generated.h"

/**
 * 
 */
UCLASS()
class OMOK_API ABattleGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ABattleGameState();
	~ABattleGameState();

	virtual void BeginPlay() override;

public:
};
