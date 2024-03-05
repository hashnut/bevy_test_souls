// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowQuestMain.h"
#include "UI/UIContent/Window/Quest/Tab/GsUIWindowQuestTabBase.h"

#include "TreeView.h"
#include "WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Quest/Table/GsQuestMainTableCacheSet.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Quest/Main/GsSchemaQuestMainStory.h"

#include "UI/UIContent/Window/Quest/GsUIWindowQuestTitleItem.h"
#include "UI/UIContent/Common/Reward/GsUIRewardInfoBase.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Message/MessageParam/GsQuestMessageParam.h"
#include "GsUIWindowQuestMainItem.h"
#include "GsUIWindowQuestMainEntry.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


void UGsUIWindowQuestMain::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnTeleport->OnClicked.AddDynamic(this, &UGsUIWindowQuestMain::OnClickTeleport);
	_btnAutoMove->OnClicked.AddDynamic(this, &UGsUIWindowQuestMain::OnClickAutoMove);
	_btnReward->OnClicked.AddDynamic(this, &UGsUIWindowQuestMain::OnClickReward);

	_treeView->OnEntryWidgetGenerated().AddUObject(this, &UGsUIWindowQuestMain::OnEntryWidgetGenerated);
	_treeView->OnEntryWidgetReleased().AddUObject(this, &UGsUIWindowQuestMain::OnEntryWidgetReleased);
	_treeView->OnItemClicked().AddUObject(this, &UGsUIWindowQuestMain::OnClickedListViewItem);
	_treeView->OnItemExpansionChanged().AddUObject(this, &UGsUIWindowQuestMain::OnItemExpansionChanged);
}

void UGsUIWindowQuestMain::BeginDestroy()
{
	FreeTreeItem();
	Super::BeginDestroy();
}

void UGsUIWindowQuestMain::NativeConstruct()
{
	Super::NativeConstruct();
	ClearData();
}

void UGsUIWindowQuestMain::ClearData()
{
	_treeView->CollapseAll();
}

void UGsUIWindowQuestMain::OnEntryWidgetGenerated(UUserWidget& userWidget)
{
	UGsUIWindowQuestMainEntry* entry = Cast<UGsUIWindowQuestMainEntry>(&userWidget);
	if (nullptr != entry)
	{
		TWeakObjectPtr<class UGsUIWindowQuestMainItem> itemPtr = entry->GetItem();
		if (itemPtr.IsValid())
		{
			// 새로 출현 한 widget이 챕터면?
			bool isChapter = itemPtr->GetIsChapter();
			if (isChapter)
			{
				int32 chapterIndex = itemPtr->GetChapterIndex();
				entry->SetDataChapter(chapterIndex);

				if (_selectChapterIndex == chapterIndex)
				{
					if (UGsUIWindowQuestMainItem* item = Cast<UGsUIWindowQuestMainItem>(itemPtr))
					{
						_treeView->SetItemExpansion(item, true);
					}

					entry->SetSelect(true);
				}
			}
			else // 새로 출현 한 widget이 스토리면?
			{
				int32 chapterIndex = itemPtr->GetChapterIndex();
				StoryId storyId = itemPtr->GetStoryId();

				entry->SetDataStory(chapterIndex, storyId);

				if (_selectStoryId == storyId)
				{					
					entry->SetSelect(true);
				}
			}
		}
	}
}

void UGsUIWindowQuestMain::OnEntryWidgetReleased(UUserWidget& userWidget)
{
	if (UGsUIWindowQuestMainEntry* entry = Cast<UGsUIWindowQuestMainEntry>(&userWidget))
	{		
		entry->SetSelect(false);
	}
}

void UGsUIWindowQuestMain::OnClickedListViewItem(UObject* InListItem)
{
	UGsUIWindowQuestMainItem* clickItem = Cast<UGsUIWindowQuestMainItem>(InListItem);
	if (nullptr == clickItem)
		return;

	// 슬롯이 눌러졌다.
	SetSlotClicked(true);

	if (_selectChapterIndex != clickItem->GetChapterIndex())
	{
		if (clickItem->GetIsChapter())
		{
			// 스크롤 포커스는 클릭 한 메인챕터에
			_treeView->SetScrollOffset(clickItem->GetChapterIndex());
		}
	}

	// 슬롯 처리 
	OnClickedListEntry(InListItem);

	// ListView는 버튼이 안먹기 때문에 별도 사운드 부착 
	if (UGsUIManager* uiMgr = GUI())
		uiMgr->PlayPressSound();
}

