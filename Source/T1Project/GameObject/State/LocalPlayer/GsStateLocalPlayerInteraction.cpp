#include "GsStateLocalPlayerInteraction.h"
#include "GsStateLocalPlayerHeader.h"

#include "ActorEx/GsCharacterBase.h"
#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Animation/GsAnimInstanceState.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Message/GsMessageContentDungeon.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/MessageParam/GsSanctumMessageParam.h"

#include "Data/GsResourceManager.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Prop/GsSchemaPropData.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "Gameobject/ObjectClass/GsGameObjectPropInteraction.h"
#include "Gameobject/ObjectClass/GsGameObjectSanctumDefault.h"

#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionMoveType.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Interaction/GsReserveInteractionInfo.h"

#include "UI/UILib/Manager/GsUIController.h"
#include "UTIL/GsMove.h"


uint8 FGsStateLocalPlayerInteraction::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Interaction);
}

FName FGsStateLocalPlayerInteraction::GetStateName() const
{
	return TEXT("StateLocalPlayerInteraction");
}

FName FGsStateLocalPlayerInteraction::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerInteraction::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		GS_STATE_GROUP_MOVE:
		GS_STATE_GROUP_ACTION:
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:		
		case EGsStateBase::ReactionEnd:
		case EGsStateBase::CommonActionEnd:
		case EGsStateBase::InteractionEnd:
		case EGsStateBase::ChangeWeapon:
		case EGsStateBase::Warp:
			return true;
		default:
			return false;
		}
	}
	return false;
}

bool FGsStateLocalPlayerInteraction::Ready(UGsGameObjectBase* Owner)
{
	if (Super::Ready(Owner))
	{
		UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
		if (FGsInteractionHandlerLocalPlayer* interactionHandler = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>())
		{
			// 타겟 이동이라면
			if (interactionHandler->GetReserveMoveType() == EGsInteractionMoveType::TargetMove)
			{
				UGsGameObjectBase* interactionTarget = interactionHandler->GetInteractionTarget();
				if (interactionTarget == nullptr)
				{
					FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::Idle);
					return false;
				}
				if (IsInsideInteractionRange(Owner, interactionTarget, interactionHandler->GetReserveInfo()))
				{
					// 타겟 방향으로 회전한다.			
					Owner->LookAtTarget(interactionTarget, true);

					// 다른 이동중 대화 인터랙션 성공 가능 위치에서 인터랙션 시도시
					// 이전 이동 데이터로 이동할수 있어서 클리어 한번 해주기
					//https://jira.com2us.com/jira/browse/C2URWQ-7545
					local->ClearReserveDataByReserveWork(interactionHandler);
				}
				else
				{
					interactionHandler->OnMoveToInteractionGameObject(interactionTarget);
					return false;
				}
			}
			else if (interactionHandler->GetReserveMoveType() == EGsInteractionMoveType::PosMove)
			{
				// 범위 안인가
				if (IsInsideInteractionMovePosRange(Owner,
					interactionHandler->GetReserveMapId(),
					interactionHandler->GetReserveMovePos(),
					interactionHandler->GetReserveAddAcceptanceRadius()))
				{
				}
				else
				{
					interactionHandler->OnMoveToInteractionPos(
						interactionHandler->GetReserveMapId(),
						interactionHandler->GetReserveMovePos(),
						interactionHandler->GetReserveAddAcceptanceRadius());
					return false;
				}

				// target 이동으로 돌리기위해 다시 시도
				interactionHandler->TryInteractionNpcTblId(interactionHandler->GetReserveInfo()._target._tblId, EGsInteractionMoveType::TargetMove);
				return false;
			}
			else
			{
				GSLOG(Warning, TEXT("State failed ReserveInteraction Empty"));

				// 예약 데이터가 제거 되었는데 이동 중이었다면 정지
				FGsMovementHandlerBase* movementHandler = local->GetMovementHandler();
				if (EGsMovementState::STOP != movementHandler->GetState())
				{
					movementHandler->Stop();
				}
				else
				{
					FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::Idle);
				}
				return false;
			}

		}
	}
	return true;
}

