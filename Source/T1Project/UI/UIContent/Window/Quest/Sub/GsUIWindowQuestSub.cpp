// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Quest/Sub/GsUIWindowQuestSub.h"
#include "UI/UIContent/Window/Quest/Tab/GsUIWindowQuestTabBase.h"

#include "Text.h"
#include "Components/ListView.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

#include "SharedInclude/SharedTypes.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "DataSchema/Quest/Sub/GsSchemaQuestSubChapter.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Quest/Icon/GsSchemaQuestImg.h"
#include "DataSchema/Map/Spawn/GsSchemaClientSpawnData.h"

#include "UI/UIContent/Window/Quest/Item/GsUIWindowQuestItemBase.h"
#include "UI/UIContent/Common/Reward/GsUIRewardInfoBase.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Quest/Table/GsQuestSubTableCacheSet.h"
#include "Quest/GsQuestData.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Quest/GsQuestInstance.h"

#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "GameObject/Quest/GsQuestHandler.h"

#include "Classes/GsPoolUObject.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "GsUIWindowQuestSubEntry.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "GsUIWindowQuestSubItem.h"
#include "UMG/Public/Components/TreeView.h"
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "Quest/Table/GsQuestTableCache.h"


void UGsUIWindowQuestSub::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_btnGiveUp->OnClicked.AddDynamic(this, &UGsUIWindowQuestSub::OnClickGiveUp);
	_btnTeleport->OnClicked.AddDynamic(this, &UGsUIWindowQuestSub::OnClickTeleport);
	_btnAutoMove->OnClicked.AddDynamic(this, &UGsUIWindowQuestSub::OnClickAutoMove);
	_btnReward->OnClicked.AddDynamic(this, &UGsUIWindowQuestSub::OnClickReward);

	_treeView->OnEntryWidgetGenerated().AddUObject(this, &UGsUIWindowQuestSub::OnEntryWidgetGenerated);
	_treeView->OnEntryWidgetReleased().AddUObject(this, &UGsUIWindowQuestSub::OnEntryWidgetReleased);
	_treeView->OnItemClicked().AddUObject(this, &UGsUIWindowQuestSub::OnClickedListViewItem);
	_treeView->OnItemSelectionChanged().AddUObject(this, &UGsUIWindowQuestSub::OnItemSelectionChanged);
}

void UGsUIWindowQuestSub::BeginDestroy()
{
	FreeTreeItem();
	Super::BeginDestroy();
}

void UGsUIWindowQuestSub::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIWindowQuestSub::NativeDestruct()
{
	ClearData();
	Super::NativeDestruct();
}

void UGsUIWindowQuestSub::ClearData()
{
	_mapSlotIndex = -1;
	_npcSlotIndex = -1;
	_mapId = 0;
	_npcId = 0;
	_selectStoryId = 0;
	_selectQuestId = 0;
	_isAutoCloseSlot = false;

	_selectedNpcEntry.Reset();
}

bool UGsUIWindowQuestSub::IsDungeonMap(IN int32 inDungeonGroupId)
{
	int32 key = GLevel()->GetCurrentLevelId();

	TArray<const FGsSchemaDungeonGroupData*> dungeonGroupDataList;
	if (const UGsTable* table = FGsSchemaDungeonGroupData::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaDungeonGroupData>(dungeonGroupDataList);
	}

	for (const FGsSchemaDungeonGroupData* dungeonGroupData : dungeonGroupDataList)
	{
		if (nullptr == dungeonGroupData)
			continue;

		if (dungeonGroupData->id != inDungeonGroupId)
			continue;

		for (auto& iterDungeon : dungeonGroupData->dungeonList)
		{
			const FGsSchemaDungeonData* schemaDungeonData = iterDungeon.GetRow();
			if (nullptr == schemaDungeonData)
				continue;

			const FGsSchemaMapData* schemaMapData = schemaDungeonData->mapId.GetRow();
			if (nullptr == schemaMapData)
				continue;
			
			if (schemaMapData->id == key)
			{
				return true;
			}
		}		
	}

	return false;
}

bool UGsUIWindowQuestSub::GetIsMyMapId(int32 mapId)
{
	int32 key = GLevel()->GetCurrentLevelId();

	const FGsSchemaMapData* mapData =
		UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(mapId);
	if (nullptr == mapData)
	{
		if (IsDungeonMap(mapId))
		{
			return key == mapId ? true : false;
		}
	}
	else
	{
		return mapData->id == mapId ? true : false;
	}

	return false;
}

