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

	RootComponent=Omak_Mesh;
}

// Called when the game starts or when spawned
void AOmak_Actor::BeginPlay()
{
	Super::BeginPlay();

	Material = Omak_Mesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(Material,this);
	Omak_Mesh->SetMaterial(0,DynamicMaterial);
	DynamicMaterial->SetScalarParameterValue(TEXT("Opacity"),0.3f);
}

// Called every frame
void AOmak_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UMaterialInstanceDynamic* AOmak_Actor::GetMaterial_Dynamic()
{
	return DynamicMaterial;
}
