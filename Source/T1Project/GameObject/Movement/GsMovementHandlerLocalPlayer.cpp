// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMovementHandlerLocalPlayer.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "ControllerEx/GsPlayerController.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorComponentEx/GsMovementDebugShapeComponent.h"
#include "ActorComponentEx/GsQueryFloorHeightComponent.h"
#include "ActorComponentEx/GsLocalCharacterMovementComponent.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Stat/GsStatBase.h"
#include "GameObject/Stat/GsStatHelper.h"
//#include "GameObject/Movement/Remote/GsMovementStateExtendRemote.h"
#include "GameObject/Movement/GsMovementHandlerRemote.h"

#include "Util/GsClassUtil.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"

//TestCode
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsCameraModeManager.h"


#include "Message/MessageParam/GsMessageParam.h"

#include "T1Project/T1Project.h"
#include "Input/GsInputEventMsgBase.h"
#include "Engine/Classes/Engine/Engine.h"
#include "UTIL/GsDir.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "../UTIL/GsLevelUtil.h"
#include "Engine/EngineTypes.h"
#include "Classes/SecureVar/GsSecureVar.h"
#include "../AI/GsAIAvoidCollisionHandler.h"

FGsMovementStateStop FGsMovementHandlerLocalPlayer::_stopMethod;

void FGsMovementHandlerLocalPlayer::Initialize(UGsGameObjectBase* owner)
{
	Super::Initialize(owner);

	_local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
	_inputMethod.Initialize(_local);	
	_autoMethod.Initialize(_local);	

	_inputMethod.SetHandleOpposideDirection([this]()->void{
		_netSender.SendLocalPlayerOpposideDir();
	});

	// Stop 코드 처리
	_autoMethod.GetStopFuncRouter().AddRaw(this, &FGsMovementHandlerLocalPlayer::InternalStop);
	_inputMethod.GetStopFuncRouter().AddRaw(this, &FGsMovementHandlerLocalPlayer::InternalStop);
	_spectatorMethod.GetStopFuncRouter().AddRaw(this, &FGsMovementHandlerLocalPlayer::InternalStop);
	
	// 런상태 전환 비율 설정
	FGsMovementStateInput::CacheChangeScaleRateRunWalk = GData()->GetGlobalData()->LocalPlayerRunStateChangeRate;

	const struct FGsSchemaCreatureCommonInfo* commonInfo = _local->GetCommonInfoTable();

	//네트워크 패킷 전송객체 초기화
	_netSender.Initialize(_local, commonInfo->cylinderRadius, commonInfo->cylinderHeight);
	
	_delegateTargetClear =
		GMessage()->GetGameObject().AddRaw(
			MessageGameObject::LOCAL_TARGET_CLEAR, this, &FGsMovementHandlerLocalPlayer::OnLocalTargetClear);

	_movementHandlerspectator = new FGsMovementHandlerSpectatorLocalPlayer;
	_movementHandlerspectator->Initialize(owner);

	_checkMoveIdleCount = 0;

	MGameObject& gameObject = GMessage()->GetGameObject();
	_gameObjectHandleArray.Add(gameObject.AddRaw(MessageGameObject::COMMON_DESPAWN, this, &FGsMovementHandlerLocalPlayer::OnInvalidActor));
	_gameObjectHandleArray.Add(gameObject.AddRaw(MessageGameObject::COMMON_DIE, this, &FGsMovementHandlerLocalPlayer::OnInvalidActor));
}

void FGsMovementHandlerLocalPlayer::Finalize()
{
	MGameObject& msgGameObject = GMessage()->GetGameObject();
	for (MsgGameObjHandle& msgGameObjectHandle : _gameObjectHandleArray)
	{
		msgGameObject.Remove(msgGameObjectHandle);
	}
	_gameObjectHandleArray.Empty();

	_netSender.Finalize();
	Super::Finalize();

	GMessage()->GetGameObject().Remove(_delegateTargetClear);

	_autoMethod.Finalize();

	SAFE_DELETE(_movementHandlerspectator);
}

