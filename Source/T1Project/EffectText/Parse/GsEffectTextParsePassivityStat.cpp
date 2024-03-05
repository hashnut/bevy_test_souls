// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParsePassivityStat.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectStatSet.h"
#include "DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectStatParam.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "UTIL/GsTableUtil.h"


bool FGsEffectTextParsePassivityStat::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	return (3 <= InData->intParams.Num()) ? true : false;
}

bool FGsEffectTextParsePassivityStat::GetString(const FGsSchemaEffectTextData* InData, int InLevel, 
	int32 InOption, OUT TArray<FString>& OutData)
{
	if (IsValidData(InData))
	{
		TPair<StatType, int32> pairData;
		GetData(InData, pairData);

		FString strStat;
		if (ShowStatValue(pairData.Key))
		{
			FGsStatHelper::GetDesplayStatNameAndValue(pairData.Key, pairData.Value, strStat);
		}
		else
		{
			FText textStatName;
			if (FGsStatHelper::GetDisPlayStatName(pairData.Key, textStatName))
			{
				strStat = textStatName.ToString();
			}
		}

		if (false == strStat.IsEmpty())
		{
			OutData.Emplace(strStat);

			return true;
		}
	}

	return false;
}

bool FGsEffectTextParsePassivityStat::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	if (IsValidData(InData))
	{
		TPair<StatType, int32> pairData;
		GetData(InData, pairData);
		
		TPair<FText, FText> pairStat;
		if (ShowStatValue(pairData.Key))
		{
			FGsStatHelper::GetDesplayStatNameAndValue(pairData.Key, pairData.Value, pairStat);			
		}
		else
		{
			FGsStatHelper::GetDisPlayStatName(pairData.Key, pairStat.Key);
		}

		if (false == pairStat.Key.IsEmpty())
		{
			OutData.Emplace(pairStat);
			return true;
		}
	}

	return false;
}

bool FGsEffectTextParsePassivityStat::GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT FGsEffectTextCollection& OutData)
{
	// 스탯을 누적으로 쌓아야해서 Condition 정보 무시
	if (IsValidData(InData))
	{
		TPair<StatType, int32> pairData;
		GetData(InData, pairData);

		if (OutData._statMap.Contains(pairData.Key))
		{
			OutData._statMap[pairData.Key] += pairData.Value;
		}
		else
		{
			OutData._statMap.Emplace(pairData.Key, pairData.Value);
		}
		
		return true;
	}

	return false;
}

bool FGsEffectTextParsePassivityStat::SetData(const FGsSchemaPassivitySet* InData, int32 InIndex,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	if (0 == InData->effectInfoList.Num())
	{
		return false;
	}

	const FGsSchemaPassivityEffectInfo& effectInfo = InData->effectInfoList[InIndex];

	if (const FGsSchemaPassivityEffectStatSet* tableData =
		UGsTableUtil::FindRowById<UGsTablePassivityEffectStatSet,
		FGsSchemaPassivityEffectStatSet>(effectInfo.effectId))
	{
		for (const FGsSchemaPassivityEffectStatParam& iter : tableData->param)
		{
			FGsSchemaEffectTextData data;
			data.textParams.Emplace(InData->effectText);
			SetConditionData(effectInfo, data);

			SetDataInter(iter, data);

			OutData.Emplace(data);
		}
	}

	if (0 < OutData.Num())
	{
		return true;
	}

	return false;
}

void FGsEffectTextParsePassivityStat::SetDataInter(const FGsSchemaPassivityEffectStatParam& InData, 
	OUT FGsSchemaEffectTextData& OutData)
{
	OutData.parseType = GetParseType();
	OutData.intParams.Emplace(static_cast<int32>(InData.type.GetStatType()));
	OutData.intParams.Emplace(InData.value);
}

void FGsEffectTextParsePassivityStat::GetData(const FGsSchemaEffectTextData* InData, 
	OUT TPair<StatType, int32>& OutData)
{
	int32 index = IsCondition(InData) ? 5 : 1;

	OutData.Key = static_cast<StatType>(InData->intParams[index]);
	OutData.Value = InData->intParams[index + 1];
}

bool FGsEffectTextParsePassivityStat::ShowStatValue(StatType InType) const
{
	uint16 statNum = static_cast<uint16>(InType);

	if (static_cast<uint16>(StatType::IMMUNE_DAMAGE) > statNum ||
		static_cast<uint16>(StatType::IMMUNE_ABNORMALITY_OTHER) < statNum)
	{
		return true;
	}

	return false;
}