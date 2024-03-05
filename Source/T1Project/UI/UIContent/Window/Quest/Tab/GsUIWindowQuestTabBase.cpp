// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowQuestTabBase.h"

#include "UI/UIContent/Window/Quest/GsUIWindowQuestTitleItem.h"
#include "UI/UIContent/Common/Reward/GsUIRewardInfoBase.h"

#include "GameObject/Quest/GsQuestHandler.h"
#include "Quest/GsQuestInstance.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "TreeView.h"
#include "Components/TextBlock.h"

#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Message/GsMessageContents.h"
#include "Management/GsMessageHolder.h"


void UGsUIWindowQuestTabBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (_treeView)
	{
		_treeView->ClearListItems();
		_treeView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	}
		
	//_treeRootSlotList.Reset();

	UpdateList();
}

void UGsUIWindowQuestTabBase::BeginDestroy()
{
	//FreeTreeItem();

	Super::BeginDestroy();
}

void UGsUIWindowQuestTabBase::SetTreeViewExpansion(UObject* inSlot, bool inIsExpansion)
{
	if (nullptr == inSlot)
		return;

	UGsUIWindowQuestTitleItem* slot = Cast<UGsUIWindowQuestTitleItem>(inSlot);
	_treeView->SetItemExpansion(slot, inIsExpansion);
}
//
//UGsUIWindowQuestTitleItem* UGsUIWindowQuestTabBase::CreateTreeItem()
//{
//	if (0 == _treeItemPool.Num())
//	{
//		if (UGsUIWindowQuestTitleItem* item = NewObject<UGsUIWindowQuestTitleItem>(this))
//		{
//			_treeItemPool.Push(item);
//		}
//	}
//
//	return _treeItemPool.Pop();
//}
//
//void UGsUIWindowQuestTabBase::ReleaseTreeItem(UGsUIWindowQuestTitleItem* inItem)
//{
//	_treeItemPool.Push(inItem);
//}
//
//void UGsUIWindowQuestTabBase::ReleaseAllTreeItem()
//{
//	for (UGsUIWindowQuestTitleItem* item : _treeItemPool)
//	{
//		for (UGsUIWindowQuestTitleItem* childItem : item->GetChildList())
//		{
//			_treeItemPool.Push(childItem);
//		}
//
//		_treeItemPool.Push(item);
//	}
//}
//
//void UGsUIWindowQuestTabBase::FreeTreeItem()
//{
//	for (UGsUIWindowQuestTitleItem* item : _treeItemPool)
//	{
//		for (UGsUIWindowQuestTitleItem* childItem : item->GetChildList())
//		{
//			childItem->ConditionalBeginDestroy();
//		}
//
//		item->ConditionalBeginDestroy();
//	}
//}

void UGsUIWindowQuestTabBase::SetRewardData(int32 InRewardId, int32 InSubRewardId)
{
	_rewardInfo->SetData(InRewardId, InSubRewardId);
}

void UGsUIWindowQuestTabBase::RefreshAll()
{
	UpdateList();
}

void UGsUIWindowQuestTabBase::UpdateList()
{
	//// 반납
	//ReleaseAllTreeItem();

	// 초기화
	_treeView->ClearListItems();
	//_treeRootSlotList.Reset();
}

void UGsUIWindowQuestTabBase::SetQuestObjective(StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex)
{
	TSharedPtr<FGsQuestData> questData = GSQuest()->GetQuestData(QuestKey(inQuestId, inQuestIndex));
	if (!questData.IsValid())
		return;

	//퀘스트오브젝티브 출력	
	TArray<FString> objectiveTextList;
	FString allText;

	int objectiveCount = 0;
	objectiveCount = questData->GetObjectiveNum();
	for (int i = 0; i < objectiveCount; ++i)
	{
		FText questName;				// name
		int32 questObjectveValue = 0;	// static value
		int32 questDynamicValue = 0;	// dynamic value

		//FText descText;
		FText countText;

		// 카운트를 보여줘야 할지 말아야 할지..
		bool IsShowParam = questData->IsShowUIObjectiveCount(i);

		questData->GetObjectiveDestText(i, questName);		
		if (IsShowParam)
		{
			questData->GetObjectiveValue(i, questObjectveValue);
			questData->GetObjectiveDynamicValue(i, questDynamicValue, inQuestIndex);

			FText findText;
			FText::FindText(TEXT("QuestUIText"), TEXT("countQuest"), findText);

			// (5/10)
			countText = FText::Format(findText, questDynamicValue, questObjectveValue);
		}
		else
		{
			countText = FText::FromString(TEXT(""));
		}

		FString resultString = FString::Format(TEXT("{0} {1}\n"), { *questName.ToString(), *countText.ToString() });
		objectiveTextList.Emplace(resultString);
	}

	for (auto& iter : objectiveTextList)
	{
		allText += iter;
	}

	if (_questWidget)
	{
		_questWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (_questText)
	{
		_questText->SetText(FText::FromString(allText));		
	}
}

void UGsUIWindowQuestTabBase::OnGiveUp()
{
	FGsStoryParam param(_selectStoryId);
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_UIACTION_GIVEUP, &param);
}

void UGsUIWindowQuestTabBase::OnTeleport()
{
	FGsQuestTeleportParam param(0, _selectStoryId, 0);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_TELEPORT, &param);

	OnClose();
}

void UGsUIWindowQuestTabBase::OnAutoMove()
{
	FGsQuestSlotTouchParam param(QuestKey(0), true, _selectStoryId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_TOUCH, &param);

	OnClose();
}

void UGsUIWindowQuestTabBase::OnReward()
{
	FGsStoryParam param(_selectStoryId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_COMPLETE_REWARD, &param);
}

void UGsUIWindowQuestTabBase::OnClose()
{
	_onCloseQuestTab.ExecuteIfBound();

	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}