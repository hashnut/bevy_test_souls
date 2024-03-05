#include "GsMovementNetReceiver.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Shared/Client/SharedEnums/SharedMoveEnum.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Movement/LocalPlayer/GsMovementNetSender.h"
#include "UTIL/GsDir.h"

void FGsMovementNetReceiver::ReceveMoveToLocation(struct PD::SC::PKT_SC_CORRECTION_POS_READ* inPkt)
{
	if (nullptr != inPkt)
	{
		SetNetMotionType(MotionType::NORMAL);

		FVector netDirection = FGsDir::angleToDir(inPkt->Dir());
		SetNetDirection(netDirection);
		SetNetDestination(inPkt->ConstRefPos());
		SetNetLocation(inPkt->ConstRefPos());
	}
}

void FGsMovementNetReceiver::ReceveMoveToLocation(EGsGameObjectType inObjectType,
	struct PD::SC::PKT_SC_MOVE_TO_LOCATION_READ* inPkt, bool inIgnoreDirection)
{
	if (nullptr != inPkt)
	{
		MotionType motionType = static_cast<MotionType>(inPkt->MotionType());
		bool firstMove = (GetNetMtionType() == MotionType::STOP) && (motionType != MotionType::STOP);
		SetNetMotionType(motionType);

		FVector netDirection = GetNetLocation();
		if (false == inIgnoreDirection)
		{
			netDirection = FGsDir::angleToDir(inPkt->CurDir());
			SetNetDirection(netDirection);
		}

		uint64 clientCurrentTime = GNet()->GetNetMilliSecond();

		// 레이텐시 계산코드 넣어야한다.
		// 테스트 해본결과 시간이 서버랑 오차가 생긴다.. (내시간보다서버시간이 더큼) : 원래는 내가 더커야함
		//Estimatelaytency(pkt->ClientTick(), clientCurrentTime);

		// 목적지는 PC와 NPC가 다르게 세팅되어야한다.
		// NPC같은경우는 목적지를 정해놓고 이동처리하지만
		// RemotePC 같은경우는 목적지가 이전좌표여야 동일한 위치를 Trail한다.
		if (inObjectType == EGsGameObjectType::NonPlayer
			|| inObjectType == EGsGameObjectType::Sandbag 
			|| motionType == MotionType::NORMAL_OFFLINE_PLAY 
			|| motionType == MotionType::NORMAL_FEAR)
		{
			SetNetDestination(inPkt->ConstRefDestPos());
		}
		else
		{			
			// PC 이동동기화 컨셉
			// 이동시에는 실제 전송된 목적지를 쓰는것이 아닌..보정된 목적지 사용
			// ○ 예상목적지 = 전송된 좌표 + 전송된 방향 * (실제 목적지까지의 거리 90%반영) 
			// 패킷을 받는 쪽은 항상 뒤늦게(레이턴시 : 클라->서버->클라) 방향전환 정보를 받기문에 이전 방향으로 좀더 이동하게된다.
			// 이동시에는 결국 전송자의 목적지까지의 이동거리보다 전송받는사람이 더긴 거리를 이동해야한다.
			// 항상 직교가 아니라 적절한 예는 아니지만 이해를 돕기위한 예시로 직각삼각형의 빗변의 길이를 떠올리면된다.
			// 전송된 방향선상에 기존 목적지보다 (90% : 예측치) 정도 짧은 목적좌표를 설정하면 대략 10프로의 짧은동선 구성이 가능하고(방향전환시)
			// 이를통해 좀더 이동한 거리를 상쇄한다.								
			if (motionType == MotionType::NORMAL || motionType == MotionType::WALK 
				|| motionType == MotionType::VEHICLE)
			{
				FVector considerPos = inPkt->CurPos() + netDirection * (inPkt->Speed() * 
					(firstMove ? FGsMovementNetSender::SendDestinationTime : FGsMovementNetSender::RecvDestinationTime));
				SetNetDestination(considerPos);
			}
			else
			{
				//(PC 기준)
				// ○ 정지시에는 마지막 위치를 목적지로 삼아야 정확한 위치임
				SetNetDestination(inPkt->ConstRefCurPos());
			}
		}

		SetNetLocation(inPkt->ConstRefCurPos());		
	}
}

