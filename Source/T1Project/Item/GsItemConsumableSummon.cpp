#include "GsItemConsumableSummon.h"
#include "T1Project.h"
#include "GsItemManager.h"
#include "SharedEnums/SharedItemEnum.h"
#include "Management/GsScopeHolder.h"

#include "DataSchema/Item/GsSchemaItemConsumeDetail.h"
#include "DataSchema/Item/GsSchemaItemEffectSummonCostume.h"
#include "DataSchema/Item/GsSchemaItemEffectSummonFairy.h"
#include "DataSchema/Fairy/GsSchemaFairyProbability.h"
#include "DataSchema/Fairy/GsSchemaFairyCommon.h"
#include "DataSchema/Costume/GsSchemaCostumeProbability.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"

#include "Algo/ForEach.h"
#include "Algo/Sort.h"

/** TextStyle_ItemGrade 를 통해 에서 사용할 메타 태그 정의 (URichTextBlock 용))*/
const FString FGsItemConsumableSummon::GRADE_NORMAL = TEXT("<Normal>");
const FString FGsItemConsumableSummon::GRADE_MAGIC = TEXT("<Magic>");
const FString FGsItemConsumableSummon::GRADE_RARE = TEXT("<Rare>");
const FString FGsItemConsumableSummon::GRADE_EPIC = TEXT("<Epic>");
const FString FGsItemConsumableSummon::GRADE_UNIQUE = TEXT("<Unique>");
const FString FGsItemConsumableSummon::GRADE_LEGEND = TEXT("<Legend>");
const FString FGsItemConsumableSummon::GRADE_TAG_CLOSING = TEXT("</>");


FGsItemConsumableSummon::FGsItemConsumableSummon(const int32 In_ItemDetailID)
	: FGsItemConsumable(In_ItemDetailID)
{
}

FText FGsItemConsumableSummon::GetUIContentsText() const
{
	int32 consumeDetailId = Super::GetDetailID();
	if (INVALID_ITEM_DETAIL_ID == consumeDetailId)
	{
		GSLOG(Error, TEXT("INVALID_ITEM_DETAIL_ID == consumeDetailId"));
		return FText::GetEmpty();
	}

	const UGsTableItemConsumeDetail* itemConsumeDetailTable = Cast<UGsTableItemConsumeDetail>(FGsSchemaItemConsumeDetail::GetStaticTable());
	if (nullptr == itemConsumeDetailTable)
	{
		GSLOG(Error, TEXT("nullptr == itemConsumeDetailTable"));
		return FText::GetEmpty();
	}
	const FGsSchemaItemConsumeDetail* itemConsumeDetailData = nullptr;
	if (false == itemConsumeDetailTable->FindRowById(consumeDetailId, itemConsumeDetailData))
	{
		GSLOG(Error, TEXT("false == itemConsumeDetailTable->FindRowById(consumeDetailId, itemConsumeDetailData)"));
		return FText::GetEmpty();
	}

	// 만약 effectIDList 가 비어 있으면 리턴
	if (itemConsumeDetailData->effectIDList.Num() == 0)
	{
		GSLOG(Error, TEXT("itemConsumeDetailData->effectIDList.Num() == 0, consumeDetailId : %d"), consumeDetailId);
		return FText::GetEmpty();
	}

	// 소환권 데이터 파싱에 사용할 effectId
	int32 effectId = itemConsumeDetailData->effectIDList.IsValidIndex(0) ? itemConsumeDetailData->effectIDList[0] : INVALID_ITEM_EFFECT_ID;

	if (INVALID_ITEM_EFFECT_ID == effectId)
	{
		GSLOG(Error, TEXT("INVALID_ITEM_EFFECT_ID == effectId"));
		return FText::GetEmpty();
	}

	// 코스튬/페어리 분기하여 텍스트 파싱
	if (itemConsumeDetailData->effectType == ItemEffectType::SUMMON_COSTUME)
	{
		const UGsTableItemEffectSummonCostume* itemEffectSummonCostumeTable = Cast<UGsTableItemEffectSummonCostume>(FGsSchemaItemEffectSummonCostume::GetStaticTable());
		if (nullptr == itemEffectSummonCostumeTable)
		{
			GSLOG(Error, TEXT("nullptr == itemEffectSummonCostumeTable"));
			return FText::GetEmpty();
		}

		const FGsSchemaItemEffectSummonCostume* costumeData = nullptr;
		if (false == itemEffectSummonCostumeTable->FindRowById(effectId, costumeData))
		{
			GSLOG(Error, TEXT("false == itemEffectSummonCostumeTable->FindRowById(effectId, costumeData)"));
			return FText::GetEmpty();
		}

		return SetCostumeContentsText(costumeData);
	}
	else if (itemConsumeDetailData->effectType == ItemEffectType::SUMMON_FAIRY)
	{
		const UGsTableItemEffectSummonFairy* itemEffectSummonFairyTable = Cast<UGsTableItemEffectSummonFairy>(FGsSchemaItemEffectSummonFairy::GetStaticTable());
		if (nullptr == itemEffectSummonFairyTable)
		{
			GSLOG(Error, TEXT("nullptr == itemEffectSummonFairyTable"));
			return FText::GetEmpty();
		}

		const FGsSchemaItemEffectSummonFairy* fairyData = nullptr;
		if (false == itemEffectSummonFairyTable->FindRowById(effectId, fairyData))
		{
			GSLOG(Error, TEXT("false == itemEffectSummonFairyTable->FindRowById(effectId, fairyData)"));
			return FText::GetEmpty();
		}

		return SetFairyContentsText(fairyData);
	}
	// effectType 이 잘못 들어가 있을 경우
	else
	{
		GSLOG(Error, TEXT("itemConsumeDetailData->effectType is not SUMMON_COSTUME or SUMMON_FAIRY"));
		return FText::GetEmpty();
	}
}

