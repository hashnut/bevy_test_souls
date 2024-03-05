// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillUIDataHandler.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "DataSchema/Costume/GsSchemaCostumeSkill.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Config/GsSchemaBattleConfig.h"
#include "Item/GsItemManager.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillUIData.h"
#include "Skill/GsSkillHelper.h"
#include "Costume/GsCostumeData.h"
#include "UTIL/GsTableUtil.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


FGsSkillUIDataHandler::~FGsSkillUIDataHandler()
{

}

void FGsSkillUIDataHandler::Initialize()
{

}

void FGsSkillUIDataHandler::Finalize()
{
	ClearData();
}

void FGsSkillUIDataHandler::InitializeData(CreatureGenderType InGender)
{
	InitCostumeSkillMap(InGender);
}

void FGsSkillUIDataHandler::ClearData()
{
	_costumeSkillMap.Empty();
	_skillDataMap.Empty();
	
	_inventorySkillBookMap.Empty();
	SetIsSkillBookCached(false);

	_windowOpenSelectedSkillId = INVALID_SKILL_ID;
}

void FGsSkillUIDataHandler::InitCostumeSkillMap(CreatureGenderType InGender)
{
	// 코스튬 테이블 가져오기
	TArray<const FGsSchemaCostumeCommon*> rowList;
	if (const UGsTable* table = FGsSchemaCostumeCommon::GetStaticTable())
	{
		table->GetAllRows(rowList);
	}

	// 현재 성별 가져오기
	UGsSkillManager* skillMgr = GSkill();
	for (const FGsSchemaCostumeCommon* costume : rowList)
	{
		// 현재 성별과 다르거나 All타입이 아니면 제외
		if (InGender != costume->genderType &&
			CreatureGenderType::ALL != costume->genderType)
		{
			continue;
		}

		const FGsSchemaUserSkillSet* userSkillSet = costume->skillActiveInfo.userSkillSetId.GetRow();
		if (nullptr == userSkillSet)
		{
			continue;
		}

		const FGsSchemaSkillSet* skillSet = nullptr;
		if (CreatureGenderType::MALE == InGender)
		{
			skillSet = userSkillSet->maleSkillId.GetRow();
		}
		else
		{
			skillSet = userSkillSet->femaleSkillId.GetRow();
		}

		TWeakPtr<const FGsSkill> skill = skillMgr->FindSkillWeakPtr(skillSet->id, skillSet->requireWeapon);
		if (skill.IsValid())
		{
			_costumeSkillMap.Emplace(costume->id, skill);
		}
	}
}

void FGsSkillUIDataHandler::InitSkillDataMap()
{
	if (0 < _skillDataMap.Num())
	{
		for (TPair<CreatureWeaponType, DefCategorySkillDataMap>& itPairWeapon : _skillDataMap)
		{
			for (TPair<SkillCategorySet, TArray<FGsSkillUIData>>& itPairCategory : itPairWeapon.Value)
			{
				for (FGsSkillUIData& skillData : itPairCategory.Value)
				{
					skillData.SetSkillCurrUpgradeStep();
				}
			}
		}

		return;
	}

	for (uint8 i = static_cast<uint8>(CreatureWeaponType::SWORD), maxWeapon = static_cast<uint8>(CreatureWeaponType::MAX);
		i < maxWeapon; ++i)
	{
		for (uint8 j = static_cast<uint8>(SkillCategorySet::ACTIVE), maxCategory = static_cast<uint8>(SkillCategorySet::MAX);
			j < maxCategory; ++j)
		{
			MakeSkillDataMap(static_cast<CreatureWeaponType>(i), static_cast<SkillCategorySet>(j));
		}
	}

	MakeCostumeSkillDataMap();
}

