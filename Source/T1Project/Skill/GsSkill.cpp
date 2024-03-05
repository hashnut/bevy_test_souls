// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkill.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "DataSchema/Skill/GsSchemaSkillLevel.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "Util/GsTableUtil.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"

FGsSkill::FGsSkill()
{
	UpdateCycleTimer();
}


FGsSkill& FGsSkill::operator = (const FGsSkill& SkillData)
{
	Set(SkillData);

	return *this;
}

FGsSkill& FGsSkill::operator = (const FGsSkill* SkillData)
{
	Set(SkillData);

	return *this;
}

void FGsSkill::Set(const FGsSkill& SkillData)
{
	_tableData = SkillData._tableData;
	_userSkillSetData = SkillData._userSkillSetData;
	_isLearn = SkillData._isLearn;
	_isNew = SkillData._isNew;
	_level = SkillData.GetLevel();
	_bonusLevel = SkillData.GetBonusLevel();
	_slotId = SkillData._slotId;
	_prevSkillId = SkillData._prevSkillId;
	_slotData._skillCheckResult = SkillData._slotData._skillCheckResult;
	SetUseAuto(SkillData.GetSlotData()._bUseAuto);
	SetReserve(SkillData.GetSlotData()._isReserve);
	_presetData = SkillData._presetData;
	_cycleTimer = SkillData._cycleTimer;

	if (0 == SkillData.GetMaxLevel())
	{
		CacheMaxLevel();
	}

	SetSkillCycle(SkillData._isActiveCycle, SkillData._isActiveCycleInPVP, SkillData._cycleSecond);
}

void FGsSkill::Set(const FGsSkill* SkillData) 
{
	Set(*SkillData);
}

void FGsSkill::SetUseAuto(bool UseAuto)
{
	_slotData._bUseAuto = UseAuto;
}

void FGsSkill::SetReserve(bool Reserve)
{
	_slotData._isReserve = Reserve;
}

void FGsSkill::SetSkillCheckResult(EGsSkillSlotCheckType Type, bool Isfail)
{
	uint16 val = 1 << (uint16)Type;
	if (Isfail)
	{
		_slotData._skillCheckResult |= val;
	}
	else
	{
		_slotData._skillCheckResult &= ~val;
	}
}

void FGsSkill::SetPresetInfo(PresetId InPresetId, SkillSlotId InSlotId, bool InUseAuto)
{
	if (_presetData.Contains(InPresetId))
	{
		_presetData[InPresetId] = TPair<SkillSlotId, bool>{ InSlotId, InUseAuto };
	}
	else
	{
		_presetData.Emplace(InPresetId, TPair<SkillSlotId, bool>{ InSlotId, InUseAuto });
	}
}

const FSoftObjectPath* FGsSkill::GetIconPath() const
{
	return (_userSkillSetData) ? &_userSkillSetData->icon : nullptr;
}

const SkillId FGsSkill::GetUserSkillSetId() const
{
	return (_userSkillSetData) ? _userSkillSetData->id : INVALID_SKILL_ID;
}

const FGsSchemaSkillLevelSet* FGsSkill::GetCurrLevelSet() const
{
	if (_tableData)
	{
		// 패시브 스킬은 레벨셋이 없음
		if (SkillCategory::PASSIVE != _tableData->category)
		{
			return UGsTableUtil::FindRowById<UGsTableSkillLevelSet, FGsSchemaSkillLevelSet>(_tableData->id);
		}
	}

	return nullptr;
}

const FGsSchemaSkillLevelInfo* FGsSkill::GetLevelInfo(int32 InId, int32 InLevel)
{
	if (const FGsSchemaSkillLevelSet* skilllevelSet =
		UGsTableUtil::FindRowById<UGsTableSkillLevelSet, FGsSchemaSkillLevelSet>(InId))
	{
		const FGsSchemaSkillLevelInfo* skillLevelInfo = skilllevelSet->levelInfoList.FindByPredicate(
			[InLevel](FGsSchemaSkillLevelInfo& el)
			{
				return el.level == InLevel;
			});

		if (nullptr == skillLevelInfo)
		{
			return nullptr;
		}

		return skillLevelInfo;
	}

	return nullptr;
}

const FGsSchemaSkillLevelInfo* FGsSkill::GetLevelInfo(int32 InLevel) const
{
	if (const FGsSchemaSkillLevelInfo*const* findSkillLevel = _levelInfoMap.Find(InLevel))
	{
		return *findSkillLevel;
	}
	else if (const FGsSchemaSkillLevelInfo* newSkillLevelSet = GetLevelInfo(_tableData->id, InLevel))
	{
		// 새로 찾은거면 캐싱 버퍼에 등록
		_levelInfoMap.Emplace(InLevel, newSkillLevelSet);
		return newSkillLevelSet;
	}

	return nullptr;
}

bool FGsSkill::IsFirstNormalSkill() const
{
	if (_tableData)
	{
		if (_tableData->isNormal &&
			1 == _tableData->sequenceInfo.comboInfo.currentStage)
		{
			return true;
		}
	}

	return false;
}

bool FGsSkill::IsNormalSkill() const
{
	if (_tableData)
	{
		return _tableData->isNormal;
	}
	return false;
}


