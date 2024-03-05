// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAbnormalityHandlerBase.h"

#include "Shared/Client/SharedEnums/SharedStatEnum.h"
//#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsEffectComponent.h"

#include "CrowdControl/GsCrowdControlBase.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectPlayer.h"

#include "Sound/GsSoundPlayer.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/GsMessageHolder.h"

#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Abnormality/AbnormalityConvert/GsSchemaAbnormalityIndexSet.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityStatSet.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityStat.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityCrowdControlSet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"

#include "Util/GsTableUtil.h"
#include "UTIL/GsGameObjectUtil.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Algo/ForEach.h"
#include "Algo/AnyOf.h"
#include "../ActorEx/GsCharacterPlayer.h"
#include "Copy.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"

bool FGsAbnormalityHandlerBase::IsAbnormalityEffectType(AbnormalityEffectType FindType,
	const FGsSchemaAbnormalitySet* AbnormalitySetTable)
{
	if (nullptr == AbnormalitySetTable)
	{
		return false;
	}

	for (int32 effectId : AbnormalitySetTable->effectIdList)
	{
		const FGsSchemaAbnormalityIndexSet* effectIndexSet =
			UGsTableUtil::FindRowById<UGsTableAbnormalityIndexSet, FGsSchemaAbnormalityIndexSet>(effectId);
		if (nullptr == effectIndexSet)
		{
			continue;
		}
		if (FindType == effectIndexSet->effectType)
		{
			return true;
		}
	}
	return false;
}

void FGsAbnormalityHandlerBase::Initialize(UGsGameObjectBase* Owner)
{
	_owner = Owner;
	ClearTypeCount();

	if (_crowdControl == nullptr)
	{
		_crowdControl = CreateCrowdControl();
		_crowdControl->Initialize(Owner);
	}
}

void FGsAbnormalityHandlerBase::Finalize()
{
	StopSound(0);

	_abnormalityDebuffList.Empty();
	_mapAbnormality.Reset();
	_listMaterialChange.Reset();
	_materialAppliedAbnormality = nullptr;
	ClearTypeCount();

	if (_crowdControl)
	{
		_crowdControl->Finalize();
		delete _crowdControl;
		_crowdControl = nullptr;
	}
}

void FGsAbnormalityHandlerBase::InitializeActor(class AActor* Owner)
{
	_materialAppliedAbnormality = nullptr;
	UpdateMaterial();
	UpdateEffectScale();
}

void FGsAbnormalityHandlerBase::Update(float Delta)
{
	IGsGameObjectHandler::Update(Delta);

	CheckTeamChangedInHidden();
}

void FGsAbnormalityHandlerBase::CheckTeamChangedInHidden()
{
	if(_materialAppliedAbnormality.IsValid() && _materialAppliedAbnormality.Pin()->IsHidingType())
	{
		auto abnormality = _materialAppliedAbnormality.Pin();
		if (abnormality && abnormality->IsHidingType())
		{
			if (auto castObject = _owner->CastGameObject<UGsGameObjectCreature>())
			{
				if (castObject->IsHidden() && IsTeamMate())
				{
					SetVisibleOwner(castObject, true);
					castObject->OnOverlayMaterialChange(abnormality->GetMaterialData());
				}
				else if (!castObject->IsHidden() && !IsTeamMate())
				{
					castObject->OnOverlayMaterialChange(nullptr);
					SetVisibleOwner(castObject, false);
				}
			}
		}
	}
}

FGsCrowdControlBase* FGsAbnormalityHandlerBase::CreateCrowdControl()
{
	return new FGsCrowdControlBase();
}

void FGsAbnormalityHandlerBase::Set(PD::SC::PKT_SC_ABNORMALITY_READ* Packet)
{
	for (PD::SC::PKT_SC_ABNORMALITY_READ::AbnormalityListIterator it = Packet->GetFirstAbnormalityListIterator();
		it != Packet->GetLastAbnormalityListIterator(); ++it)
	{
		Add(it->ConstRefAbnormalityData(), false);
	}
}

