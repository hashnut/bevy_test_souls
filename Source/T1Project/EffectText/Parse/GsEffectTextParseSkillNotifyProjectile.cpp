// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextParseSkillNotifyProjectile.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyProjectile.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileActionInfo.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "EffectText/GsEffectTextParser.h"
#include "UTIL/GsTableUtil.h"


bool FGsEffectTextParseSkillNotifyProjectile::IsValidData(const FGsSchemaEffectTextData* InData) const
{
	if (0 == InData->intParams.Num())
	{
		return false;
	}

	//if (2 != InData->floatParams.Num() ||
	//	2 != InData->intParams.Num())
	//{
	//	return false;
	//}

	return true;
}

bool FGsEffectTextParseSkillNotifyProjectile::GetString(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<FString>& OutData)
{
#ifdef USE_CUSTOM_TEXT
	if (false == InData->textParams[0].IsEmpty())
	{
		OutData.Emplace(InData->textParams[0].ToString());
		return true;
	}
#endif

	if (IsValidData(InData))
	{
		//FString strEffect;	
		TArray<FString> strArray;
		if (_parser->GetStringSkillNotify(InData->intParams[0], InLevel, InOption, strArray))
		{
			if (0 < strArray.Num())
			{
				FString strMerge;
				FGsEffectTextParser::GetMergedString(strArray, InOption, strMerge);

				//if (0 == InData->intParams[1])
				//{
				//	GetStringInter(InData, strEffect);
				//}

				//strEffect.Append(strMerge);

				//OutData.Emplace(strEffect);
				OutData.Emplace(strMerge);
				return true;
			}
		}
	}	

	return false;
}

bool FGsEffectTextParseSkillNotifyProjectile::GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel,
	int32 InOption, OUT TArray<TPair<FText, FText>>& OutData)
{
#ifdef USE_CUSTOM_TEXT
	if (false == InData->textParams[0].IsEmpty())
	{
		TPair<FText, FText> pairText;
		pairText.Key = InData->textParams[0];

		OutData.Emplace(pairText);
		return true;
	}
#endif

	if (IsValidData(InData))
	{
		//FString strEffect;		

		TArray<TPair<FText, FText>> pairArray;
		if (_parser->GetStringPairSkillNotify(InData->intParams[0], InLevel, InOption, pairArray))
		{
			if (0 < pairArray.Num())
			{
				FString strMerge;
				FGsEffectTextParser::GetMergedStringByPair(pairArray, InOption, strMerge);

				//if (0 == InData->intParams[1])
				//{
				//	GetStringInter(InData, strEffect);
				//}
				//
				//strEffect.Append(strMerge);

				//TPair<FText, FText> pairEffect;
				//pairEffect.Key = FText::FromString(strEffect);
				//pairEffect.Value = FText::GetEmpty();

				//OutData.Emplace(pairEffect);

				TPair<FText, FText> pairEffect;
				pairEffect.Key = FText::FromString(strMerge);
				pairEffect.Value = FText::GetEmpty();

				return true;
			}
		}
	}

	return false;
}

bool FGsEffectTextParseSkillNotifyProjectile::SetSkillNotifyElementData(const FGsSchemaSkillNotifyElement* InData,
	OUT TArray<FGsSchemaEffectTextData>& OutData)
{
	if (nullptr == InData)
	{
		return false;
	}

	const FGsSchemaSkillNotifyProjectile* projectileNotify = InData->typeInfo.GetRowByType<FGsSchemaSkillNotifyProjectile>();
	if (nullptr == projectileNotify)
	{
		return false;
	}

	const FGsSchemaProjectileSet* projectileSet =
		UGsTableUtil::FindRowById<UGsTableProjectileSet, FGsSchemaProjectileSet>(projectileNotify->projecttileId);
	if (nullptr == projectileSet)
	{
		return false;
	}

	if (0 == projectileSet->actionInfoList.Num())
	{
		return false;
	}

	//int32 actionInfoCount = 0;
	for (const FGsSchemaProjectileActionInfo& actionInfo : projectileSet->actionInfoList)
	{
		const FGsSchemaSkillNotifySet* skillNotifySet = actionInfo.skillNotifyId.GetRow();
		if (nullptr == skillNotifySet)
		{
			return false;
		}

		FGsSchemaEffectTextData data;
		data.parseType = GetParseType();
		data.textParams.Emplace(InData->effectText);
		//data.floatParams.Emplace(projectileSet->lifeTime);
		//data.floatParams.Emplace(actionInfo.tickInterval);
		data.intParams.Emplace(skillNotifySet->id);
		//data.intParams.Emplace(actionInfoCount);

		OutData.Emplace(data);

		//++actionInfoCount;
	}
	
	return true;
}
/*
void FGsEffectTextParseSkillNotifyProjectile::GetStringInter(const FGsSchemaEffectTextData* InData, OUT FString& OutString)
{
	if (0.f != InData->floatParams[0])
	{
		TEXT{ 0 }초간
			FText textFormat;
		if (FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_Projectile_lifeTime"), textFormat))
		{
			FText textLife = FText::Format(textFormat, InData->floatParams[0]);
			OutString.Append(textLife.ToString());
		}
	}

	if (0.f != InData->floatParams[1])
	{
		TEXT{ 0 }초마다
			FText textFormat;
		if (FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_Projectile_intervalTime"), textFormat))
		{
			if (false == OutString.IsEmpty())
			{
				OutString.Append(TEXT(" "));
			}

			FText textInterval = FText::Format(textFormat, InData->floatParams[1]);
			OutString.Append(textInterval.ToString());
			OutString.Append(TEXT(" "));
		}
	}
}
*/