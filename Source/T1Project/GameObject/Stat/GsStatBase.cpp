// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStatBase.h"
#include "T1Project.h"

#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "Stat/GsSchemaStatConvertData.h"

#include "Item/Data/GsItemStatData.h"

#include "GsStatHelper.h"
#include "UTIL/GsText.h"

TSet<StatType> SecuStatTypes = {
	StatType::ATTACK_SPEED_RATE,
	StatType::CASTING_SPEED_RATE,
	StatType::MOVE_SPEED
};

void FGsStatBase::Initialize(UGsGameObjectBase* owner)
{
	_ownerObject = owner;
	/*
	if (_ownerObject.IsValid())
	{
	}
	*/

}

void FGsStatBase::Finalize()
{
	for (auto& iter : _statGroup)
	{
		delete iter.Value;
		iter.Value = nullptr;
	}

	_statGroup.Empty();
	_ownerObject.Reset();

	if (OnChangeStatDelegate.IsBound())
	{
		OnChangeStatDelegate.Clear();
	}
}

void FGsStatBase::Update()
{
}

// 서버에서 Stat Packet 내려줄때, MAX 수치에 대한 Packet 도 별도로 내려오므로
// Max Stat 에 대하여서는 Pair 가 되는 StatType 으로 별도 저장처리 함
void FGsStatBase::UpdateStatInfo(const TArray<TPair<StatType, int32>>& InStatInfoMap)
{
	TArray<StatType> statBroadcastList;
	for (const auto& iter : InStatInfoMap)
	{
		StatType statType = iter.Key;
		int32 statValue = iter.Value;

		//GSLOG(Error, TEXT("UpdateStatInfo Iterator StatType[%s] : Value[%d]"), *StatTypeEnumToString(statType), statValue);


		if (IsMaxStatType(statType))
		{
			StatType pairStatType = GetPairMaxStatType(statType);
			if (StatType::MAX == pairStatType)
				continue;

			if (INDEX_NONE == statBroadcastList.Find(pairStatType))
			{
				statBroadcastList.Emplace(pairStatType);
			}

			UpdateMaxStatInfo(pairStatType, statValue);
			UpdateStatInfo(statType, statValue);

			continue;
		}

		if (_statGroup.Contains(statType))
		{
			if (INDEX_NONE == statBroadcastList.Find(statType))
			{
				statBroadcastList.Emplace(statType);
			}

			SetStatInfo(statType, statValue);
			continue;
		}

		FGsStatInfo* newStatInfo = nullptr;
		if (SecuStatTypes.Contains(statType))
		{
			newStatInfo = new FGsStatInfoSecure();
		}
		else
		{
			newStatInfo = new FGsStatInfoCommon();
		}

		newStatInfo->SetStatType(statType);
		newStatInfo->SetStatValue(statValue);
		_statGroup.Add(statType, newStatInfo);

		if (INDEX_NONE == statBroadcastList.Find(statType))
		{
			statBroadcastList.Emplace(statType);
		}
	}

	// change stat Broadcast
	bool bIsCostType = false;
	for (uint16 i = 0; i < statBroadcastList.Num(); ++i)
	{
		const FGsStatInfo* statInfo = GetStatInfo(statBroadcastList[i]);
		if (nullptr == statInfo)
			continue;

		if (true == OnChangeStatDelegate.IsBound())
		{
			OnChangeStatDelegate.Broadcast(*statInfo);
		}

		if (true == IsAquireCostSkillType(statInfo->GetStatType()))
		{
			bIsCostType = true;
		}
	}

	if (bIsCostType)
	{
		GMessage()->GetUI().SendMessage(MessageUI::INVALIDATE_SLOT_AQUIRE_STAT, nullptr);
	}
}

bool FGsStatBase::IsAquireCostSkillType(const StatType InType)
{
	switch (InType)
	{
	case StatType::HP:
	case StatType::MP:
	case StatType::SENSITIVE:
	{
		return true;
	}
	}

	return false;
}

