// Fill out your copyright notice in the Description page of Project Settings.

#include "GsGameObjectPlayer.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "ActorComponentEx/GsHitEffectComponent.h"
#include "ActorComponentEx/GsEffectComponent.h"
#include "ActorComponentEx/EGsMeshShaderEffect.h"

#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"

#include "Animation/GsAnimInstanceState.h"

#include "ActorEx/GsCharacterPlayer.h"
#include "ActorEx/GsActorFieldFairy.h"
#include "ActorEx/GsWeaponActor.h"

#include "GameObject/Define/EGsPartsType.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Parts/GsPartsHandlerPlayer.h"
#include "GameObject/Fairy/GsFairyHandlerPlayer.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "Gameobject/Customize/GsCustomizeHandler.h"
#include "GameObject/Customize/GsCustomizeFunc.h"
#include "GameObject/Interaction/GsInteractionHandlerBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/UseItemEffect/GsUseItemEffectHandler.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerPlayer.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/Boss/GsBossHandler.h"
#include "GsGameObjectNonPlayer.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsFairyManager.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "DataSchema/GameObject/GsSchemaPlayerShape.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/GameObject/GsSchemaPlayerWeaponDetailInfo.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/Fairy/GsSchemaVehicleConfig.h"

#include "UI/UIContent/Billboard/GsUIBillboardPlayer.h"
#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"

#include "Util/GsTableUtil.h"
#include "Util/GsGameObjectUtil.h"
#include "UTIL/GsLevelUtil.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

EGsGameObjectType			UGsGameObjectPlayer::GetObjectType() const { return EGsGameObjectType::Player; }

FGsInteractionHandlerBase* UGsGameObjectPlayer::GetInteractionHandler() { return GetGameObjectHandler<FGsInteractionHandlerBase>(GameObjectHandlerType::INTERACTION); }
FGsFairyHandlerPlayer* UGsGameObjectPlayer::GetFairyHandler() { return GetGameObjectHandler<FGsFairyHandlerPlayer>(GameObjectHandlerType::FAIRY); }

AGsCharacterPlayer* UGsGameObjectPlayer::GetPlayerCharacter() const
{
	if (GetActor())
	{
		return Cast<AGsCharacterPlayer>(GetActor());
	}
	return nullptr;
}

const FGsSchemaCreatureCommonInfo* UGsGameObjectPlayer::GetCommonInfoTable() const
{
	if (_pcData)
	{
		return UGsTableUtil::FindRowById<UGsTableCreatureCommonInfo,
			FGsSchemaCreatureCommonInfo>(_pcData->commonInfoId);
	}

	return nullptr;
}

UGsGameObjectPlayer::UGsGameObjectPlayer() : UGsGameObjectCreature()
{
}
UGsGameObjectPlayer::~UGsGameObjectPlayer()
{
}

void UGsGameObjectPlayer::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectPlayer::GetObjectType());
	_pcData = UGsTableUtil::FindRow<FGsSchemaUserData>(_gameObjectData->GetTableId());
    check(_pcData);

	_isVehicleEffect = false;
}

// 이 함수를 타면 내부에서 Compare 체크를 하여, 변경된 Parts 만 Set 처리
void UGsGameObjectPlayer::OnUserBasicDataChange(const struct UserLookInfo& Data)
{
	if (GetPartsHandler() == nullptr)
	{
		return;
	}

	GetPartsHandler()->InvalidatePartsData(&Data);
}

// 이 함수를 타면 별도 compare 하지 않고, userlookInfo 데이터 기준으로 모두 Set 처리
void UGsGameObjectPlayer::SetUserEquipData(const struct UserLookInfo& Data)
{
	if (GetPartsHandler() == nullptr)
	{
		return;
	}

	GetPartsHandler()->SetPartsData(&Data);
}


bool UGsGameObjectPlayer::IsTransPolymorph()
{
	if (AGsCharacterPlayer* characterActor = GetPlayerCharacter())
	{
		return  GetPlayerCharacter()->IsMeshChanged();

	}

	return false;
}

bool UGsGameObjectPlayer::IsVehicleMode()
{
	if (FGsAbnormalityHandlerBase* abnormalityHandler = GetAbnormalityHandler())
	{
		return abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::UTIL_MOUNT_VEHICLE);
	}
	return false;
}

void UGsGameObjectPlayer::Update(float Delta)
{
	Super::Update(Delta);

	//if (UpperFsm)	{ UpperFsm->Update(this, Delta); }
}

void UGsGameObjectPlayer::ActorSpawned(AActor* Spawn)
{
	Super::ActorSpawned(Spawn);
}

void UGsGameObjectPlayer::ActorDespawn()
{
	if (AGsCharacterPlayer* character = Cast<AGsCharacterPlayer>(GetCharacter()))
	{
		character->SetEnableDeathActor(false);
		// Actor가 소멸된 상태면 페어리도 어떤상황이라도 Destory되어야 한다고 생각합니다.
		if (FGsFairyHandlerPlayer* fairyHandler = GetFairyHandler())
		{
			if (AGsActorFieldFairy* fairyActor = fairyHandler->GetFairyActor())
			{
				fairyActor->Destroy();
			}
		}
//  		if (FGsFairyHandlerPlayer* fairyHandler = GetFairyHandler())
//  		{
//  			fairyHandler->UpdateEquipFairy(INVALID_FAIRY_ID, true); //normal despawn 시 페어리도 삭제해준다.
//  		}
	}

	Super::ActorDespawn();
}

