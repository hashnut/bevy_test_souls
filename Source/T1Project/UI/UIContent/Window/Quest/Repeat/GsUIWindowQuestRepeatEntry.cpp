// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Quest/Repeat/GsUIWindowQuestRepeatEntry.h"

#include "Quest/Table/GsQuestMainTableCacheSet.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"


#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "GsSchemaEnums.h"

#include "Engine/AssetManager.h"

#include "TextBlock.h"
#include "TreeView.h"
#include "WidgetSwitcher.h"
#include "Text.h"
#include "Image.h"
#include "SlateBrush.h"
#include "PaperSprite.h"
#include "CanvasPanel.h"
#include "Color.h"

#include "Option/GsServerOption.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Window/Quest/GsUIWindowQuestTitleItem.h"
#include "UI/UIContent/Window/Quest/GsUIQuestSlotHelper.h"
#include "GsUIWindowQuestRepeatItem.h"
#include "Quest/Table/GsQuestRepeatTableCacheSet.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Repeat/GsSchemaQuestRepeatStory.h"
#include "Quest/GsQuestData.h"
#include "Runtime/DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

void UGsUIWindowQuestRepeatEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_sBtnUp->OnClicked.AddDynamic(this, &UGsUIWindowQuestRepeatEntry::OnClickUp);
	_sBtnDown->OnClicked.AddDynamic(this, &UGsUIWindowQuestRepeatEntry::OnClickDown);
	_sBtnSelectUp->OnClicked.AddDynamic(this, &UGsUIWindowQuestRepeatEntry::OnClickUp);
	_sBtnSelectDown->OnClicked.AddDynamic(this, &UGsUIWindowQuestRepeatEntry::OnClickDown);
}

void UGsUIWindowQuestRepeatEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	_item = Cast<UGsUIWindowQuestRepeatItem>(ListItemObject);
	if (_item.IsValid())
	{
		_storyId = _item->GetStoryId();
		_questKey = _item->GetQuestKey();
	}

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UGsUIWindowQuestRepeatEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}

void UGsUIWindowQuestRepeatEntry::OnClickHudUIOnOffBtn()
{
	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
		return;

	if (false == _bIsHudUIOn)
	{
		int activeQuestNum = serverOption->GetQuestRepeatUIVisibilityCount();
		if (activeQuestNum >= MAX_ACTIVE_QUEST_REPEAT_COUNT)
		{
			FText findText;
			FText::FindText(TEXT("QuestUIText"), TEXT("UI_Ticker_NoMoreShow"), findText);

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
			_item->_delegateClickHudUIOnOffBtn.ExecuteIfBound(GetSlotIndex(), _bIsHudUIOn);
		}
	}

	serverOption->SetQuestRepeatUIVisibility(_bIsHudUIOn, _questKey._questIndex);

	//업다운버튼 셋팅
	SetViewUpdDownButtons();
}	

bool UGsUIWindowQuestRepeatEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIWindowQuestRepeatEntry::SetRedDot(bool bInIsOn)
{
	_bIsRedDot = bInIsOn;
	_redDot->SetRedDot(bInIsOn, EGsIconRedDotType::CHECK);
}

void UGsUIWindowQuestRepeatEntry::ReSetDataStory(StoryId inStoryId, const QuestKey& inQuestKey)
{
	if (_item.IsValid())
	{
		_item->ReSetQuestItem(inStoryId, inQuestKey);
	}
	
	SetDataStory(inStoryId, inQuestKey);
}

