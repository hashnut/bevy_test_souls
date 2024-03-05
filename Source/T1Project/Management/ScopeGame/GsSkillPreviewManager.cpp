// Fill out your copyright notice in the Description page of Project Settings.


#include "Management/ScopeGame/GsSkillPreviewManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "NoExportTypes.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Camera/CameraComponent.h"
#include "GsGameObjectManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Classes/GsSpawn.h"
#include "ActorEx/GsActorSkillPreviewSceneObject.h"
#include "ActorEx/GsCharacterBase.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "GameObject/Skill/GsSkillHandlerRemotePlayer.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectSkillPreviewPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectSkillPreviewProjectile.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerSkillPreviewPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Define/GsGameObjectStruct.h"

#include "DataSchema/SkillPreview/GsSchemaSkillPreviewSet.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/SkillPreview/GsSchemaSkillPreviewWeaponSet.h"
#include "DataSchema/SkillPreview/GsSchemaSkillPreviewCameraModeDataSet.h"
#include "DataSchema/SkillPreview/GsSchemaSkillPreviewInfo.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHeal.h"

#include "Sound/GsSoundMixerController.h"
#include "Message/GsMessageUI.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GsGameFlowGame.h"
#include "UTIL/GsDir.h"
#include "../ScopeGlobal/GsSkillManager.h"


void UGsSkillPreviewManager::Initialize()
{
	// 단일 데이터만 존재할수 있으므로 Rowname 하드코딩
	FString rowName(TEXT("SkillPreview"));
	_skillPreviewData = UGsTableUtil::FindRow<FGsSchemaSkillPreviewSet>(rowName);
}

void UGsSkillPreviewManager::Finalize()
{
}

void UGsSkillPreviewManager::Update(float inTick)
{
	if (_owner)
	{
		_owner->Update(inTick);
	}

	for (UGsGameObjectBase* el : _listPartyPlayer)
	{
		el->Update(inTick);
	}

	for (UGsGameObjectBase* el : _listEnemyPlayer)
	{
		el->Update(inTick);
	}

	for (UGsGameObjectBase* el : _listProjectile)
	{
		el->Update(inTick);
	}
}

void UGsSkillPreviewManager::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	// 절전모드에서 스킬 미리보기로 돌아갈 때는 이벤트 처리를 하지 않는다.
	if (In_preType == EGsEventProgressType::SAVE_BATTERY)
	{
		return;
	}

	if (In_startType != EGsEventProgressType::SKILL_PREVIEW_ROOM)
	{
#if UE_EDITOR
		GEngine->AddOnScreenDebugMessage(99, 5.f, FColor::Yellow, TEXT("[SkillPreviewError] not sync EGsEventProgressType"));
#endif
		return;
	}

	if (nullptr == _sceneObject)
	{
		_sceneObject = SpawnSceneObject();
	}
}

void UGsSkillPreviewManager::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	// 스킬 미리보기에서 절전 모드가 켜진다면 이벤트 처리를 하지 않는다.
	if (In_nextType == EGsEventProgressType::SAVE_BATTERY)
	{
		return;
	}

	// 시련의 전당에서 문지기로 인터랙션 이동 시켜놓고
	// 스킬 window 열면 화면 깨짐 현상 수정
	// https://jira.com2us.com/jira/browse/C2URWQ-5168
	GUI()->CloseByKeyName(TEXT("WindowSkill"));
}

AGsActorSkillPreviewSceneObject* UGsSkillPreviewManager::SpawnSceneObject()
{
	if (_sceneObject && _sceneObject->IsValidLowLevel())
	{
		DespawnSceneObject();
	}

	UWorld* world = GetWorld();
	// https://com2us-raon.sentry.io/issues/4276840579/events/ee4262efdf8c41d1eddc8ca4009e2229/
	// 요정도 처리하면 될것 같은데... 이범용차석님
	if (nullptr == world || nullptr == GSGameObject())
	{
		return nullptr;
	}
	
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	GLevel()->HoldStreamingLevelByLocation(player->GetLocation());

	//  조이스틱 비활성 처리
	if (APlayerController* playerController = world->GetFirstPlayerController())
	{
		playerController->ActivateTouchInterface(nullptr);
		playerController->SetIgnoreMoveInput(true);
	}

	// 컨텐츠 타입 변경(임시)
	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
		{
			contents->ChangeState(FGsContentsMode::InGame::ContentsSkill);
		}
	}

	// 사운드 믹서 변경
	if (UGsSoundManager* soundManager = GSound())
	{
		UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController();
		soundMixerController->OnMixChanged(EGsSoundMixType::SkillPreview);
	}
	
	// 내캐릭터 필요정보 설정
	if (FGsGameObjectDataLocalPlayer* localData = player->GetCastData<FGsGameObjectDataLocalPlayer>())
	{
		_currentGenderType = localData->GetCreatureGenderType();
	}
	//

	// 씬 액터 로딩
	FString path = _skillPreviewData->sceneActor.ToString();
	path += TEXT("_C");
	UClass* bpClass = LoadObject<UClass>(nullptr, *path);

	FTransform tr = FTransform::Identity;
	FVector pos = player->GetLocation();
	pos += _skillPreviewData->sceneActorPosition;
	tr.SetLocation(pos);

	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (AGsActorSkillPreviewSceneObject* actor = Cast<AGsActorSkillPreviewSceneObject>(
		world->SpawnActor(bpClass, &tr, spawnParam)))
	{
		_sceneObject = actor;
	}

	_partyDummyScene[0] = _sceneObject->_dummyPartyPlayer_Left;
	_partyDummyScene[1] = _sceneObject->_dummyPartyPlayer_Right;
	_enemyDummyScene[0] = _sceneObject->_dummyEnemyPlayer_Center;
	_enemyDummyScene[1] = _sceneObject->_dummyEnemyPlayer_Left;
	_enemyDummyScene[2] = _sceneObject->_dummyEnemyPlayer_Right;
	
	return _sceneObject;
}

