// Fill out your copyright notice in the Description page of Project Settings.


#include "UserPawn.h"

#include <string>

#include "UserController.h"
#include "Kismet/GameplayStatics.h"
#include <OMok/Server/PacketHandle.h>

// Sets default values
AUserPawn::AUserPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Omak Actor
	SpawnActor=CreateDefaultSubobject<AOmak_Actor>(TEXT("Actor"));
	SpawnActor->SetActorRotation(FRotator(0.f,270.f,0.f));
	
	//CameraArm
	CameraArm=CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetUsingAbsoluteRotation(true);
	CameraArm->SetRelativeRotation(FRotator(270.f,0.f,0.f));
	CameraArm->bDoCollisionTest=false;
	
	//Camera
	Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AUserPawn::BeginPlay()
{
	Super::BeginPlay();

	SpawnActor = GetWorld()->SpawnActor<AOmak_Actor>(AOmak_Actor::StaticClass(),FVector(750,750,-100),FRotator(0,0,0));
}

// Called every frame
void AUserPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(AUserController* UC = Cast<AUserController>(GetController()))
	{
		FHitResult TraceHitResult;
		UC->GetHitResultUnderCursor(ECC_Visibility,true,TraceHitResult);
		FVector Loc=TraceHitResult.Location;
		FString b=Loc.ToString();
		//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,b);
		const float VecX=floor(Loc.X/100.f)*100.f+50.f;
		const float VecY=floor(Loc.Y/100.f)*100.f+50.f;
		SpawnActor->SetActorLocation(FVector(VecX,VecY,0));
	}
}

// Called to bind functionality to input
void AUserPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Click",IE_Pressed,this,&AUserPawn::Click);
}

void AUserPawn::Click()
{
	if(AUserController* UC = Cast<AUserController>(GetController()))
	{
		FHitResult TraceHitResult;
		UC->GetHitResultUnderCursor(ECC_Visibility,true,TraceHitResult);
		FVector Loc = TraceHitResult.Location;
		const float VecX=floor(Loc.X/100.f)*100.f+50.f;
		const float VecY=floor(Loc.Y/100.f)*100.f+50.f;

		const int x=floor((Loc.X)/100.f);
		const int y=floor((Loc.Y)/100.f);
		
		//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("x : %d, y : %d"),x,y));

		auto gameinstance  = Cast<UOMokGameInstance>(GetWorld()->GetGameInstance());
		if (gameinstance)
		{
			Protocol::C_NEXT_TURN pkt;
			pkt.set_posx(x);
			pkt.set_posy(y);
			pkt.set_team(1);
			pkt.set_playerid(gameinstance->_playerId);
		}
	


		//Omak_Board->SetBoard_Array(x,y);
	}
}