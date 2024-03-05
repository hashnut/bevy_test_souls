// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerAutoMove.h"
#include "GsStateLocalPlayerHeader.h"

#include "AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "ControllerEx/GsPlayerController.h"
#include "ActorComponentEx/GsCharacterMovementComponent.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Movement/GsAutoMoveInfo.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Looting/GsLootingHandlerLocalPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/AI/GsAIReserveHandler.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "AI/Navigation/NavigationTypes.h"
#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"
#include "AI/Quest/GsAIQuestHandler.h"

#include "UTIL/GsMove.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "Data/GsDataContainManager.h"

#include "Classes/AI/Navigation/NavigationTypes.h"
#include "../../AI/GsAIAvoidCollisionHandler.h"

//----------------------------------------------------------------------------------------
// FGsStateLocalPlayerAutoMove
//----------------------------------------------------------------------------------------
uint8 FGsStateLocalPlayerAutoMove::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::AutoMove);
}

FName FGsStateLocalPlayerAutoMove::GetStateName() const
{
	return TEXT("StateLocalPlayerAutoMove");
}

FName FGsStateLocalPlayerAutoMove::GetBPStateName() const
{
	return TEXT("Move");
}

bool FGsStateLocalPlayerAutoMove::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{		
		switch (static_cast<EGsStateBase>(StateID))
		{
		GS_STATE_GROUP_MOVE:
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:
		case EGsStateBase::Attack:
		case EGsStateBase::Interaction:
		case EGsStateBase::Looting:
		case EGsStateBase::ChangeWeapon:
		case EGsStateBase::Warp:
		case EGsStateBase::AutoMoveStop:
		case EGsStateBase::ReactionEnd:
			return true;			
		default:
			return false;
		}		
	}
	return false;
}

//auto Move는 변이 상태에만 reEnter(Enter) 처리한다.
void FGsStateLocalPlayerAutoMove::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	if (IsShapeChangedLock(Owner)) Enter(Owner);

	MoveToReservedMoveData(Owner);
}

void FGsStateLocalPlayerAutoMove::Enter(UGsGameObjectBase* Owner)
{
	//GSLOG(Warning, TEXT("[AUTO] FGsStateLocalPlayerAutoMove::Enter"));

	Super::Enter(Owner);

	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	// Movement 클래스에 이동 설정 알림
	if (FGsMovementHandlerBase* movement = local->GetMovementHandler())
	{
		movement->SetMoveState(true);

		if (FGsAIReserveHandler* aiReserveHandler = local->GetAIReserve())
		{
			if (FGsAIManager* aiMgr = GSAI())
			{
				if (true == aiMgr->IsAIOn())
				{
					aiReserveHandler->StartPauseTime(local->GetActor()->GetActorLocation());
				}					
			}			
		}
	}

	// 자동 이모션출력 방지처리
	if (auto skillHandler = local->GetSkillHandler())
	{
		skillHandler->SetAutoIdleEmotionEnable(false);
	}

	// 이동 타입 갱신처리
	local->ApplyMoveSpeedFromStatInfo();

	local->SetFairyVehicleCondition(EGsFairyVehicleCondition::MOVE_STATE);
	// 페어리 버프 이펙트 확인
	local->OnVehicleEffect(false);	

	MoveToReservedMoveData(Owner);

	// 자동 사용 스킬 갱신 요청
	// FGsStateLocalPlayerAutoMove는 FGsStateLocalPlayerMoveBase를 상속받지 않았는데
	// 빌드 종료 이후 확인해 봐야할것 같다.
	FGsGameObjectMessageParamBuffSkill messageParam(Owner, 0, FGsGameObjectMessageParamBuffSkill::BuffSkillAuto::Enable);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_AUTOSKILL_UPDATE, &messageParam);

	// auto play시 drop item 있을때 멍때리고 있는 이슈 대응
	// 자동 이동시 ai에서 서버 응답대기 건거 해지
	if (FGsAIManager* aiManager = GSAI())
	{
		aiManager->GetContentsConditionDefault()->SetIsLootServerResponseWait(false);
	}
}

