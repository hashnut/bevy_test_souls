#include "GsUIPopupItemCollectionStatus.h"

#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"

#include "ProgressBar.h"
#include "TextBlock.h"
#include "Text.h"

#include "UI/UIContent/Window/ItemCollection/GsUIItemCollectionApplyStatus.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UMG/Public/Components/PanelWidget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#if WITH_EDITOR
#include "../../../../T1Project.h"
#endif


void UGsUIPopupItemCollectionStatus::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperApplyStat = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperApplyStat->Initialize(_subClassOfApplyStat, _panelApplyStatList);
	_slotHelperApplyStat->OnRefreshEntry.AddDynamic(this, &UGsUIPopupItemCollectionStatus::OnRefreshEntryApplyStat);

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupItemCollectionStatus::OnClickOk);
}

void UGsUIPopupItemCollectionStatus::NativeConstruct()
{
	Super::NativeConstruct();

	//const FGsItemCollectionManager* ItemCollectionManager = GSItemCollection();
	//if (nullptr == ItemCollectionManager)
	//{
	//	GSLOG(Error, TEXT("nullptr == ItemCollectionManager"));
	//	return;
	//}

	//PopupInitData InitData;
	//ItemCollectionManager->GetItemCollectionStatusInfos(InitData);

	//const float ItemCollectionTotalCount = InitData._itemCollectionCompletedCount
	//	+ InitData._itemCollectionProgressingCount
	//	+ InitData._itemCollectionIncompleteCount;

	//const float ItemCollectionItemSlotTotalCount = InitData._itemCollectionItemSlotCompletedCount
	//	+ InitData._itemCollectionItemSlotIncompleteCount;

	//const float ItemCollectionCompleteRatio = FMath::Clamp((InitData._itemCollectionCompletedCount / ItemCollectionTotalCount), 0.f, 1.f);
	//const float ItemCollectionProgressingRatio = FMath::Clamp((InitData._itemCollectionProgressingCount / ItemCollectionTotalCount), 0.f, 1.f);
	//const float ItemCollectionIncompleteRatio = FMath::Clamp((InitData._itemCollectionIncompleteCount / ItemCollectionTotalCount), 0.f, 1.f);

	//const float ItemCollectionItemSlotCompleteRatio = FMath::Clamp((InitData._itemCollectionItemSlotCompletedCount / ItemCollectionItemSlotTotalCount), 0.f, 1.f);
	//const float ItemCollectionItemSlotIncompleteRatio = FMath::Clamp((InitData._itemCollectionItemSlotIncompleteCount / ItemCollectionItemSlotTotalCount), 0.f, 1.f);

	//_progressCollectionCompleted->SetPercent(ItemCollectionCompleteRatio);
	//_progressCollectionProgressing->SetPercent(ItemCollectionProgressingRatio);

	//_textCollectionCompletedCount->SetText(FText::AsNumber(InitData._itemCollectionCompletedCount));
	//_textCollectionProgressingCount->SetText(FText::AsNumber(InitData._itemCollectionProgressingCount));
	//_textCollectionIncompleteCount->SetText(FText::AsNumber(InitData._itemCollectionIncompleteCount));

	//_textCollectionCompletedRatio->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"), ItemCollectionCompleteRatio * 100.f)));
	//_textCollectionProgressingRatio->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"), ItemCollectionProgressingRatio * 100.f)));
	//_textCollectionIncompleteRatio->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"), ItemCollectionIncompleteRatio * 100.f)));

	//_progressItemCompleted->SetPercent(ItemCollectionItemSlotCompleteRatio);

	//_textItemCompletedCount->SetText(FText::AsNumber(InitData._itemCollectionItemSlotCompletedCount));
	//_textItemIncompleteCount->SetText(FText::AsNumber(InitData._itemCollectionItemSlotIncompleteCount));

	//_textItemCompletedRatio->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"), ItemCollectionItemSlotCompleteRatio * 100.f)));
	//_textItemIncompleteRatio->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"), ItemCollectionItemSlotIncompleteRatio * 100.f)));

	//_applyEffectTextList.Empty();

	//const IGsCollectionData* collectionData = ItemCollectionManager->GetCollectionData();
	//if (nullptr != collectionData)
	//{
	//	FGsEffectStringHelper::GetCollectionEffectStringList(collectionData, _applyEffectTextList);
	//}

	//int32 maxCount = _applyEffectTextList.Num();
	//_slotHelperApplyStat->RefreshAll(maxCount);

	//const static int32 NORMAL_LIST_INDEX = 0;
	//const static int32 EMPTY_LIST_INDEX = 1;
	//_switcherApplyStatList->SetActiveWidgetIndex((0 >= maxCount) ? EMPTY_LIST_INDEX : NORMAL_LIST_INDEX);
}

void UGsUIPopupItemCollectionStatus::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIPopupItemCollectionStatus::BeginDestroy()
{
	if (_slotHelperApplyStat)
	{
		_slotHelperApplyStat->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupItemCollectionStatus::OnRefreshEntryApplyStat);
	}

	Super::BeginDestroy();
}

void UGsUIPopupItemCollectionStatus::OnClickOk()
{
	Close();
}

void UGsUIPopupItemCollectionStatus::OnRefreshEntryApplyStat(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIItemCollectionApplyStatus> widget = Cast<UGsUIItemCollectionApplyStatus>(InEntry);
	if (false == widget.IsValid())
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("UGsUIPopupItemCollectionStatus::OnRefreshEntryApplyStat : false == widget"));
#endif
		return;
	}

	if (false == _applyEffectTextList.IsValidIndex(InIndex))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("UGsUIPopupItemCollectionStatus::OnRefreshEntryApplyStat : invaild InIndex %d"), InIndex);
#endif
		return;
	}

	const TPair<FText, FText >& _EffectText = _applyEffectTextList[InIndex];

	const FText textEffect = FText::FromString(FString::Format(TEXT("{0} {1}"), { *_EffectText.Key.ToString(), *_EffectText.Value.ToString() }));
	widget->SetData(textEffect);
}