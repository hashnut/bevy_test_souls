// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Quest/Repeat/GsUIWindowQuestRepeat.h"
#include "../Tab/GsUIWindowQuestTabBase.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "GameObject/Quest/GsQuestHandler.h"

#include "Quest/GsQuestInstance.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Quest/Table/GsQuestRepeatTableCacheSet.h"
#include "Quest/Repeat/GsSchemaQuestRepeatStory.h"
#include "Quest/Repeat/GsSchemaQuestRepeatScroll.h"
#include "Quest/Repeat/GsSchemaQuestRepeatScrollRefreshInfoSet.h"

#include "TreeView.h"
#include "TextBlock.h"
#include "WidgetSwitcher.h"

#include "Reward/GsSchemaRewardData.h"

#include "UI/UIContent/Common/Reward/GsUIRewardInfoBase.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/Quest/GsUIWindowQuestTitleItem.h"
#include "UI/UIContent/Common/GsUICurrencyRefreshCountButton.h"

#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Object.h"
#include "Message/GsMessageGameObject.h"
#include "UTIL/GsTableUtil.h"
#include "GsUIWindowQuestRepeatEntry.h"
#include "GsUIWindowQuestRepeatItem.h"
#include "Quest/GsQuestData.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Option/GsServerOption.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"


void UGsUIWindowQuestRepeat::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnGiveUp->OnClicked.AddDynamic(this, &UGsUIWindowQuestRepeat::OnClickGiveUp);
	_btnTeleport->OnClicked.AddDynamic(this, &UGsUIWindowQuestRepeat::OnClickTeleport);
	_btnAutoMove->OnClicked.AddDynamic(this, &UGsUIWindowQuestRepeat::OnClickAutoMove);
	_btnReward->OnClicked.AddDynamic(this, &UGsUIWindowQuestRepeat::OnClickReward);
	_btnRefresh->OnClickRefreshButton.BindUObject(this, &UGsUIWindowQuestRepeat::OnClickRefresh);

	_treeView->OnEntryWidgetGenerated().AddUObject(this, &UGsUIWindowQuestRepeat::OnEntryWidgetGenerated);
	_treeView->OnEntryWidgetReleased().AddUObject(this, &UGsUIWindowQuestRepeat::OnEntryWidgetReleased);
	_treeView->OnItemClicked().AddUObject(this, &UGsUIWindowQuestRepeat::OnClickedListViewItem);
}

void UGsUIWindowQuestRepeat::BeginDestroy()
{
	FreeTreeItem();
	Super::BeginDestroy();
}

void UGsUIWindowQuestRepeat::NativeConstruct()
{
	Super::NativeConstruct();

	_selectStoryId = 0;
	_questKey = QuestKey::KeyNone();
	_isSlotMoveded = false;

	BindMessages();
}

void UGsUIWindowQuestRepeat::NativeDestruct()
{
	if (_isSlotMoveded)
	{
		UGsQuestManager* questManager = GSQuest();
		if (nullptr == questManager)
			return;

		TArray<QuestStoryIdPair> saveViewIndexList;
		for (QuestKey key : _repeatQuestBoardProgressList)
		{
			if (RepeatStoryType::BOARD == questManager->GetRepeatStoryType(key))
			{
				saveViewIndexList.Add(QuestStoryIdPair(0, key._questId, key._questIndex));
			}
		}

		if (FGsServerOption* serverOption = GServerOption())
		{
			serverOption->SetRepeatQuestviewIndexList(saveViewIndexList);
			serverOption->SaveRepeatQuestviewIndexList();
		}
	}

	UnbindMessages();

	Super::NativeDestruct();
}

void UGsUIWindowQuestRepeat::BindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	_msgGameObjectHandleList.Emplace(msgMgr->GetGameObject().AddUObject(MessageGameObject::QUEST_REPEAT_REWARD_BOX_RECEIVE,
		this, &UGsUIWindowQuestRepeat::OnReceiveRepeatRewardBox));

	_msgGameObjectHandleList.Emplace(msgMgr->GetGameObject().AddUObject(MessageGameObject::QUEST_REPEAT_WINDOW_UPDATE,
		this, &UGsUIWindowQuestRepeat::OnReceiveRepeatUpdate));	

	_msgGameObjectHandleList.Emplace(msgMgr->GetGameObject().AddUObject(MessageGameObject::QUEST_REPEAT_REACCEPT,
		this, &UGsUIWindowQuestRepeat::OnReceiveRepeatReAccept));
}