void FGsStateLocalPlayerInteraction::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	Enter(Owner);
}

void FGsStateLocalPlayerInteraction::Enter(UGsGameObjectBase* _owner)
{
	Super::Enter(_owner);

	UGsGameObjectLocalPlayer* local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsInteractionHandlerLocalPlayer* interactionHandler = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();

	// 타입별 상황 처리 분리
	const FGsReserveInteractionInfo& info = interactionHandler->GetReserveInfo();

	switch (info._target._gameObjectType)
	{
	case EGsGameObjectType::Camp:
	{
		PlayInteraction_Camp(_owner, info._target._npcData, interactionHandler);
	}
	break;
	case EGsGameObjectType::Sanctum:
	{
		PlayInteraction_Sanctum(_owner, info._target._npcData, interactionHandler);
	}
	break;
	case EGsGameObjectType::NonPlayer:
	{
		PlayInteraction_Npc(_owner, info._target._npcData, interactionHandler);
	}
		break;	
	case EGsGameObjectType::PropInteraction:
	{
		PlayInteraction_Prop(_owner, info._target._propData, interactionHandler);
		interactionHandler->StartTimer(info._target._propData->touchInteractionTime);
	}
		break;
	case EGsGameObjectType::PropInteractInvasion:
	{
		if (auto invasionProp = Cast<UGsGameObjectPropTouchInvasion>(interactionHandler->GetInteractionTarget()))
		{
			FGsGameObjectData* data = invasionProp->GetData();
			FGsPropInteractionMessageParamContents param(data->GetGameId(), data->GetTableId());
			param._targetGameID = data->GetGameId();
			param._objectType = info._target._gameObjectType;
			GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_INVASIONPROP_REGISTER, &param);

			if (invasionProp->GetCurrentTouchAction() == InvasionTouchAction::Invasion)
			{
				PlayInteraction_PropInvasion(_owner, info._target._propData, interactionHandler);
			}

			/* auto action = invasionProp->GetCurrentTouchAction();
			if (action == InvasionTouchAction::Register)
			{
				FGsGameObjectData* data = invasionProp->GetData();
				FGsPropInteractionMessageParamContents param(data->GetGameId(), data->GetTableId());

				param._targetGameID = data->GetGameId();
				param._objectType = info._target._gameObjectType;

				GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_INVASIONPROP_REGISTER, &param);
			}
			else if (action == InvasionTouchAction::EnrollingIn)
			{ 
				//PlayInteraction_Prop(_owner, info._target._propData, interactionHandler);
				//interactionHandler->StartTimer(info._target._propData->touchInteractionTime);
			}
			else if (action == InvasionTouchAction::Invasion)
			{
				PlayInteraction_PropInvasion(_owner, info._target._propData, interactionHandler);
			}*/
		}
	}
		break;
	}	

	interactionHandler->ClearReserveInteraction();
}

void FGsStateLocalPlayerInteraction::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);

	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (FGsInteractionHandlerLocalPlayer* interactionHandler = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>())
	{
		interactionHandler->UpdateTimer(Delta);
	}
}

void FGsStateLocalPlayerInteraction::PlayInteraction_Npc(UGsGameObjectBase* _owner, const FGsSchemaNpcData* _npcData,
	FGsInteractionHandlerLocalPlayer* _handler)
{
	if (_npcData)
	{
		// gameid에서 spawnid로 변환한다
		UGsGameObjectBase* target = _handler->GetInteractionTarget();
		if (target != nullptr)
		{
			int64 gameId = target->GetGameId();
			uint32 spawnId = GSClientSpawn()->GetUniqueID(gameId);

			bool isExistWidgetPreventAutoClose = false;
			UGsUIController* uiController =	GUI()->GetUIController();
			if (uiController != nullptr)
			{
				isExistWidgetPreventAutoClose = 
					uiController->IsExistWidgetPreventAutoClose();
			}

#if INTERACTION_DEBUG
	GSLOG(Log, TEXT("Interact npc - id : %d"), _npcData->id);
#endif

			FGsInteractionMessageParamContents sendParam(
				target,
				_npcData->id, spawnId, gameId,
				isExistWidgetPreventAutoClose);

			// 대화 타입 메세지 전송
			GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_NPC, &sendParam);
		}
	}
	if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
	{
		fsm->ProcessEvent(EGsStateBase::InteractionEnd);
	}
}

