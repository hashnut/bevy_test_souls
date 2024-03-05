// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDSaveBattery.h"

#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSkillManager.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"


void UGsUIHUDSaveBattery::BeginDestroy()
{
	if (nullptr != _btnSaveBattery)
	{
		_btnSaveBattery->OnClicked.RemoveDynamic(this, &UGsUIHUDSaveBattery::OnClickSaveBattery);
	}
	_btnSaveBattery = nullptr;

	Super::BeginDestroy();
}

void UGsUIHUDSaveBattery::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSaveBattery->OnClicked.AddDynamic(this, &UGsUIHUDSaveBattery::OnClickSaveBattery);
}

void UGsUIHUDSaveBattery::NativeConstruct()
{
	Super::NativeConstruct();

	_unlockDelegateList.Emplace(GMessage()->GetUnlock().AddUObject(MessageContentUnlock::INVALIDATE_ALL_UNLOCK,
		this, &UGsUIHUDSaveBattery::OnContentsUnlock));
	_unlockDelegateList.Emplace(GMessage()->GetUnlock().AddUObject(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK,
		this, &UGsUIHUDSaveBattery::OnContentsUnlock));
}

void UGsUIHUDSaveBattery::NativeDestruct()
{
	MUnlock& msgUnlock = GMessage()->GetUnlock();
	for (MsgHandleUnlock& elemUnlock : _unlockDelegateList)
	{
		msgUnlock.Remove(elemUnlock);
	}
	_unlockDelegateList.Empty();

	Super::NativeDestruct();
}

void UGsUIHUDSaveBattery::OnClickSaveBattery()
{
	if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::SAVE_BATTERY, true))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_SAVE_BATTERY);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDSaveBattery::OnContentsUnlock(const IGsMessageParam* InParam)
{
	_btnSaveBattery->SetSwitcherIndex(
		GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::SAVE_BATTERY) ? 0 : 1); // 0 :UNLOCKED, 1 : LOCKED
}
