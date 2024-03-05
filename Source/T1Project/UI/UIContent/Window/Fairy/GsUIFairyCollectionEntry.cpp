// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIFairyCollectionEntry.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"

#include "Management/ScopeGame/GsEffectTextManager.h"
//#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "GameObject/Stat/GsStatHelper.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "GsUIListItemFairyCollectionData.h"
#include "GsUIFairyItem.h"

#include "TextBlock.h"
#include "WidgetSwitcher.h"
#include "Image.h"
#include "UTIL/GsTableUtil.h"

void UGsUIFairyCollectionEntry::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIFairyCollectionEntry::OnCreateEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIFairyCollectionEntry::OnRefreshEntry);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIFairyCollectionEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIFairyItem>())
		{
			child->RemoveFromParent();
		}
	}

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIFairyCollectionEntry::OnCreateEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIFairyCollectionEntry::OnRefreshEntry);
}

void UGsUIFairyCollectionEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UGsUIListItemFairyCollectionData* item = Cast< UGsUIListItemFairyCollectionData>(ListItemObject))
	{
		if (auto data = item->GetData().Pin())
		{
			auto list = data->GetCollectionList();
			_scrollBoxHelper->RefreshAll(list.Num());

			auto rewards = data->GetRewardPassivity();
			_textCollectionTitle->SetText(data->_title);

			int completeCount = list.FilterByPredicate([](auto& e) { return e._complete; }).Num();
			
			FGsEffectTextCollection collectRewards;
			for (PassivityId statPassivity : rewards)
			{
				GSEffectText()->GetEffectTextColectionPassivity(statPassivity, collectRewards);
			}

			FString statExplain;
			for (TPair<StatType, int32> iter : collectRewards._statMap)
			{
				// 개행처리
				if (false == statExplain.IsEmpty())
				{
					statExplain.Append(TEXT("\n"));
				}

				FGsStatHelper::GetDesplayStatNameAndValue(iter.Key, iter.Value, statExplain);
			}
			
			_redDot->SetVisibility(data->_isNew? ESlateVisibility::Visible : ESlateVisibility::Hidden);

			_textCount->SetText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { completeCount, list.Num()})));
			_textReward->SetText(FText::FromString(statExplain));
			_textRewardComplete->SetText(FText::FromString(statExplain));

			_collectedSwicher->SetActiveWidgetIndex(data->IsCompleted() ? 1 : 0);
			_collectedTxtSwitcher->SetActiveWidgetIndex(data->IsCompleted() ? 1 : 0);
		}
	}


	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

}

void UGsUIFairyCollectionEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
	if (bIsSelected && _cbSelection)
	{
		if(auto data = Cast<UGsUIListItemFairyCollectionData>(GetListItem()))
			_cbSelection(data);
	}
}

void UGsUIFairyCollectionEntry::OnCreateEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIFairyItem>(InEntry))
	{
		if (WidgetType == FairyCollectionType::NOACTION)
			 entry->WidgetType = UGsUIFairyItem::UIItemType::POPUP_COLLECTION;
		else
		{
			entry->WidgetType = UGsUIFairyItem::UIItemType::COLLECTION;
			entry->OnItemSelected.BindLambda([this](FairyId id) {
					OnCollectionPopup.ExecuteIfBound(id);
				});
		}
	}
}

void UGsUIFairyCollectionEntry::OnRefreshEntry(int32 index, UWidget* InEntry)
{
	if (UGsUIListItemFairyCollectionData* item = Cast< UGsUIListItemFairyCollectionData>(GetListItem()))
	{
		if (auto data = item->GetData().Pin())
		{
			auto list = data->GetCollectionList();
			if (list.Num() > index)
			{
				auto& entrydata = list[index];
				if (auto entry = Cast<UGsUIFairyItem>(InEntry))
				{
					entry->SetCollectionData(entrydata);
					entry->SetSelected(false);
					entry->RefreshUI();
				}
			}
		}
	}

}
