// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMainMenuBtnBase.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"


void UGsUIMainMenuBtnBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIMainMenuBtnBase::OnClickSlot);
}

void UGsUIMainMenuBtnBase::NativeConstruct()
{
	Super::NativeConstruct();

	_btnSlot->SetOnClickLockSec(0.2f);
}

void UGsUIMainMenuBtnBase::OnClickSlot()
{
	OnClickMenu.ExecuteIfBound();

	if (IsTutorialInteraction())
	{
		OnTutorialInteraction.ExecuteIfBound();
	}
}

void UGsUIMainMenuBtnBase::SetButtonLock(bool bIsLocked)
{
	_btnSlot->SetSwitcherIndex(bIsLocked ? 1 : 0);
}

void UGsUIMainMenuBtnBase::UpdateContentLock()
{
	if (FGsUnlockManager* unlockMgr = GSUnlock())
	{
		// 강제로 막았으면 무조건 lock, 아니면 Unlock 상태를 따름
		bool bIsLocked = _bForcedDisable ? true : !unlockMgr->IsContentsUnlock(_unlockContentsType);	
		SetButtonLock(bIsLocked);

		SetContentIsNew(unlockMgr->IsNewContents(_unlockContentsType));
	}
}

bool UGsUIMainMenuBtnBase::IsEnableButton() const
{
	return (0 == _btnSlot->GetSwitcherIndex()) ? true : false;
}

bool UGsUIMainMenuBtnBase::OnCheckCanClick()
{
	// 눌러지면 New마크는 무조건 끈다
	SetContentIsNew(false);

	if (FGsUnlockManager* unlockMgr = GSUnlock())
	{
		// New가 켜져있다면 끈다
		if (unlockMgr->IsNewContents(_unlockContentsType))
		{
			unlockMgr->SetIsNewContents(_unlockContentsType, false);
		}
	}

	// 불가능할 경우 티커 출력
	if (false == IsEnableButton())
	{
		GSUnlock()->ShowLockTicker(_unlockContentsType);
		return false;
	}

	return true;
}

void UGsUIMainMenuBtnBase::SetIsTutorialInteraction(bool bIsTutorial)
{
	if (bIsTutorial)
	{
		_backupBtnLockSec = _btnSlot->_clickLockSecond;
		_btnSlot->SetOnClickLockSec(1.f);
	}
	else
	{
		_btnSlot->SetOnClickLockSec(_backupBtnLockSec);
	}
}
