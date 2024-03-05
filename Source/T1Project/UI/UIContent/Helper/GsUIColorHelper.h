// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

/*
 * FGsUIColorHelper
 * UI의 Color 사용 편의용 static 함수 등록
 */
struct T1PROJECT_API FGsUIColorHelper
{
public:
	// 컬러 값을 테이블에서 얻어온다
	static const FLinearColor& GetColor(EGsUIColorType InType);

	// 아이템 등급에 따른 색상
	static const FLinearColor& GetColorItemGrade(ItemGrade InGrade);
	
	// 아이템 등급에 따른 색상
	static const FLinearColor& GetColorItemGradeByItemId(const ItemId InItemId);

	// 아이템 등급에 따른 배경 색상
	static const FLinearColor& GetColorItemGradeBg(ItemGrade InGrade);

	// 상태이상 이름 색상
	static const FLinearColor& GetColorNameTextAbnormality(bool bInIsBuff);
	
	// 패시브 이름 색상
	static const FLinearColor& GetColorNameTextPassivity(bool bInIsBuff, PassivityOwnerType InOwnerType);

	// 아이템 등급에 따른 색상
	static const FLinearColor& GetColorFairyGrade(FairyGrade InGrade);

	template<typename E>
	static const FLinearColor& GetColorGradeWidthItemGrade(E InGrade);

	template<typename E>
	static const FLinearColor& GetColorBGGradeWidthItemBGGrade(E InGrade);

	// ui bg 뽑기 페어리 등급색상 구하기
	static const FLinearColor& GetColorSummonFairyGradeBG(FairyGrade In_grade);
	// ui text 뽑기 페어리 등급색상 구하기
	static const FLinearColor& GetColorSummonFairyGradeText(FairyGrade In_grade);

	// ui bg 뽑기 코스튬 등급색상 구하기
	static const FLinearColor& GetColorSummonCostumeGradeBG(CostumeGrade In_grade);
	// ui text 뽑기 코스튬 등급색상 구하기
	static const FLinearColor& GetColorSummonCostumeGradeText(CostumeGrade In_grade);

	// 아이템 제련 옵션 수치별 색상 구하기
	static const FLinearColor& GetColorItemRefineOption(int32 InValue, int32 InMinValue, int32 InMaxValue);

	// 2022/06/02 PKT - 던전 난이도 별 컬러 구하기
	static const FLinearColor& GetDungeonDifficultyType(DungeonDifficulty InType);
};

template<typename E>
const FLinearColor& FGsUIColorHelper::GetColorGradeWidthItemGrade(E InGrade)
{
	//typename std::underlying_type<E>::type T nGrade = static_cast<typename std::underlying_type<E>::type>(InGrade);
	ItemGrade nGrade = static_cast<ItemGrade>(InGrade);

	EGsUIColorType colorType = EGsUIColorType::ITEM_GRADE_NORMAL;
	switch (nGrade)
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

template<typename E>
const FLinearColor& FGsUIColorHelper::GetColorBGGradeWidthItemBGGrade(E InGrade)
{
	//typename std::underlying_type<E>::type T nGrade = static_cast<typename std::underlying_type<E>::type>(InGrade);
	ItemGrade nGrade = static_cast<ItemGrade>(InGrade);

	EGsUIColorType colorType = EGsUIColorType::ITEM_GRADE_NORMAL;
	switch (nGrade)
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