void UGsGameObjectPlayer::CreateSubClass()
{
	Super::CreateSubClass();

	AppendInitialize(CreateParts());
	AppendInitialize(CreateCustomize());
	AppendInitialize(CreateInteractionHandler());
	AppendInitialize(CreateFairyHandler());
}

void UGsGameObjectPlayer::LoadAnimInstance(const FString& ClassPath)
{
	if (AGsCharacterPlayer* character = GetPlayerCharacter())
	{
		EGsStateBase prevStateType = EGsStateBase::None;
		if (UGsAnimInstanceState* prevAnim = character->GetAnim())
		{
			// 마스터 포즈 버그인지 AnimCurve에서 Slave 컴퍼넌트들이 마져 처리가 안되는것 같다.
			// @see : void USkeletalMeshComponent::ApplyAnimationCurvesToComponent(const TMap<FName, float>* InMaterialParameterCurves, const TMap<FName, float>* InAnimationMorphCurves)
			// prevAnim->ResetCurves();
			prevAnim->UninitializeAnimation();
			prevStateType = prevAnim->CurrentStateType;
		}

		TSoftClassPtr<UObject> classPtr = TSoftClassPtr<UObject>(ClassPath);
		UClass* bpClass = classPtr.Get();
		if (nullptr == bpClass)
		{
			bpClass = classPtr.LoadSynchronous();
		}

		if (bpClass)
		{
			// AnimInstance Class 설정
			USkeletalMeshComponent* meshComponent = character->GetMesh();
			meshComponent->SetAnimInstanceClass(bpClass);

			if (UGsAnimInstanceState* anim = character->GetAnim())
			{
				if (const FGsSchemaCreatureCommonInfo* commoninfo = GetCommonInfoTable())
				{
					// 기본 속도 설정
					anim->InitMoveSpeedValue(commoninfo->walkSpeed, commoninfo->moveSpeed);
				}

				// 기존 상태 동기화
				anim->ChangeState((uint8)prevStateType);
			}
		}
	}
}

void UGsGameObjectPlayer::NetChangeWeaponStartAck(CreatureWeaponType WeaponType)
{
	if (WeaponType != CreatureWeaponType::HAND)
	{
		_fsm->ProcessEvent(EGsStateBase::ChangeWeapon);
	}

	if (AGsCharacterPlayer* playerActor = Cast<AGsCharacterPlayer>(GetCharacter()))
	{
		playerActor->SetWeaponTypeChangeHitPose(WeaponType);
	}
}

void UGsGameObjectPlayer::NetChangeWeaponFinishedAck(PD::SC::PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED_READ* Packet)
{
	_fsm->ProcessEvent(EGsStateBase::CommonActionEnd);

	// 스킬, 이동을 Stop
	if (FGsSkillHandlerBase* skill = GetSkillHandler())
	{
		skill->StopSkill();
	}

	if (FGsMovementHandlerBase* move = GetMovementHandler())
	{
		move->Stop();
	}
}


void UGsGameObjectPlayer::UpdateTargetMark(bool inVisibleTargetMaker, bool inIsFirst)
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
			UMaterialInterface* targetMarkerMaterial = (inIsFirst) ? globalConstant->TargetMarkerDecal_first :
				globalConstant->TargetMarkerDecal_second;

			characterActor->SetVisibleTargetMaker(true, targetMarkerMaterial);
		}
	}
}

void UGsGameObjectPlayer::OnTargetClear(const struct IGsMessageParam* inParam)
{
	Super::OnTargetClear(inParam);

	if (FGsInteractionHandlerBase* interactionHandler = GetInteractionHandler())
	{
		const FGsGameObjectMessageParam* castParam = inParam->Cast<const FGsGameObjectMessageParam>();
		if (castParam->_paramOwner == interactionHandler->GetInteractionTarget())
		{
			interactionHandler->ClearTarget();
		}
	}
}

void UGsGameObjectPlayer::OnDie()
{
	Super::OnDie();

	// 상태이상이 먼저 해제가 된다면 굳이 할필요 없음
	//OnRestorePolymorph();
}

void UGsGameObjectPlayer::OnRevive()
{
	Super::OnRevive();

	// Dissolve 처리 복원
	if (UGsMeshShaderControlComponent* meshShaderComponent = GetCharacter()->GetMeshShaderComponent())
	{
		meshShaderComponent->Stop();
		meshShaderComponent->Restore(EGsMeshShaderEffect::Dissolve);
	}

	_fsm->ProcessEvent(EGsStateBase::Revive);
}

void UGsGameObjectPlayer::OnWarp()
{
	if (FGsGameObjectStateManager* fsm = GetBaseFSM())
	{
		// 캐릭터가 살아있을 경우에만 워프 연출
		if (false == fsm->IsState(EGsStateBase::Dead))
		{
			_fsm->ProcessEvent(EGsStateBase::Warp);

			OnPlayerTeleportEffect(ETeleportEffectType::DEPART);

			// 페어리 워프 연출
			if (FGsFairyHandlerPlayer* fairyHandler = GetFairyHandler())
			{
				fairyHandler->ShowFairyWarpEffect();
			}
		}
	}
}

