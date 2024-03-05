// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Message/GsMessageGameObject.h"

#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GameObject/Stat/GsStatDefine.h"
#include "GameObject/Stat/GsStatBase.h"

#include "Item/Data/GsItemStatData.h"


/**
 *
 */
struct FGsBonusStat
{
public:
	StatType statType;
	int32 statValue;
};

const TArray<StatType> MAIN_STAT_ARRAY = {
StatType::STR,
StatType::DEX,
StatType::INT,
StatType::CON,
StatType::AGI,
StatType::WIS,
};

class T1PROJECT_API FGsPlayerStatDistributionController
{
private:
	//서버에서 받은 현재 유저가 분배할 수 있는 스탯 포인트 양
	int32 _remainBonusStatPoint = 0;
	//서버에 보낼 유저가 분배한 스탯 포인트
	int32 _usedBonusStatPoint;
	//MAX value 변경시 서버에서 알려줄 것 
	int32 _maxStatValue = 20;
	//현재 플레이어의 스탯 
	TUniquePtr<class FGsStatBase> _originStat;
	//분배 된 후 변화된 스탯, 2차 스탯 계산을 위해 존재
	TUniquePtr<class FGsStatBase> _nextStat;
	//무기, 버프, 페어리 등 합친 현재 스탯
	TMap<StatType, int32> _originalStatMap;
	//서버에 저장된 이전에 분배했던 스탯
	TMap<StatType, int32> _oriDistributeStatMap;
	//분배한 스탯 스탯
	TMap<StatType, int32> _distributedStatMap;
	//분배한 스탯을 백업 하기 위한 스탯
	TMap<StatType, int32> _backUpDistributedStatMap;
	TSharedPtr<FGsStatBase> _playerStat;

	MsgGameObjHandle _userDataHandle;

public:
	//초기화
	void Init();
	//BackToLobby 때 호출
	void Close();
	//종료시 호출
	void Reset();

	//초기화
private:
	void InitRemainStatPoint();
	void SetStatData();
	void InitOriginalStat();
	void InitOriginalDistribution();
	void ClearStatDistribution();
	void OnInitRemainStatPoint(const struct IGsMessageParam* inParam);

	//stat calculate
private:
	void InvalidateSecondaryStats();
	bool TryGetOriginStatValue(OUT int32& outValue, StatType inStatType);
	bool TryGetNextStatValue(OUT int32& outValue, StatType inStatType);
	bool TryGetStatValue(OUT int32& outValue, const TUniquePtr<class FGsStatBase>& inStat, StatType inStatType);
	TArray<TPair<StatType, int32>> GetBonusStatTypeMap();
	int32 GetMaxHp(int32 inValue);
	int32 GetMaxMp(int32 inValue);
	int32 GetCurrentPlayerStatValue(StatType inStatType);
	const struct FGsItemStatData* GetConvertData(StatType inType, int32 inValue, StatType inSecondType);
	const struct FGsSchemaStatConvertTable* GetConvertData(StatType inStatType, int32 inStatPoint);
	bool IsOriginalDistributedStatMax(StatType inType);

	//external call
public:
	void InitStat();
	void ResetStatDistribution();
	void ResetBackupStatDistribution();
	void Recalculate();
	void BackUpDistributedPoint();
	void UpdateStatPoint();

	//get set
public:
	bool IncreaseStat(StatType inType, int32 inPoint = 1);
	bool DecreaseStat(StatType inType, int32 inPoint = 1);
	bool CanIncrease(StatType inType);
	bool CanDecrease(StatType inType);
	int32 GetRemainBonusStatPoint() { return _remainBonusStatPoint; }
	int32 GetDistributeStatPoint() { return _usedBonusStatPoint; }
	int32 GetCurrentRemainBonusStatPoint();
	int32 GetCurrentDistributeStatPoint(StatType inStatType);
	bool TryGetStatValue(OUT FText& outStatValue, StatType inType);
	bool TryGetSecondaryStatValue(OUT TArray<FGsBonusStat>& outStatValue, StatType inType);
	bool TryGetSecondaryStatValueText(OUT TArray<FText>& outStatValue, StatType inType);
	bool TryGetSecondaryStatValueAndText(OUT TArray<FGsBonusStat>& outStatValue, OUT TArray<FText>& outStatValueText, StatType inType);
	bool TryCheckChangeSecondaryStatValue(OUT TArray<bool>& outStatChange, StatType inType);
	bool TryGetUsedBonusStatList(OUT TArray<StatInfo>& outBonusStatList);
	int32 GetOriginalDistributePoint(StatType inStatType);
	static bool IsSecondaryStat(StatType inStatType);
	bool IsMaxStat(StatType inStatType);
};
