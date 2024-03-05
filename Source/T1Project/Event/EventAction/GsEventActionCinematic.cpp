#include "GsEventActionCinematic.h"

#include "Runtime/Core/Public/Templates/UnrealTemplate.h"
#include "GsSchemaBase.h"

#include "Cinematic/GsSchemaSequenceResData.h"

#include "Management/GsMessageHolder.h"
#include "Management/GsScopeHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Message/GsMessageSystem.h"
#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Util/GsDir.h"

void UGsEventActionCinematic::BeginDestroy()
{
	RemoveObserver();

	Super::BeginDestroy();
}

void UGsEventActionCinematic::Initialize(const FGsSchemaBase* EventActionData)
{
	const FGsSchemaSequenceResData* SequenceResData = StaticCast<const FGsSchemaSequenceResData*>(EventActionData);
	if (nullptr == SequenceResData)
	{
		GSLOG(Error, TEXT("nullptr == SequenceResData"));
		return;
	}

	_sequenceResId = SequenceResData->sequenceResId;
}

bool UGsEventActionCinematic::ShouldReplayOnReconnection() const
{
	return true;
}

bool UGsEventActionCinematic::OnPlay()
{
	AddObserver();

	// 시네마틱 플레이 전 ui 다 닫기
	GUI()->CloseAllStack();
	GUI()->HideTicker(); //Ticker 선별적 hide & 추가되는 Ticker에 대해서도 

	// 소환 연출중이면 닫기
	FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
	if (summonHandler == nullptr)
	{
		return false;
	}

	if (summonHandler->GetIsSummonPlay() == true)
	{
		summonHandler->EndSummon();
		GUI()->CloseByKeyName("TraySummon", true);
	}

	//시네마틱 플레이시 플레이어 위치에서 시쿼스가 멀리 플레이 된다면 플레이어가 선 streamingLevel이 unload되는 현상이 있다. 
	//시네마틱 플레이가 끝날 때 까지 플레이어 위치의 streaming level을 언로드하지 않도록 설정한다. 
	UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (localPlayer)
	{
		if (AActor* playerActor = localPlayer->GetActor())
		{
			GLevel()->HoldStreamingLevelByLocation(playerActor->GetActorLocation());
		}
	}

	//https://jira.gamevilcom2us.com/wiki/pages/viewpage.action?pageId=241003210
	FVector dir = localPlayer->GetRotation().Vector();
	FGsNetSendServiceWorld::SendCinematicStartByEventAction(_sequenceResId, localPlayer->GetLocation(), FGsDir::dirToAngle(dir));

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		FGsSequenceMessageParam Param(_sequenceResId);
		GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_START, &Param);
	}

	return true;
}

void UGsEventActionCinematic::OnFinish()
{
	GSLOG(Log, TEXT("_sequenceResId:%d"), _sequenceResId);

	GUI()->ShowTicker();

	//https://jira.gamevilcom2us.com/wiki/pages/viewpage.action?pageId=241003210
	FGsNetSendServiceWorld::SendCinematicEndByEventAction(_sequenceResId);

	RemoveObserver();
}

void UGsEventActionCinematic::AddObserver()
{
	if (_SequencePlayerStopHandle.Value.IsValid())
	{
		GSLOG(Error, TEXT("_gameObjectSequencePlayerStopHandle.Value.IsValid(), _sequenceResId:%d"), _sequenceResId);
	}

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		_SequencePlayerStopHandle
			= MessageHolder->GetSystemParam().AddUObject(MessageSystem::SEQUENCE_PLAYER_END
				, this, &UGsEventActionCinematic::OnStopMovie);
	}
}

void UGsEventActionCinematic::RemoveObserver()
{
	if (!_SequencePlayerStopHandle.Value.IsValid())
	{
		return;
	}

	if (nullptr == GScope())
	{
		// 시네마틱 진행 중 게임 정지하면 이 부분에서 크래쉬를 발생시켜 예외 처리함.
		return;
	}

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetSystemParam().Remove(_SequencePlayerStopHandle);
		_SequencePlayerStopHandle.Value.Reset();
	}
}

void UGsEventActionCinematic::OnStopMovie(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsSequenceMessageParam* Param = InParam->Cast<const FGsSequenceMessageParam>();
	if (_sequenceResId != Param->_sequenceId)
	{
		return;
	}

	Finish();
}

bool UGsEventActionCinematic::OnRewind()
{
	if (!IsPlaying())
	{
		// 플레이 중일때만 아래 작업 필요
		return false;
	}

	RemoveObserver();

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		FGsSequenceMessageParam Param;
		MessageHolder->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_FORCE_STOP, &Param);
	}

	return true;
}
