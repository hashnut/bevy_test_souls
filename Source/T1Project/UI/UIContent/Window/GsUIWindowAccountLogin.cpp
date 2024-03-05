// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowAccountLogin.h"

#include "Net/GsNetSendService.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "EditableTextBox.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUIWindowAccountLogin::NativeConstruct()
{
	Super::NativeConstruct();

	_editAccountID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("INPUT_ID")));
	_btnLogin = Cast<UGsButton>(GetWidgetFromName(TEXT("GsButton_Login")));

	if (nullptr != _btnLogin)
	{
		_btnLogin->OnClicked.AddDynamic(this, &UGsUIWindowAccountLogin::OnClickLogin);
	}
}

void UGsUIWindowAccountLogin::NativeDestruct()
{
	if (_btnLogin)
	{
		_btnLogin->OnClicked.RemoveDynamic(this, &UGsUIWindowAccountLogin::OnClickLogin);
	}

	Super::NativeDestruct();
}

void UGsUIWindowAccountLogin::SetData(const FString& InAccountID)
{
	if (nullptr != _editAccountID)
	{
		_editAccountID->SetText(FText::FromString(InAccountID));
	}
}

void UGsUIWindowAccountLogin::OnClickLogin()
{
	// 로그인 버튼을 눌렀을 때
	if (nullptr != _editAccountID)
	{
		FString accountID = _editAccountID->GetText().ToString();
		if (accountID.IsEmpty())
		{
			FText text;
			FText::FindText(TEXT("UICommonText"), TEXT("AccountIDEmpty"), text); // TEXT: 알림

			FGsUIHelper::PopupOK(text);
		}
		else
		{
			//FGsNetSendService::SendAuthReqUpdateAccountName(*_editAccountID->GetText().ToString());
			OnClickButton.ExecuteIfBound(*_editAccountID->GetText().ToString());
		}
	}
}

bool UGsUIWindowAccountLogin::OnBack()
{
	// 뒤로가기 처리
	return false;
}