void UGsUIWindowQuestMain::OnItemExpansionChanged(UObject* InListItem, bool isValue)
{
	if (isValue)
	{
		bool isSlotClicked = IsSlotClicked();
		// 슬롯 누른 처리가 완료되어서 슬롯 클릭이 끝났다.
		SetSlotClicked(false);

		UGsUIWindowQuestMainItem* item = Cast<UGsUIWindowQuestMainItem>(InListItem);
		if (nullptr == item)
			return;

		if (item->GetIsChapter())
		{	
			if (isSlotClicked)
			{
				if (_selectChapterIndex == item->GetChapterIndex())
				{
					return;
				}
			}

			_treeView->SetScrollOffset(_selectChapterIndex + _selectStoryIndex + 1);
		}
	}	
}

UGsUIWindowQuestMainItem* UGsUIWindowQuestMain::GetChapterItem(int32 inChapterIndex)
{
	TArray<UObject*> itemList = _treeView->GetListItems();
	for (UObject* obj : itemList)
	{
		UGsUIWindowQuestMainItem* item = Cast<UGsUIWindowQuestMainItem>(obj);
		if (nullptr != item)
		{
			if (item->GetIsChapter())
			{
				if (item->GetChapterIndex() == inChapterIndex)
				{
					return item;
				}
			}
		}
	}

	return nullptr;
}

UGsUIWindowQuestMainItem* UGsUIWindowQuestMain::GetStoryItem(StoryId inStoryId)
{
	TArray<UObject*> itemList = _treeView->GetListItems();
	for (UObject* obj : itemList)
	{
		UGsUIWindowQuestMainItem* item = Cast<UGsUIWindowQuestMainItem>(obj);
		if (nullptr != item)
		{
			if (item->GetIsChapter())
			{
				TArray<UGsUIWindowQuestMainItem*> childList = item->GetChildList();
				for (UGsUIWindowQuestMainItem* childItem : childList)
				{
					if (nullptr == childItem)
						continue;

					if (childItem->GetStoryId() == inStoryId)
					{
						return childItem;
					}
				}
			}
		}
	}

	return nullptr;
}

StoryId UGsUIWindowQuestMain::GetInChapterFirstStoryId(int32 InChapterIndex)
{
	FGsQuestMainTableCacheSet* questMainData = GSQuest()->GetQuestMainTableCache();
	if (nullptr == questMainData)
		return INVALID_STORY_ID;

	TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = questMainData->GetchapterTableCacheInIndex(InChapterIndex);
	if (!chapterTableCache.IsValid())
		return INVALID_STORY_ID;
	
	return chapterTableCache->GetInChapterFirstStoryId();
}

StoryId UGsUIWindowQuestMain::GetInChapterStoryId(int32 InChapterIndex)
{
	FGsQuestMainTableCacheSet* questMainData = GSQuest()->GetQuestMainTableCache();
	if (nullptr == questMainData)
		return INVALID_STORY_ID;

	return GetInChapterFirstStoryId(InChapterIndex);
}

