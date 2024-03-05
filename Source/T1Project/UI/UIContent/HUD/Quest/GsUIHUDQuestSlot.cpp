// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDQuestSlot.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Quest/GsQuestInstance.h"
#include "Quest/Table/GsQuestTableCache.h"
#include "Quest/GsSchemaQuest.h"
#include "Quest/Main/GsSchemaQuestMainStory.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

#include "TextBlock.h"
#include "WidgetSwitcher.h"
#include "VerticalBox.h"
#include "UserWidget.h"
#include "Image.h"
#include "Components/Border.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UI/UILib/Container/GsWidgetPool.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"

#include "GsUIHUDQuestObjectiveSlot.h"
#include "GameObject/Quest/GsQuestHandler.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UTIL/GsIntervalChecker.h"


void UGsUIHUDQuestSlot::NativeOnInitialized()
{	
	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_objectiveSlotClass, (UPanelWidget*)_progressingVerticalBox);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIHUDQuestSlot::OnRefreshEntry);	
	
	_slotBtn->OnClicked.AddDynamic(this, &UGsUIHUDQuestSlot::OnClickSlot);
	_teleportBtn->OnClicked.AddDynamic(this, &UGsUIHUDQuestSlot::OnClickTeleport);	

	//_windowQuestBtn->OnClicked.AddDynamic(this, &UGsUIHUDQuestSlot::OnClickWindowQuest);

	_checkerButton.SetIntervalSecond(0.3f);

	Super::NativeOnInitialized();
}

void UGsUIHUDQuestSlot::NativeConstruct()
{
	ClearEffect();

	Super::NativeConstruct();
}

void UGsUIHUDQuestSlot::NativeDestruct()
{
	_questInstance = nullptr;
	ClearEffect();
	_bTutorialHideTeleport = false;

	if (_delegateClickSlot.IsBound())
	{
		_delegateClickSlot.Unbind();
	}
	_checkerButton.Reset();

	Super::NativeDestruct();
}

void UGsUIHUDQuestSlot::ClearEffect()
{
	CompleteEffect(false);
	RenewalEffect(false);
	NewEffect(false);
	RewardEffect(false);
}