void UGsGameObjectPlayer::OnSpawnFairyEffect()
{
	if (FGsFairyHandlerPlayer* fairyHandler = GetFairyHandler())
	{
		fairyHandler->ShowFairySpawnEffect();
	}
}

void UGsGameObjectPlayer::OnPlayerTeleportEffect(ETeleportEffectType teleportEffectType, FName socket, FVector scale, FVector locOffset, FRotator rotOffset, USkeletalMeshComponent* mesh)
{
	if (FGsFairyHandlerPlayer* fairyHandler = GetFairyHandler())
	{
		TWeakPtr<FGsFairyData> fairyDataPtr = fairyHandler->GetFairyData();
		if (FGsFairyData* fairyData = fairyDataPtr.Pin().Get())
		{
			TArray<UParticleSystem*> pcTeleportParticleEffectList = fairyData->GetPcTeleportParticleEffectList();
			{
				if (AGsCharacterBase* character = GetCharacter())
				{
					if (UGsEffectComponent* effectComp = character->GetEffectComponent())
					{
						if ((uint8)teleportEffectType < pcTeleportParticleEffectList.Num())
						{
							effectComp->OnEffect(
								pcTeleportParticleEffectList[(uint8)teleportEffectType],
								true,
								EPSCPoolMethod::AutoRelease,
								EAttachLocation::SnapToTargetIncludingScale,
								locOffset,
								scale,
								socket,
								true);
						}
						else
						{
							GSLOG(Warning, TEXT("Teleport particle effect for player is not properly set in the table!!!"));
						}
					}
				}
			}
		}
	}
}

void UGsGameObjectPlayer::OnVehicleStartAnimation()
{
	if (const FGsSchemaVehicleConfig* vehicleConfigTable = GSFairy() ? GSFairy()->GetVehicleConfigTable() : nullptr)
	{
		// 발동 애니메이션 플레이
		if (UGsAnimInstanceState* anim = GetAnimInstance())
		{
			if (nullptr == anim->GetCurrentActiveMontage())
			{
				FGsGameObjectDataPlayer* playerData = GetCastData<FGsGameObjectDataPlayer>();
				const FSoftObjectPath& aniPath = playerData->GetCreatureGenderType() == CreatureGenderType::MALE ?
					vehicleConfigTable->vehicleMoveStart_M : vehicleConfigTable->vehicleMoveStart_F;

				if (UAnimMontage* playAniRes = GResource()->LoadSync<UAnimMontage>(aniPath))
				{
					anim->Montage_Play(playAniRes);
				}
			}
		}
	}
}

void UGsGameObjectPlayer::OnVehicleEffect(bool useTalk)
{
	if (FGsAbnormalityHandlerBase* abnormalityHandler = GetAbnormalityHandler())
	{
		if (false == abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::UTIL_MOUNT_VEHICLE))
		{
			return;
		}
	}

	// 이동 상태 체크
	if (FGsMovementHandlerBase* movementHandler = GetMovementHandler())
	{
		if (false == movementHandler->IsMoveState())
		{
			return;
		}
	}

	if (_isVehicleEffect)
	{
		return;
	}

	_isVehicleEffect = true;
	if (const FGsSchemaVehicleConfig* vehicleConfigTable = GSFairy() ? GSFairy()->GetVehicleConfigTable() : nullptr)
	{
		// 페어리 연출
		if (FGsFairyHandlerPlayer* fairyHandler = GetFairyHandler())
		{
			fairyHandler->OnVehicleEffect(vehicleConfigTable->vehicleFairyTargetLength,
				vehicleConfigTable->vehicleFairyPosition, useTalk);

			// 탈것 이동 이펙트 설정
			TWeakPtr<FGsFairyData> fairyData = fairyHandler->GetFairyData();
			if (const FGsSchemaFairyDetail* fairyDetailTable = fairyData.Pin()->GetFairyDetail())
			{
				if (AGsCharacterBase* ownertActor = GetCharacter())
				{
					// 전용 발자국 이펙트 등록
					if (fairyDetailTable->vehicleEffect.ETCParticlePath.IsValid())
					{
						TWeakObjectPtr<AGsCharacterBase> objThiz = ownertActor;
						FString particleBlueprintClassPath = fairyDetailTable->vehicleEffect.ETCParticlePath.ToString();
						GResource()->LoadAsync(particleBlueprintClassPath, FStreamableDelegateParam::CreateLambda([objThiz](UObject* inRes)->void
							{
								if (objThiz.IsValid())
								{
									objThiz->_customParticle = Cast<UParticleSystem>(inRes);
								}								
							}));

					}
					else
					{
						ownertActor->_customParticle = nullptr;
					}

					// 페어리 소환자에게 붙일 이펙트
					if (fairyDetailTable->vehicleEffect.OwnerParticlePath.IsValid())
					{
						if (UGsEffectComponent* effectComp = ownertActor->GetEffectComponent())
						{
							TWeakObjectPtr<AGsCharacterBase> objThiz = ownertActor;
							FString particleBlueprintClassPath = fairyDetailTable->vehicleEffect.OwnerParticlePath.ToString();
							GResource()->LoadAsync(particleBlueprintClassPath, FStreamableDelegateParam::CreateLambda([objThiz](UObject* inRes)->void
								{
									if (objThiz.IsValid() && objThiz->GetEffectComponent())
									{
										objThiz->GetEffectComponent()->OnVehicleEffectActive(Cast<UParticleSystem>(inRes));
									}									
								}));

						}
					}
				}
			}
		}
	}
}

