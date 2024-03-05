#pragma once

#include "Util/GsTimeUtil.h"
#include "Shared/Client/SharedEnums/SharedMoveEnum.h"

class FGsMovementState;
class UGsGameObjectLocalPlayer;
//-------------------------------------------------------------------------------------------------
// 이동패킷을 실제로 전송하는 객체.. 전송타이밍을 제어한다.
// FGsMovementState의 Dirty 상태를 체크해서 전송을 안할수도 있음
//-------------------------------------------------------------------------------------------------
class T1PROJECT_API FGsMovementNetSender
{
public:
	static float		NetSendTimeInterval;
	static float		SendDestinationTime;				// NetSendTimeInterval * 2.0f;
	static float		RecvDestinationTime;				// sendDestinationTime * 0.9f;

private:
	FGsMovementState*	_active = nullptr;
	FGsSimpleTimer		_sendTimer;

	FVector				_lastSendPos = FVector::ZeroVector;
	FVector				_lastSendDest = FVector::ZeroVector;
	MotionType			_lastSendMotionType = MotionType::MAX;
	float				_sendDistanceSquared = 900.f;	// 0.3M

	uint64				_seqNum = 0;
	
	bool				_moveAble = true;

	float				_agentRadius;
	float				_agentHalfHeight;
	//Test
	UGsGameObjectLocalPlayer* _owner;
public:
	void Initialize(UGsGameObjectLocalPlayer* inOwner, float fRadius, float fHalfHeight);
	void Finalize();
	void Update(float inDelta);

	void SetInterval(float inValue);

public:
	void ChangeMoveState(FGsMovementState* inState);

private:
	void SendLocalPlayerMove(bool checkDistance = true);
public:
	void SendLocalPlayerOpposideDir();
private:
	bool IsDirtyMotionType(FGsMovementState* inState) const;
	bool IsVaildDistanceToSend() const;
	
	void Send(MotionType inType, const FVector& inSrc, const FVector& inDest);

public:
	void SetMoveable(bool inMoveable) { _moveAble = inMoveable; }
	void ValidatePosSequence(const uint64& inSeq, bool inResponse);

	//Synchronously check if path between two points exists
	static bool CheckNavMeshRayCast(class AGsCharacterBase* inCharacter, const FVector& inSrc, const FVector& inDest, FVector& outDetect, FVector& outHitNormal);
};