void UGsUIWindowQuestRepeat::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MGameObject& msgGameObject = msgMgr->GetGameObject();
	for (MsgGameObjHandle& handle : _msgGameObjectHandleList)
	{
		msgGameObject.Remove(handle);
	}
	_msgGameObjectHandleList.Empty();
}

void UGsUIWindowQuestRepeat::OnEntryWidgetGenerated(UUserWidget& userWidget)
{
	UGsUIWindowQuestRepeatEntry* entry = Cast<UGsUIWindowQuestRepeatEntry>(&userWidget);
	if (nullptr != entry)
	{
		TWeakObjectPtr<class UGsUIWindowQuestRepeatItem> itemPtr = entry->GetItem();
		if (itemPtr.IsValid())
		{
			int slotIndex = itemPtr->GetSlotIndex();
			StoryId storyId = itemPtr->GetStoryId();
			const QuestKey& questKey = itemPtr->GetQuestKey();

			entry->SetDataStory(storyId, questKey);

			if (!_questKey.IsKeyNone())
			{
				if (_questKey == questKey)
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

void UGsUIWindowQuestRepeat::OnEntryWidgetReleased(UUserWidget& userWidget)
{
	UGsUIWindowQuestRepeatEntry* entry = Cast<UGsUIWindowQuestRepeatEntry>(&userWidget);
	if (nullptr != entry)
	{
		entry->SetSelect(false);
	}
}

void UGsUIWindowQuestRepeat::OnClickedListViewItem(UObject* InListItem)
{
	OnClickedListEntry(InListItem);

	// ListView는 버튼이 안먹기 때문에 별도 사운드 부착 
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->PlayPressSound();
	}
}

TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> UGsUIWindowQuestRepeat::GetRepeatItem(StoryId inStoryId, QuestKey inQuestKey)
{
	TArray<UObject*> itemList = _treeView->GetListItems();
	for (UObject* obj : itemList)
	{
		UGsUIWindowQuestRepeatItem* item = Cast<UGsUIWindowQuestRepeatItem>(obj);
		if (nullptr != item)
		{
			if (item->GetStoryId() == inStoryId)
			{
				if (item->GetQuestKey() == inQuestKey)
				{
					UUserWidget* widget = _treeView->GetEntryWidgetFromItem(item);
					if (nullptr != widget)
					{
						return Cast<UGsUIWindowQuestRepeatEntry>(widget);
					}
				}
			}
		}
	}

	return nullptr;
}

void UGsUIWindowQuestRepeat::OnClickedListEntry(UObject* InListItem)
{
	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> prevEntry = GetRepeatItem(_selectStoryId, _questKey);
	if (prevEntry.IsValid())
	{
		prevEntry->SetSelect(false);
	}

	UGsUIWindowQuestRepeatItem* item = Cast<UGsUIWindowQuestRepeatItem>(InListItem);
	if (nullptr != item)
	{
		_selectStoryId = item->GetStoryId();
		_questKey = item->GetQuestKey();
		_selectSlotIndex = item->GetSlotIndex();

		UUserWidget* widget = _treeView->GetEntryWidgetFromItem(item);
		if (nullptr != widget)
		{
			TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> entry = Cast<UGsUIWindowQuestRepeatEntry>(widget);
			if (entry.IsValid())
			{
				entry->SetSelect(true);
			}
		}

		SetData(_selectStoryId, _questKey);
	}
}

void UGsUIWindowQuestRepeat::OnClickTab()
{
	_treeView->SetScrollOffset(0);

	UObject* topItem = _treeView->GetItemAt(0);
	UGsUIWindowQuestRepeatItem* item = Cast<UGsUIWindowQuestRepeatItem>(topItem);
	if (nullptr != item)
	{		
		OnClickedListEntry(topItem);
	}
}

void UGsUIWindowQuestRepeat::UpdateList()
{
	// 반납
	ReleaseAllTreeItem();

	Super::UpdateList();

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	UGsUIWindowQuestRepeatItem* runingItem = nullptr;	
	_repeatQuestBoardProgressList.Empty();
	_treeView->ClearListItems();
	_treeRootSlotList.Reset();

	TArray<QuestKey> repeatQuestBoardCompletedList;
	TArray<QuestKey> repeatQuestScrollList;

	questManager->GetRepeatQuestKeyList(repeatQuestBoardCompletedList, _repeatQuestBoardProgressList, repeatQuestScrollList);
	int num = repeatQuestBoardCompletedList.Num() + _repeatQuestBoardProgressList.Num() + repeatQuestScrollList.Num();
	_switcherNonActive->SetActiveWidgetIndex((num <= 0) ? 0 : 1);	

	int slotIndex = 0;
	auto Func = [&](TArray<QuestKey>& inList, bool inIsProgress)
	{
		int inListNum = inList.Num();
		int inListCount = 0;
		for (QuestKey& questKey : inList)
		{
			TSharedPtr<FGsQuestData> questData = questManager->GetQuestData(questKey._questId);
			if (!questData.IsValid())
				continue;

			StoryId storyId = questData->GetStoryId();
			if (storyId == INVALID_STORY_ID)
				continue;

			QuestId questId = questData->GetQuestId();
			if (questId == INVALID_QUEST_ID)
				continue;

			FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questKey._questIndex);
			if (nullptr == dynamicData)
				continue;

			UGsUIWindowQuestRepeatItem* storyItem = CreateTreeItem();

			if (inIsProgress)
			{
				bool isFirst = (inListCount == 0) ? true : false;
				bool isLast = (inListCount == inListNum - 1) ? true : false;

				storyItem->SetQuestItem(storyId, questKey, slotIndex, isFirst, isLast);
			}
			else
			{
				storyItem->SetQuestItem(storyId, questKey, slotIndex);
			}
			
			storyItem->_delegateClickHudUIUpBtn.BindUObject(this, &UGsUIWindowQuestRepeat::OnClickHudUIUpBtn);
			storyItem->_delegateClickHudUIDownBtn.BindUObject(this, &UGsUIWindowQuestRepeat::OnClickHudUIDownBtn);

			_treeRootSlotList.Add(storyItem);
			_treeView->SetOnGetItemChildren<UGsUIWindowQuestRepeatItem>(storyItem, &UGsUIWindowQuestRepeatItem::OnGetChildren);
			++slotIndex;
			++inListCount;
		}
	};

	Func(repeatQuestBoardCompletedList, false);
	Func(_repeatQuestBoardProgressList, true);
	Func(repeatQuestScrollList, false);

	_treeView->SetListItems(_treeRootSlotList);
}

void UGsUIWindowQuestRepeat::RefreshAll()
{
}

void UGsUIWindowQuestRepeat::RefreshQuestObjective()
{
	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> curruntEntry = GetRepeatItem(_selectStoryId, _questKey);
	if (curruntEntry.IsValid())
	{
		SetData(curruntEntry->GetStoryId(), curruntEntry->GetQuestKey());
	}

	TArray<UObject*> itemList = _treeView->GetListItems();
	for (UObject* obj : itemList)
	{
		UGsUIWindowQuestRepeatItem* item = Cast<UGsUIWindowQuestRepeatItem>(obj);
		if (nullptr != item)
		{
			UUserWidget* widget = _treeView->GetEntryWidgetFromItem(item);
			if (nullptr != widget)
			{
				TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> entry = Cast<UGsUIWindowQuestRepeatEntry>(widget);
				if (entry.IsValid())
				{
					TSharedPtr<FGsQuestData> questData = GSQuest()->GetQuestData(item->GetQuestKey());
					if (!questData.IsValid())
						continue;

					FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(item->GetQuestKey()._questIndex);
					if (nullptr == dynamicData)
						continue;

					if (QuestState::COMPLETED == dynamicData->GetQuestState())
					{
						if (false == entry->GetIsRedDot())
						{
							UpdateList();
							break;
						}
					}
				}
			}			
		}
	}
}

void UGsUIWindowQuestRepeat::SetWorkingCount()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	FText concurrentText;
	FText::FindText(TEXT("QuestUIText"), TEXT("Concurrent"), concurrentText);

	FText concurrentNumText;
	FText::FindText(TEXT("QuestUIText"), TEXT("ConcurrentNum"), concurrentNumText);

	concurrentNumText = FText::Format(concurrentNumText, questManager->GetQuestCount(QuestType::REPEAT), MAX_ACTIVE_QUEST_REPEAT_COUNT);
	FString maxAciveString = FString::Format(TEXT("{0} {1}"), { *concurrentText.ToString(), *concurrentNumText.ToString() });

	_maxActiveCountText = FText::FromString(maxAciveString);
}

void UGsUIWindowQuestRepeat::SetReward(const FGsSchemaQuestRepeatStory* inSchemaQuestStory)
{
	int32 mainRewardId = 0;
	if (const FGsSchemaRewardData* schemaRewardData = inSchemaQuestStory->mainRewardId.GetRow())
		mainRewardId = schemaRewardData->id;

	int32 subRewardId = 0;
	if (const FGsSchemaRewardData* schemaRewardData = inSchemaQuestStory->subRewardId.GetRow())
		subRewardId = schemaRewardData->id;

	if (RewardType::PICK_REWARD_BOX_LIST == FGsRewardHelper::GetRewardType(mainRewardId) ||
		RewardType::PICK_REWARD_BOX_LIST == FGsRewardHelper::GetRewardType(subRewardId))
	{
		TArray<RewardBoxItemIdPair> boxIdPairList;
		FGsRewardHelper::GetRewardBoxItemIdPairList(_questKey._questId, _questKey._questIndex, boxIdPairList);

		_rewardInfo->SetData(mainRewardId, subRewardId, boxIdPairList);
	}
	else
	{
		_rewardInfo->SetData(mainRewardId, subRewardId);
	}

	// 새로고침 버튼
	if (RepeatStoryType::BOARD == inSchemaQuestStory->repeatStoryType)
	{
		_btnRefresh->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_btnRefresh->SetVisibility(ESlateVisibility::Visible);

		SetRefreshData(_questKey._questId, _questKey._questIndex);
	}
}

void UGsUIWindowQuestRepeat::SetQuestSlotState(QuestState inQuestState)
{	
	bool isCompleted = (inQuestState == QuestState::COMPLETED) ? true : false;
	bool isProgress = (inQuestState == QuestState::ACCEPTED) ? true : false;

	_btnSwicher->SetActiveWidgetIndex(isCompleted ? 1 : 0);
	if (isProgress)
	{
		// 퀘스트 상태에 따라 텔레포트 버튼을 show/hide 처리
		if (_btnTeleport)
		{
			_btnTeleport->SetIsEnabled((GSQuest()->IsTeleportPossible(QuestType::REPEAT, _selectStoryId)) ? true : false);
		}

		if (_btnAutoMove)
		{
			_btnAutoMove->SetIsEnabled((GSQuest()->IsAutoMovePossible(QuestType::REPEAT, _selectStoryId)) ? true : false);
		}
	}
}

void UGsUIWindowQuestRepeat::SetData(StoryId inStoryId, QuestKey inQuestKey)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	TSharedPtr<FGsQuestRepeatStoryTableCache> repeatStoryTableCache = questManager->GetQuestRepeatStoryTableCache(inStoryId);
	if (!repeatStoryTableCache.IsValid())
		return;

	const FGsSchemaQuestRepeatStory* schemaQuestStory = repeatStoryTableCache->GetSchemaQuestStory();
	if (nullptr == schemaQuestStory)
		return;

	if (inQuestKey.IsKeyNone())
		return;

	TSharedPtr<FGsQuestData> questData = questManager->GetQuestData(inQuestKey);
	if (!questData.IsValid())
		return;

	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(inQuestKey._questIndex);
	if (nullptr == dynamicData)
		return;

	_selectStoryId = inStoryId;

	// 진행 상태에 따른 버튼처리 (보상받기가 나올것인지, 자동이동 등이 나올것인지)
	SetQuestSlotState(dynamicData->GetQuestState());
	// 진행중인 퀘스트 카운트 / 진행가능 MAX 퀘스트 카운트
	SetWorkingCount();
	// 보상 셋팅
	SetReward(schemaQuestStory);
	// 수행목표 갱신
	SetQuestObjective(inStoryId, inQuestKey._questId, inQuestKey._questIndex);

	// 제목, 설명
	_textTitle->SetColorAndOpacity(questManager->GetTextColor(QuestType::REPEAT, inStoryId));
	_textTitle->SetText(schemaQuestStory->storyTitleText);
	_descriptionText = schemaQuestStory->storyDescriptionText;

#if !UE_BUILD_SHIPPING
	_storyIdText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FText questTextName = FText::FromString(FString::Printf(TEXT("StoryId : %lld"), inStoryId));
	_storyIdText->SetText(questTextName);

	_questIdText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	questTextName = FText::FromString(FString::Printf(TEXT("QuestId : %lld, QuestIndex : %d"), inQuestKey._questId, inQuestKey._questIndex));
	_questIdText->SetText(questTextName);
#else
	_storyIdText->SetVisibility(ESlateVisibility::Collapsed);
	_questIdText->SetVisibility(ESlateVisibility::Collapsed);
#endif
}

