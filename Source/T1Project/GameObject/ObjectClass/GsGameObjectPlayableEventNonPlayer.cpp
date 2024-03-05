#include "GsGameObjectPlayableEventNonPlayer.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectProjectile.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Damage/GsDamageHandlerBase.h"
#include "GameObject/Skill/GsSkillHandlerPlayableEventNonPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"

#include "ActorEx/GsCharacterBase.h"
#include "ActorEx/GsCharacterNonPlayer.h"
#include "ActorEx/GsActorPlayableEventMonsterSpawn.h"

#include "ActorComponentEx/GsPlayableEventNonPlayerComponent.h"
#include "ActorComponentEx/GsHitEffectComponent.h"
#include "ActorComponentEx/GsEffectComponent.h"
#include "ActorComponentEx/GsWidgetComponentPooling.h"

#include "Animation/GsAnimInstanceState.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"

#include "GameFlow/Stage/GsStageMode.h"

#include "GameFlow/GsGameFlowLobby.h"
#include "GameFlow/GameContents/ContentsLobby/GsContentsManagerLobby.h"
#include "GameFlow/GameContents/ContentsLobby/Hud/GsStateLobbyHud.h"

#include "GameFlow/GameContents/ContentsLobby/Hud/PlayableEvent/GsPlayableEventHandler.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"

#include "Runtime/DataCenter/Public/ActorComponentEx/EGsMeshShaderEffect.h"

#include "UMG/Public/Components/WidgetComponent.h"

#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/GameFramework/Controller.h"
#include "Engine/Classes/GameFramework/PawnMovementComponent.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/Animation/AnimMontage.h"
#include "Engine/Classes/Materials/MaterialInterface.h"

