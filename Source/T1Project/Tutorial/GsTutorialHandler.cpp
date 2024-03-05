// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialHandler.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"


void FGsTutorialHandler::Init()
{
	BindMessages();
}

void FGsTutorialHandler::Close()
{
	UnbindMessages();
}

void FGsTutorialHandler::BindMessages()
{
	FGsMessageHolder* messageMgr = GMessage();

	MSystem& msgSystem = messageMgr->GetSystem();
	_msgHandleSystemList.Emplace(msgSystem.AddRaw(MessageSystem::RECONNECT_END,
		this, &FGsTutorialHandler::OnReconnection));	

	MSystemParam& msgSystemParam = messageMgr->GetSystemParam();

	_msgHandleSystemParamList.Emplace(msgSystemParam.AddRaw(MessageSystem::LOAD_TOPO_START,
		this, &FGsTutorialHandler::OnLoadTopoStart));
	_msgHandleSystemParamList.Emplace(msgSystemParam.AddRaw(MessageSystem::SEQUENCE_PLAYER_START,
		this, &FGsTutorialHandler::OnCinematicStart));
	_msgHandleSystemParamList.Emplace(msgSystemParam.AddRaw(MessageSystem::SEQUENCE_PLAYER_END,
		this, &FGsTutorialHandler::OnCinematicEnd));

	MTutorial& msgTutorial = messageMgr->GetTutorial();
	_msgHandleTutorialList.Emplace(msgTutorial.AddRaw(MessageContentTutorial::START_TUTORIAL,
		this, &FGsTutorialHandler::OnStartTutorial));
	_msgHandleTutorialList.Emplace(msgTutorial.AddRaw(MessageContentTutorial::END_TUTORIAL,
		this, &FGsTutorialHandler::OnEndTutorial));
	_msgHandleTutorialList.Emplace(msgTutorial.AddRaw(MessageContentTutorial::FORCED_END_TUTORIAL,
		this, &FGsTutorialHandler::OnForcedEndTutorial));
	_msgHandleTutorialList.Emplace(msgTutorial.AddRaw(MessageContentTutorial::EVENT_ACTION_TUTORIAL_START,
		this, &FGsTutorialHandler::OnEventActionTutorialStart));
	_msgHandleTutorialList.Emplace(msgTutorial.AddRaw(MessageContentTutorial::OPEN_UI,
		this, &FGsTutorialHandler::OnOpenUI));
	_msgHandleTutorialList.Emplace(msgTutorial.AddRaw(MessageContentTutorial::OPEN_UI_CHECK_PARAM,
		this, &FGsTutorialHandler::OnOpenUICheckParam));
	_msgHandleTutorialList.Emplace(msgTutorial.AddRaw(MessageContentTutorial::END_ACTION,
		this, &FGsTutorialHandler::OnEndAction));
	_msgHandleTutorialList.Emplace(msgTutorial.AddRaw(MessageContentTutorial::END_UI_INTERACTION,
		this, &FGsTutorialHandler::OnEndUIInteraction));
	_msgHandleTutorialList.Emplace(msgTutorial.AddRaw(MessageContentTutorial::LEARN_SKILL,
		this, &FGsTutorialHandler::OnLearnSkill));
	_msgHandleTutorialList.Emplace(msgTutorial.AddRaw(MessageContentTutorial::SUMMON_BUTTON_STATE,
		this, &FGsTutorialHandler::OnSummonButtonState));
	_msgHandleTutorialList.Emplace(msgTutorial.AddRaw(MessageContentTutorial::FIRST_TUTORIAL,
		this, &FGsTutorialHandler::OnFirstTutorial));

	MGameObject& msgGameObject = messageMgr->GetGameObject();
	_msgHandleGameObjectList.Emplace(msgGameObject.AddRaw(MessageGameObject::LOCAL_DEAD,
		this, &FGsTutorialHandler::OnLocalPlayerDead));
	_msgHandleGameObjectList.Emplace(msgGameObject.AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
		this, &FGsTutorialHandler::OnLocalSpawnMeComplete));

	MUI& msgUI = messageMgr->GetUI();
	_msgHandleUIList.Emplace(msgUI.AddRaw(MessageUI::UI_RESERVED_QUEUE_EMPTY,
		this, &FGsTutorialHandler::OnUIReservedQueueEmpty));
	_msgHandleUIList.Emplace(msgUI.AddRaw(MessageUI::STOP_MOVIE,
		this, &FGsTutorialHandler::OnStopMovie));

	MUnlock& msgUnlock = messageMgr->GetUnlock();
	_msgHandleUnlockList.Emplace(msgUnlock.AddRaw(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK,
		this, &FGsTutorialHandler::OnContentsUnlock));

	MQuest& msgQuest = messageMgr->GetQuest();
	_msgHandleQuestList.Emplace(msgQuest.AddRaw(MessageContentQuest::QUEST_MAIN_ACCEPTED,
		this, &FGsTutorialHandler::OnAcceptQuest));
	_msgHandleQuestList.Emplace(msgQuest.AddRaw(MessageContentQuest::QUEST_MAIN_READY,
		this, &FGsTutorialHandler::OnReadyQuest));

	MSaveBattery& msgSaveBattery = messageMgr->GetSaveBattery();
	_msgHandleSaveBatteryList.Emplace(msgSaveBattery.AddRaw(MessageContentSaveBattery::ENTER,
		this, &FGsTutorialHandler::OnSaveBatteryEnter));
	_msgHandleSaveBatteryList.Emplace(msgSaveBattery.AddRaw(MessageContentSaveBattery::EXIT,
		this, &FGsTutorialHandler::OnSaveBatteryExit));
}

