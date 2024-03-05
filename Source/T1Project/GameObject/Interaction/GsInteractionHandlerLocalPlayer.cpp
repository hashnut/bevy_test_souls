#include "GsInteractionHandlerLocalPlayer.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGameClientVersion.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsCampManager.h"
#include "Management/ScopeGame/GsSanctumManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "Message/GsMessageInput.h"
#include "Message/GsMessageContentHud.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsAIMessageParam.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectPropInteraction.h"
#include "GameObject/ObjectClass/GsGameObjectCamp.h"
#include "GameObject/ObjectClass/GsGameObjectSanctumBase.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "DataCenter/Public/DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"

#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDQuestDistance.h"

#include "DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "AI/Function/GsAIFunc.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "ControllerEx/GsPlayerController.h"

#include "UTIL/GsMove.h"
#include "UTIL/GsClassUtil.h"

#include "Net/GsNetSendServiceWorld.h"
#include "GsInteractionNpcInfo.h"

#include "DrawDebugHelpers.h"


// 인터랙션 범위 구하기
float FGsInteractionHandlerLocalPlayer::GetInteractionRange(const FGsSchemaNpcData* In_npcData, UGsGameObjectBase* In_Target, bool In_isOrigin)
{
	if (nullptr == In_npcData)
	{
		return 0.f;
	}

	if (NpcFunctionType::CAMP == In_npcData->npcFunctionType)
	{
		float scale = In_isOrigin ? 1.f : GData()->GetGlobalData()->LocalPlayerMoveToTargetGoalScale_Camp;

		return nullptr == In_Target ? In_npcData->interactionRange * scale :
			GSCamp()->GetCampInteractionRange(In_npcData, In_Target) * scale;
	}
	else if (NpcFunctionType::SANCTUM == In_npcData->npcFunctionType ||
		NpcFunctionType::SANCTUM_NEXUS == In_npcData->npcFunctionType)
	{
		// 주의: : GlobalData의 캠프 값을 그대로 썼다. 필요 시 새 값을 추가
		float scale = In_isOrigin ? 1.f : GData()->GetGlobalData()->LocalPlayerMoveToTargetGoalScale_Camp;

		if (In_Target)
		{
			if (UGsGameObjectSanctumBase* sanctum = In_Target->CastGameObject<UGsGameObjectSanctumBase>())
			{
				return sanctum->GetInteractionRange() * scale;
			}
		}

		return In_npcData->interactionRange * scale;
	}

	float scale = In_isOrigin ? 1.f : GData()->GetGlobalData()->LocalPlayerMoveToTargetGoalScale;

	return In_npcData->interactionRange * scale;
}

/*
float FGsInteractionHandlerLocalPlayer::GetInteractionRange(const FGsSchemaNpcData* In_npcData, bool In_isOrigin )
{
	if (nullptr == In_npcData)
	{
		return 0.f;
	}
	float scale = (In_isOrigin) ? 1.f : GData()->GetGlobalData()->LocalPlayerMoveToTargetGoalScale;
	return In_npcData->interactionRange * scale;
}
*/

float FGsInteractionHandlerLocalPlayer::GetInteractionRange(const FGsSchemaPropData* In_propData, bool In_isOrigin)
{
	if (nullptr == In_propData)
	{
		return 0.f;
	}
	float scale = (In_isOrigin) ? 1.f : GData()->GetGlobalData()->LocalPlayerMoveToTargetGoalScale;
	return In_propData->touchInteractionRange * scale;
}

// 초기화
void FGsInteractionHandlerLocalPlayer::Initialize(UGsGameObjectBase* In_owner)
{
	Super::Initialize(In_owner);	
	_local = Cast<UGsGameObjectLocalPlayer>(In_owner);

	FGsMessageHolder* msg = GMessage();
	_actionDelegates.Emplace(
		msg->GetInput().AddRaw(MessageInput::PressJoystick,
			this, &FGsInteractionHandlerLocalPlayer::CallbackPressJoystick));


	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_AUTOMOVE_IGNORED,
			this, &FGsInteractionHandlerLocalPlayer::OnLocalAutoMoveIgnored));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AUTO_MOVE_CONTENTS_TYPE_CHANGED,
			this, &FGsInteractionHandlerLocalPlayer::OnAutoMoveContentsTypeChanged));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
			this, &FGsInteractionHandlerLocalPlayer::OnLocalSpawnMeComplete));
}

void FGsInteractionHandlerLocalPlayer::Finalize()
{
	// 메세지 해제
	FGsMessageHolder* msg = GMessage();
	for (TPair<MessageInput, FDelegateHandle>& el : _actionDelegates)
	{
		msg->GetInput().Remove(el);
	}
	_actionDelegates.Empty();

	for (MsgGameObjHandle& el : _gameobjectDelegates)
	{
		msg->GetGameObject().Remove(el);
	}
	_gameobjectDelegates.Empty();

	if (_autoMoveRequestedDelegate.Value.IsValid() &&
		_isAutoMoveRequest == true)
	{
		msg->GetGameObject().Remove(_autoMoveRequestedDelegate);

		_isAutoMoveRequest = false;
	}

	Super::Finalize();
}