void FGsStatBase::UpdateStatInfo(StatType inStatType, int32 inValue)
{
	if (_statGroup.Contains(inStatType))
	{
		if (_statGroup[inStatType])
		{
			_statGroup[inStatType]->SetStatValue(inValue);
		}
	}
	else
	{
		FGsStatInfo* newStatInfo = nullptr;
		if (SecuStatTypes.Contains(inStatType))
		{
			newStatInfo = new FGsStatInfoSecure();
		}
		else
		{
			newStatInfo = new FGsStatInfoCommon();
		}

		newStatInfo->SetStatType(inStatType);
		newStatInfo->SetStatValue(inValue);
		_statGroup.Add(inStatType, newStatInfo);
	}
}

void FGsStatBase::SetMaxStatInfo(StatType InType, int32 InMaxValue)
{
	if (_statGroup.Contains(InType))
	{
		FGsStatInfo* statInfo = *_statGroup.Find(InType);
		statInfo->SetMaxStatValue(InMaxValue);
	}
	else
	{
		GSLOG(Log, TEXT("SetMaxStatInfo - InType Error : %d"), (uint16)InType);
	}
}

void FGsStatBase::SetStatInfo(StatType InStatType, int32 InValue)
{

	if (_statGroup.Contains(InStatType))
	{
		FGsStatInfo* statInfo = *_statGroup.Find(InStatType);
		statInfo->SetStatValue(InValue);
	}
	else
	{
		GSLOG(Log, TEXT("OnChangeStatData - InType Error : %d"), (uint16)InStatType);
	}
}

const FGsStatInfo* FGsStatBase::GetStatInfo(StatType InStatType) const
{
	if (_statGroup.Contains(InStatType))
	{
		FGsStatInfo* statInfo = *_statGroup.Find(InStatType);
		return statInfo;
	}

#if STAT_DEBUG
		FString statTypeName = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), InStatType);
		GSLOG(Error, TEXT("stat missing - name : %s"), *statTypeName);
#endif	

	return nullptr;
}

const TArray<TPair<StatType, int32>> FGsStatBase::GetAllStatInfo()
{
	TArray<TPair<StatType, int32>> statMap;
	for (const TPair<StatType, FGsStatInfo*>& pair : _statGroup)
	{
		statMap.Emplace(pair.Key, pair.Value->GetStatValue());
	}

	return statMap;
}

void FGsStatBase::UpdateMaxStatInfo(const StatType InType, const int32 InValue)
{
	if (_statGroup.Contains(InType))
	{
		SetMaxStatInfo(InType, InValue);
	}
	else
	{
		FGsStatInfo* newStatInfo = nullptr;
		if (SecuStatTypes.Contains(InType))
		{
			newStatInfo = new FGsStatInfoSecure();
		}
		else
		{
			newStatInfo = new FGsStatInfoCommon();
		}

		newStatInfo->SetStatType(InType);
		newStatInfo->SetMaxStatValue(InValue);
		_statGroup.Add(InType, newStatInfo);
	}
}

bool FGsStatBase::IsMaxStatType(StatType InStatType) const
{
	switch (InStatType)
	{
	case StatType::MAX_HP:
		return true;
	case StatType::MAX_MP:
		return true;
	case StatType::MAX_WEIGHT:
		return true;
	case  StatType::MAX_SENSITIVE:
		return true;
	}

	return false;
}

StatType FGsStatBase::GetPairMaxStatType(StatType InStatType)
{
	switch (InStatType)
	{
	case StatType::MAX_HP:
		return StatType::HP;
	case StatType::MAX_MP:
		return StatType::MP;
	case StatType::MAX_WEIGHT:
		return StatType::WEIGHT;
	case StatType::MAX_SENSITIVE:
		return StatType::SENSITIVE;
	}

	return StatType::MAX;
}

StatType FGsStatBase::GetMaxStatTypeFromStatType(const StatType inStatType)
{
	switch (inStatType)
	{
	case StatType::HP:
		return StatType::MAX_HP;
	case StatType::MP:
		return StatType::MAX_MP;
	case StatType::WEIGHT:
		return StatType::MAX_WEIGHT;
	case StatType::SENSITIVE:
		return StatType::MAX_SENSITIVE;
	}

	return StatType::MAX;
}

