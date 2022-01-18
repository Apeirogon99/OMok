// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class OMOK_API UW_Lobby : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	//virtual void NativeOnInitialized() override;
	//virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	//virtual void NativeDestruct() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UPROPERTY(Meta = (BindWidget))
	class UButton*		_btn_startOnlineMatch;

	UPROPERTY(Meta = (BindWidget))
	class UButton* _btn_endOlineMatch;

	UPROPERTY(Meta = (BindWidget))
	class UButton*		_btn_localMatch;

	UPROPERTY(Meta = (BindWidget))
	class UButton*		_btn_playHistory;

	UPROPERTY(Meta = (BindWidget))
	class UButton*		_btn_setting;

	UPROPERTY(Meta = (BindWidget))
	class UScrollBox*	_SB_chatBox;

	UPROPERTY(Meta = (BindWidget))
	class UEditableTextBox* _ET_chat;

public:
	UFUNCTION()
	void Click_startOnlineMatch();

	UFUNCTION()
	void Click_endOlineMatch();

	UFUNCTION()
	void Click_localMatch();

	UFUNCTION()
	void Click_playHistory();
	
	UFUNCTION()
	void Click_setting();

	UFUNCTION()
	void Update_chatBox(const FText& NickName, const FText& text);

	UFUNCTION()
	void Committed_Chat(const FText& message, ETextCommit::Type commitType);

public:
	TSubclassOf<class UW_ChatMessage> W_ChatMessage;
	TArray<UW_ChatMessage*> _ChatLog;
};
