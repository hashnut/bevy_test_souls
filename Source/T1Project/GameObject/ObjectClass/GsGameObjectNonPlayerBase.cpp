// Fill out your copyright notice in the Description page of Project Settings.

#include "GsGameObjectNonPlayerBase.h"
#include "Components/CapsuleComponent.h"
#include "ActorEx/GsCharacterBase.h"
#include "ActorEx/GsCharacterNonPlayerBase.h"
#include "ActorComponentEx/GsWidgetComponentTalkBalloon.h"
#include "ActorComponentEx/GsHitEffectComponent.h"

#include "GsGameObjectBase.h"

#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "DataSchema/TalkBalloon/GsSchemaTalkBalloonData.h"
#include "Data/GsDataContainManager.h"

#include "Classes/GsSpawn.h"

#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Skill/GsSkillHandlerNonPlayer.h"
#include "GameObject/Movement/GsMovementHandlerRemote.h"
#include "GameObject/Target/GsTargetHandlerNonPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Stat/GsStatNonPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"

#include "Quest/Management/GsQuestTargetManagement.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsMinimapManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Map/Minimap/GsMinimapFunc.h"
#include "Map/Minimap/MapIcon/GsMinimapIconHandler.h"

#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "UI/UIContent/Billboard/GsUIBillboardTalkBalloon.h"

#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsGameObjectUtil.h"
#include "DrawDebugHelpers.h"

AActor* UGsGameObjectNonPlayerBase::GetActor() const { return _actor; }

void UGsGameObjectNonPlayerBase::Initialize(const FGsSpawnParam* inSpawnParam)
{
	Super::Initialize(inSpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectNonPlayerBase::GetObjectType());

	//NonPlayer 데이터 얻기
	_npcData = UGsTableUtil::FindRow<FGsSchemaNpcData>(_gameObjectData->GetTableId());
	if (_npcData)
	{
		_creatureCommonInfo = _npcData->commonInfoId.GetRow();
		_npcBalance = _npcData->balanceId.GetRow();
		// User 이름은 서버에서 주지만 그외는 따로 설정해야한다.
		_gameObjectData->SetName(_npcData->nameText.ToString());
		_sightRadius = _npcData->sightRadius;
		_spawnInfo.Reset();
	}

	_spawnPos = inSpawnParam->_pos;

	_isTargetedByLocalPlayer = false;
}

void UGsGameObjectNonPlayerBase::Update(float inDelta)
{
	Super::Update(inDelta);

	/*DrawDebugSightRadius();*/
}