void UGsSkillPreviewManager::DespawnSceneObject()
{
	UWorld* world = GetWorld();

	ClearTimer();

	// 액터 소멸
	if (_sceneObject)
	{
		_sceneObject->Destroy();
		_sceneObject->MarkPendingKill();
		_sceneObject = nullptr;
	}

	// 지형 처리 복구
	if (UGsLevelManager* levelManager = GLevel())
	{
		levelManager->UnHoldStreamingLevel();
	}

	// 조이스틱 복구
	if (APlayerController* playerController = world->GetFirstPlayerController())
	{
		playerController->CreateTouchInterface();
		playerController->SetIgnoreMoveInput(false);
	}

	// 컨텐츠 타입 되돌림(임시)
	//GMessage()->GetUI().SendMessage(MessageUI::RECOVER_TO_DEFAULT_STATE, nullptr);

	// 사운드 믹서 복구
	if (UGsSoundManager* soundManager = GSound())
	{
		UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController();
		soundMixerController->OffMixMode(EGsSoundMixType::SkillPreview);
	}
}

void UGsSkillPreviewManager::PlaySkillPreview(int In_SkillId, uint8 In_SkillLevel, const FGsSchemaCostumeCommon* In_CostumeData)
{
	const FGsSchemaSkillSet* skillData = UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(In_SkillId);
	if (nullptr == skillData)
	{
#if UE_EDITOR
		FString msg = FString::Printf(TEXT("[SkillPreviewError] not found SkillSet %d"), In_SkillId);
		GEngine->AddOnScreenDebugMessage(99, 5.f, FColor::Yellow, msg);
#endif
		return;
	}	

	PlaySkillPreview(skillData, In_SkillLevel, In_CostumeData);
}

void UGsSkillPreviewManager::PlaySkillPreview(const FGsSchemaSkillSet* In_SkillData, uint8 In_SkillLevel, 
	const FGsSchemaCostumeCommon* In_CostumeData)
{
	// 씬로딩 확인
	if (nullptr == _sceneObject)
	{
		_sceneObject = SpawnSceneObject();
	}

	ClearTimer();

	// 캐릭터 스폰
	if (false == SetupGameObject(In_SkillData, In_CostumeData))
	{
#if UE_EDITOR
		FString msg = FString::Printf(TEXT("[SkillPreviewError] Spawn Object Failed %d"), In_SkillData->id);
		GEngine->AddOnScreenDebugMessage(99, 5.f, FColor::Yellow, msg);
#endif
		return;
	}

	FGsTargetHandlerBase* targetHandler = _owner->GetTargetHandler();
	targetHandler->ClearTarget();
	// 타겟 정보 얻기 
	if (UGsGameObjectBase* targetObject = FindObject((int)EGsSkillPreivewGameObjectStartID::EnemyPlayer))
	{
		targetHandler->SetTarget(targetObject);
	}

	// 스킬 사용
	PD::SC::PKT_SC_START_SKILL_READ packet;
	packet.mCasterId = _owner->GetGameId();
	packet.mSkillId = In_SkillData->id;
	packet.mSkillLevel = In_SkillLevel;
	packet.mCurPos = _ownerPosition;
	packet.mCurrDir = FGsDir::dirToAngle(_ownerDir);
	packet.mAimStartPos = FVector::ZeroVector;
	packet.mAimTargetPos = FVector::ZeroVector;
	packet.mTargetId = 0;
	if (const FGsSchemaRootmotionSet* rootmotionSet = In_SkillData->rootMotionId.GetRow())
	{
		packet.mReactionId = rootmotionSet->id;
	}
	else
	{
		packet.mReactionId = 0;
	}
	packet.mSkillStartType = GetSkillStartType(In_SkillData);
	ExcuteStartSkill(_owner, &packet);

	// 스폰 딜레이 처리
// 	GetWorld()->GetTimerManager().SetTimer(_delayTimer, [In_SkillData, In_SkillLevel, In_CostumeData, this]()
// 		{
// 			// 오브젝트 보임 처리
// 			for (UGsGameObjectBase* el : _listCacheObject)
// 			{
// 				el->SetGameObjectActorVisible(true);
// 			}
// 
// 			// 스킬 사용
// 			PD::SC::PKT_SC_START_SKILL_READ packet;
// 			packet.mCasterId = _owner->GetGameId();
// 			packet.mSkillId = In_SkillData->id;
// 			packet.mSkillLevel = In_SkillLevel;
// 			packet.mCurPos = _owner->GetLocation();
// 			packet.mCurrDir = FGsDir::dirToAngle(_owner->GetRotation().Vector());
// 			packet.mAimStartPos = FVector::ZeroVector;
// 			packet.mAimTargetPos = FVector::ZeroVector;
// 			packet.mTargetId = 0;
// 			if (const FGsSchemaRootmotionSet* rootmotionSet = In_SkillData->rootMotionId.GetRow())
// 			{
// 				packet.mReactionId = rootmotionSet->id;
// 			}
// 			else
// 			{
// 				packet.mReactionId = 0;
// 			}
// 			packet.mSkillStartType = GetSkillStartType(In_SkillData);
// 			ExcuteStartSkill(_owner, &packet);
// 
// 			GetWorld()->GetTimerManager().ClearTimer(_delayTimer);
// 		}, _skillPreviewData->startingDelay, false);
}

