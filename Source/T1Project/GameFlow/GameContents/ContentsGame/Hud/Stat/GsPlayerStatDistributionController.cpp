// Fill out your copyright notice in the Description page of Project Settings.


#include "GsPlayerStatDistributionController.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "DataSchema/Stat/GsSchemaStatConvertData.h"
#include "DataSchema/Stat/GsSchemaStatConvertTable.h"
#include "DataSchema/Stat/GsSchemaStatFormatingData.h"
#include "DataSchema/Stat/GsSchemaBonusStatConfig.h"

#include "Item/GsItemManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/Stat/GsStatHelper.h"

#include "UTIL/GsTableUtil.h"


void FGsPlayerStatDistributionController::Init()
{
	Reset();

	int32 num = MAIN_STAT_ARRAY.Num();
	_distributedStatMap.Empty();
	for (int i = 0; i < num; ++i)
	{
		_distributedStatMap.Add(MAIN_STAT_ARRAY[i], 0);
	}

	//Hud가 UserDataBase 이후에 생기므로 콜백으로 RemainBonusStatPoint를 초기화 하기엔 시점이 느리다
	//DistributionHandler 초기화시 _remainBonusStatPoint를 초기화 할 것 
	//또한 UserData의 StatPoint를 늘 최신화 할 것
	InitRemainStatPoint();

	_usedBonusStatPoint = 0;

	_originStat = TUniquePtr<FGsStatBase>(new FGsStatBase());
	_nextStat = TUniquePtr<FGsStatBase>(new FGsStatBase());


	if (const UGsTableBonusStatConfig* statConfigTable = Cast<UGsTableBonusStatConfig>(FGsSchemaBonusStatConfig::GetStaticTable()))
	{
		TArray<const FGsSchemaBonusStatConfig*> rows;
		if (statConfigTable->GetAllRows(rows))
		{
			if (rows.Num() > 0)
			{
				_maxStatValue = rows[0]->bonusStatDistributionMax;
#if WITH_EDITOR
				GSLOG(Log, TEXT("FGsStatDistributionHandler - init - max stat value : %d"), _maxStatValue);
#endif
			}
		}
	}
	
	_userDataHandle = GMessage()->GetGameObject().AddRaw(MessageGameObject::UPDATE_USER_DATA, this, &FGsPlayerStatDistributionController::OnInitRemainStatPoint);
}

void FGsPlayerStatDistributionController::Close()
{
	if (_userDataHandle.Value.IsValid())
	{
		GMessage()->GetGameObject().Remove(_userDataHandle);
	}

	ClearStatDistribution();
}

void FGsPlayerStatDistributionController::Reset()
{
	Close();	
}

void FGsPlayerStatDistributionController::InitStat()
{
	SetStatData();
	InitOriginalStat();
	InitOriginalDistribution();
}

void FGsPlayerStatDistributionController::ResetStatDistribution()
{
	_usedBonusStatPoint = 0;

	for (TPair<StatType, int32>& pair : _distributedStatMap)
	{
		pair.Value = 0;
	}
}

void FGsPlayerStatDistributionController::ResetBackupStatDistribution()
{
	for (TPair<StatType, int32>& pair : _backUpDistributedStatMap)
	{
		pair.Value = 0;
	}
}

void FGsPlayerStatDistributionController::Recalculate()
{
	InitRemainStatPoint();
	ResetStatDistribution();

	for (TPair<StatType, int32> pair : _backUpDistributedStatMap)
	{
#if WITH_EDITOR && STAT_DISTRIBUTION_DEBUG
		const FString& statName = FGsTextUtil::GetStringFromEnum(TEXT("EPlusStatType"), pair.Key);
		GSLOG(Log, TEXT("FGsStatDistributionHandler - Recalculate - type : %s\t point : %d"), *statName, pair.Value);
#endif

		int32 distributedStat = _distributedStatMap.Contains(pair.Key) ? _distributedStatMap[pair.Key] : 0;
		int32 backukp = _backUpDistributedStatMap.Contains(pair.Key) ? _backUpDistributedStatMap[pair.Key] : 0;
		if (distributedStat != backukp)
		{
			IncreaseStat(pair.Key, backukp);
		}
	}
}

