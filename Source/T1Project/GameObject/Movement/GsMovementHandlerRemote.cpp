// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMovementHandlerRemote.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "UnrealMathUtility.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsQueryFloorHeightComponent.h"
#include "ActorComponentEx//GsMovementDebugShapeComponent.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Util/GsClassUtil.h"
#include "UTIL/GsDir.h"
#include "DrawDebugHelpers.h"
#include "Data/GsDataContainManager.h"
#include "GsMovementHandlerBase.h"

//------------------------------------------------------------------------------------------------
// FGsMovementHandlerBase
//------------------------------------------------------------------------------------------------
void FGsMovementHandlerRemote::Initialize(UGsGameObjectBase* inOwner)
{
	check(inOwner);
	FGsMovementHandlerBase::Initialize(inOwner);

	_fsm = inOwner->CastGameObject<UGsGameObjectCreature>()->GetBaseFSM();
	check(_fsm);

	_ownerObjectType = inOwner->GetObjectType();

	_runner = inOwner->CastGameObject<UGsGameObjectCreature>();
	check(_runner);

	float allowDistance = GData()->GetGlobalData()->_allowDistance;
	_allowDistanceSquared2D = allowDistance * allowDistance;

}

void FGsMovementHandlerRemote::InitializeActor(class AActor* Owner)
{
	Super::InitializeActor(Owner);

	//디버그용
	AGsCharacterBase* character = GetCharacter();
	if (nullptr != character)
	{
		_debugShape = Cast<UGsMovementDebugShapeComponent>(character->GetComponentByClass(UGsMovementDebugShapeComponent::StaticClass()));
		_remoteMove.SetDebugShapeComponent(_debugShape);
	}
	
	
}

void FGsMovementHandlerRemote::Update(float inDelta)
{
	// 속도계산
	FGsMovementHandlerBase::Update(inDelta);	

	if (_runner)
	{		

		if (nullptr != _debugShape)
		{
			_debugShape->SetDebugString(FString::Printf(TEXT("_eState : %d,  _netMotionType : %d"), _eState, _motionType));
		}

		// 회전은 정지상태에서도 반영될수 있다.
		FVector finalDirection = FVector::ZeroVector;
		_runner->StopMovingAttack();
		if (_eState == EMoveState::SKILL)
		{
			if (_lerpRotInfo._isTurning)
			{
				ClearLerpRotation();
			}

			if (false == HasReachedInternal(_remoteMove.GetDestination(), 0.0f, _runner->GetRunnerLocation(), 0.0f, _movetoGoalRadius))
			{
				// 방향설정
				finalDirection = _remoteMove.GetNetDirection();

				// 이동 설정
				// 스킬에서는 Velocity를 현재 위치에 맞춰 따로 설정해준다.(거리비례해서 속도를 올림)
				FVector dir = (_remoteMove.GetDestination() - _runner->GetRunnerLocation());
				float weight = 1.f;
				if (dir.SizeSquared() > _skillMoveBustRangeSquared)
				{
					weight = 2.f;
				}
				
				// 실제 이동중인 경우에만 반영
				if (UCharacterMovementComponent* moveComponent = GetMovementComponent())
				{
					if (false == moveComponent->Velocity.IsZero())
					{
						_runner->RequestMovingAttack();
					}
				}
				
				_runner->RequestDirectMove((dir * weight) / inDelta);
			}
			else
			{
				//이동 중지
				_runner->StopActive();				

				//마지막에 전송받은 방향을 바라보게 변경한다
				finalDirection = _remoteMove.GetNetDirection();

				//디버깅
				if (nullptr != _debugShape)
				{
					_debugShape->SetNetDirection(_remoteMove.GetNetDirection());
				}
				//정말로 도착한 상태임
				ChangeMoveStateSendFsmMessage(EMoveState::STOP);
			}
		}
		else if (_eState != EMoveState::STOP)
		{
			FVector velocity = _remoteMove.GetVelocity();
			FVector finalvelocity;
			// 여기서 액터의 포지션을 갱신한다.
			if (false == HasReached(_remoteMove.GetDestination(), _runner->GetRunnerLocation(), velocity, finalvelocity, inDelta, _remoteMove.GetDirection(), finalDirection))
			{
				// 이동 설정
				_runner->RequestDirectMove(finalvelocity);

				if (_motionType == MotionType::STOP)
				{
					// DrawDebugSphere(_character->GetWorld(), _remoteMove.GetDestination(), 10.f, 20, FColor::Red, false, 3.f);
					// Stop 패킷이 왔으면 방향은 바꾸지 않고 서버 방향 그대로 진행
					finalDirection = _remoteMove.GetNetDirection();
				}

				// Fsm 변경
				ChangeMoveStateSendFsmMessage(_motionType == MotionType::WALK ? EMoveState::WALK : EMoveState::RUN);

				//디버깅
				if (nullptr != _debugShape)
				{
					_debugShape->SetNetDirection(_remoteMove.GetNetDirection());
				}
			}
			else
			{
				//이동 중지
				_runner->StopActive();

				finalDirection = _remoteMove.GetNetDirection();
				// 현재 서버와의 회전동기화 쪽은 확인이 더 필요하므로 최종 위치는 클라이언트 회전 위치로 맞춘다.
				//finalDirection = _owner->GetRotation().Vector();

				//정말로 도착한 상태임
				if (GetMotionType() != MotionType::BLOCK)
				{
					ChangeMoveStateSendFsmMessage(EMoveState::STOP);
				}
				else
				{
					ChangeMoveStateSendFsmMessage(EMoveState::RUN);
				}

				//디버깅
				if (nullptr != _debugShape)
				{
					_debugShape->SetEpicVelocity(FVector::ZeroVector);
				}
			}
		}

		if (_lerpRotInfo._isTurning > 0)
		{
			UpdateBlendAngle(inDelta);
			UpdateLerpRotation(inDelta, finalDirection);
		}


		if (false == finalDirection.IsZero())
		{
			_runner->SetRunnerRotation(finalDirection.Rotation());
		}
	}
}