/*
// 서버에서 Stat Packet 내려줄때, MAX 수치에 대한 Packet 도 별도로 내려오므로
// Max Stat 에 대하여서는 Pair 가 되는 StatType 으로 별도 저장처리 함
void FGsStatBase::FinishedAddStat()
{
	// MaxStat Type 일때는 Pair가 되는 StatType 원소에 별도 저장한다
	if (0 < _statGroup.Num())
	{
		TArray<StatType> removeType;
		for (auto& stat : _statGroup)
		{
			StatType type = stat.Key;
			FGsStatInfo* info = stat.Value;
			if (true == IsMaxStatType(type) && nullptr != info)
			{
				StatType pairType = GetPairMaxStatType(type);
				if(StatType::MAX == pairType)
					continue;

				removeType.Add(type);
				SetMaxStatInfo(pairType, info->GetStatValue());
			}
		}

		// 저장 후, MaxStat Type 에 대해서 Remove 처리함
		for (int i = 0; i< removeType.Num(); ++i)
		{
			if (_statGroup.Contains(removeType[i]))
			{
				delete _statGroup[removeType[i]];
				_statGroup[removeType[i]] = nullptr;

				_statGroup.Remove(removeType[i]);
			}
		}
	}
}
*/

// 공격받은자가 이미 Hp가 0 이하라면 false 반환
void FGsStatBase::CalculateStat(uint32 InDmgValue, DamageStatType InDmgType)
{
	// die
	if (true == IsZeroHP())
	{
		return;
	}

	// 스탯 타입 얻기
	StatType statType = SharedFunc::DamageStatTypeToStatType(InDmgType);

	// hp 차감 처리
	const FGsStatInfo* statInfo = GetStatInfo(statType);
	if (nullptr == statInfo)
	{
		return;
	}

	uint32 statValue = (uint32)statInfo->GetStatValue();
	uint32 statCalcValue = InDmgValue > statValue ? 0 : statValue - InDmgValue;

	SetStatInfo(statType, statCalcValue);

	if (true == OnChangeStatDelegate.IsBound())
	{
		OnChangeStatDelegate.Broadcast(*statInfo);
	}
}

bool FGsStatBase::IsZeroHP() const
{
	const FGsStatInfo* statInfo = GetStatInfo(StatType::HP);
	if (nullptr != statInfo)
	{
		return 0 >= statInfo->GetStatValue();
	}
	return false;
}

//서버에서 스탯을 받는 형식이기 때문에 항상 DecreaseStat으로 원래 값을 빼 준 후 IncreaseStat으로 올려준다.
void FGsStatBase::IncreaseStat(const StatType inStat, const int32 inValue)
{
	const FGsStatInfo* find = GetStatInfo(inStat);
	if (nullptr == find)
	{
		return;
	}

	int32 newValue = find->GetStatValue() + inValue;
	SetStatInfo(inStat, newValue);

	int32 newSecondaryStatValue;
	FGsSchemaStatConvertTable convertData;
	if (TryGetConvertData(convertData, inStat))
	{
		for (FGsItemStatData& stat : convertData.statList)
		{
			const FGsStatInfo* findDetail = GetStatInfo(stat.type);

			if (findDetail)
			{
				newSecondaryStatValue = findDetail->GetStatValue() + stat.value;
				SetStatInfo(stat.type, newSecondaryStatValue);
			}
		}
	}
}

//서버에서 스탯을 받는 형식이기 때문에 항상 DecreaseStat으로 원래 값을 빼 준 후 IncreaseStat으로 올려준다.
void FGsStatBase::DecreaseStat(const StatType inStat, const int32 inValue)
{
	const FGsStatInfo* find = GetStatInfo(inStat);
	if (nullptr == find)
	{
		return;
	}

	int32 oldValue = find->GetStatValue();

	FGsSchemaStatConvertTable convertData;
	if (TryGetConvertData(convertData, inStat))
	{
		for (FGsItemStatData& stat : convertData.statList)
		{
			const FGsStatInfo* findDetail = GetStatInfo(stat.type);

			if (findDetail)
			{
				int32 newDetailValue = findDetail->GetStatValue() - stat.value;
				SetStatInfo(stat.type, newDetailValue);
			}
		}
	}

	int32 newValue = find->GetStatValue() - inValue;
	SetStatInfo(inStat, newValue);
}


