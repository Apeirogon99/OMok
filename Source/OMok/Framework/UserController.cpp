// Fill out your copyright notice in the Description page of Project Settings.


#include "UserController.h"
#include <OMok/Widget/W_Lobby.h>

AUserController::AUserController()
{
	W_Lobby = LoadClass<UW_Lobby>(NULL, TEXT("UserWidget'/Game/Widget/BW_Lobby.BW_Lobby_C'"));
}

AUserController::~AUserController()
{

}

void AUserController::ShowLobbyWidget()
{
	if (W_Lobby)
	{
		auto widget = CreateWidget(this, W_Lobby);
		widget->AddToViewport();
	}
}