void FGsInteractionHandlerLocalPlayer::ClearTarget()
{
	// 예약된 정보 확인하여 같은 타겟이면 클리어 처리
	if (IsReserveData())
	{
		const FGsReserveInteractionInfo& reserveInteractionInfo = GetReserveInfo();
		if (reserveInteractionInfo._moveType == EGsInteractionMoveType::TargetMove)
		{
			if (UGsGameObjectBase* interactionTarget = GetInteractionTarget())
			{
				ClearReserveData();
			}
		}
	}

	Super::ClearTarget();
}

void FGsInteractionHandlerLocalPlayer::StartInteraction(UGsGameObjectBase* In_taret)
{
	Super::StartInteraction(In_taret);
}

void FGsInteractionHandlerLocalPlayer::EndInteraction(UGsGameObjectBase* In_taret)
{
	Super::EndInteraction(In_taret);

	// 타이머 비활성
	_timer.Clear();
}

void FGsInteractionHandlerLocalPlayer::CancelInteration(UGsGameObjectBase* In_taret)
{
	// Prop 인터랙션 종료 메세지
	if (_interactionTarget.IsValid() 
	&& _interactionTarget.Get()->GetActor()) // 없으면 안되는데...
	{
		// 타이머 비활성
		_timer.Clear();

		FGsGameObjectData* data = _interactionTarget->GetData();
		FGsPropInteractionMessageParamContents param(data->GetGameId(), data->GetTableId());

		// 켄슬 전용 메세지 전송
		GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_CANCEL, &param);
	}

	// 인터렉션중에 새로운 인터렉션을 시도할시 캔슬 패킷을 전송한다. 
	// 따라서 타겟에 대한 검사를 해서 지울지(선행) 말지(후행) 판단한다.
	// 기존 ClearReserveInteraction 위치는 캔슬 패킷전송시 처리했었음
	if (In_taret == _interactionTarget)
	{
		ClearReserveInteraction();
	}
	
	Super::CancelInteration(In_taret);
}

void FGsInteractionHandlerLocalPlayer::CancelInteration()
{
	// Prop 인터랙션 종료 메세지
	if (_interactionTarget.IsValid() 
	&& _interactionTarget.Get()->GetActor()) // 없으면 안되는데...
	{
		// 타이머 비활성
		_timer.Clear();

		FGsGameObjectData* data = _interactionTarget->GetData();
		FGsPropInteractionMessageParamContents param(data->GetGameId(), data->GetTableId());

		// 켄슬 전용 메세지 전송
		GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_CANCEL, &param);
	}

	UGsGameObjectBase* interactionTarget = GetInteractionTarget();
	if (interactionTarget
	&& interactionTarget->IsObjectType(EGsGameObjectType::PropInteraction))
	{
		//StateLocalPlayerLooting을 호출하나 FSM Update가 한틱이 느리기 때문에 
		//cancel을 정상 호출하지 않음		
		FGsNetSendServiceWorld::SendInteractPropCancel(Cast<UGsGameObjectPropObject>(interactionTarget));
	}

	Super::CancelInteration();	
}

// 공통 인터액션 처리
void FGsInteractionHandlerLocalPlayer::TryInteraction(EGsGameObjectType In_type, UGsGameObjectBase* In_target, 
	EGsInteractionMoveType In_moveType, int In_tblId, int In_mapId, FVector In_pos)
{
	// 진행중인 인터랙션 정보 초기화(Timer)
	_timer.Clear();

	int currMapId = GLevel()->GetCurrentLevelId();

	// check old data(interaction target marker off)
	if (_interactionTarget != nullptr)
	{
		_interactionTarget->SetVisibleInteractionMoveTargetMarker(false);
	}
	if (IsReserveInteraction() == true &&
		EGsInteractionMoveType::PosMove == GetReserveMoveType())
	{		
		if (currMapId == _reserveInteractionInfo._mapId)
		{
			if (UGsGameObjectBase* nonPlayer = GSGameObject()->FindObjectNearestNonPlayerByNPCId(
				_local->GetActor(), _reserveInteractionInfo._target._tblId))
			{
				nonPlayer->SetVisibleInteractionMoveTargetMarker(false);
			}
		}
	}	

	// 저장
	_interactionTarget = In_target;

	if (_interactionTarget == nullptr)
	{
		if (currMapId == In_mapId)
		{			
			if (UGsGameObjectBase* nonPlayer = GSGameObject()->FindObjectNearestNonPlayerByNPCId(
				_local->GetActor(), In_tblId))
			{
				nonPlayer->SetVisibleInteractionMoveTargetMarker(true);
			}		
		}
	}
	else
	{
		// show intearction move target marker
		_interactionTarget->SetVisibleInteractionMoveTargetMarker(true);
	}

	// 예약
	_reserveInteractionInfo._isMoveComplete = false;
	_reserveInteractionInfo._moveType = In_moveType;
	_reserveInteractionInfo._addAcceptanceRadius = 0.0f;
	_reserveInteractionInfo._mapId = In_mapId;
	_reserveInteractionInfo._target._tblId = In_tblId;
	_reserveInteractionInfo._target._gameObjectType = In_type;
	_reserveInteractionInfo._pos = In_pos;

	// 인터랙션 상태로 전환
	FGsGameObjectStateManager::ProcessEvent(_local.Get(), EGsStateBase::Interaction);
}