void FGsMovementNetReceiver::ReceveStartSkill(EGsGameObjectType inObjectType,
	PD::SC::PKT_SC_START_SKILL_READ* inPkt)
{
	if (nullptr != inPkt)
	{
		FVector netDirection = FGsDir::angleToDir(inPkt->CurrDir());
		SetSyncLocation(netDirection, inPkt->ConstRefCurPos());
	}
}

void FGsMovementNetReceiver::ReceveFinishSkill(EGsGameObjectType inObjectType,
	PD::SC::PKT_SC_SKILL_FINISHED_READ* inPkt)
{
	if (nullptr != inPkt)
	{
		FVector netDirection = FGsDir::angleToDir(inPkt->NewDir());
		SetSyncLocation(netDirection, inPkt->ConstRefNewPos());
	}
}

void FGsMovementNetReceiver::ReceveSkillRotation(EGsGameObjectType inObjectType, 
	PD::SC::PKT_SC_SKILL_DAMAGED_READ* inPkt)
{
	if (nullptr != inPkt)
	{
		FVector netDirection = FGsDir::angleToDir(inPkt->CasterDir());
		SetNetDirection(netDirection);
	}
}

void FGsMovementNetReceiver::ReceveCorrection(EGsGameObjectType inObjectType,
	struct PD::SC::PKT_SC_CORRECTION_POS_READ* inPkt)
{
	if (nullptr != inPkt)
	{
		FVector netDirection = FGsDir::angleToDir(inPkt->Dir());
		SetSyncLocation(netDirection, inPkt->ConstRefPos());
	}
}

void FGsMovementNetReceiver::ReceveUserInteractNpcState(struct PD::SC::PKT_SC_USER_NPC_INTERACT_STATE_READ* inPkt)
{
	if (nullptr != inPkt)
	{
		FVector netDirection = FGsDir::angleToDir(inPkt->CurrDir());
		SetSyncLocation(netDirection, inPkt->ConstRefCurrPos());
	}
}

void FGsMovementNetReceiver::ReceveStiffen(struct PD::SC::PKT_SC_MOVE_STIFFEN_READ* inPkt)
{
	if (nullptr != inPkt)
	{
		FVector netDirection = FGsDir::angleToDir(inPkt->CurDir());
		SetLocation(netDirection, inPkt->ConstRefCurPos());
		SetNetStop();
	}
}

void FGsMovementNetReceiver::ReceveStiffen(const FVector& inDireciton, const FVector& inLocation)
{
	SetLocation(inDireciton, inLocation);
	SetNetStop();
}

void FGsMovementNetReceiver::ReceveDeath(EGsGameObjectType inObjectType,
	PD::SC::PKT_SC_CREATURE_DEATH_READ* inPkt)
{
	if (nullptr != inPkt)
	{
		FVector netDirection = FGsDir::angleToDir(inPkt->Dir());
		SetSyncLocation(netDirection, inPkt->ConstRefPos());
	}
}

void FGsMovementNetReceiver::ReceveStartRootmotion(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_CREATURE_SKILL_ROOT_MOTION_START_READ* inPkt)
{
	if (nullptr != inPkt)
	{
		// FVector netDirection = FGsDir::angleToDir(inPkt->DestDir());
		// SetSyncLocation(netDirection, inPkt->ConstRefCurPos());
	}
}

void FGsMovementNetReceiver::ReceveFinishRootmotion(EGsGameObjectType inObjectType, struct PD::SC::PKT_SC_CREATURE_SKILL_ROOT_MOTION_FINISH_READ* inPkt)
{
	if (nullptr != inPkt)
	{
		// FVector netDirection = FGsDir::angleToDir(inPkt->CurDir());
		// SetSyncLocation(netDirection, inPkt->ConstRefCurPos());
	}
}

void FGsMovementNetReceiver::SetNetMotionType(MotionType inMotionType)
{
	_motionType = inMotionType;
}

void FGsMovementNetReceiver::Estimatelaytency(uint64 inPacketTime, uint64 inCurrentTime)
{
	// 마지막 레이턴시기준으로 현재 레이턴시 상황에 따라 다른 보정치를 적용
	uint64 lat = inCurrentTime - inPacketTime;
	if (lat < 0)
	{
		lat = 0;
	}

	double flat = lat * GNet()->GetNetTickMultiply();
	if (flat >= _latency)
	{
		_latency = (_latency + flat) * 0.5;
	}
	else
	{
		_latency = (_latency * 7 + flat) * 0.125;
	}
}
