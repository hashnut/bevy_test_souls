// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupPKBookAddBoundaryTarget.h"
#include "EditableTextBox.h"
#include "UTIL/GsText.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUIPopupPKBookAddBoundaryTarget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_editNickName->OnTextChanged.AddDynamic(this, &UGsUIPopupPKBookAddBoundaryTarget::OnTextChangedNickName);
	
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupPKBookAddBoundaryTarget::OnClickCancel);
	_btnApply->OnClicked.AddDynamic(this, &UGsUIPopupPKBookAddBoundaryTarget::OnClickOK);
}

void UGsUIPopupPKBookAddBoundaryTarget::NativeConstruct()
{
	_editNickName->SetText(FText::GetEmpty());

	Super::NativeConstruct();
}

void UGsUIPopupPKBookAddBoundaryTarget::NativeDestruct()
{
	_okCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupPKBookAddBoundaryTarget::OnInputOk()
{
	FString strText = _editNickName->GetText().ToString();
	
	auto result = FGsTextUtil::IsValidUserSearchNameOnly(strText);
	if (FGsTextUtil::EUserNameValid::Valid == result.Get<0>())
	{
		this->CallOkCallback(result.Get<1>(), result.Get<2>());
		this->Close();
	}
}

void UGsUIPopupPKBookAddBoundaryTarget::OnInputCancel()
{
	Close();
}

void UGsUIPopupPKBookAddBoundaryTarget::SetOkCallback(TFunction<void(WorldId InWorldId, const FString&)> InOkCallback)
{
	_okCallback = InOkCallback;
}

void UGsUIPopupPKBookAddBoundaryTarget::CallOkCallback(WorldId InWorldId, const FText& InUserName)
{
	if (_okCallback)
	{
		_okCallback(InWorldId, InUserName.ToString());
	}
}

void UGsUIPopupPKBookAddBoundaryTarget::OnTextChangedNickName(const FText& InText)
{
	FString strText;
	// 2023/8/31 PKT - 제한된 문자열 길이에 맞게 조절
	if (FGsTextUtil::EUserNameValid::Valid != FGsTextUtil::CheckedUserNameLengthLimit(InText.ToString(), strText))
	{
		_editNickName->SetText(FText::FromString(strText));
	}
}

void UGsUIPopupPKBookAddBoundaryTarget::OnClickOK()
{
	OnInputOk();
}

void UGsUIPopupPKBookAddBoundaryTarget::OnClickCancel()
{
	OnInputCancel();
}