void FGsPlayerStatDistributionController::InitRemainStatPoint()
{
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		_remainBonusStatPoint = userData->_statPoint;

#if WITH_EDITOR && STAT_DISTRIBUTION_DEBUG
		GSLOG(Log, TEXT("FGsStatDistributionHandler - InvalidRemainStatPoint - remainBonusStatPoint : %d"), _remainBonusStatPoint);
#endif
	}
}

void FGsPlayerStatDistributionController::BackUpDistributedPoint()
{
	_backUpDistributedStatMap = _distributedStatMap;
}

//스탯 분배 성공
void FGsPlayerStatDistributionController::UpdateStatPoint()
{
	//백업 정보 삭제
	_backUpDistributedStatMap.Empty();
	//현재 UI에서 분배한 스탯 초기화
	ResetStatDistribution();
	//플레이어의 스탯으로 업데이트
	InitStat();
	//보너스 스탯 업데이트
	InitRemainStatPoint();
}

/*
1차 스탯 분배 : STR, DEX 등...
*/
bool FGsPlayerStatDistributionController::IncreaseStat(StatType inStatType, int32 inPoint)
{
	if (false == CanIncrease(inStatType))
	{
		return false;
	}
	
	if (false == _distributedStatMap.Contains(inStatType))
	{
		return false;
	}

	int32 currentDistributePoint = _distributedStatMap[inStatType];
	int32 originDistributePoint = GetOriginalDistributePoint(inStatType);

	//분배 된 점수(서버에 저장된 이전 스탯 점수 + 현재 분배한 점수 + 지금 더한 점수)
	int32 distributedPoint = originDistributePoint + currentDistributePoint + inPoint;
	//남아있는 스탯 포인트
	int32 remainPoint = GetCurrentRemainBonusStatPoint();
	//현재 할당할 포인트
	int32 resultPoint = inPoint;

	//최대 할당량을 넘을 경우
	if (distributedPoint > _maxStatValue)
	{
		resultPoint = _maxStatValue - originDistributePoint - currentDistributePoint;

#if WITH_EDITOR && STAT_DISTRIBUTION_DEBUG
		GSLOG(Log, TEXT("FGsStatDistributionHandler - IncreaseStat - over than max -  max : %d\torigin : %d\tresult : %d"), _maxStatValue, originDistributePoint, resultPoint);
#endif
	}

	//할당된 포인트보다 더 많이 분배한 경우
	if (resultPoint > remainPoint)
	{
		resultPoint = remainPoint;

#if WITH_EDITOR && STAT_DISTRIBUTION_DEBUG
		GSLOG(Log, TEXT("FGsStatDistributionHandler - IncreaseStat - over than remain -  max : %d\torigin : %d\tresult : %d"), _maxStatValue, originDistributePoint, resultPoint);
#endif
	}

	_distributedStatMap[inStatType] += resultPoint;
	_usedBonusStatPoint += resultPoint;

#if WITH_EDITOR && STAT_DISTRIBUTION_DEBUG
	FString statName = FGsTextUtil::GetStringFromEnum(TEXT("EPlusStatType"), inStatType);
	GSLOG(Log, TEXT("FGsStatDistributionHandler - IncreaseStat - type : %s\t point : %d\tused stat point : %d"), *statName, _distributedStatMap[inStatType], _usedBonusStatPoint);
#endif

	InvalidateSecondaryStats();

	return true;
}

bool FGsPlayerStatDistributionController::DecreaseStat(StatType inType, int32 inPoint)
{
	if (false == CanDecrease(inType))
	{
		return false;
	}

	if (false == _distributedStatMap.Contains(inType))
	{
		return false;
	}

	int32 currentDistributePoint = _distributedStatMap[inType];
	int32 point = inPoint;
	int32 resultPoint = point;

	//이미 사용된 포인트가 할당된 포인트를 넘을 경우 
	if ((currentDistributePoint - point) < 0)
	{
		resultPoint = currentDistributePoint;
	}

	_distributedStatMap[inType] -= resultPoint;
	_usedBonusStatPoint -= resultPoint;

	InvalidateSecondaryStats();

	return true;
}