bool UGsSkillPreviewManager::SetupGameObject(const FGsSchemaSkillSet* In_SkillData, const FGsSchemaCostumeCommon* In_CostumeData)
{
	// DespawnAllGameObject();

	CreatureWeaponType weaponType = In_SkillData->requireWeapon;
	const FGsSchemaSkillPreviewWeaponSetRow* findWeaponRow = _skillPreviewData->skillPreviewWeaponInfo.Find(weaponType);
	if (nullptr == findWeaponRow)
	{
#if UE_EDITOR
		FString msg = FString::Printf(TEXT("[SkillPreviewError] not found SkillPreviewWeaponSet %d"), In_SkillData->id);
		GEngine->AddOnScreenDebugMessage(99, 5.f, FColor::Yellow, msg);
#endif
		return false;
	}
	
	const FGsSchemaSkillPreviewWeaponSet* weaponSet = findWeaponRow->GetRow();
	const FGsSchemaSkillPreviewGenderInfo* shapeInfo = weaponSet->playerInfo.Find(_currentGenderType);
	const FGsSchemaSkillPreviewInfo* skillInfo = weaponSet->skillPreviewInfo.Find(In_SkillData->id);
	if (nullptr == weaponSet || nullptr == skillInfo || nullptr == shapeInfo)
	{
#if UE_EDITOR
		FString msg = FString::Printf(TEXT("[SkillPreviewError] not found GenderInfo or SkillPreviewInfo  %d"), In_SkillData->id);
		GEngine->AddOnScreenDebugMessage(99, 5.f, FColor::Yellow, msg);
#endif
		return false;
	}

	const FGsSchemaSkillPreviewTransformSet* gameObjectTransform = skillInfo->gameObjectTransform.GetRow();
	const FGsSchemaSkillPreviewCameraModeDataSet* cameraTransform = skillInfo->cameraTransform.GetRow();
	if (nullptr == gameObjectTransform || nullptr == cameraTransform)
	{
#if UE_EDITOR
		FString msg = FString::Printf(TEXT("[SkillPreviewError] not found SkillPreviewTransformSet or SkillPreviewCameraModeDataSet %d"), In_SkillData->id);
		GEngine->AddOnScreenDebugMessage(99, 5.f, FColor::Yellow, msg);
#endif
		return false;
	}

	// 프로젝타일 제거
	for (UGsGameObjectBase* el : _listProjectile)
	{
		DespawnGameObject(el);
	}
	_listProjectile.Empty();

	// 카메라 설정
	_sceneObject->SetTransform_Camera(cameraTransform->transform);
	_sceneObject->SetFieldOfView_Camera(cameraTransform->fieldOfView);

	// 대상 캐릭터 스폰
	const FGsSchemaUserData* userData = shapeInfo->playerUserData.GetRow();
	if (nullptr == userData)
	{
#if UE_EDITOR
		FString msg = FString::Printf(TEXT("[SkillPreviewError] not found UserData %d"), In_SkillData->id);
		GEngine->AddOnScreenDebugMessage(99, 5.f, FColor::Yellow, msg);
#endif
		return false;
	}

	FGsSpawnParamPlayer spawnParam;

	// 내캐릭터의 무기 정보가 다를 경우는 다시 스폰
	if (_currentWeaponType != weaponType && _owner)
	{
		DespawnGameObject(_owner);
		_owner = nullptr;
	}

	_currentWeaponType = weaponType;

	// 커스터 마이징 반영
	const FGsNetUserData* netUserData = GGameData()->GetUserData();

	// 내캐릭터 스폰
	if (nullptr == _owner)
	{
		spawnParam._tId = userData->commonInfoId;
		spawnParam._gameId = (int)EGsSkillPreivewGameObjectStartID::OwnerPlayer;
		_owner = SpawnGameObject(shapeInfo->playerBP, _sceneObject->_dummyOwner, &spawnParam);
		
		if (FGsCustomizeHandler* customizeHandler = _owner->GetCustomizeHandler())
		{
			// 스킨 정보와 파츠 정보만 동기화
			customizeHandler->InitializeActor(_owner->GetCharacter());
			customizeHandler->SetGenderType(_currentGenderType);
			customizeHandler->SetCustomizeDataBonScale(netUserData->mFaceData, netUserData->mBodyData);			
		}

		if (AGsCharacterPlayer* characterActor = Cast<AGsCharacterPlayer>(_owner->GetCharacter()))
		{
			characterActor->OnCostumeEffectLoadComplate.AddDynamic(this, &UGsSkillPreviewManager::OnCostumeEffectLoadComplate);
		}
	}

	// 코스튬 반영
	if (FGsCustomizeHandler* customizeHandler = _owner->GetCustomizeHandler())
	{
		uint32 costumeId = (In_CostumeData) ? In_CostumeData->id : netUserData->mLookInfo.mCostumeId;
		customizeHandler->SetCostume(netUserData->mLookInfo.mTransformCostumeId, costumeId, 
			netUserData->mLookInfo.mViewHelmet, false, false, true);
	}

	InitializeLocationAndRotation(_sceneObject->_dummyOwner, _owner, gameObjectTransform->owner);

	CancelAnimation(_owner);
	CancelAbnormality(_owner);

	ClearSyncVariable();	
	
	// 아군 캐릭터 스폰
	if (const FGsSchemaUserData* partyUserData = _skillPreviewData->partyUserData.GetRow())
	{
		const FTransform* partyTransform[2] = {
			&gameObjectTransform->party_left,
			&gameObjectTransform->party_right,
		};

		for (int i = 0; i < skillInfo->playerCount - 1; ++i)
		{			
			if (_listPartyPlayer.Num() == i)
			{
				spawnParam._tId = partyUserData->commonInfoId;
				spawnParam._gameId = (int)EGsSkillPreivewGameObjectStartID::PartyPlayer + i;
				UGsGameObjectBase* party = SpawnGameObject(_skillPreviewData->partyBP,
					_partyDummyScene[i], &spawnParam);
				_listPartyPlayer.Emplace(party);
			}

			InitializeLocationAndRotation(_partyDummyScene[i], _listPartyPlayer[i], *partyTransform[i]);

			_listPartyPlayer[i]->SetGameObjectActorVisible(true);

			CancelAnimation(_listPartyPlayer[i]);
			CancelAbnormality(_listPartyPlayer[i]);
		}
	}
	// 안보이는 객체 설정
	for (int i = skillInfo->playerCount - 1; i < _listPartyPlayer.Num(); ++i)
	{
		_listPartyPlayer[i]->SetGameObjectActorVisible(false);
		CancelAnimation(_listPartyPlayer[i]);
		CancelAbnormality(_listPartyPlayer[i]);
	}

	// 적 캐릭터 스폰
	if (const FGsSchemaNpcData* npcData = _skillPreviewData->npcData.GetRow())
	{
		const FTransform* enemyTransform[3] = {
			&gameObjectTransform->enemy_center,
			&gameObjectTransform->enemy_left,
			&gameObjectTransform->enemy_right,
		};

		spawnParam._tId = npcData->id;
		
		for (int i = 0; i < skillInfo->monsterCount; ++i)
		{
			if (_listEnemyPlayer.Num() == i)
			{
				spawnParam._gameId = (int)EGsSkillPreivewGameObjectStartID::EnemyPlayer + i;
				UGsGameObjectBase* enemy = SpawnGameObject(_skillPreviewData->npcBP,
					_enemyDummyScene[i], &spawnParam);
				_listEnemyPlayer.Emplace(enemy);
			}

			InitializeLocationAndRotation(_enemyDummyScene[i], _listEnemyPlayer[i], *enemyTransform[i]);

			// 적 캐릭터는 내 캐릭터 방향 고정
			FTransform tm = *enemyTransform[i];
			FRotator rot = tm.GetRotation().Rotator();
			FVector dir = _sceneObject->_dummyOwner->GetComponentLocation() - _enemyDummyScene[i]->GetComponentLocation();
			if (AGsCharacterBase* character = _listEnemyPlayer[i]->GetCharacter())
			{
				character->SetActorRotation(dir.Rotation());
			}

			_listEnemyPlayer[i]->SetGameObjectActorVisible(true);

			CancelAnimation(_listEnemyPlayer[i]);
			CancelAbnormality(_listEnemyPlayer[i]);
		}

		// 안보이는 객체 설정
		for (int i = skillInfo->monsterCount; i < _listEnemyPlayer.Num(); ++i)
		{
			_listEnemyPlayer[i]->SetGameObjectActorVisible(false);
			CancelAnimation(_listEnemyPlayer[i]);
			CancelAbnormality(_listEnemyPlayer[i]);
		}
	}	

	return true;
}

