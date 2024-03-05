
#include "GsStatChangeNotifyManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GsGameObjectManager.h"
#include "UI/UIContent/Tray/GsUITrayTickerStatChange.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UTIL/GsText.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"

bool FGsStatChangeInfo::Save()
{
	Clear();

	_isReady = true;
	//save origin

	TryGetPlayerAllStat(_oldStatMap);


	return true;
}

bool FGsStatChangeInfo::TryGetPlayerAllStat(TMap<StatType, int32>& outAllStatMap)
{
	outAllStatMap.Empty();
	UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == localPlayer)
	{
		return false;
	}

	UGsGameObjectLocalPlayer* castLocalPlayer = Cast<UGsGameObjectLocalPlayer>(localPlayer);
	if (nullptr == castLocalPlayer)
	{
		return false;
	}

	FGsGameObjectDataCreature* creatureData = castLocalPlayer->GetCastData<FGsGameObjectDataCreature>();
	if (nullptr == creatureData)
	{
		return false;
	}

	TWeakPtr<FGsStatBase> stat = creatureData->GetStatBase();
	if (false == stat.IsValid())
	{
		return false;
	}

	const TArray<TPair<StatType, int32>>& statArray = stat.Pin()->GetAllStatInfo();
	return 	TryConvertStatMap(statArray, outAllStatMap);
}

void FGsStatChangeInfo::Update(const TArray<TPair<StatType, int32>>& inStatMap)
{
	for (auto stat : inStatMap)
	{
		_newStatMap.Emplace(stat.Key, stat.Value);
	}
}

bool FGsStatChangeInfo::TryConvertStatMap(const TArray<TPair<StatType, int32>>& inStatArray, OUT TMap<StatType, int32>& outStatMap)
{
	outStatMap.Empty();
	for (auto statInfo : inStatArray)
	{
		outStatMap.Emplace(statInfo.Key, statInfo.Value);
	}

	return outStatMap.Num() > 0;
}

bool FGsStatChangeInfo::TryDiffStat(const TMap<StatType, int32>& inOriginStatMap, const TMap<StatType, int32>& inNewStatMap, OUT TMap<StatType, TPair<int32, int32>>& outDiffStatMap)
{
	outDiffStatMap.Empty();
	for (const auto& newValue : inNewStatMap)
	{
		if (false == inOriginStatMap.Contains(newValue.Key))
		{
			outDiffStatMap.Emplace(newValue.Key, TPairInitializer<int, int>(0, newValue.Value));
			continue;
		}

		const int32 oldValue = inOriginStatMap[newValue.Key];
		if (newValue.Value == oldValue)
		{
			continue;
		}

		outDiffStatMap.Emplace(newValue.Key, TPairInitializer<int, int>(oldValue, newValue.Value));
	}

	return outDiffStatMap.Num() > 0;
}

void FGsStatChangeInfo::Notify(StatUpdateReason inReasonType)
{
	Notify(inReasonType, _oldStatMap, _newStatMap);
	Clear();
}

bool FGsStatChangeInfo::Notify(StatUpdateReason inReasonType, const TMap<StatType, int32>& inOriginStatMap, const TMap<StatType, int32>& inNewStatMap)
{
	float option = 0;
	bool isVisible = false;
	GOption()->GetValue(EGsOptionItem::CombatPower, option, isVisible);
	if (FGsStatChangeNotifyManager::EGsCombatPowerUI::Off == (FGsStatChangeNotifyManager::EGsCombatPowerUI)option)
	{
		return false;
	}

	TMap<StatType, TPair<int32, int32>> statMap;
	TryDiffStat(inOriginStatMap, inNewStatMap, statMap);
	if (0 == statMap.Num())
	{
		return false;
	}

	TWeakObjectPtr<UGsUIWidgetBase> ticker = GUI()->OpenAndGetWidget(TEXT("TrayStatChange"));
	if (false == ticker.IsValid())
	{
		return false;
	}

	UGsUITrayTickerStatChange* tickerChange = Cast<UGsUITrayTickerStatChange>(ticker.Get());
	if (nullptr == tickerChange)
	{
		return false;
	}

	tickerChange->Play(inReasonType, statMap);
	return true;
}

void FGsStatChangeInfo::Clear()
{
	_isReady = false;
	_oldStatMap.Empty();
	_newStatMap.Empty();
}

