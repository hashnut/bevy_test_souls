// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowOption.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "Management/ScopeGlobal/EGsOptionPopupType.h"

#include "Message/GsMessageContents.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Widget.h"
#include "UserWidget.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Option/GsUIOptionSubArea.h"
#include "UI/UIContent/Window/Option/GsUIOptionButton.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Option/GsSchemaOptionCategory.h"
#include "Option/GsSchemaOptionDivision.h"
#include "Option/GsSchemaOptionItemGroup.h"
#include "Option/GsSchemaOptionItem.h"
#include "HorizontalBox.h"
#include "Option/GsUIOptionButtonGroup.h"
#include "Option/GsUIOptionAccountPage.h"
#include "UMG/Public/Components/WidgetSwitcher.h"


void UGsUIWindowOption::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowOption::OnRefreshEntryScrollBox);
		_scrollBoxHelper = nullptr;
	}

	_toggleGroup.Clear();

	Super::BeginDestroy();
}

void UGsUIWindowOption::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 스크롤 셋팅
	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_scrollBoxHelper->Initialize(_scrollBoxEntryClass, _horizontalBox);
		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowOption::OnRefreshEntryScrollBox);

		_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIWindowOption::OnClickSlot);		
	}

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowOption::OnClickCloseAllStack);
}

void UGsUIWindowOption::NativeConstruct()
{	
	// 초기 셋팅
	_menuIndex = GOption()->GetOptionWindowMenuIndex();
	// 대분류 리스트 업데이트
	UpdateList();

	Super::NativeConstruct();

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->AddUIScaleFlag(EGsUIScaleFlags::UI_SCALE_OTHER_STATE);
	}
}

void UGsUIWindowOption::NativeDestruct()
{
	// 옵션 저장
	if (UGsOptionManager* optionMgr = GOption())
	{
		optionMgr->SaveAll();
	}

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->RemoveUIScaleFlag(EGsUIScaleFlags::UI_SCALE_OTHER_STATE);
	}

	Super::NativeDestruct();
}

void UGsUIWindowOption::UpdateList()
{
	_categoryList.Reset();

	TArray<const FGsSchemaOptionCategory*> categoryArray;
	categoryArray = GOption()->GetOptionData(TEXT(""));
	
	if (false == GOption()->GetIsDevelopmentMode())
	{
		for (const FGsSchemaOptionCategory* category : categoryArray)
		{
			if (false == category->isDevelopment)
				_categoryList.Emplace(category);
		}
	}
	else
	{
		_categoryList.Append(categoryArray);
	}

	int8 listNum = _categoryList.Num();
	if (0 < listNum)
	{
		_scrollBoxHelper->RefreshAll(listNum);
		_toggleGroup.Clear();
		_toggleGroup.AddToggleWidgetByParentPanel(_horizontalBox);
		_toggleGroup.SetSelectedIndex(_menuIndex, true);
	}

	//UpdateSubArea(_menuIndex);
}

void UGsUIWindowOption::CloseInternal()
{
	Super::CloseInternal();
}

bool UGsUIWindowOption::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsOption))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_OPTION);
		return true;
	}

	return false;
}

void UGsUIWindowOption::OnRefreshEntryScrollBox(int32 InIndex, UWidget * InEntry)
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
				/*if (_menuIndex == InIndex)
				{
					if (nullptr != _subArea)
					{
						_subArea->Update(category->IsDivision, &category->DivisionList);
					}
				}*/
				slot->SetData(category->nameText);
			}
		}
	}
}

void UGsUIWindowOption::OnClickSlot(int32 InIndex)
{
	_menuIndex = InIndex;
	//GOption()->SetOptionWindowMenuIndex(_menuIndex);

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

void UGsUIWindowOption::UpdateSubArea(int32 InIndex)
{
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