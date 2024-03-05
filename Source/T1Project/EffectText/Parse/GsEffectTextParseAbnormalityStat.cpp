// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseAbnormalityStat.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityStatSet.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityStat.h"
#include "DataSchema/Abnormality/AbnormalityConvert/GsSchemaAbnormalityIndexSet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextAbnormalityLevel.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsGameObjectUtil.h"

bool FGsEffectTextParseAbnormalityStat::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	if (2 == InData->intParams.Num() &&
		2 == InData->floatParams.Num())
	{
		return true;
	}

	return false;
}

bool FGsEffectTextParseAbnormalityStat::GetString(const FGsSchemaEffectTextData* InData, int InLevel, 
	int32 InOption, OUT TArray<FString>& OutData)
{
	if (IsValidData(InData))
	{
		StatType targetStat = static_cast<StatType>(InData->intParams[0]);
		
		FString strStat;
		// 값이 0이면 찍지 않음(IMMUNE일 경우 의도적으로 0값)
		if (0 == InData->intParams[1])
		{
			FText textStatName;
			if (FGsStatHelper::GetDisPlayStatName(targetStat, textStatName))
			{
				strStat = textStatName.ToString();
			}
		}
		else
		{
			FGsStatHelper::GetDesplayStatNameAndValue(targetStat, InData->intParams[1], strStat);
		}

		if (false == strStat.IsEmpty())
		{
			if (false == FGsEffectTextOptionFunc::IsOption(InOption, EGsEffectTextOption::SKIP_STAT_TIME))
			{
				// durationMax 값이 0일 경우 출력하지 않음
				if (0 < InData->floatParams[1])
				{
					FString strTime;
					GetTimeString(InData->floatParams[0], InData->floatParams[1], strTime);
					strStat.Append(TEXT(" "));
					strStat.Append(*strTime);
				}
			}

			OutData.Emplace(strStat);
			return true;
		}
	}

	return false;
}

bool FGsEffectTextParseAbnormalityStat::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	if (IsValidData(InData))
	{
		StatType targetStat = static_cast<StatType>(InData->intParams[0]);

		TPair<FText, FText> pairStat;

		// 값이 0이면 찍지 않음(IMMUNE일 경우 의도적으로 0값)
		if (0 == InData->intParams[1])
		{
			FGsStatHelper::GetDisPlayStatName(targetStat, pairStat.Key);
		}
		else
		{
			FGsStatHelper::GetDesplayStatNameAndValue(targetStat, InData->intParams[1], pairStat);
		}

		if (false == pairStat.Key.IsEmpty())
		{
			if (false == FGsEffectTextOptionFunc::IsOption(InOption, EGsEffectTextOption::SKIP_STAT_TIME))
			{
				if (0 < InData->floatParams[1])
				{
					FString strTime;
					GetTimeString(InData->floatParams[0], InData->floatParams[1], strTime);

					if (pairStat.Value.IsEmpty())
					{
						pairStat.Value = FText::FromString(strTime);
					}
					else
					{
						FString strStat = FString::Format(TEXT("{0} {1}"), { *pairStat.Value.ToString(), *strTime });
						// 시간값 추가한 것으로 다시 덮어씀
						pairStat.Value = FText::FromString(strStat);
					}
				}
			}

			OutData.Emplace(pairStat);
			return true;
		}
	}

	return false;
}

bool FGsEffectTextParseAbnormalityStat::GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel, 
	int32 InOption, OUT FGsEffectTextCollection& OutData)
{
	if (IsValidData(InData))
	{
		StatType targetStat = static_cast<StatType>(InData->intParams[0]);
		int32 targetValue = InData->intParams[1];

		// 컬렉션에서는 동스탯일 경우 합산을 한다
		if (OutData._statMap.Contains(targetStat))
		{
			OutData._statMap[targetStat] += targetValue;
		}
		else
		{
			OutData._statMap.Emplace(targetStat, targetValue);
		}

		return true;
	}

	return false;
}

bool FGsEffectTextParseAbnormalityStat::GetStringAbnormalityTime(const FGsSchemaEffectTextData* InData, int32 InOption,
	OUT FString& OutData)
{
	if (2 == InData->floatParams.Num())
	{
		GetTimeString(InData->floatParams[0], InData->floatParams[1], OutData);

		return true;
	}

	return false;
}

bool FGsEffectTextParseAbnormalityStat::SetData(const FGsSchemaAbnormalitySet* InData, const FGsSchemaAbnormalityIndexSet* InIndexSet,
	OUT TArray<FGsSchemaEffectTextAbnormalityLevel>& OutData)
{
	EGsAbnormalityEffectMainType mainType = UGsGameObjectUtil::GetAbnormalityMainType(InIndexSet->effectType);
	if (mainType != EGsAbnormalityEffectMainType::STAT)
	{
		return false;
	}

	for (const FGsSchemaAbnormalityIndexLevelInfo& itLevel : InIndexSet->levelList)
	{
		// 1레벨 부터 유효데이터이다
		if (0 == itLevel.level)
		{
			continue;
		}

		// 이미 레벨 정보가 있다면 해당 레벨에 정보 추가
		int32 outDataIndex = -1;
		for (int32 i = 0; i < OutData.Num(); ++i)
		{
			if (itLevel.level == OutData[i].level)
			{
				outDataIndex = i;
				break;
			}
		}

		if (const FGsSchemaAbnormalityStatSet* statTable = itLevel.effectRow.GetRowByType<FGsSchemaAbnormalityStatSet>())
		{
			// Stat정보는 이제 기본적으로 한개를 원칙으로 함
			const FGsSchemaAbnormalityStat& statInfo = statTable->param[0];

			FGsSchemaEffectTextData data;
			data.parseType = GetParseType();
			data.floatParams.Emplace(InData->durationMin);
			data.floatParams.Emplace(InData->durationMax);			
			data.intParams.Emplace(static_cast<int32>(statInfo.affectedStat.GetStatType()));

			if (AbnormalityEffectType::STAT_IMMUNE == statTable->type)
			{
				data.intParams.Emplace(0);
			}
			else
			{
				data.intParams.Emplace(statInfo.affectedValue);
			}

			if (0 > outDataIndex)
			{
				FGsSchemaEffectTextAbnormalityLevel dataLevel;
				dataLevel.level = itLevel.level;
				dataLevel.dataList.Emplace(data);

				OutData.Emplace(dataLevel);
			}
			else
			{
				OutData[outDataIndex].dataList.Emplace(data);
			}
		}
	}

	if (0 < OutData.Num())
	{
		return true;
	}

	return false;
}

void FGsEffectTextParseAbnormalityStat::GetTimeString(float InTimeMin, float InTimeMax, OUT FString& OutStr)
{
	if (InTimeMin == InTimeMax)
	{
		OutStr.Append(FGsTimeStringHelper::GetSecondString(InTimeMin));
	}
	else
	{
		OutStr.Append(FGsTimeStringHelper::GetSecondString(InTimeMin));
		OutStr.Append(TEXT("~"));
		OutStr.Append(FGsTimeStringHelper::GetSecondString(InTimeMax));
	}
}