// 인터랙션 시도(Npc 타겟)
void FGsInteractionHandlerLocalPlayer::TryInteractionNpcData(UGsGameObjectBase* In_target, const FGsSchemaNpcData* In_npcData)
{
	EGsGameObjectType objectType = In_target->GetObjectType();
	if (EGsGameObjectType::Camp != objectType &&
		EGsGameObjectType::Sanctum != objectType)
	{
		objectType = EGsGameObjectType::NonPlayer;
	}

	TryInteraction(objectType, In_target, EGsInteractionMoveType::TargetMove);
	// 데이터 설정
	_reserveInteractionInfo._target._npcData = In_npcData;
}

void FGsInteractionHandlerLocalPlayer::TryInteractionPropData(UGsGameObjectBase* In_target, const FGsSchemaPropData* In_propData)
{
	//모든 컨첸츠에 대해서 확인하지 못한 코드, 일단 프랍 타입만 체크
	IGsGameObjectInteractionInterface* interaction = Cast<IGsGameObjectInteractionInterface>(In_target);
	if (interaction)
	{
		if (false == interaction->IsContentsUnlock())
		{
			return;
		}
	}

	TryInteraction(In_target->GetObjectType(), In_target, EGsInteractionMoveType::TargetMove);
	// 데이터 설정
	_reserveInteractionInfo._target._propData = In_propData;
}

// 인터랙션 시도(위치)
void FGsInteractionHandlerLocalPlayer::TryInteractionPos(EGsGameObjectType In_type, int In_tblId, int In_mapId,FVector In_pos, float In_acceptanceRadius)
{
	TryInteraction(In_type, nullptr, EGsInteractionMoveType::PosMove, In_tblId, In_mapId, In_pos);
	_reserveInteractionInfo._addAcceptanceRadius = In_acceptanceRadius;
}

// 인터랙션 이동
void FGsInteractionHandlerLocalPlayer::OnMoveToInteractionGameObject(UGsGameObjectBase* In_target)
{
	if (In_target && _local.IsValid())
	{
		AActor* targetActor = In_target->GetActor();
		if (targetActor && true == targetActor->IsActorInitialized() &&
			false == targetActor->IsPendingKillPending())
		{
			const FGsReserveInteractionInfo& reserveData = GetReserveInfo();
			// 현재 예약된 타겟 타입으로 도착 범위를 설정

			float acceptanceRadius = 0.f;
			if (reserveData._target._gameObjectType == EGsGameObjectType::NonPlayer || 
				reserveData._target._gameObjectType == EGsGameObjectType::Camp ||
				reserveData._target._gameObjectType == EGsGameObjectType::Sanctum)
			{
				acceptanceRadius = GetInteractionRange(reserveData._target._npcData, GetInteractionTarget());
					//, 
					//EGsGameObjectType::Camp == reserveData._target._gameObjectType);
			}
			else
			{
				acceptanceRadius = GetInteractionRange(reserveData._target._propData);
			}
			float targetR = In_target->GetData()->GetScaledCapsuleRadius();
			
			FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();

			EGsGameObjectType objectType = In_target->GetObjectType();

			// 인터랙션 이동(gameobject) 시작때 나머지 reserve 데이터 삭제(현재는 waypoint, input handler (터치 이동) 이동만 처리)
			// https://jira.com2us.com/jira/browse/C2URWQ-6334
			_local->ClearReserveDataByReserveWork(this);


			if (objectType != EGsGameObjectType::NonPlayer && objectType != EGsGameObjectType::Camp &&
				objectType != EGsGameObjectType::Sanctum)
			{
				movement->ChangeAutoMoveState().StartActor(_local.Get()
					, In_target->GetActor(), acceptanceRadius,
					FGsAutoMoveCompletedDelegate::CreateRaw(this, &FGsInteractionHandlerLocalPlayer::CallbackMoveToInteractionEnd),
					FGsAutoMoveCompletedOneDelegate::CreateRaw(this, &FGsInteractionHandlerLocalPlayer::CallbackMoveToInteractionFailed));
			}
			else
			{
				
				FVector shiftTargetPos = In_target->GetActor()->GetActorLocation();
				float newAcceptanceRadius = acceptanceRadius;

				GetShiftTargetNonplayerPosAndAcceptanceRaidus(
					_local.Get(), In_target, acceptanceRadius, shiftTargetPos, newAcceptanceRadius);

				movement->ChangeAutoMoveState().StartPos(_local.Get(),
					shiftTargetPos, newAcceptanceRadius,
					FGsAutoMoveCompletedDelegate::CreateRaw(this, &FGsInteractionHandlerLocalPlayer::CallbackMoveToInteractionEnd),
					FGsAutoMoveCompletedOneDelegate::CreateRaw(this, &FGsInteractionHandlerLocalPlayer::CallbackMoveToInteractionFailed));
			}
		}
	}
}