void UGsUIWindowQuestSub::OnEntryWidgetGenerated(UUserWidget& userWidget)
{
	UGsUIWindowQuestSubEntry* entry = Cast<UGsUIWindowQuestSubEntry>(&userWidget);
	if (nullptr != entry)
	{
		TWeakObjectPtr<class UGsUIWindowQuestSubItem> itemPtr = entry->GetItem();
		if (itemPtr.IsValid())
		{
			QuestSubItemType itemType = itemPtr->GetQuestSubItemType();
			int32 mapId = itemPtr->GetMapId();
			int slotIndex = itemPtr->GetSlotIndex();
			int32 npcId = itemPtr->GetNpcId();

			QuestContentsType contentsType = itemPtr->GetQuestContentsType();
			TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& npcStoryList 
												= itemPtr->GetNpcStoryList();

			TSharedPtr<FGsQuestSubStoryTableCache> subStoryTableCache = itemPtr->GetSubStoryTableCache();

			if (itemType == QuestSubItemType::SUB_AREA)
			{
				_treeView->SetItemExpansion(itemPtr.Get(), slotIndex == _mapSlotIndex ? true : false);

				bool isSelectedMap = (slotIndex == _mapSlotIndex) ? true : false;
				entry->SetDataMap(slotIndex, mapId);
				entry->SetSelect(isSelectedMap);
				
				_mapId = (isSelectedMap)? mapId : _mapId;
			}
			else if (itemType == QuestSubItemType::SUB_NPC)
			{
				_treeView->SetItemExpansion(itemPtr.Get(), true);

				bool isSelectedNpc = (_mapId == mapId && slotIndex == _npcSlotIndex ) ? true : false;
				entry->SetDataNpc(mapId, npcId);
				entry->SetSelect(isSelectedNpc);

				if (isSelectedNpc)
				{
					_npcId = npcId;
					OnClickedListEntry(Cast<UObject>(itemPtr));
				}
			}
			else
			{
				bool isSelectedStory = (_npcId == npcId && slotIndex == _storyIndex) ? true : false;
				entry->SetDataStory(mapId, npcId, contentsType, subStoryTableCache);
				entry->SetSelect(isSelectedStory);

				if (isSelectedStory)
				{
					OnClickedListEntry(Cast<UObject>(itemPtr));
				}
			}
		}
	}
}

void UGsUIWindowQuestSub::ClearIndex()
{
	SetMapIndex(INVALID_SLOT_INDEX);
	SetNpcIndex(INVALID_SLOT_INDEX);
	SetStoryIndex(INVALID_SLOT_INDEX);
}

void UGsUIWindowQuestSub::SetMyMapIndex()
{
	int myMapIndex = GetMyMapIndex();

	if (myMapIndex != _mapSlotIndex)
	{
		UObject* selectObject = _treeView->GetItemAt(myMapIndex);
		UGsUIWindowQuestSubItem* item = Cast<UGsUIWindowQuestSubItem>(selectObject);
		if (nullptr != item)
		{
			_treeView->SetItemExpansion(item, true);
			_treeView->SetScrollOffset(myMapIndex);
		}

		SetMapIndex(myMapIndex);
		SetNpcIndex(0);
		SetStoryIndex(0);

		SetCurrentMapScrollOffset(myMapIndex);
		//OnClickedListEntry(_treeView->GetItemAt(_mapSlotIndex));		
	}	
}

void UGsUIWindowQuestSub::SetMapIndex(int32 inMapIndex)
{
	if (_mapSlotIndex == inMapIndex)
	{
		// 내가 나를 눌렀다.
		ClearIndex();
		return;
	}

	if (0 < _mapSlotIndex)
	{
		UObject* selectObject = _treeView->GetItemAt(_mapSlotIndex);
		UGsUIWindowQuestSubItem* selectItem = Cast<UGsUIWindowQuestSubItem>(selectObject);
		if (nullptr != selectItem)
		{
			_treeView->SetItemExpansion(selectItem, false);

			if (UUserWidget* widget = _treeView->GetEntryWidgetFromItem(selectItem))
			{
				TWeakObjectPtr<UGsUIWindowQuestSubEntry> entry = Cast<UGsUIWindowQuestSubEntry>(widget);
				if (entry.IsValid())
				{
					entry->SetSelect(false);
				}
			}			
		}
	}

	_mapSlotIndex = inMapIndex;
}

void UGsUIWindowQuestSub::SetNpcIndex(int32 inNpcIndex)
{
	_npcSlotIndex = inNpcIndex;
	SetStoryIndex(0);
}

void UGsUIWindowQuestSub::SetStoryIndex(int32 inStroyIndex)
{
	_storyIndex = inStroyIndex;	
}

void UGsUIWindowQuestSub::OnEntryWidgetReleased(UUserWidget& userWidget)
{
	UGsUIWindowQuestSubEntry* entry = Cast<UGsUIWindowQuestSubEntry>(&userWidget);
	if (nullptr != entry)
	{
		entry->SetSelect(false);

		if (_isAutoCloseSlot)
		{
			entry->SetSelectParents(false);
		}
	}
}

void UGsUIWindowQuestSub::CloseArea()
{
	if (0 < _mapSlotIndex)
	{
		UObject* selectObject = _treeView->GetItemAt(_mapSlotIndex);
		UGsUIWindowQuestSubItem* selectItem = Cast<UGsUIWindowQuestSubItem>(selectObject);
		if (nullptr != selectItem)
		{
			_treeView->SetItemExpansion(selectItem, false);
		}
	}
}

