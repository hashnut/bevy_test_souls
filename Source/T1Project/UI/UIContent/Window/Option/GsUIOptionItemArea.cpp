// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIOptionItemArea.h"
#include "GsUIOptionItemSlot.h"
#include "Option/GsSchemaOptionItem.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/GsUIWindowOption.h"

#include "UserWidget.h"
#include "PanelWidget.h"
#include "VerticalBox.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBox.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "DataSchema/Option/GsSchemaOptionItemGroup.h"


void UGsUIOptionItemArea::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 스크롤 셋팅
	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_scrollBoxHelper->Initialize(_scrollBoxEntryClass, _verticalBox);
		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIOptionItemArea::OnRefreshEntryScrollBox);
	}
}

void UGsUIOptionItemArea::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIOptionItemArea::OnRefreshEntryScrollBox);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIOptionItemArea::NativeDestruct()
{
	// 스크롤 옵셋 저장
	_scrollOffset = _scrollBox->GetScrollOffset();	
	//GOption()->SetScrollOffset(_scrollOffset);

	Super::NativeDestruct();
}


void UGsUIOptionItemArea::OnRefreshEntryScrollBox(int32 InIndex, UWidget * InEntry)
{
	if (nullptr == _itemList)
		return;

	int8 listNum = _itemList->Num();
	if (listNum > InIndex)
	{
		FGsSchemaOptionItemRow row = _itemList->GetData()[InIndex];
		const FGsSchemaOptionItem* item = row.GetRow();

		if (nullptr != item)
		{
			UGsUIOptionItemSlot* slot = Cast<UGsUIOptionItemSlot>(InEntry);
			if (nullptr != slot)
			{				
				bool isDisable = false;
				slot->Update(item->OptionSelectType, item->OptionItemType, item->OptionDisableList);
			}
		}
	}
}

void UGsUIOptionItemArea::Update(const TArray<FGsSchemaOptionItemRow>* InOptionList)
{
	TArray<FGsSchemaOptionItemRow> optionItemList;	
	for (int i = 0; i < InOptionList->Num(); i++)
	{
		FGsSchemaOptionItemRow itemRow = InOptionList->GetData()[i];
		if (const FGsSchemaOptionItem* item = itemRow.GetRow())
		{
			bool isAdd = true;
			for (auto iter : item->OptionExceptionPlatformTypeList)
			{
				if (iter == EGsOptionPlatformType::OPTION_WINDOW)
				{
#if WITH_EDITOR || PLATFORM_WINDOWS
					isAdd = false;
					continue;
#endif
				}
				else if (iter == EGsOptionPlatformType::OPTION_MOBILE_AOS)
				{
#if PLATFORM_ANDROID
					isAdd = false;
					continue;
#endif
				}
				else if (iter == EGsOptionPlatformType::OPTION_MOBILE_IOS)
				{
#if PLATFORM_IOS
					isAdd = false;
					continue;
#endif
				}
			}

			if(isAdd)
				optionItemList.Emplace(itemRow);
		}
	}
	_itemList = &optionItemList;
	_scrollBoxHelper->RefreshAll(_itemList->Num());

	_scrollBox->SetScrollOffset(GOption()->GetScrollOffset());
}

void UGsUIOptionItemArea::UpdateList(const TArray<FGsSchemaOptionItemRow>* InOptionList)
{
	_itemList = InOptionList;
	for (int i = 0; i < _itemList->Num(); ++i)
	{
		UWidget* widget = _scrollBoxHelper->GetEntry(i);
		if (widget == nullptr)
		{
			continue;
		}

		UGsUIOptionItemSlot* slot = Cast<UGsUIOptionItemSlot>(widget);
		if (slot == nullptr)
		{
			continue;
		}

		FGsSchemaOptionItemRow row = _itemList->GetData()[i];
		const FGsSchemaOptionItem* item = row.GetRow();

		if (nullptr != item)
		{	
			slot->Update(item->OptionSelectType, item->OptionItemType, item->OptionDisableList);
		}
	}	
}