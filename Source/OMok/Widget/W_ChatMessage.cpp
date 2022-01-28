// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ChatMessage.h"
#include <Components/TextBlock.h>

void UW_ChatMessage::NativeConstruct()
{
	Super::NativeConstruct();

	_TB_NickName	= Cast<UTextBlock>(GetWidgetFromName(TEXT("_TB_NickName")));
	_TB_Message		= Cast<UTextBlock>(GetWidgetFromName(TEXT("_TB_Message")));
}

void UW_ChatMessage::InitMessageBox(FText NickName, FText text)
{
	_nickName = NickName;
	_message = text;

	UE_LOG(LogTemp, Warning, TEXT("InitMessageBox - name : %s, message : %s"), *NickName.ToString(), *text.ToString());

	if (_TB_NickName)
	{
		_TB_NickName->SetText(_nickName);
	}

	if (_TB_Message)
	{
		_TB_Message->SetText(_message);
	}
}
