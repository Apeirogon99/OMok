// Fill out your copyright notice in the Description page of Project Settings.


#include "UserController.h"

AUserController::AUserController()
{

}

AUserController::~AUserController()
{

}

void AUserController::ShowLobbyWidget()
{
	TSubclassOf<UW_Lobby> W_Lobby = LoadClass<UW_Lobby>(NULL, TEXT("UserWidget'/Game/Widget/BW_Lobby.BW_Lobby_C'"));
	if (W_Lobby)
	{
		_Lobby = Cast<UW_Lobby>(CreateWidget(this, W_Lobby));
		_Lobby->AddToViewport();
	}
}

