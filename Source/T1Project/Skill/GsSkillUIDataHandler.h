// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Skill/GsSkillUIData.h"

class FGsSkill;
class FGsSkillUIData;
struct IGsMessageParam;

/**
 * 스킬 UI 관련 데이터들을 관리
 * 주의: FGsSkillUIData 정보는 스킬창에서만 이용한다
 */
class T1PROJECT_API FGsSkillUIDataHandler final
{
public:
	using DefCategorySkillDataMap = TMap<SkillCategorySet, TArray<FGsSkillUIData>>;

private:	
	const SkillCategorySet COSTUME_CATEGORY_TYPE = SkillCategorySet::MAX;

	// 코스튬 스킬 캐싱해두기
	TMap<CostumeId, TWeakPtr<const FGsSkill>> _costumeSkillMap;

	// 스킬창 열 때 사용함에 유의
	TMap<CreatureWeaponType, DefCategorySkillDataMap> _skillDataMap;

	// 스킬 습득, 레드닷 체크 위한 보유 스킬북 저장
	TMap<SkillId, TArray<ItemId>> _inventorySkillBookMap;

	// 스킬창 열 때, 특정 스킬을 선택된 채로 열 경우 사용
	SkillId _windowOpenSelectedSkillId = INVALID_SKILL_ID;

	bool _bIsSkillBookCached = false;

public:
	FGsSkillUIDataHandler() = default;
	~FGsSkillUIDataHandler();
	
public:
	void Initialize();
	void Finalize();

	// UGsSkillManager::InitializeData 시
	void InitializeData(CreatureGenderType InGender);
	// UGsSkillManager::ClearData() 시
	void ClearData();		

	// 스킬창을 열 때 만든다(필요한 타이밍에 쓰기 위함)
	void InitSkillDataMap();

private:
	void InitCostumeSkillMap(CreatureGenderType InGender);

	void MakeSkillDataMap(CreatureWeaponType InWeapon, SkillCategorySet InCategory);
	bool CheckAndMakeChainSkill(TWeakPtr<const FGsSkill> InSkill, OUT FGsSkillUIData& OutData);
	void MakeCostumeSkillDataMap();

	// InSkill 기준으로 다음 체인 스킬을 찾아서 OutList에 순서대로 채움
	void RecursiveGetSkillListByChainStep(TWeakPtr<const FGsSkill> InSkill, OUT TArray<TWeakPtr<const FGsSkill>>& OutList);

public:
	SkillId GetWindowOpenSelectedSkillId() const { return _windowOpenSelectedSkillId; }
	void SetWindowOpenSelectedSkillId(SkillId InSkillId) { _windowOpenSelectedSkillId = InSkillId; }

	TMap<CreatureWeaponType, DefCategorySkillDataMap>& GetSkillDataMap() { return _skillDataMap; }
	TMap<SkillCategorySet, TArray<FGsSkillUIData>>* GetCategoryMapPtr(CreatureWeaponType InType);
	FGsSkillUIData* GetSkillUIData(CreatureWeaponType InType, SkillCategorySet InCategory, SkillId InSkillId);
	CostumeId GetCostumeIdBySkillId(SkillId InSkillId) const;

	// 습득한 코스튬 스킬이 있는지 체크
	bool IsLearnedCostumeSkill() const;

	// 스킬 습득, 레드닷 체크 위한 보유 스킬북 캐싱
public:
	void SetIsSkillBookCached(bool bIsCached) { _bIsSkillBookCached = bIsCached; }
	bool GetIsSkillBookCached() const { return _bIsSkillBookCached; }
	void AddSkillBook(SkillId InSkillId, ItemId InSkillBookId);
	void RemoveSkillBook(ItemId InSkillBookId);
	const TArray<ItemId>* GetExistSkillBookList(SkillId InSkillId) const;
	bool IsCollectedCostume(CostumeId InCostumeId) const;
	TWeakPtr<const FGsSkill> GetSkillByCostumeId(CostumeId InCostumeId);

	// 스킬창 MP관련 연산
public:
	float CalcMPRecovery();
	float CalcMpUsage(CreatureWeaponType InWeapon);
private:
	float CalcMPUsageInter(const FGsSkill* InSkill, float InCoolTimeRate, float InRegenerationTick);

	float GetBuffSkillDuration(const FGsSkill* InSkill);
};