float FGsInteractionHandlerLocalPlayer::GetInteractionRangeReserveNpcData()
{
	const FGsReserveInteractionInfo& reserveData = GetReserveInfo();
	return  GetInteractionRange(reserveData._target._npcData, GetInteractionTarget());
}
FVector FGsInteractionHandlerLocalPlayer::GetShiftTargetNonplayerPos(
	UGsGameObjectBase* In_local, UGsGameObjectBase* In_target, float In_acceptanceRaidus, 
	bool In_isTestDrawLine)
{
	FVector shifTargetPos = FVector::ZeroVector;

	// sentry 크래쉬 대응
	if (In_local == nullptr || In_target == nullptr ||
	In_local->GetActor() == nullptr || In_target->GetActor() == nullptr)
	{
		return shifTargetPos;
	}


	FVector localPos = In_local->GetActor()->GetActorLocation();
	FVector targetPos = In_target->GetActor()->GetActorLocation();
	localPos.Z = targetPos.Z;

	FVector rotVec = localPos - targetPos;

	rotVec.Normalize();

	// 1. try local - npc direction pos 
	shifTargetPos = targetPos + rotVec * In_acceptanceRaidus;

	// check move possible

	UGsGameObjectLocalPlayer* localObject = Cast<UGsGameObjectLocalPlayer>(In_local);

	if (localObject == nullptr)
	{
		return shifTargetPos;
	}

	AGsCharacterLocalPlayer* localChar = localObject->GetLocalCharacter();
	if (localChar == nullptr)
	{
		return shifTargetPos;
	}
	AGsPlayerController* controller = localChar->GetPlayerController();
	if (controller == nullptr)
	{
		return shifTargetPos;
	}

	bool isMovePossible = FGsMove::IsMovePossible(controller,
		localChar,
		shifTargetPos,
		localPos,
		In_acceptanceRaidus,
		In_isTestDrawLine);

	// 2. try npc opposite direction pos
	if (isMovePossible == false)
	{
		FVector forwardVector = In_target->GetActor()->GetActorForwardVector();
		shifTargetPos = targetPos + forwardVector * In_acceptanceRaidus;
	}

	return shifTargetPos;
}
bool FGsInteractionHandlerLocalPlayer::GetShiftTargetNonplayerPosAndAcceptanceRaidus(
	UGsGameObjectBase* In_localGameObject,
	UGsGameObjectBase* In_targetGameObject,
	float In_acceptanceRadius,
	FVector& Out_newShiftPos, float& Out_newRadius)
{
	if (In_localGameObject == nullptr ||
		In_targetGameObject == nullptr)
	{
		return false;
	}

	UGsGameObjectLocalPlayer* local = In_localGameObject->CastGameObject<UGsGameObjectLocalPlayer>();

	if (nullptr == local)
	{
		return false;
	}

	float rateNpcInteractionShiftPos =
		GData()->GetGlobalData()->_rateNpcInteractionShiftPos;

	float shiftDist = In_acceptanceRadius * rateNpcInteractionShiftPos;

	bool testDebugDraw = false;
	if (FGsInteractionHandlerLocalPlayer* localInteraction =
		local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>())
	{
		if (localInteraction->GetIsTestShowInteractionSphere() == true)
		{
			testDebugDraw = true;
		}
	}

	FVector shiftTargetPos = GetShiftTargetNonplayerPos(In_localGameObject, In_targetGameObject, shiftDist, testDebugDraw);

	float newAcceptanceRadius = FMath::Max(In_acceptanceRadius - shiftDist, 0.0f);

	Out_newShiftPos = shiftTargetPos;
	Out_newRadius = newAcceptanceRadius;

	
	if (testDebugDraw == true)
	{
		DrawDebugSphere(In_localGameObject->GetWorld(), shiftTargetPos, newAcceptanceRadius, 8, FColor::Yellow, false, 10.0f);

		DrawDebugSphere(In_localGameObject->GetWorld(), In_targetGameObject->GetActor()->GetActorLocation(), In_acceptanceRadius, 8, FColor::Green, false, 10.0f);
	}
	

	return true;

}
// 인터랙션 이동(위치)
void FGsInteractionHandlerLocalPlayer::OnMoveToInteractionPos(int In_mapId, FVector In_targetPos, float In_addAcceptanceRadius)
{
	if (_local.IsValid())
	{
		FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();

		//float acceptanceRadius = FGsMove::GetMovePosRange() + In_addAcceptanceRadius;
		float acceptanceRadius = In_addAcceptanceRadius;

		if (_isTestShowInteractionSphere == true)
		{
			DrawDebugSphere(_local->GetWorld(), In_targetPos, 10.0f, 8, FColor::Red, false, 10.0f);
		}

		// 2022/11/15 PKT - https://jira.com2us.com/jira/browse/C2URWQ-2024
		//					하드 코딩. 근본적으로 문제를 해결 해야 함. MoveStateAuto::ClearOldFailDelegate 내부에 Delegate 검사 부분이 문제.
		//					히스토리를 알수 없어 깊고 수정하진 못했음.
		HideNpcListMoveInfo();

		// 인터랙션 이동(pos) 시작때 나머지 reserve 데이터 삭제(현재는 waypoint, input handler (터치 이동) 이동만 처리)
		// https://jira.com2us.com/jira/browse/C2URWQ-6334
		_local->ClearReserveDataByReserveWork(this);

		
		movement->ChangeAutoMoveState().StartMapIdPos(_local.Get(),
			In_mapId,
			In_targetPos, acceptanceRadius,
			FGsAutoMoveCompletedDelegate::CreateRaw(this,
				&FGsInteractionHandlerLocalPlayer::CallbackMoveToInteractionEnd),
			FGsAutoMoveCompletedOneDelegate::CreateRaw(this,
				&FGsInteractionHandlerLocalPlayer::CallbackMoveToInteractionFailed));

		
	}
}