bool FGsStatBase::TryGetConvertData(OUT FGsSchemaStatConvertTable& outData, const StatType inStatType)
{
	const FGsStatInfo* strStatInfo = GetStatInfo(inStatType);
	if (strStatInfo)
	{
		return FGsStatHelper::TryGetConvertData(outData, inStatType, strStatInfo->GetStatValue());
	}

	return false;
}

bool FGsStatBase::TryGetConvertData(OUT int32& outData, const StatType inStatType, const StatType inSecondStatType)
{
	FGsSchemaStatConvertTable statConvertData;
	if (TryGetConvertData(statConvertData, inStatType))
	{
		return true;
	}

	FGsItemStatData* find = statConvertData.statList.FindByPredicate([=](const FGsItemStatData& statData)->bool {
		return statData.type == inSecondStatType;
		});

	if (find)
	{
		outData = find->value;

		return true;
	}

	return false;
}

/*
void FGsStatBase::CalculateStat(UGsGameObjectProjectile* InAttackerProjectTileInfo)
{
	if (nullptr == InAttackerProjectTileInfo)
		return;

	FGsStatInfo* onHitHpinfo = GetStatInfo(ECharStatType::TypeHp);
	if (nullptr == onHitHpinfo || IsZeroHP())
	{
		GSLOG(Log, TEXT("CalculateStat - Already die Creature !!! "));
		return;
	}

	int64 hp = onHitHpinfo->GetCurStatValue();
	const int64 value = InAttackerProjectTileInfo->GetDmgValue();
	int64 calc = hp - value;

	SetCurStatInfo(ECharStatType::TypeHp, calc);
	OnAttackDelegate.Broadcast(*InAttackerProjectTileInfo , value ,EAttackType::DefaultAtk);
}
*/

/*
void FGsStatBase::GetSkillCoefficientValue(UGsGameObjectCreature* InAttackerCreatureInfo, float& outValue)
{
	if (nullptr == InAttackerCreatureInfo->GetSkill())
		return;

	if (nullptr == InAttackerCreatureInfo->GetSkill()->GetSkillRunner())
		return;

	FGsSkillRunnerBase* runnerBase = InAttackerCreatureInfo->GetSkill()->GetSkillRunner();;
	if (const FGsDC_SkillSet* skillData = runnerBase->GetSkillData())
	{
		outValue = skillData->SkillCoefficientValue == 0.f ? 1.f : skillData->SkillCoefficientValue;
	}
}


InAttackerCreatureInfo : 공격 한 Creature

void FGsStatBase::CalculateStat(UGsGameObjectCreature* InAttackerCreatureInfo , float InSkillCoefficientValue)
{
	// 나 자신(공격 받은 자) 의 hp가 0 과 같거나 이하라면 죽은것이므로 return 처리.
	// 몬스터 hp 0이 되면 바로 사라지지 않고 소멸되는 딜레이 시간동안 들어온다면 예외처리.
	FGsStatInfo* onHitHpinfo = GetStatInfo(ECharStatType::TypeHp);
	if (nullptr == onHitHpinfo || IsZeroHP())
	{
		GSLOG(Log, TEXT("CalculateStat - Already die Creature !!! "));
		return;
	}

	if (nullptr == InAttackerCreatureInfo)
		return;

	float coefficientValue = 0.f >= InSkillCoefficientValue ? 1.f : InSkillCoefficientValue;
	EGsGameObjectType eObjType = InAttackerCreatureInfo->GetObjectType();
	//GetSkillCoefficientValue(InAttackerCreatureInfo , coefficientValue);

	// Calc Battle
	FGsStatInfo* attackerDmgInfo = InAttackerCreatureInfo->GetCreatureStat()->GetStatInfo(ECharStatType::TypeAtkDmg);
	FGsStatInfo* criChanceInfo = InAttackerCreatureInfo->GetCreatureStat()->GetStatInfo(ECharStatType::TypeCriChance);

	// defaultAtk 데미지에 랜덤 수치적용 - 기획 CD님 요구사항
	int64 minDmg = attackerDmgInfo->GetCurStatValue(); // 최소
	int64 maxDmg = attackerDmgInfo->GetMaxStatValue();  // 최대
	int32 randDmg = FMath::RandRange(minDmg , maxDmg);

	int64 defaultAtkDmg = randDmg;
	float convertDmg = (float)defaultAtkDmg;
	float criDmg = 1;
	int32 randNum = FMath::RandRange(0, 100);
	if (criChanceInfo->GetCurStatValue() >= randNum) // Critical Attack
	{
		criDmg = 1.5; // 고정수치라고 함.
	}

	//GSLOG(Log , TEXT("coefficientValue : %f ") , coefficientValue)
	float atkDmg = convertDmg * criDmg;
	float realDmg = atkDmg * coefficientValue;
	int64 hp = onHitHpinfo->GetCurStatValue();
	int64 calc = hp - realDmg;

	SetCurStatInfo(ECharStatType::TypeHp, calc);
	OnAttackDelegate.Broadcast(*InAttackerCreatureInfo, realDmg, criDmg == 1 ? EAttackType::DefaultAtk : EAttackType::CriticalAtak);

	// Drop Exp
	if (0 >= onHitHpinfo->GetCurStatValue() && eObjType == EGsGameObjectType::LocalPlayer)
	{
		//DropExp(InAttackerCreatureInfo);
	}

	//GSLOG(Log, TEXT("CalculateStat - On Hit Creature HP : %d"), onHitHpinfo->GetCurStatValue());
}
*/