void UGsUIWindowQuestMain::OnClickedListEntry(UObject* InListItem)
{
	FGsQuestMainTableCacheSet* questMainData = GSQuest()->GetQuestMainTableCache();
	if (nullptr == questMainData)
		return;

	// 현재 진행 중인 나의 챕터
	int32 myChapterIndex = questMainData->GetCurrentQuestChapterIndex();
	// 현재 진행 중인 나의 스토리
	StoryId myStoryId = questMainData->GetCurrentStoryId();

	UGsUIWindowQuestMainItem* clickItem = Cast<UGsUIWindowQuestMainItem>(InListItem);
	if (nullptr == clickItem)
		return;

	UUserWidget* clickWidget = _treeView->GetEntryWidgetFromItem(clickItem);
	if (nullptr == clickWidget)
		return;

	TWeakObjectPtr<UGsUIWindowQuestMainEntry> clickEntry = Cast<UGsUIWindowQuestMainEntry>(clickWidget);
	if (!clickEntry.IsValid())
		return;

	// 클릭 된 타입 (챕터 or 스토리)
	bool isChapter = clickItem->GetIsChapter();	
	// 클릭 된 슬롯의 챕터
	int32 clickChapterIndex = clickItem->GetChapterIndex();
	// 클릭 된 슬롯이 현재 진행 중인 나의 챕터와 같은가?
	bool isMyChpater = (myChapterIndex == clickChapterIndex) ? true : false;

	// 클릭 된 슬롯의 챕터가 현재 진행 중인 챕터보다 미래의 챕터면 열어주지않는다.	
	if (true == isChapter && (myChapterIndex < clickChapterIndex))
	{
		_treeView->SetItemExpansion(clickItem, false);
		return;
	}

	// 선택된 슬롯이 챕터일때
	if (isChapter)
	{		
		if (_selectChapterIndex != clickChapterIndex)
		{
			// 모든 열린것들을 닫거나 끄기
			AllItemNonExpansion();

			// 챕터의 스토리 아이디 저장
			_selectStoryId = GetInChapterStoryId(clickChapterIndex);
			_selectStoryIndex = 0;
		}

		// 클릭된 챕터를 열어준다.
		_treeView->SetItemExpansion(clickItem, true);
		// 클릭된 챕터의 이미지를 선택 된 이미지로 바꾼다.
		clickEntry->SetSelect(true);
		// 선택 된 챕터 아이디 저장
		_selectChapterIndex = clickChapterIndex;
	}
	else // 선택된 슬롯이 스토리일때
	{
		// 클릭 된 슬롯의 스토리
		int32 clickStoryId = clickItem->GetStoryId();

		if (_selectStoryId != clickStoryId)
		{
			/*
			*  이미 선택 되어 있던 스토리
			*/
			// 이미 선택된 스토리의 선택 이미지를 끈다.
			if (UGsUIWindowQuestMainItem* selectStoryItem = GetStoryItem(_selectStoryId))
			{
				UUserWidget* selectWidget = _treeView->GetEntryWidgetFromItem(selectStoryItem);
				if (nullptr != selectWidget)
				{
					TWeakObjectPtr<UGsUIWindowQuestMainEntry> selectStoryEntry = Cast<UGsUIWindowQuestMainEntry>(selectWidget);
					if (selectStoryEntry.IsValid())
					{
						selectStoryEntry->SetSelect(false);
					}
				}
			}
		}

		// 클릭 된 슬롯의 이미지를 바꾼다.
		clickEntry->SetSelect(true);
		// 클릭 된 스토리 아이디 저장
		_selectStoryId = clickStoryId;
		// 클릭 된 스토리 인덱스 저장
		_selectStoryIndex = clickItem->GetStoryIndex();
	}

	// 상세 페이지 셋팅
	SetData();
}

void UGsUIWindowQuestMain::AllItemNonExpansion()
{
	/*
	*  이미 선택 되어 있던 챕터
	*/
	// 이미 선택되어 있던 챕터의 선택 이미지를 끄고 닫아준다.		
	if (UGsUIWindowQuestMainItem* selectChapterItem = GetChapterItem(_selectChapterIndex))
	{
		UUserWidget* selectWidget = _treeView->GetEntryWidgetFromItem(selectChapterItem);
		if (nullptr != selectWidget)
		{
			TWeakObjectPtr<UGsUIWindowQuestMainEntry> selectChapterEntry = Cast<UGsUIWindowQuestMainEntry>(selectWidget);
			if (selectChapterEntry.IsValid())
			{
				selectChapterEntry->SetSelect(false);
			}
		}

		/*
		*  이미 선택 되어 있던 스토리
		*/
		// 이미 선택된 스토리의 선택 이미지를 끈다.
		if (UGsUIWindowQuestMainItem* selectStoryItem = GetStoryItem(_selectStoryId))
		{
			UUserWidget* selectStoryWidget = _treeView->GetEntryWidgetFromItem(selectStoryItem);
			if (nullptr != selectStoryWidget)
			{
				TWeakObjectPtr<UGsUIWindowQuestMainEntry> selectStoryEntry = Cast<UGsUIWindowQuestMainEntry>(selectStoryWidget);
				if (selectStoryEntry.IsValid())
				{
					selectStoryEntry->SetSelect(false);
				}
			}
		}

		_treeView->SetItemExpansion(selectChapterItem, false);
	}
}

void UGsUIWindowQuestMain::RefreshAll()
{
	SetData();
}

