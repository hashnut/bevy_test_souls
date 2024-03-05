// Fill out your copyright notice in the Description page of Project Settings.

#include "GsGameObjectSpaceCrackNonPlayer.h"

#include "Components/WidgetComponent.h"

#include "ActorEx/GsCharacterNonPlayer.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorComponentEx/GsHitEffectComponent.h"
#include "ActorComponentEx/GsEffectComponent.h"
#include "ActorComponentEx/GsWidgetComponentPooling.h"


#include "GameObject/Define/GsGameObjectText.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/State/NonPlayer/GsStateNonPlayerIdle.h"
#include "GameObject/Stat/GsStatNonPlayer.h"
#include "GameObject/Target/GsTargetHandlerNonPlayer.h"
#include "GameObject/Skill/GsSkillHandlerNonPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Movement/GsMovementHandlerRemote.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerNonPlayer.h"
#include "GameObject/State/NonPlayer/GsStateNonPlayerSpawn.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Npc/GsSchemaNpcBalance.h"

#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"

#include "AI/Function/GsAIFunc.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsMinimapManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"

#include "Quest/Management/GsQuestTargetManagement.h"

#include "Data/GsResourceManager.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"

#include "Map/MiniMap/GsMinimapFunc.h"
#include "Map/MiniMap/MapIcon/GsMinimapIconHandler.h"

#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "UTIL/GsUIUtil.h"

#include "Components/SphereComponent.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/Classes/Components/CapsuleComponent.h"

#include "Engine/Classes/Particles/ParticleSystem.h"
#include "GameObject/State/NonPlayer/GsStateNonPlayerSpaceCrackIdle.h"

#include "Net/GsNetSendServiceWorld.h"
#include "../Management/ScopeGlobal/GsSoundManager.h"
#include "../UTIL/GsTableUtil.h"



void UGsGameObjectSpaceCrackNonPlayer::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectSpaceCrackNonPlayer::GetObjectType());

	// interaction 
	_interactionMessageParam._objectType = GetObjectType();
	_interactionMessageParam._targetGameID = GetGameId();

	//bak1210 : final 에서호출
	CreateSubClass();
}

void UGsGameObjectSpaceCrackNonPlayer::InitializeActor(AActor* inOwner)
{
	Super::InitializeActor(inOwner);

	if (AGsCharacterNonPlayer* npc = Cast<AGsCharacterNonPlayer>(inOwner))
	{
		npc->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UGsGameObjectSpaceCrackNonPlayer::OnBeginOverlapActor);
		_interactionBlockCollider = npc->GetInteractionBlockCollider();
		if (npc->GetCapsuleComponent())
		{
			npc->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Trigger"));
			npc->GetNameWidgetComponent()->SetVisibility(false);
		}
	}
}

void UGsGameObjectSpaceCrackNonPlayer::InitializeFSM()
{
	if (_spawnReason == SPAWN_REASON::NORMAL || _spawnReason == SPAWN_REASON::SUMMON)
	{
		_fsm->Initialize(BPStateMachineName, this, FGsStateNonPlayerSpawn::GetInstance());
	}
	else
	{
		_fsm->Initialize(BPStateMachineName, this, FGsStateNonPlayerSpaceCrackIdle::GetInstance());
	}
}

bool UGsGameObjectSpaceCrackNonPlayer::OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId,
	uint32 StrikerNotifyId, HitResultType ResultType, DamageByType DamageType, DamageStatType DamageStat, HealByType HealType,
	int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage)
{
	return false;
}

void UGsGameObjectSpaceCrackNonPlayer::ActorDespawn()
{
	if (AGsCharacterNonPlayer* npc = Cast<AGsCharacterNonPlayer>(_actor))
	{
		npc->GetCapsuleComponent()->OnComponentBeginOverlap.Clear();
	}

	Super::ActorDespawn();
}

void UGsGameObjectSpaceCrackNonPlayer::OnBeginOverlapActor(UPrimitiveComponent* inHitComponent, AActor* inOtherActor, UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex, bool inbFromSweep, const FHitResult& inHit)
{
	if (inOtherActor)
	{
		if (inOtherActor->IsA(AGsCharacterLocalPlayer::StaticClass()))
		{
			FGsNetSendServiceWorld::SendNpcSpecialFunction(GetGameId());
			
			// 사운드 재생
			PlayOverlapSound();		
		}
	}
}

void UGsGameObjectSpaceCrackNonPlayer::PlayOverlapSound()
{
	bool isSpaceCrackBoss = GetNpcData()->grade == CreatureRankType::ELITE;
	int32 soundResId = 0;
	if (isSpaceCrackBoss)
	{
		soundResId = GData()->GetGlobalData()->_spaceCrackBossSoundResId;
	}
	else
	{
		soundResId = GData()->GetGlobalData()->_spaceCrackNormalSoundResId;
	}

	//play sound
	const FGsSchemaSoundResData* soundData = UGsTableUtil::FindRowById<UGsTableSoundResData, FGsSchemaSoundResData, int32>(soundResId);	
	if (soundData)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySoundTarget(soundData, GetActor()->GetRootComponent());
		}
	}
}