void FGsTutorialHandler::UnbindMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
	{
		return;
	}

	MSystem& msgSystem = messageMgr->GetSystem();
	for (MsgSystemHandle& handle : _msgHandleSystemList)
	{
		msgSystem.Remove(handle);
	}
	_msgHandleSystemList.Empty();

	MSystemParam& msgSystemParam = messageMgr->GetSystemParam();
	for (MsgSystemHandle& handle : _msgHandleSystemParamList)
	{
		msgSystemParam.Remove(handle);
	}
	_msgHandleSystemParamList.Empty();

	MTutorial& msgTutorial = messageMgr->GetTutorial();
	for (MsgHandleTutorial& handle : _msgHandleTutorialList)
	{
		msgTutorial.Remove(handle);
	}
	_msgHandleTutorialList.Empty();

	MGameObject& msgGameObject = messageMgr->GetGameObject();
	for (MsgGameObjHandle& handle : _msgHandleGameObjectList)
	{
		msgGameObject.Remove(handle);
	}
	_msgHandleGameObjectList.Empty();

	MUI& msgUI = messageMgr->GetUI();
	for (MsgUIHandle& handle : _msgHandleUIList)
	{
		msgUI.Remove(handle);
	}
	_msgHandleUIList.Empty();

	MUnlock& msgUnlock = messageMgr->GetUnlock();
	for (MsgHandleUnlock& handle : _msgHandleUnlockList)
	{
		msgUnlock.Remove(handle);
	}
	_msgHandleUnlockList.Empty();

	MQuest& msgQuest = messageMgr->GetQuest();
	for (MsgHandleQuest& handle : _msgHandleQuestList)
	{
		msgQuest.Remove(handle);
	}
	_msgHandleQuestList.Empty();

	MSaveBattery& msgSaveBattery = messageMgr->GetSaveBattery();
	for (MsgHandleSaveBattery& handle : _msgHandleSaveBatteryList)
	{
		msgSaveBattery.Remove(handle);
	}
	_msgHandleSaveBatteryList.Empty();
}

void FGsTutorialHandler::OnReconnection()
{
	// 주의: 원래 FGsGameStateHud::OnReconectionEnd 에 연결했었으나, HUDState가 아닌 경우에도 호출을 받아야해서 메시지로 변경
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->ForcedEndTutorial();
	}
}

void FGsTutorialHandler::OnLoadTopoStart(const IGsMessageParam* inParam)
{
	// 워프, 인스턴스 던전 등의 상황에서 UI가 닫히므로 튜토리얼도 강제로 종료시킴
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->ForcedEndTutorial();
	}
}

void FGsTutorialHandler::OnCinematicStart(const IGsMessageParam*)
{
	// 시네마틱 시작 시 강제 종료(C2URWQ-5811). 시네마틱 후 창이 닫히는 경우가 있어서.
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->ForcedEndTutorial();
	}
}

void FGsTutorialHandler::OnCinematicEnd(const IGsMessageParam*)
{
	// 시네마틱 중 튜토리얼이 들어올 경우 무시되었던 것을 체크하기 위함
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->OnCheckProcessQueue();
	}
}

void FGsTutorialHandler::OnStartTutorial(const IGsMessageParam*)
{
	FGsTutorialManager* tutorialMgr = GSTutorial();
	if (nullptr == tutorialMgr)
	{
		return;
	}

	if (tutorialMgr->IsActiveTutorialUseStopAuto())
	{
		// 튜토리얼 시작 시 자동 강제 멈춤
		if (FGsAIManager* aiMgr = GSAI())
		{
			aiMgr->SetActiveAI(false);
		}

		// 타겟 강제 해제(자동 전투를 꺼도 타게팅 된 대상은 계속 공격함)
		if (UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			// 튜토리얼 시작 시 퀘스트 진행 강제 멈춤
			if (UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(localBase))
			{
				if (FGsQuestHandler* questHandler = localPlayer->GetQuestHandler())
				{
					questHandler->SetAutoMoveStop();
				}
			}

			if (FGsTargetHandlerLocalPlayer* targetHandler = localBase->GetCastTarget<FGsTargetHandlerLocalPlayer>())
			{
				targetHandler->ClearTarget(true);
			}
		}
	}
}	

