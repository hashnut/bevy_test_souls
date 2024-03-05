// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDAuto.h"
#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsSkillManager.h"

#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "UMG/Public/Components/WidgetSwitcher.h"


void UGsUIHUDAuto::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_switcher->SetActiveWidgetIndex(0);
}

// 버튼 클릭
void UGsUIHUDAuto::OnClickAutoButton()
{
	ClickedAutoButton();
}
void UGsUIHUDAuto::ClickedAutoButton()
{
	if (2 == _switcher->GetActiveWidgetIndex())
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_BUTTON_CLICK, nullptr);
		if (GSAI()->IsAIOn())
		{
			GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_ACTIVE_TOGGLE, nullptr);
		}
	}
	else
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_ACTIVE_TOGGLE, nullptr);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}
void UGsUIHUDAuto::InvalidateAllInternal()
{
	SetOnOffState(GSAI()->IsAIOn());
	SetQuestAutoState(GSQuest()->IsAutoProgressing());
}

// 상태 바꾸기
void UGsUIHUDAuto::SetOnOffState(bool In_isOn)
{
	if (_switcher == nullptr)
	{
		return;
	}

	if (2 == _switcher->GetActiveWidgetIndex())
	{
		return;
	}

	int index = (In_isOn == true) ? 1 : 0;
	_switcher->SetActiveWidgetIndex(index);

	if (index == 1)
	{
		OnPlayAni();
	}
}

// 클릭 테스트
void UGsUIHUDAuto::TestClick()
{
	if (_btnAuto == nullptr)
	{
		return;
	}

#ifdef TEST_CLICK_OUT
	_btnAuto->SlateHandleClickedCustomized();
#endif
}


void UGsUIHUDAuto::SetQuestAutoState(bool In_isOn)
{
	if (_switcher == nullptr)
	{
		return;
	}

	int index = (In_isOn == true) ? 2 : (GSAI()->IsAIOn() == true) ? 1 : 0;
	_switcher->SetActiveWidgetIndex(index);

	if (index != 0)
	{
		OnPlayAni();
	}
}
