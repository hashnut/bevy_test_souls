// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildQuestListEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PaperSprite.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Common/GsUIProgressBar.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "Guild/GsGuildQuestData.h"
#include "DataSchema/Quest/Guild/GsSchemaQuestGuildStory.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "Reward/GsRewardHelper.h"
#include "Reward/GsRewardIconDataWrapper.h"
#include "Quest/GsQuestData.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "Quest/Table/GsQuestGuildTableCacheSet.h"
#include "Quest/GsQuestInstance.h"
#include "Net/GsNetSendServiceQuest.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"


void UGsUIGuildQuestListEntry::BeginDestroy()
{
	_iconHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIGuildQuestListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_iconHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_iconHelper->Initialize(_iconSelectorReward);
	_iconHelper->OnRefreshIcon.AddDynamic(this, &UGsUIGuildQuestListEntry::OnRefreshIcon);
	
	_sBtnState->OnClicked.AddDynamic(this, &UGsUIGuildQuestListEntry::OnClickState);

	_checkerState.SetIntervalSecond(2.f);
}

void UGsUIGuildQuestListEntry::SetData(const FGsGuildQuestData* InData)
{
	_storyId = InData->_storyId;
	_questId = InData->_questId;

	const FGsSchemaQuestGuildStory* storyTable = GetStoryTable(_storyId);
	if (nullptr == storyTable)
	{
		return;
	}

	TWeakObjectPtr<UGsUIGuildQuestListEntry> weakThis(this);
	FGsUIHelper::AsyncLoadSpriteFromPath(storyTable->questIcon, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* InRes)
		{
			if (UGsUIGuildQuestListEntry* strongThis = weakThis.Get())
			{
				strongThis->SetIconImageSprite(Cast<UPaperSprite>(InRes));
			}
		}));

	int32 goalCount = 0;
	if (const FGsQuestObjectiveTableCache* objectiveTable = GetObjectiveTable(InData->_storyId))
	{
		_textBlockTitle->SetText(storyTable->questTitleText);
		_textBlockDesc->SetText(objectiveTable->GetDescText());
		// 수행상태 표시
		goalCount = objectiveTable->GetObjectiveValue();
	}

	// 열정 포인트 세팅
	_textBlockZealPoint->SetText(FText::AsNumber(storyTable->zealPointAmount));

	// 보상 아이콘 세팅
	_dataList.Empty();
	if (const FGsSchemaRewardData* rewardTable = storyTable->mainRewardId.GetRow())
	{
		FGsRewardUnboxingData rewardData;
		FGsRewardHelper::UnboxingRewardData(rewardTable, rewardData);
		FGsRewardHelper::GetIconDataList(&rewardData, _dataList);
	}

	_iconHelper->RefreshAll(_dataList.Num());

	// 상태 버튼 설정
	EGsGuildQuestStateType questState = InData->GetQuestState();
	_sBtnState->SetSwitcherIndex(static_cast<int32>(questState));

	// 완료 시 버튼 터치 막음
	_sBtnState->SetVisibility((EGsGuildQuestStateType::REWARDED == questState) ?
		ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);

	switch (questState)
	{
	case EGsGuildQuestStateType::NONE:
	{
		SetUIProgressBar(0, goalCount);
	}	
	break;
	case EGsGuildQuestStateType::ACCEPTED:
	{
		int32 currCount = 0;
		if (FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler())
		{
			if (FGsQuestInstance* questInstance = questHandler->FindQuestInstance(QuestKey(_questId), _storyId))
			{
				currCount = questInstance->GetObjectiveValue(0);
			}
		}

		SetUIProgressBar(currCount, goalCount);
	}
	break;
	case EGsGuildQuestStateType::COMPLETED:
	{
		SetUIProgressBar(goalCount, goalCount);
	}
	break;
	case EGsGuildQuestStateType::REWARDED:
	{
		SetUIProgressBar(goalCount, goalCount);
	}
	break;
	}
}

void UGsUIGuildQuestListEntry::SetUIProgressBar(int32 InCurrCount, int32 InGoalCount)
{
	FText textCount;
	FGsUIStringHelper::GetTwoValueSlashedText(InCurrCount, InGoalCount, textCount);

	float percent = 0.f;
	if (0 < InCurrCount &&
		0 < InGoalCount)
	{
		percent = static_cast<float>(InCurrCount) / static_cast<float>(InGoalCount);
	}

	_progressBar->SetProgressText(textCount);
	_progressBar->SetPercent(percent);
}

void UGsUIGuildQuestListEntry::SetIconImageSprite(UPaperSprite* InSprite)
{
	_imgIcon->SetBrushFromAtlasInterface(InSprite);
}

