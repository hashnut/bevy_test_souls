// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDSideTab.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Management/ScopeGlobal/GsSkillManager.h"

#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "UMG/Public/Components/CanvasPanel.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

//----------------------------------------------------------------------------------------------------------------------
bool UGsUIHUDSideTab::FGsHUDSideTab::SetToggle()
{
	bool bIsSelected = !IsSelected();
	SetIsSelected(bIsSelected);

	return bIsSelected;
}

bool UGsUIHUDSideTab::FGsHUDSideTab::IsSelected() const
{
	return (1 == _switcher->GetActiveWidgetIndex()) ? true : false;
}

void UGsUIHUDSideTab::FGsHUDSideTab::SetIsSelected(bool bIsSelected)
{
	_switcher->SetActiveWidgetIndex((bIsSelected) ? 1 : 0);
}
//----------------------------------------------------------------------------------------------------------------------
void UGsUIHUDSideTab::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_tabList.Emplace(FGsHUDSideTab(EGsHUDSideTabType::QUEST, _btnQuest, _switcherQuest));
	_tabList.Emplace(FGsHUDSideTab(EGsHUDSideTabType::SCAN, _btnScan, _switcherScan));

	_btnQuest->OnClicked.AddDynamic(this, &UGsUIHUDSideTab::OnClickQuest);
	_btnScan->OnClicked.AddDynamic(this, &UGsUIHUDSideTab::OnClickScan);
}

void UGsUIHUDSideTab::SetHUDMode(EGsUIHUDMode InMode)
{
	Super::SetHUDMode(InMode);

	// 켤 때만 OnClickSideTab 이벤트 발송을 위해 갱신을 해준다.
	if (false == IsHideMode(InMode))
	{
		SetSelectedIndex(_selectedTab);
	}
}

void UGsUIHUDSideTab::OnClickQuest()
{
	//if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::QUEST, true))
	{
		SetSelectedIndex(EGsHUDSideTabType::QUEST, true);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}


void UGsUIHUDSideTab::OnClickScan()
{
	SetSelectedIndex(EGsHUDSideTabType::SCAN, true);

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDSideTab::SetSelectedIndex(EGsHUDSideTabType InType, bool bIsToggle)
{
	for (FGsHUDSideTab& tab : _tabList)
	{
		//if (tab._tabType == EGsHUDSideTabType::QUEST)
		//{
		//	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::QUEST))
		//	{
		//		tab.SetIsSelected(false);
		//		OnClickSideTab.ExecuteIfBound(static_cast<int32>(tab._tabType), tab.IsSelected());
		//		continue;
		//	}
		//}

		if (tab._tabType == InType)
		{
			if (bIsToggle)
			{
				bool bIsSelected = tab.SetToggle();
				_selectedTab = (bIsSelected) ? InType : EGsHUDSideTabType::NONE;
			}
			else
			{
				tab.SetIsSelected(true);
				_selectedTab = InType;
			}
		}
		else
		{
			tab.SetIsSelected(false);
		}

		OnClickSideTab.ExecuteIfBound(static_cast<int32>(tab._tabType), tab.IsSelected());
	}
}

void UGsUIHUDSideTab::InvalidateAllInternal()
{

}

void UGsUIHUDSideTab::OnClickNextTab()
{
	EGsHUDSideTabType nextIndex = static_cast<EGsHUDSideTabType>(static_cast<int>(_selectedTab) + 1);
	if (EGsHUDSideTabType::MAX <= nextIndex)
	{
		return;
	}

	if (EGsHUDSideTabType::NONE < nextIndex)
	{
		SetSelectedIndex(nextIndex);
	}
}

void UGsUIHUDSideTab::OnClickPrevTab()
{
	EGsHUDSideTabType prevIndex = static_cast<EGsHUDSideTabType>(static_cast<int>(_selectedTab) - 1);
	if (EGsHUDSideTabType::NONE >= prevIndex)
	{
		return;
	}

	if (EGsHUDSideTabType::MAX > prevIndex)
	{
		SetSelectedIndex(prevIndex);
	}
}