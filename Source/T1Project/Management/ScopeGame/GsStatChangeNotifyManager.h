// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Classes/GsManager.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedStatEnum.h"

struct FGsStatChangeInfo
{
public:
	bool _isReady;
	TMap<StatType, int32> _oldStatMap;
	TMap<StatType, int32> _newStatMap;

public:	
	FGsStatChangeInfo() = default;
	FGsStatChangeInfo(const TArray<TPair<StatType, int32>>& inStatArray)
	{
		for (const auto& stat : inStatArray)
		{

		}
	}
	bool Save();
	static bool TryGetPlayerAllStat(TMap<StatType, int32>& outAllStatMap);
	void Update(const TArray<TPair<StatType, int32>>& inStatMap);	
	static bool TryConvertStatMap(const TArray<TPair<StatType, int32>>& inStatArray, OUT TMap<StatType, int32>& outStatMap);
	static bool TryDiffStat(const TMap<StatType, int32>& inOriginStatMap, const TMap<StatType, int32>& inNewStatMap, OUT TMap<StatType, TPair<int32, int32>>& outDiffStatMap);
	void Notify(StatUpdateReason inReasonType);
	static bool Notify(StatUpdateReason inReasonType, const TMap<StatType, int32>& inOriginStatMap, const TMap<StatType, int32>& inNewStatMap);
	void Clear();
	//쌓아서 Notify해야하는 스탯 정보 저장 여부 예 : WindowSeal
	const bool IsNotifyReady() { return _isReady; }
};

class T1PROJECT_API FGsStatChangeNotifyManager : public IGsManager
{
public:
	enum class EGsUpDown
	{
		Up,
		Down,
		Same
	};

	enum class EGsCombatPowerUI
	{
		Off,		
		ShowOnlyCombatPower,
		ShowAll,
		Max
	};

public:
	TMap<StatUpdateReason, TSharedPtr<FGsStatChangeInfo>> _statChangeMap;
	CreatureWeaponType _preWeaponType;
	CreatureWeaponType _curWeaponType;
	TPair<int, int> _levelUpCombatPower;

public:
	void CheckNotify(StatUpdateReason inReasonType, const TArray<TPair<StatType, int32>>& inStatArray);

	//다수의 스탯 업데이트한 후 Notify해야하는 StatUpdateReason
public:
	bool Save(StatUpdateReason inReasonType);	
	bool CheckNotify(StatUpdateReason inReasonType);

public:
	bool IsImmediateReasonType(StatUpdateReason inReasonType);
	void SetPreWeaponType();
	CreatureWeaponType GetPreWeaponType() {
		return _preWeaponType;
	}	
	TPair<int, int> GetLevelUpCombatPower() {
		return _levelUpCombatPower;
	}
};

#define GSStatChangeNotify() UGsScopeHolder::GetGameManagerFType<FGsStatChangeNotifyManager>(UGsScopeGame::EManagerType::StatChangeNotify)
