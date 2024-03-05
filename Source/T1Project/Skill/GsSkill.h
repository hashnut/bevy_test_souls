// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/User/GsSchemaUserSkillSet.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsSkillSlotData.h"

/**
 * 클라이언트에서 사용되는 SkillData 정보
 */
class T1PROJECT_API FGsSkill
{
public:
	const FGsSchemaSkillSet* _tableData = nullptr;
	const FGsSchemaUserSkillSet* _userSkillSetData = nullptr;
	bool _isLearn = false;
	bool _isNew = false; // 신규 획드 스킬 레드닷 체크
	bool _isSelectTarget = false; // 스킬 사용시 타겟 설정 인터페이스 출력 여부

	SkillSlotId _slotId = 0;
	SkillId _prevSkillId = INVALID_SKILL_ID;

	FGsSkillSlotData _slotData;
	// 스킬 주기 설정용 타이머
	FDateTime _cycleTimer;

	TMap<PresetId, TPair<SkillSlotId, bool>> _presetData; // <프리셋ID, <슬롯ID, Auto여부>>

	// 스킬 레벨 테이블 캐싱
	mutable TMap<int32, const struct FGsSchemaSkillLevelInfo*> _levelInfoMap;

private:
	int32 _level = 1; // 강화로 올린 레벨
	int32 _bonusLevel = 0; // 코스튬 패시비티 등으로 추가로 올라간 레벨
	int32 _cachedMaxLevel = 0; // FGsSkill 생성 시 캐싱해둔다(_level + _optionLevel 로 가능한 최대레벨)	

	bool _isActiveCycle = false; // 스킬 사용 주기가 설정되어 있는가
	bool _isActiveCycleInPVP = false; // PVP 중 스킬 사용 주기를 사용할 것인가
	int32 _cycleSecond = 0; // 스킬 쿨타임 후 몇 초를 대기한 후 사용할 것인가

public:
	FGsSkill();
	virtual ~FGsSkill() = default;

public:
	FGsSkill& operator = (const FGsSkill& SkillData);
	FGsSkill& operator = (const FGsSkill* SkillData);

	void Set(const FGsSkill& SkillData);
	void Set(const FGsSkill* SkillData);

public:
	// SkillSlotData 정보 설정
	void SetUseAuto(bool UseAuto);
	void SetReserve(bool Reserve);
	void SetSkillCheckResult(EGsSkillSlotCheckType Type, bool Isfail);
	void SetPresetInfo(PresetId InPresetId, SkillSlotId	InSlotId, bool InUseAuto);

	const FGsSkillSlotData& GetSlotData() const { return _slotData; }

public:
	const FSoftObjectPath* GetIconPath() const;
	const SkillId GetUserSkillSetId() const;

	const struct FGsSchemaSkillLevelSet* GetCurrLevelSet() const;
	const struct FGsSchemaSkillLevelInfo* GetLevelInfo(int32 InLevel) const;
	static const struct FGsSchemaSkillLevelInfo* GetLevelInfo(int32 InId, int32 InLevel);

	//void GetCurrLevelCostConsumeInfoData(OUT struct FGsSchemaSkillCostConsumeInfo& outConsumeInfoList);
	const struct FGsSchemaSkillCostConsumeInfo* GetCurrLevelCostConsumeInfoData(int32 InLevel) const;

	int32 GetSkillId() const { return (_tableData) ? _tableData->id : -1; }
	SkillSlotId GetSlotId() const { return _slotId; }
	int32 GetSkillGroupId() const { return (_tableData) ? _tableData->skillGroupId : 0; }
	float GetUIEnableDelayTime() const { return (_tableData) ? _tableData->enableDelayTime : 0.f;  }
	bool IsFirstNormalSkill() const;
	bool IsNormalSkill() const;
	SkillCategorySet GetSkillCateogrySet() const;
	CreatureWeaponType GetSkillRequireWeaponType() const { return (_tableData) ? _tableData->requireWeapon : CreatureWeaponType::NONE; }
	IffApplyType GetIffType() const { return (_tableData) ? _tableData->targetFilterInfo.iffType : IffApplyType::NONE; }

	bool IsEnable() const;
	bool IsReserve() const;
	bool IsAuto() const;
	bool IsSelectTarget() const;
	bool IsComboSkill() const;
	bool IsBuffSkill() const;

	// FGsSchemaSkillSet의 cooldownTime은 몬스터들 용이며, 내 캐릭터는 FGsSchemaSkillLevelSet을 써야한다.
	// 실수하지 않기 위해 별도 함수로 만든다
	float GetCooldownTime(int32 InLevel) const;
	float GetCurrentCooldownTime() const;

	int32 GetUpgradeStep() const;
	int32 GetMaxUpgradeStep() const;
	int32 GetChainStep() const;
	int32 GetMaxChainStep() const;
	bool IsChainSkill() const;
	bool IsLastChainStep() const;
	bool IsFirstSkill() const;
	bool IsFirstChainSkill() const;
	bool IsCostumeSkill() const;
	SkillId GetPrevSkillId() const { return _prevSkillId; }
	// RootSkillId를 구하려면 FGsSkillHelper::GetUpgradeRootSkillId 를 사용하자. 로직이 복잡해서 뺐음.

	// Level, 강화 관련 함수
public:
	void CacheMaxLevel();

	// 스킬 강화레벨을 체크할 때는 아래 내용을 분리해서 볼 것
	void SetLevel(int32 InLevel) { _level = InLevel; }
	void SetBonusLevel(int32 InBonusLevel) { _bonusLevel = InBonusLevel; }

	// 실제 적용되는 값은 GetTotalLevel로 사용
	int32 GetTotalLevel() const;
	int32 GetLevel() const { return _level; } // 강화로 올린 레벨
	int32 GetBonusLevel() const { return _bonusLevel; } // 배운 스킬만 표시	
	int32 GetMaxLevel() const { return _cachedMaxLevel; } // 스킬 최대 레벨(levelSet 테이블에 데이터가 존재하는 최종 레벨) 
	int32 GetMaxEnchantLevel() const; // 강화로 올릴 수 있는 최대 레벨(GetMaxLevel보다 작거나 같은 값)
	
	bool IsEnableEnchant() const;

	// 스킬 주기 설정
public:
	void SetSkillCycle(bool InActive, bool InActiveInPVP, int32 InSecond);

	bool IsActiveSkillCycle() const { return _isActiveCycle; }
	bool IsActiveSkillCycleInPVP() const { return _isActiveCycleInPVP; }
	int32 GetCycleSecond() const { return _cycleSecond; }

	// 스킬 주기 타이머는 Tick처리를 하며 갱신할 필요가 없다.
	float GetCycleElapsedTime() const;
	void UpdateCycleTimer();
};
