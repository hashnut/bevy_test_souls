#pragma once

#include "GameObject/GsGameObjectHandler.h"
#include "GsMovementState.h"

class AGsCharacterBase;
class UGsGameObjectBase;
class UCharacterMovementComponent;
class UGsQueryFloorHeightComponent;

#define GS_MOVEMENT_HANDLER_DEFAULT_MOVE_SPEED 400

/**
 *  이동을 담당하는 기반 클래스 
 */
class T1PROJECT_API FGsMovementHandlerBase : public IGsGameObjectHandler
{
protected:
	bool				_dirty = true;

	UGsGameObjectBase*	_owner	= nullptr;
	FGsMovementState*	_active = nullptr;
protected:
	TWeakObjectPtr<UGsQueryFloorHeightComponent>	_queryFloorHeightComponent = nullptr;

protected:
	bool				_isMoveState = false;	
	MotionStopReason	_motionStopReason = MotionStopReason::NORMAL;
	int					_moveWalkSpeed = 200;
	int					_moveVehicleSpeed = 1500;

private:
	int					_moveMoveSpeed = GS_MOVEMENT_HANDLER_DEFAULT_MOVE_SPEED;

public:
	FGsMovementHandlerBase() : IGsGameObjectHandler(GameObjectHandlerType::MOVEMENT) {};
	virtual ~FGsMovementHandlerBase()	= default;

public:
	virtual void Initialize(UGsGameObjectBase* Owner) override;
	virtual void InitializeActor(class AActor* Owner) override;
	virtual void Update(float inDelta) override;
	virtual void Finalize() override;

	UCharacterMovementComponent* GetMovementComponent() const;
	AGsCharacterBase* GetCharacter() const;

public:
	virtual void InitSpeed(int32 inMoveSpeed, int32 inWalkSpeed);

public:
	virtual void Stop(bool inSendFsmMessage = true);

public:
	/*virtual void Reset(uint16 inDir, const FVector& inPos);*/

public:
	// 즉시 위치 갱신
	virtual void DirectMoveUpdate(const FVector& inPos, const FVector& inDir);
	virtual void DirectMoveUpdate(const FVector& inPos, const FRotator& inRot);

protected:
	virtual bool ChangeState(FGsMovementState& inState);	

public:	
	virtual void ReceveNetMotionStopReason(MotionStopReason inReason);

public:
	// 더티 구조에서만 업데이트 처리함
	void SetDirty(bool inDirty) { _dirty = inDirty; }

public:
	//이동중인가?
	bool IsMoveState() const		{ return _isMoveState; }	

	virtual void SetMoveState(bool IsMove)	{ _isMoveState = IsMove; }
	virtual void SetSpeedMovementComponent(StatValue inSpeed);	
	virtual bool IsUpdateByMovetoLocation() { return false; }

public:
	virtual int GetMoveSpeed() const;
	virtual const MotionType GetMotionType() const;

public:
	// 현재 상태 얻기
	EGsMovementState GetState() const;
};

// bak1210 
// 내케릭터는 Update를 State(Move)에서 처리하는데, Handler 인터페이스에서 처리되는것(중복) 제외하기 위한 용처로 활용
struct FGsMovementDirtyMark
{
	FGsMovementHandlerBase* _target;
public:
	FGsMovementDirtyMark(FGsMovementHandlerBase* inTarget) : _target(inTarget)
	{
		_target->SetDirty(true);
	}
	~FGsMovementDirtyMark()
	{
		_target->SetDirty(false);
	}
};