void UGsUIWindowQuestMain::UpdateList()
{
	// 반납
	ReleaseAllTreeItem();

	Super::UpdateList();

	const FGsQuestMainTableCacheSet* questMainData = GSQuest()->GetQuestMainTableCache();
	if (nullptr != questMainData)
	{
		_treeView->ClearListItems();
		_treeRootSlotList.Reset();

		const TArray<TSharedPtr<FGsQuestChapterTableCache>>& chapterDataList = questMainData->GetQuestChapterTableCacheList();

		for (TSharedPtr<FGsQuestChapterTableCache> chapterTableCache : chapterDataList)
		{
			if (nullptr == chapterTableCache)
				continue;

			if (false == chapterTableCache->IsExposeQuest())
				continue;			

			UGsUIWindowQuestMainItem* chapterItem = CreateTreeItem();
			if (nullptr == chapterItem)
				continue;

			int32 chapterIndex = chapterTableCache->GetChapterIndex();
			chapterItem->SetChapterItem(chapterIndex);

			TArray<TSharedPtr<FGsQuestStoryTableCache>> storyTableCacheList = chapterTableCache->GetStoryTableCacheList();
			for (TSharedPtr<FGsQuestStoryTableCache> storyTableCache : storyTableCacheList)
			{
				if (!storyTableCache.IsValid())
					continue;

				if(false == storyTableCache->IsExposeQuest())
					continue;

				UGsUIWindowQuestMainItem* storyItem = CreateTreeItem();
				if (nullptr == storyItem)
					continue;

				StoryId storyId = storyTableCache->GetStoryId();
				int8 storyIndex = storyTableCache->GetStoryIndex();

				storyItem->SetStoryItem(chapterIndex, storyIndex, storyId);
				chapterItem->AddChild(storyItem);			
			}

			_treeRootSlotList.Add(chapterItem);
			_treeView->SetOnGetItemChildren<UGsUIWindowQuestMainItem>(chapterItem, &UGsUIWindowQuestMainItem::OnGetChildren);
		}

		_treeView->SetListItems(_treeRootSlotList);	

		// 현재 진행 중인 나의 챕터
		_selectChapterIndex = questMainData->GetCurrentQuestChapterIndex();
		// 현재 진행 중인 나의 스토리 아이디
		_selectStoryId = questMainData->GetCurrentStoryId();
		// 현재 진행 중인 나의 스토리 인덱스
		_selectStoryIndex = questMainData->GetCurrentStoryIndex();

		if (UGsUIWindowQuestMainItem* chapterItem = GetChapterItem(_selectChapterIndex))
		{
			_treeView->SetItemExpansion(chapterItem, true);			
			_treeView->SetScrollOffset(chapterItem->GetChapterIndex());
		}
	}

	// 상세 페이지 셋팅
	SetData();
}

void UGsUIWindowQuestMain::RefreshQuestObjective()
{
	SetData();
}

void UGsUIWindowQuestMain::OnClickTab(bool isOnTab)
{
	if (false == isOnTab)
	{
		const FGsQuestMainTableCacheSet* questMainData = GSQuest()->GetQuestMainTableCache();
		if (nullptr != questMainData)
		{
			AllItemNonExpansion();

			// 현재 진행 중인 나의 챕터
			_selectChapterIndex = questMainData->GetCurrentQuestChapterIndex();
			// 현재 진행 중인 나의 스토리 아이디
			_selectStoryId = questMainData->GetCurrentStoryId();
			// 현재 진행 중인 나의 스토리 인덱스
			_selectStoryIndex = questMainData->GetCurrentStoryIndex();

			if (UGsUIWindowQuestMainItem* chapterItem = GetChapterItem(_selectChapterIndex))
			{				
				_treeView->SetScrollOffset(chapterItem->GetChapterIndex());
				_treeView->SetItemExpansion(chapterItem, true);
			}
		}
	}
	else
	{
		const FGsQuestMainTableCacheSet* questMainData = GSQuest()->GetQuestMainTableCache();
		if (nullptr != questMainData)
		{
			if (UGsUIWindowQuestMainItem* chapterItem = GetChapterItem(_selectChapterIndex))
			{
				UUserWidget* chapterWidget = _treeView->GetEntryWidgetFromItem(chapterItem);
				if (nullptr == chapterWidget)
					return;

				TWeakObjectPtr<UGsUIWindowQuestMainEntry> chapterEntry = Cast<UGsUIWindowQuestMainEntry>(chapterWidget);
				if (!chapterEntry.IsValid())
					return;

				chapterEntry->SetSelect(true);
			}

			if (UGsUIWindowQuestMainItem* storyItem = GetStoryItem(_selectStoryId))
			{
				_treeView->SetScrollOffset(_selectChapterIndex + _selectStoryIndex + 1);
				OnClickedListViewItem(storyItem);
			}
		}
	}
}

