// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameObjectRemotePlayer.h"

#include "ActorEx/GsCharacterRemotePlayer.h"
#include "ActorComponentEx/GsEffectComponent.h"

#include "GameObject/Parts/GsPartsHandlerPlayer.h"
#include "GameObject/Skill/GsSkillHandlerRemotePlayer.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Stat/GsStatRemotePlayer.h"
#include "GameObject/Data/GsGameObjectDataRemotePlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Define/GsGameObjectText.h"
#include "GameObject/State/RemotePlayer/GsStateRemotePlayerIdle.h"
#include "GameObject/Movement/GsMovementHandlerRemote.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/Fairy/GsFairyHandlerPlayer.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/LockOn/GsLockOnHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Observer/GsObserverHandler.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GsGameFlowGame.h"

#include "Data/GsDataContainManager.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"

#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"

#include "Map/WorldMap/GsWorldMapFunc.h"
#include "Map/Minimap/GsMinimapFunc.h"

#include "Party/GsPartyMemberInfo.h"

#include "Message/MessageParam/GsMinimapMessageParam.h"
#include "Message/GsMessageContentHud.h"
#include "Classes/GsSpawn.h"

#include "Net/GsNetSendService.h"

#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"



//프로퍼티
EGsGameObjectType UGsGameObjectRemotePlayer::GetObjectType() const			{ return EGsGameObjectType::RemotePlayer; }
AActor* UGsGameObjectRemotePlayer::GetActor() const							{ return GetRemoteCharacter(); }
void	UGsGameObjectRemotePlayer::ClearActor()								{ _actor = nullptr; }

void UGsGameObjectRemotePlayer::OnDie()
{
	Super::OnDie();

	GMessage()->GetGameObject().SendMessage(MessageGameObject::REMOTEPLAYER_DIE, nullptr);

	GSGameObject()->StopCounterAttackShow(GetGameId());
}

AGsCharacterPlayer* UGsGameObjectRemotePlayer::GetRemoteCharacter() const	{ return _actor; }
//


UGsGameObjectRemotePlayer::UGsGameObjectRemotePlayer() : Super()
{

}

UGsGameObjectRemotePlayer::~UGsGameObjectRemotePlayer()
{

}

void UGsGameObjectRemotePlayer::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

	//타입 정의
	SET_FLAG_TYPE(_objectType, UGsGameObjectRemotePlayer::GetObjectType());

	//bak1210 : final 에서호출
	CreateSubClass();

	_actor = nullptr;	
}


TSharedPtr<FGsPartsHandlerBase> UGsGameObjectRemotePlayer::CreateParts()
{
	return MakeShareable(new FGsPartsHandlerPlayer());
}

TSharedPtr<FGsSkillHandlerBase> UGsGameObjectRemotePlayer::CreateSkill()
{
	return MakeShareable(new FGsSkillHandlerRemotePlayer());
}

TSharedPtr<FGsStatBase> UGsGameObjectRemotePlayer::CreateCreatureStat()
{
	if (FGsGameObjectDataCreature* creatureData = GetCastData<FGsGameObjectDataCreature>())
	{
		if (false == creatureData->GetStatBase().IsValid())
		{
			return creatureData->CreateCreatureStat<FGsStatRemotePlayer>(this);
		}

		return StaticCastSharedPtr<FGsStatBase>(creatureData->GetStatBase());
	}

	return nullptr;
}

void UGsGameObjectRemotePlayer::Update(float Delta)
{
	Super::Update(Delta);
}

void UGsGameObjectRemotePlayer::InitializeActor(AActor* Owner)
{
	_actor = Cast<AGsCharacterPlayer>(Owner);
	check(_actor);

	Super::InitializeActor(Owner);
	
	// 필요 Primitive 셋업
	_actor->SetupPrimitives(false);

	//bak1210 : 케릭터끼리 오버랩된 상황 캐취
	_actor->OnOverlapedCharacterBase.BindUObject(this, &UGsGameObjectRemotePlayer::CallbackOverlapedCharacterBase);
	_clientCollisionRule = false;

	//Hit 모션 추가
	FSoftObjectPath path;
	if (_actor->LoadHitAnimInstance(path))
	{
		LegacyHit = false;
	}
	// 네임플레이트 정보 설정 
	if (UGsUIBillboardCreature* widget = _actor->GetWidget())
	{
		widget->Reset();
		widget->SetTarget(this);

		// 주의: SetTarget 후에 수행
		if (AGsCharacterBase* character = GetCharacter())
		{
			character->UpdateRenderState();
		}
	}

	// 무기 타입에 따른 AnimationBP설정
	if (FGsGameObjectDataRemotePlayer* remoteData = GetCastData<FGsGameObjectDataRemotePlayer>())
	{
		UpdateWeaponChange(remoteData->GetCurrentWeaponType());
	}

	// visible 업데이트
	UpdateVisible();

	// 타게팅 클릭 연동
	if (AGsCharacterRemotePlayer* remoteCharacter = Cast<AGsCharacterRemotePlayer>(GetRemoteCharacter()))
	{
		remoteCharacter->SetInteractionClickCallback([this](bool In_isByWidget)
			{
				OnClickTargeting();
			});
	}
}