bool FGsAbnormalityHandlerBase::EmplaceExecuteData(const AbnormalityData& NetData) //, FGsAbnormalityData*& executeData)
{
	auto iter = _mapAbnormality.Find(NetData.GetInstanceId());
	TSharedPtr<FGsAbnormalityData> executeData = iter == nullptr ? nullptr : *iter;
		// 다른 소모품 사용으로 인한 기존 효과 갱신 시(음식: 1개 적용), 기존 정보 제거
	if (executeData && (executeData->GetTableId() != NetData.GetId()))
	{
		Remove((NetData.GetInstanceId()));
		executeData = nullptr;
	}

	if (executeData == nullptr)
	{
		executeData = MakeShared<FGsAbnormalityData>(UGsTableUtil::FindRowById<UGsTableAbnormalitySet, FGsSchemaAbnormalitySet>(NetData.GetId()), NetData, 0);
		if (nullptr == executeData->GetTable())
		{
			GSLOG(Error, TEXT("not found AbnormalitySet Data Table!"));
			return false;
		}
		_mapAbnormality.Emplace(NetData.GetInstanceId(), executeData);
		return true;
	}
	executeData->UpdateAbnormalityInfo(NetData, 0);
	return false;
}

bool FGsAbnormalityHandlerBase::EmplaceMaterialChangeData(TSharedPtr<FGsAbnormalityData> executeData) //AbnormalityEffectType subEffectType)
{
	if (executeData->IsMaterialChangedType())
	{
		//같은 abnormality여도 material 순서는 우선
		_listMaterialChange.Remove(executeData);
		_listMaterialChange.Add(executeData);

		_listMaterialChange.StableSort([](auto le, auto re) { 
				auto lePriority = le.IsValid() ? le.Pin()->GetMaterialPriority() : -1;
				auto rePriority = re.IsValid() ? re.Pin()->GetMaterialPriority() : -1;
				return lePriority < rePriority; 
			});
	}
	return _listMaterialChange.Num() > 0 && _materialAppliedAbnormality != _listMaterialChange.Last();
}

void FGsAbnormalityHandlerBase::Add(const AbnormalityData& NetData, bool isRefresh)
{
	GSLOG(Log, TEXT(" ||*********** FGsAbnormalityHandlerBase::Add [%d]"), NetData.GetId());

	TArray<AbnormalityEffectType> firstTimeApplyEffects;

	bool isNewData = EmplaceExecuteData(NetData);
	if (auto iter = _mapAbnormality.Find(NetData.GetInstanceId()))
	{
		TSharedPtr<FGsAbnormalityData> executeData = *iter;
		bool isAnimationChangeEffect = executeData->IsAnimtionControlType();

		if (isNewData)
		{
			TArray<AbnormalityEffectType> effects;
			executeData->GetAllEffectType(effects);
			Algo::ForEach(effects, [&firstTimeApplyEffects, this](auto subEffectType){ 
					_typeCount[subEffectType]++;
					if (_typeCount[subEffectType] == 1)
					{
						firstTimeApplyEffects.Add(subEffectType);
					}
				});

			// 디버프만 따로 저장(HUD 표시 위함)
			if (false == executeData->GetTable()->isBuff)
			{
				_abnormalityDebuffList.AddUnique(NetData.GetInstanceId());
			}

			if (isAnimationChangeEffect) // CC기 처리
			{
				TArray<EGsAbnormalityAnimProcessType> animTypes;
				executeData->GetAnimationControlTypeList(animTypes);
				Algo::ForEach(animTypes, [this](auto animType) { _crowdControl->ChangeReactionState(animType); });//막으시면 안됩니다
			}
		}

		if (firstTimeApplyEffects.Num() > 0)
		{
			Algo::ForEach(firstTimeApplyEffects, [this](auto subEffectType) { UpdateAbnormalityEffectType(subEffectType, true); });
		}
		
		if (isAnimationChangeEffect) // CC기 처리
		{
			_crowdControl->UpdateReactionControl(_mapAbnormality); 	// CC기 동기화
		}

		// https://jira.com2us.com/jira/browse/CHR-13643
		// Hiding 발생시의 effect 출력을 위해 앞으로 당김
		OnActive(NetData.GetInstanceId(), executeData, isNewData, isRefresh); 

		if (isRefresh && isNewData && IsTargetedAvailableState() == false)
		{
			FGsGameObjectMessageParam param(_owner);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::COMMON_TARGET_CLEAR, &param);
		}

		FGsGameObjectMessageParamAbnormality param(_owner, NetData.GetInstanceId(), false, executeData->GetEffectType());
		GMessage()->GetGameObject().SendMessage(MessageGameObject::ABNORMALITY_ALL_UPDATE, &param);

		// 블루 프린트 통신
		UGsGameObjectCreature* castObject = _owner->CastGameObject<UGsGameObjectCreature>();
		if (AGsCharacterBase* charactorActor = castObject->GetCharacter())
		{
			charactorActor->ReceiveAddAbnormality(executeData->GetTable()->id);
		}
	}
}

