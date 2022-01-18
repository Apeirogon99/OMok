// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Lobby.h"
#include <Components/Button.h>
#include <Components/CanvasPanel.h>
#include <Components/EditableTextBox.h>
#include <Components/ScrollBox.h>
#include <OMok/Server/PacketHandle.h>
#include <Blueprint/WidgetBlueprintLibrary.h>
#include <OMok/Widget/W_ChatMessage.h>
#include "Kismet/GameplayStatics.h"

void UW_Lobby::NativeConstruct()
{
	Super::NativeConstruct();

	//Init
	_btn_startOnlineMatch	= Cast<UButton>(GetWidgetFromName(TEXT("_btn_startOnlineMatch")));
	_btn_endOlineMatch		= Cast<UButton>(GetWidgetFromName(TEXT("_btn_endOlineMatch")));
	_btn_localMatch			= Cast<UButton>(GetWidgetFromName(TEXT("_btn_localMatch")));
	_btn_playHistory		= Cast<UButton>(GetWidgetFromName(TEXT("_btn_playHistory")));
	_btn_setting			= Cast<UButton>(GetWidgetFromName(TEXT("_btn_setting")));

	_SB_chatBox				= Cast<UScrollBox>(GetWidgetFromName(TEXT("_SB_chatBox")));

	_ET_chat				= Cast<UEditableTextBox>(GetWidgetFromName(TEXT("_ET_chat")));

	//ConstructorHelper
	W_ChatMessage = LoadClass<UW_ChatMessage>(NULL, TEXT("UserWidget'/Game/Widget/BW_ChatMessage.BW_ChatMessage_C'"));

	//UScrollBox
	
	//UEditableTextBox
	_ET_chat->HintText = FText::FromString(FString(TEXT("Input Comment...")));

	if (_ET_chat != nullptr)
	{
		_ET_chat->OnTextCommitted.AddDynamic(this, &UW_Lobby::Committed_Chat);
	}



	//UButton
	if (_btn_startOnlineMatch != nullptr)
	{
		_btn_startOnlineMatch->OnClicked.AddDynamic(this, &UW_Lobby::Click_startOnlineMatch);
	}

	if (_btn_endOlineMatch != nullptr)
	{
		_btn_endOlineMatch->OnClicked.AddDynamic(this, &UW_Lobby::Click_endOlineMatch);
	}

	if (_btn_localMatch != nullptr)
	{
		_btn_localMatch->OnClicked.AddDynamic(this, &UW_Lobby::Click_localMatch);
	}

	if (_btn_playHistory != nullptr)
	{
		_btn_playHistory->OnClicked.AddDynamic(this, &UW_Lobby::Click_playHistory);
	}

	if (_btn_setting != nullptr)
	{
		_btn_setting->OnClicked.AddDynamic(this, &UW_Lobby::Click_setting);
	}
}

void UW_Lobby::Click_startOnlineMatch()
{
	UE_LOG(LogTemp, Warning, TEXT("Click_startOnlineMatch"));
	//TODO : Start Match Game Packet
	//Protocol::S_MATCHING_GAME pkt;
	//PacketHandle::MakeSendBuffer(pkt);
}

void UW_Lobby::Click_endOlineMatch()
{
	UE_LOG(LogTemp, Warning, TEXT("Click_endOlineMatch"));
	//TODO : End Match Game Packet
	//Protocol::S_MATCHING_GAME pkt;
	//PacketHandle::MakeSendBuffer(pkt);
}

void UW_Lobby::Click_localMatch()
{
	UE_LOG(LogTemp, Warning, TEXT("Click_localMatch"));
}

void UW_Lobby::Click_playHistory()
{
	UE_LOG(LogTemp, Warning, TEXT("Click_playHistory"));
}

void UW_Lobby::Click_setting()
{
	UE_LOG(LogTemp, Warning, TEXT("Click_setting"));
}

void UW_Lobby::Update_chatBox(const FText& NickName, const FText& text)
{
	//TODO : Create Chat Message Widget
	UW_ChatMessage* message = CreateWidget<UW_ChatMessage>(this, W_ChatMessage);
	if(message != nullptr)
	{
		message->_nickName = NickName;
		message->_message = text;
		_SB_chatBox->AddChild(message);
		_SB_chatBox->ScrollToEnd();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("message == nullptr"));
	}

}

void UW_Lobby::Committed_Chat(const FText& message, ETextCommit::Type commitType)
{
	if (ETextCommit::Type::OnEnter == commitType)
	{
		std::string outMessage = std::string(TCHAR_TO_ANSI(*message.ToString()));

		//TODO : Make Send Chat Packet
		Protocol::C_CHAT_LOBBY pkt;
		//pkt.set_playerid(1);
		//pkt.set_playernickname("hong");
		pkt.set_message(outMessage);
		PacketHandle::MakeSendBuffer(pkt);


		_ET_chat->SetText(FText::FromString(FString(TEXT(""))));

		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		UWidgetBlueprintLibrary::SetInputMode_GameAndUI(PC, _ET_chat);
	}
}
