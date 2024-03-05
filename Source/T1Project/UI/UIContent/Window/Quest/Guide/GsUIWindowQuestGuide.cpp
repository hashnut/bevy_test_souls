// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Quest/Guide/GsUIWindowQuestGuide.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

#include "GameObject/Quest/GsQuestHandler.h"
#include "Quest/Table/GsQuestGuideTableCacheSet.h"
#include "Quest/Guide/GsSchemaQuestGuideStory.h"
#include "Quest/GsQuestInstance.h"

#include "TreeView.h"
#include "TextBlock.h"
#include "Reward/GsSchemaRewardData.h"
#include "WidgetSwitcher.h"
#include "Components/CanvasPanel.h"
#include "Components/RichTextBlock.h"

#include "UI/UIContent/Window/Quest/GsUIWindowQuestTitleItem.h"
#include "UI/UIContent/Common/Reward/GsUIRewardInfoBase.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Option/GsServerOption.h"
#include "GsUIWindowQuestGuideItem.h"
#include "GsUIWindowQuestGuideEntry.h"


void UGsUIWindowQuestGuide::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnReward->OnClicked.AddDynamic(this, &UGsUIWindowQuestGuide::OnClickReward);
	_btnAccept->OnClicked.AddDynamic(this, &UGsUIWindowQuestGuide::OnClickAccept);
	_btnGuide->OnClicked.AddDynamic(this, &UGsUIWindowQuestGuide::OnClickGuide);

	_treeView->OnEntryWidgetGenerated().AddUObject(this, &UGsUIWindowQuestGuide::OnEntryWidgetGenerated);
	_treeView->OnEntryWidgetReleased().AddUObject(this, &UGsUIWindowQuestGuide::OnEntryWidgetReleased);
	_treeView->OnItemClicked().AddUObject(this, &UGsUIWindowQuestGuide::OnClickedListViewItem);

	_treeRootSlotList.Reset();
}

void UGsUIWindowQuestGuide::BindMessage()
{
	// 이벤트 등록
	FGsMessageHolder* message = GMessage();

	// Window 가 열려 있는 상태에서 신규 가이드 퀘스트 해금 시 해금 상태를 바로 갱신해주기 위해서 (정렬 X)
	_listQuestDelegate.Emplace(
		message->GetQuest().AddUObject(MessageContentQuest::QUEST_GUIDE_AREA_REFRESH, this, &UGsUIWindowQuestGuide::RefreshSlotState));
}

void UGsUIWindowQuestGuide::BeginDestroy()
{	
	Super::BeginDestroy();
}

void UGsUIWindowQuestGuide::UnbindMessage()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	if (_listQuestDelegate.Num() != 0)
	{
		for (auto iter : _listQuestDelegate)
		{
			MessageManager->GetQuest().Remove(iter);
		}

		_listQuestDelegate.Empty();
	}
}

void UGsUIWindowQuestGuide::NativeConstruct()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	_AcceptingStoryId = 0;
	_scrollSelectIndex = 0;
	_selectStoryId = 0;
	_selectedEntry.Reset();

	BindMessage();

	Super::NativeConstruct();
}

void UGsUIWindowQuestGuide::NativeDestruct()
{
	FreeTreeItem();
	UnbindMessage();

	Super::NativeDestruct();
}

void UGsUIWindowQuestGuide::OnEntryWidgetGenerated(UUserWidget& userWidget)
{
	SetEntryData(&userWidget);
}

void UGsUIWindowQuestGuide::OnEntryWidgetReleased(UUserWidget& userWidget)
{
	UGsUIWindowQuestGuideEntry* entry = Cast<UGsUIWindowQuestGuideEntry>(&userWidget);
	if (nullptr != entry)
	{
		entry->SetSelect(false);
	}
}

void UGsUIWindowQuestGuide::OnClickedListViewItem(UObject* InListItem)
{
	OnClickedListEntry(InListItem);

	// ListView는 버튼이 안먹기 때문에 별도 사운드 부착 
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->PlayPressSound();
	}
}