FText FGsItemConsumableSummon::SetCostumeContentsText(const FGsSchemaItemEffectSummonCostume* costumeData) const
{
	const FGsSchemaCostumeSummon* costumeSummon = costumeData->costumeSummonId.GetRow();
	if (nullptr == costumeSummon)
	{
		GSLOG(Error, TEXT("nullptr == costumeSummon"));
		return FText::GetEmpty();
	}

	TArray<TSharedRef<FGsSummonee>> summoneeList;

	// 소환 가능 목록 만들기
	for (const FGsSchemaCostumeSummonItem& costumeProb : costumeSummon->probabilityList)
	{
		float groupProbability = static_cast<float>(costumeProb.probability);
		MakeSummoneeList(costumeProb.probabilityId.GetRow(), groupProbability, summoneeList);
	}

	// 소환 가능 목록 정렬
	SortSummonee(summoneeList);

	// 정렬된 목록으로 텍스트 파싱
	return MakeContentsTextWithList(summoneeList);
}

FText FGsItemConsumableSummon::SetFairyContentsText(const FGsSchemaItemEffectSummonFairy* fairyData) const
{
	const FGsSchemaFairySummon* fairySummon = fairyData->fairySummonId.GetRow();
	if (nullptr == fairySummon)
	{
		GSLOG(Error, TEXT("nullptr == fairySummon"));
		return FText::GetEmpty();
	}

	TArray<TSharedRef<FGsSummonee>> summoneeList;

	// 소환 가능 목록 만들기
	for (const FGsSchemaFairySummonItem& fairyProb : fairySummon->probabilityList)
	{
		float groupProbability = static_cast<float>(fairyProb.probability);
		MakeSummoneeList(fairyProb.probabilityId.GetRow(), groupProbability, summoneeList);
	}

	// 소환 가능 목록 정렬
	SortSummonee(summoneeList);

	// 정렬된 목록으로 텍스트 파싱
	return MakeContentsTextWithList(summoneeList);
}

void FGsItemConsumableSummon::MakeSummoneeList(const FGsSchemaCostumeProbability* costumeProbability, float InGroupProbability, OUT TArray<TSharedRef<FGsSummonee>>& outSummoneeList) const
{
	for (const FGsSchemaCostumeProbabilityItem& costumeCommon : costumeProbability->costumeList)
	{
		int64 prob = costumeCommon.probability;

		// C2URWQ-6811 | 기획 규칙 변경으로, 각 티어별 그룹 내 확률은 동일하고, 티어별로만 구분을 한다고 한다.
		// 다만 고티어라고 해서 확률이 적은 것이 아니다. 왜냐하면 저티어는 숫자가 많고, 고티어는 숫자가 많기 때문이다.
		outSummoneeList.Emplace(MakeShared<FGsSummonee>(costumeCommon.costumeId.GetRow(), InGroupProbability / costumeProbability->costumeList.Num(), prob));
	}
}

void FGsItemConsumableSummon::MakeSummoneeList(const FGsSchemaFairyProbability* fairyProbability, float InGroupProbability, OUT TArray<TSharedRef<FGsSummonee>>& outSummoneeList) const
{
	for (const FGsSchemaFairyProbabilityItem& fairyCommon : fairyProbability->fairyList)
	{
		int64 prob = fairyCommon.probability;

		// C2URWQ-6811 | 기획 규칙 변경으로, 각 티어별 그룹 내 확률은 동일하고, 티어별로만 구분을 한다고 한다.
		// 다만 고티어라고 해서 확률이 적은 것이 아니다. 왜냐하면 저티어는 숫자가 많고, 고티어는 숫자가 많기 때문이다.
		outSummoneeList.Emplace(MakeShared<FGsSummonee>(fairyCommon.fairyId.GetRow(), InGroupProbability / fairyProbability->fairyList.Num(), prob));
	}
}