void UGsGameObjectPlayer::OnRestoreVehicleEffect()
{
	if (FGsFairyHandlerPlayer* fairyHandler = GetFairyHandler())
	{
		fairyHandler->OnRestoreVehicleEffect();
	}

	if (AGsCharacterBase* ownertActor = GetCharacter())
	{
		if (UGsEffectComponent* effectComp = ownertActor->GetEffectComponent())
		{
			effectComp->OnVehicleEffectDeActive();
		}
	}

	_isVehicleEffect = false;
}

void UGsGameObjectPlayer::BindMessage()
{
	Super::BindMessage();

	MGameObject& gameObjectMsg = GMessage()->GetGameObject();
	AddMessageDelegate(gameObjectMsg.AddUObject(MessageGameObject::LOCAL_DEAD,
		this, &UGsGameObjectPlayer::OnHideContribution));

	MUserBasicInfo& userInfoMsg = GMessage()->GetUserBasicInfo();
	TPair<MessageUserInfo, FDelegateHandle> lookInfoHandler = userInfoMsg.AddUObject(MessageUserInfo::LOOK_INFO, this, &UGsGameObjectPlayer::OnLookInfoChanged);
	_userInfoMsgList.Add(lookInfoHandler);

	MUI& uiMsg = GMessage()->GetUI();

	{
		TPair<MessageBoss, FDelegateHandle> changeAggroHandle = GMessage()->GetBoss().AddUObject(MessageBoss::CHANGE_AGGRO, this, &UGsGameObjectPlayer::OnChangeAggroTarget);
		TPair<MessageBoss, FDelegateHandle> changeContributionHandle = GMessage()->GetBoss().AddUObject(MessageBoss::CHANGE_CONTRIBUTION, this, &UGsGameObjectPlayer::OnChangeContribution);
		TPair<MessageBoss, FDelegateHandle> hideAggroHandle = GMessage()->GetBoss().AddUObject(MessageBoss::HIDE_AGRRO, this, &UGsGameObjectPlayer::OnHideAggroTarget);
		TPair<MessageBoss, FDelegateHandle> hideContributionHandle = GMessage()->GetBoss().AddUObject(MessageBoss::HIDE_CONTRIBUTION, this, &UGsGameObjectPlayer::OnHideContribution);
		TPair<MessageBoss, FDelegateHandle> hideBossBillboardHandle = GMessage()->GetBoss().AddUObject(MessageBoss::HIDE_BOSS_BILLBOARD, this, &UGsGameObjectPlayer::OnHideFieldBossBillboard);

		_bossMsgList.Emplace(changeContributionHandle);
		_bossMsgList.Emplace(changeAggroHandle);
		_bossMsgList.Emplace(hideAggroHandle);
		_bossMsgList.Emplace(hideContributionHandle);
		_bossMsgList.Emplace(hideBossBillboardHandle);
	}

	{	// 2022/12/19 PKT - Title Update Billboard ( 침공전 / 침공전 아닐때 칭호를 바꿔 준다. )
		_uiMsgList.Emplace(
			GMessage()->GetUI().AddUObject(MessageUI::INVADE_MAPSTATE_UPDATE, this, &UGsGameObjectPlayer::OnTitleUpdateBillboard)
		);		
	}
	
}

void UGsGameObjectPlayer::FinalizeMessageDelegateList()
{
	//user message
	MUserBasicInfo& usrMsg = GMessage()->GetUserBasicInfo();
	for (TPair<MessageUserInfo, FDelegateHandle> handler : _userInfoMsgList)
	{
		usrMsg.Remove(handler);
	}
	_userInfoMsgList.Empty();

	//ui message
	MUI& uiMsg = GMessage()->GetUI();
	for (MsgUIHandle& handler : _uiMsgList)
	{
		uiMsg.Remove(handler);
	}
	_uiMsgList.Empty();

	MGameObject& gameObjectMsg = GMessage()->GetGameObject();
	for (MsgGameObjHandle& handler : _gameObjectMsgList)
	{
		gameObjectMsg.Remove(handler);
	}
	_gameObjectMsgList.Empty();

	MBoss& bossMsg = GMessage()->GetBoss();
	for (TPair<MessageBoss, FDelegateHandle> handler : _bossMsgList)
	{
		bossMsg.Remove(handler);
	}
	_bossMsgList.Empty();

	Super::FinalizeMessageDelegateList();
}

void UGsGameObjectPlayer::OnLookInfoChanged(uint64 InGameId)
{
	if (GetGameId() == InGameId)
	{
		if (AGsCharacterBase* character = GetCharacter())
		{
			UpdateBillboard(EGsBillboardUpdateType::NameColor);
		}
	}
}

