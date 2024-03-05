#include "GsMovementNetSender.h"
#include "../GsMovementState.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Util/GsDir.h"
#include "Util/GsLevelUtil.h"
#include "NavigationSystem.h"

#include "NavFilters/NavigationQueryFilter.h"
#include "NavigationSystem/Public/NavMesh/RecastNavMesh.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorComponentEx/GsQueryFloorHeightComponent.h"

//Test
#include "ActorComponentEx/GsMovementDebugShapeComponent.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"

#include "T1Project.h"

#if UE_EDITOR
#include "DrawDebugHelpers.h"
#endif

#define LOG_ON 0

//==================================================================================================
// 네트워크 전송 타임
float FGsMovementNetSender::NetSendTimeInterval = 0.2f;
float FGsMovementNetSender::SendDestinationTime = FGsMovementNetSender::NetSendTimeInterval * 2.0f;
float FGsMovementNetSender::RecvDestinationTime = FGsMovementNetSender::SendDestinationTime * 0.9f;
//==================================================================================================

void FGsMovementNetSender::Initialize(UGsGameObjectLocalPlayer* inOwner, float fRadius, float fHalfHeight)
{
	_sendTimer.SetInterval(NetSendTimeInterval);
	_sendTimer.Router.AddLambda([this]()->void
	{
		SendLocalPlayerMove(true);
	});
	_sendTimer.Start();
	//Test;
	_owner = inOwner;

	_agentRadius = fRadius;
	_agentHalfHeight = fHalfHeight;
}

void FGsMovementNetSender::Finalize()
{
}

void FGsMovementNetSender::Update(float inDelta)
{
	if (nullptr == _active)
	{
		return;
	}

	// 변화량이 없으면 갱신하지 않는다.
	if (false == _active->GetDirty())
	{
		return;
	}

	_sendTimer.Update(inDelta);
}

void FGsMovementNetSender::SetInterval(float inValue)
{
	NetSendTimeInterval = inValue;
	SendDestinationTime = NetSendTimeInterval * 2.0f;
	RecvDestinationTime = SendDestinationTime * 0.9f;

	_sendTimer.SetInterval(inValue);
	_sendTimer.Reset();
	_sendTimer.Start();
}

void FGsMovementNetSender::ChangeMoveState(FGsMovementState* inState)
{
	check(inState);

	bool dirtyMotionType = IsDirtyMotionType(inState);	
	_active = inState;

	if (true == dirtyMotionType)
	{
		SendLocalPlayerMove(false);
	}
}

bool FGsMovementNetSender::IsVaildDistanceToSend() const
{
	if (nullptr == _active)
	{
		return true;
	}

	if (_active->GetMotionType() == MotionType::WALK)
	{
		return true;
	}
	
	if ((_lastSendPos - _active->GetLocation()).SizeSquared2D() > _sendDistanceSquared)
	{
		return true;
	}
	return false;
}

bool FGsMovementNetSender::IsDirtyMotionType(FGsMovementState* inState) const
{
	if (nullptr != _active)
	{
		if (_active->GetMotionType() == inState->GetMotionType())
		{
			return false;
		}
		return true;
	}
	return false;
}