void FGsStateLocalPlayerInteraction::PlayInteraction_Prop(UGsGameObjectBase* _owner, const FGsSchemaPropData* _propData,
	FGsInteractionHandlerLocalPlayer* _handler)
{
	if (nullptr == _propData)
	{
		return;
	}

	UGsGameObjectBase* target = _handler->GetInteractionTarget();
	if (nullptr == target)
	{
		return;
	}

	UGsGameObjectPropObject* castProp = Cast<UGsGameObjectPropObject>(target);
	if (nullptr == castProp)
	{
		return;
	}

	//프랍 캔슬 중 인터렉션을 시도하면 취소 시킨다. 
	if (castProp->IsCanceling())
	{
		GSLOG(Warning, TEXT("prop is canceling - id : %lld"), castProp->GetGameId());
		return;		
	}

	CommonActionType commonType = CommonActionType::TOUCHPROP;

	if (const FGsSchemaSkillCommon* commonSkill = GSkill()->GetPlayerCommonActionData(_owner, commonType))
	{
		UGsAnimInstanceState* anim = _owner->GetAnimInstance();
		anim->SetPhysicsAnimation(commonSkill->physicsAnimation);
		// 타임라인 개념이 모호하기 때문에 SkillRunner를 타지 않는다.

		//FGsSkillHandlerBase* skillhandler = _owner->GetSkillHandler();			
		//FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
		//skillRunner->StartRunner(Owner, commonSkill);
		if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(commonSkill->aniPath))
		{
			AGsCharacterBase* caracterActor = _owner->GetCharacter();
			caracterActor->PlayAnimMontage(res, 1.f, TEXT("Start"));
		}
	}

	FGsGameObjectData* data = target->GetData();
	FGsPropInteractionMessageParamContents param(data->GetGameId(), data->GetTableId());
	GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_START, &param);
}

void FGsStateLocalPlayerInteraction::PlayInteraction_Camp(class UGsGameObjectBase* _owner, const struct FGsSchemaNpcData* _npcData, class FGsInteractionHandlerLocalPlayer* _handler)
{
	if (_npcData)
	{
		// gameid에서 spawnid로 변환한다
		UGsGameObjectBase* target = _handler->GetInteractionTarget();
		if (target != nullptr)
		{
			int64 gameId = target->GetGameId();
			uint32 spawnId = GSClientSpawn()->GetUniqueID(gameId);

			bool isExistWidgetPreventAutoClose = false;
			UGsUIController* uiController = GUI()->GetUIController();
			if (uiController != nullptr)
			{
				isExistWidgetPreventAutoClose =
					uiController->IsExistWidgetPreventAutoClose();
			}

			// 인터렉션 anim play
			if (const FGsSchemaSkillCommon* commonSkill = GSkill()->GetPlayerCommonActionData(_owner, CommonActionType::INTERACTION_CAMP))
			{
				UGsAnimInstanceState* anim = _owner->GetAnimInstance();
				anim->SetPhysicsAnimation(commonSkill->physicsAnimation);
				// 타임라인 개념이 모호하기 때문에 SkillRunner를 타지 않는다.

				if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(commonSkill->aniPath))
				{
					AGsCharacterBase* caracterActor = _owner->GetCharacter();
					caracterActor->PlayAnimMontage(res, 1.f, TEXT("Start"));

					FGsGameObjectData* data = target->GetData();
					FGsPropInteractionMessageParamContents param(data->GetGameId(), data->GetTableId());
					// Camp 인터랙션 시작 메세지
					GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_START, &param);

					// 망치
					if (UGsGameObjectLocalPlayer* localPlayer = _owner->CastGameObject<UGsGameObjectLocalPlayer>())
					{
						AGsCharacterLocalPlayer* localActor = localPlayer->GetLocalCharacter();
						if (nullptr != localActor)
						{
							localActor->SetToolWeaponStaticMeshVisible(true);
						}
					}
				}
			}

			FGsInteractionMessageParamContents sendParam(
				target,
				_npcData->id, spawnId, gameId,
				isExistWidgetPreventAutoClose);

			GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_NPC, &sendParam);
		}
		else
		{
			if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
			{
				fsm->ProcessEvent(EGsStateBase::InteractionEnd);
			}
		}
	}
	else
	{
		if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
		{
			fsm->ProcessEvent(EGsStateBase::InteractionEnd);
		}
	}
}