void UGsUIWindowQuestGuide::OnClickedListEntry(UObject* InListItem)
{
	if (_selectedEntry.IsValid())
	{
		_selectedEntry->SetSelect(false);
	}

	UGsUIWindowQuestGuideItem* item = Cast<UGsUIWindowQuestGuideItem>(InListItem);
	if (nullptr != item)
	{
		int slotIndex = item->GetSlotIndex();
		StoryId storyId = item->GetStoryId();

		UUserWidget* widget = _treeView->GetEntryWidgetFromItem(item);
		if (nullptr != widget)
		{
			_selectedEntry = Cast<UGsUIWindowQuestGuideEntry>(widget);
			if (_selectedEntry.IsValid())
			{
				_selectedEntry->SetSelect(true);
				_selectStoryId = storyId;
			}
		}

		SetData(storyId);
	}
}

void UGsUIWindowQuestGuide::OnClickHudUIOnOffBtn(int32 inSlotIndex, int32 inIsOn)
{
	if (inSlotIndex != _selectedEntry->GetSlotIdex())
	{
		if (UObject* onClickItem = _treeView->GetItemAt(inSlotIndex))
		{
			OnClickedListEntry(onClickItem);
		}
	}
}

void UGsUIWindowQuestGuide::UpdateList()
{
	// 반납
	ReleaseAllTreeItem();

	Super::UpdateList();	

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	uint16 userLevel = userData->mLevel;

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	_treeView->ClearListItems();
	_treeRootSlotList.Reset();

	UGsUIWindowQuestGuideItem* runingItem = nullptr;

	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>> guideStoryTableCacheList;
	questManager->GetGuideDataList(guideStoryTableCacheList);
	int num = guideStoryTableCacheList.Num();

	_switcherNonActive->SetActiveWidgetIndex((num <= 0) ? 0 : 1);

	int scrollOffset = 0;
	int slotIndex = 0;
	StoryId storyId = INVALID_STORY_ID;
	for (TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache : guideStoryTableCacheList)
	{
		storyId = storyTableCache->GetStoryId();

		UGsUIWindowQuestGuideItem* storyItem = CreateTreeItem();
		storyItem->SetStoryItem(slotIndex, storyId);
		storyItem->_delegateClickHudUIOnOffBtn.BindUObject(this, &UGsUIWindowQuestGuide::OnClickHudUIOnOffBtn);

		if (_selectStoryId == storyId)
		{
			scrollOffset = slotIndex;
		}

		_treeRootSlotList.Add(storyItem);
		_treeView->SetOnGetItemChildren<UGsUIWindowQuestGuideItem>(storyItem, &UGsUIWindowQuestGuideItem::OnGetChildren);
		++slotIndex;
	}

	_treeView->SetListItems(_treeRootSlotList);
	_treeView->SetScrollOffset(scrollOffset);
}

void UGsUIWindowQuestGuide::RefreshSlotState(const IGsMessageParam* In_data)
{
	for (UUserWidget* userWidget : _treeView->GetDisplayedEntryWidgets())
	{
		SetEntryData(userWidget);
	}
}

void UGsUIWindowQuestGuide::RefreshAll()
{
	if (_selectedEntry.IsValid())
	{
		SetData(_selectedEntry->GetStoryId());
	}
	
	TArray<UObject*> itemList = _treeView->GetListItems();
	for (UObject* obj : itemList)
	{
		UGsUIWindowQuestGuideItem* item = Cast<UGsUIWindowQuestGuideItem>(obj);
		if (nullptr != item)
		{		
			if (UUserWidget* widget = _treeView->GetEntryWidgetFromItem(item))
			{
				TWeakObjectPtr<UGsUIWindowQuestGuideEntry> entry = Cast<UGsUIWindowQuestGuideEntry>(widget);
				if (entry.IsValid())
				{
					entry->UpdateRedDot();
					entry->UpdateHudOnOff();
				}
			}
		}
	}
}

void UGsUIWindowQuestGuide::RefreshQuestObjective()
{
	if (_selectedEntry.IsValid())
	{
		SetData(_selectedEntry->GetStoryId());
	}
}