// 이함수에서는 _active 정보를 참고해서 전송처리
void FGsMovementNetSender::SendLocalPlayerMove(bool checkDistance)
{
	if (nullptr == _active)
	{
		return;
	}

	if (false == _moveAble)
	{
		_sendTimer.Reset();
		_sendTimer.Start();
		return;
	}

	// 현재 활성화된 모션타입
	MotionType activeMotionType = _active->GetMotionType();
	// 계산용 변수
	FVector SendPos = _active->GetLocation();
	FVector SendDest = _active->GetDestination();	

	// 목적지 까지 이동가능한지 여부를 체크한다.
	FVector outDetectPos;
	FVector outHitNormal;
	bool navModifyDest = false;
#if WITH_EDITOR
	auto movement = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>()->GetDebugShapeComponent();
#endif
	if (false == CheckNavMeshRayCast(_owner->GetCharacter(), SendPos, SendDest, outDetectPos, outHitNormal))
	{
		GSLOG(Log, TEXT("CheckPathTwoPoints Failed"));

		FHitResult hit;
		if (UGsLevelUtil::TrySweepToLand(hit, _owner->UObject::GetWorld(), outDetectPos, _agentRadius, _agentHalfHeight))
		{
			outDetectPos = hit.ImpactPoint + _agentHalfHeight;
		}

#if WITH_EDITOR
		if (nullptr != movement)
		{
			movement->SetEpicLocation(outDetectPos);
		}
#endif
		SendDest = outDetectPos;
		navModifyDest = true;
	}
#if WITH_EDITOR && LOG_ON
	else
	{
		GSLOG(Log, TEXT("\n"));
	}
#endif
	

//#if WITH_EDITOR && LOG_ON
//	GSLOG(Log, TEXT("SendLocalPlayerMove : navModifyDest [%d], _lastSendMotionType [%d], SendMotionType [%d]\n"), navModifyDest,  _lastSendMotionType, activeMotionType);
//#endif

	//최소이동거리를 충족하지 못했다면 현재좌표로
	//bool vaildDistance = checkDistance ? IsVaildDistanceToSend() : true;
	bool bMoveBlock = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>()->IsBlockedMovement();

	if (bMoveBlock && checkDistance)
	{
		if (_lastSendMotionType != MotionType::STOP) // 현재좌표에 세우는 정보를 보내고
		{
			activeMotionType = MotionType::BLOCK;
			SendDest = SendPos;
		}
		// 경직코드때문에 동기화 이슈 생김
		if(activeMotionType != MotionType::STOP && _lastSendMotionType == MotionType::BLOCK)
		{
			_sendTimer.Reset();
			_sendTimer.Start();
			return;
		}
	}
#if WITH_EDITOR
	if (nullptr != movement)
	{
		movement->SetDestination(SendDest);
	}
#endif
	// 다음 전송시간 조절
	_sendTimer.Reset();

#if WITH_EDITOR && LOG_ON
	GSLOG(Log, TEXT("Send : activeMotionType %d"), activeMotionType);
#endif 

	Send(activeMotionType, SendPos, SendDest);

	// 다음 인터벌 체크 시작
	_sendTimer.Start();
}

void FGsMovementNetSender::SendLocalPlayerOpposideDir()
{
	if (nullptr == _active)
	{
		return;
	}

	if (false == _moveAble)
	{
		return;
	}

	// 현재 활성화된 모션타입
	MotionType activeMotionType = _active->GetMotionType();
	// 계산용 변수
	FVector SendPos = _active->GetLocation();
	FVector SendDest = _active->GetDestination();

	// 목적지 까지 이동가능한지 여부를 체크한다.
	FVector outDetectPos;
	FVector outHitNormal;
	bool navModifyDest = false;
	auto movement = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>()->GetDebugShapeComponent();
	if (false == CheckNavMeshRayCast(_owner->GetCharacter(), SendPos, SendDest, outDetectPos, outHitNormal))
	{
		GSLOG(Log, TEXT("CheckPathTwoPoints Failed"));

		FHitResult hit;
		if (UGsLevelUtil::TrySweepToLand(hit, _owner->UObject::GetWorld(), outDetectPos, _agentRadius, _agentHalfHeight))
		{
			outDetectPos = hit.ImpactPoint + _agentHalfHeight;
		}

		if (nullptr != movement)
		{
			movement->SetEpicLocation(outDetectPos);
		}
		SendDest = outDetectPos;
		navModifyDest = true;
	}
#if WITH_EDITOR && LOG_ON
	else
	{
		GSLOG(Log, TEXT("\n"));
	}
#endif

#if WITH_EDITOR && LOG_ON
	GSLOG(Log, TEXT("SendLocalPlayerMove : navModifyDest [%d], _lastSendMotionType [%d], SendMotionType [%d]\n"), navModifyDest, _lastSendMotionType, activeMotionType);
#endif
	
	if (nullptr != movement)
	{
		movement->SetDestination(SendDest);
	}
	// 다음 전송시간 조절
	_sendTimer.Reset();

#if WITH_EDITOR && LOG_ON
	GSLOG(Log, TEXT("Send : activeMotionType %d"), activeMotionType);
#endif 

	Send(activeMotionType, SendPos, SendDest);

	// 다음 인터벌 체크 시작
	_sendTimer.Start();
}

