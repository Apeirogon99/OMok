// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <OMok/Widget/W_Lobby.h>
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
	~AUserController();

public:
	void ShowLobbyWidget();

public:
	UW_Lobby* _Lobby;
};