void FGsAbnormalityHandlerBase::Remove(AbnormalityInstanceId InstanceId)
{
	if (auto iter = _mapAbnormality.Find(InstanceId))
	{
		auto findData = *iter;
		bool isMaterialChangeEffect = findData->IsMaterialChangedType();
		bool isAnimationChangeEffect = findData->IsAnimtionControlType();

		GSLOG(Log, TEXT(" ||*********** FGsAbnormalityHandlerBase::Remove [%d]"), findData->GetTableId());

		TArray<AbnormalityEffectType> noneApplyEffects;

		TArray<AbnormalityEffectType> effects;
		findData->GetAllEffectType(effects);
		// Type Count 갱신
		Algo::ForEach(effects, [&noneApplyEffects, this](auto subEffectType) {
				_typeCount[subEffectType]--;
				if (_typeCount[subEffectType] == 0)
				{
					noneApplyEffects.Add(subEffectType);
				}
			});

		if (false == findData->GetTable()->isBuff)
		{
			_abnormalityDebuffList.Remove(InstanceId);
		}

		if (noneApplyEffects.Num() > 0)
		{
			Algo::ForEach(noneApplyEffects, [this](auto subEffectType) { UpdateAbnormalityEffectType(subEffectType, false); });
		}

		// 블루 프린트 통신
		UGsGameObjectCreature* castObject = _owner->CastGameObject<UGsGameObjectCreature>();
		if (AGsCharacterBase* charactorActor = castObject->GetCharacter())
		{
			charactorActor->ReceiveRemoveAbnormality(findData->GetTable()->id);
		}

		// 주의: OnDeActive 순서에 유의. _mapAbnormality.Remove 전에 불려야 합니다.
		// FGsAbnormalityHandlerLocalPlayer의 LOCAL_ABNORMALITY_REMOVE 일 경우, 
		// instanceId로 객체 데이터 얻어가 참조하는 경우가 있음
		OnDeActive(InstanceId, findData); // 비 활성화 despawn effect

		_mapAbnormality.Remove(InstanceId);
		_crowdControl->UpdateReactionControl(_mapAbnormality); // CC기 동기화		

		// [B.Y] R4 개선 예정 현재 임시 처리 진행
		// https://jira.com2us.com/jira/browse/CHR-13643

		FGsGameObjectMessageParamAbnormality param(_owner, InstanceId, true, findData->GetEffectType());
		GMessage()->GetGameObject().SendMessage(MessageGameObject::ABNORMALITY_ALL_UPDATE, &param);

	}
}

void FGsAbnormalityHandlerBase::UpdateTypeCount()
{
	ClearTypeCount();

	for (const auto& pair : _mapAbnormality)
	{
		TArray<AbnormalityEffectType> effects;
		pair.Value->GetAllEffectType(effects);

		Algo::ForEach(effects, [this](auto subEffectType) { _typeCount[subEffectType]++; });
	}
}

void FGsAbnormalityHandlerBase::ClearTypeCount()
{
	for (int effectType = int(AbnormalityEffectType::NONE); effectType < int(AbnormalityEffectType::MAX); ++effectType)
	{
		_typeCount[AbnormalityEffectType(effectType)] = 0;
	}
}

