// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Floor_Part = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Floor_Part_Mesh(TEXT("/Game/Client/Actor/floor.floor"));
	if (Floor_Part_Mesh.Succeeded())
	{
		Floor_Part->SetStaticMesh(Floor_Part_Mesh.Object);
	}

	RootComponent=Floor_Part;
	
	Board_Arr.Init(0,225);
}

void ABoard::SetBoard_Array(const int X, const int Y)
{
	//RemoveAt이용
}

void ABoard::Print_OMok(const int x, const int y, const int64 team, const int64 playerid)
{
	AOmak_Actor* Spawn_Omak_Actor = GetWorld()->SpawnActor<AOmak_Actor>(AOmak_Actor::StaticClass(), FVector(x, y, 0.f), FRotator(0, 0, 0));
	UMaterialInstanceDynamic* Spawn_Omak_Material = Spawn_Omak_Actor->GetMaterial_Dynamic();
	Spawn_Omak_Material->SetScalarParameterValue(TEXT("Opacity"), 1.f);

	SetBoard_Array(x, y);
	
}