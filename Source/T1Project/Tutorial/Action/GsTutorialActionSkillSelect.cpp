// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionSkillSelect.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Skill/GsSkill.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Window/GsUIWindowSkill.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"


void FGsTutorialActionSkillSelect::OnPrevStart(const IGsMessageParam* InParam)
{
	if (const FGsTutorialMsgParamSkill* param = InParam->Cast<const FGsTutorialMsgParamSkill>())
	{
		_skillId = param->_data;
	}
}

bool FGsTutorialActionSkillSelect::OnStart()
{
	if (false == Super::OnStart())
	{
		return false;
	}

	if (INVALID_SKILL_ID == _skillId)
	{
		return false;
	}

	_actionUITable = _table->actionUIInteractionRow.GetRow();
	if (nullptr == _actionUITable)
	{
		return false;
	}

	const FGsSkill* skill = GSkill()->FindSkill(_skillId, CreatureWeaponType::MAX, true);
	if (nullptr == skill)
	{
		return false;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(_actionUITable->wbpKey);
	if (false == widget.IsValid())
	{
		return false;
	}

	_window = Cast<UGsUIWindowSkill>(widget.Get());
	if (false == _window.IsValid())
	{
		return false;
	}

	_window->OnTutorialSkillListEvent.BindRaw(this, &FGsTutorialActionSkillSelect::OnTutorialSkillListEvent);
	_window->SetTutorial(skill);

	return true;
}

void FGsTutorialActionSkillSelect::OnEnd(bool bInSendMessage)
{
	// 튜토리얼 끝나서 창 닫힐때까지 유지하기 위해 해제하지 않음
	//if (_window.IsValid())
	//{
	//	_window->SetTutorial(nullptr);
	//}

	_actionUITable = nullptr;
	_window = nullptr;
	
	Super::OnEnd(bInSendMessage);
}

void FGsTutorialActionSkillSelect::OnTutorialSkillListEvent(UWidget* InTargetWidget)
{
	if (false == _window.IsValid())
	{
		return;
	}

	_window->OnTutorialSkillListEvent.Unbind();

	if (InTargetWidget->IsA<UUserWidget>())
	{
		UWidget* targetWidget = UGsUITrayTutorial::RecursiveFindWidget(Cast<UUserWidget>(InTargetWidget),
			FName(_actionUITable->targetRootWidget), FName(_actionUITable->targetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, _actionUITable, targetWidget))
		{
			OnEnd();
		}
	}
}