void UGsUIWindowQuestRepeat::SetRefreshData(QuestId inQuestId, QuestIndex inQuestIndex)
{
	TSharedPtr<FGsQuestData> questData = GSQuest()->GetQuestData(inQuestId);
	if (false == questData.IsValid())
	{
		return;
	}

	if (FGsQuestRepeatData* repeatData = static_cast<FGsQuestRepeatData*>(questData.Get()))
	{
		if (const FGsSchemaQuestRepeatScrollRefreshInfoSet* refreshInfoSet =
			GetRefrehTable(repeatData->GetRepeatScrollId(inQuestIndex)))
		{
			uint8 refreshCount = repeatData->GetRefreshCount(inQuestIndex);
			int32 remainCount = refreshInfoSet->currencyInfoList.Num() - refreshCount;

			if (refreshInfoSet->currencyInfoList.IsValidIndex(refreshCount))
			{
				const FGsSchemaQuestRepeatScrollRefreshInfo& refreshInfo = refreshInfoSet->currencyInfoList[refreshCount];
				_btnRefresh->SetData(remainCount, refreshInfo.costCurrencyType, refreshInfo.costCurrencyAmount);

				return;
			}
		}
	}

	// 갱신횟수 모두 소모 or 데이터 설정 실패 시 비활성화
	_btnRefresh->SetData(0, CurrencyType::NONE, 0);
}

