// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsSaveBatteryManager.h"
//#include "Management/ScopeGame/GsGameObjectManager.h"
#include "DataSchema/Tutorial/GsSchemaTutorial.h"
#include "Tutorial/GsTutorialFactory.h"
#include "Tutorial/Tutorial/GsTutorialBase.h"
#include "Tutorial/Action/GsTutorialActionBase.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"
#include "Input/GsInputEventMsgBase.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Classes/Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "Runtime/Core/Public/Misc/EnumClassFlags.h"


void FGsTutorialManager::Initialize()
{
	
}

void FGsTutorialManager::Finalize()
{
	ClearEndDelayTimer();

	_currTutorial.Reset();
	_tutorialMap.Empty();
	_completeTutorialIdList.Empty();
	_reservedQueue.Empty();
}

void FGsTutorialManager::InitializeAfterResourceDownload()
{
	CreateTutorialMap();
}

void FGsTutorialManager::SetCompleteTutorialList(PD::SC::PKT_SC_TUTORIAL_READ& InPacket)
{
	_completeTutorialIdList.Empty();

	for (PD::SC::PKT_SC_TUTORIAL_READ::TutorialIdListIterator iter = InPacket.GetFirstTutorialIdListIterator();
		iter != InPacket.GetLastTutorialIdListIterator(); ++iter)
	{
		_completeTutorialIdList.Emplace(iter->TutorialId());
	}

	// 완료된 튜토리얼 항목 저장
	for (TutorialId& tutorialId : _completeTutorialIdList)
	{
		TWeakPtr<FGsTutorialBase> tutorial = GetTutorial(tutorialId);
		if (tutorial.IsValid())
		{
			tutorial.Pin()->SetIsComplete(true);
		}
	}
}

void FGsTutorialManager::SetCompleteTutorial(TutorialId InTutorialId)
{
	if (false == _completeTutorialIdList.Contains(InTutorialId))
	{
		_completeTutorialIdList.Emplace(InTutorialId);

		TWeakPtr<FGsTutorialBase> tutorial = GetTutorial(InTutorialId);
		if (tutorial.IsValid())
		{
			tutorial.Pin()->SetIsComplete(true);
		}
	}
}

void FGsTutorialManager::CreateTutorialMap()
{
	TArray<const FGsSchemaTutorial*> rowList;
	if (const UGsTable* table = FGsSchemaTutorial::GetStaticTable())
	{	
		table->GetAllRows(rowList);
	}

	for (const FGsSchemaTutorial* row : rowList)
	{
		TSharedPtr<FGsTutorialBase> tutorial = FGsTutorialFactory::CreateTutorial(row);
		if (tutorial.IsValid())
		{
			_tutorialMap.Emplace(row->id, tutorial);
		}
	}
}

void FGsTutorialManager::SetReserveTutorial(TutorialId InTutorialId)
{
	if (false == _reservedQueue.Contains(InTutorialId))
	{
		_reservedQueue.Emplace(InTutorialId);
	}

	ProcessQueue();
}

void FGsTutorialManager::OnEndTutorial(TutorialId InTutorialId)
{
	EndTutorial(InTutorialId);
}

void FGsTutorialManager::OnEndAction(int32 InTutorialActionId)
{
	if (_currTutorial.IsValid())
	{
		_currTutorial.Pin()->OnEndAction(InTutorialActionId);
	}
}

void FGsTutorialManager::OnEventActionTutorialStart(const IGsMessageParam* InParam)
{
	// 튜토리얼 시작 이벤트 액션만 옵니다
	TutorialId tutorialId = CheckStartCondition(EGsTutorialConditionType::QUEST, InParam);
	if (INVALID_TUTORIAL_ID != tutorialId)
	{
		SetReserveTutorial(tutorialId);
		return;
	}

	// 튜토리얼 시작이 왔는데 못찾았으면, 튜토리얼 완료를 바로 보내서 이벤트 액션을 강제로 종료
	if (InParam)
	{
		if (const FGsTutorialMsgParamTutorialId* param = InParam->Cast<const FGsTutorialMsgParamTutorialId>())
		{
			GSLOG(Warning, TEXT("[Tutorial] OnEventActionStart. Fail to reserve tutorial. tutorialId: %d"), param->_data);

			FGsTutorialMsgParamTutorialId sendParam(param->_data);
			GMessage()->GetTutorial().SendMessage(MessageContentTutorial::COMPLETE_TUTORIAL, &sendParam);
		}
	}
}

