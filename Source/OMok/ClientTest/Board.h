// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Board.generated.h"

UCLASS()
class OMOK_API ABoard : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category=Floor)
	UStaticMeshComponent* Floor_Part;
	
	UPROPERTY(VisibleAnywhere)
	FVector2D SpawnPoint[15][15];

public:	
	// Sets default values for this actor's properties
	ABoard();
};