UGsGameObjectBase* UGsSkillPreviewManager::SpawnProjectile(const FGsSchemaProjectileSet* In_ProjectileData,
	const FVector& In_Pos, const FVector& In_Dir, uint32 In_SkillId)
{
	if (const FGsSchemaShapeData* shapeData = In_ProjectileData->shapeId.GetRow())
	{		
		FString path = shapeData->bPClass.ToString();
		path += TEXT("_C");
		UClass* bpClass = LoadObject<UClass>(nullptr, *path);

		if (UGsGameObjectSkillPreviewProjectile* projectile = NewObject<UGsGameObjectSkillPreviewProjectile>(this))
		{
			int64 gameId = (_listProjectile.Num() > 0) ? _listProjectile.Last()->GetGameId() + 1 :
				(int64)EGsSkillPreivewGameObjectStartID::Projectile;

			projectile->SetSpawnSKillId(In_SkillId);

			FGsSpawnParam spawnParam;
			spawnParam._tId = In_ProjectileData->id;
			spawnParam._gameId = gameId;
			spawnParam._pos = In_Pos;
			spawnParam._rot = In_Dir.Rotation();
			projectile->Initialize(&spawnParam);
			AActor* actor = GsSpawn::BPClass(GetWorld(), bpClass, spawnParam._pos, spawnParam._rot);
			if (nullptr == actor)
			{
#if UE_EDITOR
				FString msg = FString::Printf(TEXT("[SkillPreviewError] projectile actor Load failed %d, %s"), In_ProjectileData->id,
					*path);
				GEngine->AddOnScreenDebugMessage(99, 5.f, FColor::Yellow, msg);
#endif
				return nullptr;
			}

			// 액터 디스폰 델리게이트 연결
			actor->OnDestroyed.AddDynamic(this, &UGsSkillPreviewManager::OnActorDestroyed);
			projectile->ActorSpawned(actor);

			// 인게임에서 사용되는 프로젝타일의 기본적인 LifeTime은 데이터 보다 길게 설정 하지만
			// 이경우는 동일하게 설정
			actor->SetLifeSpan(In_ProjectileData->lifeTime);

			_listProjectile.Emplace(projectile);
			return projectile;
		}
	}
	return nullptr;
}

