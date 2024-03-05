// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Quest/Guide/GsUIWindowQuestGuideEntry.h"

#include "Management/ScopeGlobal/GsQuestManager.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

#include "UI/UIContent/Window/Quest/GsUIQuestSlotHelper.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "GsSchemaEnums.h"

#include "Engine/AssetManager.h"

#include "TreeView.h"
#include "WidgetSwitcher.h"
#include "Text.h"
#include "Image.h"

#include "Option/GsServerOption.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "GsUIWindowQuestGuideItem.h"
#include "Quest/Table/GsQuestGuideTableCacheSet.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Guide/GsSchemaQuestGuideStory.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageContents.h"
#include "Management/GsMessageHolder.h"


void UGsUIWindowQuestGuideEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	_item = Cast<UGsUIWindowQuestGuideItem>(ListItemObject);
	if (_item.IsValid())
	{
		_storyId =  _item->GetStoryId();
		_slotIndex = _item->GetSlotIndex();
	}

	SetData();

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UGsUIWindowQuestGuideEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}

void UGsUIWindowQuestGuideEntry::OnClickHudUIOnOffBtn()
{/*
	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
		return;*/

	if (false == _bIsHudUIOn)
	{
		if (false == GSQuest()->IsGuideQuestHudOn())
		{
			FText findText;
			FText::FindText(TEXT("QuestUIText"), TEXT("UI_Ticker_Guide_NoMoreShow"), findText);

			FGsUIHelper::TrayMessageTicker(findText);

			return;
		}
	}
	
	_bIsHudUIOn = !_bIsHudUIOn;
	_switcherHudOnOff->SetActiveWidgetIndex(_bIsHudUIOn ? 0 : 1);
	_unSwitcherHudOnOff->SetActiveWidgetIndex(_bIsHudUIOn ? 0 : 1);

	if (_item.IsValid())
	{
		if (_item->_delegateClickHudUIOnOffBtn.IsBound())
		{
			_item->_delegateClickHudUIOnOffBtn.ExecuteIfBound(_slotIndex, _bIsHudUIOn);
		}
	}

	FGsQuestSlotTouchParam param(QuestKey::KeyNone(), _bIsHudUIOn, _storyId);
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_GUIDE_HUD_ONOFF, &param);
	//serverOption->SetQuestGuideUIVisibility(_bIsHudUIOn, _storyId);
}

bool UGsUIWindowQuestGuideEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIWindowQuestGuideEntry::SetRedDot(bool bInIsOn)
{
	_redDot->SetRedDot(bInIsOn, EGsIconRedDotType::CHECK);
}

void UGsUIWindowQuestGuideEntry::UpdateRedDot()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	QuestState outQuestState = QuestState::NONE;
	QuestWindowUISlotType outSlotType = QuestWindowUISlotType::STORY_PROGRESS;
	questManager->GetGuideQuestSlotStateInfo(_storyId, outQuestState, outSlotType);

	SetRedDot((outQuestState == QuestState::COMPLETED)? true : false);
}

void UGsUIWindowQuestGuideEntry::UpdateHudOnOff()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	_bIsHudUIOn = false;
	if (FGsQuestGuideManagement* guideManagement = questManager->GetQuestGuideManagement())
	{
		_bIsHudUIOn = guideManagement->IsShow(_storyId);
	}
	_switcherHudOnOff->SetActiveWidgetIndex(_bIsHudUIOn ? 0 : 1);
	_unSwitcherHudOnOff->SetActiveWidgetIndex(_bIsHudUIOn ? 0 : 1);
}