void UGsUIWindowQuestRepeat::OnClickGiveUp()
{
	OnGiveUp();
}

void UGsUIWindowQuestRepeat::OnClickTeleport()
{
	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> curruntEntry = GetRepeatItem(_selectStoryId, _questKey);
	if (!curruntEntry.IsValid())
	{
		return;
	}
	
	FGsQuestTeleportParam param(curruntEntry->GetQuestKey()._questId, curruntEntry->GetStoryId(), curruntEntry->GetQuestKey()._questIndex);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_TELEPORT, &param);

	OnClose();
}

void UGsUIWindowQuestRepeat::OnClickAutoMove()
{
	OnAutoMove();
}

void UGsUIWindowQuestRepeat::OnClickReward()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	if (netMgr->GetCoolTime().IsCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_QUEST_REPEAT_REWARD)))
	{
		// TEXT: 잠시 후 다시 시도해주세요.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		return;
	}

	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> curruntEntry = GetRepeatItem(_selectStoryId, _questKey);
	if (!curruntEntry.IsValid())
	{
		return;
	}

	FGsUIHelper::ShowBlockUI();
	FGsStoryParam param(_selectStoryId, curruntEntry->GetQuestKey());
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_COMPLETE_REWARD, &param);
}

void UGsUIWindowQuestRepeat::OnClickRefresh()
{
	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> curruntEntry = GetRepeatItem(_selectStoryId, _questKey);
	if (!curruntEntry.IsValid())
	{
		return;
	}

	FGsQuestParam param(curruntEntry->GetQuestKey()._questId, curruntEntry->GetQuestKey()._questIndex);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_REPEAT_REWARD_REFRESH, &param);
}

