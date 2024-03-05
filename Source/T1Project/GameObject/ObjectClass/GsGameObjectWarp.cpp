// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameObjectWarp.h"

//ue4
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

//network
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "Net/GsNetSendServiceWorld.h"

#include "ActorEx/GsActorWarp.h"

//gameobject
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionMoveType.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/Define/GsGameObjectStruct.h"

//t1 project
#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "AI/Quest/GsAIQuestHandler.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsActorPropObject.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UILib/Manager/GsUIController.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapSpotInfo.h"
#include "DataSchema/Map/GsSchemaMapWarpInfo.h"
#include "DataSchema/Map/GsSchemaMapWarpSpot.h"
#include "DataSchema/Map/Spot/GsSchemaSpotData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"

#include "Cheat/GsCheatManager.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsLevelUtil.h"

#include "WeakObjectPtrTemplates.h"


UGsGameObjectWarp::UGsGameObjectWarp()
{

}

UGsGameObjectWarp::~UGsGameObjectWarp()
{

}

void UGsGameObjectWarp::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectWarp::GetObjectType());

	AActor* player = GSClientSpawn()->GetPlayer();
	int32 levelKey = GLevel()->GetCurrentLevelId();
	const FGsSchemaMapWarpInfo* warpData = GSClientSpawn()->TryGetWarp(levelKey, SpawnParam->_tId);
	if (nullptr == warpData)
	{
		return;
	}

	_warpId = warpData->warpId;
	_destMapID = warpData->destSpot.MapId.GetRow()->mapId;
	_destSpotIndex = warpData->destSpot.SpotIndex;
	_warpType = warpData->WarpType;

	FGsSchemaSpotInfo spotData;
	if (false == GSClientSpawn()->TryGetSpot(levelKey, warpData->sourceSpot.SpotIndex, spotData))
	{
		return;
	}

	_radius = spotData.radius;

	//bak1210 : final 에서호출
	CreateSubClass();
}

void UGsGameObjectWarp::InitializeActor(AActor* inOwner)
{
	Super::InitializeActor(inOwner);	

	//액터 저장
	AGsActorWarp* actor = Cast<AGsActorWarp>(inOwner);
	if (actor)
	{
		_actor = actor;

		// 충돌 범위 설정
		if (USphereComponent* collider = actor->GetSphereComponent())
		{
			collider->SetSphereRadius(_radius);
		}

		// 지형 위치에 배치 보정
		FCollisionQueryParams queryParam;
		queryParam.AddIgnoredActor(actor);

		FHitResult hit;
		if (UGsLevelUtil::TrySweepToLand(hit, GetWorld(), actor->GetActorLocation(), 100.f, 200.f, queryParam))
		{
			actor->SetActorLocation(hit.ImpactPoint);
		}
	}
}