void UGsGameObjectPlayer::OnChangeContribution(const IGsMessageParam* inParam)
{	
	FGsBossHandler* bossHandler = FGsBossHandler::GetInstance();
	if (nullptr == bossHandler)
	{
		return;
	}

	FGsFieldBossHandler* handler = bossHandler->GetFieldBossHandler();
	if (nullptr == handler)
	{
		return;
	}

	bool isShowContribution = handler->GetContributionUserId().Contains(GetGameId());
	FGsGameObjectDataPlayer* data = GetCastData<FGsGameObjectDataPlayer>();
	if (nullptr == data)
	{
		return;
	}

#if CONTRIBUTION_DEBUG	
#if WITH_EDITOR
	if (GetActor())
	{
		if (isShowContribution)
		{
			GSLOG(Log, TEXT("UGsGameObjectPlayer::OnChangeContribution - show - name : %s"), *GetActor()->GetActorLabel());
		}
		else
		{
			GSLOG(Log, TEXT("UGsGameObjectPlayer::OnChangeContribution - hide - name : %s"), *GetActor()->GetActorLabel());
		}
	}
#endif
#endif

	data->SetShowContribution(isShowContribution);
	UpdateBillboard(EGsBillboardUpdateType::Contribution);
}

void UGsGameObjectPlayer::OnHideContribution(const IGsMessageParam* inParam)
{
	if (FGsGameObjectDataPlayer* data = GetCastData<FGsGameObjectDataPlayer>())
	{
		data->SetShowContribution(false);
		UpdateBillboard(EGsBillboardUpdateType::Contribution);
	}
}

void UGsGameObjectPlayer::OnChangeAggroTarget(const IGsMessageParam* inParam)
{
	FGsBossHandler* bossHandler = FGsBossHandler::GetInstance();
	if (nullptr == bossHandler)
	{
		return;
	}

	FGsFieldBossHandler* handler = bossHandler->GetFieldBossHandler();
	if (nullptr == handler)
	{
		return;
	}

	const TMap<int64, int64>& aggroTargetMap = handler->GetAllAggroTarget();
	bool isAggro = false;
	for (const TPair<int64, int64>& aggroPair : aggroTargetMap)
	{
		if (aggroPair.Value != GetGameId())
		{
			continue;
		}
		
		isAggro = true;

#if AGGRO_DEBUG	
			if (GetActor()
				&& isAggro)
			{				
				GSLOG(Log, TEXT("AGGRO_DEBUG - UGsGameObjectPlayer::OnChangeAggroTarget - show - npc game id : %lld\ttarget game id : %lld\tname : %s")
					, aggroPair.Key
					, GetGameId()					
					, *GetData()->GetName());
			}
#endif

		break;
	}

	FGsGameObjectDataPlayer* data = GetCastData<FGsGameObjectDataPlayer>();
	if (nullptr == data)
	{
		return;
	}

#if AGGRO_DEBUG	
	if (GetActor()
		&& false == isAggro)
	{		
		GSLOG(Log, TEXT("AGGRO_DEBUG - UGsGameObjectPlayer::OnChangeAggroTarget - hide - game id : %lld\tname : %s")
			, GetGameId()
			, *GetData()->GetName());
	}
#endif

	data->SetShowAggro(isAggro);
	UpdateBillboard(EGsBillboardUpdateType::Aggro);
}

void UGsGameObjectPlayer::OnHideAggroTarget(const IGsMessageParam* inParam)
{
	if (FGsGameObjectDataPlayer* data = GetCastData<FGsGameObjectDataPlayer>())
	{
		data->SetShowAggro(false);
		UpdateBillboard(EGsBillboardUpdateType::Aggro);
	}
}

/// <summary>
/// Aggro와 Contribution을 끈다
/// </summary>
/// <param name="inParam"></param>
void UGsGameObjectPlayer::OnHideFieldBossBillboard(const IGsMessageParam* inParam /*= nullptr*/)
{
	FGsGameObjectDataPlayer* data = GetCastData<FGsGameObjectDataPlayer>();
	if (nullptr == data)
	{
		return;
	}

	data->SetShowAggro(false);
	data->SetShowContribution(false);

	UpdateBillboard(EGsBillboardUpdateType::Aggro);
	UpdateBillboard(EGsBillboardUpdateType::Contribution);
}

void UGsGameObjectPlayer::OnTitleUpdateBillboard(const IGsMessageParam* inParam /*= nullptr*/)
{
	UpdateBillboard(EGsBillboardUpdateType::Title);
}

void UGsGameObjectPlayer::SetGameObjectActorVisible(bool In_isVisible)
{
	Super::SetGameObjectActorVisible(In_isVisible);
	// player면 페어리도 hidden true
	if (FGsFairyHandlerPlayer* fairyHandler = GetFairyHandler())
	{
		if (AGsActorFieldFairy* actorFairy = fairyHandler->GetFairyActor())
		{
			actorFairy->SetActorHiddenInGame(!In_isVisible);
		}
	}
}

//void UGsGameObjectPlayer::OnPolymorp()
//{
//	if (const FGsSchemaPlayerShape* playerShpaeData = _pcData->shapeId.GetRow())
//	{
//		if (const FGsSchemaShapeData* shpaeData = playerShpaeData->polymorpShapeId.GetRow())
//		{
//			OnPolymorpChange(shpaeData);
//		}
//	}
//}
//
//void UGsGameObjectPlayer::RestorePolymorp()
//{
//	OnPolymorpChange(nullptr);
//}
// 아이템 사용
void UGsGameObjectPlayer::OnUseItem(ItemCategorySub In_category)
{
	if (FGsUseItemEffectHandler* useItemEffectHandler = GetUseItemEffectHandler())
	{
		useItemEffectHandler->OnUseItemCategory(In_category);
	}

}

