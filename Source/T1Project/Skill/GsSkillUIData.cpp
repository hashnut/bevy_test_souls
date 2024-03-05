// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillUIData.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillHelper.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Costume/GsCostumeData.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"


void FGsSkillUIData::SetSkill(TWeakPtr<const FGsSkill> InSkill)
{
	_skillPtr = InSkill;

	if (_skillPtr.IsValid())
	{
		_skillId = _skillPtr.Pin()->GetSkillId();
		_weaponType = _skillPtr.Pin()->GetSkillRequireWeaponType();
	}
}

void FGsSkillUIData::SetSkillCurrUpgradeStep()
{
	// 업그레이드 스킬이 있을때만 체크함
	if (0 < _upgradeList.Num())
	{
		int32 index = 0;
		for (int32 i = 0; i < _upgradeList.Num(); ++i)
		{
			if (_upgradeList[i].Pin()->_isLearn)
			{
				// 주의: 해당 무기 장착상태가 아닐때 패시비티가 없으므로 이런식의 체크는 하지 말 것
				//if (FGsSkillHelper::IsRequiredPassivity(_upgradeList[i]))
				{
					index = i;
				}
			}
		}

		SetSkill(_upgradeList[index]);
	}
}

void FGsSkillUIData::UpdateRedDot()
{
	const FGsSkill* skill = GetSkill();
	if (nullptr == skill)
	{
		return;
	}

	_bIsRedDot = FGsSkillHelper::CheckSkillRedDot(skill);
	
	// 레드닷이 강화보다 우선하므로, 레드닷 있으면 강화 체크 안함
	if (false == _bIsRedDot)
	{	
		// 골드등의 재화가 업데이트 될때마다 레드닷 검사를 할 수는 없으므로, 재화량은 검사하지 않음
		_bIsReinforce = FGsSkillHelper::CanReinforce(skill, false);
	}
}

const FGsSkill* FGsSkillUIData::GetSkill()
{
	if (false == _skillPtr.IsValid())
	{
		_skillPtr = GSkill()->FindSkillWeakPtr(_skillId, _weaponType);
	}

	if (_skillPtr.IsValid())
	{
		return _skillPtr.Pin().Get();
	}

	return GSkill()->FindSkill(_skillId, _weaponType);
}

const FGsSkill* FGsSkillUIData::GetFirstChainSkill() const
{
	if (INVALID_SKILL_ID != _firstChainSkillId)
	{
		return GSkill()->FindSkill(_firstChainSkillId, _weaponType);
	}

	return nullptr;
}

const FGsSkill* FGsSkillUIData::GetUpgradeSkillByStep(int32 InStep) const
{
	// 0은 Root스킬이므로, 인덱스여도 -1을 하지 않음
	if (_upgradeList.IsValidIndex(InStep))
	{
		if (_upgradeList[InStep].IsValid())
		{
			return _upgradeList[InStep].Pin().Get();
		}
	}

	return nullptr;
}

const FGsSkill* FGsSkillUIData::GetUpgradeRootSkill() const
{
	return GetUpgradeSkillByStep(0);
}

bool FGsSkillUIData::IsUpgradeSkill() const
{
	return (0 < _upgradeList.Num()) ? true : false;
}

int32 FGsSkillUIData::GetUpgradeStep() const
{
	for (int32 i = 0; i < _upgradeList.Num(); ++i)
	{
		if (_upgradeList[i].IsValid())
		{
			if (_skillId == _upgradeList[i].Pin()->GetSkillId())
			{
				return i;
			}
		}

	}

	return 0;
}

bool FGsSkillUIData::IsCollectedCostume() const
{
	if (_costume)
	{
		TWeakPtr<FGsCostumeData> costumeData = GSCostume()->GetCostumeData(_costume->id);
		if (costumeData.IsValid())
		{
			return costumeData.Pin()->isCollectedCostume();
		}
	}

	return false;
}

bool FGsSkillUIData::IsLearn() const
{
	if (_skillPtr.IsValid())
	{
		if (_costume)
		{
			return (IsCollectedCostume() && _skillPtr.Pin()->_isLearn) ? true : false;
		}

		return _skillPtr.Pin()->_isLearn;
	}

	return false;
}
