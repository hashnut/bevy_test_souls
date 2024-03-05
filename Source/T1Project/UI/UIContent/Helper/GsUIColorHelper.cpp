// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIColorHelper.h"

#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/GsSchemaUIColorData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"

#include "UI/UIContent/Define/EGsUIColorType.h"

#include "T1Project.h"


const FLinearColor& FGsUIColorHelper::GetColor(EGsUIColorType InType)
{
	const UGsTableUIColorData* table = Cast<UGsTableUIColorData>(FGsSchemaUIColorData::GetStaticTable());
	if (table)
	{
		const FGsSchemaUIColorData* row = nullptr;
		if (table->FindRowById(InType, row))
		{
			return row->color;
		}
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[ColorHelper] Fail to find color. colorType: %d"), static_cast<uint8>(InType));
#endif

	return FLinearColor::White;
}

const FLinearColor& FGsUIColorHelper::GetColorItemGrade(ItemGrade InGrade)
{
	EGsUIColorType colorType = EGsUIColorType::ITEM_GRADE_NORMAL;

	switch(InGrade)
	{
	case ItemGrade::NORMAL:
		colorType = EGsUIColorType::ITEM_GRADE_NORMAL;
		break;
	case ItemGrade::MAGIC:
		colorType = EGsUIColorType::ITEM_GRADE_MAGIC;
		break;
	case ItemGrade::RARE:
		colorType = EGsUIColorType::ITEM_GRADE_RARE;
		break;
	case ItemGrade::EPIC:
		colorType = EGsUIColorType::ITEM_GRADE_EPIC;
		break;
	case ItemGrade::UNIQUE:
		colorType = EGsUIColorType::ITEM_GRADE_UNIQUE;
		break;
	case ItemGrade::LEGEND:
		colorType = EGsUIColorType::ITEM_GRADE_LEGEND;
		break;
	case ItemGrade::ETERNAL:
		colorType = EGsUIColorType::ITEM_GRADE_ETERNAL;
		break;
	}

	return GetColor(colorType);
}

const FLinearColor& FGsUIColorHelper::GetColorItemGradeByItemId(const ItemId InItemId)
{
	const UGsTableItemCommon* ItemCommonTable = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr == ItemCommonTable)
	{
		GSLOG(Error, TEXT("nullptr == ItemCommonTable"));
		return FLinearColor::White;
	}

	const FGsSchemaItemCommon* Row = nullptr;
	if (!ItemCommonTable->FindRowById(InItemId, Row))
	{
		GSLOG(Error, TEXT("!ItemCommonTable->FindRowById(InItemId, Row), InItemId:%d"), InItemId);

#if WITH_EDITOR
		return FLinearColor::Red;
#else
		return FLinearColor::White;
#endif

	}

	return FGsUIColorHelper::GetColorItemGrade(Row->grade);
}

const FLinearColor& FGsUIColorHelper::GetColorItemGradeBg(ItemGrade InGrade)
{
	EGsUIColorType colorType = EGsUIColorType::ITEM_BG_GRADE_NORMAL;

	switch (InGrade)
	{
	case ItemGrade::NORMAL:
		colorType = EGsUIColorType::ITEM_BG_GRADE_NORMAL;
		break;
	case ItemGrade::MAGIC:
		colorType = EGsUIColorType::ITEM_BG_GRADE_MAGIC;
		break;
	case ItemGrade::RARE:
		colorType = EGsUIColorType::ITEM_BG_GRADE_RARE;
		break;
	case ItemGrade::EPIC:
		colorType = EGsUIColorType::ITEM_BG_GRADE_EPIC;
		break;
	case ItemGrade::UNIQUE:
		colorType = EGsUIColorType::ITEM_BG_GRADE_UNIQUE;
		break;
	case ItemGrade::LEGEND:
		colorType = EGsUIColorType::ITEM_BG_GRADE_LEGEND;
		break;
	case ItemGrade::ETERNAL:
		colorType = EGsUIColorType::ITEM_BG_GRADE_ETERNAL;
		break;
	}

	return GetColor(colorType);
}