bool FGsInteractionHandlerLocalPlayer::IsInsideInteractionRange()
{
	float distanceSq = FVector::DistSquared2D(_interactionTarget->GetLocation(), _local->GetLocation());
	float interactionRange = GetInteractionRange(_reserveInteractionInfo._target._npcData, GetInteractionTarget(), true);
	if (distanceSq <= interactionRange * interactionRange)
	{
		return true;
	}
	return false;
}

void FGsInteractionHandlerLocalPlayer::CallbackMoveToInteractionFailed(const FPathFollowingResult& Result)
{
	if (nullptr != _local && false != _interactionTarget.IsValid())
	{
		if (IsInsideInteractionRange())
		{
			// https://jira.com2us.com/jira/browse/C2URWQ-3437
			// 1. hide npc move info
			// 2. move npc function slot update
			HideNpcListMoveInfo();
			GSAI()->NpcListMoveFailed(EGsMoveFailedReason::ControllerAutoMoveFailed);
			return;
		}
	}

	// auto move state exit -> controller stopmovement -> (moving) failed called
	//https://jira.com2us.com/jira/browse/C2URWQ-1302


	// 피격시 자동 이동 취소 되는 현상 수정하기 위해
	// 여기서 데이터 클리어 안함
	// 1. 수동 입력(이동, 스킬)시 클리어
	// 2. 다른 reserve data 입력시 클리어
	// https://jira.com2us.com/jira/browse/C2URWQ-6334

	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

	// 퀘스트 이동 실패 메시지 전송(2. 실패)
	FGsMoveFailedMessageParam param(EGsMoveFailedReason::ControllerAutoMoveFailed, Result.Code);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AUTO_MOVE_FAILED, &param);

#ifndef TEST_CALLSTACK
	//FGsClassUtil::PrintCallStack();
#endif

	HideNpcListMoveInfo();
}

void FGsInteractionHandlerLocalPlayer::CallbackMoveToInteractionEnd()
{
	_reserveInteractionInfo._isMoveComplete = true;
	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

	// 자동 이동 도착후 자동 이동 타입 none으로 초기화
	// https://jira.com2us.com/jira/browse/C2URWQ-3193
	if(FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
	{
		movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::None);
	}


	HideNpcListMoveInfo();
}
// 예약된 데이터 클리어
void FGsInteractionHandlerLocalPlayer::ClearReserveInteraction()
{
	if (_interactionTarget == nullptr)
	{
		int currMapId = GLevel()->GetCurrentLevelId();
		if (currMapId == _reserveInteractionInfo._mapId)
		{
			if (UGsGameObjectBase* nonPlayer = GSGameObject()->FindObjectNearestNonPlayerByNPCId(
				_local->GetActor(), _reserveInteractionInfo._target._tblId))
			{
				// hide intearction move target marker
				nonPlayer->SetVisibleInteractionMoveTargetMarker(false);
			}
		}
	}
	else
	{
		// hide intearction move target marker
		_interactionTarget->SetVisibleInteractionMoveTargetMarker(false);
	}


	_reserveInteractionInfo._isMoveComplete = false;
	_reserveInteractionInfo._mapId = 0;
	_reserveInteractionInfo._addAcceptanceRadius = 0.0f;
	_reserveInteractionInfo._target.Clear();
}

void FGsInteractionHandlerLocalPlayer::TryInteractionInternal(UGsGameObjectBase* In_target)
{
	if (_reserveInteractionInfo._moveType == EGsInteractionMoveType::TargetMove)
	{
		if (_reserveInteractionInfo._target._gameObjectType == EGsGameObjectType::NonPlayer)
		{
			TryInteractionNpcData(In_target, _reserveInteractionInfo._target._npcData);
		}
		else if (In_target->IsObjectType(EGsGameObjectType::PropInteraction))
		{
			TryInteractionPropData(In_target, _reserveInteractionInfo._target._propData);
		}
	}
	else if(_reserveInteractionInfo._moveType == EGsInteractionMoveType::PosMove)
	{
		TryInteractionPos(_reserveInteractionInfo._target._gameObjectType, _reserveInteractionInfo._target._tblId,
			_reserveInteractionInfo._mapId, _reserveInteractionInfo._pos, _reserveInteractionInfo._addAcceptanceRadius);
	}
}

void FGsInteractionHandlerLocalPlayer::Update(float In_delta)
{
	//if (IsReserveInteraction())
	//{
	//	// 도착했으면 다시 시도
	//	if (_reserveInteractionInfo._isMoveComplete == true)
	//	{
	//		TryInteractionInternal(_interactionTarget);
	//	}
	//}
}

// 예약된게 있나
bool FGsInteractionHandlerLocalPlayer::IsReserveInteraction()
{
	return _reserveInteractionInfo._target._gameObjectType != EGsGameObjectType::Base;
}

// 예약 데이터 있는가
bool FGsInteractionHandlerLocalPlayer::IsReserveData()
{
	return IsReserveInteraction();
}
// 재시도 해라
void FGsInteractionHandlerLocalPlayer::DoRetry()
{
	if (false == _interactionTarget.IsValid())
	{
		GSLOG(Error, TEXT("Target is missing"));
		return;
	}

	TryInteractionInternal(_interactionTarget.Get());
}