void FGsStateLocalPlayerInteraction::PlayInteraction_Sanctum(UGsGameObjectBase* _owner, const  FGsSchemaNpcData* _npcData, FGsInteractionHandlerLocalPlayer* _handler)
{
	if (nullptr == _npcData)
	{
		if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
		{
			fsm->ProcessEvent(EGsStateBase::InteractionEnd);
		}

		return;
	}
	
	UGsGameObjectBase* target = _handler->GetInteractionTarget();
	if (nullptr == target)
	{
		if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
		{
			fsm->ProcessEvent(EGsStateBase::InteractionEnd);
		}

		return;
	}

	// 건설 시작해야 인터렉션 되기때문에 다른 경우엔 바로 종료시킴
	if (NpcFunctionType::SANCTUM_NEXUS != _npcData->npcFunctionType)
	{
		if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
		{
			fsm->ProcessEvent(EGsStateBase::InteractionEnd);
		}

		if (NpcFunctionType::SANCTUM == _npcData->npcFunctionType)
		{
			if (UGsGameObjectSanctumDefault* sanctum = target->CastGameObject<UGsGameObjectSanctumDefault>())
			{
				if (sanctum->GetIsActiveInteraction())
				{
					FGsSanctumMsgParamSanctumId param(sanctum->GetSanctumId(), sanctum->GetGameId());
					GMessage()->GetSanctum().SendMessage(MessageContentSanctum::TRY_START_CONSTRUCT, &param);
				}
			}
		}	

		return;
	}

	int64 gameId = target->GetGameId();
	uint32 spawnId = GSClientSpawn()->GetUniqueID(gameId);

	bool isExistWidgetPreventAutoClose = false;
	UGsUIController* uiController = GUI()->GetUIController();
	if (uiController != nullptr)
	{
		isExistWidgetPreventAutoClose =
			uiController->IsExistWidgetPreventAutoClose();
	}

	// 인터렉션 anim play
	if (const FGsSchemaSkillCommon* commonSkill = GSkill()->GetPlayerCommonActionData(_owner, CommonActionType::INTERACTION_CAMP))
	{
		UGsAnimInstanceState* anim = _owner->GetAnimInstance();
		anim->SetPhysicsAnimation(commonSkill->physicsAnimation);
		// 타임라인 개념이 모호하기 때문에 SkillRunner를 타지 않는다.

		if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(commonSkill->aniPath))
		{
			AGsCharacterBase* caracterActor = _owner->GetCharacter();
			caracterActor->PlayAnimMontage(res, 1.f, TEXT("Start"));

			FGsGameObjectData* data = target->GetData();
			FGsPropInteractionMessageParamContents param(data->GetGameId(), data->GetTableId());
			// Camp 인터랙션 시작 메세지
			GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_START, &param);

			// 망치
			if (UGsGameObjectLocalPlayer* localPlayer = _owner->CastGameObject<UGsGameObjectLocalPlayer>())
			{
				AGsCharacterLocalPlayer* localActor = localPlayer->GetLocalCharacter();
				if (nullptr != localActor)
				{
					localActor->SetToolWeaponStaticMeshVisible(true);
				}
			}
		}
	}

	FGsInteractionMessageParamContents sendParam(
		target,
		_npcData->id, spawnId, gameId,
		isExistWidgetPreventAutoClose);

	GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_NPC, &sendParam);
}