void FGsSkillUIDataHandler::MakeSkillDataMap(CreatureWeaponType InWeapon, SkillCategorySet InCategory)
{
	// 무기스킬 리스트 가져오기
	TArray<TSharedPtr<const FGsSkill>> tempWeaponList;
	GSkill()->GetSkillUiSet(tempWeaponList, InWeapon, InCategory);

	TArray<FGsSkillUIData> skillList;
	for (TSharedPtr<const FGsSkill> skill : tempWeaponList)
	{
		// 코스튬 스킬 제외
		if (skill->_tableData)
		{
			if (skill->_tableData->isCostume)
			{
				continue;
			}
		}

		// 업그레이드 스킬 체크
		if (const FGsSchemaSkillLevelSet* levelSet = skill->GetCurrLevelSet())
		{
			// 업그레이드 스킬이 아닐 경우
			if (0 == levelSet->maxUpgradeStep)
			{
				// 체인스킬 검사
				if (skill->IsChainSkill())
				{
					FGsSkillUIData skillData;
					if (CheckAndMakeChainSkill(skill, skillData))
					{
						skillList.Emplace(skillData);
					}
				}
				else
				{
					FGsSkillUIData skillData;
					skillData.SetSkill(skill);
					skillList.Emplace(skillData);
				}

				continue;
			}

			// 주의: R3 0단계일때를 체크하고 싶은데 현재 이런 데이터가 없음. 기획 데이터 들어가면 수정할 것
			if (1 == levelSet->upgradeInfo.currentStep)
			{
				// 업그레이드 단계정보 채우기
				FGsSkillUIData skillData;
				skillData.SetSkill(skill);

				FGsSkillHelper::GetSkillListByUpgradeStep(skill.Get(), skillData._upgradeList);

				// R3: 업그레이드 이후 단계 중 체인이 있는지 검사. 가장 낮은 단계의 체인정보 표기
				// 스텝마다 체인 스킬이 다를 경우 데이터 집행 오류.(기획서 14페이지에 명시)
				for (int i = 0; i < skillData._upgradeList.Num(); ++i)
				{
					if (skillData._upgradeList[i].Pin()->IsChainSkill())
					{
						if (CheckAndMakeChainSkill(skillData._upgradeList[i], skillData))
						{
							break;
						}
					}
				}

				// 현재 업그레이드 스탭 정보로 세팅
				skillData.SetSkillCurrUpgradeStep();

				skillList.Emplace(skillData);
			}

			continue;
		}

		// 패시브 스킬 등은 레벨셋이 존재하지 않음
		FGsSkillUIData skillData;
		skillData.SetSkill(skill);
		skillList.Emplace(skillData);
	}

	// 공통스킬 리스트 가져오기
	// [B.Y] 공통 스킬은 더이상 수집하지 않음 
	// 기존엔 모든 CreatureWeaponType에 포함 되었지만 따로 CreatureWeaponType::All 카테고리로 수집
	// 
	// TArray<TSharedPtr<const FGsSkill>> tempCommonList;
	// GSkill()->GetSkillUiSet(tempCommonList, CreatureWeaponType::ALL, InCategory);

	// 공통스킬엔 체인, 업그레이드 기능 없으므로 그냥 저장
	// 	for (TSharedPtr<const FGsSkill> skill : tempCommonList)
	// 	{
	// 		FGsSkillUIData skillData;
	// 		skillData.SetSkill(skill);
	// 		skillList.Emplace(skillData);
	// 	}

	// 맵에 저장
	if (DefCategorySkillDataMap* categoryMapPtr = _skillDataMap.Find(InWeapon))
	{
		if (categoryMapPtr->Contains(InCategory))
		{
			(*categoryMapPtr)[InCategory] = skillList;
		}
		else
		{
			categoryMapPtr->Emplace(InCategory, skillList);
		}
	}
	else
	{
		DefCategorySkillDataMap categoryMap;
		categoryMap.Emplace(InCategory, skillList);

		_skillDataMap.Emplace(InWeapon, categoryMap);
	}
}

bool FGsSkillUIDataHandler::CheckAndMakeChainSkill(TWeakPtr<const FGsSkill> InSkill, OUT FGsSkillUIData& OutData)
{
	if (1 != InSkill.Pin()->GetChainStep())
	{
		return false;
	}

	TArray<TWeakPtr<const FGsSkill>> chainList;
	RecursiveGetSkillListByChainStep(InSkill, chainList);

	for (int32 i = 0; i < chainList.Num(); ++i)
	{
		if (0 == i)
		{
			OutData.SetSkill(chainList[i]);
			OutData.SetFirstChainSkill(InSkill.Pin()->GetSkillId());
		}
		else
		{
			FGsSkillUIData chainSkillData;
			chainSkillData.SetSkill(chainList[i]);
			chainSkillData.SetFirstChainSkill(InSkill.Pin()->GetSkillId());

			OutData._chainList.Emplace(chainSkillData);
		}
	}

	return true;
}