bool FGsPlayerStatDistributionController::CanIncrease(StatType inType)
{
	if (false == _distributedStatMap.Contains(inType))
	{
		return false;
	}

	//서버에 저장된 스탯 + 현재 분배한 스탯 > 최대 분배 가능한 스탯
	if ((GetOriginalDistributePoint(inType) + _distributedStatMap[inType]) >= _maxStatValue)
	{
		return false;
	}

	//분배할 스탯이 없을 때 증가 불가 
	return (_remainBonusStatPoint > 0) && ((_remainBonusStatPoint - _usedBonusStatPoint) > 0);
}

bool FGsPlayerStatDistributionController::CanDecrease(StatType inType)
{
	if (false == _distributedStatMap.Contains(inType))
	{
		return false;
	}

	//현재 스탯이 최대 스탯에 도달하면 스탯을 감소 시킬 수 없다.
	if (IsOriginalDistributedStatMax(inType))
	{
		return false;
	}

	//분배된 스탯이 0보다 크면 스탯을 감소 시킬 수 있다
	return _distributedStatMap[inType] > 0;
}

bool FGsPlayerStatDistributionController::IsOriginalDistributedStatMax(StatType inType)
{
	return (GetOriginalDistributePoint(inType) == _maxStatValue);
}

int32 FGsPlayerStatDistributionController::GetCurrentRemainBonusStatPoint()
{
	int32 sumStatPoint = 0;

	for (const TPair<StatType, int32>& pair : _distributedStatMap)
	{
		sumStatPoint += pair.Value;
	}

	return (_remainBonusStatPoint - sumStatPoint);
}

int32 FGsPlayerStatDistributionController::GetCurrentDistributeStatPoint(StatType inStatType)
{
	if (false == _distributedStatMap.Contains(inStatType))
	{
		return 0;
	}

	return _distributedStatMap[inStatType];
}

const FGsSchemaStatConvertTable* FGsPlayerStatDistributionController::GetConvertData(StatType inStatType, int32 inStatPoint)
{	
	const FGsSchemaStatConvertData* statConvert = UGsTableUtil::FindRowById<UGsTableStatConvertData, FGsSchemaStatConvertData>(inStatType);
	if (nullptr == statConvert)
	{
		return nullptr;
	}

	const FGsSchemaStatConvertTable* find = statConvert->convertList.FindByPredicate([&inStatPoint](const FGsSchemaStatConvertTable& statConvertTable)->bool {
		return statConvertTable.baseValue == inStatPoint;
	});

	return find;
}

void FGsPlayerStatDistributionController::OnInitRemainStatPoint(const IGsMessageParam* inParam)
{
	const FGsNetUserData* data = inParam->Cast<const FGsNetUserData>();
	if (nullptr == data)
	{
		return;
	}

	Recalculate();
}

bool FGsPlayerStatDistributionController::TryGetStatValue(OUT FText& outStatValue, StatType inStatType)
{
	int32 originValue = 0;
	if (false == TryGetOriginStatValue(originValue, inStatType))
	{
		return false;
	}

	if (false == _distributedStatMap.Contains(inStatType))
	{
		return false;
	}

	int32 statValue = originValue + _distributedStatMap[inStatType];
	if (false == FGsStatHelper::GetDisPlayStatValueWithMarkFormat(inStatType, statValue, outStatValue))
	{
		FString statName = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), inStatType);
		GSLOG(Error, TEXT("Stat format not exist - stat name : %s"), *statName);
		return false;
	}

	return true;
}