int UGsGameObjectPlayer::UpdateCombatStatus(bool InImmediately)
{
	Super::UpdateCombatStatus(InImmediately);

	AGsCharacterPlayer* character = GetPlayerCharacter();
	if (nullptr == character)
	{
		return 0;
	}

	// 현재 WeaponType 얻기
	FGsGameObjectDataPlayer* playerData = GetCastData<FGsGameObjectDataPlayer>();
	CreatureWeaponType currWeapon = playerData->GetCurrentWeaponType();
	const FGsSchemaUserData* pcTable = GetPcTable();
	// 현재 모드 얻기
	bool isBattleMode = IsBattleMode();
	

	if (InImmediately)
	{
		// 소켓 변경
		GetCustomizeHandler()->ChangeWeaponSocket(pcTable, currWeapon, isBattleMode);
		return 0;
	}

	int result = 0;
	// 맨손 상태가 아닐 경우에만 처리
	if (currWeapon != CreatureWeaponType::HAND)
	{
		FGsAbnormalityHandlerBase* abnormality = GetAbnormalityHandler();
		// 전투 상태 전환 이거나 변신중일 경우 연출 없음
		if (isBattleMode
			|| abnormality->IsAbnormalityEffectType(AbnormalityEffectType::CC_POLYMORP)
			|| character->IsHidden())
		{
			// 소켓 변경
			character->StopWeaponTransformByCurve();
			GetCustomizeHandler()->ChangeWeaponSocket(pcTable, currWeapon, isBattleMode);
			// character->StopMeshShader();
		}
		// 평화 상태로 갈때
		else
		{
			const FGsSchemaWeaponPeaceEffect* findeffectInfo = _pcData->weaponPeaceEffectList.Find(currWeapon);
			const FGsSchemaWeaponSocketInfo* findSocketInfo = _pcData->weaponSocketList.Find(currWeapon);
			if (nullptr == findeffectInfo || nullptr == findSocketInfo)
			{
				return result;
			}

			// 전투 소켓에서 Detach 전 효과 얻기
			const FGsSchemaWeaponFrontEffect& effectInfo = findeffectInfo->detachEffect;

			TArray<FName> listSocketName;
			listSocketName.Emplace(findSocketInfo->battle.socketName_L);
			listSocketName.Emplace(findSocketInfo->battle.socketName_R);

			TArray<FVector> listOffset;
			listOffset.Emplace(effectInfo.rootOffset_L);
			listOffset.Emplace(effectInfo.rootOffset_R);

			character->ChangeWeaponSocketProcess(listSocketName, listOffset, effectInfo.curveDissolve, effectInfo.curveFresnel,
				effectInfo.particle, true, [this]() { CallbackStatusAttachSocketStart(); });

			if (const FGsSchemaWeaponSocketInfo* findInfo = pcTable->weaponSocketList.Find(currWeapon))
			{
				character->SetWeaponTransformByCurve(character->GetWeaponChildActorComponent(), &findInfo->peace, isBattleMode);
				character->SetWeaponTransformByCurve(character->GetWeaponSubChildActorComponent(), &findInfo->peace, isBattleMode);
			}

			result = 1;
		}
	}
	_fsm->ProcessEvent(EGsStateBase::ChangeIdle);
	return result;
}

void UGsGameObjectPlayer::UpdateWeaponChange(CreatureWeaponType CurrWeaponType)
{
	if (const FGsSchemaPlayerShape* shapeData = GetPcTable()->shapeId.GetRow())
	{
		const FGsSchemaPlayerWeaponDetailInfo* findWeaponDetail =
			shapeData->weaponDetailInfo.FindByPredicate([CurrWeaponType](FGsSchemaPlayerWeaponDetailInfo& el)
				{
					return el.type == CurrWeaponType;
				});

		if (findWeaponDetail)
		{
			FString path = (*findWeaponDetail).animationBP.ToString() + TEXT("_C");
			LoadAnimInstance(path);
		}

		// 소켓 변경
		GetCustomizeHandler()->ChangeWeaponSocket(GetPcTable(), CurrWeaponType, IsBattleMode());
	}
	UpdatePartyWeaponChange(CurrWeaponType);
}
void UGsGameObjectPlayer::UpdatePartyWeaponChange(CreatureWeaponType CurrWeaponType)
{
	// 내가 파티에 속했나
	// 파티 멤버인가
	int64 gameId = GetGameId();
	if (GSParty()->GetIsInParty() == true &&
		GSParty()->IsInPartyGameId(gameId) == true)
	{
		GSParty()->OnChangedWeapon(gameId, CurrWeaponType);
	}
}

void UGsGameObjectPlayer::UpdateFairyEquip(FairyId id)
{
	if (GetFairyHandler() == nullptr)
	{
		return;
	}
	GetFairyHandler()->UpdateEquipFairy(id);

	// bak1210 : 비동기로 변화되면서 내부에서 갱신한다.
	// [B1] | ejrrb10 | 페어리 연출 방식이 바뀜(비동기->동기)에 따라 다시 visible 상태 갱신
	// 만들어진 페어리 visible 상태 갱신
	UpdateFairyActorVisible();
}

void UGsGameObjectPlayer::ChangeFairyEquip(FairyId id)
{
	UpdateFairyEquip(id);
}