void FGsMovementHandlerLocalPlayer::InitializeActor(class AActor* Owner)
{
	Super::InitializeActor(Owner);

	_local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
	AGsCharacterLocalPlayer* actor = _local->GetLocalCharacter();
	check(actor);
		
	_inputMethod.SetCharacterLocal(actor);
	_stopMethod.SetCharacterLocal(actor);
	_autoMethod.SetCharacterLocal(actor);
	_autoMethod.SetPlayerController(actor->GetPlayerController());

	//Test
	if (auto component = Cast<UGsMovementDebugShapeComponent>(actor->GetComponentByClass(UGsMovementDebugShapeComponent::StaticClass())))
	{
		_test = component;
	}

	if (_movementHandlerspectator)
	{
		_movementHandlerspectator->InitializeActor(Owner);
	}
}

void FGsMovementHandlerLocalPlayer::Update(float Delta)
{
	// 경직
	if (_dirty == false)
	{
		return;
	}

	if (EGsMovementState::SPECTATOR == GetState())
	{
		return;
	}

	Super::Update(Delta);

	UpdateNetSender(Delta);
}
void FGsMovementHandlerLocalPlayer::UpdateNetSender(float In_delta)
{
	//최종적인계산을 네트워크로 전송
	_netSender.Update(In_delta);
}

void FGsMovementHandlerLocalPlayer::InitSpeed(int32 inMoveSpeed, int32 inWalkSpeed)
{
	FGsMovementHandlerBase::InitSpeed(inMoveSpeed, inWalkSpeed);

	if (_movementHandlerspectator)
	{
		_movementHandlerspectator->InitSpeed(inMoveSpeed, inWalkSpeed);
	}
}

void FGsMovementHandlerLocalPlayer::Stop(bool inSendFsmMessage)
{
	// 현재 상태가 AutoMove 상태가 아니지만, 내부 데이터는 설정되어있는 경우가 있음.
	_autoMethod.ClearData();

	if (_local && _local->GetAIAvoidCollision())
	{
		_local->GetAIAvoidCollision()->EndDetector();
	}
	
	Super::Stop(inSendFsmMessage);
}

void FGsMovementHandlerLocalPlayer::InternalStop()
{
	ChangeState(_stopMethod);
}

bool FGsMovementHandlerLocalPlayer::IsBlockedMovement()
{
	if (UCharacterMovementComponent* moveComponent = GetMovementComponent())
	{
		UGsLocalCharacterMovementComponent* localMove = Cast<UGsLocalCharacterMovementComponent>(moveComponent);
		if (localMove)
		{
			return localMove->IsHitBlocked();
		}
	}
	return false;
}

void FGsMovementHandlerLocalPlayer::Stiffen(const FVector& inLocation)
{
	if (UCharacterMovementComponent* moveComponent =  GetMovementComponent())
	{
		moveComponent->StopActiveMovement();
	}

	if (nullptr == GetCharacter())
	{
		_owner->SetActorLocation(inLocation);
	}
}