void UGsSkillPreviewManager::ExcuteDamage(UGsGameObjectBase* In_TargetObject, PD::SC::PKT_SC_SKILL_DAMAGED_READ* In_Packet)
{
	if (UGsGameObjectCreature* castObject = In_TargetObject->CastGameObject<UGsGameObjectCreature>())
	{
		castObject->NetSkillDamagedAck(In_Packet);
	}
}

void UGsSkillPreviewManager::ExcuteHeal(UGsGameObjectBase* In_TargetObject, PD::SC::PKT_SC_SKILL_HEALED_READ* In_Packet)
{
	if (UGsGameObjectCreature* castObject = In_TargetObject->CastGameObject<UGsGameObjectCreature>())
	{
		castObject->NetSkillHealAck(In_Packet);
	}
}

void UGsSkillPreviewManager::ExcuteStartSkill(UGsGameObjectBase* In_TargetObject, PD::SC::PKT_SC_START_SKILL_READ* In_Packet)
{
	if (FGsSkillHandlerBase* skillHandler = In_TargetObject->GetSkillHandler())
	{
		FGsSkillRunnerBase* runner = skillHandler->GetSkillRunner();
		if (runner->IsValid())
		{
			skillHandler->CompulsionCancelSkill(runner->GetSkillData()->id);
			FGsGameObjectStateManager::ProcessEvent(_owner, EGsStateBase::AttackEnd);
		}
		
		skillHandler->NetSkillStart(In_Packet);
	}
}

void UGsSkillPreviewManager::ExecuteNotify(uint32 In_SkillId, int64 In_CastObjectID, int64 In_TargetObjectID, 
	const FGsSchemaSkillNotifySet* In_NotifyData, int64 In_SubGameId)
{
	for (const FGsSchemaSkillNotifyElementRow& row : In_NotifyData->notifyElementList)
	{
		const FGsSchemaSkillNotifyElement* element = row.GetRow();
		if (nullptr == element)
		{
			continue;
		}

		UGsGameObjectBase* target = FindObject(In_TargetObjectID);
		if (nullptr == target)
		{
			return;
		}

		// 특정 타입들 처리
		if (In_NotifyData->type == SkillNotifyType::VARY_HP_MP)
		{
			if (const FGsSchemaSkillNotifyHeal* healSet = element->typeInfo.GetRowByType<FGsSchemaSkillNotifyHeal>())
			{
				PD::SC::PKT_SC_SKILL_HEALED_READ packet;
				packet.mCasterId = In_CastObjectID;
				packet.mTargetId = In_TargetObjectID;
				packet.mSkillId = In_SkillId;
				packet.mSkillLevel = 1;
				packet.mHealByType = HealByType::SKILL;
				packet.mSkillNotifyId = In_NotifyData->id;
				ExcuteHeal(target, &packet);
			}
		}
		else if (const FGsSchemaSkillNotifyHitSet* hitSet = element->typeInfo.GetRowByType<FGsSchemaSkillNotifyHitSet>())
		{
			// 데미지 처리
			if (In_NotifyData->type == SkillNotifyType::DAMAGE && hitSet->noDamage == false)
			{
				PD::SC::PKT_SC_SKILL_DAMAGED_READ packet;
				packet.mCasterId = In_CastObjectID;
				packet.mTargetId = In_TargetObjectID;
				packet.mSkillId = In_SkillId;
				packet.mSkillNotifyId = In_NotifyData->id;
				packet.mMediumId = In_SubGameId;
				packet.mHitResultType = (hitSet->noDamage) ? HitResultType::MAX : HitResultType::NORMAL;
				ExcuteDamage(target, &packet);
			}

			// abnormality 처리
			// abnormalityApply컬럼이 2개가 되어 2번 체크 진행
			Internal_ExecuteAbnormality(target, hitSet->abnormalityApplyType1,
				hitSet->abnormalityId1);
			Internal_ExecuteAbnormality(target, hitSet->abnormalityApplyType2,
				hitSet->abnormalityId2);
		}
	}
}