void FGsAbnormalityHandlerBase::OnActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData, bool IsNewDataAdded, bool isSpawnEffect)
{
	auto executeData = inData.Pin();
	// 추후 구현에 맞게 처리
	if (nullptr == executeData || nullptr == executeData->GetTable())
	{
		return;
	}

	TArray<AbnormalityEffectType> effects;
	if (executeData->GetAllEffectType(effects) > 0 && effects.Find(AbnormalityEffectType::CC_POLYMORP) != INDEX_NONE)
	{
		if (UGsAnimInstanceState* anim = _owner->GetAnimInstance())
		{
			if (anim->IsPoseAnimation) //변이 전에 풀어두어야 함
			{
				anim->Resume();
			}

			if(auto castObject = _owner->CastGameObject<UGsGameObjectCreature>())
			{
				castObject->OnPolymorpChange();
			}
		}
	}

	bool isMaterialChangeEffect = executeData->IsMaterialChangedType();
	bool isHiddenEffect = effects.Find(AbnormalityEffectType::UTIL_HIDING) != INDEX_NONE && !IsTeamMate(); //처음 적용
	bool isHiddenApplied = !isMaterialChangeEffect && IsMaterialEffectApplied(AbnormalityEffectType::UTIL_HIDING) && _owner->IsHidden(); //이미 적용되어 있음

	if (isMaterialChangeEffect && EmplaceMaterialChangeData(executeData))  
		UpdateMaterial(_listMaterialChange.Last());


	// 대표 이펙트 출력했는지
	bool isPlayedPresentEffect = false;
	const FGsSchemaAbnormalitySet* abnormalitySet = executeData->GetTable();
	auto effectKey = executeData->GetTable()->id;

	if (AGsCharacterBase* character = _owner->GetCharacter())
	{
		UGsEffectComponent* effComp = character->GetEffectComponent();
		if (effComp && isSpawnEffect) effComp->OnAllPresentAbnormalityEffectsDeactive(effectKey);

		for (const FGsSchemaAbnormalityEffect& el : abnormalitySet->effectInfo)
		{
			if (el.particleSpawnType == EGsAbnormalityEffectParticleSpawnType::DeactiveSpawn)
			{
				continue;
			}

			if (isSpawnEffect == false && el.particleSpawnType != EGsAbnormalityEffectParticleSpawnType::LoopSpawn)
			{
				continue;
			}


			UParticleSystem* particleEffect = el.particleEffect;
			FName socketName = el.socketName;
			FVector posEffect = el.posOffset;
			EGsAbnormalityEffectParticleType attachType = el.emitterAttachType;

			// 디버프 걸림 연출 시작(대표)
			if (effComp != nullptr && particleEffect != nullptr)
			{
				EPSCPoolMethod poolType = el.particleSpawnType == EGsAbnormalityEffectParticleSpawnType::LoopSpawn ?
					EPSCPoolMethod::ManualRelease : EPSCPoolMethod::AutoRelease;
				effComp->OnPresentAbnormalityEffectActive(effectKey, particleEffect, attachType, socketName, posEffect,
					_abnormalityEffectScale, poolType);
				// 대표 이펙트 출력함
				isPlayedPresentEffect = true;
			}

			// 사운드 재생
			const FGsSchemaSoundResData* schemaSoundResData = el.soundRes.GetRow();
			if (!isHiddenApplied && schemaSoundResData != nullptr && isSpawnEffect)
			{
				PlaySound(schemaSoundResData, InstanceId);
			}
		}

		for (int32 effectId : abnormalitySet->effectIdList)
		{
			const FGsSchemaAbnormalityIndexSet* effectIndexSet =
				UGsTableUtil::FindRowById<UGsTableAbnormalityIndexSet, FGsSchemaAbnormalityIndexSet>(effectId);
			if (nullptr == effectIndexSet)
			{
				continue;
			}

			if (effectIndexSet->levelList.Num() <= executeData->GetLevel())
			{
				continue;
			}


		}
	}

}

void FGsAbnormalityHandlerBase::UpdateAbnormalityEffectType(AbnormalityEffectType EffectType, bool IsAdd)
{
	switch (EffectType)
	{
	case AbnormalityEffectType::CC_BIND:
	{
		if (UGsGameObjectCreature* castObject = _owner->CastGameObject<UGsGameObjectCreature>())
		{
			castObject->SetRootmotionLock(IsAdd);
		}
	}
	break;
	case AbnormalityEffectType::UTIL_MOUNT_VEHICLE:
	{
		if (UGsGameObjectCreature* castObject = _owner->CastGameObject<UGsGameObjectCreature>())
		{
			if (IsAdd)
			{
				castObject->OnVehicleEffect(true);
			}
			else
			{
				castObject->OnRestoreVehicleEffect();
			}
		}
	}
	break;
	default:
		break;
	}
}