void FGsSkillUIDataHandler::MakeCostumeSkillDataMap()
{
	for (TPair<CostumeId, TWeakPtr<const FGsSkill>> itPair : _costumeSkillMap)
	{
		if (itPair.Value.IsValid())
		{
			FGsSkillUIData skillData;
			skillData.SetSkill(itPair.Value);

			const FGsSchemaCostumeCommon* costumeTable = 
				UGsTableUtil::FindRowById<UGsTableCostumeCommon, FGsSchemaCostumeCommon>(itPair.Key);
			skillData._costume = costumeTable;

			if (DefCategorySkillDataMap* weaponMap = _skillDataMap.Find(itPair.Value.Pin()->GetSkillRequireWeaponType()))
			{
				if (TArray<FGsSkillUIData>* list = weaponMap->Find(COSTUME_CATEGORY_TYPE))
				{
					list->Emplace(skillData);
				}
				else
				{
					TArray<FGsSkillUIData> newList;
					newList.Emplace(skillData);

					weaponMap->Emplace(COSTUME_CATEGORY_TYPE, newList);
				}
			}
		}
	}
}

void FGsSkillUIDataHandler::RecursiveGetSkillListByChainStep(TWeakPtr<const FGsSkill> InSkill, 
	OUT TArray<TWeakPtr<const FGsSkill>>& OutList)
{
	if (false == InSkill.IsValid() ||
		nullptr == InSkill.Pin()->_tableData)
	{
		return;
	}

	if (false == InSkill.Pin()->IsChainSkill())
	{
		return;
	}

	if (0 == InSkill.Pin()->_tableData->sequenceInfo.comboInfo.currentStage)
	{
		return;
	}

	// 저장
	OutList.Emplace(InSkill);

	// 다음 단계 재귀 호출
	if (InSkill.Pin()->_tableData->sequenceInfo.maxStage == InSkill.Pin()->_tableData->sequenceInfo.comboInfo.currentStage)
	{
		return;
	}

	// 무한 루프 돌지 않도록 수정
	if (InSkill.Pin()->GetSkillId() == InSkill.Pin()->_tableData->sequenceInfo.comboInfo.nextComboSkillId)
	{
		return;
	}

	// 다음 단계
	if (0 < InSkill.Pin()->_tableData->sequenceInfo.comboInfo.nextComboSkillId)
	{
		TWeakPtr<const FGsSkill> nextSkill = GSkill()->FindSkillWeakPtr(
			InSkill.Pin()->_tableData->sequenceInfo.comboInfo.nextComboSkillId, InSkill.Pin()->_tableData->requireWeapon);

		if (nextSkill.IsValid())
		{
			RecursiveGetSkillListByChainStep(nextSkill, OutList);
		}
	}
}

TMap<SkillCategorySet, TArray<FGsSkillUIData>>* FGsSkillUIDataHandler::GetCategoryMapPtr(CreatureWeaponType InType)
{
	return _skillDataMap.Find(InType);
}

FGsSkillUIData* FGsSkillUIDataHandler::GetSkillUIData(CreatureWeaponType InType, SkillCategorySet InCategory, SkillId InSkillId)
{
	if (TMap<SkillCategorySet, TArray<FGsSkillUIData>>* categoryMap = GetCategoryMapPtr(InType))
	{
		if (TArray<FGsSkillUIData>* dataListPtr = categoryMap->Find(InCategory))
		{
			for (FGsSkillUIData& iter : *dataListPtr)
			{
				if (iter.GetSkillId() == InSkillId)
				{
					return &iter;
				}
			}
		}
	}

	return nullptr;
}

CostumeId FGsSkillUIDataHandler::GetCostumeIdBySkillId(SkillId InSkillId) const
{
	for (TPair<CostumeId, TWeakPtr<const FGsSkill>> itPair : _costumeSkillMap)
	{
		if (itPair.Value.IsValid())
		{
			if (itPair.Value.Pin()->GetSkillId() == InSkillId)
			{
				return itPair.Key;
			}
		}
	}

	return INVALID_COSTUME_ID;
}

bool FGsSkillUIDataHandler::IsLearnedCostumeSkill() const
{
	for (TPair<CostumeId, TWeakPtr<const FGsSkill>> itPair : _costumeSkillMap)
	{
		if (itPair.Value.IsValid())
		{
			if (itPair.Value.Pin()->_isLearn &&
				IsCollectedCostume(itPair.Key))
			{
				return true;
			}
		}
	}

	return false;
}

void FGsSkillUIDataHandler::AddSkillBook(SkillId InSkillId, ItemId InSkillBookId)
{
	if (TArray<ItemId>* list = _inventorySkillBookMap.Find(InSkillId))
	{
		if (list->Contains(InSkillBookId))
		{
			return;
		}

		list->Emplace(InSkillBookId);
	}
	else
	{
		TArray<ItemId> itemArray;
		itemArray.Emplace(InSkillBookId);

		_inventorySkillBookMap.Emplace(InSkillId, itemArray);
	}
}