// 데이터에 설정되어있는 원거리 소모품 데이터
const FGsSchemaSkillCostConsumeInfo* FGsSkill::GetCurrLevelCostConsumeInfoData(int32 InLevel) const
{
	const FGsSchemaSkillLevelInfo* levelSetData = GetLevelInfo(InLevel);
	if (nullptr != levelSetData)
	{
		return &levelSetData->costConsumeInfo;
	}
	
	return nullptr;
}

SkillCategorySet FGsSkill::GetSkillCateogrySet() const
{
	if (_tableData)
	{
		return SharedFunc::GetSkillCategorySet(_tableData->category);
	}

	return SkillCategorySet::MAX;
}

bool FGsSkill::IsEnable() const
{
	return _slotData._skillCheckResult == 0;
}

bool FGsSkill::IsReserve() const
{
	return _slotData._isReserve;
}

bool FGsSkill::IsAuto() const
{
	return _slotData._bUseAuto;
}

bool FGsSkill::IsSelectTarget() const
{
	return _isSelectTarget;
}

bool FGsSkill::IsComboSkill() const
{
	return _tableData->category == SkillCategory::COMBO;
}

bool FGsSkill::IsBuffSkill() const
{
	return GetSkillCateogrySet() == SkillCategorySet::BUFF;
}

float FGsSkill::GetCooldownTime(int32 InLevel) const
{
	const FGsSchemaSkillLevelInfo* levelSetData = GetLevelInfo(InLevel);
	if (levelSetData)
	{
		return levelSetData->cooldownTime;
	}

	return 0.f;
}

float FGsSkill::GetCurrentCooldownTime() const
{
	return GCoolTime()->GetCoolTime(EGsCoolTime::Skill, GetSkillId());
}

int32 FGsSkill::GetUpgradeStep() const
{
	if (const FGsSchemaSkillLevelSet* levelSet = GetCurrLevelSet())
	{
		return levelSet->upgradeInfo.currentStep;
	}

	return 0;
}

int32 FGsSkill::GetMaxUpgradeStep() const
{
	if (const FGsSchemaSkillLevelSet* levelSet = GetCurrLevelSet())
	{
		return levelSet->maxUpgradeStep;
	}

	return 0;
}

int32 FGsSkill::GetChainStep() const
{
	if (_tableData)
	{
		return _tableData->sequenceInfo.comboInfo.currentStage;
	}

	return 0;
}

int32 FGsSkill::GetMaxChainStep() const
{
	if (_tableData)
	{
		if (IsChainSkill())
		{
			return _tableData->sequenceInfo.maxStage;
		}
	}	

	return 0;
}

bool FGsSkill::IsChainSkill() const
{
	if (_tableData)
	{
		return (SkillSequenceType::CHAIN == _tableData->skillSequenceType) ? true : false;
	}

	return false;
}

bool FGsSkill::IsFirstSkill() const
{
	if (_tableData)
	{
		return (_tableData->sequenceInfo.comboInfo.currentStage == 1) ? true : false;
	}
	return false;
}

bool FGsSkill::IsFirstChainSkill() const
{
	return IsChainSkill() && IsFirstSkill();
}

bool FGsSkill::IsLastChainStep() const
{
	if (_tableData)
	{
		return (_tableData->sequenceInfo.maxStage == _tableData->sequenceInfo.comboInfo.currentStage) ? true : false;
	}

	return false;
}

bool FGsSkill::IsCostumeSkill() const
{
	if (_tableData)
	{
		return _tableData->isCostume;
	}
	return false;
}

void FGsSkill::CacheMaxLevel()
{
	const FGsSchemaSkillLevelSet* levelSet = GetCurrLevelSet();
	if (nullptr == levelSet)
	{
		_cachedMaxLevel = 1;
		return;
	}

	for (const FGsSchemaSkillLevelInfo& iter : levelSet->levelInfoList)
	{
		if (iter.level > _cachedMaxLevel)
		{
			_cachedMaxLevel = iter.level;
		}
	}
}

int32 FGsSkill::GetTotalLevel() const
{
	return FMath::Clamp(_level + _bonusLevel, 1, GetMaxLevel());
}

int32 FGsSkill::GetMaxEnchantLevel() const
{
	if (const FGsSchemaSkillLevelSet* levelSet = GetCurrLevelSet())
	{
		return levelSet->maxEnchantLevel;
	}

	return 1;
}

bool FGsSkill::IsEnableEnchant() const
{
	// 배우지 않았으면 강화 불가
	if (false == _isLearn)
	{
		return false;
	}

	// 최대강화 레벨 이상이면 강화 불가
	if (GetMaxEnchantLevel() <= _level)
	{
		return false;
	}

	// 최대 레벨 이상이면 강화 불가
	if (GetMaxLevel() <= _level)
	{
		return false;
	}

	return true;
}

void FGsSkill::SetSkillCycle(bool InActive, bool InActiveInPVP, int32 InSecond)
{
	_isActiveCycle = InActive;
	_isActiveCycleInPVP = InActiveInPVP;
	_cycleSecond = InSecond;
}


float FGsSkill::GetCycleElapsedTime() const
{
	return (FDateTime::Now() - _cycleTimer).GetTotalSeconds();
}

void FGsSkill::UpdateCycleTimer()
{
	_cycleTimer = FDateTime::Now();
}
