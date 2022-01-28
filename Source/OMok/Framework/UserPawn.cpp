// Fill out your copyright notice in the Description page of Project Settings.


#include "UserPawn.h"

#include <string>

#include "UserController.h"
#include "Kismet/GameplayStatics.h"

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

	TSubclassOf<AActor> Board_Actor;
	UGameplayStatics::GetActorOfClass(GetWorld(),Board_Actor);
	Omak_Board = Cast<ABoard>(Board_Actor);
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
		AOmak_Actor* Spawn_Omak_Actor = GetWorld()->SpawnActor<AOmak_Actor>(AOmak_Actor::StaticClass(),FVector(VecX,VecY,0.f),FRotator(0,0,0));
		UMaterialInstanceDynamic* Spawn_Omak_Material = Spawn_Omak_Actor->GetMaterial_Dynamic();
		Spawn_Omak_Material->SetScalarParameterValue(TEXT("Opacity"),1.f);

		const int x=floor((Loc.X)/100.f);
		const int y=floor((Loc.Y)/100.f);
		
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("x : %d, y : %d"),x,y));
		
		Omak_Board->SetBoard_Array(x,y);
	}
}