void UGsGameObjectPlayableEventNonPlayer::InitializeActor(AActor* Owner)
{
	_actor = Cast<AGsCharacterBase>(Owner);


	AGsCharacterBase* characterActor = GetCharacter();
	if (characterActor == nullptr)
	{
		return;
	}

	UGsPlayableEventNonPlayerComponent* playableEventNonPlayerComponent =
		characterActor->FindComponentByClass<UGsPlayableEventNonPlayerComponent>();

	int tblId = 0;
	if (playableEventNonPlayerComponent != nullptr)
	{
		tblId = playableEventNonPlayerComponent->GetTableId();
	}

	_npcData = UGsTableUtil::FindRow<FGsSchemaNpcData>(tblId);
	if (_npcData)
	{
		_creatureCommonInfo = _npcData->commonInfoId.GetRow();
		_npcBalance = _npcData->balanceId.GetRow();
	}


	UGsGameObjectBase::InitializeActor(Owner);

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

	if (AGsCharacterBase* actorCharacter = GetCharacter())
	{
		if (UGsEffectComponent* effComp = actorCharacter->GetEffectComponent())
		{
			effComp->FindAttachTarget();
		}
		actorCharacter->InitMeshShaderComponent(GetCommonInfoTable());

		// 네임 플레이트 hide
		if (UWidgetComponent* nameWidgetComponent = actorCharacter->GetNameWidgetComponent())
		{
			nameWidgetComponent->SetActive(false);
			nameWidgetComponent->SetVisibility(false);
		}

		InitializeHitEffect();
	}

	AGsCharacterNonPlayer* npc = Cast<AGsCharacterNonPlayer>(_actor);
	if (nullptr == npc)
	{
		return;
	}
	npc->SetVisibleTargetMaker(false, nullptr);

	_interactionBlockCollider = npc->GetInteractionBlockCollider();
	if (_interactionBlockCollider != nullptr)
	{
		_interactionBlockCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	UGsDataContainManager* dataContainerManager = GData();
	if (nullptr == dataContainerManager)
	{
		return;
	}
	const UGsGlobalConstant* globalConstant = dataContainerManager->GetGlobalData();

	if (nullptr == globalConstant)
	{
		return;
	}

	npc->InitMeshShaderComponent(GetCommonInfoTable());

	_hitCount = 0;
	_deathHitCount = globalConstant->_monsterDeathHitCount;
}

void UGsGameObjectPlayableEventNonPlayer::Initialize(const struct FGsSpawnParam* SpawnParam)
{
	UGsGameObjectBase::Initialize(SpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectNonPlayer::GetObjectType());

	if (FGsGameObjectDataCreature* creatureData = GetCastData<FGsGameObjectDataCreature>())
	{
		CreateCreatureStat();
	}


	//// interaction 
	//_interactionMessageParam._objectType = GetObjectType();
	//_interactionMessageParam._targetGameID = GetGameId();

	CreateSubClass();
}

// 필요한 핸들러만 쓰자
void UGsGameObjectPlayableEventNonPlayer::CreateSubClass()
{
	UGsGameObjectBase::CreateSubClass();

	// gameobectCreatrue에서 하던거
	AppendInitialize(CreateTarget());
	AppendInitialize(CreateDamageHandler());
	

	// UGsGameObjectNonPlayerBase 에서 하던거
	AppendInitialize(CreateSkill());
	InitializeMovement();	
}

void UGsGameObjectPlayableEventNonPlayer::Update(float Delta)
{
	// non player 클래스 update는 인터랙션 갱신때문에 그윗단을 갱신
	UGsGameObjectNonPlayerBase::Update(Delta);
}

TSharedPtr<FGsSkillHandlerBase> UGsGameObjectPlayableEventNonPlayer::CreateSkill()
{
	// 스킬 핸들러 생성
	return MakeShareable(new FGsSkillHandlerPlayableEventNonPlayer());
}
void UGsGameObjectPlayableEventNonPlayer::InitializeMovement()
{
	// fsm 사용 안해서 ,movement handler도 사용 못함...
	if (const FGsSchemaCreatureCommonInfo* commonInfoTable = GetCommonInfoTable())
	{
		if (AGsCharacterBase* actorCharacter = GetCharacter())
		{
			if (UCharacterMovementComponent* moveComponent = Cast<UCharacterMovementComponent>(actorCharacter->GetMovementComponent()))
			{
				moveComponent->MaxWalkSpeed = commonInfoTable->moveSpeed;
			}
		}
		
	}
}

bool UGsGameObjectPlayableEventNonPlayer::OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StrikerSkillId, uint32 StrikerNotifyId,
	HitResultType ResultType, DamageByType DamageType , DamageStatType DamageStat, HealByType HealType, int Damage ,
	PassivityOwnerType PassivityOwner, bool IsNoDamage)
{
	//Super::OnHit(Striker, SubStriker, StrikerSkillId, StrikerNotifyId, ResultType, DamageType, Damage, PassivityOwner);

	AGsCharacterBase* actorCharacter = GetCharacter();
	if (nullptr == actorCharacter)
	{
		return false;
	}

	if (Striker != nullptr)
	{
		const FGsSchemaSkillSet* skillData =
			UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(StrikerSkillId);

		// 때린넘이 플레이어인가
		bool isPlayerAttacker = Striker->IsObjectType(EGsGameObjectType::Player);
		if (isPlayerAttacker == true)
		{
			// 공격자의 이펙트 제거
			if (skillData != nullptr)
			{
				if (AGsCharacterBase* strikerActor = Striker->GetCharacter())
				{
					UGsAnimInstanceState* anim = strikerActor->GetAnim();
					if (skillData->targetFilterInfo.targetCount == 1)
					{
						anim->RemoveAnimNotifyParticle(EPlayParticleManagedType::TargetHitSync_OneTarget);
					}
					anim->RemoveAnimNotifyParticle(EPlayParticleManagedType::TargetHitSync_Immediate);
				}
			}
		}

		const FGsSchemaSkillNotifySet* skillNotifyData = UGsTableUtil::FindRowById<
			UGsTableSkillNotifySet, FGsSchemaSkillNotifySet>(StrikerNotifyId);
		const FGsSchemaPassivitySet* passivityData = nullptr;
		if (PassivityOwner == PassivityOwnerType::FAIRY)
		{
			passivityData = UGsTableUtil::FindRowById<UGsTablePassivitySet, FGsSchemaPassivitySet>(StrikerSkillId);
		}

		const FGsSchemaProjectileSet* projectileData = nullptr;
		if (nullptr != SubStriker && SubStriker->IsObjectType(EGsGameObjectType::Projectile))
		{
			UGsGameObjectProjectile* projectile = SubStriker->CastGameObject<UGsGameObjectProjectile>();
			projectileData = projectile->GetProjectileData();
		}

		EGsStateBase state = EGsStateBase::Idle;	
		actorCharacter->OnHit(ResultType, state, GetCommonInfoTable());
		actorCharacter->OnHitEffect(Striker->GetLocation(), ResultType,
			skillData, skillNotifyData, passivityData, projectileData, nullptr, nullptr,
			isPlayerAttacker);
	}

	// 한대 맞으면 사망

	++_hitCount;

	// hit count over death count
	if (_hitCount >= _deathHitCount)
	{
		// die
		if (AGsCharacterBase* characterActor = GetCharacter())
		{

			UGsPlayableEventNonPlayerComponent* playableEventNonPlayerComponent =
				characterActor->FindComponentByClass<UGsPlayableEventNonPlayerComponent>();

			int tblId = 0;
			if (playableEventNonPlayerComponent != nullptr)
			{
				playableEventNonPlayerComponent->SetAlive(false);
				tblId = playableEventNonPlayerComponent->GetTableId();
			}



			//https://jira.com2us.com/jira/browse/CHR-15711
			//ai unpossess
			if (AController* controller = characterActor->GetController())
			{
				controller->UnPossess();
			}

			if (FGsSkillHandlerBase* skillhandler = GetSkillHandler())
			{
				// 스킬 종료
				skillhandler->StopSkill(0);

				FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
				if (const FGsSchemaSkillCommon* commonData = GSkill()->GetNpcCommonActionData(this, CommonActionType::DYING))
				{
					skillRunner->StartRunner(this, commonData);
				}

				if (UAnimMontage* resAni = skillRunner->GetAni())
				{
					float aniLen = characterActor->PlayAnimMontage(resAni);

					// https://jira.com2us.com/jira/browse/CHR-15711
					float calcedAniLen = aniLen * 0.8f;

					// 애니 끝나면 디스폰
					if (UWorld* world = characterActor->GetWorld())
					{
						if (_deathHandle.IsValid())
						{
							world->GetTimerManager().ClearTimer(_deathHandle);
						}

						world->GetTimerManager().SetTimer(_deathHandle,
							FTimerDelegate::CreateLambda([this]()
								{
									OnDeath();

								}), calcedAniLen, false);
					}
				}
			}

			// 공격자 타겟 대상에서 제거
			if (Striker)
			{
				FGsTargetHandlerBase* targethandler = Striker->GetTargetHandler();
				targethandler->ClearTarget(this);
			}

			AddQuestKillCount(tblId);

			// 충돌체 꺼줌(pawn)
			ChangeCollisionPresetToPawn();
		}
	}
	// show hit reaction
	else
	{
		
	}
	return true;
}
void UGsGameObjectPlayableEventNonPlayer::OnDeath()
{
	AGsCharacterBase* characterActor = GetCharacter();

	if (characterActor == nullptr)
	{
		return;
	}

	//https://jira.com2us.com/jira/browse/CHR-15945
	// effect 제거	
	characterActor->StopCurrentSkillEffectNSound();

	if (UGsAnimInstanceState* anim = GetAnimInstance())
	{				
		if (FGsSkillHandlerBase* skillhandler = GetSkillHandler())
		{
			FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();				
			if (UAnimMontage* resAni = skillRunner->GetAni())
			{
				anim->Montage_Pause(resAni);
			}
		}					
	}	

	UGsDataContainManager* dataContainerManager = GData();
	if (nullptr == dataContainerManager)
	{
		return;
	}
	const UGsGlobalConstant* globalConstant = dataContainerManager->GetGlobalData();

	if (nullptr == globalConstant)
	{
		return;
	}

	float despawnSecond =
		globalConstant->_playableEventMonsterDespawnSecond;

	if (UWorld* world = characterActor->GetWorld())
	{
		if (_despawnHandle.IsValid())
		{
			world->GetTimerManager().ClearTimer(_despawnHandle);
		}

		world->GetTimerManager().SetTimer(_despawnHandle,
			FTimerDelegate::CreateLambda([this]()
				{
					OnDespawn();

				}), despawnSecond, false);
	}

	if (const FGsSchemaCreatureCommonInfo* commonInfoTable = GetCommonInfoTable())
	{
		characterActor->StartMeshShader(EGsMeshShaderEffect::Dissolve, commonInfoTable);
	}


}
void UGsGameObjectPlayableEventNonPlayer::OnDespawn()
{
	int spawnId = -1;
	if (AGsCharacterBase* characterActor = GetCharacter())
	{

		UGsPlayableEventNonPlayerComponent* playableEventNonPlayerComponent =
			characterActor->FindComponentByClass<UGsPlayableEventNonPlayerComponent>();
		
		if (playableEventNonPlayerComponent != nullptr)
		{
			spawnId = playableEventNonPlayerComponent->GetSpawnId();
		}
	}
	LSLobbyPlayableEvent()->AddDespawnList(spawnId);

	RequestRespawn();
}