void UGsGameObjectNonPlayerBase::InitializeActor(AActor* inOwner)
{
	_actor = Cast<AGsCharacterBase>(inOwner);
	check(_actor);

	Super::InitializeActor(inOwner);
	// 액터 저장
	
	// 필요 Primitive 셋업
	_actor->SetupPrimitives(false);

	float radius = DEFAULT_CAPSULE_RADIUS;
	float halfheight = DEFAULT_CAPSULE_HEIGHT;

	//스폰 정보에서 고정된 좌표인지 정보를 얻어온다.		
	FGsSchemaSpawnData spawnData;
	FGsSchemaSpawnElement spawnElement;
	bool isFixedLocation = false;
	SpawnId uniqueId = _gameObjectData->GetSpawnId();
	if (UGsLevelUtil::TryFindSpawnInfo(GLevel()->GetCurrentLevelId(), uniqueId, spawnData, spawnElement))
	{
		isFixedLocation = spawnElement.isFixedLocation;
	}	
	
	if (_creatureCommonInfo)
	{
		radius = _creatureCommonInfo->cylinderRadius * _npcData->scale;
		halfheight = _creatureCommonInfo->cylinderHeight * _npcData->scale;

		inOwner->SetActorScale3D(FVector(_npcData->scale, _npcData->scale, _npcData->scale));
		if (UCapsuleComponent* capsule = _actor->GetCapsuleComponent())
		{
			// Camp는 bp 메시 구조가 달라서 별도로 높이보정 체크를 진행하기에 제외처리 (중력 영향도 받지 않아야함)
			if (EGsGameObjectType::Camp != GetObjectType() &&
				EGsGameObjectType::Sanctum != GetObjectType())
			{
				float scale = _npcData->scale;

#if SPAWN_DEBUG
				//Npc 스폰 위치 			
				FString log = FString::Printf(TEXT("Nonplayer spawn - label : %s\tlocation : %s"), *_actor->GetActorLabel(), *_spawnPos.ToString());
				DrawDebugString(UObject::GetWorld(), _spawnPos, log, NULL, FColor::Yellow, 300);
				DrawDebugCapsule(UObject::GetWorld(), _spawnPos, halfheight, radius, FQuat::Identity, FColor::Yellow, false, 300);				
				GSLOG(Log, TEXT("%s"), *log);
#endif

				//고정된 좌표라면 높이 보정을 하지 않는다. 
				if (isFixedLocation)
				{
					FVector reCalcPos = _spawnPos + FVector(0, 0, capsule->GetScaledCapsuleHalfHeight());
					_actor->SetActorLocation(reCalcPos);

					//중력 비활성화
					if (AGsCharacterBase* character = Cast<AGsCharacterBase>(_actor))
					{
						character->ActiveGravity(false);
					}
				}
				else
				{
					// 스케일 변화가 있을경우 위치를 다시 갱신
					// 높이 보정
					FHitResult hit;
					if (UGsLevelUtil::TrySweepToLand(hit, GetWorld(), _spawnPos, radius, halfheight))
					{
						FVector reCalcPos = hit.ImpactPoint + FVector(0, 0, capsule->GetScaledCapsuleHalfHeight());
						_actor->SetActorLocation(reCalcPos);

#if SPAWN_DEBUG
						//Npc spawn 보정 위치 
						log = FString::Printf(TEXT("Nonplayer spawn - label : %s\tlocation : %s"), *_actor->GetActorLabel(), *reCalcPos.ToString());
						DrawDebugString(UObject::GetWorld(), reCalcPos, log, NULL, FColor::Blue, 300);						
						DrawDebugCapsule(UObject::GetWorld(), reCalcPos, halfheight, radius, FQuat::Identity, FColor::Blue, false, 300);
						GSLOG(Log, TEXT("%s"), *log);
#endif
					}
					else
					{						
#if SPAWN_DEBUG						
						GSLOG(Log, TEXT("Nonplayer spawn  - fall - label : %s"), *_actor->GetActorLabel());
#endif
					}
				}							
			}
		}
	}

	if (nullptr != _npcData)
	{
		const FGsSchemaShapeData* shape = _npcData->shapeId.GetRow();
		if (nullptr != shape)
		{
			if (_actor->LoadHitAnimInstance(shape->hitAnimInstancePath))
			{
				LegacyHit = false;
			}
		}
	}

	InitializeHitEffect();

	//InitNpcDebugWidget();

	FindTargetHandler();
	// visible 업데이트
	UpdateVisible();
}

void UGsGameObjectNonPlayerBase::FindTargetHandler()
{
	_targetHandler = UGsGameObjectUtil::FindTargetHandlerLocalPlayer();
}

void UGsGameObjectNonPlayerBase::BindMessage()
{
	Super::BindMessage();

	FGsMessageHolder* msg = GMessage();
	if (msg)
	{
		AddMessageDelegate(msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_TARGET_CHANGED,
			this, &UGsGameObjectNonPlayerBase::OnCheckIsTargetedByLocalPlayer));
		AddMessageDelegate(msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_TARGET_CLEAR,
			this, &UGsGameObjectNonPlayerBase::OnCheckIsTargetedByLocalPlayer));
		AddMessageDelegate(msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_TARGET_CLEAR_FORCE,
			this, &UGsGameObjectNonPlayerBase::OnCheckIsTargetedByLocalPlayer));
	}	
}

