// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Quest/Sub/GsUIWindowQuestSubEntry.h"

#include "GsSchemaEnums.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

#include "UI/UIContent/Window/Quest/Item/GsUIWindowQuestItemBase.h"
#include "WidgetSwitcher.h"

#include "Quest/Table/GsQuestSubTableCacheSet.h"

#include "GsUIWindowSubAreaEntry.h"
#include "GsUIWindowSubNpcEntry.h"
#include "GsUIWindowSubStoryEntry.h"
#include "GsUIWindowQuestSubItem.h"


void UGsUIWindowQuestSubEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	_item = Cast<UGsUIWindowQuestSubItem>(ListItemObject);
	if (_item.IsValid())
	{
		_mapId = _item->GetMapId();
		_slotIndex = _item->GetSlotIndex();
		_npcId = _item->GetNpcId();
		//_isMap = _item->GetIsMap();
		_subItemType = _item->GetQuestSubItemType();
	}

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UGsUIWindowQuestSubEntry::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIWindowQuestSubEntry::OnStoryClickFunc(UGsUIWindowSubNPCListItem* inItem)
{
	/*if (nullptr == inItem)
		return;

	if (nullptr != _selectItem)
	{
		if (_selectItem != inItem)
		{
			_selectItem->SetSelect(false);
		}
	}
	
	_selectItem = inItem;
	_selectItem->SetSelect(true);
	SetSelect(true);
	_selectStoryId = _selectItem->GetStoryId();

	if (_item.IsValid())
	{
		if (_item->_delegateClickStorySlot.IsBound())
		{
			_item->_delegateClickStorySlot.ExecuteIfBound(this);
		}
	}*/
}

void UGsUIWindowQuestSubEntry::Refresh()
{
	switch (_subItemType)
	{
	case QuestSubItemType::SUB_AREA:
		_areaEntry->Refresh();
		break;
	case QuestSubItemType::SUB_NPC:
		_npcEntry->Refresh();
		break;
	case QuestSubItemType::SUB_STORY:
		_storyEntry->Refresh();
		break;
	default:
		break;
	}
}

void UGsUIWindowQuestSubEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{	
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}

void UGsUIWindowQuestSubEntry::SetDataMap(int inSlotIndex, int32 inMapId)
{
	_switcherEntry->SetActiveWidgetIndex(0);

	if (nullptr != _areaEntry)
	{
		_areaEntry->SetData(inSlotIndex, inMapId);
	}
}

void UGsUIWindowQuestSubEntry::SetDataNpc(int32 inMapId, int32 inNpcId)
{
	_switcherEntry->SetActiveWidgetIndex(1);

	if (nullptr != _areaEntry)
	{
		_npcEntry->SetData(inNpcId);
	}
}

void UGsUIWindowQuestSubEntry::SetDataStory(int32 inMapId, int32 inNpcId, QuestContentsType inContentsType, TSharedPtr<FGsQuestSubStoryTableCache> inSubStoryTableCache)
{
	_switcherEntry->SetActiveWidgetIndex(2);

	if (nullptr != _areaEntry)
	{
		_storyEntry->SetData(inNpcId, inContentsType, inSubStoryTableCache);
	}
}

void UGsUIWindowQuestSubEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
	_switcherSelectImg->SetActiveWidgetIndex((_bIsSelected) ? 0 : 1);
}

void UGsUIWindowQuestSubEntry::SetSelect(bool inIsSelect)
{
	switch (_subItemType)
	{
	case QuestSubItemType::SUB_AREA:
		_areaEntry->SetSelect(inIsSelect);
		break;
	case QuestSubItemType::SUB_NPC:
		_npcEntry->SetSelect(inIsSelect);
		break;
	case QuestSubItemType::SUB_STORY:
		_storyEntry->SetSelect(inIsSelect);
		break;
	default:
		break;
	}
}

void UGsUIWindowQuestSubEntry::SetSelectParents(bool inIsSelect)
{
	/*if (nullptr != _verticalBox)
	{
		_verticalBox->SetVisibility((inIsSelect) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}*/
}


StoryId UGsUIWindowQuestSubEntry::GetStoryId()
{
	if (QuestSubItemType::SUB_STORY == _subItemType)
	{
		return _storyEntry->GetStoryId();
	}
	else if (QuestSubItemType::SUB_NPC == _subItemType)
	{
		_item->GetStoryId();
	}

	return INVALID_STORY_ID;
}