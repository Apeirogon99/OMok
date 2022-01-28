// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ChatMessage.generated.h"

/**
 * 
 */
UCLASS()
class OMOK_API UW_ChatMessage : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//virtual void NativeOnInitialized() override;
	//virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	//virtual void NativeDestruct() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* _TB_NickName;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* _TB_Message;

public:

	UFUNCTION()
	void InitMessageBox(FText NickName, FText text);

	//UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	FText _nickName;

	//UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	FText _message;
};