void FGsMovementHandlerRemote::ChangeMoveStateSendFsmMessage(EMoveState inEnumState)
{
	if (_eState == inEnumState)
	{
		return;
	}
	_eState = inEnumState;

	switch (_eState)
	{
	case EMoveState::RUN:
		_fsm->ProcessEvent(EGsStateBase::Run);
		break;
	case EMoveState::WALK:
		_fsm->ProcessEvent(EGsStateBase::ForwardWalk);
		break;
	case EMoveState::STOP:
		_fsm->ProcessEvent(EGsStateBase::Idle);
		break;
	}
}

void FGsMovementHandlerRemote::SetSpeedMovementComponent(StatValue inMoveSpeed)
{
	// Dash 스피드는 따로 설정할 필요가 없다 (Stat의 Speed값으로 반영되어야 한다.)
	float speed = (_motionType == MotionType::WALK) ? _moveWalkSpeed : inMoveSpeed;
	_remoteMove.SetMoveSpeed(speed);
	// Velocity 설정
	_remoteMove.SetVelocity(_remoteMove.GetDirection() * speed);
	
	FGsMovementHandlerBase::SetSpeedMovementComponent(speed);
}

//void FGsMovementHandlerRemote::Reset(uint16 inDir, const FVector& inPos)
//{
//	FGsMovementHandlerBase::Reset(inDir, inPos);
//
//	_eState = EMoveState::STOP;
//	_motionType = MotionType::STOP;
//	ClearLerpRotation();
//}

void FGsMovementHandlerRemote::ReceveNetMotionStopReason(MotionStopReason inReason)
{
	_eState = EMoveState::STOP;
	_motionType = MotionType::STOP;

	ClearLerpRotation();
		
	FGsMovementHandlerBase::ReceveNetMotionStopReason(inReason);
}