void FGsMovementHandlerLocalPlayer::PositionValidateByServer(const uint64& inSeq, bool inResponse, const FVector& inDireciton, const FVector& inLocation)
{
	_netSender.ValidatePosSequence(inSeq, inResponse);

	// 캐릭터 생성후
	// 1. 무비
	// 2. 시퀀스
	// 3. 루나와 대화연출 시작후
	// 회전과 위치 세팅이 내려와서 대화 세팅이 깨져서
	// 서버에서 수정해주신다고 했는데(@서버팀장님)
	// 클라에서도 일단 막음
	if (nullptr == GSCameraMode())
	{
		return;
	}

	if (GSCameraMode()->GetIsCameraDialogPlay() == true)
	{
		return;
	}

	AGsCharacterLocalPlayer* actor = _local->GetLocalCharacter();
	if (nullptr == actor)
	{
		return;
	}

	FNavLocation DestNavLocation;
	if (Cast<UGsCharacterMovementComponent>(actor->GetMovementComponent())->FindNavFloor(inLocation, DestNavLocation))
	{
		/**
		 * https://jira.com2us.com/jira/browse/C2URWQ-3658
		 * [퀘스트-시네마틱 플로우] 아이리스 마을 리그릿과 대화 시네마틱 연결 문제
		 */
		FVector calcSpawnPos = DestNavLocation.Location;
		FHitResult hitResult;
		if (UGsLevelUtil::TrySweepToLand(hitResult, actor->GetWorld(), DestNavLocation.Location, actor->GetCapsuleComponent()->GetScaledCapsuleRadius(), actor->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()))
		{
			calcSpawnPos = hitResult.ImpactPoint + FVector(0, 0, actor->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		}
		
		/**
		 * https://jira.com2us.com/jira/browse/C2URWQ-3929
		 * 2023/4/21 PKT - 시네마틱 진행 이전 내 좌표와 서버가 갖고 있는 내 최종 위치가 정확하게 맞지 않을 수 있다. 그 때문에 무조건 Set Postion 하면 화면이 튀는 현상이 있을 수 있음.
		 * 일정 거리 내에 있으면 동기화를 무시
		 */

		FVector2D oriPosXY = FVector2D(actor->GetActorLocation().X, actor->GetActorLocation().Y);
		FVector2D nextPosXY = FVector2D(calcSpawnPos.X, calcSpawnPos.Y);

		const float IGNORE_DIST = 50.f;
		if (IGNORE_DIST > FVector2D::Distance(oriPosXY, nextPosXY))
		{
			return;
		}

		actor->SetActorLocation(calcSpawnPos);
	}
	else
	{
		//거리에 따라서 어떤식으로 보정할지는 테스트 해보면서 처리
		if (false == actor->TeleportTo(inLocation, inDireciton.Rotation()))
		{
			// 해당 위치로의 이동이 실패할 경우
			GSLOG(Error, TEXT("Object Teleport failed! Pos : %f, %f, %f"), inLocation.X, inLocation.Y, inLocation.Z);
		}
	}	
}

void FGsMovementHandlerLocalPlayer::SetNetMoveFailed(const FVector& inDireciton, const FVector& inLocation)
{
	if (AGsCharacterLocalPlayer* actor = _local->GetLocalCharacter())
	{
		if (FVector::DistSquared2D(actor->GetActorLocation(), inLocation) > _allowDistanceSquared2D)
		{
			FNavLocation DestNavLocation;
			if (Cast<UGsCharacterMovementComponent>(actor->GetMovementComponent())->FindNavFloor(inLocation, DestNavLocation))
			{
				DestNavLocation.Location.Z += actor->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				actor->TeleportTo(DestNavLocation.Location, inDireciton.Rotation());
			}
			else
			{
				//거리에 따라서 어떤식으로 보정할지는 테스트 해보면서 처리
				if (false == actor->TeleportTo(inLocation, inDireciton.Rotation()))
				{
					// 해당 위치로의 이동이 실패할 경우
					GSLOG(Error, TEXT("Object Teleport failed! Pos : %f, %f, %f"), inLocation.X, inLocation.Y, inLocation.Z);
				}
			}
		}
	}
}

void FGsMovementHandlerLocalPlayer::SetMoveState(bool inMove)
{
	Super::SetMoveState(inMove);
	_inputMethod.SetDirty(inMove);
	_spectatorMethod.SetDirty(inMove);
}

void FGsMovementHandlerLocalPlayer::SetSpeedMovementComponent(StatValue inSpeed)
{
	Super::SetSpeedMovementComponent(inSpeed);

	_moveMoveSpeedSecure.Set(inSpeed);
	
	// Motion 타입 판단등을 위해 스피드값 갱신이 필요
	_autoMethod.SetMoveSpeed(inSpeed);
	_inputMethod.SetMoveSpeed(inSpeed);
	_stopMethod.SetMoveSpeed(inSpeed);
	_spectatorMethod.SetMoveSpeed(inSpeed);
}

void FGsMovementHandlerLocalPlayer::SpectatorEnd()
{
	Stop();
	InternalStop();
	// 위 함수에서 상태 변경을 이미 하고있다. (Stop)
	// ChangeState(_inputMethod);
}

bool FGsMovementHandlerLocalPlayer::CheckMoveIdle()
{
	// 진짜 이동중인가를 확인하여 Count 일정 대기수준을 넘겼음을 체크
	if (UCharacterMovementComponent* moveComponent = GetMovementComponent())
	{
		if (UGsCharacterMovementComponent* castMoveComponent = Cast<UGsCharacterMovementComponent>(moveComponent))
		{
			if (castMoveComponent->IsMoved())
			{
				_checkMoveIdleCount = 0;
			}
			else
			{
				_checkMoveIdleCount++;
			}

			if (_checkMoveIdleCount > GData()->GetGlobalData()->_autoMoveIdleCheckCount)
			{
				_checkMoveIdleCount = 0;
				return true;
			}
		}
	}
	return false;
}

int FGsMovementHandlerLocalPlayer::GetMoveSpeed() const
{
	/*if (UCharacterMovementComponent* moveComponent =  GetMovementComponent())
	{
		return moveComponent->MaxWalkSpeed;
	}*/
	int MoveSpeed;
	_moveMoveSpeedSecure.Get(MoveSpeed);
	return MoveSpeed;
}

void FGsMovementHandlerLocalPlayer::SetMoveable(bool inMoveable)
{
	_moveAble = inMoveable; 
	_netSender.SetMoveable(inMoveable);
}

void FGsMovementHandlerLocalPlayer::SetJoystickAxisRotate(const FVector& Dir)
{
	GetStateInputMove().SetActorRotationFromJoyAxis(_local->GetLocalCharacter(), Dir);
}

// 타겟 클리어 이벤트
void FGsMovementHandlerLocalPlayer::OnLocalTargetClear(const IGsMessageParam*)
{
	// 오토 일때
	if (GetState() == EGsMovementState::AUTO)
	{
		const FAIMoveRequest& moveData = GetStateAutoMove().GetDataRequest();
		// 데이터 유효하고 액터 이동이면
		if (moveData.IsValid() && moveData.IsMoveToActorRequest())
		{
			// 스톱
			Stop();
		}
	}
}

const MotionType FGsMovementHandlerLocalPlayer::GetMotionType() const
{
	if (nullptr == _active)
	{
		return MotionType::MAX;
	}
	return _active->GetMotionType();
}

bool FGsMovementHandlerLocalPlayer::IsAutoMove() const
{
	if (_active && _active->GetType() == EGsMovementState::AUTO)
	{
		return true;
	}
	return false;
}
bool FGsMovementHandlerLocalPlayer::IsInputMove() const
{
	if (_active && _active->GetType() == EGsMovementState::INPUT)
	{
		return true;
	}
	return false;
}

FGsMovementStateSpectatorLocalPlayer& FGsMovementHandlerLocalPlayer::ChangeSpectatorState()
{
	// 2021/10/21 PKT - 아무 조건 없이 바로 상태 치환		
	ChangeState(_spectatorMethod);
	return _spectatorMethod;
}

// 2021/10/22 PKT - OfflinePlay monitoring Dev Only
void FGsMovementHandlerLocalPlayer::SpectatorReceveStartSkill(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_START_SKILL_READ* inPkt)
{
	if (nullptr == _movementHandlerspectator)
	{
		return;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SPRECTATOR : Start Skill : [%d]"), inPkt->SkillId()));
	}
	GetSpectatorLocalPlayer();
	_movementHandlerspectator->ReceveStartSkill(inObjectType, inPkt);
}

// 2021/10/22 PKT - OfflinePlay monitoring Dev Only
void FGsMovementHandlerLocalPlayer::SpectatorReceveFinishSkill(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_SKILL_FINISHED_READ* inPkt)
{
	if (nullptr == _movementHandlerspectator)
	{
		return;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SPRECTATOR : Finish Skill : [%d]"), inPkt->SkillId()));
	}
	GetSpectatorLocalPlayer();
	_movementHandlerspectator->ReceveFinishSkill(inObjectType, inPkt);
}

// 2021/10/22 PKT - OfflinePlay monitoring Dev Only
void FGsMovementHandlerLocalPlayer::SpectatorReceveMoveToLocation(EGsGameObjectType inObjectType
	, struct PD::SC::PKT_SC_MOVE_TO_LOCATION_READ* inPkt
	, bool inTouchEffect /*= true*/
	, bool inIgnoreDirection /*= false*/)
{
	if (nullptr == _movementHandlerspectator)
	{
		return;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SPRECTATOR : MoveToLocation : [%s]"), *inPkt->ConstRefDestPos().ToString()));
	}

	if (inTouchEffect)
	{
		const AGsCharacterLocalPlayer* player = Cast<const AGsCharacterLocalPlayer>(_local->GetActor());
		const UPawnMovementComponent* moveComp = (nullptr != player) ? player->GetMovementComponent() : nullptr;
		if (moveComp)
		{

			FVector destPos = inPkt->ConstRefDestPos();
			destPos.Z = moveComp->GetActorLocation().Z + 1000.f;	// 2021/10/22 PKT - 대략 10미터 정도 위로 올림.

			FVector dir = FVector(0.f, 0.f, -1.0f);

			FGsInputEventMsgLocation location(destPos, dir);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::SpectatorTerrainTouchEffect, &location);
		}
	}
	
	GetSpectatorLocalPlayer();
	_movementHandlerspectator->ReceveMoveToLocation(inObjectType, inPkt, inIgnoreDirection);
}