void UGsUIWindowQuestSub::OnClickedArea(UGsUIWindowQuestSubItem* inItem)
{
	if (nullptr == inItem)
		return;

	UUserWidget* widget = _treeView->GetEntryWidgetFromItem(inItem);
	if (nullptr == widget)
		return;

	TWeakObjectPtr<UGsUIWindowQuestSubEntry> entry = Cast<UGsUIWindowQuestSubEntry>(widget);
	if (!entry.IsValid())
		return;

	int32 slotIndex = inItem->GetSlotIndex();

	if (_mapSlotIndex == slotIndex)
	{
		_mapSlotIndex = -1;
		_npcSlotIndex = -1;		
		_storyIndex = -1;

		entry->SetSelect(false);
		return;
	}

	CloseArea();

	entry->SetSelect(true);
	_mapSlotIndex = slotIndex;
	_mapId = inItem->GetMapId();
	_npcSlotIndex = 0;
	_storyIndex = 0;
}

void UGsUIWindowQuestSub::OnClickedNpc(UGsUIWindowQuestSubItem* inItem)
{
	if (nullptr == inItem)
		return;

	UUserWidget* widget = _treeView->GetEntryWidgetFromItem(inItem);
	if (nullptr == widget)
		return;

	TWeakObjectPtr<UGsUIWindowQuestSubEntry> entry = Cast<UGsUIWindowQuestSubEntry>(widget);
	if (!entry.IsValid())
		return;

	// 다른 지역의 npc를 눌렀다.
	if (_mapSlotIndex != inItem->GetParentsSlotIndex())
	{
		// 무조건 지역은 하나만 열리므로
		// 이런경우는 올수없다.
	}

	_treeView->SetItemExpansion(inItem, true);

	if (nullptr != _selectedNpcEntry && _selectedNpcEntry != entry)
	{
		// 기존에 열려 있던 npc를 안눌림 처리하고
		_selectedNpcEntry->SetSelect(false);
		UpdateChildList(_selectedNpcEntry->GetItem(), false);
	}

	_npcSlotIndex = inItem->GetSlotIndex();	
	_selectedNpcEntry = entry;
	// 새로운거 눌림 처리한다.
	_selectedNpcEntry->SetSelect(true);
	// 그 자식인 스토리 처리도 한다.
	UpdateChildList(_selectedNpcEntry->GetItem(), true);
}

void UGsUIWindowQuestSub::CloseAreaSlot()
{
	UObject* selectObject = _treeView->GetItemAt(_mapSlotIndex);
	UGsUIWindowQuestSubItem* selectItem = Cast<UGsUIWindowQuestSubItem>(selectObject);
	if (nullptr != selectItem)
	{
		TArray<UGsUIWindowQuestSubItem*> childList = selectItem->GetChildList();

		for (UGsUIWindowQuestSubItem* childItem : childList)
		{
			if (nullptr == childItem)
				continue;

			if (_npcSlotIndex == childItem->GetSlotIndex())
			{
				UUserWidget* childWidget = _treeView->GetEntryWidgetFromItem(childItem);
				if (nullptr == childWidget)
					return;

				TWeakObjectPtr<UGsUIWindowQuestSubEntry> childEntry = Cast<UGsUIWindowQuestSubEntry>(childWidget);
				if (!childEntry.IsValid())
					continue;

				childEntry->SetSelect(false);
				UpdateChildList(childItem, false);

				break;
			}
		}
	}
}

void UGsUIWindowQuestSub::SelectNpcSlot(int32 inNpcIndex)
{
	UObject* selectObject = _treeView->GetItemAt(_mapSlotIndex);
	UGsUIWindowQuestSubItem* selectItem = Cast<UGsUIWindowQuestSubItem>(selectObject);
	if (nullptr != selectItem)
	{
		TArray<UGsUIWindowQuestSubItem*> childList = selectItem->GetChildList();

		for (UGsUIWindowQuestSubItem* childItem : childList)
		{
			if (nullptr == childItem)
				continue;

			if (childItem->GetSlotIndex() == inNpcIndex)
			{
				UUserWidget* childWidget = _treeView->GetEntryWidgetFromItem(childItem);
				if (nullptr == childWidget)
					return;

				TWeakObjectPtr<UGsUIWindowQuestSubEntry> childEntry = Cast<UGsUIWindowQuestSubEntry>(childWidget);
				if (!childEntry.IsValid())
					continue;

				childEntry->SetSelect(true);

				_npcSlotIndex = childItem->GetSlotIndex();
				_selectedNpcEntry = childEntry;

				break;
			}
		}
	}
}

void UGsUIWindowQuestSub::OnClickedStory(UGsUIWindowQuestSubItem* inItem)
{
	if (nullptr == inItem)
		return;

	UUserWidget* widget = _treeView->GetEntryWidgetFromItem(inItem);
	if (nullptr == widget)
		return;

	TWeakObjectPtr<UGsUIWindowQuestSubEntry> entry = Cast<UGsUIWindowQuestSubEntry>(widget);
	if (!entry.IsValid())
		return;

	// 다른 npc를 눌렀다.
	if (_npcSlotIndex != inItem->GetParentsSlotIndex())
	{
		CloseAreaSlot();
	}
	else
	{
		if (_storyIndex != inItem->GetSlotIndex())
		{
			_selectedStoryEntry->SetSelect(false);
		}
	}

	if (_npcSlotIndex != inItem->GetParentsSlotIndex())
	{
		SelectNpcSlot(inItem->GetParentsSlotIndex());
	}

	_npcId = entry->GetNpcId();
	_selectStoryId = entry->GetStoryId();
	_storyIndex = inItem->GetSlotIndex();

	_selectedStoryEntry = entry;
	_selectedStoryEntry->SetSelect(true);

	SetData();
}