// 퀘스트 킬카우트 추가 처리
void UGsGameObjectPlayableEventNonPlayer::AddQuestKillCount(int In_tblId)
{
	// 퀘스트 카운트 처리
	if (FGsPlayableEventHandler* handler = GetPlayableEventHandler())
	{
		handler->OnAddKillCount(In_tblId);
	}
	
}
void UGsGameObjectPlayableEventNonPlayer::RequestRespawn()
{
	// check finish quest
	FGsPlayableEventHandler* handler = GetPlayableEventHandler();
	if (handler ==  nullptr ||
		nullptr == _spawnActor)
	{
		return;
	}
	// finished
	if (false == handler->GetIsProgress())
	{
		return;
	}

	_spawnActor->OnRequestRespawn();
}

FGsPlayableEventHandler* UGsGameObjectPlayableEventNonPlayer::GetPlayableEventHandler()
{
	if (nullptr == GMode())
	{
		return nullptr;
	}
	FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow();
	if (nullptr == lobbyFlow)
	{
		return nullptr;
	}
	FGsContentsManagerLobby* contents = lobbyFlow->GetContentsManagerLobby();
	if (nullptr == contents)
	{
		return nullptr;
	}
	FGsStateLobbyHud* hud =
		contents->GetContentsStateObject<FGsStateLobbyHud>(FGsContentsMode::InLobby::HUD);

	if (nullptr == hud)
	{
		return nullptr;
	}

	FGsPlayableEventHandler* handler = hud->GetPlayableEventHandler();
	return handler;
}
void UGsGameObjectPlayableEventNonPlayer::DespawnHadleReset()
{
	if (_despawnHandle.IsValid())
	{
		if (AActor* actor = GetActor())
		{
			if (UWorld* world = actor->GetWorld())
			{
				world->GetTimerManager().ClearTimer(_despawnHandle);
			}
		}
	}
}
void UGsGameObjectPlayableEventNonPlayer::ActorDespawn()
{
	Super::ActorDespawn();
	DespawnHadleReset();
}

void UGsGameObjectPlayableEventNonPlayer::RemoveSubClass()
{
	DespawnHadleReset();

	Super::RemoveSubClass();
}

void UGsGameObjectPlayableEventNonPlayer::MakeSpawnEffect()
{
	UGsDataContainManager* dataContainerManager = GData();
	if (nullptr == dataContainerManager)
	{
		return;
	}
	const UGsGlobalConstant* globalConstant = dataContainerManager->GetGlobalData();

	if (nullptr == globalConstant)
	{
		return;
	}

	AGsCharacterBase* characterActor = GetCharacter();
	if (characterActor == nullptr)
	{
		return;
	}

	if (UGsEffectComponent* effectComp = characterActor->GetEffectComponent())
	{
		if (UPawnMovementComponent* moveComp = characterActor->GetMovementComponent())
		{
			effectComp->OnEffectPos(globalConstant->_playableEventMonsterSpawnPath,
				moveComp->GetActorFeetLocation(),
				FVector::OneVector);
		}
	}
}