void UGsUIWindowQuestRepeat::OnGiveUp()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
		return;

	if (netMgr->GetCoolTime().IsCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_QUEST_REPEAT_GIVE_UP)))
	{
		// TEXT: 잠시 후 다시 시도해주세요.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		return;
	}

	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> curruntEntry = GetRepeatItem(_selectStoryId, _questKey);
	if (!curruntEntry.IsValid())
	{
		return;
	}

	FGsStoryParam param(_selectStoryId, curruntEntry->GetQuestKey());
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_UIACTION_GIVEUP, &param);
}

void UGsUIWindowQuestRepeat::OnClickHudUIOnOffBtn(int32 inSlotIndex, int32 inIsOn)
{
	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> curruntEntry = GetRepeatItem(_selectStoryId, _questKey);
	if (!curruntEntry.IsValid())
	{
		return;
	}

	if (inSlotIndex != curruntEntry->GetSlotIndex())
	{
		if (UObject* onClickItem = _treeView->GetItemAt(inSlotIndex))
		{
			OnClickedListEntry(onClickItem);
		}
	}
}

void UGsUIWindowQuestRepeat::OnClickHudUIUpBtn(int32 inSlotIndex, int32 inIsUp)
{
	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> curruntEntry = GetRepeatItem(_selectStoryId, _questKey);
	if (!curruntEntry.IsValid())
		return;

	UObject* onClickItem = _treeView->GetItemAt(inSlotIndex);
	if (nullptr == onClickItem)
		return;
	
	UGsUIWindowQuestRepeatItem* myItem = Cast<UGsUIWindowQuestRepeatItem>(onClickItem);
	if (nullptr == myItem)
		return;

	int listNum = _treeRootSlotList.Num();
	int indexMy = _treeRootSlotList.IndexOfByKey(myItem);
	int index = indexMy - 1;
	if (0 > index)
		return;

	UUserWidget* myWidget = _treeView->GetEntryWidgetFromItem(myItem);
	if (nullptr == myWidget)
		return;

	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> myEntry = Cast<UGsUIWindowQuestRepeatEntry>(myWidget);
	if (!myEntry.IsValid())
		return;

	UObject* targetObject = _treeView->GetItemAt(index);
	if (nullptr == targetObject)
		return;

	UGsUIWindowQuestRepeatItem* targetItem = Cast<UGsUIWindowQuestRepeatItem>(targetObject);
	if (nullptr == targetItem)
		return;

	UUserWidget* targetWidget = _treeView->GetEntryWidgetFromItem(targetItem);
	if (nullptr == targetWidget)
		return;

	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> targetEntry = Cast<UGsUIWindowQuestRepeatEntry>(targetWidget);
	if (!targetEntry.IsValid())
		return;

	if (targetItem->GetIsFirst())
	{
		myItem->SetFirst(true);
		targetItem->SetFirst(false);
	}

	if (myItem->GetIsLast())
	{
		myItem->SetLast(false);
		targetItem->SetLast(true);
	}

	myItem->SetSlotIndex(index);
	targetItem->SetSlotIndex(indexMy);

	myEntry->SetViewUpdDownButtons();
	targetEntry->SetViewUpdDownButtons();

	_treeRootSlotList.Swap(indexMy, index);	

	listNum = _repeatQuestBoardProgressList.Num();
	indexMy = _repeatQuestBoardProgressList.IndexOfByKey(myItem->GetQuestKey());
	index = indexMy - 1;
	if (0 <= index)
	{
		_repeatQuestBoardProgressList.Swap(indexMy, index);
	}

	_treeView->SetListItems(_treeRootSlotList);
	_isSlotMoveded = true;
}