void FGsTutorialManager::OnOpenUI(const IGsMessageParam* InParam)
{
	// 진행중이던 튜토리얼이 있을 경우
	if (_currTutorial.IsValid())
	{
		TWeakPtr<FGsTutorialActionBase> action = _currTutorial.Pin()->GetCurrentAction();
		if (action.IsValid())
		{
			action.Pin()->OnOpenUI(InParam);
		}

		return;
	}

	// 없으면 시작 조건 체크
	TutorialId tutorialId = CheckStartCondition(EGsTutorialConditionType::OPEN_UI, InParam);
	if (INVALID_TUTORIAL_ID != tutorialId)
	{
		SetReserveTutorial(tutorialId);
	}
}

void FGsTutorialManager::OnOpenUICheckParam(const IGsMessageParam* InParam)
{
	// 진행 중인 튜토리얼 있으면 무시
	if (IsActiveTutorial())
	{
		return;
	}

	// 없으면 시작 조건 체크
	TutorialId tutorialId = CheckStartCondition(EGsTutorialConditionType::OPEN_UI_CHECK_PARAM, InParam);
	if (INVALID_TUTORIAL_ID != tutorialId)
	{
		SetReserveTutorial(tutorialId);
	}
}

void FGsTutorialManager::OnEndUIInteraction(int32 InTutorialActionId)
{
	if (_currTutorial.IsValid())
	{
		TWeakPtr<FGsTutorialActionBase> action = _currTutorial.Pin()->GetCurrentAction();
		if (action.IsValid())
		{
			if (action.Pin()->GetActionId() == InTutorialActionId)
			{
				action.Pin()->OnEndUIInteraction();
			}
		}
	}
}

void FGsTutorialManager::OnLearnSkill(SkillId InSkillId)
{
	if (_currTutorial.IsValid())
	{
		TWeakPtr<FGsTutorialActionBase> action = _currTutorial.Pin()->GetCurrentAction();
		if (action.IsValid())
		{
			action.Pin()->OnLearnSkill(InSkillId);
		}
	}
}

void FGsTutorialManager::OnSummonButtonState(const IGsMessageParam* InParam)
{
	if (_currTutorial.IsValid())
	{
		TWeakPtr<FGsTutorialActionBase> action = _currTutorial.Pin()->GetCurrentAction();
		if (action.IsValid())
		{
			action.Pin()->OnSummonButtonState(InParam);
		}
	}
}

void FGsTutorialManager::OnContentsUnlock(const IGsMessageParam* InParam)
{
	TutorialId tutorialId = CheckStartCondition(EGsTutorialConditionType::UNLOCK_CONTENT, InParam);
	if (INVALID_TUTORIAL_ID != tutorialId)
	{
		SetReserveTutorial(tutorialId);
	}
}

void FGsTutorialManager::OnAcceptQuest(const IGsMessageParam* InParam)
{
	TutorialId tutorialId = CheckStartCondition(EGsTutorialConditionType::QUEST_ID, InParam);
	if (INVALID_TUTORIAL_ID != tutorialId)
	{
		SetReserveTutorial(tutorialId);
	}
}

void FGsTutorialManager::OnReadyQuest(const IGsMessageParam* InParam)
{
	TutorialId tutorialId = CheckStartCondition(EGsTutorialConditionType::QUEST_STORY_ID, InParam);
	if (INVALID_TUTORIAL_ID != tutorialId)
	{
		SetReserveTutorial(tutorialId);
	}
}

void FGsTutorialManager::OnFirstTutorial(const IGsMessageParam* InParam)
{
	TutorialId tutorialId = CheckStartCondition(EGsTutorialConditionType::FIRST_TUTORIAL, InParam);
	if (INVALID_TUTORIAL_ID != tutorialId)
	{
		SetReserveTutorial(tutorialId);
	}
}

void FGsTutorialManager::OnCheckProcessQueue()
{
	ProcessQueue();
}

TutorialId FGsTutorialManager::CheckStartCondition(EGsTutorialConditionType InType, const IGsMessageParam* InParam)
{
	for (TPair<int32, TSharedPtr<FGsTutorialBase>> itPair : _tutorialMap)
	{
		if (InType == itPair.Value->GetConditionType())
		{
			if (itPair.Value->CheckStartCondition(InParam))
			{
				return itPair.Value->GetTutorialId();
			}
		}
	}

	return INVALID_TUTORIAL_ID;
}