const FLinearColor& FGsUIColorHelper::GetColorNameTextAbnormality(bool bInIsBuff)
{
	return bInIsBuff ? GetColor(EGsUIColorType::ABNORMALITY_POSITIVE) : GetColor(EGsUIColorType::ABNORMALITY_NEGATIVE);
}

const FLinearColor& FGsUIColorHelper::GetColorNameTextPassivity(bool bInIsBuff, PassivityOwnerType InOwnerType)
{
	if (false == bInIsBuff)
	{
		if (PassivityOwnerType::MAP == InOwnerType)
		{
			return GetColor(EGsUIColorType::PASSIVE_NEGATIVE_ENV);
		}

		return GetColor(EGsUIColorType::PASSIVE_NEGATIVE);
	}

	return GetColor(EGsUIColorType::PASSIVE_POSITIVE);
}

const FLinearColor& FGsUIColorHelper::GetColorFairyGrade(FairyGrade InGrade) 
{
	return GetColorGradeWidthItemGrade(InGrade);
}

// ui bg 뽑기 페어리 등급색상 구하기
const FLinearColor& FGsUIColorHelper::GetColorSummonFairyGradeBG(FairyGrade In_grade)
{
	ItemGrade iGrade = static_cast<ItemGrade>(In_grade);
	return GetColorItemGradeBg(iGrade);
}
// ui text 뽑기 페어리 등급색상 구하기
const FLinearColor& FGsUIColorHelper::GetColorSummonFairyGradeText(FairyGrade In_grade)
{
	ItemGrade iGrade = static_cast<ItemGrade>(In_grade);
	return GetColorItemGrade(iGrade);
}

// ui bg 뽑기 페어리 등급색상 구하기
const FLinearColor& FGsUIColorHelper::GetColorSummonCostumeGradeBG(CostumeGrade In_grade)
{
	ItemGrade iGrade = static_cast<ItemGrade>(In_grade);
	return GetColorItemGradeBg(iGrade);
}
// ui text 뽑기 페어리 등급색상 구하기
const FLinearColor& FGsUIColorHelper::GetColorSummonCostumeGradeText(CostumeGrade In_grade)
{
	ItemGrade iGrade = static_cast<ItemGrade>(In_grade);
	return GetColorItemGrade(iGrade);
}

const FLinearColor& FGsUIColorHelper::GetColorItemRefineOption(int32 InValue, int32 InMinValue, int32 InMaxValue)
{
	EGsUIColorType colorType = EGsUIColorType::DEFAULT_STAT_COLOR;
	float averageValue = (InMinValue + InMaxValue) / 2.f;
	if (InMaxValue <= InValue)
		colorType = EGsUIColorType::ITEM_REFINE_HIGH;
	else if (averageValue <= InValue)
		colorType = EGsUIColorType::ITEM_REFINE_MIDDLE;
	else
		colorType = EGsUIColorType::ITEM_REFINE_LOW;

	return GetColor(colorType);
}

const FLinearColor& FGsUIColorHelper::GetDungeonDifficultyType(DungeonDifficulty InType)
{
	EGsUIColorType colorType = EGsUIColorType::NONE;
	switch (InType)
	{
	case DungeonDifficulty::EASY:
		colorType = EGsUIColorType::DUNGEON_DIFFICULTY_TYPE_EASY;
		break;
	case DungeonDifficulty::NORMAL:
		colorType = EGsUIColorType::DUNGEON_DIFFICULTY_TYPE_NORMAL;
		break;
	case DungeonDifficulty::DIFFICULT:
		colorType = EGsUIColorType::DUNGEON_DIFFICULTY_TYPE_DIFFICULT;
		break;
	case DungeonDifficulty::NIGHTMARE:
		colorType = EGsUIColorType::DUNGEON_DIFFICULTY_TYPE_NIGHTMARE;
		break;
	}

	return GetColor(colorType);
}