void UGsUIWindowQuestSub::OnClickedListViewItem(UObject* InListItem)
{
	OnClickedListEntry(InListItem);

	// ListView는 버튼이 안먹기 때문에 별도 사운드 부착 
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->PlayPressSound();
	}
}

void UGsUIWindowQuestSub::OnItemSelectionChanged(UObject* InListItem)
{
	UGsUIWindowQuestSubItem* item = Cast<UGsUIWindowQuestSubItem>(InListItem);
	if (nullptr == item)
		return;

	QuestSubItemType itemType = item->GetQuestSubItemType();
	int slotIndex = item->GetSlotIndex();

	if (itemType == QuestSubItemType::SUB_AREA)
	{
		if (slotIndex != _mapSlotIndex)
		{
			UObject* preItem = _treeView->GetItemAt(_mapSlotIndex);
			if (nullptr == preItem)
				return;
			
			UGsUIWindowQuestSubItem* subItem = Cast<UGsUIWindowQuestSubItem>(preItem);
			if (nullptr == subItem)
				return;

			UUserWidget* subWidget = _treeView->GetEntryWidgetFromItem(subItem);
			if (nullptr == subWidget)
				return;

			TWeakObjectPtr<UGsUIWindowQuestSubEntry> entry = Cast<UGsUIWindowQuestSubEntry>(subWidget);
			if (!entry.IsValid())
				return;

			entry->SetSelect(false);
		}
	}
}

void UGsUIWindowQuestSub::OnClickedListEntry(UObject* InListItem)
{
	UGsUIWindowQuestSubItem* item = Cast<UGsUIWindowQuestSubItem>(InListItem);
	if (nullptr == item)
		return;

	UUserWidget* widget = _treeView->GetEntryWidgetFromItem(item);
	if (nullptr == widget)
		return;

	TWeakObjectPtr<UGsUIWindowQuestSubEntry> entry = Cast<UGsUIWindowQuestSubEntry>(widget);
	if (!entry.IsValid())
		return;

	QuestSubItemType itemType = item->GetQuestSubItemType();
	if (itemType == QuestSubItemType::SUB_AREA)
	{
		OnClickedArea(item);
	}
	else if (itemType == QuestSubItemType::SUB_NPC)
	{
		OnClickedNpc(item);
	}
	else
	{
		OnClickedStory(item);
	}

	// 하위탭 모두 닫거나 처리 후 스크롤 셋팅
	if (itemType == QuestSubItemType::SUB_AREA)
	{
		SetCurrentMapScrollOffset(_mapSlotIndex);
	}
}

void UGsUIWindowQuestSub::UpdateChildList(TWeakObjectPtr<class UGsUIWindowQuestSubItem> inSubItem, bool isVisible, int32 inIndex)
{
	int index = 0;
	TArray<UGsUIWindowQuestSubItem*> childList = inSubItem->GetChildList();
	for (UGsUIWindowQuestSubItem* childItem : childList)
	{
		if (nullptr == childItem)
			continue;

		UUserWidget* childWidget = _treeView->GetEntryWidgetFromItem(childItem);
		if (nullptr == childWidget)
			return;

		TWeakObjectPtr<UGsUIWindowQuestSubEntry> childEntry = Cast<UGsUIWindowQuestSubEntry>(childWidget);
		if (!childEntry.IsValid())
			continue;

		if (isVisible)
		{
			if (index == inIndex)
			{
				OnClickedListEntry(childItem);
				return;
			}
		}
		else
		{
			childEntry->SetSelect(false);
		}

		++index;
	}
}

void UGsUIWindowQuestSub::RefreshAll()
{	
	const TArray<UObject*>& itemList = _treeView->GetListItems();
	for (UObject* obj : itemList)
	{		
		UGsUIWindowQuestSubItem* item = Cast<UGsUIWindowQuestSubItem>(obj);
		if (nullptr == item)
			continue;

		RefreshChild(item);			
	}
	SetData();
}

void UGsUIWindowQuestSub::RefreshChild(UGsUIWindowQuestSubItem* inItem)
{	
	TArray<UGsUIWindowQuestSubItem*> childList = inItem->GetChildList();
	for (UGsUIWindowQuestSubItem* childItem : childList)
	{
		RefreshChild(childItem);

		UUserWidget* childWidget = _treeView->GetEntryWidgetFromItem(childItem);
		if (nullptr != childWidget)
		{
			TWeakObjectPtr<UGsUIWindowQuestSubEntry> childEntry = Cast<UGsUIWindowQuestSubEntry>(childWidget);
			if (!childEntry.IsValid())
				continue;

			childEntry->Refresh();
		}
	}

	UUserWidget* widget = _treeView->GetEntryWidgetFromItem(inItem);
	if (nullptr != widget)
	{
		TWeakObjectPtr<UGsUIWindowQuestSubEntry> entry = Cast<UGsUIWindowQuestSubEntry>(widget);

		if (!entry.IsValid())
			return;

		entry->Refresh();
	}
}