// 2021/10/22 PKT - OfflinePlay monitoring Dev Only
void FGsMovementHandlerLocalPlayer::SpectatorSetSpeedMovementComponent(StatValue inMoveSpeed)
{
	if (nullptr == _movementHandlerspectator)
	{
		return;
	}
	GetSpectatorLocalPlayer();
	_movementHandlerspectator->SetSpeedMovementComponent(inMoveSpeed);
}

void FGsMovementHandlerLocalPlayer::AddBlockMoveObject(const UGsGameObjectBase* InBlockObject)
{
	/**
	 * 현재 컨텐츠에서는 Quest Auto Move 할때만 해당 기능을 활용함.
	 */
	if (IsAutoMove())
	{
		_autoMethod.ObstacleDetectedFront(InBlockObject);
	}
}

void FGsMovementHandlerLocalPlayer::RemoveBlockMoveObject(const UGsGameObjectBase* InBlockObject)
{
	// 2023/10/13 PKT - 기능 없음. 추후 함수 지워도 됨
}

void FGsMovementHandlerLocalPlayer::OnInvalidActor(const IGsMessageParam* InParam)
{
	// 2023/9/5 PKT - 만약 Auto Move가 활성화 중일때 회피/타겟에 대한 Actor가 Despawn / Die 상황일때에 대한 처리
	// 2023/9/5 PKT - https://com2us-raon.sentry.io/issues/4453869456/?alert_rule_id=12817173&alert_timestamp=1693875925073&alert_type=email&environment=Release&project=4504115468566528&referrer=alert_email
	//if (IsAutoMove() && _autoMethod.IsEvasiveManeuver())
	//{
	//	if (nullptr == InParam)
	//	{
	//		return;
	//	}

	//	const FGsGameObjectMessageParam* param = InParam->Cast<const FGsGameObjectMessageParam>();
	//	if (nullptr == param)
	//	{
	//		return;
	//	}

	//	UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(param->_paramOwner);
	//	if (nullptr == creature)
	//	{
	//		return;
	//	}

	//	//_autoMethod.ClearEvasiveManeuverInfo(creature->GetActor());
	//}
}