void UGsSkillPreviewManager::Internal_ExecuteAbnormality(UGsGameObjectBase* In_TargetObject,
	AbnormalityApplyType In_ApplyType, int In_Id)
{
	FGsAbnormalityHandlerBase* abnormalityHandler = In_TargetObject->GetAbnormalityHandler();
	if (nullptr == abnormalityHandler)
	{
		return;
	}

	// RemoveType은 처리 대상 아니고,
	// AbnormalityApplyType::ADD_GROUP 처리가 필요할 경우 구현 필요
	if (In_ApplyType != AbnormalityApplyType::ADD)
	{
		return;
	}
	
	if (In_Id == 0)
	{
		return;
	}

	const FGsSchemaAbnormalitySet* abnormalitySet = UGsTableUtil::FindRowById<
		UGsTableAbnormalitySet, FGsSchemaAbnormalitySet>(In_Id);
	if (nullptr == abnormalitySet)
	{
		return;
	}

	// 임의 GUID만들기
	bool isSame = abnormalityHandler->IsAbnormalityId(abnormalitySet->id);
	int typeCount = 0;
	if (abnormalitySet->effectIdList.Num() > 0)
	{
		if (const FGsSchemaAbnormalityIndexSet* effectIndexSet = UGsTableUtil::FindRowById<UGsTableAbnormalityIndexSet,
			FGsSchemaAbnormalityIndexSet>(abnormalitySet->effectIdList[0]))
		{
			typeCount = abnormalityHandler->GetAbnormalityEffectType(effectIndexSet->effectType);
		}
	}
	AbnormalityInstanceId guid = (((abnormalitySet->id << 1) + isSame) << 3) + typeCount;
	AbnormalityData newData;
	newData.SetId(abnormalitySet->id);
	newData.SetInstanceId(guid);
	newData.SetLevel(1);
	newData.SetCasterId((int)EGsSkillPreivewGameObjectStartID::OwnerPlayer);
	newData.SetRemainTime(abnormalitySet->durationMax);
	abnormalityHandler->Add(newData);
}

UGsGameObjectBase* UGsSkillPreviewManager::SpawnGameObject(const FSoftObjectPath& In_Path,
	USceneComponent* In_Attach, const FGsSpawnParam* In_SpawnParam)
{
	FString path = In_Path.ToString();
	path += TEXT("_C");
	UClass* bpClass = LoadObject<UClass>(nullptr, *path);

	if (UGsGameObjectSkillPreviewPlayer* object = NewObject<UGsGameObjectSkillPreviewPlayer>(this))
	{
		object->Initialize(In_SpawnParam);

		FCollisionResponseParams responseParams(ECR_Ignore);
		responseParams.CollisionResponse.SetResponse(ECC_WorldStatic, ECR_Block);
		responseParams.CollisionResponse.SetResponse(ECC_WorldDynamic, ECR_Ignore);

		FCollisionQueryParams queryParam;
		FHitResult result;
		FVector start = In_SpawnParam->_pos;
		start.Z -= 80.f;
		FVector end = In_SpawnParam->_pos;
		GetWorld()->LineTraceSingleByChannel(result, start, end, ECollisionChannel::ECC_WorldStatic, queryParam, responseParams);
		AActor* actor = GsSpawn::BPClass(GetWorld(), bpClass, result.Location, In_SpawnParam->_rot);
		if (nullptr == actor)
		{
#if UE_EDITOR
			FString msg = FString::Printf(TEXT("[SkillPreviewError] PreviewPlayer actor Load failed %s"), *path);
			GEngine->AddOnScreenDebugMessage(99, 5.f, FColor::Yellow, msg);
#endif
			return nullptr;
		}

		// 중력값 받지 않게 설정
		ACharacter* characterActor = Cast<ACharacter>(actor);
		UCharacterMovementComponent* moveComponent = characterActor->GetCharacterMovement();
		moveComponent->GravityScale = 0.f;

		object->ActorSpawned(actor);

		//if (FGsCustomizeHandler* customizeHandler = _owner->GetCustomizeHandler())
		//{
		//	// 스킨 정보와 파츠 정보만 동기화
		//	customizeHandler->InitializeActor(_owner->GetCharacter());
		//	customizeHandler->SetGenderType(_currentGenderType);
		//	customizeHandler->SetCustomizeDataBonScale(netUserData->mFaceData, netUserData->mBodyData);
		//	uint32 costumeId = (In_CostumeData) ? In_CostumeData->id : netUserData->mLookInfo.mCostumeId;
		//	customizeHandler->SetCostume(netUserData->mLookInfo.mTransformCostumeId, costumeId, netUserData->mLookInfo.mViewHelmet, false, false, true);
		//}
		
		if (In_Attach)
		{
			actor->AttachToComponent(In_Attach, FAttachmentTransformRules::KeepRelativeTransform);
		}

		return object;
	}

	return nullptr;
}