void UGsUIWindowQuestRepeat::OnClickHudUIDownBtn(int32 inSlotIndex, int32 inIsUp)
{
	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> curruntEntry = GetRepeatItem(_selectStoryId, _questKey);
	if (!curruntEntry.IsValid())
		return;

	UObject* onClickItem = _treeView->GetItemAt(inSlotIndex);
	if (nullptr == onClickItem)
		return;
	
	UGsUIWindowQuestRepeatItem* myItem = Cast<UGsUIWindowQuestRepeatItem>(onClickItem);
	if (nullptr == myItem)
		return;

	int listNum = _treeRootSlotList.Num();
	int indexMy = _treeRootSlotList.IndexOfByKey(myItem);
	int index = indexMy + 1;
	if (listNum - 1 < index)
		return;

	UUserWidget* myWidget = _treeView->GetEntryWidgetFromItem(myItem);
	if (nullptr == myWidget)
		return;

	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> myEntry = Cast<UGsUIWindowQuestRepeatEntry>(myWidget);
	if (!myEntry.IsValid())
		return;	

	UObject* targetObject = _treeView->GetItemAt(index);
	if (nullptr == targetObject)
		return;

	UGsUIWindowQuestRepeatItem* targetItem = Cast<UGsUIWindowQuestRepeatItem>(targetObject);
	if (nullptr == targetItem)
		return;

	UUserWidget* targetWidget = _treeView->GetEntryWidgetFromItem(targetItem);
	if (nullptr == targetWidget)
		return;

	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> targetEntry = Cast<UGsUIWindowQuestRepeatEntry>(targetWidget);
	if (!targetEntry.IsValid())
		return;

	if (myItem->GetIsFirst())
	{
		myItem->SetFirst(false);
		targetItem->SetFirst(true);
	}

	if (targetItem->GetIsLast())
	{
		myItem->SetLast(true);
		targetItem->SetLast(false);
	}

	myItem->SetSlotIndex(index);
	targetItem->SetSlotIndex(indexMy);

	myEntry->SetViewUpdDownButtons();
	targetEntry->SetViewUpdDownButtons();

	_treeRootSlotList.Swap(indexMy, index);	

	listNum = _repeatQuestBoardProgressList.Num();
	indexMy = _repeatQuestBoardProgressList.IndexOfByKey(myItem->GetQuestKey());
	index = indexMy + 1;
	if (listNum - 1 >= index)
	{
		_repeatQuestBoardProgressList.Swap(indexMy, index);
	}

	_treeView->SetListItems(_treeRootSlotList);
	_isSlotMoveded = true;
}

