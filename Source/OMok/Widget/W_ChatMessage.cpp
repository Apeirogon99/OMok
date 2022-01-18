// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ChatMessage.h"
#include <Components/TextBlock.h>

void UW_ChatMessage::NativeConstruct()
{
	Super::NativeConstruct();

	_TB_NickName	= Cast<UTextBlock>(GetWidgetFromName(TEXT("_TB_NickName")));
	_TB_Message		= Cast<UTextBlock>(GetWidgetFromName(TEXT("_TB_Message")));

	_TB_NickName->SetText(_nickName);
	_TB_Message->SetText(_message);
}