void UGsUIWindowQuestGuideEntry::SetData()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache = questManager->GetGuideStoryTableCache(_storyId);
	if (!storyTableCache.IsValid())
		return;

	const FGsSchemaQuestGuideStory* schemaQuestGuideStory = storyTableCache->GetSchemaQuestStory();
	if (nullptr == schemaQuestGuideStory)
		return;

	// 수행가능 레벨
	_requireLevel = schemaQuestGuideStory->requireLevel;
	// 스토리 제목
	_storyText = schemaQuestGuideStory->storyTitleText;
	// 선택 이미지는 일단 끄기
	_selectImg->SetVisibility(ESlateVisibility::Hidden);
	// 레드닷도 처음엔 끄기
	_redDot->SetRedDot(false, EGsIconRedDotType::NORMAL);
}

void UGsUIWindowQuestGuideEntry::SetDataStory(int inSlotIndex, StoryId inStoryId)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	QuestState outQuestState = QuestState::NONE;
	QuestWindowUISlotType outSlotType = QuestWindowUISlotType::STORY_PROGRESS;
	questManager->GetGuideQuestSlotStateInfo(inStoryId, outQuestState, outSlotType);

	/*
	* 0 : 잠금
	* 1 : 진행
	* 2 : 완료
	*/
	int swichIndex = 0;	
	if (outSlotType == QuestWindowUISlotType::STORY_LOCKED_EQUIRE_LEVEL ||
		outSlotType == QuestWindowUISlotType::STORY_LOCKED_MAIN_STORY ||
		outSlotType == QuestWindowUISlotType::STORY_LOCKED)
	{
		_swichIndex = 0;
	}
	else if (outSlotType == QuestWindowUISlotType::STORY_REWARDED)
	{
		_swichIndex = 2;
	}
	else
	{
		_swichIndex = 1;
	}

	// 슬롯 결정
	_switcherSlot->SetActiveWidgetIndex(_swichIndex);

	if (outSlotType == QuestWindowUISlotType::STORY_LOCKED_EQUIRE_LEVEL ||
		outSlotType == QuestWindowUISlotType::STORY_LOCKED_MAIN_STORY ||
		outSlotType == QuestWindowUISlotType::STORY_LOCKED)
	{
		FText lockText = FText::GetEmpty();
		if (outSlotType == QuestWindowUISlotType::STORY_LOCKED_EQUIRE_LEVEL)
		{
			FText findText;
			FText::FindText(TEXT("UICommonText"), TEXT("LevelShort"), findText);

			FString value = FString::Format(TEXT("{0}{1}"), { *findText.ToString(), _requireLevel });
			lockText = FText::FromString(value);
		}
		else if (outSlotType == QuestWindowUISlotType::STORY_LOCKED_MAIN_STORY)
		{
			FText::FindText(TEXT("UICommonText"), TEXT("HUD_Quest"), lockText);
		}
		
		_levelLimitText = lockText;
		_bIsLock = true;
	}
	else
	{
		_bIsHudUIOn = false;
		if (FGsQuestGuideManagement* guideManagement = GSQuest()->GetQuestGuideManagement())
		{
			_bIsHudUIOn = guideManagement->IsShow(inStoryId);
		}
		_switcherHudOnOff->SetActiveWidgetIndex(_bIsHudUIOn ? 0 : 1);	
		_unSwitcherHudOnOff->SetActiveWidgetIndex(_bIsHudUIOn ? 0 : 1);
	}

	// 보상가능하면 레드닷 온
	SetRedDot(outQuestState == QuestState::COMPLETED ? true : false);
}

void UGsUIWindowQuestGuideEntry::SetSelect(bool inIsSelect)
{
	_bIsSelected = inIsSelect;

	if (_bIsSelected)
	{
		_switcherSlot->SetActiveWidgetIndex(_swichIndex + SLOT_STATE_COUNT);
		_switcherGuideIcon->SetActiveWidgetIndex(static_cast<int32>(true));
	}
	else
	{
		_switcherSlot->SetActiveWidgetIndex(_swichIndex);
		_switcherGuideIcon->SetActiveWidgetIndex(static_cast<int32>(false));
	}

	if (nullptr != _selectImg)
	{
		_selectImg->SetVisibility((_bIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}