void UGsUIWindowQuestRepeat::OnReceiveRepeatRewardBox(const IGsMessageParam* InParam)
{
	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> curruntEntry = GetRepeatItem(_selectStoryId, _questKey);
	if (!curruntEntry.IsValid())
	{
		return;
	}

	// 랜덤 보상 목록 수령 후 보상 목록 업데이트
	if (const FGsQuestRepeatPopUpParam* param = InParam->Cast<const FGsQuestRepeatPopUpParam>())
	{
		if (param->_questIndex == curruntEntry->GetQuestKey()._questIndex)
		{			
			RefreshQuestObjective();
		}
	}
}

bool UGsUIWindowQuestRepeat::IsVaildData(QuestId inQuestId, QuestIndex inQuestIndex)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return false;

	TArray<TSharedPtr<FGsQuestData>> repeatQuestList;
	questManager->GetRepeatQuestList(repeatQuestList);

	for (TSharedPtr<FGsQuestData> questData : repeatQuestList)
	{
		if (!questData.IsValid())
			continue;

		StoryId storyId = questData->GetStoryId();
		if (storyId == INVALID_STORY_ID)
			continue;

		QuestId questId = questData->GetQuestId();
		if (questId == INVALID_QUEST_ID)
			continue;

		TMap<QuestIndex, TSharedPtr<FGsQuestDynamicData>>& dynamicDatas = questData->GetQuestDynamicDataList();
		for (auto& iter : dynamicDatas)
		{
			QuestIndex questIndex = iter.Key;

			if (questId == inQuestId && questIndex == inQuestIndex)
			{
				return true;
			}
		}
	}

	return false;
}