// [U1] | ejrrb10 | 자동이동 후 워프 시, 움직이면서 워프 모션이 나오는데, 자동이동의 경우 타겟팅을 바꾸려고 해도 무시되는 현상이 있다.
// 정확한 원인 파악이 이루어지기 전까지는, 아래 함수를 추가하여 강제로 RemotePlayer 의 움직임을 막는다
void FGsMovementHandlerRemote::StopRunnerMovement()
{
	//이동 중지
	_runner->StopActive();

	////마지막에 전송받은 방향을 바라보게 변경한다
	//finalDirection = _remoteMove.GetNetDirection();

	//디버깅
	if (nullptr != _debugShape)
	{
		_debugShape->SetNetDirection(_remoteMove.GetNetDirection());
	}
	//정말로 도착한 상태임
	ChangeMoveStateSendFsmMessage(EMoveState::STOP);
}

//------------------------------------------------------------------------------------------------
// FGsMovementNetReceiver
//------------------------------------------------------------------------------------------------
float LastDist = FLT_MAX;
void FGsMovementHandlerRemote::SetNetMotionType(MotionType inMotionType)
{
	// 모션타입에 맞도록 재갱신
	_eState = EMoveState::REFRESH;

	LastDist = FLT_MAX;

	if ((int)inMotionType == (int)MotionType::STOP)
	{
		//bak1210 : 서버에서 이동좌표를 줄때 Stop 상태로 브로드 케스트를 해준다.
		// 그럴때에도 좌표는 보정해야하니까 일딴 이동으로 상태변경해서 틀어진 좌표를 맞춘다.
		//ChangeMoveStateSendFsmMessage(EMoveState::WALK);
		
	}
	else if(inMotionType == MotionType::NORMAL || 
		inMotionType == MotionType::VEHICLE)
	{
		ChangeMoveStateSendFsmMessage(EMoveState::RUN);
		
	}
	else if (inMotionType == MotionType::WALK)
	{
		ChangeMoveStateSendFsmMessage(EMoveState::WALK);
	}

	FGsMovementNetReceiver::SetNetMotionType(inMotionType);
}

void FGsMovementHandlerRemote::SetNetDirection(const FVector& inDireciton)
{
	_remoteMove.SetNetDirection(inDireciton);

}
void FGsMovementHandlerRemote::SetNetDestination(const FVector& inDestination)
{
	_remoteMove.SetNetDestination(inDestination);

	if (nullptr != _debugShape)
	{
		_debugShape->SetDestination(inDestination);
	}
}

void FGsMovementHandlerRemote::SetNetLocation(const FVector& inNetLocation)
{	
	_remoteMove.SetNetLocation(inNetLocation);
	
	FVector actorLocation = inNetLocation;
	
	actorLocation = _runner->GetRunnerFeetLocation();
		//거리차가 심해지면 새로운 위치로 포지션해준다.
	float Dist = FVector::DistSquared2D(actorLocation, inNetLocation);

	if (Dist > _allowDistanceSquared2D)
	{
		FHitResult hitResult;
		FVector teleportLocation;
		AGsCharacterBase* character = GetCharacter();
		if (character && _queryFloorHeightComponent.IsValid() && _queryFloorHeightComponent->TryGetTeleportLocation(teleportLocation, hitResult, character, inNetLocation))
		{
			_runner->SetRunnerLocation(teleportLocation);
			character->TeleportTo(teleportLocation, _remoteMove.GetNetDirection().Rotation());
		}

		actorLocation = _runner->GetRunnerLocation();
	}
	
	_remoteMove.SetLocation(actorLocation);

	if (nullptr != _debugShape)
	{
		_debugShape->SetLocation(inNetLocation);
	}
	
}

const FVector FGsMovementHandlerRemote::GetNetLocation() const
{
	return _remoteMove.GetNetDirection();
}

void FGsMovementHandlerRemote::SetNetStop()
{
	ReceveNetMotionStopReason(MotionStopReason::ABNORMALITY);
}

