
#include "GsUIItemCollectionStatus.h"
#include "Collection/GsCollectionData.h"

#include "../Item/Collection/EItemCollectionEnumData.h"
#include "../Item/Collection/GsItemCollectionField.h"

#include "../Management/ScopeGame/GsItemCollectionManager.h"

#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

#include "../UI/UIContent/Helper/GsEffectStringHelper.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/ItemCollection/GsUIItemCollectionApplyStatus.h"




void UGsUIItemCollectionStatus::BeginDestroy()
{
	if (_slotHelperApplyStat)
	{
		_slotHelperApplyStat->OnRefreshEntry.RemoveDynamic(this, &UGsUIItemCollectionStatus::OnRefreshEntry);
	}
	Super::BeginDestroy();
}

void UGsUIItemCollectionStatus::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperApplyStat = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperApplyStat->Initialize(_subClassOfApplyStat, _panelApplyStatList);
	_slotHelperApplyStat->OnRefreshEntry.AddDynamic(this, &UGsUIItemCollectionStatus::OnRefreshEntry);
}

void UGsUIItemCollectionStatus::SetData()
{
	// 2023/1/9 PKT - 전체 / 완료
	TArray<const FGsItemCollectionField*> fieldSet;
	GSItemCollection()->FieldAll(fieldSet);

	int32 fieldTotalCount = 0;
	int32 fieldComplatedCount = 0;
	int32 fieldProgressCount = 0;

	int32 conditionTotalCount = 0;
	int32 conditionCompletedCount = 0;

	for (const auto& item : fieldSet)
	{
		if (false == item->IsEnableFlag())
		{
			continue;
		}

		// 2023/1/9 PKT - Field 전체
		++fieldTotalCount;

		bool isProgress = false;
		for (const auto& condition : item->Conditions())
		{
			// 2023/1/9 PKT - 조건 전체
			++conditionTotalCount;

			if (ECollectionMaterialActionType::Completed == condition.ActionType())
			{	// 2023/1/9 PKT - 조건 완료 갯수
				++conditionCompletedCount;
				// 2023/1/9 PKT - 한개라도 등록한 것이 있다면 진행 중..
				isProgress = true;
			}
		}

		if (ECollectionSortOrder::Completed == item->SortOrder())
		{
			++fieldComplatedCount;
		}
		else if (true == isProgress)
		{
			++fieldProgressCount;
		}		
	}


	auto ProgressText = [](UTextBlock* InTextCount, UTextBlock* InTextRatio, double InCount, double InMax)
	{
		float ratio = float(int32((InCount / InMax) * 10000)) / 100.f;
	
		InTextCount->SetText(FText::AsNumber(InCount));
		InTextRatio->SetText(FText::FromString(FString::Printf(TEXT("%0.2f%%"), ratio)));
	};

	ProgressText(_textFieldCompletedCount, _textFieldCompletedRatio, fieldComplatedCount, fieldTotalCount);
	ProgressText(_textFieldProgressingCount, _textFieldProgressingRatio, fieldProgressCount, fieldTotalCount);
	ProgressText(_textFieldIncompleteCount, _textFieldIncompleteRatio, fieldTotalCount - fieldComplatedCount, fieldTotalCount);

	ProgressText(_textConditionCompletedCount, _textConditionCompletedRatio, conditionCompletedCount, conditionTotalCount);
	ProgressText(_textConditionIncompleteCount, _textConditionIncompleteRatio, conditionTotalCount - conditionCompletedCount, conditionTotalCount);


	_textPassivitySet.Empty();

	const IGsCollectionData* passivityCollection = GSItemCollection()->PassivityCollectionData();
	if (nullptr != passivityCollection)
	{
		FGsEffectStringHelper::GetCollectionEffectStringList(passivityCollection, _textPassivitySet);
	}

	const static int32 SWITCHER_ENOUGHT_ITEM_INDEX = 0;
	const static int32 SWITCHER_EMPTY_ITEM_INDEX = 1;
	const int32 maxCount = _textPassivitySet.Num();
	if (maxCount <= 0)
	{
		_switcherApplyStatList->SetActiveWidgetIndex(SWITCHER_EMPTY_ITEM_INDEX);
	}
	else
	{
		_switcherApplyStatList->SetActiveWidgetIndex(SWITCHER_ENOUGHT_ITEM_INDEX);
		_slotHelperApplyStat->RefreshAll(maxCount);
	}
}

void UGsUIItemCollectionStatus::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIItemCollectionApplyStatus> item = Cast<UGsUIItemCollectionApplyStatus>(InEntry);
	if (false == item.IsValid())
	{
		GSLOG(Warning, TEXT("false == item.IsValid()"));
		return;
	}

	if (false == _textPassivitySet.IsValidIndex(InIndex))
	{
		GSLOG(Warning, TEXT("_textPassivitySet.IsValidIndex(InIndex) : [%d]"), InIndex);
		return;
	}

	const FText textEffect 
		= FText::FromString(FString::Format(TEXT("{0} {1}"), 
			{ *_textPassivitySet[InIndex].Key.ToString(), *_textPassivitySet[InIndex].Value.ToString() }));
	item->SetData(textEffect);
}