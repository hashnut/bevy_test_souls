// Fill out your copyright notice in the Description page of Project Settings.

#include "GameObject/ObjectClass/GsGameObjectSkillPreviewPlayer.h"

#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsHitEffectComponent.h"

#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/Define/GsGameObjectText.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Stat/GsStatRemotePlayer.h"
#include "GameObject/Skill/GsSkillHandlerSkillPreviewPlayer.h"
#include "GameObject/Movement/GsMovementHandlerRemote.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Parts/GsPartsHandlerPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerSkillPreviewPlayer.h"
#include "GameObject/State/RemotePlayer/GsStateRemotePlayerIdle.h"

#include "Management/ScopeGame/GsSkillPreviewManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "DataSchema/Skill/HitEffect/GsSchemaHealByTypeEffectSet.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityEffectUtilSet.h"
#include "Data/GsDataContainManager.h"

AActor* UGsGameObjectSkillPreviewPlayer::GetActor() const { return _actor; }
AGsCharacterBase* UGsGameObjectSkillPreviewPlayer::GetCharacter() const { return _actor; }
void	UGsGameObjectSkillPreviewPlayer::ClearActor() { _actor = nullptr; }

void UGsGameObjectSkillPreviewPlayer::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

	// User 
	if (SpawnParam->_gameId < (int)EGsSkillPreivewGameObjectStartID::EnemyPlayer)
	{
		// CommonInfo테이블 설정
		_creatureCommonInfo = UGsTableUtil::FindRowById<UGsTableCreatureCommonInfo, FGsSchemaCreatureCommonInfo>(SpawnParam->_tId);

		// 히트 이펙트 설정
		// 현재는 FGsSchemaUserData Rowname과 CreatureCommonInfo Id가 같지만 달라질 경우에는 다르게 처리되어야 함
		const FGsSchemaUserData* userData = UGsTableUtil::FindRowById<UGsTableUserData, FGsSchemaUserData>(SpawnParam->_tId);
		HitEffectDataOverride = userData->HitEffectDataOverride == nullptr ?
			GData()->GetGlobalData()->HitEffectData : userData->HitEffectDataOverride;
		HitSoundDataOverride = userData->HitSoundDataOverride.IsNull() ?
			GData()->GetGlobalData()->HitSoundData.GetRow() : userData->HitSoundDataOverride.GetRow();
	}
	else
	{
		//NonPlayer 데이터 얻기
		const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(SpawnParam->_tId);
		if (npcData)
		{
			_creatureCommonInfo = npcData->commonInfoId.GetRow();
			
			HitEffectDataOverride  = _creatureCommonInfo->hitEffectDataOverride == nullptr ?
				GData()->GetGlobalData()->HitEffectData : _creatureCommonInfo->hitEffectDataOverride;
			HitSoundDataOverride = _creatureCommonInfo->hitSoundDataOverride.IsNull() ?
				GData()->GetGlobalData()->HitSoundData.GetRow() : _creatureCommonInfo->hitSoundDataOverride.GetRow();
		}
	}

	//bak1210 : final 에서호출
	CreateSubClass();
}

const FGsSchemaShapeData* UGsGameObjectSkillPreviewPlayer::GetPolymorphShapeData()
{
	FGsGameObjectData* gameData = GetData();
	int64 objectType = gameData->GetGameId();
	if (objectType >= (int)EGsSkillPreivewGameObjectStartID::EnemyPlayer &&
		objectType < (int)EGsSkillPreivewGameObjectStartID::Projectile)
	{
		const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(gameData->GetTableId());
		if (npcData)
		{
			return npcData->polymorpShapeId.GetRow();
		}
	}
	return nullptr;
}

void UGsGameObjectSkillPreviewPlayer::InitializeActor(AActor* Owner)
{
	// 액터 저장
	_actor = Cast<AGsCharacterBase>(Owner);

	Super::InitializeActor(Owner);

	// 배틀아이들 고정
	FGsGameObjectDataCreature* data = GetCastData<FGsGameObjectDataCreature>();
	data->SetUserStatus(UserStatus::USER_STATE_COMBAT);
	if (UGsAnimInstanceState* anim = GetAnimInstance())
	{
		anim->SetBattleIdleType(true);
	}

	// 히트 이펙트 기본 데이터 설정
	if (AGsCharacterBase* characterActor = GetCharacter())
	{
		if (UGsHitEffectComponent* hitComp = characterActor->GetHitEffect())
		{
			hitComp->OverrideData(HitEffectDataOverride, HitSoundDataOverride, 0.f, 0.f);
		}
	}

	// Npc Hit 모션 로드
	FGsGameObjectData* gameData = GetData();
	if (gameData->GetGameId() >= (int)EGsSkillPreivewGameObjectStartID::EnemyPlayer &&
		gameData->GetGameId() < (int)EGsSkillPreivewGameObjectStartID::Projectile)
	{
		const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(gameData->GetTableId());
		if (npcData)
		{
			const FGsSchemaShapeData* shape = npcData->shapeId.GetRow();
			if (nullptr != shape)
			{
				if (_actor->LoadHitAnimInstance(shape->hitAnimInstancePath))
				{
					LegacyHit = false;
				}
			}
		}
	}
}

