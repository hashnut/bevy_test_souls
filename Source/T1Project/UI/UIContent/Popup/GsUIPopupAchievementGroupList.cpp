// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupAchievementGroupList.h"
#include "Management/ScopeGame/GsAchievementManager.h"
#include "Achievement/GsAchievementData.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "ScrollBox.h"
#include "PanelWidget.h"
#include "UI/UIContent/Window/Achievement/GsUIAchievementItem.h"


void UGsUIPopupAchievementGroupList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	auto childList = _listScrollPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIAchievementItem>())
		{
			child->RemoveFromParent();
		}
	}
	childList.Empty();

	_itemScrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_itemScrollBoxHelper->Initialize(_listEntryWidgetClass, _listScrollPanel);
	_itemScrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIPopupAchievementGroupList::OnCreateListEntry);
	_itemScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupAchievementGroupList::OnRefreshListEntry);
}

void UGsUIPopupAchievementGroupList::BeginDestroy()
{
	if (nullptr != _itemScrollBoxHelper)
	{
		_itemScrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPopupAchievementGroupList::OnCreateListEntry);
		_itemScrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupAchievementGroupList::OnRefreshListEntry);

		_itemScrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupAchievementGroupList::InitializeData(PopupInitData* param)
{
	if (param)
	{
		_groupId = param->groupId; 
		_groupAchievements.Reset();

		TArray<TWeakPtr<FGsAchievementData>> allAchievements;
		GSAchivement()->GetGroupAchievements(param->groupId, allAchievements);

		auto index = allAchievements.IndexOfByPredicate([](auto e) { 
			return e.Pin()->AchievementProcessState() == FGsAchievementData::AchieveProcess::GoOn; });
		if (index == INDEX_NONE) _groupAchievements.Append(allAchievements);
		else
		{
			_groupAchievements.Append(allAchievements.GetData(), index + 1);
			
			auto count = index + 4;
			for (++index; index < count; index++)
			{
				if (allAchievements.IsValidIndex(index)) _groupAchievements.Add(allAchievements[index]);
				else break;
			}
		}

		_itemScrollBoxHelper->RefreshAll(_groupAchievements.Num());
	}
}

void UGsUIPopupAchievementGroupList::RefreshUI()
{
	_listView->ScrollToEnd();
}

void UGsUIPopupAchievementGroupList::OnCreateListEntry(UWidget* InEntry)
{

}

void UGsUIPopupAchievementGroupList::OnRefreshListEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIAchievementItem>(InEntry))
	{
		if (_groupAchievements.IsValidIndex(index) && _groupAchievements[index].IsValid())
		{
			entry->SetData(_groupAchievements[index]);
			entry->RefreshUI();
		}
	}
}


void UGsUIPopupAchievementGroupList::OnInputCancel()
{
	Close();
}

