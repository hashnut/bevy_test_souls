// Fill out your copyright notice in the Description page of Project Settings.

#include "GsEffectStringHelper.h"
#include "Classes/Collection/GsCollectionData.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectStatSet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectInvokeAbnormalitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectInvokeSkillNotifySet.h"
#include "GameObject/Abnormality/GsPassivityHelper.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Management/ScopeGame/GsEffectTextManager.h"


bool FGsEffectStringHelper::GetCollectionEffectString(PassivityOwnerType InType, OUT FString& OutString)
{
	if (const IGsCollectionData* data = FGsPassivityHelper::GetCollectionData(InType))
	{
		return GetCollectionEffectString(data, OutString);
	}

	return false;
}

bool FGsEffectStringHelper::GetCollectionEffectStringList(PassivityOwnerType InType, OUT TArray<TPair<FText, FText>>& OutList)
{
	if (const IGsCollectionData* data = FGsPassivityHelper::GetCollectionData(InType))
	{
		return GetCollectionEffectStringList(data, OutList);
	}

	return false;
}

bool FGsEffectStringHelper::GetCollectionEffectString(const IGsCollectionData* InData, OUT FString& OutString)
{
	FGsEffectTextCollection addData;
	if (false == AddCollectionEffect(InData, addData))
	{
		return false;
	}

	SetEffectString(addData, OutString);

	return true;
}

bool FGsEffectStringHelper::GetCollectionEffectStringList(const IGsCollectionData* InData, OUT TArray<TPair<FText, FText>>& OutList)
{
	FGsEffectTextCollection addData;
	if (false == AddCollectionEffect(InData, addData))
	{
		return false;
	}

	SetEffectString(addData, OutList);

	return true;
}

bool FGsEffectStringHelper::GetPassivityEffectString(PassivityId InId, OUT FString& OutString, int32 InOption)
{
	return GSEffectText()->GetEffectTextPassivity(InId, OutString, InOption);
}

bool FGsEffectStringHelper::GetPassivityEffectString(const FGsSchemaPassivitySet* InData, 
	OUT FString& OutString, int32 InOption)
{
	return GSEffectText()->GetEffectTextPassivity(InData->id, OutString, InOption);
}

bool FGsEffectStringHelper::GetPassivityEffectString(const FGsSchemaPassivitySet* InData, 
	OUT TArray<TPair<FText, FText>>& OutList, int32 InOption)
{
	return GSEffectText()->GetEffectTextListPassivity(InData->id, OutList, InOption);
}

bool FGsEffectStringHelper::AddPassivityEffect(const FGsSchemaPassivitySet* InData, OUT FGsEffectAddData& OutAddData)
{
	if (PassivityType::STAT != InData->type)
	{
		return false;
	}

	const UGsTablePassivityEffectStatSet* statTable = Cast<const UGsTablePassivityEffectStatSet>(
		FGsSchemaPassivityEffectStatSet::GetStaticTable());
	if (nullptr == statTable)
	{
		return false;
	}

	if (0 < InData->effectInfoList.Num())
	{
		int effectId = InData->effectInfoList[0].effectId;
		const FGsSchemaPassivityEffectStatSet* statRow = nullptr;
		if (false == statTable->FindRowById(effectId, statRow))
		{
			return false;
		}

		for (const FGsSchemaPassivityEffectStatParam& itStat : statRow->param)
		{
			if (int32* valuePtr = OutAddData._statMap.Find(itStat.type))
			{
				*valuePtr += itStat.value;
			}
			else
			{
				OutAddData._statMap.Emplace(itStat.type, itStat.value);
			}
		}
	}

	return true;
}

bool FGsEffectStringHelper::AddCollectionEffect(const IGsCollectionData* InData, OUT FGsEffectTextCollection& OutAddData)
{
	if (FGsEffectTextManager* effectTextMgr = GSEffectText())
	{
		for (int i = 0, maxCount = InData->GetPassivityCount(); i < maxCount; ++i)
		{
			effectTextMgr->GetEffectTextColectionPassivity(InData->GetPassivityId(i), OutAddData);
		}
	}

	return true;
}

void FGsEffectStringHelper::SetEffectString(const FGsEffectTextCollection& InData, OUT FString& OutString)
{
	for (TPair<StatType, int32> iter : InData._statMap)
	{
		// 개행처리
		if (false == OutString.IsEmpty())
		{
			OutString.Append(TEXT("\n"));
		}

		FGsStatHelper::GetDesplayStatNameAndValue(iter.Key, iter.Value, OutString);
	}

	for (TPair<FText, FText> iter : InData._effectList)
	{
		// 개행처리
		if (false == OutString.IsEmpty())
		{
			OutString.Append(TEXT("\n"));
		}

		OutString.Append(iter.Key.ToString());
		if (false == iter.Value.IsEmpty())
		{
			OutString.Append(TEXT(" "));
			OutString.Append(iter.Value.ToString());
		}
	}
}

void FGsEffectStringHelper::SetEffectString(const FGsEffectTextCollection& InData, OUT TArray<TPair<FText, FText>>& OutList)
{
	for (TPair<StatType, int32> iter : InData._statMap)
	{
		TPair<FText, FText> pairData;
		if (FGsStatHelper::GetDesplayStatNameAndValue(iter.Key, iter.Value, pairData))
		{
			OutList.Emplace(pairData);
		}
	}

	OutList.Append(InData._effectList);
}
