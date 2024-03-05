#include "GsUIQuestBoardDetailPanel.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Image.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "UI/UIContent/Common/Reward/GsUIRewardInfoBase.h"
#include "Quest/Table/GsQuestRepeatTableCacheSet.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageGameObject.h"
#include "Reward/GsRewardHelper.h"
#include "DataSchema/Quest/Repeat/GsSchemaQuestRepeatStory.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/Quest/GsSchemaQuest.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateQuestBoard.h"

void UGsUIQuestBoardDetailPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIQuestBoardDetailPanel::NativeConstruct()
{
	Super::NativeConstruct();

	_allQuestObjectCount = 0;

	if (FGsMessageHolder* msg = GMessage())
	{
		MGameObject& msgGameObject = msg->GetGameObject();
		_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::QUEST_WINDOW_UPDATE_OBJECTIVE,
		this, &UGsUIQuestBoardDetailPanel::OnInvalidObject));

		_uiDelegateArray.Emplace(msg->GetUI().AddUObject(MessageUI::SELECT_QUEST_BOARD, this, &UGsUIQuestBoardDetailPanel::OnSelect));
	}
}

void UGsUIQuestBoardDetailPanel::NativeDestruct()
{	
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_msgGameObjectHandleList.Num() > 0)
		{
			for (auto iter : _msgGameObjectHandleList)
			{
				msg->GetGameObject().Remove(iter);
			}
			_msgGameObjectHandleList.Empty();
		}

		if (_uiDelegateArray.Num() > 0)
		{
			for (auto iter : _uiDelegateArray)
			{
				msg->GetUI().Remove(iter);
			}
			_uiDelegateArray.Empty();
		}
	}

	Super::NativeDestruct();
}

void UGsUIQuestBoardDetailPanel::Invalidate(const QuestStoryIdPair& inQuest)
{
	InvalidReward(inQuest);

	_selectedQuest = inQuest;
	TSharedPtr<FGsQuestRepeatStoryTableCache> storyTableCache = GSQuest()->GetQuestRepeatStoryTableCache(_selectedQuest.mStoryId);
	if (false == storyTableCache.IsValid())
	{
		return;
	}
	_storyTable = storyTableCache;

	const FGsSchemaQuestRepeatStory* schemaQuestRepeatStory = _storyTable.Pin()->GetSchemaQuestStory();
	if (nullptr == schemaQuestRepeatStory)
	{
		return;
	}				

	InvalidQuestInfo();
	InvalidQuestObject();
	InvalidReward(inQuest);
}

bool UGsUIQuestBoardDetailPanel::IsNeedInvalid(const QuestStoryIdPair& inQuest)
{
	if (0 >= inQuest.mQuestId)
		return false;

	int32 questCount = GSQuest()->GetCurrentAllObjectCount(inQuest.mQuestId);	
	bool isSame = (_selectedQuest == inQuest);
	return (!isSame) || (questCount != _allQuestObjectCount);
}

void UGsUIQuestBoardDetailPanel::InvalidQuestInfo()
{
	if (nullptr == _storyTable || !_storyTable.IsValid())
	{
		return;
	}

	const FGsSchemaQuestRepeatStory* schemaQuestRepeatStory = _storyTable.Pin()->GetSchemaQuestStory();
	if (nullptr == schemaQuestRepeatStory)
	{
		return;
	}

	if (UGsQuestManager* questManager = GSQuest())
	{
		UPaperSprite* icon = questManager->GetRepeatQuestGradeIcon(_selectedQuest);
		if (icon)
		{
			_questIcon->SetBrushFromAtlasInterface(icon);
		}

		FLinearColor gradeColor = questManager->GetQuestGradeColor(_selectedQuest);
		_questTitleTextBlock->SetColorAndOpacity(gradeColor);
	}	

	_questTitleTextBlock->SetText(schemaQuestRepeatStory->storyTitleText);
	_questDescriptionTextBlock->SetText(schemaQuestRepeatStory->storyDescriptionText);
}

void UGsUIQuestBoardDetailPanel::InvalidQuestObject()
{
	_allQuestObjectCount = GSQuest()->GetCurrentAllObjectCount(_selectedQuest.mQuestId);
	FString questObject = GSQuest()->GetQuestObjectText(_selectedQuest.mStoryId, _selectedQuest.mQuestId, _selectedQuest.mQuestIndex);
	_questTargetTextBlock->SetText(FText::FromString(questObject));
}

void UGsUIQuestBoardDetailPanel::InvalidReward(const QuestStoryIdPair& inQuest)
{
	if (nullptr == _storyTable || !_storyTable.IsValid())
	{
		return;
	}

	const FGsSchemaQuestRepeatStory* schemaQuestRepeatStory = _storyTable.Pin()->GetSchemaQuestStory();
	if (nullptr == schemaQuestRepeatStory)
	{
		return;
	}

	TSharedPtr<FGsQuestBoardData> data = FGsGameStateQuestBoard::GetInstance()->GetQuestData(inQuest);
	if (false == data.IsValid())
	{
		return;
	}

	int32 mainRewardId = 0;
	if (const FGsSchemaRewardData* schemaRewardData = schemaQuestRepeatStory->mainRewardId.GetRow())
	{
		mainRewardId = schemaRewardData->id;
	}		

	int32 subRewardId = 0;
	if (const FGsSchemaRewardData* schemaRewardData = schemaQuestRepeatStory->subRewardId.GetRow())
	{
		subRewardId = schemaRewardData->id;
	}		

	_rewardInfo->SetData(mainRewardId, subRewardId, data->GetRewardArray());
}

void UGsUIQuestBoardDetailPanel::OnInvalidObject(const struct IGsMessageParam* inParam)
{
	InvalidQuestObject();
}

void UGsUIQuestBoardDetailPanel::OnSelect(const struct IGsMessageParam* inParam)
{
	QuestStoryIdPair selectQuest = FGsGameStateQuestBoard::GetInstance()->GetSelectQuest();
	Invalidate(selectQuest);
}