void FGsAbnormalityHandlerBase::OnDeActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData)
{
	auto removeData = inData.Pin();
	// 추후 구현에 맞게 처리
	if (nullptr == removeData || nullptr == removeData->GetTable())
	{
		return;
	}

	bool isMaterialChangeEffect = removeData->IsMaterialChangedType();
	if (isMaterialChangeEffect)
	{
		_listMaterialChange.Remove(removeData);
		UpdateMaterial(_listMaterialChange.IsValidIndex(0) ? _listMaterialChange.Last().Pin() : nullptr);
	}

	TMap<AbnormalityInstanceId, TWeakPtr<FGsAbnormalityData>> remainAbnormality;
	Algo::CopyIf(_mapAbnormality, remainAbnormality, [inData](auto e) { return e.Value != inData.Pin();});
	
	TArray<AbnormalityEffectType> effects;
	removeData->GetAllEffectType(effects);
	if (effects.Find(AbnormalityEffectType::CC_POLYMORP) != INDEX_NONE)
	{	
		if (Algo::FindByPredicate(remainAbnormality, [](auto& pair) {
				return pair.Value.IsValid() && pair.Value.Pin()->IsEnableAbnormalityEffectType(AbnormalityEffectType::CC_POLYMORP);
			}) == nullptr)
		{
			if (auto castObject = _owner->CastGameObject<UGsGameObjectCreature>())
			{
				castObject->OnRestorePolymorph();
			}
		}
	}
	bool isHiddenEffect = effects.Find(AbnormalityEffectType::UTIL_HIDING) != INDEX_NONE && !IsTeamMate();

	// 대표 이펙트 출력했는지
	bool isPlayedPresentEffect = false;
	bool isPlaySound = false;
	UGsEffectComponent* effComp = nullptr;

	const FGsSchemaAbnormalitySet* abnormalitySet = removeData->GetTable();
	if (AGsCharacterBase* character = _owner->GetCharacter())
	{
		auto effectKey = removeData->GetTable()->id;

		effComp = character->GetEffectComponent();
		if (effComp) effComp->OnAllPresentAbnormalityEffectsDeactive(effectKey);

		// 해제시 발생하는 이펙트 출력
		for (const FGsSchemaAbnormalityEffect& el : abnormalitySet->effectInfo)
		{
			if (el.soundRes.GetRow() != nullptr)
			{
				isPlaySound = true;
			}

			UParticleSystem* particleEffect = el.particleEffect;
			if (nullptr == particleEffect)
			{
				continue;
			}
			isPlayedPresentEffect = true;

			if (el.particleSpawnType != EGsAbnormalityEffectParticleSpawnType::DeactiveSpawn)
			{
				continue;
			}

			FName socketName = el.socketName;
			FVector posEffect = el.posOffset;
			EGsAbnormalityEffectParticleType attachType = el.emitterAttachType;
			if(effComp) effComp->OnPresentAbnormalityEffectActive(effectKey, particleEffect, attachType, socketName, posEffect,
				_abnormalityEffectScale, EPSCPoolMethod::AutoRelease);
			// 사운드 재생
			const FGsSchemaSoundResData* schemaSoundResData = el.soundRes.GetRow();
			if (schemaSoundResData != nullptr)
			{
				PlaySound(schemaSoundResData, InstanceId);
			}
		}

		if (isPlaySound) StopSound(InstanceId);
		
		if (isPlayedPresentEffect == false && effComp)
		{
			auto level = removeData->GetLevel();
			TArray<const FGsSchemaAbnormalityStatSet*> statAbnormalities;
			Algo::TransformIf(abnormalitySet->effectIdList, statAbnormalities, [level](auto effectId) {

				if (const auto effectIndexSet = UGsTableUtil::FindRowById<UGsTableAbnormalityIndexSet, FGsSchemaAbnormalityIndexSet>(effectId))
				{
					return effectIndexSet->levelList.Num() > level && effectIndexSet->effectType == AbnormalityEffectType::STAT && 
						  !effectIndexSet->levelList[level].effectRow.IsNull();
				}
				return false;

				}, [level](auto effectId) {
					const auto effectIndexSet = UGsTableUtil::FindRowById<UGsTableAbnormalityIndexSet, FGsSchemaAbnormalityIndexSet>(effectId);
					return effectIndexSet->levelList[level].effectRow.template GetRow<FGsSchemaAbnormalityStatSet>();
				});

			statAbnormalities.RemoveAll([](const auto e) { return e == nullptr; });

		}
	}
}

bool FGsAbnormalityHandlerBase::IsMaterialEffectApplied(AbnormalityEffectType effectType)
{
	return _listMaterialChange.FindByPredicate([effectType](auto e) { 
			return e.IsValid() && e.Pin()->IsEnableAbnormalityEffectType(effectType);
		}) != nullptr;
}