//서버 데이터 - 이전 스탯 + 현재 스탯
bool FGsPlayerStatDistributionController::TryGetSecondaryStatValue(OUT TArray<FGsBonusStat>& outStatValue, StatType inStatType)
{
	outStatValue.Empty();

	if (false == _nextStat.IsValid())
	{
		return false;
	}

	FGsSchemaStatConvertTable convertData;	
	const FGsStatInfo* statInfo = _nextStat->GetStatInfo(inStatType);
	int32 statValue = statInfo->GetStatValue();
	if (false == FGsStatHelper::TryGetConvertData(convertData, inStatType, statValue))
	{
		return false;
	}

	int32 num = convertData.statList.Num();	
	const FGsStatInfo* nextStatInfo = nullptr;

	for (int32 i = 0; i < num; ++i)
	{
		FGsBonusStat temp;

		//MAX_HP 와 MAX_MP 예외처리
		if (IsSecondaryStat(convertData.statList[i].type))
		{
			if (convertData.statList[i].type == StatType::MAX_HP)
			{
				temp.statType = StatType::MAX_HP;
				int32 value = _nextStat->GetStatInfo(StatType::CON)->GetStatValue();
				temp.statValue = GetMaxHp(value);
			}
			else if (convertData.statList[i].type == StatType::MAX_MP)
			{
				temp.statType = StatType::MAX_MP;
				int32 value = _nextStat->GetStatInfo(StatType::WIS)->GetStatValue();
				temp.statValue = GetMaxMp(value);
			}
		}
		else
		{
			temp.statType = convertData.statList[i].type;
			nextStatInfo = _nextStat->GetStatInfo(temp.statType);
			if (nullptr == nextStatInfo)
			{
				GSLOG(Error, TEXT("Secondary stat is missing : %s"), *FGsTextUtil::GetStringFromEnum(TEXT("StatType"), temp.statType));
				break;
			}

			temp.statValue = _nextStat->GetStatInfo(temp.statType)->GetStatValue();
		}

		outStatValue.Add(temp);
	}

	return true;
}

bool FGsPlayerStatDistributionController::TryGetSecondaryStatValueText(OUT TArray<FText>& outStatValue, StatType inType)
{
	outStatValue.Empty();
	TArray<FGsBonusStat> statInfoList;
	if (false == TryGetSecondaryStatValue(statInfoList, inType))
	{
		return false;
	}

	int32 num = statInfoList.Num();
	int32 detailStatValue;
	StatType statType;
	FText formattedDetailStatValue;

	for (int32 i = 0; i < num; ++i)
	{
		detailStatValue = statInfoList[i].statValue;
		statType = statInfoList[i].statType;

		if (IsSecondaryStat(statType))
		{
			if (statType == StatType::MAX_HP)
			{
				const FGsStatInfo* info = _nextStat->GetStatInfo(StatType::CON);
				if (info)
				{
					detailStatValue = GetMaxHp(info->GetStatValue());
				}
			}
			else if (statType == StatType::MAX_MP)
			{
				const FGsStatInfo* info = _nextStat->GetStatInfo(StatType::WIS);
				if (info)
				{
					detailStatValue = GetMaxMp(info->GetStatValue());
				}
			}
		}
		
		if (false == FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statType, detailStatValue, formattedDetailStatValue))
		{
			FString statName = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), statType);
			GSLOG(Error, TEXT("Stat format not exist - stat name : %s"), *statName);
			continue;
		}

		outStatValue.Add(formattedDetailStatValue);
	}

	return true;
}

bool FGsPlayerStatDistributionController::TryGetSecondaryStatValueAndText(OUT TArray<FGsBonusStat>& outStatValue, OUT TArray<FText>& outStatValueText, StatType inType)
{
	if (false == TryGetSecondaryStatValue(outStatValue, inType))
	{
		return false;
	}

	int32 statNum = outStatValue.Num();
	FText formattedDetailStatValue;
	for (int32 i = 0; i < statNum; ++i)
	{
		if (false == FGsStatHelper::GetDisPlayStatValueWithMarkFormat(outStatValue[i].statType, outStatValue[i].statValue, formattedDetailStatValue))
		{
			FString statName = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), outStatValue[i].statType);
			GSLOG(Error, TEXT("Stat format not exist - stat name : %s"), *statName);
			continue;
		}

		outStatValueText.Add(formattedDetailStatValue);
	}

	return true;
}

bool FGsPlayerStatDistributionController::TryCheckChangeSecondaryStatValue(OUT TArray<bool>& outStatChange, StatType inStatType)
{
	outStatChange.Empty();
	TArray<FGsBonusStat> statInfoList;
	if (false == TryGetSecondaryStatValue(statInfoList, inStatType))
	{
		return false;
	}

	int32 num = statInfoList.Num();
	StatType statType = StatType::MAX;
	int32 originValue = 0;
	int32 nextValue;
	bool change;

	for (int32 i = 0; i < num; ++i)
	{		
		statType = statInfoList[i].statType;
		if (false == TryGetOriginStatValue(originValue, statType))
		{
			continue;
		}

		nextValue = statInfoList[i].statValue;
		change = originValue < nextValue;

		outStatChange.Add(change);
	}

	return true;
}