const MotionType FGsMovementHandlerRemote::GetMotionType() const
{
	return _motionType;
}

void FGsMovementHandlerRemote::SetSyncLocation(const FVector& inNetDirection, const FVector& inNetLocation)
{
	_eState = EMoveState::SKILL;
	_motionType = MotionType::STOP;

	SetNetDirection(inNetDirection);
	FVector actorLocation = _runner->GetRunnerFeetLocation();

	_remoteMove.SetLocation(actorLocation);
	_remoteMove.SetNetDestination(inNetLocation);

	// _lerpRotInfo._isTurning = false;
	// 블렌드 스페이스 보간용 앵글값 초기 설정
	// UGsAnimInstanceState* anim = _character->GetAnim();
	// anim->StopBlendSpaceAngle();
}


void FGsMovementHandlerRemote::SetLocation(const FVector& inNetDirection, const FVector& inNetLocation)
{
	_eState = EMoveState::RUN;
	_motionType = MotionType::NORMAL;

	SetNetDirection(inNetDirection);

	_remoteMove.SetNetDestination(inNetLocation);

	// _lerpRotInfo._isTurning = false;
	// 블렌드 스페이스 보간용 앵글값 초기 설정
	// UGsAnimInstanceState* anim = _character->GetAnim();
	// anim->StopBlendSpaceAngle();
}

bool FGsMovementHandlerRemote::HasReached(const FVector& inGoalLocation, const FVector& inLocation, const FVector& inVelocity,
	OUT FVector& outVelocity, float fDelta, const FVector& inDirection, OUT FVector& outDirection) const
{
	// 현재 틱에서 움직여야 하는 좌표보다 멀다
	float sqRemainDist = FVector::DistSquared2D(inGoalLocation, inLocation);
	StatValue Velocity = _remoteMove.GetMoveSpeed();
	float fTickMove = Velocity * fDelta;

	/*if (fRemainDist < LastDist)
	{
		LastDist = fRemainDist;
	}
	else if (fRemainDist > LastDist)
	{
		GSLOG(Error, TEXT("에러상태"));
	}*/
	
	FVector vDir = (inGoalLocation - inLocation).GetSafeNormal2D();
	outDirection = inDirection;
	outVelocity = FVector::ZeroVector;

	// bak1210
	// 1M 이내로 접어들면 거리기반 속도를 세팅한다.
	if (sqRemainDist > 10000.0f)
	{
		outVelocity = inVelocity;
		return false;
	}
	else
	{		
		if (false == HasReachedInternal(inGoalLocation, 0.0f, inLocation, 0.0f, _movetoGoalRadius) )
		{
			outVelocity = (vDir * FMath::Sqrt(sqRemainDist)) / fDelta;
			return false;
		}
	}
	return true;
}


bool FGsMovementHandlerRemote::HasReachedInternal(const FVector& inGoalLocation, float inGoalRadius,
	const FVector& inLocation, float inRadiusThreshold, float inAgentRadiusMultiplier) const
{
	// get cylinder of moving agent
	float AgentRadius = 0.0f;
	float AgentHalfHeight = 0.0f;
	_runner->GetSimpleCollisionCylinder(AgentRadius, AgentHalfHeight);

	// check if they overlap (with added AcceptanceRadius)
	const float sqDist2D = FVector::DistSquared2D(inGoalLocation, inLocation);
	const float UseRadius = inRadiusThreshold + inGoalRadius + (AgentRadius * inAgentRadiusMultiplier);
	if (sqDist2D > FMath::Square(UseRadius))
	{
		return false;
	}
	return true;
}