void UGsGameObjectNonPlayerBase::ActorDespawn()
{
	bool isSubQuest = false;

	if (UGsQuestManager* questManager = GSQuest())
	{
		FGsQuestTargetManagement* questTargetManagement = questManager->GetQuestTargetManagement();
		if (questTargetManagement != nullptr && _npcData != nullptr)
		{
			// 퀘스트 마크 처리
			int tblId = GetNpcTblId();
			// npc 면
			if (_npcData->iffFactionType != IffFactionType::MONSTER)
			{
				// 퀘스트 마크 처리				
				if (questTargetManagement->IsQuestSubNpc(tblId))
				{
					isSubQuest = true;
				}
			}
		}
	}


	// 미니맵 npc 쓴다면
	// npc  디스폰
	// 미니맵 타이머 때문에 크래쉬 날수도 있어서 미리 디스폰 처리
	// 서브 퀘스트면 여기서 처리할필요 없음(글로벌하게 처리로 변경됨)
	if (isSubQuest == false)
	{
		FGsGameObjectMessageParamMapIcon paramIcon(GetGameId());
		GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_DESPAWN_ICON, &paramIcon);
	}

	HideTalkBalloon();

	// 모든 SubClass들을 정리하고 난후 _actor 포인터 소멸
	Super::ActorDespawn();

	DeathHandleReset();
}


TSharedPtr<FGsSkillHandlerBase> UGsGameObjectNonPlayerBase::CreateSkill()
{
	// 스킬 핸들러 생성
	return MakeShareable(new FGsSkillHandlerNonPlayer());
}

TSharedPtr<FGsMovementHandlerBase> UGsGameObjectNonPlayerBase::CreateMovement()
{
	// 무브먼트 생성
	return MakeShareable(new FGsMovementHandlerRemote());
}

TSharedPtr<FGsStatBase> UGsGameObjectNonPlayerBase::CreateCreatureStat()
{
	if (FGsGameObjectDataCreature* creatureData = GetCastData<FGsGameObjectDataCreature>())
	{
		if (false == creatureData->GetStatBase().IsValid())
		{
			return creatureData->CreateCreatureStat<FGsStatNonPlayer>(this);
		}

		return StaticCastSharedPtr<FGsStatBase>(creatureData->GetStatBase());
	}

	return nullptr;
}

void UGsGameObjectNonPlayerBase::CreateSubClass()
{
	Super::CreateSubClass();

	// Fsm 초기화
	// Spawn 상태에 따라 초기 상태설정 변경
	InitializeFSM();
	AppendInitialize(CreateSkill());
	InitializeMovement();	


	// 지연 Initialize 등록
	AddLateCreateSubClassEvent([&]() { InitializeNameWidget(); });
	AddLateCreateSubClassEvent([&]() { InitializeUI(); });
}

void UGsGameObjectNonPlayerBase::RemoveSubClass()
{
	DeathHandleReset();

	Super::RemoveSubClass();
}

void UGsGameObjectNonPlayerBase::InitializeFSM()
{

}

void UGsGameObjectNonPlayerBase::InitializeMovement()
{

}

void UGsGameObjectNonPlayerBase::InitializeNameWidget()
{
	// 네임플레이트 바인딩 설정(Stat 바인딩을 하므로 Stat 생성 뒤에 위치해야 한다)
	if (UGsUIBillboardCreature* widget = _actor->GetWidget())
	{		
		if (_npcData)
		{
			widget->SetNameText(_npcData->nameText);
			widget->Reset();
			widget->SetTarget(this);

			// 주의: SetTarget 후에 수행
			if (AGsCharacterBase* character = GetCharacter())
			{
				character->UpdateRenderState();
			}
		}
	}
}

void UGsGameObjectNonPlayerBase::InitializeHitEffect()
{
	// 히트 이펙트 데이터 설정
	if (UGsHitEffectComponent* hitComp = _actor->GetHitEffect())
	{
		if (_npcData)
		{
			UGsHitEffectData* effectInfo = _creatureCommonInfo->hitEffectDataOverride == nullptr ?
				GData()->GetGlobalData()->HitEffectData : _creatureCommonInfo->hitEffectDataOverride;
			const FGsSchemaSoundResData* soundInfo = _creatureCommonInfo->hitSoundDataOverride.GetRow() ?
				_creatureCommonInfo->hitSoundDataOverride.GetRow() : GData()->GetGlobalData()->HitSoundData.GetRow();

			const FGsSchemaCreatureCommonInfo* creatureInfo = GetCommonInfoTable();

			hitComp->OverrideData(effectInfo, soundInfo, 
				creatureInfo->hitEffectParticleSizeRate, creatureInfo->hitEffectModelRadius);
		}
	}

}