void UGsUIGuildQuestListEntry::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon))
	{
		if (false == itemIcon->OnFinishedLongPressEvent.IsBound())
		{
			itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIGuildQuestListEntry::OnLongPressIcon);
		}

		itemIcon->SetEnableDetailPopup(true);
		itemIcon->SetCenterPopup(true);

		// 아이콘에 데이터 세팅
		FGsRewardIconDataWrapper::SetUIIconItem(&_dataList[InIndex], itemIcon);
	}
}

void UGsUIGuildQuestListEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUIGuildQuestListEntry::OnClickState()
{
	EGsGuildQuestStateType questState = static_cast<EGsGuildQuestStateType>(_sBtnState->GetSwitcherIndex());
	
	// 완료 상태일 때는 클릭 무시 처리
	if (EGsGuildQuestStateType::REWARDED == questState)
	{
		return;
	}

	// 광클릭 방지
	if (false == _checkerState.IsTimeOver())
	{
		// TEXT: 잠시 후 다시 시도해주세요.
		FText textMsg;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		return;
	}

	_checkerState.UpdateTime();
	
	switch (questState)
	{
	case EGsGuildQuestStateType::NONE:
	{
		if (FGsGuildQuestDataHandler* questHandler = GGuild()->GetQuestHandler())
		{
			int activeCount = questHandler->GetCompleteCount() + questHandler->GetAcceptCount();

			if (MAX_COMPLETE_QUEST_GUILD_COUNT <= questHandler->GetCompleteCount() ||
				MAX_ACTIVE_QUEST_GUILD_COUNT <= activeCount)
			{
				// TEXT: 퀘스트 수행 가능 횟수가 부족합니다.
				FText textMsg;
				if (FText::FindText(TEXT("GuildText"), TEXT("Ticker_NotEnough_QuestCount"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}

				return;
			}
		}

		// 수락창 따로 표시하지 않기로 함
		FGsNetSendServiceQuest::SendReqQuestGuildAccept(_questId);
	}
	break;
	case EGsGuildQuestStateType::ACCEPTED:
	{
		// TEXT: 퀘스트를 포기하시겠습니까?
		FText textMsg;
		FText::FindText(TEXT("QuestUIText"), TEXT("ConfirmGiveUp"), textMsg);

		QuestId questIdReq = _questId;
		FGsUIHelper::PopupYesNo(textMsg, [questIdReq](bool bIsOk)
			{
				if (bIsOk)
				{
					FGsNetSendServiceQuest::SendReqQuestGuildGiveUp(questIdReq);
				}
			});
	}
	break;
	case EGsGuildQuestStateType::COMPLETED:
	{
		if (FGsGuildQuestDataHandler* questHandler = GGuild()->GetQuestHandler())
		{
			if (MAX_COMPLETE_QUEST_GUILD_COUNT <= questHandler->GetCompleteCount())
			{
				// TEXT: 퀘스트 수행 가능 횟수가 부족합니다.
				FText textMsg;
				if (FText::FindText(TEXT("GuildText"), TEXT("Ticker_NotEnough_QuestCount"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}

				return;
			}
		}

		if (FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler())
		{
			if (FGsQuestInstance* questInstance = questHandler->FindQuestInstance(QuestKey(_questId), _storyId))
			{
				questHandler->OpenPopupQuestReward(questInstance);
			}
		}
	}
	break;
	case EGsGuildQuestStateType::REWARDED:
	break;
	}
}

const FGsQuestObjectiveTableCache* UGsUIGuildQuestListEntry::GetObjectiveTable(StoryId InStroyId)
{
	TSharedPtr<FGsQuestData> questData;
	if (GSQuest()->GetGuildQuestDataList(InStroyId, questData))
	{
		TSharedPtr<FGsQuestTableCache> questTableCache = questData->GetQuestTableCache();
		if (questTableCache.IsValid())
		{
			TArray<TSharedPtr<FGsQuestObjectiveTableCache>> objectiveTableCacheList =
				questTableCache->GetObjectiveTableCache();
			if (0 < objectiveTableCacheList.Num())
			{
				return objectiveTableCacheList[0].Get();
			}
		}
	}

	return nullptr;
}

const FGsSchemaQuestGuildStory* UGsUIGuildQuestListEntry::GetStoryTable(StoryId InStroyId) const
{
	TSharedPtr<FGsQuestGuildStoryTableCache> cacheTable = GSQuest()->GetQuestGuildStoryTableCache(InStroyId);
	if (cacheTable.IsValid())
	{
		return cacheTable->_schemaStoryItem;
	}

	return nullptr;
}