// npc list 이동 거리
bool FGsInteractionHandlerLocalPlayer::GetNpcListMoveTargetDist(OUT float& Out_remainDist)
{
	if (_local == nullptr)
	{
		return false;
	}

	FVector2D targetPos = FVector2D::ZeroVector;
	if (_reserveInteractionInfo._moveType == EGsInteractionMoveType::TargetMove)
	{
		if (_interactionTarget == nullptr)
		{
			return false;
		}
		targetPos = FVector2D(_interactionTarget->GetLocation());
		
	}
	else if(_reserveInteractionInfo._moveType == EGsInteractionMoveType::PosMove)
	{
		targetPos = FVector2D(_reserveInteractionInfo._pos);
	}

	FVector2D localPos = FVector2D(_local->GetLocation());

	Out_remainDist = (localPos - targetPos).Size();

	return true;
}

bool FGsInteractionHandlerLocalPlayer::IsInteraction() const
{
	return _interactionTarget.IsValid() && _timer.IsValid();
}

void FGsInteractionHandlerLocalPlayer::StartTimer(float In_EndTime)
{
	_timer._isEnable = true;
	_timer._endTime = In_EndTime;
	_timer._currTime = 0.f;
}

double FGsInteractionHandlerLocalPlayer::GetElapsed() const
{ 
	return _timer._currTime;
}

void FGsInteractionHandlerLocalPlayer::UpdateTimer(float In_Delta)
{
	if (_timer._isEnable)
	{
		// 타이머 갱신
		_timer._currTime += In_Delta;

		// 종료 확인
		if (_timer._currTime >= _timer._endTime)
		{
#if PROP_INTERACTION_DEBUG && WITH_EDITOR
			if (_interactionTarget.IsValid())
			{
				GSLOG(Log, TEXT("Prop interaction - label : %s\tid : %lld\tcurrent time : %f")
					, *_interactionTarget->GetActor()->GetActorLabel()
					, _interactionTarget.Get()->GetGameId()
					, _timer._currTime);
			}
#endif
			_timer._isEnable = false;
			_timer._currTime = -1.f;

			if (_interactionTarget.IsValid()
			&& UGsGameObjectBase::IsVaildClass(_interactionTarget.Get()))
			{
				FGsGameObjectData* data = _interactionTarget->GetData();
				FGsPropInteractionMessageParamContents param(data->GetGameId(), data->GetTableId());
				// Prop 인터랙션 종료 메세지
				GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_END, &param);

				// 테스트				
				//FGsNetSendServiceWorld::SendInteractPropEnd(data->GetGameId(), 0);
			}
			else
			{
				GSLOG(Error, TEXT("Interaction Target is Destroy"));
			}
		}
	}
}

void FGsInteractionHandlerLocalPlayer::CallbackTargetClearObject(const IGsMessageParam* inParam)
{
	if (nullptr == inParam)
	{
		return;
	}

	const FGsGameObjectMessageParam* castParam = inParam->Cast<const FGsGameObjectMessageParam>();
	if (_interactionTarget.IsValid()
	&& UGsGameObjectBase::IsVaildClass(_interactionTarget.Get())
	&& _interactionTarget == castParam->_paramOwner)
	{
		// 현재 인터랙션 중이었으면 Cancel
		if (_timer._isEnable)
		{
			// 켄슬 패킷 요청 보내기
			FGsNetSendServiceWorld::SendInteractPropCancel(Cast<UGsGameObjectPropObject>(_interactionTarget));
		}

		// 관련 정보 제거
		ClearReserveInteraction();
		_interactionTarget = nullptr;
	}
}

void FGsInteractionHandlerLocalPlayer::CallbackCheckDestroyObject(const IGsMessageParam* inParam)
{
	if (nullptr == inParam)
	{
		return;
	}

	const FGsGameObjectMessageParam* castParam = inParam->Cast<const FGsGameObjectMessageParam>();
	if (_interactionTarget.IsValid()
	&& UGsGameObjectBase::IsVaildClass(_interactionTarget.Get())
	&& _interactionTarget == castParam->_paramOwner)
	{
		// 현재 인터랙션 중이었으면 Cancel
		if (_timer._isEnable)
		{
			// 켄슬 패킷 요청 보내기
			FGsNetSendServiceWorld::SendInteractPropCancel(Cast<UGsGameObjectPropObject>(_interactionTarget));
		}
		// 관련 정보 제거
		ClearReserveInteraction();
		_interactionTarget = nullptr;
	}
}

void FGsInteractionHandlerLocalPlayer::CallbackPressJoystick(const FGsInputEventMsgBase& In_msg)
{
	ClearReserveDataAndAutoMoveStop();	
}


// npc tbl id로 인터랙션
void FGsInteractionHandlerLocalPlayer::TryInteractionNpcTblId(int In_npcTblId,
	EGsInteractionMoveType In_interactionType)
{	
	// 현재맵 npc id로 간다
	int currMapId = GLevel()->GetCurrentLevelId();

	TryInteractionNpcTblIdWithMapId(In_npcTblId, currMapId, In_interactionType);
}