void UGsGameObjectNonPlayerBase::InitializeUI()
{
	InvalidateQuestMark();
	InvalidateMiniMap();
}

void UGsGameObjectNonPlayerBase::InvalidateQuestMark()
{
	bool questTargetActive = false;
	FGsQuestTargetManagement* questTargetManagement = GSQuest()->GetQuestTargetManagement();
	if (questTargetManagement == nullptr || _npcData == nullptr)
	{
		return;
	}

	// 퀘스트 마크 처리
	bool isSimpleMark = true;
	int tblId = GetNpcTblId();
	QuestType outQuestType = QuestType::MAX;
	// 몬스터면
	if (_npcData->iffFactionType == IffFactionType::MONSTER)
	{
		questTargetActive = questTargetManagement->IsQuestMonster(tblId, outQuestType);
	}
	// npc 이면
	else
	{
		// 퀘스트 마크 처리
		FGsGameObjectData* objData = GetData();
		if (objData != nullptr)
		{
			// obj 생성뒤에 호출 필요
			if (questTargetManagement->IsQuestNpc(tblId))
			{
				questTargetActive = true;
			}

		}
	}

	SetQuestMarkActive(questTargetActive, static_cast<int>(outQuestType));
}

void UGsGameObjectNonPlayerBase::InvalidateMiniMap()
{
	FGsQuestTargetManagement* questTargetManagement = GSQuest()->GetQuestTargetManagement();
	if (questTargetManagement == nullptr || _npcData == nullptr)
	{
		return;
	}

	// 퀘스트 마크 처리
	int tblId = GetNpcTblId();
	// 몬스터면
	if (_npcData->iffFactionType == IffFactionType::MONSTER)
	{
		// 미니맵 아이콘 처리
		EGsMapIconType monsterType = EGsMapIconType::None;
		if (false == IsTreasureMonster())
		{
			monsterType = FGsMinimapFunc::GetMapIconTypeByCreatureRankType(_npcData->grade, tblId);
		}

		if (UGsMinimapIconHandler* minimapIconHandler = GSMinimap()->GetIconHandler())
		{
			bool isShowInMinimap = minimapIconHandler->CanMakeMonsterIcon(monsterType);
			if (isShowInMinimap == true)
			{
				FGsGameObjectMessageParamMapIcon paramIcon(this, monsterType);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_SPAWN_ICON, &paramIcon);
			}
		}
	}
	// npc 이면
	else
	{
		// 퀘스트 마크 처리
		FGsGameObjectData* objData = GetData();
		if (objData != nullptr)
		{
			// obj 생성뒤에 호출 필요
			if (questTargetManagement->IsQuestNpc(tblId))
			{
				// 미니맵 quest icon 출력				
				FGsGameObjectMessageParamMapIcon paramIcon(this, EGsMapIconType::MainQuest);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_SPAWN_ICON, &paramIcon);
			}
		}
	}
}

TSharedPtr<FGsTargetHandlerBase> UGsGameObjectNonPlayerBase::CreateTarget()
{
	return MakeShareable(new FGsTargetHandlerNonPlayer());
}


FVector UGsGameObjectNonPlayerBase::GetLootTargetPoint(UGsGameObjectBase* LootTarget) const
{
	if (LootTarget)
	{
		float fRadius = 1.f;
		if (FGsGameObjectData* data = GetData())
		{
			fRadius = data->GetScaledCapsuleRadius();
		}

		FVector dir = (LootTarget->GetLocation() - GetLocation()).GetSafeNormal();
		return dir * fRadius + GetLocation();
	}

	return GetLocation();
}