void FGsSkillUIDataHandler::RemoveSkillBook(ItemId InSkillBookId)
{
	for (TPair<SkillId, TArray<ItemId>>& itPair : _inventorySkillBookMap)
	{
		if (itPair.Value.Contains(InSkillBookId))
		{
			itPair.Value.Remove(InSkillBookId);
			if (0 == itPair.Value.Num())
			{
				_inventorySkillBookMap.Remove(itPair.Key);
			}

			break;
		}
	}
}

const TArray<ItemId>* FGsSkillUIDataHandler::GetExistSkillBookList(SkillId InSkillId) const
{
	return _inventorySkillBookMap.Find(InSkillId);
}

bool FGsSkillUIDataHandler::IsCollectedCostume(CostumeId InCostumeId) const
{
	TWeakPtr<FGsCostumeData> costumeData = GSCostume()->GetCostumeData(InCostumeId);
	if (costumeData.IsValid())
	{
		return costumeData.Pin()->isCollectedCostume();
	}

	return false;
}

TWeakPtr<const FGsSkill> FGsSkillUIDataHandler::GetSkillByCostumeId(CostumeId InCostumeId)
{
	TWeakPtr<const FGsSkill>* skillPtr = _costumeSkillMap.Find(InCostumeId);
	if (nullptr != skillPtr)
	{
		return *skillPtr;
	}
	
	return nullptr;
}

float FGsSkillUIDataHandler::CalcMPRecovery()
{
	UGsGameObjectManager* gameObjectMgr = GSGameObject();
	if (nullptr == gameObjectMgr)
	{
		return 0.f;
	}

	float outValue = 0.f;
	if (UGsGameObjectBase* localbase = gameObjectMgr->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* localPlayer = localbase->CastGameObject<UGsGameObjectLocalPlayer>())
		{
			// 무게 패널티 상태가 아닐 때: MP+TICK_RECOVERY_NORMAL / 패널티 상태일 때: MP_TICK_RECOVERY_PENALTY
			if (UGsItemManager* itemMgr = GItem())
			{
				if (WeightPenaltyGrade::NONE == itemMgr->GetWeightPenaltyGradeType())
				{
					if (const FGsStatInfo* mpRecoveryNormal = localPlayer->GetCreatureStatInfoByType(StatType::MP_TICK_RECOVERY_NORMAL))
					{
						int32 statValue = mpRecoveryNormal->GetStatValue();
						outValue = static_cast<float>(statValue);
					}
				}
				else
				{
					if (const FGsStatInfo* mpRecovery = localPlayer->GetCreatureStatInfoByType(StatType::MP_TICK_RECOVERY_PENALTY))
					{
						int32 statValue = mpRecovery->GetStatValue();
						outValue = static_cast<float>(statValue);
					}
				}
			}
		}
	}

	return outValue;
}

float FGsSkillUIDataHandler::CalcMpUsage(CreatureWeaponType InWeapon)
{
	UGsSkillManager* skillMgr = GSkill();
	if (nullptr == skillMgr)
	{
		return 0.f;
	}

	UGsGameObjectManager* gameObjectMgr = GSGameObject();
	if (nullptr == gameObjectMgr)
	{
		return 0.f;
	}

	// 1. skillCoolTimeRate 가져오기
	float skillCoolTimeRate = 1.f;
	if (UGsGameObjectBase* localbase = gameObjectMgr->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* localPlayer = localbase->CastGameObject<UGsGameObjectLocalPlayer>())
		{
			if (const FGsStatInfo* statCoolTimeRate = localPlayer->GetCreatureStatInfoByType(StatType::SKILL_COOLTIME_RATE))
			{
				int32 statValue = statCoolTimeRate->GetStatValue();
				skillCoolTimeRate = statValue * 0.0001f; // 만분율
			}
		}
	}

	// 2. regenerationTic 가져오기
	float regenerationTick = 5.f;
	if (const UGsTable* battelTable = FGsSchemaBattleConfig::GetStaticTable())
	{
		TArray<const FGsSchemaBattleConfig*> battleRows;
		battelTable->GetAllRows(battleRows);

		if (battleRows.IsValidIndex(0))
		{
			regenerationTick = battleRows[0]->regenerationTic;
		}
	}

	// 3. mp 및 쿨타임 가져와 계산
	const TMap<SkillSlotId, const FGsSkill*>* slotMapPtr = skillMgr->GetSkillSlotMap(InWeapon);
	if (nullptr == slotMapPtr)
	{
		return 0.f;
	}

	float outValue = 0.f;
	for (TPair<SkillSlotId, const FGsSkill*> itPair : *slotMapPtr)
	{
		const FGsSkill* skill = itPair.Value;
		if (nullptr == skill)
		{
			continue;
		}

		// 오토 켜놓은 스킬만
		if (false == skill->IsAuto())
		{
			continue;
		}

		// 스킬 MP계산
		outValue += CalcMPUsageInter(skill, skillCoolTimeRate, regenerationTick);

		// 다음 체인 스킬의 오토가 켜져있는 경우, 연산에 추가
		if (skill->IsChainSkill())
		{
			FGsSkillUIData* skillUIData = GetSkillUIData(InWeapon, skill->GetSkillCateogrySet(), skill->GetSkillId());
			if (skillUIData)
			{
				for (FGsSkillUIData& iter : skillUIData->_chainList)
				{
					if (const FGsSkill* nextSkill = iter.GetSkill())
					{
						if (nextSkill->IsAuto())
						{
							outValue += CalcMPUsageInter(nextSkill, skillCoolTimeRate, regenerationTick);
						}
					}
				}
			}
		}
	}

	return outValue;
}

