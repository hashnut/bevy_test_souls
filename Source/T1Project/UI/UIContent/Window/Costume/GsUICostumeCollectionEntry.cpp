
#include "GsUICostumeCollectionEntry.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"

#include "Management/ScopeGame/GsEffectTextManager.h"
#include "GameObject/Stat/GsStatHelper.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "GsUIListCostumeCollectionData.h"
#include "GsUICostumeItem.h"

#include "TextBlock.h"
#include "WidgetSwitcher.h"
#include "Image.h"
#include "UTIL/GsTableUtil.h"

void UGsUICostumeCollectionEntry::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUICostumeCollectionEntry::OnCreateEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUICostumeCollectionEntry::OnRefreshEntry);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUICostumeCollectionEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUICostumeItem>())
		{
			child->RemoveFromParent();
		}
	}

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUICostumeCollectionEntry::OnCreateEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUICostumeCollectionEntry::OnRefreshEntry);
}

void UGsUICostumeCollectionEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UGsUIListCostumeCollectionData* item = Cast< UGsUIListCostumeCollectionData>(ListItemObject))
	{
		_genderType = item->GetGenderType();
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

			_redDot->SetVisibility(data->_isNew ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

			_textCount->SetText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { completeCount, list.Num() })));
			_textReward->SetText(FText::FromString(statExplain));
			_textRewardComplete->SetText(FText::FromString(statExplain));

			_collectedSwicher->SetActiveWidgetIndex(data->IsCompleted() ? 1 : 0);
			_collectedTxtSwitcher->SetActiveWidgetIndex(data->IsCompleted() ? 1 : 0);
		}
	}


	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

}

void UGsUICostumeCollectionEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
	if (bIsSelected && _cbSelection)
	{
		if (auto data = Cast<UGsUIListCostumeCollectionData>(GetListItem()))
			_cbSelection(data);
	}
}

void UGsUICostumeCollectionEntry::OnCreateEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeItem>(InEntry))
	{
		if (WidgetType == CostumeCollectionType::NOACTION)
			entry->WidgetType = UGsUICostumeItem::UIItemType::POPUP_COLLECTION;
		else
		{
			entry->WidgetType = UGsUICostumeItem::UIItemType::COLLECTION;
			entry->OnItemSelected.BindLambda([this](CostumeId id) {
					OnCollectionPopup.ExecuteIfBound(id);
				});
		}
	}
}

void UGsUICostumeCollectionEntry::OnRefreshEntry(int32 index, UWidget* InEntry)
{
	if (UGsUIListCostumeCollectionData* item = Cast< UGsUIListCostumeCollectionData>(GetListItem()))
	{
		if (auto data = item->GetData().Pin())
		{
			auto list = data->GetCollectionList();
			if (list.Num() > index)
			{
				auto& entrydata = list[index];
				if (auto entry = Cast<UGsUICostumeItem>(InEntry))
				{
					entry->SetCollectionData(entrydata, _genderType);
					entry->SetSelected(false);
					entry->RefreshUI();
				}
			}
		}
	}

}