void UGsGameObjectNonPlayerBase::LookAt(const FRotator& LookRotator, bool IsSmoothRotation /*= true*/)
{
	if (IsSmoothRotation)
	{
		// 현재 GsCharacterBase 클래스 타입만 기능 제공
		if (AGsCharacterBase* characterActor = GetCharacter())
		{
			characterActor->SetSmoothRotaion(LookRotator);
		}
		else if (GetActorState()== ActorState::LOADING)
		{
			_ActorTM.Rotation = LookRotator;
		}

		if (AActor* actor = GetActor())
		{
			GetActor()->SetActorRotation(LookRotator);
		}
		else if(GetActorState() == ActorState::LOADING)
		{
			_ActorTM.Rotation = LookRotator;
		}
	}
	else
	{
		if (AActor* actor = GetActor())
		{
			GetActor()->SetActorRotation(LookRotator);
		}
		else if (GetActorState() == ActorState::LOADING)
		{
			_ActorTM.Rotation = LookRotator;
		}
	}
}

void UGsGameObjectNonPlayerBase::OnCheckIsTargetedByLocalPlayer(const struct IGsMessageParam* inParam)
{
	if (_targetHandler)
	{
		_isTargetedByLocalPlayer = (_targetHandler->GetTarget() == this);
	}
	else
	{
		_isTargetedByLocalPlayer = false;
	}

#if WITH_EDITOR
	if (_isTargetedByLocalPlayer)
	{
		GSLOG(Log, TEXT("Targeted by local player : %s"), *GetActor()->GetActorLabel());
	}
#endif

	/*InvalidDebugWidget();*/
}

void UGsGameObjectNonPlayerBase::CallbackCognitionTargetEffectFinish(class UParticleSystemComponent* inParticleComp)
{
	InvalidateQuestMark();
}

bool UGsGameObjectNonPlayerBase::OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId,
	uint32 StrikerNotifyId,	HitResultType ResultType, DamageByType DamageType, DamageStatType DamageStat, HealByType HealType,
	int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage)
{
	// 상위 Return 결과에 따라 UI 반영 여부를 결정
	if (Super::OnHit(Striker, SubStriker, StriketSkillId, StrikerNotifyId, ResultType, DamageType, DamageStat,
		HealType, Damage, PassivityOwner, IsNoDamage))
	{
		if (_actor)
		{
			if (UGsUIBillboardCreature* widget = _actor->GetWidget())
			{
				widget->OnHit(Striker);
			}
		}
	}

	if (nullptr != Striker)
	{
		// 현재는 배틀 / 평화 상태 처리가 서버와 작업이 안되어있다
		// NonPlayer는 타겟이 설정된 타이밍에 배틀 모드 전환
		if (FGsTargetHandlerBase* target = GetTargetHandler())
		{
			target->SetTarget(Striker);
		}
	}

	return true;
}

void UGsGameObjectNonPlayerBase::OnDie()
{
	Super::OnDie();

	HideTalkBalloon();

	// 죽을때 마크 끄자
	SetQuestMarkActive(false);

	/*InvalidDebugWidget();*/
}

void UGsGameObjectNonPlayerBase::UpdateTargetMark(bool inVisibleTargetMaker, bool inIsFirst)
{
	if (_npcBalance)
	{
		Super::UpdateTargetMark(inVisibleTargetMaker, inIsFirst);

		if (AGsCharacterBase* characterActor = GetCharacter())
		{
			if (false == inVisibleTargetMaker)
			{
				characterActor->SetVisibleTargetMaker(false, nullptr);
			}
			else
			{
				UGsGlobalConstant* globalConstant = GData()->GetGlobalData();
				UMaterialInterface* targetMarkerMaterial;
				// interaction target(interaction possible)
				// https://jira.com2us.com/jira/browse/CHR-19890
				if (_npcData->iffFactionType == IffFactionType::NEUTRAL ||
					_npcData->iffFactionType == IffFactionType::NPC_WAGON ||
					_npcData->iffFactionType == IffFactionType::NPC_ESCORT)
				{
					targetMarkerMaterial = globalConstant->_interactionMoveTargetMarkerDecal;
				}
				// monster(targeting possible)
				else
				{
					if (inIsFirst)
					{
						targetMarkerMaterial = globalConstant->TargetMarkerDecal_first;
					}
					else
					{
						targetMarkerMaterial = globalConstant->TargetMarkerDecal_second;
					}
				}

				characterActor->SetVisibleTargetMaker(true, targetMarkerMaterial);
			}
		}
	}
}

