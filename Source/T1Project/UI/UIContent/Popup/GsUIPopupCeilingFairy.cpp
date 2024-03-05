// Fill out your copyright notice in the Description page of Project Settings.
#include "GsUIPopupCeilingFairy.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemIngredient.h"

#include "DataSchema/Fairy/GsSchemaFairyCeiling.h"
#include "Fairy/GsFairySummonConfirmData.h"

#include "UI/UIContent/Window/Fairy/GsUIFairyComposeRes.h"
#include "UI/UIContent/Popup/GsUIPopupSummonConfirmProbability.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "Components/RichTextBlock.h"

#include "Algo/MaxElement.h"
#include "Algo/ForEach.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Summon/GsSummonFunc.h"

void UGsUIPopupCeilingFairy::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TMap<FairyGrade, UGsSwitcherButton*> gradeTabs;
	{
		gradeTabs.Add(FairyGrade::LEGEND, _btnGradeLegend);
		gradeTabs.Add(FairyGrade::UNIQUE, _btnGradeUnique);
		gradeTabs.Add(FairyGrade::EPIC, _btnGradeEpic);
	}
	if (const UGsTable* table = FGsSchemaFairyCeiling::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaFairyCeiling>(_ceilingData);
	}

	int32 index = 0; // gradeTabs.Num() - 1
	for (auto iter : gradeTabs)
	{
		auto grade = iter.Key;
		auto gradeiter = _ceilingData.FindByPredicate([grade](const auto e) { return e->grade == grade; });

		iter.Value->SetVisibility(gradeiter != nullptr ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		if (gradeiter)
		{
			_tabGradeIndex.Add(index, grade);
			_toggleGroupGrade.AddToggleWidget(iter.Value);
			index++;
		}
	}

	_btnCeilingInfo->OnClicked.AddDynamic(this, &UGsUIPopupCeilingFairy::OnClickCellingProbability);
}

void UGsUIPopupCeilingFairy::OnClickCellingProbability()
{
	if (_selectData == nullptr || _runCeiling) return;

	static auto SetProbability = [](TArray<FGsFairySummonChangeData>& summonableList) {
		int64 sumProbability = 0;
		Algo::ForEach(summonableList, [&sumProbability](auto& e) { sumProbability += e._probability; });
		Algo::ForEach(summonableList, [&sumProbability](auto& e) { e._perProbability = (e._probability * 100.f) / float(sumProbability); });
	};

	TArray<FGsFairySummonChangeData> summonableList;
	if (const FGsSchemaFairyProbability* summonProbabilityData = _selectData->successProbabilityId.GetRow())
	{
		Algo::ForEachIf(summonProbabilityData->fairyList, [](auto& e) { return  e.fairyId.GetRow() != nullptr; },
			[&summonableList](auto& e) { summonableList.Add({ GSFairy()->GetFairyData(e.fairyId.GetRow()->id), e.probability }); });

		SetProbability(summonableList);
	}

	if (auto popup = Cast<UGsUIPopupSummonConfirmProbability>(GUI()->OpenAndGetWidget(TEXT("PopupFairySummonProbability"))))
	{
		_childwidgets.Add(popup);

		FText FairyUI_Ceiling_SuccessReward;
		FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Ceiling_SuccessReward"), FairyUI_Ceiling_SuccessReward);

		FText FairyUI_ProbDesc;
		FText::FindText(TEXT("FairyText"), TEXT("FairyUI_ProbDesc"), FairyUI_ProbDesc);

		popup->SetTitleDesc(FairyUI_Ceiling_SuccessReward, FairyUI_ProbDesc);
		popup->SetFairyData(summonableList);
	}
}

void UGsUIPopupCeilingFairy::OnSelectedTab(int32 index)
{
	bool refresh = _selectIndex != index;
	Super::OnSelectedTab(index);

	if (_tabGradeIndex.Find(index))
	{
		SetSelectedGrade(_tabGradeIndex[index], refresh);
	}
}

void UGsUIPopupCeilingFairy::OnStartSummonCeiling()
{
	if (_selectData == nullptr) return;

	if (FGsSummonFunc::CheckFairySummonWaitCount() == false)
	{
		return;
	}

	//https://jira.com2us.com/jira/browse/C2URWQ-5176
	// stack 갯수 최대 인지 체크
	if (FGsSummonFunc::CheckFairySummonStackCount() == false)
	{
		return;
	}

	if (CheckEnableCeiling())
	{
		for (auto widget : _childwidgets)
		{
			if (widget.IsValid()) widget->Close();
		}
		_childwidgets.Reset();

		OnCelingStart.ExecuteIfBound(static_cast<int32>(_selectData->grade), _selectLayer + 1);

		Super::OnStartSummonCeiling();
	}
}

