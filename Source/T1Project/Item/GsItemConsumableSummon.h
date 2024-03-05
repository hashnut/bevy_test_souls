#pragma once

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsItemConsumable.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"

#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "DataSchema/Fairy/GsSchemaFairyCommon.h"

struct FGsSchemaItemEffectSummonCostume;
struct FGsSchemaItemEffectSummonFairy;
struct FGsSchemaCostumeProbability;
struct FGsSchemaFairyProbability;


/**
 * 코스튬 / 페어리 등급을 통일하기 위한 Enum
 */ 
enum class SummoneeGrade : uint8
{
	// 에러값
	NONE = 0,

	// 등급 - NORMAL
	NORMAL = 1,

	// 등급 - MAGIC
	MAGIC = 2,

	// 등급 - RARE
	RARE = 3,

	// 등급 - EPIC
	EPIC = 4,

	// 등급 - UNIQUE
	UNIQUE = 5,

	// 등급 - LEGEND
	LEGEND = 6,

	MAX = 7,
};


/**
 * 코스튬/페어리 정보를 담는 소환체 정보
 */
struct FGsSummonee
{
	int32 _id;						// Costume/Fairy Common 의 id 값
	FText _name;					// 코스튬/페어리 의 이름
	SummoneeGrade _grade;			// 코스튬/페어리 등급
	float _groupProbability;		// 코스튬/페어리 그룹별 등급을 그룹 갯수만큼 나눈 값 (묶음끼리 티어를 나누기 위함)
	int32 _probability;				// 해당 코스튬/페어리 등급 내 확률 (같은 등급 내에서 티어를 나누기 위함)

	explicit FGsSummonee(int32 InId, FText InName, SummoneeGrade InGrade, int64 InGroupProbability, int64 InProbability) :
		_id(InId),
		_name(InName),
		_grade(InGrade),
		_groupProbability(InGroupProbability),
		_probability(InProbability)
	{
	}

	explicit FGsSummonee(const FGsSchemaCostumeCommon* InCostumeCommon, int64 InGroupProbability, int64 InProbability) : FGsSummonee(InCostumeCommon->id, InCostumeCommon->name, FGsSummonee::GetSummoneeGrade(InCostumeCommon->grade), InGroupProbability, InProbability)
	{
	}

	explicit FGsSummonee(const FGsSchemaFairyCommon* InFairyCommon, int64 InGroupProbability, int64 InProbability) : FGsSummonee(InFairyCommon->id, InFairyCommon->name, FGsSummonee::GetSummoneeGrade(InFairyCommon->grade), InGroupProbability, InProbability)
	{
	}

	virtual ~FGsSummonee() = default;

	/** 코스튬 등급을 공용 소환체 등급으로 변환 */
	static SummoneeGrade GetSummoneeGrade(CostumeGrade InCostumeGrade);

	/** 페어리 등급을 공용 소환체 등급으로 변환 */
	static SummoneeGrade GetSummoneeGrade(FairyGrade InFairyGrade);
};

class T1PROJECT_API FGsItemConsumableSummon : public FGsItemConsumable
{
	using Super = FGsItemConsumable;

public:
	explicit FGsItemConsumableSummon(const int32 In_ItemDetailID);
	FGsItemConsumableSummon() = default;
	virtual ~FGsItemConsumableSummon() = default;

private:
	/** 각 등급에 맞는 메타 태그 선언 */
	static const FString GRADE_NORMAL;
	static const FString GRADE_MAGIC;
	static const FString GRADE_RARE;
	static const FString GRADE_EPIC;
	static const FString GRADE_UNIQUE;
	static const FString GRADE_LEGEND;

	/** 태그 닫기 */
	static const FString GRADE_TAG_CLOSING;

public:
	/** GsUIItemDetailInfo 에서 호출할, '내용물' 에 들어갈 FText */
	virtual FText GetUIContentsText() const override;

private:
	/** 코스튬 내용물 채우기 */
	FText SetCostumeContentsText(const FGsSchemaItemEffectSummonCostume* costumeData) const;
	
	/** 페어리 내용물 채우기 */
	FText SetFairyContentsText(const FGsSchemaItemEffectSummonFairy* fairyData) const;


	/** 코스튬 데이터로 소환체 리스트 만들기 */
	void MakeSummoneeList(const FGsSchemaCostumeProbability* costumeProbability, float InGroupProbability, OUT TArray<TSharedRef<FGsSummonee>>& outSummoneeList) const;

	/** 페어리 데이터로 소환체 리스트 만들기 */
	void MakeSummoneeList(const FGsSchemaFairyProbability* fairyProbability, float InGroupProbability, OUT TArray<TSharedRef<FGsSummonee>>& outSummoneeList) const;

	/** 소환체 정렬 (등급 > 티어 > Id) */
	void SortSummonee(OUT TArray<TSharedRef<FGsSummonee>>& outSummoneeList) const;

	/** 정렬된 리스트로 텍스트 파싱 */
	FText MakeContentsTextWithList(TArray<TSharedRef<FGsSummonee>>& InSummoneeList) const;

	/** 등급에 맞게 컬러를 바꿀 수 있도록 메타 정보 추가 (URichTextBlock 용) */
	static FString  MakeColoredStringWithGrade(const TSharedRef<FGsSummonee> InSummonee, const FString& InSeperator);
};