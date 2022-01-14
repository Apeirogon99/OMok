// Fill out your copyright notice in the Description page of Project Settings.


#include "Omak_Actor.h"

// Sets default values
AOmak_Actor::AOmak_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Omak_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Omak_Mesh_Data(TEXT("/Game/Client/Actor/OmakMesh.OmakMesh"));
	if (Omak_Mesh_Data.Succeeded())
	{
		Omak_Mesh->SetStaticMesh(Omak_Mesh_Data.Object);
	}
}

// Called when the game starts or when spawned
void AOmak_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOmak_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

