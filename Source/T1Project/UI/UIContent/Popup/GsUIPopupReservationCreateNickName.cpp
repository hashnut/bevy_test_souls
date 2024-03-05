// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupReservationCreateNickName.h"
#include "UTIL/GsText.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/TextBlock.h"


void UGsUIPopupReservationCreateNickName::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupReservationCreateNickName::OnClickOK);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupReservationCreateNickName::OnClickCancel);
}

void UGsUIPopupReservationCreateNickName::NativeConstruct()
{
	Super::NativeConstruct();	
}

void UGsUIPopupReservationCreateNickName::NativeDestruct()
{
	_okCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupReservationCreateNickName::OnInputOk()
{
	CallOkCallback();
}

void UGsUIPopupReservationCreateNickName::OnInputCancel()
{
	Close();
}

void UGsUIPopupReservationCreateNickName::SetOkCallback(TFunction<void(const FString&)> InOkCallback)
{
	FString advReservationUserName = GNet()->GetAdvReservationUserName();
	if (!advReservationUserName.IsEmpty())
	{
		_nickName->SetText(FText::FromString(advReservationUserName));
	}

	_okCallback = InOkCallback;
}

void UGsUIPopupReservationCreateNickName::CallOkCallback()
{
	if (_okCallback)
	{
		FString advReservationUserName = GNet()->GetAdvReservationUserName();
		_okCallback(advReservationUserName);
	}
}

void UGsUIPopupReservationCreateNickName::OnClickOK()
{	
	OnInputOk();	
}

void UGsUIPopupReservationCreateNickName::OnClickCancel()
{
	OnInputCancel();
}