void UGsGameObjectNonPlayerBase::OnTargetClear(const struct IGsMessageParam* inParam)
{
	Super::OnTargetClear(inParam);

	/*InvalidDebugWidget();*/
}

// 스폰 위치 저장
void UGsGameObjectNonPlayerBase::SaveSpawnPos()
{
	if (_actor == nullptr)
	{
		return;
	}

	_spawnPos = _actor->GetActorLocation();
}

void UGsGameObjectNonPlayerBase::WaitUntilDie(FName in_anim)
{
	if (nullptr == GetActor())
	{
		return;
	}

	AGsCharacterBase* character = Cast<AGsCharacterBase>(GetActor());
	if (nullptr == character)
	{
		return;
	}

	if (UGsAnimInstanceState* anim = character->GetAnim())
	{
		if (FGsGameObjectStateManager* fsm = GetBaseFSM())
		{
			float aniLen = anim->GetStateAnimationLength(fsm->GetBPStateName(), in_anim);

			// AI에서 이동 명령이 계속 들어오므로 임시로 타이머를 돌려 Despawn처리 진행
			if (UWorld* world = character->GetWorld())
			{
				if (_deathHandle.IsValid())
				{
					world->GetTimerManager().ClearTimer(_deathHandle);
				}

				world->GetTimerManager().SetTimer(_deathHandle,
					FTimerDelegate::CreateLambda([this, aniLen]()
						{
							GSAI()->RemoveMannerCheckTarget(GetGameId());
							//Boss 는 Despawn 처리 되지 않고 별도 프로세스탐									
							GSGameObject()->DespawnObject(this);
						}), aniLen, false);
			}
		}
	}
}

void UGsGameObjectNonPlayerBase::DeathHandleReset()
{
	if (_deathHandle.IsValid())
	{
		if (AActor* actor = GetActor())
		{
			if (UWorld* world = actor->GetWorld())
			{
				world->GetTimerManager().ClearTimer(_deathHandle);
			}
		}
	}
}

void UGsGameObjectNonPlayerBase::AsyncLoadClassComplete(class UObject* inRes)
{
	UClass* uclass = Cast<UClass>(inRes);
	AActor* actor = GsSpawn::BPClass(UObject::GetWorld(), uclass, _ActorTM.Location, _ActorTM.Rotation/*, uclass->*/);
	ActorSpawned(actor);

	_ActorState = ActorState::ACTIVE;
}

UGsWidgetComponentTalkBalloon* UGsGameObjectNonPlayerBase::SetTalkBalloonActive()
{
	AGsCharacterNonPlayerBase* npcActor = Cast<AGsCharacterNonPlayerBase>(_actor);
	if (nullptr == npcActor)
		return nullptr;

	UWidgetComponent* widgetComponent = npcActor->GetTalkBalloonWidgetComponent();
	UGsWidgetComponentTalkBalloon* talkBalloonComponent = Cast<UGsWidgetComponentTalkBalloon>(widgetComponent);
	if (nullptr == talkBalloonComponent)
		return nullptr;

	if (_npcData)
	{
		if (_npcData->isPalyBalloon && _npcData->balloonData > 0)
		{
			const UGsTableTalkBalloonData* table = Cast<UGsTableTalkBalloonData>(FGsSchemaTalkBalloonData::GetStaticTable());
			if (nullptr == table)
				return nullptr;

			TArray<const FGsSchemaTalkBalloonData*> allRows;
			if (table->GetAllRows(allRows))
			{
				if (allRows.IsValidIndex(_npcData->balloonData - 1))
				{
					talkBalloonComponent->SetData(allRows[_npcData->balloonData - 1]);
				}
			}
		}
	}

	bool isActive = talkBalloonComponent->IsAvailable();
	widgetComponent->SetActive(isActive);
	widgetComponent->SetVisibility(isActive);
	npcActor->SetUseTalkBalloon(isActive);

	return talkBalloonComponent;
}