void FGsTutorialHandler::OnEndTutorial(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (const FGsTutorialMsgParamTutorialId* param = InParam->Cast<const FGsTutorialMsgParamTutorialId>())
		{
			if (FGsTutorialManager* tutorialMgr = GSTutorial())
			{
				tutorialMgr->OnEndTutorial(param->_data);
			}
		}
	}
}

void FGsTutorialHandler::OnForcedEndTutorial(const IGsMessageParam*)
{
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->ForcedEndTutorial();
	}
}

void FGsTutorialHandler::OnEndAction(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (const FGsTutorialMsgParamInt* param = InParam->Cast<const FGsTutorialMsgParamInt>())
		{
			if (FGsTutorialManager* tutorialMgr = GSTutorial())
			{
				tutorialMgr->OnEndAction(param->_data);
			}
		}
	}
}

void FGsTutorialHandler::OnEventActionTutorialStart(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (FGsTutorialManager* tutorialMgr = GSTutorial())
		{
			tutorialMgr->OnEventActionTutorialStart(InParam);
		}
	}
}

void FGsTutorialHandler::OnOpenUI(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (FGsTutorialManager* tutorialMgr = GSTutorial())
		{
			tutorialMgr->OnOpenUI(InParam);
		}
	}
}

void FGsTutorialHandler::OnOpenUICheckParam(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (FGsTutorialManager* tutorialMgr = GSTutorial())
		{
			tutorialMgr->OnOpenUICheckParam(InParam);
		}
	}
}

void FGsTutorialHandler::OnEndUIInteraction(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (const FGsTutorialMsgParamInt* param = InParam->Cast<const FGsTutorialMsgParamInt>())
		{
			if (FGsTutorialManager* tutorialMgr = GSTutorial())
			{
				tutorialMgr->OnEndUIInteraction(param->_data);
			}
		}
	}
}

void FGsTutorialHandler::OnLearnSkill(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (const FGsTutorialMsgParamSkill* param = InParam->Cast<const FGsTutorialMsgParamSkill>())
		{
			if (FGsTutorialManager * tutorialMgr = GSTutorial())
			{
				tutorialMgr->OnLearnSkill(param->_data);
			}
		}
	}
}

void FGsTutorialHandler::OnSummonButtonState(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (FGsTutorialManager* tutorialMgr = GSTutorial())
		{
			tutorialMgr->OnSummonButtonState(InParam);
		}
	}
}

void FGsTutorialHandler::OnFirstTutorial(const IGsMessageParam* InParam)
{
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->OnFirstTutorial(InParam);
	}
}

void FGsTutorialHandler::OnLocalPlayerDead(const IGsMessageParam*)
{
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		// 진행 중인 튜토리얼이 있다면 강종시킴
		tutorialMgr->ForcedEndTutorial();
	}
}

void FGsTutorialHandler::OnLocalSpawnMeComplete(const IGsMessageParam*)
{
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		// 사망 중 튜토리얼이 들어올 경우 무시되었던 것을 체크하기 위함
		tutorialMgr->OnCheckProcessQueue();
	}
}

void FGsTutorialHandler::OnUIReservedQueueEmpty(const IGsMessageParam*)
{
	// UI 큐가 비었을때 호출, 예약된 튜토리얼 있으면 진행
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->OnCheckProcessQueue();
	}	
}

void FGsTutorialHandler::OnStopMovie(const IGsMessageParam*)
{
	// 무비가 끝났을 때 호출, 예약된 튜토리얼 있으면 진행
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->OnCheckProcessQueue();
	}
}

void FGsTutorialHandler::OnContentsUnlock(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (FGsTutorialManager* tutorialMgr = GSTutorial())
		{
			tutorialMgr->OnContentsUnlock(InParam);
		}
	}
}

void FGsTutorialHandler::OnAcceptQuest(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (FGsTutorialManager* tutorialMgr = GSTutorial())
		{
			tutorialMgr->OnAcceptQuest(InParam);
		}
	}
}

void FGsTutorialHandler::OnReadyQuest(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (FGsTutorialManager* tutorialMgr = GSTutorial())
		{
			tutorialMgr->OnReadyQuest(InParam);
		}
	}
}

void FGsTutorialHandler::OnSaveBatteryEnter(const IGsMessageParam*)
{
	// 절전모드 입장 시 튜토리얼 강종
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->ForcedEndTutorial();
	}
}

void FGsTutorialHandler::OnSaveBatteryExit(const IGsMessageParam*)
{
	// 절전모드 UI 닫혔을 때 호출, 예약된 튜토리얼 있으면 진행
	// 주의 절전모드 상태에서 게임종료 시 튜토리얼 매니저 삭제 후 들어올 수 있어서 널 체크 필수
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->OnCheckProcessQueue();
	}
}