void FGsStatChangeNotifyManager::CheckNotify(StatUpdateReason inReasonType, const TArray<TPair<StatType, int32>>& inStatArray)
{
#if STAT_CHANGE_NOTIFY_DEBUG
	FString statUpdateReasonText = FGsTextUtil::GetStringFromEnum(TEXT("StatUpdateReason"), inReasonType);

	GSLOG(Log, TEXT("STAT_CHANGE_NOTIFY_DEBUG - stat update reason : %s"), *statUpdateReasonText);
#endif

	if (StatUpdateReason::NONE == inReasonType
		|| StatUpdateReason::MAX == inReasonType
		|| StatUpdateReason::USER_LEVEL_UP == inReasonType
		|| StatUpdateReason::USER_LEVEL_DOWN == inReasonType)
	{
		if (StatUpdateReason::USER_LEVEL_UP == inReasonType)
		{
			const TPair<StatType, int32>* find = inStatArray.FindByPredicate([](const TPair<StatType, int32>& data)->bool {
				return (StatType::COMBAT_POWER == data.Key);
				});
			if (find)
			{
				auto* localPlayer = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer));
				if (localPlayer)
				{
					if (auto* combatPowerStat = localPlayer->GetCreatureStatInfoByType(StatType::COMBAT_POWER))
					{
						_levelUpCombatPower.Key = combatPowerStat->GetStatValue();
					}
				}

				_levelUpCombatPower.Value = find->Value;
			}			
		}

		return;
	}

	//변경 이유가 즉시면 노티파이
	if (IsImmediateReasonType(inReasonType))
	{
#if STAT_CHANGE_NOTIFY_DEBUG
		statUpdateReasonText = FGsTextUtil::GetStringFromEnum(TEXT("StatUpdateReason"), inReasonType);
		GSLOG(Log, TEXT("STAT_CHANGE_NOTIFY_DEBUG - notify immediate - stat update reason : %s"), *statUpdateReasonText);
#endif

		//현재 유저 스탯 저장 
		TMap<StatType, int32> playerAllStatMap;

		//변경된 스탯 비교
		FGsStatChangeInfo::TryGetPlayerAllStat(playerAllStatMap);

		TMap<StatType, int32> newStatMap;
		FGsStatChangeInfo::TryConvertStatMap(inStatArray, newStatMap);

		FGsStatChangeInfo::Notify(inReasonType, playerAllStatMap, newStatMap);
	}
	//변경 이유가 즉시가 아니면 저장
	else
	{
		if (false == _statChangeMap.Contains(inReasonType))
		{
			TSharedPtr<FGsStatChangeInfo> statInfo = MakeShareable<FGsStatChangeInfo>(new FGsStatChangeInfo());
			_statChangeMap.Emplace(inReasonType, statInfo);
		}

		if (false == _statChangeMap[inReasonType].IsValid())
		{
			return;
		}

		_statChangeMap[inReasonType]->Update(inStatArray);

#if STAT_CHANGE_NOTIFY_DEBUG
		statUpdateReasonText = FGsTextUtil::GetStringFromEnum(TEXT("StatUpdateReason"), inReasonType);
		GSLOG(Log, TEXT("STAT_CHANGE_NOTIFY_DEBUG - notify info update - stat update reason : %s"), *statUpdateReasonText);
#endif
	}
}

bool FGsStatChangeNotifyManager::Save(StatUpdateReason inReasonType)
{
	if (false == _statChangeMap.Contains(inReasonType))
	{
		TSharedPtr<FGsStatChangeInfo> statInfo = MakeShareable<FGsStatChangeInfo>(new FGsStatChangeInfo());
		_statChangeMap.Emplace(inReasonType, statInfo);
	}

	if (false == _statChangeMap[inReasonType].IsValid())
	{
		return false;
	}

#if STAT_CHANGE_NOTIFY_DEBUG
	FString statUpdateReasonText = FGsTextUtil::GetStringFromEnum(TEXT("StatUpdateReason"), inReasonType);
	GSLOG(Log, TEXT("STAT_CHANGE_NOTIFY_DEBUG - save - stat update reason : %s"), *statUpdateReasonText);
#endif

	return _statChangeMap[inReasonType]->Save();
}

bool FGsStatChangeNotifyManager::CheckNotify(StatUpdateReason inReasonType)
{
	if (IsImmediateReasonType(inReasonType))
	{
		return false;
	}

	if (false == _statChangeMap.Contains(inReasonType))
	{
		return false;
	}

	if (false == _statChangeMap[inReasonType].IsValid())
	{
		return false;
	}

#if STAT_CHANGE_NOTIFY_DEBUG
	FString statUpdateReasonText = FGsTextUtil::GetStringFromEnum(TEXT("StatUpdateReason"), inReasonType);
	GSLOG(Log, TEXT("STAT_CHANGE_NOTIFY_DEBUG - notify delay - stat update reason : %s"), *statUpdateReasonText);
#endif

	_statChangeMap[inReasonType]->Notify(inReasonType);
	_statChangeMap[inReasonType]->Clear();

	return true;
}

bool FGsStatChangeNotifyManager::IsImmediateReasonType(StatUpdateReason inReasonType)
{
	switch (inReasonType)
	{
	case StatUpdateReason::USER_BONUS_STAT_DISTRIBUTE:
	case StatUpdateReason::USER_BONUS_STAT_RESET:
	case StatUpdateReason::ITEM_EQUIP:
	case StatUpdateReason::FAIRY_EQUIP:
	case StatUpdateReason::COSTUME_EQUIP:
	case StatUpdateReason::GUILD:
	case StatUpdateReason::MONSTER_KNOWLEDGE_UPDATE:
	case StatUpdateReason::SLATE_SPECIAL_NODE_ACTIVATE:
	case StatUpdateReason::PRESET_CHANGE:
		return true;
	}

	return false;
}

void FGsStatChangeNotifyManager::SetPreWeaponType()
{
	_preWeaponType = FGsStatHelper::GetPlayerWeapon();
}
