#pragma once

#include "Net/GSNet.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "GameObject/Define/GsGameObjectDefine.h"

//-------------------------------------------------------------------------------------------------
// 이동 패킷을 받아서 GsMovementHandlerRemote 에게 클라이언트 정보로 변환된 정보를 제공한다.
// 현재까지는 정보 Translate(서버->클라) 
// Epic 알고리즘
// 기본컨셉)
// 1. 서버와 클라이언트의 시간을 동기화
// 2. 네트워크 전송시 Remote의 시간을 같이 전송하여 받는사람입장에서 얼마만큼 latency가 발생했는지 판별
// 3. latancy는 속도로 변환되어 거리가 멀어지면 속도를 올리는 방식 기본은 이전 (latency + 현재 latency) * 0.5f
//-------------------------------------------------------------------------------------------------

class UGsGameObjectBase;

class T1PROJECT_API FGsMovementNetReceiver
{
protected:
	MotionType	_motionType = MotionType::STOP;

private:
	float		_latency	= 0.1f;

public: //이동
	void ReceveMoveToLocation(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_MOVE_TO_LOCATION_READ* inPkt,
		bool inIgnoreDirection = false);

	void ReceveMoveToLocation(struct PD::SC::PKT_SC_CORRECTION_POS_READ* inPkt);

public: //스킬
	void ReceveStartSkill(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_START_SKILL_READ* inPkt);
	void ReceveFinishSkill(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_SKILL_FINISHED_READ* inPkt);
	void ReceveSkillRotation(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_SKILL_DAMAGED_READ* inPkt);

	// 스킬이외 행동
	void ReceveCorrection(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_CORRECTION_POS_READ* inPkt);

	// 루트 모션
	void ReceveStartRootmotion(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_CREATURE_SKILL_ROOT_MOTION_START_READ* inPkt);
	void ReceveFinishRootmotion(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_CREATURE_SKILL_ROOT_MOTION_FINISH_READ* inPkt);

	//인터렉션 응답 패킷
	void ReceveUserInteractNpcState(struct PD::SC::PKT_SC_USER_NPC_INTERACT_STATE_READ* inPkt);

	// 경직
	void ReceveStiffen(struct PD::SC::PKT_SC_MOVE_STIFFEN_READ* inPkt);
	void ReceveStiffen(const FVector& inDireciton, const FVector& inLocation);	

public: //상태변경
	void ReceveDeath(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_CREATURE_DEATH_READ* inPkt);

protected:
	virtual void SetNetMotionType(MotionType inMotionType);
	virtual void SetNetDirection(const FVector& inDireciton) = 0;
	virtual void SetNetDestination(const FVector& inDestination) = 0;

	virtual void SetNetLocation(const FVector& inNetLocation) = 0;

	// 스킬, 죽음등 서버좌표와 동기화 맞추는 용처임
	virtual void SetSyncLocation(const FVector& inNetDirection, const FVector& inNetLocation) = 0;
	virtual void SetLocation(const FVector& inNetDirection, const FVector& inNetLocation) = 0;
	virtual const FVector GetNetLocation() const = 0;
	virtual void SetNetStop() = 0;

private:
	void Estimatelaytency(uint64 inPacketTime, uint64 inCurrentTime);

public:
	float GetEstimateLatency() const { return _latency; }
	MotionType GetNetMtionType() const { return _motionType; }
};
