// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Floor_Part = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> Floor_Part_Mesh(TEXT("/Game/Client/Actor/floor.floor"));
	if (Floor_Part_Mesh.Succeeded())
	{
		Floor_Part->SetStaticMesh(Floor_Part_Mesh.Object);
	}
	
	for (int PosX = 0; PosX < 15; PosX++)
	{
		for (int PosY = 0; PosY < 15; PosY++)
		{
			SpawnPoint[PosX][PosY] = FVector2D(PosX*100.f, PosY*100.f);
		}
	}
}