float FGsSkillUIDataHandler::CalcMPUsageInter(const FGsSkill* InSkill, float InCoolTimeRate, float InRegenerationTick)
{	
	const FGsSchemaSkillLevelInfo* levelInfo = InSkill->GetLevelInfo(InSkill->GetLevel());
	if (nullptr == levelInfo)
	{
		return 0.f;
	}

	int32 mpValue = 0;
	for (const FGsSchemaSkillStatInfo& statInfo : levelInfo->costConsumeInfo.statInfoList)
	{
		if (StatType::MP == statInfo.type.GetStatType())
		{
			mpValue = statInfo.value;
			break;
		}
	}
	
	if (0 == mpValue)
	{
		return 0.f;
	}

	float time = 0.f;

	// 버프 스킬의 경우 버프 끝나는 시간까지 연산해야 해서 다르게 연산
	if (SkillCategorySet::BUFF == InSkill->GetSkillCateogrySet())
	{
		time = GetBuffSkillDuration(InSkill);
		// 버프스킬은 스킬 주기 설정을 하지 않는다
	}
	else
	{
		if (0 < levelInfo->cooldownTime &&
			0 < InCoolTimeRate)
		{
			time = levelInfo->cooldownTime * InCoolTimeRate;

			// 스킬 주기가 활성화되어 있을 경우 더함
			if (InSkill->IsActiveSkillCycle())
			{
				time += static_cast<float>(InSkill->GetCycleSecond());
			}
		}
	}

	// 올바른 시간이 아닐 경우 0 리턴
	if (0 >= time)
	{
		return 0.f;
	}

	float outValue = (mpValue / time) * InRegenerationTick;

	return outValue;
}

float FGsSkillUIDataHandler::GetBuffSkillDuration(const FGsSkill* InSkill)
{
	if (nullptr == InSkill)
	{
		return 0.f;
	}

	const FGsSchemaSkillLevelInfo* levelInfo = InSkill->GetLevelInfo(InSkill->GetTotalLevel());
	if (nullptr == levelInfo)
	{
		return 0.f;
	}

	if (0 == levelInfo->notifyInfoList.Num())
	{
		return 0.f;
	}

	for (int i = 0; i < levelInfo->notifyInfoList.Num(); ++i)
	{
		const FGsSchemaSkillNotifySet* notifySet =
			UGsTableUtil::FindRowById<UGsTableSkillNotifySet, FGsSchemaSkillNotifySet>(levelInfo->notifyInfoList[i]);
		if (nullptr == notifySet)
		{
			continue;
		}

		if (SkillNotifyType::DAMAGE != notifySet->type)
		{
			continue;
		}

		for (int j = 0; j < notifySet->notifyElementList.Num(); ++j)
		{
			const FGsSchemaSkillNotifyElement* notifyElement = notifySet->notifyElementList[j].GetRow();
			if (nullptr == notifyElement)
			{
				continue;
			}

			const FGsSchemaSkillNotifyHitSet* hitSet = notifyElement->typeInfo.GetRowByType<FGsSchemaSkillNotifyHitSet>();
			if (nullptr == hitSet)
			{
				continue;
			}

			const FGsSchemaAbnormalitySet* abnormalitySet =
				UGsTableUtil::FindRowById<UGsTableAbnormalitySet, FGsSchemaAbnormalitySet>(hitSet->abnormalityId1);

			if (abnormalitySet)
			{
				return abnormalitySet->durationMin;
			}
		}
	}

	return 0.f;
}
