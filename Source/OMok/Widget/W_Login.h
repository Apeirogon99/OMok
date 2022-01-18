// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Login.generated.h"

/**
 * 
 */
UCLASS()
class OMOK_API UW_Login : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//virtual void NativeOnInitialized() override;
	//virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	//virtual void NativeDestruct() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


	UPROPERTY(Meta = (BindWidget))
	class UButton* _btn_Login;

	UPROPERTY(Meta = (BindWidget))
	class UButton* _btn_SingUp;

	UPROPERTY(Meta = (BindWidget))
	class UEditableTextBox* _ET_Id;

	UPROPERTY(Meta = (BindWidget))
	class UEditableTextBox* _ET_Password;

public:
	UFUNCTION()
	void Click_Login();

	UFUNCTION()
	void Click_SingUp();

public:

};
