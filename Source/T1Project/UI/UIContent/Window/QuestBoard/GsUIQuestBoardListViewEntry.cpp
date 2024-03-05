#include "GsUIQuestBoardListViewEntry.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/QuestBoard/GsUIWindowQuestBoard.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateQuestBoard.h"

#include "DataSchema/Quest/Repeat/GsSchemaQuestRepeatStory.h"
#include "Quest/Table/GsQuestRepeatTableCacheSet.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"

void UGsQuestBoardListViewEntryData::Initialize(TSharedPtr<FGsQuestBoardData>& inPair, UGsUIWindowQuestBoard* inParent)
{
	_questboardData = inPair;
	_parent = inParent;
}

void UGsUIQuestBoardListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_selectButton->OnClicked.AddDynamic(this, &UGsUIQuestBoardListViewEntry::OnClickQuestBoard);
}

void UGsUIQuestBoardListViewEntry::NativeConstruct()
{
	Super::NativeConstruct();

	if (FGsMessageHolder* msg = GMessage())
	{
		_uiDelegateArray.Emplace(msg->GetUI().AddUObject(MessageUI::SELECT_QUEST_BOARD
			, this, &UGsUIQuestBoardListViewEntry::OnSelect));		
		_uiDelegateArray.Emplace(msg->GetUI().AddUObject(MessageUI::INVALID_QUEST_BOARD
			, this, &UGsUIQuestBoardListViewEntry::OnInvalidate));				
	}
}

void UGsUIQuestBoardListViewEntry::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_uiDelegateArray.Num() > 0)
		{
			for (auto iter : _uiDelegateArray)
			{
				msg->GetUI().Remove(iter);
			}
			_uiDelegateArray.Empty();
		}

		if (_msgGameObjectHandleList.Num() > 0)
		{
			for (auto iter : _msgGameObjectHandleList)
			{
				msg->GetGameObject().Remove(iter);
			}
			_msgGameObjectHandleList.Empty();
		}
	}

	Super::NativeDestruct();
}

void UGsUIQuestBoardListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsQuestBoardListViewEntryData* data = Cast<UGsQuestBoardListViewEntryData>(inListItemObject);
	if (nullptr == data)
	{
		return;
	}

	_questBoardData = data->_questboardData;

	InvalidTitle();
	InvalidSelect();
	InvalidState();
}

void UGsUIQuestBoardListViewEntry::OnClickQuestBoard()
{
	if (false == _questBoardData.IsValid())
	{
		return;
	}

	FGsGameStateQuestBoard::GetInstance()->SetSelectQuest(_questBoardData.Pin()->GetQuestStoryId());

	FGsUIMsgParamSelectQuestBoard param(_questBoardData.Pin()->GetQuestStoryId());
	GMessage()->GetUI().SendMessage(MessageUI::SELECT_QUEST_BOARD, &param);
}

void UGsUIQuestBoardListViewEntry::OnSelect(const IGsMessageParam* inParam)
{	
	InvalidSelect();	
}

void UGsUIQuestBoardListViewEntry::OnInvalidate(const struct IGsMessageParam* inParam)
{
	/*const QuestStoryIdPair& quest = FGsGameStateQuestBoard::GetInstance()->GetSelectQuest();
	if (quest == _quest)
	{		*/
		InvalidState();
	//}	
}

void UGsUIQuestBoardListViewEntry::InvalidTitle()
{
	if (false == _questBoardData.IsValid())
	{
		return;
	}

	TSharedPtr<FGsQuestRepeatStoryTableCache> storyTableCache = GSQuest()->GetQuestRepeatStoryTableCache(_questBoardData.Pin()->GetQuestStoryId().mStoryId);
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

	_questTitleTextBlock->SetText(schemaQuestRepeatStory->storyTitleText);

	if (UGsQuestManager* questManager = GSQuest())
	{
		UPaperSprite* icon = questManager->GetRepeatQuestGradeIcon(_questBoardData.Pin()->GetQuestStoryId());
		if (icon)
		{
			_questIcon->SetBrushFromAtlasInterface(icon);
		}

		FLinearColor gradeColor = questManager->GetQuestGradeColor(_questBoardData.Pin()->GetQuestStoryId());
		_questTitleTextBlock->SetColorAndOpacity(gradeColor);
	}	
}

void UGsUIQuestBoardListViewEntry::InvalidSelect()
{
	const QuestStoryIdPair& quest = FGsGameStateQuestBoard::GetInstance()->GetSelectQuest();
	if (_questBoardData.IsValid()
		&& _questBoardData.Pin()->IsEquals(quest))
	{
		_selectImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_selectImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIQuestBoardListViewEntry::InvalidState()
{	
	if (false == _questBoardData.IsValid())
	{
		_questStateWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	TSharedPtr<FGsQuestData> questData = GSQuest()->GetQuestData(_questBoardData.Pin()->GetQuestStoryId().mQuestId);
	if (false == questData.IsValid())
	{
		_questStateWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	if (GSQuest()->IsProgress(_questBoardData.Pin()->GetQuestStoryId().mQuestId, _questBoardData.Pin()->GetQuestStoryId().mQuestIndex))
	{
		FGsQuestDynamicData* dynmaicData = questData.Get()->GetQuestDynamicData(_questBoardData.Pin()->GetQuestStoryId().mQuestIndex);
		if (nullptr == dynmaicData)
		{
			_questStateWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		switch (dynmaicData->GetQuestState())
		{
		case QuestState::COMPLETED:
		{
			_questStateWidgetSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_questStateWidgetSwitcher->SetActiveWidgetIndex(1);
		}
		break;
		case QuestState::ACCEPTED:
		{
			_questStateWidgetSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_questStateWidgetSwitcher->SetActiveWidgetIndex(0);
		}
		break;
		default:
		{
			_questStateWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
		}
		break;
		}
	}
	else
	{
		_questStateWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
	}
}