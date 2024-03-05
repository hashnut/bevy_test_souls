// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParsePassivityInvokeAbnormality.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectInvokeAbnormalitySet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "EffectText/GsEffectTextParser.h"
#include "UTIL/GsTableUtil.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"


bool FGsEffectTextParsePassivityInvokeAbnormality::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	if (1 < InData->intParams.Num() &&
		1 == InData->floatParams.Num())
	{
		return true;
	}

	return false;
}

bool FGsEffectTextParsePassivityInvokeAbnormality::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{	
	if (false == IsValidData(InData))
	{
		return false;
	}

	FGsInvokeTypeData data(InData);

	TArray<FString> strArray;
	if (false == _parser->GetStringAbnormality(data.GetId(), InLevel, InOption, strArray))
	{
		return false;
	}

	if (0 == strArray.Num())
	{
		return false;
	}

	FString strEffect;
	FGsEffectTextParser::GetMergedString(strArray, InOption, strEffect);

	OutData.Emplace(strEffect);
	return true;
}

bool FGsEffectTextParsePassivityInvokeAbnormality::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
	if (false == IsValidData(InData))
	{
		return false;
	}

	FGsInvokeTypeData data(InData);

	TArray<TPair<FText, FText>> pairArray;
	if (false == _parser->GetStringPairAbnormality(data.GetId(), InLevel, InOption, pairArray))
	{
		return false;
	}

	if (0 == pairArray.Num())
	{
		return false;
	}

	FString strEffect;
	FGsEffectTextParser::GetMergedStringByPair(pairArray, InOption, strEffect);

	TPair<FText, FText> pairEffect;
	pairEffect.Key = FText::FromString(strEffect);
	pairEffect.Value = FText::GetEmpty();

	OutData.Emplace(pairEffect);
	return true;
}

bool FGsEffectTextParsePassivityInvokeAbnormality::GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT FGsEffectTextCollection& OutData)
{
	if (false == IsValidData(InData))
	{
		return false;
	}

	FGsInvokeTypeData data(InData);

	// 주의: 이 패시비티의 정보만 보기 위해 백업해서 비워서 수집, 처리후 복원한다.
	TArray<TPair<FText, FText>> pairBackup;
	if (0 < OutData._effectList.Num())
	{
		pairBackup.Append(OutData._effectList);
		OutData._effectList.Empty();
	}

	TArray<TPair<FText, FText>> pairArray;
	if (false == _parser->GetStringCollectionAbnormality(data.GetId(), InLevel, InOption, OutData) ||
		0 == OutData._effectList.Num())
	{
		// 복원 후 종료
		if (0 < pairBackup.Num())
		{
			OutData._effectList.Append(pairBackup);
		}

		return false;
	}

	FString strEffect;
	FGsEffectTextParser::GetMergedStringByPair(OutData._effectList, InOption, strEffect);

	TPair<FText, FText> pairEffect;
	pairEffect.Key = FText::FromString(strEffect);
	pairEffect.Value = FText::GetEmpty();

	OutData._effectList.Empty();
	if (0 < pairBackup.Num())
	{
		OutData._effectList.Append(pairBackup);
	}
	OutData._effectList.Emplace(pairEffect);
	return true;
}

void FGsEffectTextParsePassivityInvokeAbnormality::GetStringPassivityCauseAndRate(const FGsSchemaEffectTextData* InData, 
	int32 InOption, OUT FString& OutData)
{
	if (false == IsValidData(InData))
	{
		return;
	}

	FGsInvokeTypeData data(InData);	

	AddStringCause(data, InOption, OutData);
	AddStringRatePercent(data.GetRatePercent(), InOption, OutData);
}

void FGsEffectTextParsePassivityInvokeAbnormality::GetStringPassivityTarget(const FGsSchemaEffectTextData* InData,
	int32 InOption, OUT FString& OutData)
{
	if (false == IsValidData(InData))
	{
		return;
	}

	FGsInvokeTypeData data(InData);
	AddStringTarget(data.GetTarget(), InOption, OutData);
}

void FGsEffectTextParsePassivityInvokeAbnormality::GetStringPassivityCoolTime(const FGsSchemaEffectTextData* InData,
	int32 InOption, OUT FString& OutData)
{
	if (false == IsValidData(InData))
	{
		return;
	}

	FGsTimeStringHelper::GetTimeString(FTimespan::FromSeconds(InData->floatParams[0]), OutData);
}

bool FGsEffectTextParsePassivityInvokeAbnormality::SetData(const FGsSchemaPassivitySet* InData, int32 InIndex,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	if (0 == InData->effectInfoList.Num())
	{
		return false;
	}

	const FGsSchemaPassivityEffectInfo& effectInfo = InData->effectInfoList[InIndex];

	if (const FGsSchemaPassivityEffectInvokeAbnormalitySet* tableData =
		UGsTableUtil::FindRowById<UGsTablePassivityEffectInvokeAbnormalitySet,
		FGsSchemaPassivityEffectInvokeAbnormalitySet>(effectInfo.effectId))
	{
		if (const FGsSchemaAbnormalitySet* abnormality = tableData->param.abnormalityId.GetRow())
		{
			FGsSchemaEffectTextData data;
			data.parseType = GetParseType();
			data.textParams.Emplace(InData->effectText);
			SetConditionData(effectInfo, data);

			data.intParams.Emplace(abnormality->id);
			data.intParams.Emplace(static_cast<int32>(tableData->param.cause));
			data.intParams.Emplace(tableData->param.ratePct);
			data.intParams.Emplace(static_cast<int32>(tableData->param.target));

			SetAdditionalCauseParam(tableData->param.cause, tableData->param.causeInfo, data);

			data.floatParams.Emplace(tableData->param.cooldownTime);

			OutData.Emplace(data);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	if (0 < OutData.Num())
	{
		return true;
	}

	return true;
}
