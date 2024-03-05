// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsStatInfo.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "Stat/GsSchemaStatConvertTable.h"

class UGsGameObjectBase;
class UGsGameObjectCreature;
class FGsStatInfo;


// Stat Change
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeStatDelegate, const FGsStatInfo&);
/*
// Attack DealScroll Delegate
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttackDelegate, UGsGameObjectBase& , int64, EAttackType);
*/

/**
 *   Stat Base Class
 */
class T1PROJECT_API FGsStatBase
{
protected:
	TMap<StatType, FGsStatInfo*> _statGroup;
	TWeakObjectPtr<UGsGameObjectBase> _ownerObject;

public:
	//특정 크리쳐의 스텟만 follow up 하기위함.(범위 공격으로 전체 message handler 에 어려움이 있음)
	FOnChangeStatDelegate	OnChangeStatDelegate;
	 
public:
	FGsStatBase() = default;
	virtual ~FGsStatBase() = default;

public:
	virtual void Initialize(UGsGameObjectBase* owner);
	virtual void Finalize();
	virtual void Update();

public:
	virtual void UpdateStatInfo(const TArray<TPair<StatType, int32>>& InStatInfoMap);
	void UpdateStatInfo(const StatType inStatType, const int32 inValue);

protected:
	void SetStatInfo(StatType InStatType, int32 InValue);
	void SetMaxStatInfo(StatType InType, int32 InMaxValue);

public:
	//HP, MAX_HP와 같이 최대 값 관계가 존재하는 스탯 을 판별하기 위한 Stat
	static StatType GetPairMaxStatType(StatType InStatType);
	static StatType GetMaxStatTypeFromStatType(const StatType inStatType);

private:
	void UpdateMaxStatInfo(const StatType InType, const int32 InValue);

public:
	const FGsStatInfo* GetStatInfo(StatType InStatType) const;
	TMap<StatType, FGsStatInfo*> GetStatGroup(){return _statGroup;}
	const TArray<TPair<StatType, int32>> GetAllStatInfo();

public:
	void CalculateStat(uint32 InDmgValue, DamageStatType InDmgType);

public:
	bool IsAquireCostSkillType(const StatType InType);
	bool IsMaxStatType(StatType InStatType) const;
	bool IsZeroHP() const;

public:
	//클라이언트 내에서 2차 스탯을 계산 해야할 경우가 있을 때 계산하기 위한 함수(예 : 스탯 분배)
	void IncreaseStat(const StatType inStat, const int32 inValue);
	void DecreaseStat(const StatType inStat, const int32 inValue);

private:	
	bool TryGetConvertData(OUT FGsSchemaStatConvertTable& outData, const StatType inStatType);
	bool TryGetConvertData(OUT int32& outData, const StatType inStatType, const StatType inSecondStatType);
};

//void DropExp(UGsGameObjectCreature* InAttackerCreatureInfo);	
//void GetSkillCoefficientValue(UGsGameObjectCreature* InAttackerCreatureInfo , float& outValue);
//void CalculateStat(UGsGameObjectCreature* InAttackerCreatureInfo , float InSkillCoefficientValue);
//void CalculateStat(UGsGameObjectProjectile* InAttackerProjectTileInfo);
//void MessageSendProc(StatType InStatType, FGsStatInfo* InInfo);
//void SendLocalPlayerProc(StatType InStatType, FGsStatInfo* InInfo);
//void SendNonPlayerProc(StatType InStatType, FGsStatInfo* InInfo);