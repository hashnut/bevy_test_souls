#include "GsUIMaterialSetGainInfoCraftTargetList.h"

#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/Widget.h"

#include "GsUIMaterialSetGainInfoCraftTargetListEntry.h"
#include "Item/Craft/GsCraftTarget.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

void UGsUIMaterialSetGainInfoCraftTargetList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GSLOG(Log, TEXT("UGsUIMaterialSetGainInfoCraftTargetList::NativeOnInitialized()"));

	// 제작 대상 목록
	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIMaterialSetGainInfoCraftTargetList::OnRefreshEntry);
}

void UGsUIMaterialSetGainInfoCraftTargetList::NativeConstruct()
{
	GSLOG(Log, TEXT("UGsUIMaterialSetGainInfoCraftTargetList::NativeConstruct()"));

	Super::NativeConstruct();

	ResetAllDatas();
}

void UGsUIMaterialSetGainInfoCraftTargetList::NativeDestruct()
{
	GSLOG(Log, TEXT("UGsUIMaterialSetGainInfoCraftTargetList::NativeDestruct()"));

	ResetAllDatas();

	Super::NativeDestruct();
}

void UGsUIMaterialSetGainInfoCraftTargetList::ResetAllDatas()
{
	_slotHelper->AllClearChildren();
}

void UGsUIMaterialSetGainInfoCraftTargetList::SetParameters(TWeakPtr<Parameters> InParameters)
{
	if (!InParameters.IsValid())
	{
		GSLOG(Error, TEXT("!InParameters.IsValid()"));
		return;
	}
	_parameters = MoveTemp(InParameters);

	// 스크롤 리셋
	_scroll->EndInertialScrolling();
	_scroll->ScrollToStart();

	_slotHelper->RefreshAll(_parameters.Pin()->_itemCollectionConditions->Num());
}

void UGsUIMaterialSetGainInfoCraftTargetList::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (!_parameters.IsValid())
	{
		GSLOG(Error, TEXT("!_parameters.IsValid()"));
		return;
	}

	const TArray<FGsItemCollectionCondition>* ItemCollectionConditions = _parameters.Pin()->_itemCollectionConditions;
	if (nullptr == ItemCollectionConditions || !ItemCollectionConditions->IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("nullptr == ItemCollectionConditions || !ItemCollectionConditions->IsValidIndex(%d)"), InIndex);
		return;
	}

	UGsUIMaterialSetGainInfoCraftTargetListEntry* ListEntry = Cast<UGsUIMaterialSetGainInfoCraftTargetListEntry>(InEntry);
	if (nullptr == ListEntry)
	{
		GSLOG(Error, TEXT("nullptr == ListEntry"));
		return;
	}

	const FGsItemCollectionCondition& ItemCollectionCondition = (*ItemCollectionConditions)[InIndex];
	ListEntry->SetParameters(MakeShared<UGsUIMaterialSetGainInfoCraftTargetListEntry::Parameters>(ItemCollectionCondition));
}