void FGsItemConsumableSummon::SortSummonee(OUT TArray<TSharedRef<FGsSummonee>>& outSummoneeList) const
{
	Algo::Sort(outSummoneeList, [](const TSharedRef<FGsSummonee> A, const TSharedRef<FGsSummonee> B)
		{
			// 등급 내림차순
			if (A->_grade != B->_grade)
			{
				return (uint8)A->_grade > (uint8)B->_grade;
			}

			// 그룹 확률 오름차순 (높은 티어부터 나옴)
			if (A->_groupProbability != B->_groupProbability)
			{
				return A->_groupProbability < B->_groupProbability;
			}

			// 등급 내 확률 오름차순 (높은 티어부터 나옴)
			if (A->_probability != B->_probability)
			{
				return A->_probability < B->_probability;
			}
			
			// UniqueId 오름차순
			return A->_id < B->_id;
		});
}

FText FGsItemConsumableSummon::MakeContentsTextWithList(TArray<TSharedRef<FGsSummonee>>& InSummoneeList) const
{
	FString contentsString;

	// 구분자 정의
	// ' - '
	FText seperator;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_RewardBoxDesc_Separator"), seperator);

	FString seperatorStr = seperator.ToString();

	// 각 목록별로 색상 메타 태그를 입혀 저장
	Algo::ForEach(InSummoneeList, [&](auto& elem)
		{
			contentsString.Append(FGsItemConsumableSummon::MakeColoredStringWithGrade(elem, seperatorStr));
			contentsString.Append("\n");
		});

	// 맨 마지막에 붙은 newLine 제거
	contentsString.RemoveFromEnd("\n");

	return FText::FromString(contentsString);
}

FString FGsItemConsumableSummon::MakeColoredStringWithGrade(const TSharedRef<FGsSummonee> InSummonee, const FString& InSeperator)
{
	// 메타 태그 입힌 스트링
	FString coloredString;

	switch (InSummonee->_grade)
	{
	case SummoneeGrade::NORMAL:
		coloredString.Append(FGsItemConsumableSummon::GRADE_NORMAL);
		break;
	case SummoneeGrade::MAGIC:
		coloredString.Append(FGsItemConsumableSummon::GRADE_MAGIC);
		break;
	case SummoneeGrade::RARE:
		coloredString.Append(FGsItemConsumableSummon::GRADE_RARE);
		break;
	case SummoneeGrade::EPIC:
		coloredString.Append(FGsItemConsumableSummon::GRADE_EPIC);
		break;
	case SummoneeGrade::UNIQUE:
		coloredString.Append(FGsItemConsumableSummon::GRADE_UNIQUE);
		break;
	case SummoneeGrade::LEGEND:
		coloredString.Append(FGsItemConsumableSummon::GRADE_LEGEND);
		break;
	default:
		GSLOG(Error, TEXT("SummoneeGrade is NONE or MAX"));
		return FString();
		break;
	}

	// 구분자 붙이기. ex) ' - '
	coloredString.Append(InSeperator);

	coloredString.Append(InSummonee->_name.ToString());
	coloredString.Append(FGsItemConsumableSummon::GRADE_TAG_CLOSING);

	return MoveTemp(coloredString);
}

SummoneeGrade FGsSummonee::GetSummoneeGrade(CostumeGrade InCostumeGrade)
{
	switch (InCostumeGrade)
	{
	case CostumeGrade::NONE:
		return SummoneeGrade::NONE;
		break;
	case CostumeGrade::NORMAL:
		return SummoneeGrade::NORMAL;
		break;
	case CostumeGrade::MAGIC:
		return SummoneeGrade::MAGIC;
		break;
	case CostumeGrade::RARE:
		return SummoneeGrade::RARE;
		break;
	case CostumeGrade::EPIC:
		return SummoneeGrade::EPIC;
		break;
	case CostumeGrade::UNIQUE:
		return SummoneeGrade::UNIQUE;
		break;
	case CostumeGrade::LEGEND:
		return SummoneeGrade::LEGEND;
		break;
	case CostumeGrade::MAX:
		return SummoneeGrade::MAX;
		break;
	default:
		return SummoneeGrade::NONE;
		break;
	}
}

SummoneeGrade FGsSummonee::GetSummoneeGrade(FairyGrade InFairyGrade)
{
	switch (InFairyGrade)
	{
	case FairyGrade::NONE:
		return SummoneeGrade::NONE;
		break;
	case FairyGrade::NORMAL:
		return SummoneeGrade::NORMAL;
		break;
	case FairyGrade::MAGIC:
		return SummoneeGrade::MAGIC;
		break;
	case FairyGrade::RARE:
		return SummoneeGrade::RARE;
		break;
	case FairyGrade::EPIC:
		return SummoneeGrade::EPIC;
		break;
	case FairyGrade::UNIQUE:
		return SummoneeGrade::UNIQUE;
		break;
	case FairyGrade::LEGEND:
		return SummoneeGrade::LEGEND;
		break;
	case FairyGrade::MAX:
		return SummoneeGrade::MAX;
		break;
	default:
		return SummoneeGrade::NONE;
		break;
	}
}