void UGsUIWindowQuestSub::UpdateList()
{
	// 반납
	ReleaseAllTreeItem();

	Super::UpdateList();

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	FGsQuestSubTableCacheSet* questSubData = questManager->GetQuestSubTableCache();
	if (nullptr == questSubData)
		return;
	
	_treeView->ClearListItems();
	_treeRootSlotList.Reset();

	int32 key = GLevel()->GetCurrentLevelId();

	TMap<int32, TArray<TSharedPtr<FGsQuestSubChapterTableCache>>>& mapMap =
		questSubData->GetChapterAreaTableCacheList();

	int slotIndex = 0;	
	for (auto& iterMap : mapMap)
	{
		UGsUIWindowQuestSubItem* mapItem = CreateTreeItem();
		if (nullptr == mapItem)
			continue;

		int32 mapId = iterMap.Key;

		mapItem->SetMapItem(slotIndex, mapId);

		int npcSlotIndex = 0;
		TArray<TSharedPtr<FGsQuestSubChapterTableCache>>& npcList = iterMap.Value;
		for (auto& iterNpc : npcList)
		{
			TSharedPtr<FGsQuestSubChapterTableCache>& npcTableCache = iterNpc;
			if (!npcTableCache.IsValid())
				continue;

			if (false == npcTableCache->IsExposeQuest())
				continue;

			StoryId firstStoryId = npcTableCache->GetFirstStroyId();

			UGsUIWindowQuestSubItem* npcItem = CreateTreeItem();
			if (nullptr == npcItem)
				continue;

			const FGsSchemaQuestSubChapter* schemaQuestChapter = npcTableCache->GetSchemaQuestChapter();
			if (nullptr == schemaQuestChapter)
				continue;

			const FGsSchemaNpcData* npcData = schemaQuestChapter->npcId.GetRow();
			if (nullptr == npcData)
				continue;

			QuestContentsType contentsType = schemaQuestChapter->questContentsType;
			npcItem->SetNpcItem(slotIndex, npcSlotIndex, mapId, npcData->id, firstStoryId, contentsType, npcTableCache->GetStoryTableCacheList());
	
			int storySlotIndex = 0;
			for (TSharedPtr<FGsQuestSubStoryTableCache> storyTableCache : npcTableCache->GetStoryTableCacheList())
			{
				if (!storyTableCache.IsValid())
					continue;

				if (false == storyTableCache->IsExposeQuest())
					continue;

				UGsUIWindowQuestSubItem* storyItem = CreateTreeItem();
				if (nullptr == storyItem)
					continue;

				storyItem->SetStoryItem(npcSlotIndex, storySlotIndex, mapId, npcData->id, schemaQuestChapter->questContentsType, storyTableCache);
				npcItem->AddChild(storyItem);
				++storySlotIndex;
			}

			if (0 < storySlotIndex)
			{
				mapItem->AddChild(npcItem);
				++npcSlotIndex;
			}
		}

		if (0 < npcSlotIndex)
		{
			_treeRootSlotList.Add(mapItem);
			_treeView->SetOnGetItemChildren<UGsUIWindowQuestSubItem>(mapItem, &UGsUIWindowQuestSubItem::OnGetChildren);
			++slotIndex;
		}
	}

	_treeView->SetListItems(_treeRootSlotList);

	int myMapIndex = GetMyMapIndex();
	UObject* selectObject = _treeView->GetItemAt(myMapIndex);
	UGsUIWindowQuestSubItem* item = Cast<UGsUIWindowQuestSubItem>(selectObject);
	if (nullptr != item)
	{
		_treeView->SetItemExpansion(item, true);
		_treeView->SetScrollOffset(myMapIndex);
	}
}

void UGsUIWindowQuestSub::RefreshQuestObjective()
{
	SetData();
}

void UGsUIWindowQuestSub::OnClickTab(bool isOnTab)
{
	if (isOnTab == false)
	{
		int myMapIndex = GetMyMapIndex();

		if (myMapIndex != _mapSlotIndex)
		{
			UObject* selectObject = _treeView->GetItemAt(_mapSlotIndex);
			UGsUIWindowQuestSubItem* item = Cast<UGsUIWindowQuestSubItem>(selectObject);
			if (nullptr != item)
			{
				_treeView->SetItemExpansion(item, false);
				if (UUserWidget* widget = _treeView->GetEntryWidgetFromItem(item))
				{
					TWeakObjectPtr<UGsUIWindowQuestSubEntry> entry = Cast<UGsUIWindowQuestSubEntry>(widget);
					if (entry.IsValid())
					{
						entry->SetSelect(false);
					}
				}
			}
		}
		

		/*UObject* item = _treeView->GetItemAt(_mapSlotIndex);
		if (nullptr == item)
			return;

		if (UGsUIWindowQuestSubItem* subItem = Cast<UGsUIWindowQuestSubItem>(item))
		{
			_treeView->SetScrollOffset(_mapSlotIndex);
			_treeView->SetItemExpansion(subItem, false);
		}*/
	}
	else
	{
		SetMyMapIndex();
	}
}

void UGsUIWindowQuestSub::SetSelectCurrentMapSlot(int inSlotIndex)
{
	SetCurrentMapScrollOffset(inSlotIndex);
	UObject* selectObject = _treeView->GetItemAt(inSlotIndex);
	UGsUIWindowQuestSubItem* item = Cast<UGsUIWindowQuestSubItem>(selectObject);
	if (nullptr != item)
	{
		_treeView->SetItemExpansion(item, true);
	}
}