void UGsUIWindowQuestRepeatEntry::SetDataStory(StoryId inStoryId, const QuestKey& inQuestKey)
{	
	_storyId = inStoryId;
	_questKey = inQuestKey;
	
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

	_repeatStoryType = questManager->GetRepeatStoryType(_storyId);
	_questState = QuestState::ACCEPTED;
	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(inQuestKey._questIndex);
	if (nullptr != dynamicData)
	{
		_questState = dynamicData->GetQuestState();
	}

	SetRedDot((_questState == QuestState::COMPLETED) ? true : false);
	_selectImg->SetVisibility(ESlateVisibility::Hidden);

	_storyText->SetColorAndOpacity(questManager->GetTextColor(QuestType::REPEAT, inStoryId));
	_storyText->SetText(schemaQuestStory->storyTitleText);
	_unStoryText->SetColorAndOpacity(questManager->GetTextColor(QuestType::REPEAT, inStoryId));
	_unStoryText->SetText(schemaQuestStory->storyTitleText);

	if (schemaQuestStory->repeatStoryType == RepeatStoryType::SCROLL)
	{
		FText findText;
		FText::FindText(TEXT("ItemCategoryDisplayNameText"), TEXT("QUEST_REPEAT"), findText);

		_typeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_typeText->SetText(findText);

		_unTypeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_typeText->SetText(findText);
	}
	else
	{
		_typeText->SetVisibility(ESlateVisibility::Collapsed);
		_unTypeText->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (const FGsSchemaQuestImg* questImg = schemaQuestStory->iconRes.GetRow())
	{
		if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(questImg->resPath))
		{
			if (UPaperSprite* sprite = Cast<UPaperSprite>(loadObject))
			{
				FSlateAtlasData atlasData = sprite->GetSlateAtlasData();
				FVector2D size = atlasData.GetSourceDimensions();

				// 선택 이미지 셋팅
				_storyTypeImg->SetBrushFromAtlasInterface(sprite);
				_storyTypeImg->SetBrushSize(size);

				// 비선택 이미지 셋팅
				_unStoryTypeImg->SetBrushFromAtlasInterface(sprite);
				_unStoryTypeImg->SetBrushSize(size);
			}
		}
	}

	_bIsHudUIOn = false;
	if (FGsServerOption* serverOption = GServerOption())
	{
		_bIsHudUIOn = (true == serverOption->IsHideQuestRepeatUI(inQuestKey._questIndex)) ? false : true;
	}

	_switcherHudOnOff->SetActiveWidgetIndex(_bIsHudUIOn ? 0 : 1);
	_unSwitcherHudOnOff->SetActiveWidgetIndex(_bIsHudUIOn ? 0 : 1);

	//업다운버튼 셋팅
	SetViewUpdDownButtons();
}

void UGsUIWindowQuestRepeatEntry::SetViewUpdDownButtons()
{
	bool isViewUpdDownButtons = false;	
	if (RepeatStoryType::BOARD == _repeatStoryType && true == _bIsHudUIOn && QuestState::ACCEPTED == _questState)
	{
		isViewUpdDownButtons = true;
	}

	_updownWidget->SetVisibility(isViewUpdDownButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_updownSelectWidget->SetVisibility(isViewUpdDownButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (isViewUpdDownButtons)
	{
		_sBtnUp->SetSwitcherIndex(0);
		_sBtnSelectUp->SetSwitcherIndex(0);
		_sBtnDown->SetSwitcherIndex(0);
		_sBtnSelectDown->SetSwitcherIndex(0);

		if (_item->IsFirst())
		{
			_sBtnUp->SetSwitcherIndex(1);
			_sBtnSelectUp->SetSwitcherIndex(1);
		}

		if (_item->IsLast())
		{
			_sBtnDown->SetSwitcherIndex(1);
			_sBtnSelectDown->SetSwitcherIndex(1);
		}
	}
}

void UGsUIWindowQuestRepeatEntry::SetSelect(bool inIsSelect)
{
	_bIsSelected = inIsSelect;

	if (nullptr != _selectImg)
	{
		_selectImg->SetVisibility((_bIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}

	_switcherSlot->SetActiveWidgetIndex(static_cast<int32>(_bIsSelected));
}

void UGsUIWindowQuestRepeatEntry::UpdateRedDot()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	if (_questKey.IsKeyNone())
		return;

	TSharedPtr<FGsQuestData> questData = questManager->GetQuestData(_questKey);
	if (!questData.IsValid())
		return;

	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(_questKey._questIndex);
	if (nullptr == dynamicData)
		return;

	_questState = dynamicData->GetQuestState();
	SetRedDot((_questState == QuestState::COMPLETED) ? true : false);

	//업다운버튼 셋팅
	SetViewUpdDownButtons();
}

void UGsUIWindowQuestRepeatEntry::OnClickUp()
{
	if (_item->IsFirst())
	{
		return;
	}

	FGsUIHelper::ShowBlockUI();
	FGsUIHelper::SetBlockUISetting(FText::FromString(TEXT("....")), false, 0.3f);

	if (_item.IsValid())
	{
		if (_item->_delegateClickHudUIUpBtn.IsBound())
		{
			_item->_delegateClickHudUIUpBtn.ExecuteIfBound(GetSlotIndex(), static_cast<int32>(true));
		}
	}
}

void UGsUIWindowQuestRepeatEntry::OnClickDown()
{
	if (_item->IsLast())
	{
		return;
	}

	FGsUIHelper::ShowBlockUI();
	FGsUIHelper::SetBlockUISetting(FText::FromString(TEXT("....")), false, 0.3f);

	if (_item.IsValid())
	{
		if (_item->_delegateClickHudUIDownBtn.IsBound())
		{
			_item->_delegateClickHudUIDownBtn.ExecuteIfBound(GetSlotIndex(), static_cast<int32>(false));
		}
	}
}

int8 UGsUIWindowQuestRepeatEntry::GetSlotIndex()
{
	return (_item == nullptr) ? 0 : _item->GetSlotIndex();
}