/*
void FGsStatBase::DropExp(UGsGameObjectCreature* InAttackerCreatureInfo)
{
	GsLocalPlayerStat* localInfo = static_cast<GsLocalPlayerStat*>(InAttackerCreatureInfo->GetCreatureStat());
	if (nullptr != localInfo)
	{
		FGsStatInfo* dropExp = GetStatInfo(ECharStatType::TypeExp);
		localInfo->AddExp(nullptr == dropExp ? 0 : dropExp->GetCurStatValue());
	}
}
*/

/*
void FGsStatBase::MessageSendProc(StatType InStatType , FGsStatInfo* InInfo)
{
	if (_ownerObject.IsValid())
	{
		EGsGameObjectType type = _ownerObject.Get()->GetObjectType();
		switch (type)
		{
		case EGsGameObjectType::LocalPlayer:
		case EGsGameObjectType::RemotePlayer:
			SendLocalPlayerProc(InStatType, InInfo);
			break;

		case EGsGameObjectType::NonPlayer:
			SendNonPlayerProc(InStatType, InInfo);
			break;
		}
	}
}

void FGsStatBase::SendLocalPlayerProc(StatType InStatType, FGsStatInfo* InInfo)
{
	if (nullptr == GMessage())
		return;

	if (nullptr == InInfo)
		return;

	switch (InStatType)
	{
	case StatType::HP:
		GMessage()->GetUserStatInfo().SendMessage(MessageUserInfo::UserStat::StatHp, *InInfo);
		break;
	case StatType::MP:
		GMessage()->GetUserStatInfo().SendMessage(MessageUserInfo::UserStat::StatCp, *InInfo);
		break;
	case StatType::ATTACK:
		GMessage()->GetUserStatInfo().SendMessage(MessageUserInfo::UserStat::StatAttack, *InInfo);
		break;
	case StatType::DEFENCE:
		GMessage()->GetUserStatInfo().SendMessage(MessageUserInfo::UserStat::StatDefence, *InInfo);
		break;
	case StatType::CRITICAL_RATE:
		GMessage()->GetUserStatInfo().SendMessage(MessageUserInfo::UserStat::StatCriticalRate, *InInfo);
		break;
	}
}

void FGsStatBase::SendNonPlayerProc(StatType InStatType, FGsStatInfo* InInfo)
{
	if (nullptr == GMessage())
		return;

	if (nullptr == InInfo)
		return;

	switch (InStatType)
	{
	case StatType::HP:
		break;
	}
}
*/