void UGsUIWindowQuestSub::SetCurrentMapScrollOffset(int inSlotIndex)
{	
	_treeView->SetScrollOffset(inSlotIndex);
}

int UGsUIWindowQuestSub::GetMyMapIndex()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return 0;
	
	return questManager->GetSubMyMapIndex();
}

void UGsUIWindowQuestSub::OnClickTeleport()
{
	if (_slotStateType == QuestWindowUISlotType::STORY_WATING)
	{
		FGsQuestTeleportParam param(_selectQuestId, _selectStoryId, 0, _npcId);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_NPC_TO_TELEPORT, &param);
	}
	else
	{
		FGsQuestTeleportParam param(0, _selectStoryId, 0);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_TELEPORT, &param);
	}

	GSQuest()->ClearAutoProgressQuestKey();
	OnClickClose();
}

void UGsUIWindowQuestSub::OnClickAutoMove()
{
	if (_slotStateType == QuestWindowUISlotType::STORY_WATING)
	{
		FGsAIQuestAutoMoveMessageParam param;
		param._questId = _selectQuestId;
		param._moveType = QuestObjectiveType::INTERACT_NPC;
		param._mapId = _npcPos.mapId;
		param._range = _npcInteractionRange;
		param._npcId = _npcId;

		if (FGsClientSpawnManager* spawnManager = GSClientSpawn())
		{
			FGsSchemaSpotInfo frontSpotData;
			if (true == spawnManager->TryGetSpot(_npcPos.mapId, _npcPos.spotId, frontSpotData))
			{
				param._arrPos.Emplace(frontSpotData.pos);
			}
		}

		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_NPC_TO_AUTOPROGRESS, &param);
	}
	else
	{
		FGsQuestSlotTouchParam param(QuestKey(0), true, _selectStoryId);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_TOUCH, &param);
	}


	OnClickClose();
}

void UGsUIWindowQuestSub::OnClickGiveUp()
{
	FGsStoryParam param(_selectStoryId);
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_UIACTION_GIVEUP, &param);
}

