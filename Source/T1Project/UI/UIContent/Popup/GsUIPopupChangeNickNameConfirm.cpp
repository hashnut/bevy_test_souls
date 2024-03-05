// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupChangeNickNameConfirm.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Management/GsMessageHolder.h"
#include "Input/GsInputEventMsgBase.h"
#include "DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"


void UGsUIPopupChangeNickNameConfirm::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupChangeNickNameConfirm::OnClickOK);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupChangeNickNameConfirm::OnClickCancel);
}

void UGsUIPopupChangeNickNameConfirm::NativeConstruct()
{
	Super::NativeConstruct();

	if (FGsMessageHolder* msgMgr = GMessage())
	{
		FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_COMMON_POPUP, true);
		msgMgr->GetInput().SendMessage(MessageInput::BlockInput, msg);
	}

	_bIsOk = false;
}

void UGsUIPopupChangeNickNameConfirm::NativeDestruct()
{
	// OK일 경우 그 다음팝업을 위해 블록 상태를 유지, 그 외의 경우는 닫을 때 블록을 푼다
	if (false == _bIsOk)
	{
		if (FGsMessageHolder* msgMgr = GMessage())
		{
			FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_COMMON_POPUP, false);
			msgMgr->GetInput().SendMessage(MessageInput::BlockInput, msg);
		}
	}

	_bIsOk = false;

	Super::NativeDestruct();
}

void UGsUIPopupChangeNickNameConfirm::OnInputOk()
{
	_bIsOk = true;

	if (FGsMessageHolder* msgMgr = GMessage())
	{
		msgMgr->GetItemContents().SendMessage(MessageContentItem::CHANGE_NICKNAME_CONFIRM_REQUEST, nullptr);
	}	

	// 성공 실패 상관 없이 무조건 닫음
	Close();
}

void UGsUIPopupChangeNickNameConfirm::OnInputCancel()
{
	Close();
}

void UGsUIPopupChangeNickNameConfirm::OnClickOK()
{
	OnInputOk();
}

void UGsUIPopupChangeNickNameConfirm::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupChangeNickNameConfirm::SetData(const FString& InCurrNickName, const FString& InChangeNickName)
{
	_tbCurrNickName->SetText(FText::FromString(InCurrNickName));
	_tbChangeNickName->SetText(FText::FromString(InChangeNickName));
}