void FGsInteractionHandlerLocalPlayer::TryInteractionNpcTblIdWithMapId(int In_npcTblId, int In_mapId,
	EGsInteractionMoveType In_interactionType, FVector In_otherMapRegionShowPos)
{
	int currMapId = GLevel()->GetCurrentLevelId();

	bool isSameMap = (In_mapId == currMapId) ? true : false;

	// 스폰 되어 있는지 체크
	// npc id로 game object 찾기
	UGsGameObjectBase* nonPlayer = nullptr;
	if (isSameMap == true)
	{
		nonPlayer = GSGameObject()->FindObjectNearestNonPlayerByNPCId(
			_local->GetActor(), In_npcTblId);
	}

	// 없으면 pos로 이동
	if (isSameMap == false || nonPlayer == nullptr)
	{
		// 위치 이동 도달후 타겟 무브 시점에
		// tbl id 대상이 없음
		if (In_interactionType == EGsInteractionMoveType::TargetMove)
		{
			// 예약된거 지우기
			if (IsReserveInteraction())
			{
				ClearReserveInteraction();
			}
			// 실패 이유 있는 메시지 전송()
			// 퀘스트가 받으면 퀘스트 에서 처리
			// npc list 쪽에서 받으면 거기서 처리

			FGsMoveFailedMessageParam param(EGsMoveFailedReason::InteractionNPCNotExist, In_npcTblId);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::AUTO_MOVE_FAILED, &param);

		}
		else
		{
			float addAcceptanceRadius = 0.0f;
			const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(In_npcTblId);
			NpcFunctionType npcFunctionType = NpcFunctionType::NONE;
			if (nullptr != npcData)
			{
				addAcceptanceRadius = FGsInteractionHandlerLocalPlayer::GetInteractionRange(npcData, nonPlayer);
				npcFunctionType = npcData->npcFunctionType;
			}
			FVector npcPos = FVector::ZeroVector;
			if (isSameMap == true)
			{
				if (npcFunctionType == NpcFunctionType::EVENT_SHOP)
				{
					npcPos = GSInteraction()->GetServerNpcPos(In_npcTblId);
				}
				else
				{
					npcPos = GSInteraction()->GetNpcPos(In_npcTblId);
				}
			}
			else
			{
				npcPos = In_otherMapRegionShowPos;
			}
			TryInteractionPos(EGsGameObjectType::NonPlayer, In_npcTblId, In_mapId, npcPos, addAcceptanceRadius);
		}
	}
	// 있으면 
	else
	{
		GSInteraction()->TryInteractionGameObject(nonPlayer);
	}

}


// 자동 진행 퀘스트 인터랙션
void FGsInteractionHandlerLocalPlayer::TryInteractionAIQuest(int In_npcId, int In_mapId,
	const FVector& In_pos, bool In_isCenterNode)
{
	int currMapId = GLevel()->GetCurrentLevelId();

	UGsGameObjectBase* nonPlayer = nullptr;
	if (currMapId == In_mapId)
	{
		// 스폰 되어 있는지 체크
		// npc id로 game object 찾기
		nonPlayer = GSGameObject()->FindObjectNearestNonPlayerByNPCId(
			_local->GetActor(), In_npcId);
	}

	// 없으면 pos로 이동
	// 중간 난입 불가이면 무조건 pos로 간다-> 현 기획
	if (nonPlayer == nullptr ||
		(In_isCenterNode == false))
	{
		float addAcceptanceRadius = 0.0f;
		/*const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(In_npcId);
		if (nullptr != npcData)
		{
			addAcceptanceRadius = FGsInteractionHandlerLocalPlayer::GetInteractionRange(npcData, nonPlayer);
		}*/

		TryInteractionPos(EGsGameObjectType::NonPlayer, In_npcId, In_mapId, In_pos, addAcceptanceRadius);
	}
	// 있으면 
	else
	{
		GSInteraction()->TryInteractionGameObject(nonPlayer);
	}

}

void FGsInteractionHandlerLocalPlayer::ShowNpcListMoveInfo()
{
	if (false == _local.IsValid())
	{
		return;
	}

	FGsMovementHandlerLocalPlayer* movement = _local.Get()->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}

	if (movement->GetStateAutoMove().GetMovementAutoContentsType() != EGsMovementAutoContentsType::NpcListMove)
	{
		return;
	}
	UGsUIHUDQuestDistance* distanceHudUI = GetDistanceHud();
	if (nullptr == distanceHudUI)
	{
		return;
	}
	int tblId = 0;
	if (_reserveInteractionInfo._moveType == EGsInteractionMoveType::PosMove)
	{		
		tblId = _reserveInteractionInfo._target._tblId;
	}
	else
	{
		if (nullptr != _reserveInteractionInfo._target._npcData)
		{
			tblId = _reserveInteractionInfo._target._npcData->id;
		}
	}

	distanceHudUI->ShowNpcListMoveInfo(tblId);
}

void FGsInteractionHandlerLocalPlayer::HideNpcListMoveInfo()
{
	UGsUIHUDQuestDistance* distanceHudUI = GetDistanceHud();
	if (nullptr == distanceHudUI)
	{
		return;
	}
	distanceHudUI->HideNpcListMoveInfo();
}

