// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OMOK_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABattleGameMode();
	virtual ~ABattleGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	class NetworkCore* _networkCore;
};