void FGsStateLocalPlayerInteraction::PlayInteraction_PropInvasion(UGsGameObjectBase* _owner, const  FGsSchemaPropData* _propData, FGsInteractionHandlerLocalPlayer* _handler)
{
	if (nullptr == _propData)
	{
		return;
	}

	CommonActionType commonType = CommonActionType::TOUCHPROP;
	if (const FGsSchemaSkillCommon* commonSkill = GSkill()->GetPlayerCommonActionData(_owner, CommonActionType::TOUCHPROP))
	{
		UGsAnimInstanceState* anim = _owner->GetAnimInstance();
		anim->SetPhysicsAnimation(commonSkill->physicsAnimation);

		if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(commonSkill->aniPath))
		{
			AGsCharacterBase* caracterActor = _owner->GetCharacter();
			caracterActor->PlayAnimMontage(res, 1.f, TEXT("Start"));
		}
	}

	if (UGsGameObjectBase* target = _handler->GetInteractionTarget())
	{
		FGsGameObjectData* data = target->GetData();
		FGsPropInteractionMessageParamContents param(data->GetGameId(), data->GetTableId());
		GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_INVASIONPROP_START, &param);
	}
}

void FGsStateLocalPlayerInteraction::Exit(UGsGameObjectBase* Owner)
{
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsInteractionHandlerLocalPlayer* interactionHandler = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
	const FGsInteractionTimer& timer = interactionHandler->GetTimerInfo();
	
	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		// 인터랙션은 Loop 타입이므로 종료 처리
		if (false == fsm->CurrentState()->IsSameState(EGsStateBase::InteractionEnd))
		{
			UGsAnimInstanceState* anim = Owner->GetAnimInstance();
			anim->StopMontagePlay();
		}
	}

	// 망치
	AGsCharacterLocalPlayer* localActor = local->GetLocalCharacter();
	if (nullptr != localActor)
	{
		localActor->SetToolWeaponStaticMeshVisible(false);
	}

	// 완료 상황 체크로 켄슬 패킷 전송
	SendCancel(Owner);

	Super::Exit(Owner);
}

void FGsStateLocalPlayerInteraction::SendCancel(UGsGameObjectBase* _owner) const
{
	UGsGameObjectLocalPlayer* local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsInteractionHandlerLocalPlayer* interactionHandler = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
	const FGsInteractionTimer& timer = interactionHandler->GetTimerInfo();
	UGsGameObjectBase* target = interactionHandler->GetInteractionTarget();
	if (nullptr == target)
	{
		return;
	}

	UGsGameObjectPropObject* prop = Cast<UGsGameObjectPropObject>(target);
	if (nullptr == prop)
	{
		return;
	}

	if (false == timer.IsValid())
	{
		return;
	}

	//캔슬 중 캔슬 패킷 보내지 말것
	if (prop->IsCanceling())
	{
		return;
	}

	// 켄슬 패킷 요청 보내기
	FGsNetSendServiceWorld::SendInteractPropCancel(prop);
}