void FGsMovementNetSender::Send(MotionType inType, const FVector& inSrc, const FVector& inDest)
{
	// 서버와 동기화된 시간전송
	uint64 sendMilisecond = GNet()->GetNetMilliSecond();

	// 마지막 전송한 타입을 지정(상대방상태를 유추하기 위함)
	_lastSendMotionType = inType;


#if UE_EDITOR
	auto movement = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>()->GetDebugShapeComponent();
	if (nullptr != movement)
	{
		DrawDebugSphere(_owner->GetWorld(), inSrc + FVector(0, 0, 50.0f), 10.f, 50.f, FColor::Green, false, 1.f);
		DrawDebugSphere(_owner->GetWorld(), inDest + FVector(0, 0, 50.0f), 10.f, 50.f, FColor::Red, false, 1.f);
	}
#endif
	// 실제로 패킷전송
	FGsNetSendServiceWorld::SendLocalPlayerMove(inSrc,
		_active->GetDirNetConverted(),
		inDest,
		_active->GetFallingSpeed(),
		static_cast<uchar>(_lastSendMotionType),
		sendMilisecond,
		_seqNum	);


	// 기록
	_lastSendPos = inSrc;
	_lastSendDest = inDest;
}

bool FGsMovementNetSender::CheckNavMeshRayCast(AGsCharacterBase* inCharacter,
	const FVector& inSrc, const FVector& inDest, FVector& outDetect, FVector& outHitNormal)
{
	// [B1] | ejrrb10 | 액터가 이미 삭제된 상황에서 이동 취소를 하려는 상황
	if (nullptr == inCharacter)
		return false;
	//check(inCharacter);

	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(inCharacter->GetWorld());
	if (NavSys)
	{
		const ANavigationData* NavData = inCharacter ? NavSys->GetNavDataForProps(inCharacter->GetNavAgentPropertiesRef()) : nullptr;
		if (NavData)
		{
			FSharedConstNavQueryFilter QueryFilter = NavData->GetDefaultQueryFilter();
			const ARecastNavMesh* RecastNavMesh = Cast<const ARecastNavMesh>(NavData);
			ARecastNavMesh::FRaycastResult hitResult;
			if (RecastNavMesh && ARecastNavMesh::NavMeshRaycast(RecastNavMesh, inSrc, inDest, outDetect, QueryFilter, inCharacter, hitResult))
			{
				return hitResult.bIsRaycastEndInCorridor && !hitResult.HasHit();
			}
		}
	}
	return true;
}

void FGsMovementNetSender::ValidatePosSequence(const uint64& inSeq, bool inResponse)
{
	_seqNum = inSeq;

	// 답신을 하지 않으도 되는 상황(SC_FORCED_POS) 에서는 값만 세팅하고 리턴
	if (false == inResponse)
	{
		return;
	}

	MotionType activeMotionType = _active->GetMotionType();
	// 계산용 변수
	FVector SendPos = _active->GetLocation();
	FVector SendDest = _active->GetDestination();
	int16	CurrDir = _active->GetDirNetConverted();
	uint64 sendMilisecond = GNet()->GetNetMilliSecond();

	// 목적지 까지 이동가능한지 여부를 체크한다.
	FVector outDetectPos;
	FVector outHitNormal;
	if (false == CheckNavMeshRayCast(_owner->GetCharacter(), SendPos, SendDest, outDetectPos, outHitNormal))
	{
		GSLOG(Log, TEXT("CheckPathTwoPoints Failed"));
		FHitResult hit;
		if (UGsLevelUtil::TrySweepToLand(hit, _owner->UObject::GetWorld(), outDetectPos, _agentRadius, _agentHalfHeight))
		{
			outDetectPos = hit.ImpactPoint + _agentHalfHeight;
		}
	}

	FGsNetSendServiceWorld::SendLocalPlayerPosValidate(SendPos,
		CurrDir,
		SendDest,
		(uchar)activeMotionType,
		sendMilisecond,
		_seqNum);
}