void FGsStateLocalPlayerAutoMove::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);
	bool isAutoMode = false;
	if (FGsAIManager* aiMgr = GSAI())
	{
		isAutoMode = aiMgr->IsAIOn();
	}

	//이동 Update 활성
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (FGsMovementHandlerBase* movement = local->GetMovementHandler())
	{
		FGsMovementDirtyMark mark(movement);

		movement->Update(Delta);
		// 블렌딩 처리를 위한 이동 속도값 알림
		if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
		{
			anim->SetMoveSpeed(movement->GetMotionType());
		}

		
		if (FGsAIReserveHandler* aiReserveHandler = local->GetAIReserve())
		{
			aiReserveHandler->UpdatePauseTime(local->GetActor()->GetActorLocation());
		}
	}

	
	bool isContentsAutoMove = false;	// 컨텐츠 자동 이동인가(퀘스트, npc list)
	
	EGsMovementAutoContentsType nowAutoContentsType = EGsMovementAutoContentsType::None;
	bool isMoveToWarp = false;
	bool isEvasiveManeuver = false;
	FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	check(movement);
	
	nowAutoContentsType =
		movement->GetStateAutoMove().GetMovementAutoContentsType();

	switch (nowAutoContentsType)
	{
	case EGsMovementAutoContentsType::NpcListMove:
	case EGsMovementAutoContentsType::QuestMove:
		isContentsAutoMove = true;
		break;
	default:
		isContentsAutoMove = false;
		break;
	}
	
	isMoveToWarp = movement->GetStateAutoMove().IsMoveNextMap();
	isEvasiveManeuver = (local->GetAIAvoidCollision()) ? local->GetAIAvoidCollision()->IsAvoidingCollision() : false;
	// 카메라 모드 이동 갱신
	if (FGsCameraModeHandler* cameraModehandler = local->GetCameraModeHandler())
	{
		cameraModehandler->GetState()->UpdateMove((isAutoMode || isContentsAutoMove));
	}	
	
	const FAIMoveRequest& moveData = movement->GetStateAutoMove().GetDataRequest();
	AGsPlayerController* controller = movement->GetPlayerController();

	
	// 길찾기를 수행후 도착완료 체크에 미달한 상태
	// 즉 목적지 세팅이 실패라 도착전에 다시 검사해서 이동해야 하는 상황
	// 지도 끝에서 끝 이동시 이동실패가 되어서 갈수 있는데 까지 일단 가봄
	// 패스 마지막 위치가 원래 목적지 +  acceptance 안에 안들어가면 해당 flag on
	if (movement->GetStateAutoMove().GetIsNotReachTargetPos() == true)
	{					
		// 목적지 근방이 되었을 때(패스 마지막 위치 근처)
		if (true == FGsMove::IsAlmostPathMoveArrived(controller, moveData, movement->GetStateAutoMove(), 100.0f))
		{
			// 종료 콜백을 막아둠(??)
			movement->SetIsLockFinishCallback(true);
			// 다시 이동 시도
			MoveToReservedMoveData(Owner);
		}
	}
	else
	{
		// not check
		// 1. not warp move		
		// 2. not detour move (회피기동)
		if (isMoveToWarp == false && isEvasiveManeuver == false)
		{
			// 3. interaction move to pos
			// 4. almost reach goal pos
			// 5. next move->actor move process
			if (FGsInteractionHandlerLocalPlayer* interactionHandler = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>())
			{
				if (interactionHandler->IsReserveInteraction() &&
					interactionHandler->GetReserveMoveType() == EGsInteractionMoveType::PosMove)
				{
					float addAcceptanceDistance = 10.0f;
					if (const UGsGlobalConstant* data = GData()->GetGlobalData())
					{
						addAcceptanceDistance = data->_interactionPosToTargetMoveAcceptanceDistance;
					}

					if (true == FGsMove::IsAlmostPathMoveArrived(controller, moveData, movement->GetStateAutoMove(), addAcceptanceDistance))
					{
						int targetTblId = interactionHandler->GetReserveInfo()._target._tblId;
						// check need move range
						if (false == FGsStateLocalPlayerInteraction::IsInsideInteractionRangeByTableId(Owner, targetTblId))
						{
							// target 이동으로 돌리기위해 다시 시도											
							interactionHandler->TryInteractionNpcTblId(
								interactionHandler->GetReserveInfo()._target._tblId, EGsInteractionMoveType::TargetMove);

						}
					}
				}
			}
		}
	}



	// 간혹 MovementHandler와 싱크가 안맞는 경우가 있는듯
	// 1. auto on이거나
	// 2. 퀘스트 완료후 이동때 못주운 아이템있는지(완료하자마자 바로 이동시켜서 or 드롭 item 쿨타임)
	// 3. not looting auto move
	// 4. not auto loot cool time
	// 5. not auto return move
	// 6. not have target // hold
	// 7. quest kill move(enemy exist)
	// 8. pause end time

	// 2~8: -> ai

	// https://jira.com2us.com/wiki/pages/viewpage.action?pageId=321265741
	// state auto move -> ai state manager

	if (false == movement->IsAutoMove())
	{
		FGsGameObjectStateManager* fsm = Owner->GetBaseFSM();
		fsm->ProcessEvent(EGsStateBase::AutoMoveStop);
	}

	// 제자리뛰기 검사 (특정 좌표로 이동하는 경우만 반영)
	if (moveData.IsValid() && movement->CheckMoveIdle())
	{
		MoveToReservedMoveData(Owner);
	}
}