void UGsUIWindowQuestGuide::SetData(StoryId inStoryId)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	FText concurrentText;
	FText::FindText(TEXT("QuestUIText"), TEXT("Concurrent"), concurrentText);

	FText concurrentNumText;
	FText::FindText(TEXT("QuestUIText"), TEXT("ConcurrentNum"), concurrentNumText);

	concurrentNumText = FText::Format(concurrentNumText, questManager->GetQuestCount(QuestType::GUIDE), MAX_ACTIVE_QUEST_GUILD_COUNT);
	FString maxAciveString = FString::Format(TEXT("{0} {1}"), { *concurrentText.ToString(), *concurrentNumText.ToString() });

	_maxActiveCountText = FText::FromString(maxAciveString);

	TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache = questManager->GetGuideStoryTableCache(inStoryId);
	if (!storyTableCache.IsValid())
		return;

	const FGsSchemaQuestGuideStory* schemaQuestStory = storyTableCache->GetSchemaQuestStory();
	if (nullptr == schemaQuestStory)
		return;

	_descriptionText = schemaQuestStory->storyDescriptionText;

	_textDetailDesc->SetText(schemaQuestStory->storyDetailDescText);

	_textTitle->SetColorAndOpacity(questManager->GetTextColor(QuestType::GUIDE, inStoryId));
	_textTitle->SetText(schemaQuestStory->storyTitleText);

	if (_btnReward)
		_btnReward->SetVisibility(ESlateVisibility::Collapsed);
	if (_btnAccept)
		_btnAccept->SetVisibility(ESlateVisibility::Collapsed);
	if (_btnGuide)
		_btnGuide->SetVisibility(ESlateVisibility::Collapsed);

	QuestState outQuestState = QuestState::NONE;
	QuestWindowUISlotType outSlotType = QuestWindowUISlotType::STORY_PROGRESS;
	questManager->GetGuideQuestSlotStateInfo(inStoryId, outQuestState, outSlotType);

	if ((QuestWindowUISlotType::STORY_PROGRESS == outSlotType))
	{
		if (outQuestState == QuestState::COMPLETED)
		{
			if (_btnReward)
				_btnReward->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		if (outQuestState == QuestState::ACCEPTED)
		{
			if (schemaQuestStory->guideQuestWindow != EGsGuideQuestWindowType::ACTIVE_HUD &&
				schemaQuestStory->guideQuestWindow != EGsGuideQuestWindowType::ACTIVE_WINDOW_QUEST)
			{
				if (_btnGuide)
					_btnGuide->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
	}
	else if ((QuestWindowUISlotType::STORY_WATING == outSlotType))
	{
		if (_btnAccept)
			_btnAccept->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if ((QuestWindowUISlotType::STORY_REWARDED != outSlotType))
	{
		if (_canvasPanelQuestDsc)
			_canvasPanelQuestDsc->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (_questText)
			_questText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		SetQuestObjective(_selectStoryId, storyTableCache->GetQuestId());
	}
	else
	{
		if (_canvasPanelQuestDsc)
			_canvasPanelQuestDsc->SetVisibility(ESlateVisibility::Collapsed);

		if (_questText)
			_questText->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (_richLockText)
		_richLockText->SetVisibility(ESlateVisibility::Collapsed);

	if (outSlotType == QuestWindowUISlotType::STORY_LOCKED_EQUIRE_LEVEL)
	{
		FText findText;
		FText::FindText(TEXT("QuestUIText"), TEXT("Condition"), findText);

		if (_richLockText)
		{
			_richLockText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_richLockText->SetText(FText::Format(findText, schemaQuestStory->requireLevel));
		}
	}

	if (outSlotType == QuestWindowUISlotType::STORY_LOCKED_MAIN_STORY)
	{
		FText outText;

		const FGsSchemaQuestMainStory* schemaQuestMainStory = schemaQuestStory->requireMainStoryId.GetRow();
		if (schemaQuestMainStory)
		{
			GSQuest()->GetRequireStoryText(QuestType::MAIN, schemaQuestMainStory->id, outText);
			if (outText.IsEmpty())
			{
				FText::FindText(TEXT("QuestUIText"), TEXT("HoldInfo"), outText);
			}

			if (_richLockText)
			{
				_richLockText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				_richLockText->SetText(outText);
			}
		}
	}

	int32 mainRewardId = 0;
	if (const FGsSchemaRewardData* schemaRewardData = schemaQuestStory->mainRewardId.GetRow())
		mainRewardId = schemaRewardData->id;

	int32 subRewardId = 0;
	if (const FGsSchemaRewardData* schemaRewardData = schemaQuestStory->subRewardId.GetRow())
		subRewardId = schemaRewardData->id;

	_rewardInfo->SetData(mainRewardId, subRewardId);

#if !UE_BUILD_SHIPPING
	_storyIdText->SetVisibility(ESlateVisibility::Visible);
	FText questTextName = FText::FromString(FString::Printf(TEXT("StoryId : %lld"), inStoryId));
	_storyIdText->SetText(questTextName);
#else
	_storyIdText->SetVisibility(ESlateVisibility::Collapsed);
#endif
}

void UGsUIWindowQuestGuide::SetEntryData(UUserWidget* userWidget)
{
	UGsUIWindowQuestGuideEntry* entry = Cast<UGsUIWindowQuestGuideEntry>(userWidget);
	if (nullptr != entry)
	{
		TWeakObjectPtr<class UGsUIWindowQuestGuideItem> itemPtr = entry->GetItem();
		if (itemPtr.IsValid())
		{
			int slotIndex = itemPtr->GetSlotIndex();
			StoryId storyId = itemPtr->GetStoryId();
			entry->SetDataStory(slotIndex, storyId);

			if (0 < _selectStoryId)
			{
				if (storyId == _selectStoryId)
				{
					OnClickedListEntry(Cast<UObject>(itemPtr));
				}
			}
			else
			{
				if (0 >= slotIndex)
				{
					OnClickedListEntry(Cast<UObject>(itemPtr));
				}
			}
		}
	}
}

void UGsUIWindowQuestGuide::OnClickReward()
{
	FGsStoryParam param(_selectStoryId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_GUIDE_POPUP_REWARD, &param);
}

void UGsUIWindowQuestGuide::OnClickAccept()
{
	_AcceptingStoryId = _selectStoryId;
	FGsStoryParam param(_AcceptingStoryId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_GUIDE_POPUP_ACCEPT, &param);
}

void UGsUIWindowQuestGuide::OnClickGuide()
{
	FGsStoryParam param(_selectStoryId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_GUIDE_AUTO_PROGRESS, &param);
}

void UGsUIWindowQuestGuide::OnClickTab()
{
	_treeView->SetScrollOffset(0);

	UObject* topItem = _treeView->GetItemAt(0);
	UGsUIWindowQuestGuideItem* item = Cast<UGsUIWindowQuestGuideItem>(topItem);
	if (nullptr != item)
	{
		_selectStoryId = item->GetStoryId();
		OnClickedListEntry(topItem);
	}
}

UGsUIWindowQuestGuideItem* UGsUIWindowQuestGuide::CreateTreeItem()
{
	if (0 == _treeItemPool.Num())
	{
		if (UGsUIWindowQuestGuideItem* item = NewObject<UGsUIWindowQuestGuideItem>(this))
		{
			_treeItemPool.Push(item);
		}
	}

	return _treeItemPool.Pop();
}

void UGsUIWindowQuestGuide::ReleaseTreeItem(UGsUIWindowQuestGuideItem* inItem)
{
	_treeItemPool.Push(inItem);
}

void UGsUIWindowQuestGuide::ReleaseAllTreeItem()
{
	for (UGsUIWindowQuestGuideItem* item : _treeItemPool)
	{
		for (UGsUIWindowQuestGuideItem* childItem : item->GetChildList())
		{
			_treeItemPool.Push(childItem);
		}

		_treeItemPool.Push(item);
	}
}

void UGsUIWindowQuestGuide::FreeTreeItem()
{
	for (UGsUIWindowQuestGuideItem* item : _treeItemPool)
	{
		for (UGsUIWindowQuestGuideItem* childItem : item->GetChildList())
		{
			childItem->ConditionalBeginDestroy();
		}

		item->ConditionalBeginDestroy();
	}
}
