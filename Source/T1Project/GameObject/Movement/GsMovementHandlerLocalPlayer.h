// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsMovementHandlerBase.h"
#include "LocalPlayer/GsMovementStateAuto.h"
#include "LocalPlayer/GsMovementStateInput.h"
#include "LocalPlayer/GsMovementStateStop.h"
#include "LocalPlayer/GsMovementNetSender.h"
#include "GameObject/Movement/GsMovementState.h"
#include "GameObject/Movement/Remote/GsMovementStateExtendRemote.h"
#include "Message/GsMessageGameObject.h"

#include "GsMovementHandlerRemote.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "T1Project.h"
#include "Classes/SecureVar/GsSecureVar.h"


/**
 * 2021/10/21 PKT
 * 기존 LocaPlayer Movement 구동 방식은 서버로 부터 받은 정보로 움직이는 것이 아닌 클라이언트 자체 적으로 동기화를 맞춤.
 * 오프라인 플레이 관전자 모드는 서버로 부터 LocalPlayer의 위치 정보 등을 받아 움직여야 하므로 기존 Remote Object를 활용하도록 했음.
 */

class AGsPlayerController;
class UGsGameObjectLocalPlayer;
class UGsLocalCharacterMovementComponent;
struct IGsMessageParam;

/**
 * 내 Player 이동 처리 클래스
 */

class FGsMovementStateExtendRemote;
class FGsMovementHandlerRemote;

class T1PROJECT_API FGsMovementStateSpectatorLocalPlayer : public FGsMovementStateExtendRemote
{
public:
	FGsMovementStateSpectatorLocalPlayer(EGsMovementState inEnumType) :
		FGsMovementStateExtendRemote(inEnumType) {}
	virtual ~FGsMovementStateSpectatorLocalPlayer() = default;

	virtual bool CanTransition(EGsMovementState inType) override { return true; } //local에서 사용하는 movement 상태는 return true가 되어야 함
};

//using FGsMovementStateSpectatorLocalPlayer = FGsMovementStateExtendRemote;
using FGsMovementHandlerSpectatorLocalPlayer = FGsMovementHandlerRemote;

class T1PROJECT_API FGsMovementHandlerLocalPlayer : public FGsMovementHandlerBase
{
	using Super = FGsMovementHandlerBase;

	//서버에서 좌표이동 오류시 보내온 좌표와 클라이언트에서 보유한 좌표차 (허용범위)
	const float						_allowDistanceSquared2D = 10000.0f;	// 1M;

	bool							_moveAble		= true;				// 이동가능한 상태인지확인(나만)
protected:
	UGsGameObjectLocalPlayer* _local = nullptr;
	bool							_moveLock		= false;

	FGsMovementNetSender			_netSender;	

private:
	FGsMovementStateAuto			_autoMethod;
	FGsMovementStateInput			_inputMethod;

	FGsMovementStateSpectatorLocalPlayer	_spectatorMethod;
	FGsMovementHandlerSpectatorLocalPlayer* _movementHandlerspectator = nullptr;

private:
	static FGsMovementStateStop		_stopMethod;

	//TestCode
	class UGsMovementDebugShapeComponent* _test		= nullptr;
	// 타겟 클리어 이벤트 델리게이트
	MsgGameObjHandle _delegateTargetClear;

	FGsSecureInt _moveMoveSpeedSecure;

	int _checkMoveIdleCount = 0;

	MsgGameObjHandleArray _gameObjectHandleArray;
	/*bool _isSpectatorRootMotion{ false };*/
public:
	FGsMovementHandlerLocalPlayer()
		: _spectatorMethod(EGsMovementState::SPECTATOR)
		, _moveMoveSpeedSecure(GS_MOVEMENT_HANDLER_DEFAULT_MOVE_SPEED)
	{/*None*/ }

	virtual ~FGsMovementHandlerLocalPlayer() = default;

public:
	virtual void	Initialize(UGsGameObjectBase* Owner) override;
	virtual void	InitializeActor(class AActor* Owner) override;
	virtual void	Finalize() override;
	virtual void	Update(float Delta) override;

public:
	virtual void InitSpeed(int32 inMoveSpeed, int32 inWalkSpeed) override;

public:
	//bak1210 : 필요시 상황에 맞게 복구한다...
	void			SetJoystickAxisRotate(const FVector& Dir);

public:
	virtual void UpdateNetSender(float In_delta);
	virtual void Stop(bool inSendFsmMessage = true) override;

public:
	bool		IsBlockedMovement();

private:
	void			InternalStop();

public:
	// 주의 !!. 단순 Access를 처리하는 함수가 아님(상태치환을 일으킴)
	// Geter는 별도로 존재합니다.
	FGsMovementStateAuto& ChangeAutoMoveState()
	{
		if (GetMoveAble())
		{
			ChangeState(_autoMethod);
		}
		return _autoMethod;
	}

public:
	// 주의 !!. 단순 Access를 처리하는 함수가 아님(상태치환을 일으킴)
	// Geter는 별도로 존재합니다.
	FGsMovementStateInput* ChangeInputMoveState(const FVector2D& inAxisValue)
	{
		_inputMethod.SetDirecitonFromMoveScale(inAxisValue);
		// 이동불가상황에선 상태변경하지 않음
		if (GetMoveAble())
		{
			if (ChangeState(_inputMethod))
			{
				return &_inputMethod;
			}
			else if (nullptr != _active)
			{
				if (_active->GetType() == EGsMovementState::INPUT)
				{
					return  &_inputMethod;
				}
			}
		}
		return nullptr;
	}
public:

	// 타겟 클리어 이벤트
	void OnLocalTargetClear(const IGsMessageParam*);

public:
	// 2021/10/22 PKT - 오프라인 관중 모드로 진입
	FGsMovementStateSpectatorLocalPlayer& ChangeSpectatorState();

protected:
	virtual bool ChangeState(FGsMovementState& inState) override
	{
		if (Super::ChangeState(inState))
		{
			_netSender.ChangeMoveState(_active);
			return true;
		}
		return false;
	}

public:
	virtual void SetNetMoveFailed(const FVector& inDireciton, const FVector& inLocation);
	virtual void PositionValidateByServer(const uint64& inSeq, bool inResponse, const FVector& inDireciton, const FVector& inLocation);
	virtual void SetMoveState(bool inMove) override;
	virtual void SetSpeedMovementComponent(StatValue inSpeed) override;	
	virtual void SpectatorEnd();	
	int GetMoveSpeed() const final;

	bool CheckMoveIdle();

public:
	void SetMoveable(bool inMoveable);

	void SetMovementAutoContentsType(EGsMovementAutoContentsType inType) 
	{
		_autoMethod.SetMovementAutoContentsType(inType);
	}
	// 이전 이동 타입 복구
	// https://jira.com2us.com/jira/browse/C2URWQ-6334
	void RestoreBackupMovementAutoContentsType()
	{
		_autoMethod.RestoreBackupMovementAutoContentsType();
	}
	void SetIsDrawWaypointDebugPos(bool isDraw)
	{
		_autoMethod.SetIsDrawWaypointDebugPos(isDraw);
	}
	void SetIsDrawPosSphere(bool isDraw)
	{
		_autoMethod.SetIsDrawPosSphere(isDraw);
	}
	void SetCurrentNavPath(FNavPathSharedPtr inPath)
	{
		_autoMethod.SetCurrentNavPath(inPath);
	}
	void SetIsAllowPartialPath(bool isAllow)
	{
		_autoMethod.SetIsAllowPartialPath(isAllow);
	}
	void SetIsLockFinishCallback(bool isLock)
	{
		_autoMethod.SetIsLockFinishCallback(isLock);
	}

	void AddAutoPathPoint(FVector In_pos)
	{
		_autoMethod.AddPathPoint(In_pos);
	}

public:
	void SetNetSenderInterval(float inValue)
	{
		_netSender.SetInterval(inValue);
	}

public:
	inline AGsPlayerController*			GetPlayerController() const		{ return _autoMethod.GetPlayerController(); }
	UGsMovementDebugShapeComponent*		GetDebugShapeComponent() const	{ return _test; }

	// FGsMovementState.. Geter 들 모음
public:
	FGsMovementStateAuto&				GetStateAutoMove()				{ return _autoMethod; }
	FGsMovementStateInput&				GetStateInputMove()				{ return _inputMethod; }
	bool								GetMoveAble()					{ return _moveAble; }
	virtual const MotionType			GetMotionType() const override;
	bool								IsAutoMove() const;
	bool								IsInputMove() const;

	/************************************************************************/
	/* OfflinePlay monitoring Dev Only                                      */
	/************************************************************************/
	FGsMovementHandlerSpectatorLocalPlayer* GetSpectatorLocalPlayer() const { return _movementHandlerspectator; }

	void SpectatorReceveStartSkill(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_START_SKILL_READ* inPkt);
	void SpectatorReceveFinishSkill(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_SKILL_FINISHED_READ* inPkt);
	void SpectatorReceveMoveToLocation(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_MOVE_TO_LOCATION_READ* inPkt, 
		bool inTouchEffect = true, bool inIgnoreDirection = false);
	void SpectatorSetSpeedMovementComponent(StatValue inMoveSpeed);
	//////////////////////////////////////////////////////////////////////////

	void AddBlockMoveObject(const class UGsGameObjectBase* InBlockObject);
	void RemoveBlockMoveObject(const class UGsGameObjectBase* InBlockObject);

	void Stiffen(const FVector& inLocation);

protected:
	void OnInvalidActor(const IGsMessageParam * InParam);
};