//void FGsAbnormalityHandlerBase::UpdateMaterial_Temp()
//{
//	//Hiding이 다른 meterial 변화 상태로 인해 풀리면 안됨. 
//	//if (IsAbnormalityEffectType(AbnormalityEffectType::UTIL_HIDING)) // 여기서 체크하면 hiding 이 실행안됨
//	//	return;
//
//	// 적용해야하는 정보
//	auto changeMaterialData = _listMaterialChange.Num() > 0 ? _listMaterialChange.Last().Pin() : nullptr;
//	// 현재 적용중이면 생략
//	if (_materialAppliedAbnormality,IsValid() && changeMaterialData && changeMaterialData->GetInstanceId())
//	{
//		return;
//	}
//
//	UGsGameObjectCreature* castObject = _owner->CastGameObject<UGsGameObjectCreature>();
//	// 머터리얼 효과 리스토어
//	if (changeMaterialData)
//	{
//		// AbnormalityEffectType::UTIL_HIDING 타입 예외처리
//		if (changeMaterialData->GetEffectType() == AbnormalityEffectType::UTIL_HIDING)
//		{
//			if (IsTeamMate())
//			{
//				castObject->OnMaterialChange(changeMaterialData->GetTable()->marterialInfo.objectData);
//			}
//			else
//			{
//				SetVisibleOwner(castObject, false);
//			}
//		}
//		else
//		{
//			castObject->OnMaterialChange(changeMaterialData->GetTable()->marterialInfo.objectData);
//		}
//
//		_currentMaterialId = changeMaterialData->GetInstanceId();
//		return;
//	}
//
//	_currentMaterialId = 0;
//	AGsCharacterBase* characterActor = castObject->GetCharacter();
//	// Restore
//	if (characterActor && characterActor->IsMaterialChanged())
//	{
//		// 사망(Dead)중일 경우에는 해당 효과를 restore하지 않는다.
//		if (false == castObject->IsDeadState())
//		{
//			// restore
//			castObject->OnMaterialChange(nullptr);
//		}
//	}
//	else if (castObject->IsHidden())
//	{
//		SetVisibleOwner(castObject, true);
//	}
//}

void FGsAbnormalityHandlerBase::UpdateMaterial(TWeakPtr<FGsAbnormalityData> updateData)
{
	if (updateData == _materialAppliedAbnormality) 
		return;

	auto preEffectType = _materialAppliedAbnormality.IsValid() && _materialAppliedAbnormality.Pin()->IsMaterialChangedType() ? 
		_materialAppliedAbnormality.Pin()->GetEffectType() : AbnormalityEffectType::NONE;
	auto newEffectType = updateData.IsValid() && updateData.Pin()->IsMaterialChangedType() ? updateData.Pin()->GetEffectType() : AbnormalityEffectType::NONE;

	bool isPreHidingType = _materialAppliedAbnormality.IsValid() ? _materialAppliedAbnormality.Pin()->IsHidingType() : false;
	bool isNewHidingType = updateData.IsValid() ? updateData.Pin()->IsHidingType() : false;

	UGsGameObjectCreature* castObject = _owner->CastGameObject<UGsGameObjectCreature>();
	AGsCharacterBase* characterActor = castObject->GetCharacter();

	if (preEffectType != newEffectType)
	{
		if (isPreHidingType && castObject->IsHidden())
		{
			SetVisibleOwner(castObject, true);
		}
		else if (isNewHidingType && !IsTeamMate())
		{
			SetVisibleOwner(castObject, false);
		}
		
		if (newEffectType == AbnormalityEffectType::NONE)
		{
			_materialAppliedAbnormality = nullptr;
			castObject->OnOverlayMaterialChange(nullptr);
		}
	}

	if (newEffectType != AbnormalityEffectType::NONE)
	{
		_materialAppliedAbnormality = updateData;

		if (isNewHidingType && !IsTeamMate()) return;
		castObject->OnOverlayMaterialChange(updateData.Pin()->GetMaterialData());
	}
}

void FGsAbnormalityHandlerBase::UpdateEffectScale()
{
	// 이펙트 스케일 비율 설정
	bool isOverride = false;
	if (const FGsSchemaCreatureCommonInfo* commonInfo = _owner->GetCommonInfoTable())
	{
		if (false == FMath::IsNearlyZero(commonInfo->abnormalityParticleSizeRate))
		{
			_abnormalityEffectScale = FVector(commonInfo->abnormalityParticleSizeRate);
			isOverride = true;
		}
	}

	if (false == isOverride)
	{
		// FGsGameObjectData 설정된 정보로 하고 싶지만, 아직 설정되기 전 상황
		// _abnormalityEffectScale = UGsGameObjectUtil::GetParticleEffectScale(_owner);
		_abnormalityEffectScale = UGsGameObjectUtil::GetParticleEffectScale(_owner->GetActor());
	}
}