void FGsStateLocalPlayerAutoMove::Exit(UGsGameObjectBase* Owner)
{
	// Movement 클래스에 이동 설정 알림
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	AGsPlayerController* controller = movement->GetPlayerController();
	controller->StopMovement();
	movement->SetMoveState(false);
	movement->Stop();

	if (FGsSkillHandlerLocalPlayer* skillHandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>())
	{
		skillHandler->SetAutoIdleEmotionEnable(true);
	}

	local->SetFairyVehicleCondition(EGsFairyVehicleCondition::MOVE_STATE, true);
	// 페어리 버프 이펙트 제거
	local->OnRestoreVehicleEffect();

	FGsGameObjectMessageParamBuffSkill messageParam(Owner, 0, FGsGameObjectMessageParamBuffSkill::BuffSkillAuto::Disable);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_AUTOSKILL_UPDATE, &messageParam);

	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_AUTOMOVE_STOP, nullptr);

	Super::Exit(Owner);
}

void FGsStateLocalPlayerAutoMove::MoveToReservedMoveData(UGsGameObjectBase* Owner)
{
	// 자동이동
	UGsGameObjectLocalPlayer* local			= Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsMovementHandlerLocalPlayer* movement	= local->GetCastMovement<FGsMovementHandlerLocalPlayer>();

	AGsPlayerController* controller		= movement->GetPlayerController();
	const FAIMoveRequest& moveData	= movement->GetStateAutoMove().GetDataRequest();

	// 요청 데이터가 초기화 안되어있으면 스톱
	if (moveData.IsValid() == false)
	{
		//GSLOG(Warning, TEXT("[AUTO] FGsStateLocalPlayerAutoMove::MoveToReservedMoveData move data is not valid"));
		movement->Stop();
		return;
	}
	// 액터 타입일 경우 액터의 존재유무 검사
	if (moveData.IsMoveToActorRequest())
	{
		AActor* targetActor = moveData.GetGoalActor();

		if (false == targetActor->IsValidLowLevel() || false == targetActor->IsActorInitialized() ||
			true == targetActor->IsPendingKillPending())
		{
			//GSLOG(Warning, TEXT("[AUTO] FGsStateLocalPlayerAutoMove::MoveToReservedMoveData move data error"));
			movement->Stop();
			return;
		}
	}

	// 2023/10/13 PKT - 회피 기동이 동작 중이라면 비용 무시
	bool isIgnoreCost = (local->GetAIAvoidCollision()) ? local->GetAIAvoidCollision()->IsAvoidingCollision() : false;
	
	//GSLOG(Warning, TEXT("[AUTO] FGsStateLocalPlayerAutoMove::MoveToReservedMoveData moveTo"));
	FNavPathSharedPtr path;
	FPathFollowingRequestResult result = controller->MoveTo(moveData, &path, isIgnoreCost);
		
	// 임시 분기 처리
	if (IsBranch(EGsGameClientBranchType::TEST_AUTO_MOVE_IDLE_CHECK))
	{
		// 이동 요청 실패 또는 이미 도착
		// 이동 요청 실패 또는 이미 도착
		if (result.Code == EPathFollowingRequestResult::Failed ||
			result.Code == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			GSLOG(Error, TEXT("controller->MoveTo errcode: %d"), result.Code);
			movement->Stop();
			return;
		}
		else
		{
			// 동적 네비데이터 변경으로 다른 경로를 찾는것을 막는다.
			// @see : ENavPathUpdateType::NavigationChanged	
			if (path.IsValid())
			{
				path.Get()->SetIgnoreInvalidation(true);
			}

			movement->SetCurrentNavPath(path);
		}
	}
	else
	{
		// 이동 요청 실패만
		if (result.Code == EPathFollowingRequestResult::Failed)
		{
			GSLOG(Error, TEXT("controller->MoveTo errcode: %d"), result.Code);
		}
		else
		{
			// 동적 네비데이터 변경으로 다른 경로를 찾는것을 막는다.
			// @see : ENavPathUpdateType::NavigationChanged	
			if (path.IsValid())
			{
				path.Get()->SetIgnoreInvalidation(true);
			}

			movement->SetCurrentNavPath(path);
		}
	}
	

#ifdef TEST_AUTO_MOVE_LOG
	GSLOG(Warning, TEXT("<<<< [MOVE TEST] FGsStateLocalPlayerAutoMove MoveToReservedMoveData - errcode: %d, targetName: %s"), 
		result.Code, *moveData.GetGoalActor()->GetName());
#endif

	bool isMoveSuccess = (result.Code == EPathFollowingRequestResult::RequestSuccessful) ? true : false;

	FGsPrimitiveInt32 param(static_cast<int32>(isMoveSuccess));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_AUTOMOVE_REQUESTED, &param);
}

bool FGsStateLocalPlayerAutoMove::IsShapeChangedLock(class UGsGameObjectBase* Owner) const
{
	if (FGsAbnormalityHandlerBase* abnormalityHandler = Owner->GetAbnormalityHandler())
	{
		return abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::CC_POLYMORP);
	}
	return false;
}
void FGsStateLocalPlayerAutoMove::IgnoreState(UGsGameObjectBase* Owner,  IGsStateBase* ChangeState)
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_AUTOMOVE_IGNORED, nullptr);
}