// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Omak_Actor.generated.h"

UCLASS()
class OMOK_API AOmak_Actor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AOmak_Actor();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Omak_Mesh;

	UPROPERTY(VisibleAnywhere)
	UMaterialInterface* Material;
	
	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* DynamicMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//get dynamic material
	UMaterialInstanceDynamic* GetMaterial_Dynamic();
};
