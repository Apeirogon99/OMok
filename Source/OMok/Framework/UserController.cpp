// Fill out your copyright notice in the Description page of Project Settings.


#include "UserController.h"

AUserController::AUserController()
{
	bShowMouseCursor=true;
}

void AUserController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	//DrawDebugLine(GetWorld(),Start,End,FColor::Orange,false,2.f);
}