void UGsGameObjectNonPlayerBase::HideTalkBalloon()
{
	if (nullptr == _actor)
	{
		return;
	}

	if (AGsCharacterNonPlayerBase* npcActor = Cast<AGsCharacterNonPlayerBase>(_actor))
	{
		if (UGsUIBillboardTalkBalloon* talkBalloon = npcActor->GetTalkBalloon())
		{
			talkBalloon->SetIsDead(true);
		}
	}
}

void UGsGameObjectNonPlayerBase::Print(UWorld* inWorld)
{
	FString log;

#if WITH_EDITOR
	log = FString::Printf(TEXT("label : %s\n"), *GetActor()->GetActorLabel());
#endif

	log += FString::Printf(TEXT("game id : %lld\n"), GetGameId());
	log += FString::Printf(TEXT("template id  : %d\tname : %s\n"), _npcData->id, *_npcData->nameText.ToString());

	GSLOG(Log, TEXT("%s"), *log);

	DrawDebugString(inWorld, GetActor()->GetActorLocation(), log, NULL, FColor::White, 300);
	DrawDebugSphere(inWorld, GetActor()->GetActorLocation(), 100.0f, 24, FColor::Red, false, 300);
}

bool UGsGameObjectNonPlayerBase::IsClientObject() const
{
	// GameId가 음수이면 클라이언트 스폰 오브젝트
	if (GetGameId() < 0)
	{
		return true;
	}
	return false;
}

// 퀘스트 타겟 액티브인지
void UGsGameObjectNonPlayerBase::SetQuestTargetActive(bool In_isActive, int In_Index)
{
	if (FGsQuestTargetManagement* questTargetManagement = GSQuest()->GetQuestTargetManagement())
	{
		QuestType outQuestType;
		int tblId = GetNpcTblId();
		if (questTargetManagement->IsQuestMonster(tblId, outQuestType))
		{
			In_Index = static_cast<int>(outQuestType);
		}
	}

	SetQuestMarkActive(In_isActive, In_Index);
}

// 퀘스트 마크 액티브인지
void UGsGameObjectNonPlayerBase::SetQuestMarkActive(bool In_isActive, int In_typeIndex)
{
	_isQuestTargetActive = In_isActive;

	if (nullptr == _actor ||
		_actor->IsPendingKill() == true)
	{
		return;
	}

	IGsActorQuestTargetInterface* target = Cast<IGsActorQuestTargetInterface>(_actor);
	if (nullptr == target)
	{
		return;
	}

	UStaticMeshComponent* meshComponent = target->GetQuestTargetMark();
	if (meshComponent == nullptr)
	{
		return;
	}

	if (_isQuestTargetActive == true)
	{
		meshComponent->SetVisibility(true);
		FSoftObjectPath path = GData()->GetGlobalData()->_questTargetMark;
		if (static_cast<int>(QuestType::REPEAT) == In_typeIndex)
		{
			path = GData()->GetGlobalData()->_repeatQuestTargetMark;
		}
		UStaticMesh* mesh = GResource()->LoadSync<UStaticMesh>(path);
		meshComponent->SetStaticMesh(mesh);
	}
	else
	{
		meshComponent->SetVisibility(false);
	}
}

void UGsGameObjectNonPlayerBase::SetCognitionTargetEffect()
{
	if (AGsCharacterBase* actor = GetCharacter())
	{
		UGsGlobalConstant* globalConstant = GData()->GetGlobalData();
		if (UGsEffectComponent* effectComponent = actor->GetEffectComponent())
		{
			if (false == effectComponent->IsCognitionTargetEffectActive())
			{
				UParticleSystemComponent* particleComponent = effectComponent->OnCognitionTargetEffectActive(
					globalConstant->_attackCognitionEffect, globalConstant->_attackCognitionEffectSocketName,
					globalConstant->_attackCognitionRemove, globalConstant->_attackCognitionOffsetLocation);
				if (nullptr == particleComponent)
				{
					return;
				}

				// 퀘스트 마크 표시가 있다면 잠시 꺼둔다
				if (_isQuestTargetActive)
				{
					SetQuestMarkActive(false, (int)QuestType::REPEAT);
					particleComponent->OnSystemFinished.AddUniqueDynamic(this, &UGsGameObjectNonPlayerBase::CallbackCognitionTargetEffectFinish);
				}
			}
		}
	}
}