void FGsAbnormalityHandlerBase::PlaySound(const FGsSchemaSoundResData* InSoundData, AbnormalityInstanceId InKey)
{
	if (uint64* value = _mapSound.Find(InKey))
	{
		// 존재 한다면 어떻게 해야되지...?
	}
	else
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			AActor* actor = _owner->GetActor();
			uint64 soundId = soundPlayer->PlaySoundTarget(InSoundData, actor->GetRootComponent());
			_mapSound.Emplace(InKey, soundId);
		}
	}
}

void FGsAbnormalityHandlerBase::StopSound(AbnormalityInstanceId InKey)
{
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		// InKey == 0 이면 모두 제거
		if (InKey != 0)
		{
			if (uint64* value = _mapSound.Find(InKey))
			{
				soundPlayer->StopSound(*value);
				_mapSound.Remove(InKey);
			}
		}
		else
		{
			for (TPair<AbnormalityInstanceId, uint64>& pair : _mapSound)
			{
				soundPlayer->StopSound(pair.Value);
			}
			_mapSound.Empty();
		}
	}
}

bool FGsAbnormalityHandlerBase::GetAbnormalityDataList(TArray<FGsAbnormalityData*>& OutList, bool bUseSort)
{
	OutList.Empty();

	for (const auto& pair : _mapAbnormality)
	{
		auto data = pair.Value;
		if (data && data->GetTable() &&
			false == data->GetTable()->hideUI)
		{
			OutList.Emplace(data.Get());
		}
	}

	if (bUseSort)
	{
		// 먼저 받은 상태이상이 리스트의 앞에 오도록 설정
		OutList.Sort([](FGsAbnormalityData& lhs, FGsAbnormalityData& rhs)
			{
				return lhs._pktReceiveAppTime < rhs._pktReceiveAppTime;
			});
	}

	return true;
}

void FGsAbnormalityHandlerBase::SetVisibleOwner(UGsGameObjectCreature* castObject, bool set, bool effectApply)
{	
	castObject->SetGameObjectActorVisible(set);
	castObject->UpdateVisible();

	if (effectApply)
	{
		if (auto character = castObject->GetCharacter())
		{
			character->EffectsHiddenInGame(!set);
		}
	}
}

int32 FGsAbnormalityHandlerBase::GetBuffCount() const
{
	int abnormalityCount = 0;
	int debuffCount = 0;
	for (const auto& pair : _mapAbnormality)
	{
		if (false == pair.Value->GetTable()->hideUI)
		{
			++abnormalityCount;

			if (false == pair.Value->GetTable()->isBuff)
			{
				++debuffCount;
			}
		}
	}

	return abnormalityCount - debuffCount;
}

int FGsAbnormalityHandlerBase::GetAbnormalityEffectType(AbnormalityEffectType Type) const
{
	return _typeCount[Type];
}

bool FGsAbnormalityHandlerBase::IsAbnormalityEffectType(AbnormalityEffectType Type) const
{
	return GetAbnormalityEffectType(Type) > 0;
}

bool FGsAbnormalityHandlerBase::IsAbnormalityMaterialChangeType(AbnormalityEffectType Type) const
{
	if (Type == AbnormalityEffectType::UTIL_HIDING)
//		||
// 		Type == AbnormalityEffectType::CC_COCOON ||
// 		Type == AbnormalityEffectType::CC_FREEZE ||
//		Type == AbnormalityEffectType::OVERTIME_BURN)
	{
		return true;
	}
	return false;
}

bool FGsAbnormalityHandlerBase::GetAbnormalityDebuffList(TArray<FGsAbnormalityData*>& OutList, bool bUseSort)
{
	OutList.Empty();

	for (const auto& pair : _mapAbnormality)
	{
		auto data = pair.Value;

		if (data && data->GetTable() &&
			false == data->GetTable()->isBuff &&
			false == data->GetTable()->hideUI)
		{
			OutList.Emplace(data.Get());
		}
	}

	if (bUseSort)
	{
		// 먼저 받은 상태이상이 리스트의 앞에 오도록 설정
		OutList.Sort([](FGsAbnormalityData& lhs, FGsAbnormalityData& rhs)
			{
				return lhs._pktReceiveAppTime < rhs._pktReceiveAppTime;
			});
	}

	return true;
}

