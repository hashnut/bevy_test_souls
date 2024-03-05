// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDTestQuestSlot.h"
#include "TextBlock.h"
#include "WidgetSwitcher.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "UI/UILib/Container/GsWidgetPool.h"
#include "VerticalBox.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "Border.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "../../Helper/GsUIColorHelper.h"
#include "../../Define/GsUIContentDefine.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Image.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

void UGsUIHUDTestQuestSlot::NativeOnInitialized()
{	
	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_objectiveSlotClass, (UPanelWidget*)_progressingWrapBox);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIHUDTestQuestSlot::OnRefreshEntry);
	
	_slotBtn->OnClicked.AddDynamic(this, &UGsUIHUDTestQuestSlot::OnClickSlot);
	_teleportBtn->OnClicked.AddDynamic(this, &UGsUIHUDTestQuestSlot::OnClickTeleport);

	Super::NativeOnInitialized();
}

void UGsUIHUDTestQuestSlot::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIHUDTestQuestSlot::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIHUDTestQuestSlot::OnShow()
{
	if (nullptr != _activeEffect)
	{
		_activeEffect->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIHUDTestQuestSlot::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	if (nullptr != _activeEffect)
	{
		_activeEffect->SetVisibility((bInIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}

	if (nullptr != _touchEffect)
	{
		_touchEffect->SetVisibility((bInIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

		if (true == bInIsSelected)
		{
			_touchEffect->ShowVFX();
		}
	}
}

bool UGsUIHUDTestQuestSlot::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIHUDTestQuestSlot::SetQuestCategoryStyle(QuestType inType)
{		
	if (nullptr != _questTypeSwicher)
	{
		_questTypeSwicher->SetActiveWidgetIndex((int32)inType);
	}
}

void UGsUIHUDTestQuestSlot::SetQuestComplete(bool inIsComplete)
{
	if (nullptr != _bgSwicher)
	{
		_bgSwicher->SetActiveWidgetIndex((inIsComplete)? 1 : 0);
	}
}

void UGsUIHUDTestQuestSlot::OnRefreshEntry(int32 InIndex, class UWidget* InEntry)
{
	/*if (0 < _objectiveNum && InIndex <= _objectiveNum)
	{
		UGsUIHUDQuestObjectiveSlot* slot = Cast<UGsUIHUDQuestObjectiveSlot>(InEntry);
		if (slot)
		{
			FText descText;
			FText countText;
			bool isComplete;
			int currValue;
			int tableValue;*/


			//if (_questInstance->GetObjectiveCountText(InIndex, isComplete, currValue, tableValue))
			//{
			//	FText findText;
			//	FText::FindText(TEXT("QuestUIText"), TEXT("countQuest"), findText);

			//	// (5/10)
			//	countText = FText::Format(findText, currValue, tableValue);
			//}
			//else
			//{
			//	countText = FText::FromString(TEXT(""));
			//}

			//slot->Update(isComplete, descText, countText);
		//}
	//}
}

void UGsUIHUDTestQuestSlot::OnClickSlot()
{	
	_delegateClickSlot.ExecuteIfBound(this);
}

void UGsUIHUDTestQuestSlot::OnClickTeleport()
{
	_delegateClickTeleport.ExecuteIfBound(this);
}

void UGsUIHUDTestQuestSlot::SetTitleText(FText inText)
{
	_textTitle->SetText(inText);
}

FText& UGsUIHUDTestQuestSlot::GetTitleText()
{ 
	return _textTitle->Text;
}

void UGsUIHUDTestQuestSlot::Update(int8 inIndex)
{
	_index = inIndex;

	// 타이틀 TEXT 셋팅

	// 타입에 따라 슬롯의 리소스가 다르다.
	SetQuestCategoryStyle(QuestType::MAIN);
	SetTitleTextColor();	

	// 완료 UI 처리	
	SetQuestComplete(0);	
	SetVisibilityTeleportBtn(true);
	
	// 퀘스트 진행 상태에 따른 ui 타입
	if (nullptr != _questStateSwicher)
	{
		_questStateSwicher->SetActiveWidgetIndex((int32)EGsQuestUIState::QUEST_STATE_READY);
	}
	//FText findText;
	//switch (_questInstance->GetQuestUIState())
	//{
	//case EGsQuestUIState::QUEST_STATE_READY:
	//case EGsQuestUIState::QUEST_STATE_COMPLETED:
	//	break;
	//case  EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED:
	//{
	//	FText::FindText(TEXT("QuestUIText"), TEXT("lockedQuest"), findText);
	//	_textContent->SetText(FText::Format(findText, _questInstance->GetRequireLevel()));
	//}
	//break;
	//case  EGsQuestUIState::QUEST_STATE_PROGRESS:
	//{
	//	if (nullptr == _slotHelper)
	//		return;

	//	_slotHelper->RefreshAll(_objectiveNum = _questInstance->GetObjectiveNum());
	//}
	//break;
	//default:
	//	break;
	//}
}

void UGsUIHUDTestQuestSlot::SetTitleTextColor()
{
	FLinearColor linearColor = FLinearColor::Green;
	if (_textTitle)
	{
		_textTitle->SetColorAndOpacity(linearColor);
	}

	/*if (inType == QuestType::REPEAT)
	{
		if (_questTypeSwicher)
		{
			UImage* img = Cast<UImage>(_questTypeSwicher->GetActiveWidget());
			img->SetColorAndOpacity(linearColor);
		}
	}	*/
}

void UGsUIHUDTestQuestSlot::SetVisibilityTeleportBtn(bool inIsTeleportAble)
{
	if (nullptr == _teleportBtn)
		return;

	if (true == inIsTeleportAble)
	{
		_teleportBtn->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{		
		_teleportBtn->SetVisibility(ESlateVisibility::Hidden);		
	}
}