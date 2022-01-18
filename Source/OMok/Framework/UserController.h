// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UserController.generated.h"

/**
 * 
 */
UCLASS()
class OMOK_API AUserController : public APlayerController
{
	GENERATED_BODY()

public:
	AUserController();

protected:
	virtual void PlayerTick(float DeltaTime) override;
	
};