TWeakPtr<FGsAbnormalityData> FGsAbnormalityHandlerBase::GetLongerstAbnormalityData(AbnormalityEffectType Type)
{
	TWeakPtr<FGsAbnormalityData> longerstData = nullptr;
	for (const auto& pair : _mapAbnormality)
	{
		TArray<AbnormalityEffectType> effects;
		pair.Value->GetAllEffectType(effects);
		if(effects.Find(Type) == INDEX_NONE)
			continue;

		if (nullptr == longerstData || longerstData.Pin()->GetRemainTime() < pair.Value->GetRemainTime())
		{
			longerstData = pair.Value;
		}
	}

	return longerstData;
}

TWeakPtr<FGsAbnormalityData> FGsAbnormalityHandlerBase::GetAbnormalityData(AbnormalityInstanceId InKey) const
{
	if (auto iter = _mapAbnormality.Find(InKey))
		return *iter;
	return nullptr;
}

bool FGsAbnormalityHandlerBase::IsAbnormalityId(AbnormalityId InAbnormalityId) const
{
	for (const auto& it : _mapAbnormality)
	{
		if (it.Value->GetTableId() == InAbnormalityId)
		{
			return true;
		}
	}

	return false;
}

bool FGsAbnormalityHandlerBase::IsAbnormalityGroupId(int32 InAbnormalityGroupID) const
{
	for (const auto& it : _mapAbnormality)
	{
		if (it.Value->GetTableGroupId() == InAbnormalityGroupID)
		{
			return true;
		}
	}

	return false;
}

bool FGsAbnormalityHandlerBase::IsStateChangedByAbnormality()
{
	if (auto crowdControl = GetCrowdControl())
	{
		//현재 animation control 되는 abnormality 가 있는지
		return crowdControl->IsAnyOfValidAnimReaction();

			/*	STUNACT = 1 UMETA(Tooltip = "기절 타입 액션"),
				FEARACT = 2 UMETA(Tooltip = "공포 타입 액션"),
				KNOCKBACK = 3 UMETA(Tooltip = "넉백"),
				KNOCKDOWN = 4 UMETA(Tooltip = "넉다운"),
				FRAMESTOP = 5 UMETA(Tooltip = "그대로 멈춤"),
				POLYMORPH = 6 UMETA(Tooltip = "변이"),*/

		//for (const auto& e : _mapAbnormality)
		//{
		//	for (auto& effectSubType :  e.Value.GetEffectTypeList())
		//	{
		//		if (crowdControl->IsCrowdControlReactionType(effectSubType))
		//			return true;
		//	}
		//}
	}
	return false;
}

bool FGsAbnormalityHandlerBase::IsDamagedByAbnormality()
{
	if (auto crowdControl = GetCrowdControl())
	{
		if (crowdControl->IsAnyOfValidAnimReaction())
			return true;
	}

	//우선 구분해야 함 damaged abnormality 상태인지
	return _abnormalityDebuffList.Num() > 0;
		
	//for (auto& effectSubType : e.Value.GetEffectTypeList())
	//{
	//	switch (effectSubType)
	//	{
	//	case AbnormalityEffectType::OVERTIME_BLEED:
	//	case AbnormalityEffectType::OVERTIME_POISON:
	//	case AbnormalityEffectType::OVERTIME_BURN:
	//		return true;
	//	}
	//}
}

bool FGsAbnormalityHandlerBase::IsCrowdControlMaterialChangeType(AbnormalityEffectType Type) const
{
	//const FGsCrowdControlInfo& info = _arrCrowdControlInfo[(int)Type];
	//if (!info.IsValid()) return false; //순서 관리 필요없는 타입

	if (IsAbnormalityMaterialChangeType(Type))
	{
		return Algo::AnyOf(_mapAbnormality, [Type](auto& e) {
				TArray<AbnormalityEffectType> effects;
				return e.Value->GetAllEffectType(effects) > 0 && effects.Find(Type) != INDEX_NONE;
			});
	}
	return false;
}

bool FGsAbnormalityHandlerBase::IsTargetedAvailableState()
{
	return (IsAbnormalityEffectType(AbnormalityEffectType::CC_BURY) == false &&
			IsAbnormalityEffectType(AbnormalityEffectType::UTIL_HIDING) == false);
}