void FGsMovementHandlerRemote::StartRotation(PD::SC::PKT_SC_START_ROTATION_READ* inPkt)
{
	_lerpRotInfo._currDir = FGsDir::angleToDir(inPkt->CurDir());
	_lerpRotInfo._destDir = FGsDir::angleToDir(inPkt->DestDir());
	_lerpRotInfo._rotateSpeed = FMath::RadiansToDegrees(FGsDir::angleToRad(inPkt->RotateSpeed()));

	_lerpRotInfo._accumAngle = 0.f;

	FRotator currRotator = _runner->GetRunnerRotation();
	FRotator destRotator = _lerpRotInfo._destDir.Rotation();
	FRotator interpolationRot = currRotator - destRotator;
	interpolationRot.Normalize();
	// 실제 회전해야하는 각도 설정
	_lerpRotInfo._marginAngle = FMath::Abs(interpolationRot.Yaw);

	// 좌,우 각도 보정값 설정
	FVector cross = FVector::CrossProduct(_runner->GetRunnerForwardVector(), _lerpRotInfo._destDir);
	int correction = (cross.Z < 0.f) ? -1 : 1;

	// 최초 처리에 해당될 경우에만 현재 서버에서준 회전 위치로 설정
	if (_lerpRotInfo._isTurning == 0)
	{
		_runner->SetRunnerRotation(_lerpRotInfo._currDir.Rotation());

		/*GSLOG(Error, TEXT("[StartRotation] StartRotation 현재 : %f(%d) 목적 : %f(%d) 회전양 : %f, 방향 : %d, 현재 블렌드값 : %f, 블랜딩 타입 : %d"),
			currRotator.Yaw, inPkt->CurDir(),
			destRotator.Yaw, inPkt->DestDir(),
			_lerpRotInfo._marginAngle,
			correction,
			_lerpRotInfo._blendAngle, _lerpRotInfo._isTurning);*/
	}
	else
	{
		// 변위량이 적은 경우
		if (_lerpRotInfo._marginAngle < _lerpRotInfo._rotateSpeed * 0.2f)
		{
			_lerpRotInfo._blendAngleSpeed = 90.0f/*anim->BlendSpaceAngleMinSpeed*/;
		}
		else
		{
			// 현재 블렌딩 각도와 반대 방향 방향전환이면
			/*int blendAngle = _lerpRotInfo._blendAngle;
			if (0 != blendAngle && (correction ^ blendAngle) > 0)
			{
				_lerpRotInfo._blendAngleSpeed = anim->BlendSpaceAngleSpeed * 2.f;
			}*/
		}

		/*GSLOG(Error, TEXT("[StartRotation] StartRotation 현재 : %f(%d) 목적 : %f(%d) 회전양 : %f, 방향 : %d, 현재 블렌드값 : %f, 블랜딩 타입 : %d"),
			currRotator.Yaw, inPkt->CurDir(),
			destRotator.Yaw, inPkt->DestDir(),
			_lerpRotInfo._marginAngle,
			correction,
			_lerpRotInfo._blendAngle, _lerpRotInfo._isTurning);*/
	}
	_lerpRotInfo._isTurning = 1;
	_lerpRotInfo._rotateCorrection = correction;

	AGsCharacterBase* character = GetCharacter();

	if (nullptr == character)
	{
		return;
	}

	if (UGsAnimInstanceState* anim = character->GetAnim())
	{
		_lerpRotInfo._blendAngle = anim->GetCurrentBlendSpaceAngle();
		_lerpRotInfo._blendAngleSpeed = anim->BlendSpaceAngleSpeed;		
	}
}

void FGsMovementHandlerRemote::ResetNetLocation()
{
	SetNetDestination(_runner->GetRunnerLocation());
	SetNetLocation(_runner->GetRunnerLocation());
}

