// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionNpcShopSelectPotionIcon.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Window/NpcShop/GsUIWindowNpcShop.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "Components/PanelWidget.h"
#include "T1Project.h"


bool FGsTutorialActionNpcShopSelectPotionIcon::OnStart()
{
	if (false == Super::OnStart())
	{
		return false;
	}

	_actionUITable = _table->actionUIInteractionRow.GetRow();
	if (nullptr == _actionUITable)
	{
		return false;
	}

	const FGsSchemaTutorialTargetId* idTable = _table->actionRow.GetRowByType<FGsSchemaTutorialTargetId>();
	if (nullptr == idTable)
	{
		return false;
	}

	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return false;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->GetWidgetByKey(_actionUITable->wbpKey);
	if (false == widget.IsValid())
	{
		return false;
	}

	_window = Cast<UGsUIWindowNpcShop>(widget.Get());
	if (false == _window.IsValid())
	{
		return false;
	}

	// 주의: 갱신 타이밍 때문에 아직 ListItem을 얻어올 수 없어서, delegate로 받도록 처리
	_window->OnTutorialListItem.BindRaw(this, &FGsTutorialActionNpcShopSelectPotionIcon::OnTutorialListItem);
	// itemId: 210102001 수동 물약 아이콘
	_window->SetTutorial(static_cast<ItemId>(idTable->targetId));
	_window->SetSlotClickLock(true);

	// 혹시 계산기나 상세팝업 열려있으면 닫기 처리
	if (uiMgr->IsActiveWidget(TEXT("PopupCalculator")))
	{
		uiMgr->CloseByKeyName(TEXT("PopupCalculator"));
	}

	if (uiMgr->IsActiveWidget(TEXT("PopupItemDetail")))
	{
		uiMgr->CloseByKeyName(TEXT("PopupItemDetail"));
	}

	return true;
}

void FGsTutorialActionNpcShopSelectPotionIcon::OnEnd(bool bInSendMessage)
{
	if (_window.IsValid())
	{
		_window->SetTutorial(INVALID_SKILL_ID);
		_window->SetSlotClickLock(false);
	}

	_window = nullptr;
	_actionUITable = nullptr;

	Super::OnEnd(bInSendMessage);
}

void FGsTutorialActionNpcShopSelectPotionIcon::OnTutorialListItem(UWidget* InWidget)
{
	if (_window.IsValid())
	{
		_window->OnTutorialListItem.Unbind();
	}

	if (_actionUITable)
	{
		UWidget* iconSelectorWidget = UGsUITrayTutorial::RecursiveFindWidget(Cast<UUserWidget>(InWidget),
			FName(_actionUITable->targetRootWidget), FName(_actionUITable->targetWidget));
		if (iconSelectorWidget)
		{
			if (UPanelWidget* panel = Cast<UPanelWidget>(iconSelectorWidget))
			{
				if (UWidget* itemIconWidget = panel->GetChildAt(0))
				{
					if (_tutorialUI->SetTargetData(_table->id, _actionUITable, itemIconWidget))
					{
						return;
					}
				}
			}
		}
	}

	// 에러 상황. 바로 종료시킴
	OnEnd();
}