bool UGsGameObjectNonPlayerBase::IsQuestTarget()
{
	FGsQuestTargetManagement* questTargetManagement = GSQuest()->GetQuestTargetManagement();
	if (questTargetManagement == nullptr)
	{
		return false;
	}

	if (_npcData == nullptr)
	{
		return false;
	}

	// 퀘스트 마크 처리
	QuestType outQuestType;
	int tblId = GetNpcTblId();
	// 몬스터면
	if (_npcData->iffFactionType == IffFactionType::MONSTER)
	{
		return 	questTargetManagement->IsQuestMonster(tblId, outQuestType);
	}

	return false;
}

bool UGsGameObjectNonPlayerBase::IsSkipTerrainCheck() const
{
	if (FGsGameObjectDataCreature* castData = GetCastData<FGsGameObjectDataCreature>())
	{
		return castData->IsSkipTerrainCheck();
	}
	return Super::IsSkipTerrainCheck();
}

void UGsGameObjectNonPlayerBase::SetSightRadius(float inRange)
{
	_sightRadius = inRange;

	/*InitNpcDebugWidget();*/
}

FVector UGsGameObjectNonPlayerBase::GetLocation() const
{
	if (GetActor())
	{
		return GetActor()->GetActorLocation();
	}
	else if (GetActorState() == ActorState::LOADING)
	{
		return _ActorTM.Location;
	}
	return FVector::ZeroVector;
}

FVector2D UGsGameObjectNonPlayerBase::GetLocation2D() const
{
	if (GetActor())
	{
		return FVector2D(GetActor()->GetActorLocation());
	}
	else if (GetActorState() == ActorState::LOADING)
	{
		return FVector2D(_ActorTM.Location);
	}
	return FVector2D::ZeroVector;
}

FRotator UGsGameObjectNonPlayerBase::GetRotation() const
{
	if (GetActor())
	{
		return GetActor()->GetActorRotation();
	}
	else if (GetActorState() == ActorState::LOADING)
	{
		return _ActorTM.Rotation;
	}
	return FRotator::ZeroRotator;
}

FVector UGsGameObjectNonPlayerBase::GetScale() const
{
	if (GetActor())
	{
		return GetActor()->GetActorScale();
	}
	else if (GetActorState() == ActorState::LOADING)
	{
		return _ActorTM.Scale;
	}
	return FVector::OneVector;
}

// npc tbl id 가져오기
int UGsGameObjectNonPlayerBase::GetNpcTblId()
{
	int resVal = 0;
	if (const FGsSchemaNpcData* npcTable = GetNpcData())
	{
		resVal = npcTable->id;
	}
	return resVal;
}

//void UGsGameObjectNonPlayerBase::DrawDebugSightRadius()
//{
//	if (false == _isTargetedByLocalPlayer)
//	{
//		return;
//	}
//
//	if (false == UGsCheatManager::_isShowNpcTargetSightRadius)
//	{
//		return;
//	}
//
//	if (_npcActor.IsValid())
//	{
//		//타겟 NPC인가?
//		_npcActor->DrawSightRadius(_sightRadius);
//	}
//}
//
//void UGsGameObjectNonPlayerBase::InitNpcDebugWidget()
//{
//	AGsCharacterNonPlayer* npc = Cast<AGsCharacterNonPlayer>(_actor);
//	if (npc)
//	{
//		npc->InitNpcDebugWidget(GetData(), _sightRadius);
//		_npcActor = npc;
//	}
//}

//void UGsGameObjectNonPlayerBase::InvalidDebugWidget()
//{
//	if (false == _isTargetedByLocalPlayer)
//	{
//		if (_npcActor.IsValid())
//		{
//			_npcActor->ShowNpcDebugWidget(false);
//		}
//
//		return;
//	}
//
//	if (false == UGsCheatManager::_isShowNpcTargetInfo)
//	{
//		if (_npcActor.IsValid())
//		{
//			_npcActor->ShowNpcDebugWidget(false);
//		}
//
//		return;
//	}
//
//	if (_npcActor.IsValid())
//	{
//		_npcActor->ShowNpcDebugWidget(true);
//	}
//}