void UGsGameObjectPlayer::RestoreEquipFairy()
{
	if (GetFairyHandler() == nullptr)
	{
		return;
	}
	GetFairyHandler()->RestoreEquipFairy();
}

void UGsGameObjectPlayer::UpdateCostumeEquip(CostumeId lookatId, CostumeId costumeId, bool isHairOn)
{
	if (GetCustomizeHandler() == nullptr)
	{
		return;
	}
	GetCustomizeHandler()->SetCostume(lookatId, costumeId, isHairOn);
}

void UGsGameObjectPlayer::ChangeCostumeEquip(CostumeId lookatId, CostumeId costumeId, bool isHairOn)
{
	UpdateCostumeEquip(lookatId, costumeId, isHairOn);
}

void UGsGameObjectPlayer::SetCustomizeDataBonScale(const CustomizeFaceData& InFaceData, const CustomizeBodyData& InBodyData)
{
	if (GetCustomizeHandler() == nullptr)
	{
		return;
	}
	GetCustomizeHandler()->SetAsyncMode();
	GetCustomizeHandler()->SetCustomizeDataBonScale(InFaceData, InBodyData);

	AGsCharacterPlayer* character = GetPlayerCharacter();
	UCapsuleComponent* capsule = character->GetCapsuleComponent();
	USkeletalMeshComponent* skeletalMeshComp = character->GetMesh();
	FGsGameObjectDataPlayer* playerData = GetCastData<FGsGameObjectDataPlayer>();
	if(capsule && character && playerData)
	{
		UGsGameObjectUtil::MeshRelativeLocation(playerData->GetCreatureGenderType()
			, InBodyData.mHeight
			, capsule->GetScaledCapsuleHalfHeight()
			, character);
	}
}

void UGsGameObjectPlayer::CallbackStatusAttachSocketStart()
{
	AGsCharacterPlayer* character = GetPlayerCharacter();
	if (nullptr == character)
	{
		return;
	}

	// 현재 WeaponType 얻기
	FGsGameObjectDataPlayer* playerData = GetCastData<FGsGameObjectDataPlayer>();
	CreatureWeaponType currWeapon = playerData->GetCurrentWeaponType();

	// 소켓 변경
	GetCustomizeHandler()->ChangeWeaponSocket(GetPcTable(), currWeapon, IsBattleMode());

	// 평화 상태 정보 얻기
	const FGsSchemaWeaponPeaceEffect* findeffectInfo = _pcData->weaponPeaceEffectList.Find(currWeapon);
	const FGsSchemaWeaponSocketInfo* findSocketInfo = _pcData->weaponSocketList.Find(currWeapon);
	if (nullptr == findeffectInfo || nullptr == findSocketInfo)
	{
		return;
	}

	// 평화 소켓 붙이기전 효과 정보 얻기
	const FGsSchemaWeaponBackEffect& effectInfo = findeffectInfo->attachEffect;

	TArray<FName> listSocketName;
	listSocketName.Emplace(findSocketInfo->peace.socketName);

	TArray<FVector> listOffset;
	listOffset.Emplace(effectInfo.rootOffset);

	character->ChangeWeaponSocketProcess(listSocketName, listOffset, effectInfo.curveDissolve, effectInfo.curveFresnel,
		effectInfo.particle, false, [character]()
		{
			character->StopMeshShader();
		});
}

TSharedPtr<FGsInteractionHandlerBase> UGsGameObjectPlayer::CreateInteractionHandler()
{
	return MakeShareable(new FGsInteractionHandlerBase());
}

TSharedPtr<FGsFairyHandlerPlayer> UGsGameObjectPlayer::CreateFairyHandler()
{
	return MakeShareable(new FGsFairyHandlerPlayer());
}

TSharedPtr<FGsAbnormalityHandlerBase> UGsGameObjectPlayer::CreateAbnormality()
{
	return MakeShareable(new FGsAbnormalityHandlerPlayer());
}

// 스탯 갱신
void UGsGameObjectPlayer::OnChangedStat(const FGsStatInfo& inStatInfo)
{
	Super::OnChangedStat(inStatInfo);

	int64 gameId = GetGameId();

	// 내가 파티에 속했나
	// 대상이 파티에 속했나
	if (GSParty()->GetIsInParty() == true &&
		GSParty()->IsInPartyGameId(gameId) == true)
	{
		GSParty()->OnChangedStat(gameId, inStatInfo);
	}
}

void UGsGameObjectPlayer::OnGameObjectDataUpdate(EGsGameObjectDataMember MemberType)
{
	Super::OnGameObjectDataUpdate(MemberType);

	switch (MemberType)
	{
	case EGsGameObjectDataMember::Level:
	{
		FGsGameObjectData* data = GetData();
		if (data != nullptr)
		{
			int newLevel = data->GetLevel();
			int64 gameId = GetGameId();
			// 내가 파티에 속했나
			// 파티 멤버인가
			if (GSParty()->GetIsInParty() == true &&
				GSParty()->IsInPartyGameId(gameId) == true)
			{
				GSParty()->OnChangedLevel(gameId, newLevel);
			}
		}
	}
	break;
	}
}

void UGsGameObjectPlayer::PopChatBubble(FText chat)
{
	if (auto player = GetPlayerCharacter())
	{
		if (auto widget = Cast<UGsUIBillboardPlayer>(player->GetWidget()))
		{
			widget->SetTalkText(chat);
		}
	}
}