bool FGsTutorialManager::StartTutorial(TutorialId InTutorialId, bool bIsForced)
{
	// 수행 중인 튜토리얼이 있으면 리턴
	if (IsActiveTutorial())
	{
		return false;
	}

	// 완료된 튜토리얼인지 체크. 강제 실행일 경우 넘어감
	if (false == bIsForced)
	{
		if (_completeTutorialIdList.Contains(InTutorialId))
		{	
			return false;
		}

		TWeakPtr<FGsTutorialBase> tempTutorial = GetTutorial(InTutorialId);
		if (false == tempTutorial.IsValid())
		{
			return false;
		}

		// 시작 가능한 상태인지 체크
		if (false == tempTutorial.Pin()->CheckIsValidToStart())
		{
			return false;
		}
	}

	GUI()->SetLockQueueProcess(true);

//#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Tutorial] Start Tutorial. TutorialId: %d"), InTutorialId);
//#endif

	bool bStartSuccess = false;
	
	_currTutorial = GetTutorial(InTutorialId);
	if (_currTutorial.IsValid())
	{
		_currTutorial.Pin()->Reset();

		bStartSuccess = _currTutorial.Pin()->StartTutorial();
	}

	// 완료 리스트에 없으면 서버에 패킷 보냄
	if (false == _completeTutorialIdList.Contains(InTutorialId))
	{
		if (false == _serverNotSave)
		{
			FGsNetSendServiceWorld::SendReqTutorialSave(InTutorialId);
		}
	}

	// 시작 메시지 보냄(TutorialHandler에 받아서 Auto를 끄는 코드가 있음)
	GMessage()->GetTutorial().SendMessage(MessageContentTutorial::START_TUTORIAL, nullptr);

	// 시작 실패 시 튜토리얼 강제 종료
	if (false == bStartSuccess)
	{
		ForcedEndTutorial();
	}

	return true;
}

void FGsTutorialManager::EndTutorial(TutorialId InTutorialId)
{
	// 튜토리얼을 바로 연달아 진행하면 튜토리얼 UI의 여닫는 타이밍이 꼬일 수 있음.
	// Tutorial UI가 닫힌 후 실제 종료는 타이머 끝난 후 진행됨
	if (UWorld* world = GUI()->GetWorld())
	{
		world->GetTimerManager().SetTimer(_timerHandleEndDelay, 
			FTimerDelegate::CreateRaw(this, &FGsTutorialManager::OnTimerEndDelay), 0.1f, false);
	}
	else
	{
		EndTutorialInter(InTutorialId);
	}
}

void FGsTutorialManager::EndTutorialInter(TutorialId InTutorialId)
{
	//#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Tutorial] End Tutorial. TutorialId: %d"), InTutorialId);
	//#endif

	_currTutorial.Reset();

	// EventAction쪽에 튜토리얼이 끝났음을 알리기 위함
	FGsTutorialMsgParamTutorialId param(InTutorialId);
	GMessage()->GetTutorial().SendMessage(MessageContentTutorial::COMPLETE_TUTORIAL, &param);

	// 키보드 이동을 풀어준다
	//FGsInputEventMsgBase inputParam;
	//inputParam.Id = 0;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_TUTORIAL, false);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
		
	GUI()->SetLockQueueProcess(false);

	// 다음 튜토리얼 검사
	ProcessQueue();

	// ProcessQueue에 의해 새 튜토리얼이 시작했는지, 큐가 비어있는지 체크. BM상점 오픈 타이밍 체크용
	if (false == IsActiveTutorial() &&
		0 == _reservedQueue.Num())
	{
		GMessage()->GetTutorial().SendMessage(MessageContentTutorial::COMPLETE_TUTORIAL_FOR_BM, nullptr);
	}
}

bool FGsTutorialManager::IsActiveTutorial() const
{ 
	if (_currTutorial.IsValid())
	{
		return true;
	}

	return false;
}

int32 FGsTutorialManager::GetActiveTutorialId() const
{
	if (_currTutorial.IsValid())
	{
		return _currTutorial.Pin()->GetTutorialId();
	}

	return 0;
}