void UGsUIHUDQuestSlot::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIHUDQuestSlot::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIHUDQuestSlot::OnShow()
{	
	if (nullptr != _activeEffect)
	{
		_activeEffect->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIHUDQuestSlot::SetIsSelected(bool bInIsSelected)
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

bool UGsUIHUDQuestSlot::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIHUDQuestSlot::SetQuestCategoryStyle(QuestType inType)
{		
	if (nullptr != _questTypeSwicher)
	{
		_questTypeSwicher->SetActiveWidgetIndex((int32)inType);
	}
}

void UGsUIHUDQuestSlot::SetQuestComplete(bool inIsComplete)
{
	if (nullptr != _bgSwicher)
	{
		_bgSwicher->SetActiveWidgetIndex((inIsComplete)? 1 : 0);
	}
}

void UGsUIHUDQuestSlot::OnRefreshEntry(int32 InIndex, class UWidget* InEntry)
{
	if (nullptr == _questInstance)
		return;
	
	if (0 < _objectiveNum && InIndex <= _objectiveNum)
	{
		UGsUIHUDQuestObjectiveSlot* slot = Cast<UGsUIHUDQuestObjectiveSlot>(InEntry);
		if (slot)
		{
			FText descText;
			FText countText;
			bool isComplete;
			bool isReady;
			int currValue;
			int tableValue;

			_questInstance->GetObjectiveContentsText(InIndex, descText);

			if (_questInstance->GetObjectiveCountText(InIndex, isComplete, isReady, currValue, tableValue))
			{
				FText findText;
				FText::FindText(TEXT("QuestUIText"), TEXT("countQuest"), findText);

				// (5/10)
				countText = FText::Format(findText, currValue, tableValue);
			}
			else
			{
				countText = FText::FromString(TEXT(""));
			}

			slot->Update(isComplete, isReady, descText, countText);
		}
	}
}

void UGsUIHUDQuestSlot::OnClickSlot()
{	
	if (false == _checkerButton.IsTimeOver())
		return;

	if (nullptr == _questInstance)
		return;
	
	if (UGsQuestManager* questMgr = GSQuest())
	{
		if (FGsQuestHandler* questHandler = questMgr->GetQuestHandler())
		{
			FGsQuestInstance* questInstance = questHandler->GetQuestInstance(_storyId, _questKey);
			if (nullptr == questInstance)
				return;

			if (false == questInstance->GetTouchLock())
			{
				// close skill target select
				GSkill()->SetIsShowSkillTargetSelect(false);

				_checkerButton.UpdateTime();
				_delegateClickSlot.ExecuteIfBound(_index, (int32)_eState);
			}
		}
	}
}

void UGsUIHUDQuestSlot::OnClickTeleport()
{
	_delegateClickTeleport.ExecuteIfBound(_index);

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDQuestSlot::OnClickWindowQuest()
{

}

void UGsUIHUDQuestSlot::Update(FGsQuestInstance* inQuestInstance, int8 inIndex)
{
	if (nullptr == inQuestInstance)
		return;

	_questInstance = inQuestInstance;
	_index = inIndex;
	_storyId = inQuestInstance->GetStoryId();
	_questKey = inQuestInstance->GetQuestKey();

	// 타이틀 TEXT 셋팅
	FText titleText;
	_questInstance->GetTitleText(titleText);

#if WITH_EDITOR	
	if (GSQuest()->GetIsShowQuestId())
	{
		const QuestId questId = _questInstance->GetQuestId();
		const QuestIndex questIndex = _questInstance->GetQuestIndex();

		FString value;

		if(0 < questIndex)
		 value = FString::Format(TEXT("{0} {1} {2}"), { *titleText.ToString(), questId, questIndex});
		else
		 value = FString::Format(TEXT("{0} {1}"), { *titleText.ToString(), questId });

		_textTitle->SetText(FText::FromString(value));
	}
	else
	{
		_textTitle->SetText(titleText);
	}

	//_windowQuestBtn->SetVisibility(ESlateVisibility::Visible);
#else
	_textTitle->SetText(titleText);
	//_windowQuestBtn->SetVisibility(ESlateVisibility::Collapsed);
#endif

	// 퀘스트의 타입
	const QuestType questType = _questInstance->GetQuestType();
	// 타입에 따라 슬롯의 리소스가 다르다.
	SetQuestCategoryStyle(questType);
	SetTitleTextColor(questType, inQuestInstance->GetStoryId());	

	// 완료 UI 처리
	_questInstance->GetQuestUIState(_eState);
	SetQuestComplete((_eState == EGsQuestUIState::QUEST_STATE_REWARD) ? 1 : 0);
	SetVisibilityTeleportBtn(_questInstance->GetIsTeleportAble());

	bool isAutoMoving = GSQuest()->IsAutoProgressQuest(_questInstance->GetQuestKey());
	SetIsSelected(isAutoMoving);

	FText findText;
	switch (_eState)
	{
	case EGsQuestUIState::QUEST_STATE_READY:
	case EGsQuestUIState::QUEST_STATE_REWARD:
		break;
	case  EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED:
	{
		FText::FindText(TEXT("QuestUIText"), TEXT("lockedQuest"), findText);
		_textContent->SetText(FText::Format(findText, _questInstance->GetRequireLevel()));
	}
	break;
	case  EGsQuestUIState::QUEST_STATE_PROGRESS:
	{
		if (nullptr == _slotHelper)
			return;

		_slotHelper->RefreshAll(_objectiveNum = _questInstance->GetObjectiveNum());
	}
	break;
	case EGsQuestUIState::QUEST_STATE_INVADE_LOCKED:
	{
		FText::FindText(TEXT("QuestUIText"), TEXT("InvadeQuestCondition"), findText);
		_textContent->SetText(FText::Format(findText, _questInstance->GetRequireLevel()));
	}
	break;
	default:
		break;
	}

	// 퀘스트 진행 상태에 따른 ui 타입
	if (nullptr != _questStateSwicher)
	{
		RewardEffect(false);

		EGsQuestUIState eState = _eState;
		if (eState >= EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED)
		{
			eState = EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED;
		}
		_questStateSwicher->SetActiveWidgetIndex(static_cast<int>(eState));
	}

	EGsQuestUIEffectType effectType;
	_questInstance->GetQuestUIEffectType(effectType);
	RewardEffect(false);

	switch (effectType)
	{
	case EGsQuestUIEffectType::QUEST_UI_EFFECT_NONE:
		break;
	case EGsQuestUIEffectType::QUEST_UI_EFFECT_NEW:
		NewEffect(true);
		break;
	case EGsQuestUIEffectType::QUEST_UI_EFFECT_UPDATE:
		RenewalEffect(true);
		break;
	case EGsQuestUIEffectType::QUEST_UI_EFFECT_COMPLETED:
		_checkerButton.UpdateTime();		
		CompleteEffect(true);
		break;
	case EGsQuestUIEffectType::QUEST_UI_EFFECT_REWARD:
		NewEffect(false);
		RewardEffect(true);
		break;
	default:
		break;
	}
}

void UGsUIHUDQuestSlot::SetTitleTextColor(QuestType inType, StoryId inStoryId)
{
	const FLinearColor& linearColor = GSQuest()->GetTextColor(inType, inStoryId);
	if (_textTitle)
	{
		_textTitle->SetColorAndOpacity(linearColor);
	}

	if (inType == QuestType::REPEAT)
	{
		if (_questTypeSwicher)
		{
			UImage* img = Cast<UImage>(_questTypeSwicher->GetActiveWidget());
			img->SetColorAndOpacity(linearColor);
		}
	}	
}

void UGsUIHUDQuestSlot::SetVisibilityTeleportBtn(bool inIsTeleportAble)
{
	if (nullptr == _teleportBtn)
		return;

	if (true == inIsTeleportAble)
	{
		if (_bTutorialHideTeleport)
		{
			_teleportBtn->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			_teleportBtn->SetVisibility((_eState == EGsQuestUIState::QUEST_STATE_PROGRESS) ?
				ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
	}
	else
	{		
		_teleportBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIHUDQuestSlot::CompleteEffect(bool bInValue)
{
	if (nullptr != _completeEffect)
	{
		if (true == bInValue)
		{
			_completeEffect->ShowVFX();		
		}
		else
		{
			_completeEffect->HideVFX();
		}
	}
}

void UGsUIHUDQuestSlot::RenewalEffect(bool bInValue)
{
	if (nullptr != _renewalEffect)
	{
		if (true == bInValue)
		{
			_renewalEffect->ShowVFX();
		}
		else
		{
			_renewalEffect->HideVFX();
		}
	}
}

void UGsUIHUDQuestSlot::NewEffect(bool bInValue)
{
	if (nullptr != _newEffect)
	{
		if (true == bInValue)
		{
			_newEffect->ShowVFX();
		}
		else
		{
			_newEffect->HideVFX();
		}
	}
}

void UGsUIHUDQuestSlot::RewardEffect(bool bInValue)
{
	if (nullptr != _rewardEffect)
	{
		if (true == bInValue)
		{
			_rewardEffect->ShowVFX();
		}
		else
		{
			_rewardEffect->HideVFX();
		}
	}
}

void UGsUIHUDQuestSlot::SetTutorialHideTeleport(bool bHideTeleport)
{
	_bTutorialHideTeleport = bHideTeleport;

	if (_questInstance)
	{
		SetVisibilityTeleportBtn(_questInstance->GetIsTeleportAble());
	}
}