void UGsUIPopupCeilingFairy::SetSelectedGrade(FairyGrade grade, bool refresh)
{
	_selectData = nullptr;
	_ceilingMaterial = nullptr;
	_maxCeilingAmount = 0;

	if (auto iter = _ceilingData.FindByPredicate([grade](const auto e) { return e->grade == grade; }))
	{
		_selectData = *iter;
		_usableOwnCount = 0;
		_ownItemCount = 0;
		_maxCeilingAmount = _selectData->maxCount;
		_ceilingMaterial = (*iter)->needItemId.GetRow();

		if (_ceilingMaterial)
		{
			auto itemList = GItem()->FindByTID(_ceilingMaterial->id, ItemType::MAX);
			for (auto item : itemList)
			{
				if (false == item.IsValid())
					continue;

				if (auto ingredientItem = item.Pin()->GetCastItem<FGsItemIngredient>())
				{
					if(ingredientItem->GetLock() == false)
						_usableOwnCount += ingredientItem->GetAmount();
					_ownItemCount += ingredientItem->GetAmount();
				}
			}

			if (const auto tableData = FGsItem::GetTableDataByID((*iter)->needItemId.GetRow()->id))
			{
				_textItemName = tableData->name;
				_colorItemName = FGsUIColorHelper::GetColorItemGrade(tableData->grade);
			}
		}

		static TMap<FairyGrade, TArray<FText>> descList;
		if (descList.Num() <= 0)
		{
			FText FairyUI_Ceiling_DetailedDesc1_Legend;
			FText FairyUI_Ceiling_DetailedDesc2_Legend;
			FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Ceiling_DetailedDesc1_Legend"), FairyUI_Ceiling_DetailedDesc1_Legend);
			FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Ceiling_DetailedDesc2_Legend"), FairyUI_Ceiling_DetailedDesc2_Legend);

			FText FairyUI_Ceiling_DetailedDesc1_Unique;
			FText FairyUI_Ceiling_DetailedDesc2_Unique;
			FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Ceiling_DetailedDesc1_Unique"), FairyUI_Ceiling_DetailedDesc1_Unique);
			FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Ceiling_DetailedDesc2_Unique"), FairyUI_Ceiling_DetailedDesc2_Unique);

			FText FairyUI_Ceiling_DetailedDesc1_Epic;
			FText FairyUI_Ceiling_DetailedDesc2_Epic;
			FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Ceiling_DetailedDesc1_Epic"), FairyUI_Ceiling_DetailedDesc1_Epic);
			FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Ceiling_DetailedDesc2_Epic"), FairyUI_Ceiling_DetailedDesc2_Epic);

			descList.Emplace(FairyGrade::EPIC, TArray<FText>{ FairyUI_Ceiling_DetailedDesc1_Epic, FairyUI_Ceiling_DetailedDesc2_Epic });
			descList.Emplace(FairyGrade::UNIQUE, TArray<FText>{ FairyUI_Ceiling_DetailedDesc1_Unique, FairyUI_Ceiling_DetailedDesc2_Unique });
			descList.Emplace(FairyGrade::LEGEND, TArray<FText>{ FairyUI_Ceiling_DetailedDesc1_Legend, FairyUI_Ceiling_DetailedDesc2_Legend });
		}

		_ceilingDesc1->SetText(FText::GetEmpty());
		_textCeilingDesc2 = FText::GetEmpty();
		if (auto descIter = descList.Find(grade))
		{
			_ceilingDesc1->SetText((*descIter)[0]);
			_textCeilingDesc2 = (*descIter)[1];
		}

		_iconSlotHelper->RefreshAll(1);
		_needCelingItemAmount.Reset();

		if (auto maxiter = Algo::MaxElement((*iter)->trialData, [](auto& le, auto& re) { return le.trialPhase < re.trialPhase; }))
		{
			_needCelingItemAmount.AddZeroed(maxiter->trialPhase);
		}

		_ItemCeilingResult->SetResult(grade, grade);

		for (auto ceiling : (*iter)->trialData)
		{
			_needCelingItemAmount[ceiling.trialPhase - 1] = ceiling.needItemCount;
			SetSelectLayer(refresh ? 0 : _selectLayer);
		}
	}
}

bool UGsUIPopupCeilingFairy::CheckEnableCeiling()
{
	if (_selectData == nullptr) return false;

	if (_ownItemCount < _needCelingItemAmount[_selectLayer])
	{
		FText msg;
		if (FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Ceiling_NotEnoughItem"), msg)) /*아이템이 부족합니다*/
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, msg);
		}
		return false;
	}
	
	if (_usableOwnCount < _ownItemCount && _usableOwnCount < _needCelingItemAmount[_selectLayer])
	{
		FText msg;
		if (FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Ceiling_NotEnoughItem"), msg)) /*사용 가능한 아이템이 부족합니다*/
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, msg);
		}
		return false;
	}

	return true;
}