bool FGsStateLocalPlayerInteraction::IsInsideInteractionRange(UGsGameObjectBase* In_owner, UGsGameObjectBase* In_target,
	const FGsReserveInteractionInfo& ReserveData)
{
	if (In_owner != nullptr && In_target != nullptr)
	{
		// 전투와 달리 서버는 인터랙션에서 점대점으로 계산하므로 충돌체 계산 필요 없음
		float distanceSq = FVector::DistSquared2D(In_target->GetLocation(), In_owner->GetLocation());

		float interactionRange = 0.f;
		if (ReserveData._target._gameObjectType == EGsGameObjectType::NonPlayer || 
			ReserveData._target._gameObjectType == EGsGameObjectType::Camp ||
			ReserveData._target._gameObjectType == EGsGameObjectType::Sanctum)
		{
			interactionRange = FGsInteractionHandlerLocalPlayer::GetInteractionRange(ReserveData._target._npcData, In_target, true);
		}
		else if (In_target->IsObjectType(EGsGameObjectType::PropInteraction)) // ReserveData._target._gameObjectType == 
		{
			interactionRange = FGsInteractionHandlerLocalPlayer::GetInteractionRange(ReserveData._target._propData, true);
		}

		if (distanceSq <= interactionRange * interactionRange)
		{
			return true;
		}
	}

	return false;
}

bool FGsStateLocalPlayerInteraction::IsInsideInteractionRangeByTableId(UGsGameObjectBase* In_owner, int In_targetNpcTblId)
{
	if (In_owner == nullptr)
	{
		return false;
	}

	UGsGameObjectBase* nonPlayer = GSGameObject()->FindObjectNearestNonPlayerByNPCId(
		In_owner->GetActor(), In_targetNpcTblId);

	if (nonPlayer == nullptr)
	{
		return false;
	}

	FGsReserveInteractionInfo reserveInfo;

	if (nonPlayer->IsObjectType(EGsGameObjectType::NonPlayer) || 
		nonPlayer->IsObjectType(EGsGameObjectType::Camp) ||
		nonPlayer->IsObjectType(EGsGameObjectType::Sanctum))
	{
		UGsGameObjectNonPlayer* interactionTarget = static_cast<UGsGameObjectNonPlayer*>(nonPlayer);
		if (interactionTarget == nullptr)
		{
			return false;
		}

		const FGsSchemaNpcData* npcTbl = interactionTarget->GetNpcData();

		if (npcTbl == nullptr)
		{
			return false;
		}
		reserveInfo._target._npcData = npcTbl;
	}
	else if (nonPlayer->IsObjectType(EGsGameObjectType::PropInteraction))
	{
		UGsGameObjectPropInteraction* interactionTarget = static_cast<UGsGameObjectPropInteraction*>(nonPlayer);
		if (interactionTarget == nullptr)
		{
			return false;
		}

		const FGsSchemaPropData* propTbl = interactionTarget->GetPropTableData();

		if (propTbl == nullptr)
		{
			return false;
		}

		reserveInfo._target._propData = propTbl;
	}
	reserveInfo._isMoveComplete = false;
	reserveInfo._moveType = EGsInteractionMoveType::TargetMove;
	reserveInfo._addAcceptanceRadius = 0.0f;
	reserveInfo._mapId = 0;
	reserveInfo._target._tblId = 0;
	reserveInfo._target._gameObjectType = nonPlayer->GetObjectType();
	reserveInfo._pos = FVector::ZeroVector;

	return IsInsideInteractionRange(In_owner, nonPlayer, reserveInfo);
}

// 인터랙션 위치 이동 범위 안인가
bool FGsStateLocalPlayerInteraction::IsInsideInteractionMovePosRange(UGsGameObjectBase* In_owner,
	int In_mapId,
	FVector In_targetPos,
	float In_addAcceptanceRadius)
{
	// 현재위치 맵id랑 목표랑 다르면
	int currMapId = GLevel()->GetCurrentLevelId();
	if (In_mapId != currMapId)
	{
		// 범위 밖
		return false;
	}

	if (In_owner != nullptr)
	{
		float distanceSq = FVector::DistSquared2D(In_targetPos, In_owner->GetLocation());
		float interactionRange = FGsMove::GetMovePosRange(true) + In_addAcceptanceRadius;

		if (distanceSq <= interactionRange * interactionRange)
		{
			return true;
		}
	}

	return false;
}