void UGsSkillPreviewManager::DespawnGameObject(UGsGameObjectBase* In_GameObject)
{
	In_GameObject->Finalize();
	// 액터 소멸
	if (AActor* actor = In_GameObject->GetActor())
	{
		// OnDestroy 델리게이트가 호출될수(프로젝타일형) 있으므로 제거
		actor->OnDestroyed.RemoveAll(this);

		actor->Destroy();
		actor->MarkPendingKill();
	}
	In_GameObject->ClearActor();

	if (In_GameObject->IsRooted())
	{
		In_GameObject->RemoveFromRoot();
	}
	In_GameObject->MarkPendingKill();
}

void UGsSkillPreviewManager::InitializeLocationAndRotation(USceneComponent* In_DummyScene, 
	UGsGameObjectBase* In_GameObject, const FTransform& In_Transform)
{
	In_DummyScene->SetRelativeTransform(In_Transform);
	if (AGsCharacterBase* character = In_GameObject->GetCharacter())
	{
		character->SetActorLocationAndRotation(
			In_DummyScene->GetComponentLocation(),
			In_DummyScene->GetComponentRotation());
	}
}

void UGsSkillPreviewManager::CancelAnimation(UGsGameObjectBase* In_GameObject)
{
	if (UGsAnimInstanceState* anim = In_GameObject->GetAnimInstance())
	{
		if (FAnimMontageInstance* activeMontage = anim->GetActiveMontageInstance())
		{
			anim->StopAllMontages(0.f);
			anim->EndNotifyStates();
		}
	}
}

void UGsSkillPreviewManager::CancelAbnormality(UGsGameObjectBase* In_GameObject)
{
	// 상태이상 정리
	FGsAbnormalityHandlerBase* abnormalityHandler = In_GameObject->GetAbnormalityHandler();
	FGsAbnormalityHandlerSkillPreviewPlayer* castAbnormality = static_cast<
		FGsAbnormalityHandlerSkillPreviewPlayer*>(abnormalityHandler);

	castAbnormality->Cancel();
}

void UGsSkillPreviewManager::DespawnAllGameObject()
{
	if (_owner)
	{
		DespawnGameObject(_owner);
		_owner = nullptr;
	}

	for (UGsGameObjectBase* el : _listPartyPlayer)
	{
		DespawnGameObject(el);
	}
	_listPartyPlayer.Empty();

	for (UGsGameObjectBase* el : _listEnemyPlayer)
	{
		DespawnGameObject(el);
	}
	_listEnemyPlayer.Empty();

	for (UGsGameObjectBase* el : _listProjectile)
	{
		DespawnGameObject(el);
	}
	_listProjectile.Empty();	
}

void UGsSkillPreviewManager::ClearTimer()
{
	// 타이머 진행중인게 있으면 제거
	if (_delayTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_delayTimer);
	}
}

void UGsSkillPreviewManager::OnActorDestroyed(AActor* DestroyedActor)
{
	for (UGsGameObjectBase* el : _listProjectile)
	{
		if (DestroyedActor == el->GetActor())
		{
			el->ActorDespawn();
			if (el->IsRooted())
			{
				el->RemoveFromRoot();
			}
			el->MarkPendingKill();
			_listProjectile.Remove(el);
			break;
		}
	}
}

void UGsSkillPreviewManager::OnCostumeEffectLoadComplate(AActor* In_Actor)
{
	// 날개 채널 반영
	if (In_Actor)
	{
		TArray<UMeshComponent*> findMeshComponents;
		In_Actor->GetComponents<UMeshComponent>(findMeshComponents);
		for (UMeshComponent* el : findMeshComponents)
		{
			el->ChangeLightingChannels(false, true, false);
		}
	}
}

void UGsSkillPreviewManager::ClearSyncVariable()
{
	if (_owner)
	{
		_ownerPosition = _sceneObject->_dummyOwner->GetComponentLocation();
		_ownerDir = _sceneObject->_dummyOwner->GetComponentRotation().Vector();
	}
}

void UGsSkillPreviewManager::GetTargetList(OUT TArray<class UGsGameObjectBase*>& Out_TargetList, int64 In_CasterId,
	const FGsSchemaSkillTargetFilter* In_TargetFilter)
{
	if (In_TargetFilter)
	{
		GetTargetList(Out_TargetList, In_CasterId, In_TargetFilter->iffType);
		if (Out_TargetList.Num() > In_TargetFilter->targetCount)
		{
			Out_TargetList.RemoveAt(In_TargetFilter->targetCount, Out_TargetList.Num() - In_TargetFilter->targetCount);
		}
	}
}