void FGsMovementHandlerRemote::UpdateBlendAngle(float inDelta)
{
	AGsCharacterBase* character = GetCharacter();
	if (nullptr == character)
		return;

	if (UGsAnimInstanceState* anim = character->GetAnim())
	{
		float updateAngle = 0.f;
		int correction = _lerpRotInfo._rotateCorrection;
		if (_lerpRotInfo._isTurning == 1)
		{
			// 블렌드 스페이스는 현재 설정된값에서 가감 처리로 진행
			updateAngle = _lerpRotInfo._blendAngleSpeed * inDelta * correction;
			_lerpRotInfo._blendAngle += updateAngle;
			anim->SetBlendSpaceAngle(_lerpRotInfo._blendAngle);

			//GSLOG(Error, TEXT("[StartRotation] UpdateBlendAngle (1) 현재 BlendAngle: %f, 보정 BlendAngle: %f"), anim->GetCurrentBlendSpaceAngle(), updateAngle);
		}
		// 회수 블렌딩 처리(클라이언트 영역)
		else if (_lerpRotInfo._isTurning == 2)
		{
			correction = _lerpRotInfo._blendAngle / FMath::Abs(_lerpRotInfo._blendAngle);
			updateAngle = anim->BlendSpaceAngleReturnSpeed * inDelta * correction;
			_lerpRotInfo._blendAngle -= updateAngle;
			// 초기값 까지 도달했으면 블렌딩 처리 종료
			if (((int)_lerpRotInfo._blendAngle ^ correction) < 0)
			{
				ClearLerpRotation();
				//GSLOG(Error, TEXT("[StartRotation] ----- 블렌딩 처리 완료 ----------------"));
			}
			else
			{
				anim->SetBlendSpaceAngle(_lerpRotInfo._blendAngle);
				//GSLOG(Error, TEXT("[StartRotation] UpdateBlendAngle (2) 현재 BlendAngle: %f, 보정 BlendAngle: %f"), anim->GetCurrentBlendSpaceAngle(), updateAngle);
			}
		}
	}
}

void FGsMovementHandlerRemote::UpdateLerpRotation(float inDelta, FVector& OutDir)
{
	if (_lerpRotInfo._isTurning == 1)
	{
		// Delta시간만큼 회전해야하는 각도
		float deltaTimeAngle = _lerpRotInfo._rotateSpeed * inDelta;
		FRotator currRotator = _runner->GetRunnerRotation();
		FRotator destRotator = _lerpRotInfo._destDir.Rotation();
		FRotator interpolationRot = currRotator - destRotator;
		interpolationRot.Normalize();

		// 실제 보간해야되는 각도보다 많다면 최종 각도 설정
		float absMarginAngle = FMath::Abs(interpolationRot.Yaw);
		// 실제 회전 종료
		if (absMarginAngle < deltaTimeAngle)
		{
			deltaTimeAngle = absMarginAngle;
			OutDir = destRotator.Vector();

			// 회수 블렌딩 처리
			_lerpRotInfo._isTurning = 2;
			//GSLOG(Error, TEXT("[StartRotation] ----- 캐릭터 회전 완료 ----------------"));
		}

		// 실제 모델 회전 반영
		currRotator.Add(0.f, deltaTimeAngle * _lerpRotInfo._rotateCorrection, 0.f);
		OutDir = currRotator.Vector();
		_lerpRotInfo._accumAngle += deltaTimeAngle;

		//int angle = FGsDir::dirToAngle(OutDir);
		//GSLOG(Error, TEXT("[StartRotation] UpdateLerpRotation  현재 캐릭터 Angle: %f(%d)  보정 Angle: %f"), currRotator.Yaw, angle, deltaTimeAngle * _lerpRotInfo._rotateCorrection);
	}
}

void FGsMovementHandlerRemote::ClearLerpRotation()
{
	_lerpRotInfo._isTurning = 0;
	_lerpRotInfo._accumAngle = 0.f;
	_lerpRotInfo._destDir = FVector::ZeroVector;
	_lerpRotInfo._marginAngle = 0.f;
	_lerpRotInfo._blendAngle = 0.f;

	AGsCharacterBase* character = GetCharacter();
	if (nullptr == character)
	{
		return;
	}

	if (UGsAnimInstanceState* anim = character->GetAnim())
	{		
		anim->StopBlendSpaceAngle();
	}
}


