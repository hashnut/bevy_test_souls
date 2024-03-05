// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialBase.h"
#include "DataSchema/Tutorial/GsSchemaTutorial.h"
#include "DataSchema/Tutorial/GsSchemaTutorialAction.h"
#include "Tutorial/GsTutorialFactory.h"
#include "Tutorial/Condition/GsTutorialConditionBase.h"
#include "Tutorial/Action/GsTutorialActionBase.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"
#include "Input/GsInputEventMsgBase.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "T1Project.h"


FGsTutorialBase::~FGsTutorialBase()
{
	Finalize();
}

void FGsTutorialBase::Initialize(const FGsSchemaTutorial* InTable)
{
	_table = InTable;
	_tutorialId = InTable->id;
	_condition = FGsTutorialFactory::CreateCondition(_table);
}

void FGsTutorialBase::Finalize()
{
	_table = nullptr;
	_tutorialId = 0;
	_condition.Reset();
	
	Reset();
}

void FGsTutorialBase::Reset()
{
	_currAction.Reset();
	_actionList.Empty();
	_bIsComplete = false;
}

bool FGsTutorialBase::CheckStartCondition(const IGsMessageParam* InParam)
{
	if (false == _bIsComplete)
	{
		if (_condition)
		{
			return _condition->CheckCondition(InParam);
		}
	}

	return false;
}

void FGsTutorialBase::OnEndAction(int32 InTutorialActionId)
{
	// 다음 액션 재생
	for (int i = 0; i < _actionList.Num(); ++i)
	{
		TSharedPtr<FGsTutorialActionBase> action = _actionList[i];
		if (false == action->IsEnd())
		{
			_currAction = action;

			if (StartActionInternal())
			{
				return;
			}
			else
			{
				// 종료 시켜 버림
				break;
			}
		}
	}

	// 재생될 다음 액션이 없으면 종료
	OnEnd();
}

bool FGsTutorialBase::CheckIsValidToStart()
{
	if (false == _bIsComplete)
	{
		if (_condition)
		{
			return _condition->IsValidToStart();
		}
	}

	return false;
}

void FGsTutorialBase::OnEnd(bool bInSendMessage)
{
	// _bIsComplete는 서버에서 받아서 세팅한다
	_currAction = nullptr;

	// 튜토리얼 UI 닫기
	GUI()->CloseByKeyName(TEXT("TrayTutorial"));

	if (bInSendMessage)
	{
		FGsTutorialMsgParamInt param(_tutorialId);
		GMessage()->GetTutorial().SendMessage(MessageContentTutorial::END_TUTORIAL, &param);
	}
}

bool FGsTutorialBase::StartActionInternal()
{
	if (_currAction.Pin()->OnStart())
	{
		return true;
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Tutorial] Fail to start action. TutorialId: %d, ActionId: %d"), 
		_tutorialId, _currAction.Pin()->GetActionId());
#endif

	return false;
}

bool FGsTutorialBase::OnPrevStart()
{
	if (nullptr == _table)
	{
		return false;
	}

	// 액션 리스트 생성
	_actionList.Empty();
	for (int i = 0; i < _table->actionList.Num(); ++i)
	{
		if (const FGsSchemaTutorialAction* table = _table->actionList[i].GetRow())
		{
			_actionList.Emplace(FGsTutorialFactory::CreateAction(table));
		}
	}

	// 조건이 OPENUI가 아닌 튜토리얼들은 튜토리얼이 가능한 기본 UI상태를 만든다.
	if (EGsTutorialConditionType::OPEN_UI != _table->conditionType &&
		EGsTutorialConditionType::OPEN_UI_CHECK_PARAM != _table->conditionType)
	{
		UGsUIManager* uiMgr = GUI();

		// 모든 열려있는 UI 닫기
		uiMgr->CloseAllStack();
		// HUD 기본상태로 돌리기
		uiMgr->SetHUDMode(EGsUIHUDMode::NORMAL);		
	}

	GMessage()->GetContents().SendMessage(MessageContents::CLOSE_QUICK_MENU);

	// 키보드 이동을 막는다
	//FGsInputEventMsgBase inputParam;
	//inputParam.Id = 1;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_TUTORIAL, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	// 서브 다이얼로그를 제거한다
	GSQuest()->OnSubDialogClose();

	// 튜토리얼 UI 열기
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("TrayTutorial"));
	if (widget.IsValid())
	{
		UGsUITrayTutorial* tutorialUI = Cast<UGsUITrayTutorial>(widget.Get());
		tutorialUI->SetVisibility(ESlateVisibility::Collapsed);
	}

	return true;
}

bool FGsTutorialBase::OnStart()
{
	if (0 == _actionList.Num())
	{
		return false;
	}

	_currAction = _actionList[0];

	return StartActionInternal();
}

bool FGsTutorialBase::StartTutorial()
{
	if (OnPrevStart())
	{
		if (OnStart())
		{
			return true;
		}
	}

	// 잘못되었을 때의 처리
#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Tutorial] Fail to start tutorial. id: %d"), _tutorialId);
#endif

	GUI()->CloseByKeyName(TEXT("TrayTutorial"));

	return false;
}

void FGsTutorialBase::ForcedEndTutorial()
{
	if (_currAction.IsValid())
	{
		_currAction.Pin()->OnEnd(false);
	}

	OnEnd(false);
}