void UGsGameObjectPlayer::PopEmojiBubble(int32 emojiID)
{
	if (auto player = GetPlayerCharacter())
	{
		if (auto widget = Cast<UGsUIBillboardPlayer>(player->GetWidget()))
		{
			widget->SetEmoticon(emojiID);
		}
	}
}

// visible 상태인지 갱신
void UGsGameObjectPlayer::UpdateVisible()
{
	Super::UpdateVisible();

	UpdateFairyActorVisible();
}

void UGsGameObjectPlayer::ChangeIdleType(EGsAnimStateIdleType inType)
{
	Super::ChangeIdleType(inType);
}

void UGsGameObjectPlayer::InitializeActor(class AActor* Owner)
{
	Super::InitializeActor(Owner);

	if (FGsCustomizeHandler* customize = GetCustomizeHandler())
	{
		if (FGsGameObjectDataPlayer* playerData = GetCastData<FGsGameObjectDataPlayer>())
		{
			//bak1210 : 내부에서 호출
			/*customize->InitializeActor(GetCharacter());*/
			customize->SetGenderType(playerData->GetCreatureGenderType());
		}
	}
	
	if (AGsCharacterBase* actorCharacter = GetCharacter())
	{
		_creatureCommonInfo = GetCommonInfoTable();
		if (nullptr != _creatureCommonInfo)
		{
			if (UCapsuleComponent* capsule = actorCharacter->GetCapsuleComponent())
			{
				// 스케일 변화가 있을경우 위치를 다시 갱신
				// 높이 보정
				FHitResult hit;
				if (UGsLevelUtil::TrySweepToLand(hit, GetWorld(), Owner->GetActorLocation(), _creatureCommonInfo->cylinderRadius, _creatureCommonInfo->cylinderHeight))
				{
					FVector reCalcPos = hit.ImpactPoint + FVector(0, 0, capsule->GetScaledCapsuleHalfHeight());
					Owner->SetActorLocation(reCalcPos);
				}
			}
		}

		// 플레이어 타입 히트 이펙트 데이터 설정
		if (UGsHitEffectComponent* hitComp = actorCharacter->GetHitEffect())
		{
			if (_pcData)
			{
				UGsHitEffectData* effectInfo = _pcData->HitEffectDataOverride == nullptr ?
					GData()->GetGlobalData()->HitEffectData : _pcData->HitEffectDataOverride;
				const FGsSchemaSoundResData* soundInfo = _pcData->HitSoundDataOverride.IsNull() ?
					GData()->GetGlobalData()->HitSoundData.GetRow() : _pcData->HitSoundDataOverride.GetRow();

				hitComp->OverrideData(effectInfo, soundInfo,
					_creatureCommonInfo->hitEffectParticleSizeRate, _creatureCommonInfo->hitEffectModelRadius);
			}
		}


		FGsGameObjectDataPlayer* playerData = GetCastData<FGsGameObjectDataPlayer>();
		CreatureWeaponType currWeapon = playerData->GetCurrentWeaponType();
		if (AGsCharacterPlayer* playerActor = Cast<AGsCharacterPlayer>(actorCharacter))
		{
			playerActor->SetWeaponType(currWeapon);
		}
	}

	// 전투 평화 상태 동기화 연출 x
	UpdateCombatStatus(true);
}

// 페어리 액터 연출에 따른 visible 상태 갱신
void UGsGameObjectPlayer::UpdateFairyActorVisible()
{
	if (FGsFairyHandlerPlayer* fairyHandler = GetFairyHandler())
	{
		if (AGsActorFieldFairy* actorFairy = fairyHandler->GetFairyActor())
		{
			// Sentry 널체크 처리 (https://com2us-raon.sentry.io/issues/4283348824/events/6ceb8880ae904dfdc6b2df10ce9f3247/)
			if (GetActor() == nullptr)
				return;

			actorFairy->SetActorHiddenInGame(GetActor()->IsHidden());
		}
	}
}

// 위젯 이름 show/hide 옵션 갱신
void UGsGameObjectPlayer::UpdateWidgetShowNameOption()
{
	AGsCharacterPlayer* character = GetPlayerCharacter();
	if (character == nullptr)
	{
		return;
	}

	UpdateBillboard(EGsBillboardUpdateType::All);
}

UserStatus UGsGameObjectPlayer::GetUserStatus()
{
	if (FGsGameObjectDataCreature* creatureData = GetCastData<FGsGameObjectDataCreature>())
	{
		return creatureData->GetUserStatus();
	}

	return UserStatus::USER_STATE_NORMAL;
}

const FGsSchemaShapeData* UGsGameObjectPlayer::GetPolymorphShapeData()
{
	if (_pcData)
	{
		if (const FGsSchemaPlayerShape* playerShapeData = _pcData->shapeId.GetRow())
		{
			return playerShapeData->polymorpShapeId.GetRow();
		}
	}

	return nullptr;
}

void UGsGameObjectPlayer::UpdateBillboard(EGsBillboardUpdateType InType)
{
	if (AGsCharacterPlayer* actor = Cast<AGsCharacterPlayer>(GetActor()))
	{
		if (UGsUIBillboardPlayer* widget = Cast<UGsUIBillboardPlayer>(actor->GetWidget()))
		{
			widget->UpdateByType(InType);
		}
	}
}
