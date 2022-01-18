// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Login.h"
#include <Components/Button.h>
#include <Components/EditableTextBox.h>
#include <OMok/Server/PacketHandle.h>

void UW_Login::NativeConstruct()
{
	Super::NativeConstruct();

	_btn_Login = Cast<UButton>(GetWidgetFromName(TEXT("_btn_Login")));
	_btn_SingUp = Cast<UButton>(GetWidgetFromName(TEXT("_btn_SingUp")));
	_ET_Id = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("_ET_Id")));
	_ET_Password = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("_ET_Password")));

	//UEditableTextBox
	_ET_Id->HintText = FText::FromString(FString(TEXT("Input Id")));
	_ET_Password->HintText = FText::FromString(FString(TEXT("Input Password")));
	_ET_Password->SetIsPassword(true);

	//UButton
	if (_btn_Login != nullptr)
	{
		_btn_Login->OnClicked.AddDynamic(this, &UW_Login::Click_Login);
	}

	if (_btn_SingUp != nullptr)
	{
		_btn_SingUp->OnClicked.AddDynamic(this, &UW_Login::Click_SingUp);
	}
}

void UW_Login::Click_Login()
{	
	if (_ET_Id->Text.ToString() == "" || _ET_Password->Text.ToString() == "")
		return;

	std::string Id = std::string(TCHAR_TO_ANSI(*_ET_Id->Text.ToString()));
	std::string Password = std::string(TCHAR_TO_ANSI(*_ET_Password->Text.ToString()));

	Protocol::C_LOGIN pkt;
	pkt.set_id(Id);
	pkt.set_password(Password);
	PacketHandle::MakeSendBuffer(pkt);

	this->SetVisibility(ESlateVisibility::Hidden);
}

void UW_Login::Click_SingUp()
{
}