void UGsGameObjectRemotePlayer::InitializeFSM()
{
	// FSM생성 초기화
	if (_fsm)
	{
		_fsm->Initialize(BPStateMachineName, this, FGsStateRemotePlayerIdle::GetInstance());
	}
}

void UGsGameObjectRemotePlayer::CreateSubClass()
{
	Super::CreateSubClass();

	AppendInitialize(CreateSkill());
	AppendInitialize(CreateMovement());
	
	if (const FGsSchemaCreatureCommonInfo* commonInfoTable = GetCommonInfoTable())
	{
		GetMovementHandler()->InitSpeed(commonInfoTable->moveSpeed, commonInfoTable->walkSpeed);
	}
	
	
	int64 gameId = GetGameId();
	// 월드맵 갱신(파티 속할때만)
	if (GSParty()->GetIsInParty() == true)
	{
		// check party member world id
		WorldId localNowMapWorldId = GGameData()->GetLoadWorldId();

		FGsPartyMemberInfo* partyMemberInfo = GSParty()->FindPartyMemberInfoByGameId(gameId);
		WorldId remoteWorldId = 0;
		if (partyMemberInfo != nullptr)
		{
			remoteWorldId = partyMemberInfo->GetWorldId();
		}

		bool isSameWorldWithLocal = true;
		if (remoteWorldId != 0 &&
			remoteWorldId != localNowMapWorldId)
		{
			isSameWorldWithLocal = false;
		}

		EGsRegionMapIconType findRegionIconType = EGsRegionMapIconType::None;
		// 1. check world id(with local)
		// 2. 해당 타입이 없으면 안함	
		if (isSameWorldWithLocal == true &&
			FGsWorldMapFunc::GetPartyIconType(gameId, findRegionIconType) == true)
		{
			FGsGameObjectMessageParamRegionMapIcon regionParamIcon(this, findRegionIconType);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::REGIONMAP_SPAWN_SERVER_ICON, &regionParamIcon);
		}
	}


	// remote player  스폰
	// 일단 레드로 놓고 갱신에서 바꿈	
	EGsMapIconType findIconType = EGsMapIconType::RemoteTeamRed;
	bool isPartyMember = FGsMinimapFunc::GetPartyIconType(gameId, findIconType);
	FGsGameObjectMessageParamMapIcon paramIcon(this, findIconType, isPartyMember);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_SPAWN_ICON, &paramIcon);

}

void UGsGameObjectRemotePlayer::ActorDespawn()
{
	// remote player  디스폰
	// 미니맵 타이머 때문에 크래쉬 날수도 있어서 미리 디스폰 처리
	int64 gameId = GetGameId();
	FGsGameObjectMessageParamMapIcon paramIcon(gameId, _despawnReason);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_DESPAWN_ICON, &paramIcon);

	// 월드맵 갱신(파티 속할때만)
	if (GSParty()->GetIsInParty() == true)
	{		
		FGsGameObjectMessageParamRegionMapIcon regionParamIcon(gameId, _despawnReason);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::REGIONMAP_DESPAWN_SERVER_ICON, &regionParamIcon);
		
		if (_despawnReason != SPAWN_REASON::GRID)
		{
			GSParty()->OnDespawnChange(gameId, true);
		}

		GSParty()->OnDespawn(gameId);
	}


	// 모든 SubClass들을 정리하고 난후 _actor 포인터 소멸
	Super::ActorDespawn();
	
}

void UGsGameObjectRemotePlayer::SetRemotePlayerHidden()
{
	if (_actor == nullptr || _actor->IsHidden())
		return;

	_actor->SetActorHiddenInGame(true);

	if (FGsFairyHandlerPlayer* fairyHandler = GetFairyHandler())
	{
		fairyHandler->SetFairyHiddenInGame();
	}
}

FGsGameObjectData* UGsGameObjectRemotePlayer::CreateData(const FGsSpawnParam* SpawnParam)
{
	return new FGsGameObjectDataRemotePlayer(SpawnParam);
}

TSharedPtr<FGsMovementHandlerBase> UGsGameObjectRemotePlayer::CreateMovement()
{
	return MakeShareable(new FGsMovementHandlerRemote());
}

void UGsGameObjectRemotePlayer::NetChangeWeaponFinishedAck(PD::SC::PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED_READ* Packet)
{
	Super::NetChangeWeaponFinishedAck(Packet);

	if (Packet->Reason() == ChangeWeaponTypeFinishReason::NORMAL)
	{
		UpdateWeaponChange(Packet->WeaponType());
	}
}

