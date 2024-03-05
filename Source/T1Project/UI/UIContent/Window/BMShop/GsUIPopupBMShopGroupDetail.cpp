// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIPopupBMShopGroupDetail.h"
#include "GsUIBMShopProvideGroup.h"
#include "T1Project.h"
#include "Components/VerticalBox.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"



void UGsUIPopupBMShopGroupDetail::BeginDestroy()
{
	if (nullptr != _itemListPanelSlotHelper)
	{
		_itemListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupBMShopGroupDetail::OnRefreshItem);
		_itemListPanelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupBMShopGroupDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUIPopupBMShopGroupDetail::OnConfirm);

	if (nullptr == _itemListPanelSlotHelper)
	{
		_itemListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_itemListPanelSlotHelper->Initialize(_itemListEntryWidgetClass, _itemListRootPanel);
		_itemListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupBMShopGroupDetail::OnRefreshItem);
	}
}

void UGsUIPopupBMShopGroupDetail::NativeConstruct()
{
	Super::NativeConstruct();

	_rewardBoxDataList.Empty();
}

void UGsUIPopupBMShopGroupDetail::NativeDestruct()
{
	_rewardBoxDataList.Empty();

	_itemListPanelSlotHelper->RefreshAll(0);

	Super::NativeDestruct();
}

void UGsUIPopupBMShopGroupDetail::SetProvideItem(TArray<FGsRewardUnboxingData> InDataList)
{
	_rewardBoxDataList = InDataList;
	_itemListPanelSlotHelper->RefreshAll(_rewardBoxDataList.Num());
}

void UGsUIPopupBMShopGroupDetail::OnRefreshItem(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopProvideGroup* entry = Cast<UGsUIBMShopProvideGroup>(InEntry);
	if (nullptr == entry)
		return;

	if (_rewardBoxDataList.IsValidIndex(InIndex))
	{
		entry->SetProvideItem(_rewardBoxDataList[InIndex],false, true);
	}
}

void UGsUIPopupBMShopGroupDetail::OnConfirm()
{
	Close();
}

void UGsUIPopupBMShopGroupDetail::OnInputOk()
{
	OnConfirm();
}

void UGsUIPopupBMShopGroupDetail::OnInputCancel()
{
	OnConfirm();
}
