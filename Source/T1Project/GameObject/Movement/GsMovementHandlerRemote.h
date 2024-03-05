// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsMovementHandlerBase.h"
#include "Remote/GsMovementStateExtendRemote.h"
#include "Remote/GsMovementNetReceiver.h"
#include "GameObject/Define/GsGameObjectStruct.h"

class UGsMovementDebugShapeComponent;
class FGsGameObjectStateManager;
class UCharacterMovementComponent;



/**
 * RemotePlayer 이동 관련 처리 클래스
   타유저와 몬스터 이동을 같이 처리한다.
 */
class T1PROJECT_API FGsMovementHandlerRemote: 
	public FGsMovementHandlerBase,
	public FGsMovementNetReceiver
{
	using Super = FGsMovementHandlerBase;
	//현재 이동상태를 나타내준다.
	UENUM()
	enum class EMoveState : uint8
	{
		REFRESH = 0,
		SKILL,
		STOP,
		WALK,
		RUN,
	};	

	// 타유저가 보내온 좌표와 클라이언트에서 보유한 좌표차 (허용범위)
	float							_allowDistanceSquared2D = 250000.0f;	// 5M;
	// 타유저 도착 반경 범위
	const float						_movetoGoalRadius = 0.35f;
	// 200ms의 레이턴시 4.5m 기준
	const float						_skillMoveBustRangeSquared = 90.f * 90.f;

	EMoveState						_eState			= EMoveState::STOP;
	FVector							_adjustHeight	= FVector::ZeroVector;

	EGsGameObjectType				_ownerObjectType = EGsGameObjectType::Base;
private:
	FGsMovementStateExtendRemote	_remoteMove;	

	class FGsMovementRunner*		_runner = nullptr;
	bool							_updateVelocityByMoveToLocation = false;

private:
	UGsMovementDebugShapeComponent*	_debugShape = nullptr;

private:
	FGsGameObjectStateManager*		_fsm = nullptr;

public:
	FGsMovementHandlerRemote() : _remoteMove(EGsMovementState::REMOTE) {}
	virtual ~FGsMovementHandlerRemote()	= default;

	//------------------------------------------------------------------------------------------------
	// FGsMovementHandlerBase
	//------------------------------------------------------------------------------------------------
public:
	virtual void Initialize(UGsGameObjectBase* inOwner) override;
	virtual void InitializeActor(class AActor* Owner) override;
	virtual void Update(float inDelta) override;

public:
	virtual void SetSpeedMovementComponent(StatValue inSpeed) override;
	virtual void UpdateVelocityByMovetoLocation() { _updateVelocityByMoveToLocation = true; }
	virtual bool IsUpdateByMovetoLocation() override {	return _updateVelocityByMoveToLocation;	}
//public:
//	virtual void Reset(uint16 inDir, const FVector& inPos) override;
private:
	void		ChangeMoveStateSendFsmMessage(EMoveState inEnumState);	

public:
	virtual void ReceveNetMotionStopReason(MotionStopReason inReason) override;
	void StopRunnerMovement();

	//------------------------------------------------------------------------------------------------
	// FGsMovementNetReceiver
	//------------------------------------------------------------------------------------------------
protected:
	virtual void SetNetMotionType(MotionType inMotionType) override;
	virtual void SetNetDirection(const FVector& inDireciton) override;

	virtual void SetNetDestination(const FVector& inDestination) override;
	virtual void SetNetLocation(const FVector& inNetLocation) override;

	virtual void SetSyncLocation(const FVector& inNetDirection, const FVector& inNetLocation) override;
	virtual void SetLocation(const FVector& inNetDirection, const FVector& inNetLocation) override;

	virtual const FVector GetNetLocation() const override;
	virtual void SetNetStop() override;

public:
	// Lerp 회전 처리 
	struct FLerpRotationInfo
	{
		// FVector _destPos = FVector::ZeroVector;
		FVector _currDir = FVector::ZeroVector;
		FVector _destDir = FVector::ZeroVector;
		float _rotateSpeed = 180.f;					// 초당 회전 속도		
		float _marginAngle = 180.f;					// 최종 회전 각도(Degree)
		float _blendAngleSpeed = 180.f;				// 블렌딩 회전 각도 설정
		float _blendAngle = 0.f;					// 현재 블렌드 스페이스 설정 값
		float _accumAngle = 0.f;					// 보간 누적 각도
		float _rotateCorrection = 1.f;				// 좌/우 판단 보정값		
		// 턴 시작 알림
		// 0 : 턴없음, 1 : 서버 동기화 회전 구간, 블렌딩 시작, 2 : 블렌딩 복원 단계
		int _isTurning = 0;
	};

	FLerpRotationInfo _lerpRotInfo;

	void StartRotation(struct PD::SC::PKT_SC_START_ROTATION_READ* inPkt);

	void ResetNetLocation();

protected:
	// 마지막 네트워크에서 전송된 위치에 도달했는지 체크한다.
	bool HasReached(const FVector& inGoalLocation, const FVector& inLocation, const FVector& inVelocity,
		OUT FVector& outVelocity, float fDelta, const FVector& inDirection, OUT FVector& outDirection) const;

	bool HasReachedInternal(
		const FVector& inGoalLocation, float inGoalRadius, const FVector& inLocation,
		float inRadiusThreshold, float AgentRadiusMultiplier
	) const;

	void UpdateLerpRotation(float inDelta, FVector& OutDir);
	void UpdateBlendAngle(float inDelta);
	void ClearLerpRotation();


public:
	virtual const MotionType GetMotionType() const override;
};