void UGsGameObjectSkillPreviewPlayer::CreateSubClass()
{
	Super::CreateSubClass();
	

	AppendInitialize(CreateSkill());
	AppendInitialize(CreateMovement());
	AppendInitialize(CreateCustomize());
	
	GetMovementHandler()->InitSpeed(400, 1);	
}

TSharedPtr<FGsMovementHandlerBase> UGsGameObjectSkillPreviewPlayer::CreateMovement()
{
	return MakeShareable(new FGsMovementHandlerRemote());
}

TSharedPtr<FGsPartsHandlerBase> UGsGameObjectSkillPreviewPlayer::CreateParts()
{
	return MakeShareable(new FGsPartsHandlerPlayer());
}

TSharedPtr<FGsSkillHandlerBase> UGsGameObjectSkillPreviewPlayer::CreateSkill()
{
	return MakeShareable(new FGsSkillHandlerSkillPreviewPlayer());
}

TSharedPtr<FGsAbnormalityHandlerBase> UGsGameObjectSkillPreviewPlayer::CreateAbnormality()
{
	return MakeShareable(new FGsAbnormalityHandlerSkillPreviewPlayer());
}

TSharedPtr<FGsStatBase> UGsGameObjectSkillPreviewPlayer::CreateCreatureStat()
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

void UGsGameObjectSkillPreviewPlayer::InitializeFSM()
{
	// FSM생성 초기화
	if (_fsm)
	{
		_fsm->Initialize(BPStateMachineName, this, FGsStateRemotePlayerIdle::GetInstance());
	}
}

void UGsGameObjectSkillPreviewPlayer::NetSkillHealAck(PD::SC::PKT_SC_SKILL_HEALED_READ* Packet)
{
	UGsGameObjectBase* casterObject = GSSkillPreview()->FindObject(Packet->CasterId());
	// 데미지 정보는 출력할 필요없으므로 설정 하지 않음
	OnHit(casterObject, nullptr, Packet->SkillId(), 0, HitResultType::HEAL, DamageByType::SKILL, Packet->HealStatType(),
		Packet->HealByType(), Packet->HpDeltaValue());
}

void UGsGameObjectSkillPreviewPlayer::NetSkillDamagedAck(PD::SC::PKT_SC_SKILL_DAMAGED_READ* Packet)
{
	UGsGameObjectBase* casterObject = GSSkillPreview()->FindObject(Packet->CasterId());
	UGsGameObjectBase* subCasterObject = nullptr;
	// 프로젝타일 처리(mMediumId 값이 있으면 프로젝타일 정보)
	if (Packet->MediumId() != 0)
	{
		subCasterObject = GSSkillPreview()->FindObject(Packet->MediumId());
	}

	// 데미지 정보는 출력할 필요없으므로 설정 하지 않음
	DamageStatType damageStatType = DamageStatType::MAX;
	switch (Packet->HitResultType())
	{
	case HitResultType::HEAL:
		damageStatType = DamageStatType::HP;
		break;
	default:
		break;
	}

	OnHit(casterObject, subCasterObject, Packet->SkillId(), Packet->SkillNotifyId(),
		Packet->HitResultType(), Packet->DamageByType(), damageStatType,
		HealByType::MAX, Packet->TotalDamage());

	// 특정 상태이상 조건 확인
	if (FGsAbnormalityHandlerBase* abnormalityHandler = GetAbnormalityHandler())
	{
		if (abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::UTIL_ON_HIT_SKILL_NOTIFY))
		{
			TArray<FGsAbnormalityData*> outAbnormalityList;
			abnormalityHandler->GetAbnormalityDataList(outAbnormalityList, false);
			for (FGsAbnormalityData* el : outAbnormalityList)
			{
				const FGsSchemaAbnormalityIndexSet* indexSet =
					el->GetEnableAbnormalityEffectByEffectType(AbnormalityEffectType::UTIL_ON_HIT_SKILL_NOTIFY);
				if (nullptr == indexSet)
				{
					continue;
				}

				if (indexSet->levelList.Num() < 2)
				{
					continue;
				}

				const FGsSchemaAbnormalityEffectUtilSet* utilSet =
					indexSet->levelList[1].effectRow.GetRow<FGsSchemaAbnormalityEffectUtilSet>();
				if (nullptr == utilSet)
				{
					continue;
				}

				const FGsSchemaSkillNotifySet* skillNotifySet = utilSet->param.SkillNoityId.GetRow();
				if (nullptr == skillNotifySet)
				{
					continue;
				}

				GSSkillPreview()->ExecuteNotify(0, GetGameId(), GetGameId(), skillNotifySet, 0);
			}
		}
	}
}