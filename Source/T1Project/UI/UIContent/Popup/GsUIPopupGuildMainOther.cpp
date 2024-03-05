// Fill out your copyright notice in the Description page of Project Settings.
#include "GsUIPopupGuildMainOther.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentMain.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentOperation.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Management/GsMessageHolder.h"


void UGsUIPopupGuildMainOther::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnBack->OnClicked.AddDynamic(this, &UGsUIPopupGuildMainOther::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupGuildMainOther::OnClickCloseAllStack);

	_toggleGroupMainTab.AddToggleWidgetByParentPanel(_panelMainTab);
	_toggleGroupMainTab.OnSelectChanged.BindUObject(this, &UGsUIPopupGuildMainOther::OnClickMainTab);
	_toggleGroupMainTab.OnChangeBlocked.BindUObject(this, &UGsUIPopupGuildMainOther::OnClickBlockedTab);

	_contentList.Empty();
	_contentList.Emplace(_contentMain);
	_contentList.Emplace(_contentOperation);

	for (int32 i = 0; i < _contentList.Num(); ++i)
	{
		_contentList[i]->SetMainTabIndex(i);
	}
}

void UGsUIPopupGuildMainOther::NativeConstruct()
{
	Super::NativeConstruct();

	/* 탭 별 언락컨텐츠타입 연결 */
	_unlockTypeMap.Empty();
	_unlockTypeMap.Emplace(EGsGuildMainTab::Main, EGsUnlockContentsType::GUILD_MAIN_TAB);
	_unlockTypeMap.Emplace(EGsGuildMainTab::Operation, EGsUnlockContentsType::GUILD_BUILDING_TAB);

	BindMessages();

	CheckUnlock();

	int32 selectedIndex = 0;
	// 열고자하는 탭에 진입할 수 없을 경우 열 수 있는 가장 왼쪽 탭을 연다.
	if (_toggleGroupMainTab.IsBlockedIndex(selectedIndex))
	{
		const int32 MAX_TAB_INDEX = StaticCast<int32>(EGsGuildMainTab::Max);
		int32 index = 0;
		for (index = StaticCast<int32>(EGsGuildMainTab::Main); index < MAX_TAB_INDEX; ++index)
		{
			if (false == _toggleGroupMainTab.IsBlockedIndex(index))
			{
				selectedIndex = index;
				break;
			}
		}
	}

	_toggleGroupMainTab.SetSelectedIndex(selectedIndex, true);

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->AddUIScaleFlag(EGsUIScaleFlags::UI_SCALE_NPC_DIALOG);
	}
}

void UGsUIPopupGuildMainOther::NativeDestruct()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->RemoveUIScaleFlag(EGsUIScaleFlags::UI_SCALE_NPC_DIALOG);
	}

	UnbindMessages();

	_tabPrev = EGsGuildMainTab::Max;

	_unlockTypeMap.Empty();

	Super::NativeDestruct();
}

FReply UGsUIPopupGuildMainOther::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUIPopupGuildMainOther::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUIPopupGuildMainOther::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

void UGsUIPopupGuildMainOther::BindMessages()
{
	MUnlock& msgUnlock = GMessage()->GetUnlock();
	_msgUnlockHandleList.Emplace(msgUnlock.AddUObject(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK,
		this, &UGsUIPopupGuildMainOther::OnContentsUnlock));
	_msgUnlockHandleList.Emplace(msgUnlock.AddUObject(MessageContentUnlock::INVALIDATE_ALL_UNLOCK,
		this, &UGsUIPopupGuildMainOther::OnContentsUnlock));
}

void UGsUIPopupGuildMainOther::UnbindMessages()
{
	MUnlock& msgUnlock = GMessage()->GetUnlock();
	for (MsgHandleUnlock& handle : _msgUnlockHandleList)
	{
		msgUnlock.Remove(handle);
	}
	_msgUnlockHandleList.Empty();
}

void UGsUIPopupGuildMainOther::OnInputCancel()
{
	OnClickBack();
}

void UGsUIPopupGuildMainOther::SetUICurrentContent(int32 InIndex)
{
	// 이전 페이지 나가기 처리
	if (EGsGuildMainTab::Max != _tabPrev)
	{
		if (_contentList.IsValidIndex(_tabPrev))
		{
			if (UGsUIGuildContentBase* prevContent = _contentList[_tabPrev])
			{
				prevContent->Hide();
			}
		}
	}

	_switcherContent->SetActiveWidgetIndex(InIndex);

	if (_contentList.IsValidIndex(InIndex))
	{
		if (UGsUIGuildContentBase* content = _contentList[InIndex])
		{
			content->Show(false, false);

			_tabPrev = static_cast<EGsGuildMainTab>(InIndex);
		}
	}
}

void UGsUIPopupGuildMainOther::CheckUnlock()
{
	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	for (TPair<EGsGuildMainTab, EGsUnlockContentsType> tabUnlockPair : _unlockTypeMap)
	{
		bool bIsUnlock = unlockManager->IsContentsUnlock(tabUnlockPair.Value);
		int32 tabIndex = tabUnlockPair.Key;

		if (bIsUnlock)
		{
			// 컨텐츠를 사용할 수 있다.
			_toggleGroupMainTab.DeleteBlockedIndex(tabIndex);
		}
		else
		{
			// 컨텐츠를 사용할 수 없다.
			if (false == _toggleGroupMainTab.IsBlockedIndex(tabIndex))
			{
				_toggleGroupMainTab.AddBlockedIndex(tabIndex);
			}
		}
	}
}

void UGsUIPopupGuildMainOther::OnClickMainTab(int32 InIndex)
{
	SetUICurrentContent(InIndex);
}

void UGsUIPopupGuildMainOther::OnClickBlockedTab(int32 InIndex)
{
	EGsGuildMainTab clickedTab = StaticCast<EGsGuildMainTab>(InIndex);
	if (_unlockTypeMap.Contains(clickedTab))
	{
		GSUnlock()->IsContentsUnlock(_unlockTypeMap[clickedTab], true);
	}
}

void UGsUIPopupGuildMainOther::OnClickCloseAllStack()
{
	Close();

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->CloseAllStack(this);
	}
}

void UGsUIPopupGuildMainOther::OnContentsUnlock(const IGsMessageParam* InParam)
{
	CheckUnlock();

	// 보고 있던 탭이 잠겼다면 닫는다.
	int32 currentTabIndex = StaticCast<int32>(_tabPrev);
	if (_toggleGroupMainTab.IsBlockedIndex(currentTabIndex))
	{
		OnClickCloseAllStack();
	}
}

UGsUIGuildContentBase* UGsUIPopupGuildMainOther::GetCurrentContent() const
{
	int32 index = _switcherContent->GetActiveWidgetIndex();
	if (_contentList.IsValidIndex(index))
	{
		return _contentList[index];
	}

	return nullptr;
}
