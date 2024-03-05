// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupLobbyOption.h"

#include "Message/GsMessageContents.h"
#include "Management/GsMessageHolder.h"

#include "Widget.h"
#include "UserWidget.h"
#include "HorizontalBox.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Option/GsUIOptionSubArea.h"
#include "UI/UIContent/Window/Option/GsUIOptionButton.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Option/GsSchemaOptionCategory.h"
#include "Option/GsSchemaOptionDivision.h"
#include "Option/GsSchemaOptionItemGroup.h"
#include "Option/GsSchemaOptionItem.h"

#include "UI/UIContent/Window/Option/GsUIOptionButtonGroup.h"
#include "UI/UIContent/Window/Option/GsUIOptionAccountPage.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

void UGsUIPopupLobbyOption::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupLobbyOption::OnRefreshEntryScrollBox);
		_scrollBoxHelper = nullptr;
	}

	_toggleGroup.Clear();

	Super::BeginDestroy();
}

void UGsUIPopupLobbyOption::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 스크롤 셋팅
	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_scrollBoxHelper->Initialize(_scrollBoxEntryClass, _horizontalBox);
		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupLobbyOption::OnRefreshEntryScrollBox);

		_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupLobbyOption::OnClickSlot);
	}

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupLobbyOption::OnClickClose);
}

void UGsUIPopupLobbyOption::NativeConstruct()
{
	// 초기 셋팅
	_menuIndex = 0;
	// 대분류 리스트 업데이트
	UpdateList();

	Super::NativeConstruct();
}

void UGsUIPopupLobbyOption::UpdateList()
{
	_categoryList.Reset();	
	_categoryList = GOption()->GetOptionData("DTGsLobbyOptionCategory");
	int8 listNum = _categoryList.Num();
	if (0 < listNum)
	{
		_scrollBoxHelper->RefreshAll(listNum);
		_toggleGroup.Clear();
		_toggleGroup.AddToggleWidgetByParentPanel(_horizontalBox);
		_toggleGroup.SetSelectedIndex(_menuIndex, true);
	}
}

void UGsUIPopupLobbyOption::CloseInternal()
{
	// 옵션 저장
	if (UGsOptionManager* optionMgr = GOption())
	{
		optionMgr->SaveAll();
	}

	Super::CloseInternal();
}

void UGsUIPopupLobbyOption::OnRefreshEntryScrollBox(int32 InIndex, UWidget* InEntry)
{
	int8 listNum = _categoryList.Num();

	if (listNum > InIndex)
	{
		UGsUIOptionButtonGroup* slot = Cast<UGsUIOptionButtonGroup>(InEntry);
		if (slot)
		{
			const FGsSchemaOptionCategory* category = _categoryList[InIndex];
			if (nullptr != category)
			{
				if (_menuIndex == InIndex)
				{
					if (nullptr != _subArea)
					{
						_subArea->Update(category->IsDivision, &category->DivisionList);
					}
				}
				slot->SetData(category->nameText);
			}
		}
	}
}

void UGsUIPopupLobbyOption::OnClickSlot(int32 InIndex)
{
	_menuIndex = InIndex;

	int8 listNum = _categoryList.Num();

	if (listNum >= InIndex)
	{
		const FGsSchemaOptionCategory* category = _categoryList[InIndex];
		if (nullptr != category)
		{
			if (category->isPopup)
			{
				_switcher->SetActiveWidgetIndex(static_cast<int>(category->popupType));
				switch (category->popupType)
				{
				case EGsOptionPopupType::ACCOUNT:
					if (nullptr != _accountPage)
					{
						_accountPage->SetData();
					}
					break;
				default:
					break;
				}
			}
			else
			{
				_switcher->SetActiveWidgetIndex(static_cast<int>(EGsOptionPopupType::NONE));
				if (nullptr != _subArea)
				{
					_subArea->Update(category->IsDivision, &category->DivisionList);
				}
			}
		}
	}
}

void UGsUIPopupLobbyOption::OnClickClose()
{
	Close();
}

void UGsUIPopupLobbyOption::OnInputCancel()
{
	OnClickClose();
}