bool FGsPlayerStatDistributionController::TryGetUsedBonusStatList(OUT TArray<StatInfo>& outBonusStatList)
{
	outBonusStatList.Empty();
	for (const TPair<StatType, int32>& pair : _distributedStatMap)
	{
		if (pair.Value > 0)
		{			
			outBonusStatList.Add(StatInfo(pair.Key, pair.Value));
		}
	}

	return outBonusStatList.Num() > 0;
}

int32 FGsPlayerStatDistributionController::GetOriginalDistributePoint(StatType inStatType)
{
	return (_oriDistributeStatMap.Contains(inStatType) ? _oriDistributeStatMap[inStatType] : 0);
}

bool FGsPlayerStatDistributionController::IsSecondaryStat(StatType inStatType)
{
	return inStatType == StatType::MAX_HP || inStatType == StatType::MAX_MP;
}

/*
현재 클라이언트에는 스탯을 계산하는 로직이 없다. 
스탯 분배 후 증가한 MaxHp 값을 알기 위해서는
현재 MaxHp - Convert(Current Con) - (MAX_HP_VARIATION_AMOUNT * MAX_HP_VARIATION_RATE / 10000) = 아이템, 제련 등으로 인한 HP 증가 값
아이템, 제련 등으로 인한 HP 증가 값 + Convert(Next con) + (MAX_HP_VARIATION_AMOUNT * MAX_HP_VARIATION_RATE / 10000) = 다음 MaxHp 증가 값 
으로 계산해야한다. 
*/
int32 FGsPlayerStatDistributionController::GetMaxHp(int32 inValue)
{	
	float currentCon = _originStat->GetStatInfo(StatType::CON)->GetStatValue();
	const FGsItemStatData* currentStatData = GetConvertData(StatType::CON, currentCon, StatType::MAX_HP);
	if (currentStatData)
	{
		const FGsItemStatData* nextCon = GetConvertData(StatType::CON, inValue, StatType::MAX_HP);
		if (nextCon)
		{
			float current_max_hp = GetCurrentPlayerStatValue(StatType::MAX_HP);
			float current_max_hp_variation_amount = GetCurrentPlayerStatValue(StatType::MAX_HP_VARIATION_AMOUNT);
			float current_max_hp_variation_rate = (float)GetCurrentPlayerStatValue(StatType::MAX_HP_VARIATION_RATE) / 10000.0f;
			float additional_max_hp = current_max_hp - (current_max_hp_variation_amount * current_max_hp_variation_rate) - currentStatData->value;
			return (current_max_hp_variation_amount * current_max_hp_variation_rate) + nextCon->value + additional_max_hp;
		}
	}

	return 0;
}

/*
현재 클라이언트에는 스탯을 계산하는 로직이 없다.
스탯 분배 후 증가한 MaxMp 값을 알기 위해서는
현재 MaxHp - Convert(Current Wis) - (MAX_MP_VARIATION_AMOUNT * MAX_MP_VARIATION_RATE / 10000) = 아이템, 제련 등으로 인한 MP 증가 값
아이템, 제련 등으로 인한 MP 증가 값 + Convert(Next WIS) + (MAX_MP_VARIATION_AMOUNT * MAX_MP_VARIATION_RATE / 10000) = 다음 MaxMp 증가 값
으로 계산해야한다.
*/
int32 FGsPlayerStatDistributionController::GetMaxMp(int32 inValue)
{	
	float currentWis = _originStat->GetStatInfo(StatType::WIS)->GetStatValue();
	const FGsItemStatData* currentStatData = GetConvertData(StatType::WIS, currentWis, StatType::MAX_MP);
	if (currentStatData)
	{
		const FGsItemStatData* nextWis = GetConvertData(StatType::WIS, inValue, StatType::MAX_MP);
		if (nextWis)
		{
			float current_max_mp = GetCurrentPlayerStatValue(StatType::MAX_MP);
			float current_max_mp_variation_amount = GetCurrentPlayerStatValue(StatType::MAX_MP_VARIATION_AMOUNT);
			float current_max_mp_variation_rate = (float)GetCurrentPlayerStatValue(StatType::MAX_MP_VARIATION_RATE) / 10000.0f;
			float additional_max_mp = current_max_mp - (current_max_mp_variation_amount * current_max_mp_variation_rate) - currentStatData->value;
			return (current_max_mp_variation_amount * current_max_mp_variation_rate) + nextWis->value + additional_max_mp;
		}
	}

	return 0;
}

