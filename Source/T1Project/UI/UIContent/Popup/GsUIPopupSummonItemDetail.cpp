// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIContent/Popup/GsUIPopupSummonItemDetail.h"

#include "DataSchema/Item/GsSchemaItemConsumeDetail.h"
#include "DataSchema/Item/GsSchemaItemEffectSummonFairy.h"
#include "DataSchema/Item/GsSchemaItemEffectSummonCostume.h"

#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Fairy/GsFairyData.h"
#include "Costume/GsCostumeData.h"

#include "UI/UIContent/Window/BattlePass/GsUIBattlePassLevelHelper.h"
#include "UI/UIContent/Popup/SummonDetailInfo/GsUISummonDetailGradeContents.h"
#include "Components/ScrollBox.h"

#include "EnumRange.h"
#include "Algo/Transform.h"
#include "Algo/Copy.h"
#include "Algo/ForEach.h"

void UGsUIPopupSummonItemDetail::BeginDestroy()
{
	if (nullptr != _scrollHelper)
	{
		_scrollHelper->ClearScrollPanel();
		_scrollHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupSummonItemDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollHelper = NewObject<UGsUIBattlePassLevelHelper>(this);
	_scrollHelper->InitScrollPanel(_contentsScroll);
}

void UGsUIPopupSummonItemDetail::NativeDestruct()
{
	if (_scrollHelper)
	{
		_scrollHelper->ClearAllScrollWidgets();
	}

	Super::NativeDestruct();
}

void UGsUIPopupSummonItemDetail::OnClickClose()
{
	Close();
}

void UGsUIPopupSummonItemDetail::InitializeData(PopupInitData* initParam /*= nullptr*/)
{
	if (initParam)
	{
		SetItemDetailId(initParam->detailId);
	}

	RefreshUI();
}

void UGsUIPopupSummonItemDetail::SetItemDetailId(int32 detailId)
{
	static auto SetProbability = [](TArray<FGsSummonChangeData>& summonableList, float groupProbability) {
		int64 sumProbability = 0;
		Algo::ForEach(summonableList, [&sumProbability](auto& e) { sumProbability += e.probability; });
		Algo::ForEach(summonableList, [&sumProbability, &groupProbability](auto& e) {
			e.perProbability = (e.probability * groupProbability * 100.f) / float(sumProbability); });
	};

	static auto SetGroupProbability = [](TArray<FGsSummonChangeGroup>& summonableList) {
		int64 sumProbability = 0;
		Algo::ForEach(summonableList, [&sumProbability](auto& e) { sumProbability += e.groupProbability; });
		Algo::ForEach(summonableList, [&sumProbability](auto& e) {
			e.groupPerProbability = (e.groupProbability * 100.f) / float(sumProbability);
			float groupProbability = e.groupProbability / float(sumProbability);
			SetProbability(e.dataList, groupProbability);
			});
	};

	TArray<FGsSummonChangeGroup> detailElements;
	if (const auto itemConsumeDetailTable = Cast<UGsTableItemConsumeDetail>(FGsSchemaItemConsumeDetail::GetStaticTable()))
	{
		const FGsSchemaItemConsumeDetail* consumDetailData = nullptr;
		if (itemConsumeDetailTable->FindRowById(detailId, consumDetailData))
		{
			auto effectId = consumDetailData->effectIDList.IsValidIndex(0) ? consumDetailData->effectIDList[0] : INVALID_ITEM_EFFECT_ID;
			if (consumDetailData->effectType == ItemEffectType::SUMMON_FAIRY)
			{
				MakeFairyData(effectId, detailElements);
			}
			else if (consumDetailData->effectType == ItemEffectType::SUMMON_COSTUME)
			{
				MakeCostumeData(effectId, detailElements);
			}
		}
	}

	_data.Reset();
	if (detailElements.Num() > 0)
	{
		SetGroupProbability(detailElements);

		for (auto& e : detailElements)
			_data.Append(e.dataList);
	}
}

ENUM_RANGE_BY_FIRST_AND_LAST(ItemGrade, ItemGrade::NORMAL, ItemGrade::LEGEND)
void UGsUIPopupSummonItemDetail::RefreshUI()
{
	TEnumRange<ItemGrade> grades;
	TMap<ItemGrade, TArray<FGsSummonChangeData>> itemByGrade;

	if (_data.Num() > 0)
	{
		for (auto grade : grades)
		{
			TArray<FGsSummonChangeData> dataList;
			Algo::CopyIf(_data, dataList, [grade](auto e) {
				return e.data.Pin()->GetGradeByItemBase() == grade; });

			if (dataList.Num() > 0)
			{
				auto& gradeList = itemByGrade.FindOrAdd(grade);
				for (auto& data : dataList)
				{
					auto id = data.data.IsValid() ? data.data.Pin()->GetId() : 0;
					if (auto preIter = gradeList.FindByPredicate([id](auto e) {
						return e.data.IsValid() ? e.data.Pin()->GetId() == id : false; }))
					{
						preIter->perProbability += data.perProbability;
					}
					else gradeList.Add(data);
				}
				//itemByGrade.FindOrAdd(grade).Append(dataList);
			}
		}

		itemByGrade.KeyStableSort([](auto le, auto re) { return le > re; });

		auto widgets = _scrollHelper->CashedAddScrollWidget(_entryContent, itemByGrade.Num(), true);

		int index = 0;
		for (auto e : itemByGrade)
		{
			e.Value.StableSort([](auto& le, auto& re) { return le.perProbability < re.perProbability; });
			if (auto content = Cast<UGsUISummonDetailGradeContents>(widgets[index++]))
			{
				content->SetData(e.Key, e.Value);
			}
		}
	}
}

void UGsUIPopupSummonItemDetail::OnInputCancel()
{
	Close();
}

int32 UGsUIPopupSummonItemDetail::MakeFairyData(int32 effectId, OUT TArray<FGsSummonChangeGroup>& detailElements)
{
	if (effectId == INVALID_ITEM_EFFECT_ID) return 0;
	if (const auto itemEffectTable = Cast<UGsTableItemEffectSummonFairy>(FGsSchemaItemEffectSummonFairy::GetStaticTable()))
	{
		const FGsSchemaItemEffectSummonFairy* itemEffectData = nullptr;
		if (itemEffectTable->FindRowById(effectId, itemEffectData))
		{
			if (const FGsSchemaFairySummon* summonFairyInfo = itemEffectData->fairySummonId.GetRow())
			{
				Algo::TransformIf(summonFairyInfo->probabilityList, detailElements, [](const FGsSchemaFairySummonItem& e) {
					return e.probabilityId.GetRow() != nullptr; }, [](const FGsSchemaFairySummonItem& e) {

						const FGsSchemaFairyProbability* row = e.probabilityId.GetRow();

						TArray<FGsSummonChangeData> fairyData;
						Algo::TransformIf(row->fairyList, fairyData, [](const FGsSchemaFairyProbabilityItem& d) {
								return d.fairyId.GetRow() != nullptr; }, [](const FGsSchemaFairyProbabilityItem& d) {
								TWeakPtr<FGsFairyData> fairy = GSFairy()->GetFairyData(d.fairyId.GetRow()->id);
								return FGsSummonChangeData{ fairy, d.probability, 0.f };
							});
					
					return  UGsUIPopupSummonItemDetail::FGsSummonChangeGroup{ fairyData, e.probability, 0.f };
				});
			}
		}
	}

	return detailElements.Num();
}

int32 UGsUIPopupSummonItemDetail::MakeCostumeData(int32 effectId, OUT TArray<FGsSummonChangeGroup>& detailElements)
{
	if (effectId == INVALID_ITEM_EFFECT_ID) return 0;
	if (const auto itemEffectTable = Cast<UGsTableItemEffectSummonCostume>(FGsSchemaItemEffectSummonCostume::GetStaticTable()))
	{
		const FGsSchemaItemEffectSummonCostume* itemEffectData = nullptr;
		if (itemEffectTable->FindRowById(effectId, itemEffectData))
		{
			if (const FGsSchemaCostumeSummon* summonCostumeInfo = itemEffectData->costumeSummonId.GetRow())
			{
				Algo::TransformIf(summonCostumeInfo->probabilityList, detailElements, [](const FGsSchemaCostumeSummonItem& e) {
					return e.probabilityId.GetRow() != nullptr; }, [](const FGsSchemaCostumeSummonItem& e) {

						const FGsSchemaCostumeProbability* row = e.probabilityId.GetRow();

						TArray<FGsSummonChangeData> costumeData;
						Algo::TransformIf(row->costumeList, costumeData, [](const FGsSchemaCostumeProbabilityItem& d) {
								return d.costumeId.GetRow() != nullptr; }, [](const FGsSchemaCostumeProbabilityItem& d) {
								TWeakPtr<FGsCostumeData> costume = GSCostume()->GetCostumeData(d.costumeId.GetRow()->id);
								return FGsSummonChangeData{ costume, d.probability, 0.f };
							});

					return  UGsUIPopupSummonItemDetail::FGsSummonChangeGroup{ costumeData, e.probability, 0.f };
				});
			}
		}
	}
	return detailElements.Num();
}