void UGsUIWindowQuestSub::OnClickClose()
{
	_onCloseQuestTab.ExecuteIfBound();

	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

void UGsUIWindowQuestSub::OnClickReward()
{
	FGsStoryParam param(_selectStoryId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_COMPLETE_REWARD, &param);
}

UGsUIWindowQuestSubItem* UGsUIWindowQuestSub::CreateTreeItem()
{
	if (0 == _treeItemPool.Num())
	{
		if (UGsUIWindowQuestSubItem* item = NewObject<UGsUIWindowQuestSubItem>(this))
		{
			_treeItemPool.Push(item);
		}
	}

	return _treeItemPool.Pop();
}

void UGsUIWindowQuestSub::ReleaseTreeItem(UGsUIWindowQuestSubItem* inItem)
{
	_treeItemPool.Push(inItem);
}

void UGsUIWindowQuestSub::ReleaseAllTreeItem()
{
	for (UGsUIWindowQuestSubItem* item : _treeItemPool)
	{
		for (UGsUIWindowQuestSubItem* childItem : item->GetChildList())
		{
			_treeItemPool.Push(childItem);
		}

		_treeItemPool.Push(item);
	}
}

void UGsUIWindowQuestSub::FreeTreeItem()
{
	for (UGsUIWindowQuestSubItem* item : _treeItemPool)
	{
		for (UGsUIWindowQuestSubItem* childItem : item->GetChildList())
		{
			childItem->ConditionalBeginDestroy();
		}

		item->ConditionalBeginDestroy();
	}
}

void UGsUIWindowQuestSub::SetData()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	FGsQuestSubTableCacheSet* questSubData = questManager->GetQuestSubTableCache();
	if (nullptr == questSubData)
		return;

	FText concurrentText;
	FText::FindText(TEXT("QuestUIText"), TEXT("Concurrent"), concurrentText);

	FText concurrentNumText;
	FText::FindText(TEXT("QuestUIText"), TEXT("ConcurrentNum"), concurrentNumText);

	concurrentNumText = FText::Format(concurrentNumText, GSQuest()->GetQuestCount(QuestType::SUB), MAX_ACTIVE_QUEST_SUB_COUNT);
	FString maxAciveString = FString::Format(TEXT("{0} {1}"), { *concurrentText.ToString(), *concurrentNumText.ToString() });

	_maxActiveCountText = FText::FromString(maxAciveString);

	QuestId outQuestId = 0;
	FText npcName;
	TSharedPtr<FGsQuestSubChapterTableCache> npcTableCache = questSubData->GetChapterTableCache(_npcId);
	if (!npcTableCache.IsValid())
		return;

	if (const FGsSchemaQuestSubChapter* schemaQuestChapter = npcTableCache->GetSchemaQuestChapter())
	{
		_npcPos = schemaQuestChapter->npcPos;
		if (const FGsSchemaNpcData* npcData = schemaQuestChapter->npcId.GetRow())
		{
			npcName = npcData->nameText;
			_npcInteractionRange = npcData->interactionRange;
		}
	}

	if (_questWidget)
	{
		_questWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& storyTableCacheList = npcTableCache->GetStoryTableCacheList();
	int32 storyNum = storyTableCacheList.Num();
	for (int i = 0; i < storyNum; ++i)
	{
		TSharedPtr<FGsQuestSubStoryTableCache> storyTableCache = storyTableCacheList[i];
		if (!storyTableCache.IsValid())
			continue;

		if (storyTableCache->GetStoryId() != _selectStoryId)
			continue;

		const FGsSchemaQuestSubStory* schemaQuestStory = storyTableCache->GetSchemaQuestStory();
		if (nullptr == schemaQuestStory)
			continue;

		_descriptionText = schemaQuestStory->storyDescriptionText;
		_titleText = schemaQuestStory->storyTitleText;

		// 마을이름
		GetTownNameText(_npcId, _townText);

		FString resultString = FString::Format(TEXT("{0}"), { *npcName.ToString() });
		_stepText = FText::FromString(resultString);

		if (const FGsSchemaQuestSubStory* story = storyTableCache->GetSchemaQuestStory())
		{
			int32 subRewardId = 0;
			if (const FGsSchemaRewardData* subReward = story->subRewardId.GetRow())
			{
				subRewardId = subReward->id;
			}

			int32 mainRewardId = 0;
			if (const FGsSchemaRewardData* mainReward = story->mainRewardId.GetRow())
			{
				mainRewardId = mainReward->id;
			}

			_rewardInfo->SetData(mainRewardId, subRewardId);
		}

		if (!_btnSwicher)
			continue;

		bool isInvadeWorld = GGameData()->IsInvadeWorld();
		if (isInvadeWorld)
		{
			_btnSwicher->SetVisibility(ESlateVisibility::Collapsed);
			_lockText->SetText(FText::FromString(TEXT("")));
		}
		else
		{
			_btnSwicher->SetVisibility(ESlateVisibility::Visible);
		}

		QuestWindowUISlotType inSlotType = QuestWindowUISlotType::STORY_LOCKED;
		questManager->GetSubQuestSlotStateInfo(_npcId, _selectStoryId, inSlotType);

		_slotStateType = inSlotType;		
		if (inSlotType == QuestWindowUISlotType::STORY_COMPLETED)
		{
			if (isInvadeWorld)
				_btnSwicher->SetVisibility(ESlateVisibility::Visible);

			_btnSwicher->SetActiveWidgetIndex((int32)5);
		}
		else if (inSlotType == QuestWindowUISlotType::STORY_REWARDED)
		{
			_btnSwicher->SetActiveWidgetIndex((int32)1);
		}
		else if (inSlotType == QuestWindowUISlotType::STORY_PROGRESS)
		{
			_btnSwicher->SetActiveWidgetIndex((int32)2);

			//bool isCompleted = GSQuest()->IsQuestStateCompleted(QuestType::SUB, schemaQuestStory->id, outQuestId);
			bool isCompleted = GSQuest()->IsQuestStateCompleted(QuestType::SUB, schemaQuestStory->id, outQuestId);
			if (false == isCompleted)
			{
				SetQuestObjective(_selectStoryId, outQuestId);
				
				if (_btnTeleport)
				{					
					bool isTeleportble = (GSQuest()->IsTeleportPossible(QuestType::SUB, _selectStoryId));
					if (false == isTeleportble)
					{						
						_btnTeleport->SetIsEnabled(isTeleportble/*schemaQuestStory->isGoToNpcTeleportAble ? true : false*/);
					}					
				}

				if (_btnAutoMove)
				{
					_btnAutoMove->SetIsEnabled((GSQuest()->IsAutoMovePossible(QuestType::SUB, _selectStoryId)) ? true : false);
				}
			}

			if (_btnGiveUp)
			{
				_btnGiveUp->SetVisibility(ESlateVisibility::Visible);
				_btnGiveUp->SetIsEnabled(!isCompleted);
			}
		}
		else if (inSlotType == QuestWindowUISlotType::STORY_LOCKED)
		{
			_btnSwicher->SetActiveWidgetIndex((int32)0);

			FText findText;
			FText::FindText(TEXT("QuestUIText"), TEXT("HoldInfo"), findText);

			_lockText->SetText(findText);
		}
		else if (inSlotType == QuestWindowUISlotType::STORY_WATING)
		{
			_btnSwicher->SetActiveWidgetIndex((int32)2);
			if (_btnGiveUp)
			{
				_btnGiveUp->SetVisibility(ESlateVisibility::Collapsed);
			}

			_selectQuestId = GSQuest()->GetQuestSubStoryInFirstQuestId(_npcId, schemaQuestStory->id);

			/*bool isAutoMove = false;
			bool isTeleportAble = false;
			_selectQuestId = GSQuest()->GetQuestSubStoryInFirstQuestId(_npcId, schemaQuestStory->id);
			TSharedPtr<FGsQuestTableCache> questTableCache = GSQuest()->GetQuestTableCache(_selectQuestId);
			if (questTableCache.isValid())
			{
				const FGsSchemaQuest* schemaQuest = questTableCache->GetSchemaQuest();
				if (nullptr != schemaQuest)
				{
					isAutoMove = schemaQuest->isAutoPlayAble;
					isTeleportAble = schemaQuest->isTeleportAble;					
				}
			}*/

			if (_btnTeleport)
			{				
				_btnTeleport->SetIsEnabled(schemaQuestStory->isGoToNpcTeleportAble ? true : false);				
			}

			if (_btnAutoMove)
			{
				_btnAutoMove->SetIsEnabled(schemaQuestStory->isGoToNpcTeleportAble ? true : false);
			}
			//
			//if (_btnAutoMove)
			//{
			//	_btnAutoMove->SetIsEnabled(schemaQuestStory->isGoToNpcTeleportAble ? true : false);
			//	// 침공월드면
			//	if (GGameData()->IsInvadeWorld())
			//	{
			//		//침공퀘스트의 경우만
			//		_btnAutoMove->SetIsEnabled(schemaQuestStory->isGoToNpcTeleportAble ? true : false);
			//	}
			//	else
			//	{
			//		// 내서버면
			//		// 침공퀘스트가 아닌경우만
			//		_btnAutoMove->SetIsEnabled((GSQuest()->IsAutoMovePossible(QuestType::SUB, _selectStoryId)) ? true : false);
			//	}				
			//}			
		}
		else if (inSlotType == QuestWindowUISlotType::STORY_LOCKED_EQUIRE_LEVEL)
		{
			_btnSwicher->SetActiveWidgetIndex((int32)0);

			// 아직 퀘스트를 진행 할 수 없습니다. (Lv{0} 이상)
			FText findText;
			FText::FindText(TEXT("QuestUIText"), TEXT("Condition"), findText);

			_lockText->SetText(FText::Format(findText, schemaQuestStory->requireLevel));
		}
		else if (inSlotType == QuestWindowUISlotType::STORY_LOCKED_SUB_STORY)
		{
			_btnSwicher->SetActiveWidgetIndex((int32)0);

			FText outText;
			GSQuest()->GetRequireStoryText(QuestType::SUB, schemaQuestStory->requireSubStoryId, outText);
			_lockText->SetText(outText);
		}
		else if (inSlotType == QuestWindowUISlotType::STORY_LOCKED_MAIN_STORY)
		{
			_btnSwicher->SetActiveWidgetIndex((int32)0);

			FText outText;
			GSQuest()->GetRequireStoryText(QuestType::MAIN, schemaQuestStory->requireMainStoryId, outText);
			if (outText.IsEmpty())
			{
				FText::FindText(TEXT("QuestUIText"), TEXT("HoldInfo"), outText);
			}
			_lockText->SetText(outText);
		}
		else
		{
			_btnSwicher->SetVisibility(ESlateVisibility::Collapsed);
		}

#if !UE_BUILD_SHIPPING
		_questIdText->SetVisibility(ESlateVisibility::Collapsed);
		_storyIdText->SetVisibility(ESlateVisibility::Collapsed);

		_storyIdText->SetVisibility(ESlateVisibility::Visible);
		FText questTextName = FText::FromString(FString::Printf(TEXT("StoryId : %lld"), _selectStoryId));
		_storyIdText->SetText(questTextName);

		FString strQuestIds;
		for (QuestId questId : storyTableCache->GetQuestIdList())
		{
			if (outQuestId != 0)
			{
				if (outQuestId == questId)
				{
					strQuestIds += FString::Printf(TEXT("V %lld\n"), questId);
				}
				else
				{
					strQuestIds += FString::Printf(TEXT("%lld\n"), questId);
				}
			}
			else
			{
				strQuestIds += FString::Printf(TEXT("%lld\n"), questId);
			}
		}

		_questIdText->SetVisibility(ESlateVisibility::Visible);
		_questIdText->SetText(FText::FromString(strQuestIds));		
#endif
	}
}

bool UGsUIWindowQuestSub::GetTownNameText(CreatureId inNpcId, OUT FText& outNameText)
{
	TArray<const FGsSchemaMapData*> mapDataList;
	if (const UGsTable* table = FGsSchemaMapData::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaMapData>(mapDataList);
	}

	for (const FGsSchemaMapData* mapData : mapDataList)
	{
		int32 mapId = mapData->id;
		if (0 == mapId || mapData->mapType == MapType::NONE)
			continue;

		FString levelKey = FString::FromInt(mapId);
		FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableClientSpawnData>(levelKey);

		const UGsTableManager& tableManager = UGsTableManager::GetInstance();
		const UGsTable* schemaClientSpawnData = tableManager.GetTable(FGsSchemaClientSpawnData::StaticStruct(), tableName);
		if (nullptr == schemaClientSpawnData)
			continue;

		TArray<const FGsSchemaClientSpawnData*> clientSpawnDataList;
		schemaClientSpawnData->GetAllRows<FGsSchemaClientSpawnData>(clientSpawnDataList);
		for (const FGsSchemaClientSpawnData* clientSpawnData : clientSpawnDataList)
		{
			if (nullptr == clientSpawnData)
				continue;

			for (const FGsSchemaSpawnElement& spawnElement : clientSpawnData->SpawnInfoList)
			{
				if (spawnElement.CreatureId == inNpcId)
				{
					outNameText = mapData->MapStringId;
					return true;
				}
			}
		}
	}
	return false;
}