int32 FGsPlayerStatDistributionController::GetCurrentPlayerStatValue(StatType inStatType)
{
	if (_playerStat)
	{
		if (const FGsStatInfo* statInfo = _playerStat->GetStatInfo(inStatType))
		{
			return statInfo->GetStatValue();
		}
	}

	return 0;
}

const FGsItemStatData* FGsPlayerStatDistributionController::GetConvertData(StatType inType, int32 inValue, StatType inSecondType)
{
	const FGsSchemaStatConvertTable* find = GetConvertData(inType, inValue);
	if (find)
	{
		const FGsItemStatData* findStat = find->statList.FindByPredicate([&](const FGsItemStatData& itemStat)->bool
		{
			return itemStat.type == inSecondType;
		});

		return findStat;
	}

	return nullptr;
}

void FGsPlayerStatDistributionController::SetStatData()
{
	UGsGameObjectBase* find = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == find)
	{
		return;
	}

	UGsGameObjectLocalPlayer* localPlayer = find->CastGameObject<UGsGameObjectLocalPlayer>();
	if (nullptr == localPlayer)
	{
		return;
	}

	//Get origin stat
	FGsGameObjectDataCreature* creatureData = localPlayer->GetGameObjectDataCreature();
	if (nullptr == creatureData)
	{
		return;
	}

	_playerStat = creatureData->GetStatBase();
}

void FGsPlayerStatDistributionController::InitOriginalStat()
{
	TArray<TPair<StatType, int32>> statMap = GetBonusStatTypeMap();

	//플레이어가 가지고 있는 현재 스탯에서 보너스 스탯과 관련된 스탯 정보를 가져온다
	if (_originStat.IsValid())
	{
		_originStat->UpdateStatInfo(statMap);
	}

	//플레이어가 분배한 값을 저장하기 위해 플레이어 기본 스탯으로 초기화
	if (_nextStat.IsValid())
	{
		_nextStat->UpdateStatInfo(statMap);
	}

	//무기, 페어리, 버프 등 저장된 현재 스탯 중 1차 스탯만 가져온다
	int32 num = MAIN_STAT_ARRAY.Num();
	StatType statType;
	for (int32 i = 0; i < num; ++i)
	{
		// crash 나서 nullptr 예외처리 진행
		// 승훈이가 신행가서 자리에 없다..
		if(nullptr == _originStat->GetStatInfo(MAIN_STAT_ARRAY[i]))
			continue;

		statType = MAIN_STAT_ARRAY[i];
		int32 statValue = _originStat->GetStatInfo(MAIN_STAT_ARRAY[i])->GetStatValue();
		_originalStatMap.Emplace(statType, statValue);
		_distributedStatMap[statType] = 0;
	}
}

void FGsPlayerStatDistributionController::InitOriginalDistribution()
{
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		TMap<StatType, int32> originalStatMap = userData->GetBonusStat();
		TMap<StatType, int32> temp;
		for (TPair<StatType, int32> pair : originalStatMap)
		{
			temp.Emplace(pair.Key, pair.Value);
		}

		_oriDistributeStatMap = temp;
	}
}

void FGsPlayerStatDistributionController::ClearStatDistribution()
{
	_originalStatMap.Empty();
	_oriDistributeStatMap.Empty();
	_distributedStatMap.Empty();
	_backUpDistributedStatMap.Empty();

	_remainBonusStatPoint = 0;
	_usedBonusStatPoint = 0;

	if (_nextStat.IsValid())
	{
		_nextStat.Reset();
	}

	if (_originStat.IsValid())
	{
		_originStat.Reset();
	}
}

void FGsPlayerStatDistributionController::InvalidateSecondaryStats()
{
	//origin - 이전 스탯 + 바뀐 스탯 
	TArray<TPair<StatType, int32>> statMap = _originStat->GetAllStatInfo();
	_nextStat->UpdateStatInfo(statMap);

	int32 num = MAIN_STAT_ARRAY.Num();
	int32 originValue = 0;
	int32 nextValue;
	StatType statType = StatType::MAX;

	for (int32 i = 0; i < num; ++i)
	{
		statType = MAIN_STAT_ARRAY[i];

		if (false == TryGetOriginStatValue(originValue, statType))
		{
			continue;
		}

		if (false == _distributedStatMap.Contains(statType))
		{
			continue;
		}

		nextValue = originValue + _distributedStatMap[statType];
		if (originValue != nextValue)
		{
			_nextStat->DecreaseStat(statType, originValue);
			_nextStat->IncreaseStat(statType, nextValue);
		}
	}
}