TSharedPtr<FGsQuestStoryTableCache> UGsUIWindowQuestMain::GetStoryTableCacheInIndex(int32 inChapterIndex, StoryId inStoryId)
{
	const FGsQuestMainTableCacheSet* questMainData = GSQuest()->GetQuestMainTableCache();
	if (nullptr != questMainData)
	{
		const TArray<TSharedPtr<FGsQuestChapterTableCache>> chapterDataList = questMainData->GetQuestChapterTableCacheList();
		for (TSharedPtr<FGsQuestChapterTableCache> chapterTableCache : chapterDataList)
		{
			if (!chapterTableCache.IsValid())
				continue;

			if(inChapterIndex == chapterTableCache->GetChapterIndex())
			{
				TArray<TSharedPtr<FGsQuestStoryTableCache>> storyTableCacheList = chapterTableCache->GetStoryTableCacheList();
				for (TSharedPtr<FGsQuestStoryTableCache> storyTableCache : storyTableCacheList)
				{
					if (inStoryId == storyTableCache->GetStoryId())
					{
						return storyTableCache;
					}
				}
			}
		}
	}

	return nullptr;
}

void UGsUIWindowQuestMain::SetData()
{
	int32 localPlayerLevel = 0;
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr != userData)
	{
		localPlayerLevel = userData->mLevel;
	}

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	const FGsQuestMainTableCacheSet* questMainData = questManager->GetQuestMainTableCache();
	if (nullptr == questMainData)
		return;

	TSharedPtr<FGsQuestStoryTableCache> storyTableCache = GetStoryTableCacheInIndex(_selectChapterIndex, _selectStoryId);
	if (!storyTableCache.IsValid())
		return;

	const FGsSchemaQuestMainStory* schemaQuestStory = storyTableCache->GetSchemaQuestStory();
	if (nullptr == schemaQuestStory)
		return;

	int8 selectStoryIndex = storyTableCache->GetStoryIndex();

	_titleText = schemaQuestStory->storyTitleText;
	_descriptionText = schemaQuestStory->storyDescriptionText;

	FText findText;
	FText::FindText(TEXT("QuestUIText"), TEXT("StoryNum"), findText);
	_stepText = FText::Format(findText, _selectChapterIndex + 1, storyTableCache->GetStoryIndex() + 1);

	// 보상 셋팅
	SetReward(schemaQuestStory);

	if (_questWidget)
		_questWidget->SetVisibility(ESlateVisibility::Hidden);

	/*
	* 0 : 자동진행, 텔레포트
	* 1 : 아직 퀘스트를 진행할 수 없습니다.
	* 2 : 퀘스트가 완료 되었습니다.
	* 3 : 캐릭터 레벨 {0} 부터 진행할 수 있습니다.
	* 4 : 보상 획득
	*/
	QuestId outQuestId = 0;
	

	int32 myStoryIndex = 0;
	TSharedPtr<FGsQuestStoryTableCache> currentStoryTableCache = questMainData->GetCurrentQuestStoryTableCache();
	if (currentStoryTableCache.IsValid())
	{
		myStoryIndex = currentStoryTableCache->GetStoryIndex();
	}

	bool isMyChapter = questManager->GetIsMyChapterInIndex(_selectChapterIndex);
	if (isMyChapter)
	{
		if (myStoryIndex < selectStoryIndex)
		{
			// 잠긴 스토리
			_btnSwicher->SetActiveWidgetIndex((int32)1);
		}
		else if (myStoryIndex > selectStoryIndex)
		{
			// 완료 스토리
			_btnSwicher->SetActiveWidgetIndex((int32)2);
		}
		else
		{			
			if (GSQuest()->IsStoryRewarded(QuestType::MAIN, _selectStoryId))
			{
				// 완료 스토리
				_btnSwicher->SetActiveWidgetIndex((int32)2);
			}
			else
			{
				bool isCompleted = GSQuest()->IsQuestStateCompleted(QuestType::MAIN, _selectStoryId, outQuestId);
				if (isCompleted)
				{
					_btnSwicher->SetActiveWidgetIndex((int32)4);
				}
				else
				{
					if (localPlayerLevel < storyTableCache->GetSchemaQuestStory()->requireLevel)
					{
						_btnSwicher->SetActiveWidgetIndex((int32)3);

						FText findConditionText;
						FText::FindText(TEXT("QuestUIText"), TEXT("Condition"), findConditionText);
						_lockLevelText->SetText(FText::Format(findConditionText, schemaQuestStory->requireLevel));
					}
					else
					{
						_btnSwicher->SetActiveWidgetIndex((int32)0);
						// 퀘스트 상태에 따라 텔레포트 버튼을 show/hide 처리
						if (_btnTeleport)
						{
							_btnTeleport->SetIsEnabled((GSQuest()->IsTeleportPossible(QuestType::MAIN, _selectStoryId)) ? true : false);
						}

						if (_btnAutoMove)
						{
							_btnAutoMove->SetIsEnabled((GSQuest()->IsAutoMovePossible(QuestType::MAIN, _selectStoryId)) ? true : false);
						}

						//if (_questWidget)
						//	_questWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

						GSQuest()->IsQuestStateCompleted(QuestType::MAIN, schemaQuestStory->id, outQuestId);
						SetQuestObjective(_selectStoryId, outQuestId);
					}
				}
			}
		}
	}
	else
	{
		int32 myChapterIndex = questMainData->GetCurrentQuestChapterIndex();
		if (myChapterIndex > _selectChapterIndex)
		{
			_btnSwicher->SetActiveWidgetIndex((int32)2);
		}
		else
		{
			// 잠긴 스토리
			_btnSwicher->SetActiveWidgetIndex((int32)1);
		}		
	}