void UGsGameObjectRemotePlayer::OnStiffen(const FVector& StiffenPos, float fDuration)
{
	if (FGsMovementHandlerRemote* moveRemort = GetCastMovement<FGsMovementHandlerRemote>())
	{
		moveRemort->ReceveStiffen(GetRotation().Vector(), StiffenPos);
	}
}

// iff 변경됨
void UGsGameObjectRemotePlayer::OnIFFChanged()
{
	// 나 미니맵 팀 변경
	FGsMinimapIconTeamMessageParam param(false, GetData()->GetGameId());
	GMessage()->GetUIMinimapIconTeam().SendMessage(MessageContentMinimap::INVALIDATE_IFF, &param);
}

void UGsGameObjectRemotePlayer::OnWarp()
{
	// [B1] | ejrrb10 | 워프 이펙트 애니메이션 출력을 해 주어야 함
	// 이전에는 리모트 플레이어의 경우 워프 애니메이션 재생이 안 되고 있었음
	// 워프 애니메이션을 기다리지 않고 바로 디스폰 처리를 하기 때문
	// SC_ACK_SOCIAL_ACTION 패킷을 받고, 딜레이 이후 디스폰 처리(SC_DESPAWN_USER)가 이루어짐
	 Super::OnWarp();

	//// 타유저는 애니 플레이 않하고 이펙트 출력만 한다
	//// 대기하다가 스폰이 먼저오면 game id가 같아서 actor를 같은걸 써서 
	//// 뒤에 타임 딜레이 디스폰오면 actor 사라지는 이슈때문
	//AGsCharacterPlayer* characterPlayer = GetRemoteCharacter();
	//if (nullptr == characterPlayer)
	//{
	//	return;
	//}
	//UGsEffectComponent* effComp = characterPlayer->GetEffectComponent();
	//if (effComp == nullptr)
	//{
	//	return;
	//}

	//effComp->OnEffectPos(GData()->GetGlobalData()->_warpParticle, 
	//	characterPlayer->GetMovementComponent()->GetActorFeetLocation(),
	//	FVector::OneVector);
}

void UGsGameObjectRemotePlayer::OnSpawn(bool In_isAlive, bool In_isNormalReason)
{
	// 죽어 있는 상태면 상태 변경 처리
	if (In_isAlive == false)
	{
		_fsm->ProcessEvent(EGsStateBase::Dead);
	}
	// 살아있고
	else
	{
		// 일반적인 스폰이면
		// SPAWN 스테이트로 넘김
		if (In_isNormalReason == true)
		{
			_fsm->ProcessEvent(EGsStateBase::Spawn);

			// [B1] | ejrrb10 | 일반적인 스폰일 때만 페어리 스폰 연출을 타 플레이어에게 노출
			Super::OnSpawnFairyEffect();
			Super::OnPlayerTeleportEffect(ETeleportEffectType::ARRIVE);
		}
	}
	int64 gameId = GetGameId();
	GSParty()->OnSpawn(gameId);

	UpdateBillboard(EGsBillboardUpdateType::All);
}

// 몸통 클릭 타게팅
void UGsGameObjectRemotePlayer::OnClickTargeting()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsLockOnHandler* lockOnHandler = hud->GetLockOnHandler();
	if (lockOnHandler == nullptr)
	{
		return;
	}

	//GSLOG(Log, TEXT("OnClickTargeting %s"), *GetActor()->GetName());
	lockOnHandler->SetLockOnTarget(this);
}

void UGsGameObjectRemotePlayer::NetDeathAck(PD::SC::PKT_SC_CREATURE_DEATH_READ* Packet)
{
	// 타유저면 매너 타겟 오너에서 삭제
	GSAI()->RemoveMannerTargetOwnerRemote(Packet->DeadGameId());

	// 관전 시스템 처리															
	if (GSObserver()->GetIsObserveStart() == true)
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud))
				{
					if (FGsObserverHandler* observerHandler = hud->GetObserverHandler())
					{
						observerHandler->RemoveObserveTarget(this);
					}
				}
			}
		}
	}

	Super::NetDeathAck(Packet);
}
// widget hp bar show/hide cheat option
void UGsGameObjectRemotePlayer::UpdateWidgetShowHPBar()
{
	AGsCharacterPlayer* character = GetRemoteCharacter();
	if (character == nullptr)
	{
		return;
	}

	UpdateBillboard(EGsBillboardUpdateType::All);
}

void UGsGameObjectRemotePlayer::CallbackOverlapedCharacterBase(int32 inOverlapCount)
{
	_clientCollisionRule = inOverlapCount == 0;
	BlockLocalPlayer(_clientCollisionRule);
}

const FName DefaultOtherPreset = TEXT("OtherCollision");
const FName BlockLocalPreset = TEXT("OtherBlockLocal");
void UGsGameObjectRemotePlayer::BlockLocalPlayer(bool InBlock)
{
	if (AGsCharacterBase* Actor = GetCharacter())
	{		
		Actor->SetCollisionProfileName(_clientCollisionRule ? BlockLocalPreset : DefaultOtherPreset);
	}
}