bool FGsTutorialManager::IsActiveTutorialUseStopAuto() const
{
	if (_currTutorial.IsValid())
	{
		return _currTutorial.Pin()->UseStopAuto();
	}

	return 0;
}

bool FGsTutorialManager::IsFirstTutorial() const
{
	if (_currTutorial.IsValid())
	{
		if (EGsTutorialConditionType::FIRST_TUTORIAL == _currTutorial.Pin()->GetConditionType())
		{
			return true;
		}
	}

	return false;
}

TWeakPtr<FGsTutorialBase> FGsTutorialManager::GetTutorial(TutorialId InTutorialId)
{
	if (TSharedPtr<FGsTutorialBase>* tutorialPtr = _tutorialMap.Find(InTutorialId))
	{
		return *tutorialPtr;
	}
	
	return nullptr;
}

void FGsTutorialManager::ProcessQueue()
{
	// 예약된 튜토리얼이 없으면 리턴
	if (0 == _reservedQueue.Num())
	{
		return;
	}

	// 진행중인 튜토리얼이 있으면 리턴
	if (IsActiveTutorial())
	{
		return;
	}
	
	if (UGsUIManager* uiMgr = GUI())
	{
		// 레벨업, 해금 팝업과 겹치는 문제 수정
		// UI큐가 비어있지 않으면 모두 비워진 후 튜로리얼 발동을 위해 리턴
		if (false == uiMgr->IsReservedQueueEmpty())
		{
			return;
		}

		// 무비 보고있으면 리턴
		if (uiMgr->IsActiveWidget(TEXT("TrayMovie")))
		{
			return;
		}

		// 혹시 모를 각종 상황 예외처리(대화 중, 시네마틱 중, 사망, 오프라인 플레이 Hide 상황)
		EGsUIHideFlags hideFlags = uiMgr->GetHideFlagsCurrent();
		if (EnumHasAnyFlags(hideFlags, EGsUIHideFlags::STATE_DIALOG) ||
			EnumHasAnyFlags(hideFlags, EGsUIHideFlags::STATE_CINEMATIC) ||
			EnumHasAnyFlags(hideFlags, EGsUIHideFlags::STATE_DIE) ||
			EnumHasAnyFlags(hideFlags, EGsUIHideFlags::STATE_OFFLINE_PLAY))
		{
			return;
		}
	}

	// 개선: 타 매니저 사용하는 함수 매니저 밖에서 체크할 수 있도록 로직 수정 필요

	// 절전모드이면 리턴
	if (GSSaveBattery()->IsSaveBatteryTurnedOn())
	{
		return;
	}
	
	//// 내 캐릭터 사망 중이면 리턴
	//if (GSGameObject()->IsLocalPlayerDead())
	//{
	//	return;
	//}

	TutorialId reservedTutorial = _reservedQueue[0];
	_reservedQueue.RemoveAt(0);

	// 시작 실패 시 다시 큐 돌림
	if (false == StartTutorial(reservedTutorial))
	{
		ProcessQueue();
	}
}

void FGsTutorialManager::ClearEndDelayTimer()
{
	if (_timerHandleEndDelay.IsValid())
	{
		if (UWorld* world = GUI()->GetWorld())
		{
			world->GetTimerManager().ClearTimer(_timerHandleEndDelay);
		}
	}
}

void FGsTutorialManager::OnTimerEndDelay()
{
	ClearEndDelayTimer();

	if (_currTutorial.IsValid())
	{
		EndTutorialInter(_currTutorial.Pin()->GetTutorialId());
	}
}

void FGsTutorialManager::ForcedEndTutorial()
{
	if (_currTutorial.IsValid())
	{
		_currTutorial.Pin()->ForcedEndTutorial();

		EndTutorial(_currTutorial.Pin()->GetTutorialId());
	}
}

//----------------------------------------------------------------------------------------------------------------------
void FGsTutorialManager::ForcedStartTutorial(TutorialId InTutorialId)
{
	//SetReserveTutorial(InTutorialId);
	StartTutorial(InTutorialId, true);
}

void FGsTutorialManager::ForcedResetTutorial()
{
	_completeTutorialIdList.Empty();

	for (TPair<int32, TSharedPtr<FGsTutorialBase>> itPair : _tutorialMap)
	{
		itPair.Value->Reset();
	}
}

void FGsTutorialManager::NotSaveTutorial(bool bNotSave)
{
	_serverNotSave = bNotSave;
}