#if !UE_BUILD_SHIPPING
	_storyIdText->SetVisibility(ESlateVisibility::Visible);
	FText questTextName = FText::FromString(FString::Printf(TEXT("StoryId : %lld"), _selectStoryId));
	_storyIdText->SetText(questTextName);

	if (GSQuest()->GetIsShowQuestId())
	{
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
	}
#else
	_questIdText->SetVisibility(ESlateVisibility::Collapsed);
	_storyIdText->SetVisibility(ESlateVisibility::Collapsed);
#endif
}

void UGsUIWindowQuestMain::SetReward(const FGsSchemaQuestMainStory* inSchemaQuestStory)
{
	int32 subRewardId = 0;
	if (const FGsSchemaRewardData* subReward = inSchemaQuestStory->subRewardId.GetRow())
	{
		subRewardId = subReward->id;
	}

	int32 mainRewardId = 0;
	if (const FGsSchemaRewardData* mainReward = inSchemaQuestStory->mainRewardId.GetRow())
	{
		mainRewardId = mainReward->id;
	}

	_rewardInfo->SetData(mainRewardId, subRewardId);
}

void UGsUIWindowQuestMain::OnClickTeleport()
{
	OnTeleport();
}

void UGsUIWindowQuestMain::OnClickAutoMove()
{
	OnAutoMove();
}

void UGsUIWindowQuestMain::OnClickReward()
{
	OnReward();
}

void UGsUIWindowQuestMain::OnClickClose()
{
	OnClose();
}

UGsUIWindowQuestMainItem* UGsUIWindowQuestMain::CreateTreeItem()
{
	if (0 == _treeItemPool.Num())
	{
		if (UGsUIWindowQuestMainItem* item = NewObject<UGsUIWindowQuestMainItem>(this))
		{
			_treeItemPool.Push(item);
		}
	}

	return _treeItemPool.Pop();
}

void UGsUIWindowQuestMain::ReleaseTreeItem(UGsUIWindowQuestMainItem* inItem)
{
	_treeItemPool.Push(inItem);
}

void UGsUIWindowQuestMain::ReleaseAllTreeItem()
{
	for (UGsUIWindowQuestMainItem* item : _treeItemPool)
	{
		for (UGsUIWindowQuestMainItem* childItem : item->GetChildList())
		{
			_treeItemPool.Push(childItem);
		}

		_treeItemPool.Push(item);
	}
}

void UGsUIWindowQuestMain::FreeTreeItem()
{
	for (UGsUIWindowQuestMainItem* item : _treeItemPool)
	{
		for (UGsUIWindowQuestMainItem* childItem : item->GetChildList())
		{
			childItem->ConditionalBeginDestroy();
		}

		item->ConditionalBeginDestroy();
	}
}