void UGsSkillPreviewManager::GetTargetList(OUT TArray<class UGsGameObjectBase*>& Out_TargetList, int64 In_CasterId,
	IffApplyType In_ApplyType)
{
	switch (In_ApplyType)
	{
	case IffApplyType::SELF:
		Out_TargetList.Emplace(_owner);
		break;
	case IffApplyType::ENEMY:
		Out_TargetList.Append(_listEnemyPlayer);
		break;
	case IffApplyType::PARTY:
	case IffApplyType::ALLY:
		Out_TargetList.Emplace(_owner);
		Out_TargetList.Append(_listPartyPlayer);
		break;
	case IffApplyType::ALL:
		Out_TargetList.Emplace(_owner);
		Out_TargetList.Append(_listEnemyPlayer);
		Out_TargetList.Append(_listPartyPlayer);
		break;
	case IffApplyType::ALL_WITHOUT_SELF:		
		Out_TargetList.Append(_listEnemyPlayer);
		Out_TargetList.Append(_listPartyPlayer);
		break;
	case IffApplyType::GUILD:
		Out_TargetList.Append(_listPartyPlayer);
		break;
	case IffApplyType::ENEMY_WITHOUT_TARGET:
		Out_TargetList.Append(_listEnemyPlayer);
		Out_TargetList.RemoveAll([In_CasterId](UGsGameObjectBase* el)
			{
				return el->GetGameId() == In_CasterId;
			});
		break;
		/*case IffApplyType::SUMMONER:
			break;
		case IffApplyType::SUMMONEE:
			break;*/
	case IffApplyType::MAX:
		break;
	default:
		break;
	}

	Out_TargetList.RemoveAll([](UGsGameObjectBase* el)
		{
			return el->IsHidden();
		});	
}

FRotator UGsSkillPreviewManager::GetObjectRotation(int64 In_GameId)
{
	if (In_GameId < (int)EGsSkillPreivewGameObjectStartID::PartyPlayer)
	{
		int idx = In_GameId - (int)EGsSkillPreivewGameObjectStartID::PartyPlayer;
		if (idx < 2)
		{
			return _partyDummyScene[idx]->GetComponentRotation();
		}
	}
	else if (In_GameId < (int)EGsSkillPreivewGameObjectStartID::EnemyPlayer)
	{
		int idx = In_GameId - (int)EGsSkillPreivewGameObjectStartID::EnemyPlayer;
		if (idx < 3)
		{
			return _enemyDummyScene[idx]->GetComponentRotation();
		}
	}
	else
	{
		if (UGsGameObjectBase* findObject = FindObject(In_GameId))
		{
			return findObject->GetRotation();
		}
	}

	return FRotator::ZeroRotator;
}

UGsGameObjectBase* UGsSkillPreviewManager::FindObject(int64 In_GameId)
{
	if (In_GameId < (int)EGsSkillPreivewGameObjectStartID::PartyPlayer)
	{
		return _owner;
	}
	else if (In_GameId < (int)EGsSkillPreivewGameObjectStartID::EnemyPlayer)
	{
		return FindObject(_listPartyPlayer, In_GameId);
	}
	else if (In_GameId < (int)EGsSkillPreivewGameObjectStartID::Projectile)
	{
		return FindObject(_listEnemyPlayer, In_GameId);
	}
	else
	{
		return FindObject(_listProjectile, In_GameId);
	}
	return nullptr;
}

UGsGameObjectBase* UGsSkillPreviewManager::FindObject(TArray<UGsGameObjectBase*>& In_FindList, int64 In_GameId)
{
	if (UGsGameObjectBase** findObject = In_FindList.FindByPredicate([In_GameId](const UGsGameObjectBase* el)
		{ return el->GetGameId() == In_GameId;}))
	{
		return *findObject;
	}
	return nullptr;
}

UCameraComponent* UGsSkillPreviewManager::GetSceneCamera() const
{
	if (_sceneObject)
	{
		return _sceneObject->GetCameraComponent();
	}
	return nullptr;
}

UGsGameObjectBase* UGsSkillPreviewManager::FindObjectByActor(class AActor* In_FindActor)
{
	if (_owner && _owner->GetActor() == In_FindActor)
	{
		return _owner;
	}

	for (UGsGameObjectBase* el : _listPartyPlayer)
	{
		if (el->GetActor() == In_FindActor)
		{
			return el;
		}
	}

	for (UGsGameObjectBase* el : _listEnemyPlayer)
	{
		if (el->GetActor() == In_FindActor)
		{
			return el;
		}
	}

	for (UGsGameObjectBase* el : _listProjectile)
	{
		if (el->GetActor() == In_FindActor)
		{
			return el;
		}
	}

	return nullptr;
}

// 네트워크 패킷대응 동기화용 함수 처리
void UGsSkillPreviewManager::SetOwnerPostion(const FVector& In_Pos)
{
	_ownerPosition = In_Pos;
}

void UGsSkillPreviewManager::SetOwnerDirection(const FVector& In_Dir)
{
	_ownerDir = In_Dir;
}

const FVector& UGsSkillPreviewManager::GetOwnerPostion() const
{
	return _ownerPosition;
}

const FVector& UGsSkillPreviewManager::GetOwnerDirection() const
{
	return _ownerDir;
}

SkillStartType UGsSkillPreviewManager::GetSkillStartType(const FGsSchemaSkillSet* In_SkillData) const
{
	if (In_SkillData->category == SkillCategory::CAST)
	{
		return SkillStartType::CAST;
	}
	return SkillStartType::SKILL;
}