bool FGsPlayerStatDistributionController::TryGetOriginStatValue(OUT int32& outValue, StatType inStatType)
{
	return TryGetStatValue(outValue, _originStat, inStatType);
}

bool FGsPlayerStatDistributionController::TryGetNextStatValue(OUT int32& outValue, StatType inStatType)
{
	return TryGetStatValue(outValue, _nextStat, inStatType);
}

bool FGsPlayerStatDistributionController::TryGetStatValue(OUT int32& outValue, const TUniquePtr<FGsStatBase>& inStat, StatType inStatType)
{
	if (false == inStat.IsValid())
	{
		return false;
	}

	const FGsStatInfo* statInfo = inStat->GetStatInfo(inStatType);
	if (nullptr == statInfo)
	{
		return false;
	}
	
	outValue = statInfo->GetStatValue();
	return true;
}

/*
스탯 분배창에 1차 스탯(STR, DEX, INT, CON, AGI, WIS)에 관련된 현재 2차 스탯가져오는 함수
MAX_HP_VARIATION_AMOUNT, MAX_HP_VARIATION_AMOUNT는 MAX_HP, MAX_MP로 대체되어 예외처리
*/
TArray<TPair<StatType, int32>> FGsPlayerStatDistributionController::GetBonusStatTypeMap()
{
	UGsGameObjectBase* find = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == find)
	{
		return TArray<TPair<StatType, int32>>();
	}

	UGsGameObjectLocalPlayer* localPlayer = find->CastGameObject<UGsGameObjectLocalPlayer>();
	if (nullptr == localPlayer)
	{
		return TArray<TPair<StatType, int32>>();
	}

	//Get origin stat
	FGsGameObjectDataCreature* creatureData = localPlayer->GetGameObjectDataCreature();
	if (nullptr == creatureData)
	{
		return TArray<TPair<StatType, int32>>();
	}

	TSharedPtr<FGsStatBase> statBase = creatureData->GetStatBase();
	if (false == statBase.IsValid())
	{
		return TArray<TPair<StatType, int32>>();
	}

	TArray<TPair<StatType, int32>> statMap = statBase->GetAllStatInfo();
	int32 num = MAIN_STAT_ARRAY.Num();
	StatType statType;
	TArray<StatType> bonusStatTypeList;
	for (int32 i = 0; i < num; ++i)
	{
		statType = MAIN_STAT_ARRAY[i];
		bonusStatTypeList.Add(statType);

		const FGsSchemaStatConvertData* data = UGsTableUtil::FindRowById<UGsTableStatConvertData, FGsSchemaStatConvertData>(statType);
		if (nullptr == data)
		{
			continue;
		}

		if (data->convertList.Num() == 0)
		{
			continue;
		}

		const TArray<FGsItemStatData> itemList = data->convertList[0].statList;
		for (const FGsItemStatData& item : itemList)
		{
			bonusStatTypeList.Add(item.type);
		}
	}

	TArray<TPair<StatType, int32>> resultMap;
	for (const StatType& bonusStatType : bonusStatTypeList)
	{
		if (TPair<StatType, int32>* findStat = statMap.FindByPredicate([&bonusStatType](const TPair<StatType, int32>& elem)->bool
			{
				return elem.Key == bonusStatType;
			}))
		{
			resultMap.Add(TPair<StatType, int32>(bonusStatType, findStat->Value));
		}
	}

	return resultMap;
}

bool FGsPlayerStatDistributionController::IsMaxStat(StatType inStatType)
{
	if (false == _distributedStatMap.Contains(inStatType))
	{
		return false;
	}

	//서버에 저장된 스탯 + 현재 분배한 스탯 > 최대 분배 가능한 스탯
	return ((GetOriginalDistributePoint(inStatType) + _distributedStatMap[inStatType]) >= _maxStatValue);
}