void UGsGameObjectWarp::OnBeginOverlap(UPrimitiveComponent* inHitComponent, AActor* inOtherActor, UPrimitiveComponent* inOtherComp, int32 inOherBodyIndex, bool inbFromSweep, const FHitResult& inHit)
{
	AGsCharacterLocalPlayer* localActor = Cast<AGsCharacterLocalPlayer>(inOtherActor);
	if (nullptr == localActor)
	{
		return;
	}

	UGsGameObjectLocalPlayer* localplayer = localActor->GetGameObjectPlayer();
	if (nullptr == localplayer)
	{
		return;
	}

	if (GGameData()->IsInvadeWorld())
	{
		FText findText;
		FText::FindText(TEXT("InvadeText"), TEXT("CantMoveOtherMap"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}

	if (localplayer->IsDeadState())
	{
		// bak1210 :
		// 인터서버 처리중 나온예외 HP0인 상태에서 텔리포트시, 움직일수있지만 스킬사용불가 대응
		return;
	}


	if (false == UGsLevelUtil::IsCanWarp(_warpId))
	{
		return;
	}
	const FGsSchemaMapData* map = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(_destMapID);
	if (map == nullptr)
	{
		return;
	}
	FGsMovementHandlerLocalPlayer* movement = localplayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (movement == nullptr)
	{
		return;
	}

	EGsMovementAutoContentsType autoMoveType = movement->GetStateAutoMove().GetMovementAutoContentsType();

	// https://jira.com2us.com/jira/browse/C2URWQ-4242
	// 퀘스트 자동이동으로 다른맵 넘어갈때
	// 워프 바로 앞에서 수동 이동으로 이동 캔슬시
	// 이동도착에서 워프를 안보내서
	// 워프 안가고 워프 안에서 서있는경우
	// 다른 유저(remote)가 자동이동으로 워프 안되는 경우 생김
	// 자동이동에서 처리하는게 아니라
	// 워프자체에서 하자	

	bool isNextMapAutoMoving = false;
	// 다음맵 자동 이동으로 온거면 바로 워프 시작
	switch (autoMoveType)
	{
	case EGsMovementAutoContentsType::QuestMove:
	case EGsMovementAutoContentsType::RegionMapBookmarkMove:
	case EGsMovementAutoContentsType::RegionMapPortalMove:
	case EGsMovementAutoContentsType::TouchUIMapMove:
		isNextMapAutoMoving = true;
		break;
	default:
		isNextMapAutoMoving = false;
	}

	if (_warpType == WarpType::YesNoDialog && 
		isNextMapAutoMoving == false)
	{	
		// bak1210
		// 팝업이 출력된후 맞아서 죽는 상황에는 UI매니져네에서 다른 팝업은 일괄 제거함.
		FText format;
		FText::FindText(TEXT("UICommonText"), TEXT("Warp_Alert"), format);
		FText msg = FText::Format(format, map->MapStringId);

		_warpPopup = FGsUIHelper::PopupYesNo(msg,
			[this](bool inSuccess) {
				OnWarp(inSuccess);
				_warpPopup.Reset();
			});	
	}
	else
	{
		// 위젯에서 워프로 자동닫기 막는 위젯이 있다면 워프 안함
		// ex) 뽑기창, 페어리창, 코스튬창,강화창
		bool isExistWidgetPreventAutoClose = false;
		UGsUIController* uiController = GUI()->GetUIController();
		if (uiController != nullptr)
		{
			isExistWidgetPreventAutoClose =
				uiController->IsExistWidgetPreventAutoClose();
		}
		if (isExistWidgetPreventAutoClose == true)
		{
			if (autoMoveType == EGsMovementAutoContentsType::QuestMove)
			{
				// hud 액티브 끄기
				// 퀘스트 쪽에 stop 요청
				GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_ACTIVE_OFF, nullptr);
			}
		}
		else
		{ 
			OnWarp();
		}
	}
	
	
}

void UGsGameObjectWarp::OnEndOverlap(UPrimitiveComponent* inHitComponent, AActor* inOtherActor, UPrimitiveComponent* inOtherComp, int32 intoherBodyIndex)
{
	AGsCharacterLocalPlayer* localActor = Cast<AGsCharacterLocalPlayer>(inOtherActor);
	if (nullptr == localActor)
	{
		return;
	}

	UGsGameObjectLocalPlayer* localplayer = localActor->GetGameObjectPlayer();
	if (nullptr == localplayer)
	{
		return;
	}

	//close popup
	if (_warpPopup.IsValid())
	{
		GUI()->Close(Cast<UGsUIWidgetBase>(_warpPopup.Get()));
		_warpPopup.Reset();
	}	
}

EGsGameObjectType UGsGameObjectWarp::GetObjectType() const
{
	return EGsGameObjectType::Warp;
}

void UGsGameObjectWarp::OnWarp(bool inSuccess)
{
	if (inSuccess)
	{
		// ai next move flag prevent
		GSAI()->SetNextMapWarpStart();

		GMessage()->GetGameObject().SendMessage(MessageGameObject::OBJ_WARP_START, nullptr);

		FGsNetSendServiceWorld::SendWarp(_warpId);

#if WITH_EDITOR
		GSLOG(Log, TEXT("Send warp request - warp id : %lld"), (int64)_warpId);
#endif
	}
	else
	{
		GSLOG(Error, TEXT("Warp fail"));
	}
}