UGsUIHUDQuestDistance* FGsInteractionHandlerLocalPlayer::GetDistanceHud()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("HUDMain"));
	if (false == widget.IsValid())
	{
		return nullptr;
	}
	UGsUIHUDFrameMain* hudWidget = Cast<UGsUIHUDFrameMain>(widget.Get());
	if (nullptr == hudWidget)
	{
		return nullptr;
	}
	UGsUIHUDQuestDistance* distanceHudUI = hudWidget->GetDistanceHud();
	return distanceHudUI;
}

void FGsInteractionHandlerLocalPlayer::BindAutoMoveMessageByNPCFunctionSlot()
{
	if (_isAutoMoveRequest == true)
	{
		return;
	}

	FGsMessageHolder* msg = GMessage();
	_autoMoveRequestedDelegate=  msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_AUTOMOVE_REQUESTED,
		this, &FGsInteractionHandlerLocalPlayer::OnLocalAutoMoveRequested);

	_isAutoMoveRequest = true;
}

void FGsInteractionHandlerLocalPlayer::OnLocalAutoMoveRequested(const IGsMessageParam* In_isMoveSuccess)
{
	const FGsPrimitiveInt32* param = In_isMoveSuccess->Cast<const FGsPrimitiveInt32>();
	bool isSuccess = static_cast<bool>(param->_data);

	ClearBindAUtoMoveMessageByNPCFunctionSlot();

	if (isSuccess == true)
	{
		FGsAIFunc::PostAutoMoveRequetedNPCFunction();

		ShowNpcListMoveInfo();
	}
}

void FGsInteractionHandlerLocalPlayer::OnLocalAutoMoveIgnored(const IGsMessageParam*)
{
	ClearBindAUtoMoveMessageByNPCFunctionSlot();
}

void FGsInteractionHandlerLocalPlayer::ClearBindAUtoMoveMessageByNPCFunctionSlot()
{
	FGsMessageHolder* msg = GMessage();
	if (_autoMoveRequestedDelegate.Value.IsValid() &&
		_isAutoMoveRequest == true)
	{
		msg->GetGameObject().Remove(_autoMoveRequestedDelegate);
		_isAutoMoveRequest = false;
	}
}

// 스킬 버튼 누름
void FGsInteractionHandlerLocalPlayer::OnPressSkill(const FGsInputEventMsgBase& In_msg)
{
	// 신규 코드
	ClearReserveDataAndAutoMoveStop();
	
}
// 스킬 클릭
void FGsInteractionHandlerLocalPlayer::OnClickSkill(const FGsInputEventMsgBase& In_msg)
{
	// 신규 코드
	ClearReserveDataAndAutoMoveStop();	
}
// 1차 타겟
void FGsInteractionHandlerLocalPlayer::OnLocalTargetChanged(const IGsMessageParam* In_param)
{
	// 신규 코드
	ClearReserveDataAndAutoMoveStop();
}

// 예약된 데이터 클리어해라
void FGsInteractionHandlerLocalPlayer::ClearReserveData()
{
	if (IsReserveInteraction())
	{
		ClearReserveInteraction();
	}	
}

// 예약 데이터 삭제와 auto move stop으로 상태 전환
// 스킬에서 특정 이동일때만 처리하는거 때문에 처리
// https://jira.com2us.com/jira/browse/C2URWQ-6334
void FGsInteractionHandlerLocalPlayer::ClearReserveDataAndAutoMoveStop()
{
	if (true == IsReserveInteraction())
	{
		// 예약된 데이터 삭제
		// https://jira.com2us.com/jira/browse/C2URWQ-6334
		ClearReserveInteraction();

		// 이동중 멈추는 현상 수정중
		// 스킬에서 처리해서 일단 막아놓음
		// https://jira.com2us.com/jira/browse/C2URWQ-6334

		//// auto move state 이면 auto move stop 으로 상태 전환
		//if (_local->GetBaseFSM()->IsState(EGsStateBase::AutoMove) == true)
		//{
		//	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

		//	if (FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
		//	{
		//		// 실제 이동하려던 정보 Clear
		//		FGsMovementStateAuto& moveStateAuto = movement->GetStateAutoMove();
		//		moveStateAuto.ClearData();
		//	}
		//}
		

	}
}

// 자동 이동 컨텐츠 타입 변경됨
// https://jira.com2us.com/jira/browse/C2URWQ-3193
void FGsInteractionHandlerLocalPlayer::OnAutoMoveContentsTypeChanged(const IGsMessageParam* In_param)
{
	const FGsAutoMoveContentsChangedParam* param = In_param->Cast<const FGsAutoMoveContentsChangedParam>();

	// npclist에서 다른걸로 바뀌었으면
	// npc list info hide 처리하기
	if (param->_oldContentsType == EGsMovementAutoContentsType::NpcListMove &&
		param->_newContentsType != EGsMovementAutoContentsType::NpcListMove)
	{
		// npc list 정보 hide(hud quest 거리쪽)
		HideNpcListMoveInfo();

		// npc slot 갱신()
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_NPCFUNCTION);
	}
}

// 로컬 스폰 완료 시점
void FGsInteractionHandlerLocalPlayer::OnLocalSpawnMeComplete(const IGsMessageParam*)
{
	// npc 슬롯으로 이동중 로비로 다녀오면
	// npc 리스트 이동 정보 남아있는 현상 수정
	// https://jira.com2us.com/jira/browse/C2URWQ-3193
	HideNpcListMoveInfo();
}