void UGsUIWindowQuestRepeat::OnReceiveRepeatUpdate(const IGsMessageParam* InParam)
{
	UpdateList();

	int listNum = _treeView->GetNumItems();
	if (0 == listNum)
	{
		_switcherNonActive->SetActiveWidgetIndex(0);
	}
	else
	{
		if (UObject* topItem = _treeView->GetItemAt(0))
		{
			OnClickedListEntry(topItem);
		}
	}
}

void UGsUIWindowQuestRepeat::OnReceiveRepeatReAccept(const IGsMessageParam* InParam)
{
	const FGsStoryParam* storyParam =
		InParam->Cast<const FGsStoryParam>();

	if (nullptr == storyParam)
		return;

	if (storyParam->_questKey.IsKeyNone())
		return;

	TWeakObjectPtr<UGsUIWindowQuestRepeatEntry> curruntEntry = GetRepeatItem(_selectStoryId, _questKey);
	if (!curruntEntry.IsValid())
	{
		return;
	}

	if (curruntEntry->GetQuestKey()._questIndex !=
		storyParam->_questKey._questIndex)
		return;

	_selectStoryId = storyParam->_storyId;
	_questKey = storyParam->_questKey;
	curruntEntry->ReSetDataStory(_selectStoryId, _questKey);
	curruntEntry->SetSelect(true);
	SetData(_selectStoryId, _questKey);
}

QuestWindowUISlotType UGsUIWindowQuestRepeat::GetRepeatSlotType(StoryId inStoryId, QuestIndex inQuestIndex)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return QuestWindowUISlotType::CHAPTER_PROGRESS;

	TArray<TSharedPtr<FGsQuestData>> repeatQuestList;
	questManager->GetRepeatQuestList(repeatQuestList);
	int num = repeatQuestList.Num();

	for (int i = 0; i < num; ++i)
	{
		TSharedPtr<FGsQuestData> questData = repeatQuestList[i];
		if (!questData.IsValid())
			continue;

		StoryId storyId = questData->GetStoryId();
		if (inStoryId != storyId)
			continue;

		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(inQuestIndex);
		if (nullptr == dynamicData)
			continue;

		return (QuestState::COMPLETED == dynamicData->GetQuestState()) ?
			QuestWindowUISlotType::STORY_COMPLETED : QuestWindowUISlotType::STORY_PROGRESS;
	}

	return QuestWindowUISlotType::CHAPTER_PROGRESS;
}

const FGsSchemaQuestRepeatScrollRefreshInfoSet* UGsUIWindowQuestRepeat::GetRefrehTable(int32 inRepeatScrollId)
{
	if (const FGsSchemaQuestRepeatScroll* row =
		UGsTableUtil::FindRowById<UGsTableQuestRepeatScroll, FGsSchemaQuestRepeatScroll>(inRepeatScrollId))
	{
		return row->refreshInfoSet.GetRow();
	}

	return nullptr;
}

UGsUIWindowQuestRepeatItem* UGsUIWindowQuestRepeat::CreateTreeItem()
{
	if (0 == _treeItemPool.Num())
	{
		if (UGsUIWindowQuestRepeatItem* item = NewObject<UGsUIWindowQuestRepeatItem>(this))
		{
			_treeItemPool.Push(item);
		}
	}

	return _treeItemPool.Pop();
}

void UGsUIWindowQuestRepeat::ReleaseTreeItem(UGsUIWindowQuestRepeatItem* inItem)
{
	_treeItemPool.Push(inItem);
}

void UGsUIWindowQuestRepeat::ReleaseAllTreeItem()
{
	for (UGsUIWindowQuestRepeatItem* item : _treeItemPool)
	{
		for (UGsUIWindowQuestRepeatItem* childItem : item->GetChildList())
		{
			_treeItemPool.Push(childItem);
		}

		_treeItemPool.Push(item);
	}
}

void UGsUIWindowQuestRepeat::FreeTreeItem()
{
	for (UGsUIWindowQuestRepeatItem* item : _treeItemPool)
	{
		for (UGsUIWindowQuestRepeatItem* childItem : item->GetChildList())
		{
			childItem->ConditionalBeginDestroy();
		}

		item->ConditionalBeginDestroy();
	}
}
