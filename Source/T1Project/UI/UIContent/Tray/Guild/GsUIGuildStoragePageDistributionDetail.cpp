//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "GsUIGuildStoragePageDistributionDetail.h"
//#include "Guild/GsGuildStorageData.h"
//#include "Components/ListView.h"
//#include "Components/WidgetSwitcher.h"
//#include "Management/ScopeGlobal/GsGuildManager.h"
//#include "Item/GsItem.h"
//#include "UI/UIContent/Tray/Guild/GsGuildStorageDistributionDetailListItem.h"
//#include "UI/UIContent/Tray/Guild/GsUIGuildStorageEntryDistributionDetail.h"
//
//
//void UGsUIGuildStoragePageDistributionDetail::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	_dataList.Empty();
//	
//}
//
//void UGsUIGuildStoragePageDistributionDetail::NativeDestruct()
//{
//	_dataList.Empty();
//	_listView->ClearListItems();
//
//	if (true == _isOpened)
//	{
//		FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData();
//		storageData->UpdateDistributedCheckTime();
//	}
//
//	Super::NativeDestruct();
//}
//
//void UGsUIGuildStoragePageDistributionDetail::InvalidateAllInternal()
//{
//	// 분배 내역에 변화가 있다면 데이터 갱신
//	FGsGuildStorageData* myGuildStorageData = GGuild()->GetMyGuildStorageData();
//	if (true == myGuildStorageData->GetDirtyDistributed() || 0 == _dataList.Num())
//	{
//		_dataList.Empty();
//		uint8 count = 0;
//		TArray<FGsGuildStorageDistributionDetailData*>& distributedList = myGuildStorageData->_distributionDetailList;
//		for (uint8 i = 0; i < distributedList.Num() && count < myGuildStorageData->MaxDistributedItemHistoryCount; ++i)
//		{
//			if (nullptr == distributedList[i])
//				continue;
//
//			UGsGuildStorageDistributionDetailListItem* distributedDetail = NewObject<UGsGuildStorageDistributionDetailListItem>(this);
//			distributedDetail->SetData(distributedList[i]);
//			_dataList.Emplace(distributedDetail);
//			++count;
//		}
//	}
//	
//	myGuildStorageData->SetDirtyDistributed(false);
//
//	// 설정
//	_listView->SetListItems(_dataList);
//	_listView->ScrollToTop();
//
//	_switcherList->SetActiveWidgetIndex((_dataList.Num() == 0) ? 1 : 0);
//}
//
//void UGsUIGuildStoragePageDistributionDetail::Show()
//{
//	Super::Show();
//
//	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
//	InvalidateAll();
//}
//
//void UGsUIGuildStoragePageDistributionDetail::Hide()
//{
//	FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData();
//	storageData->UpdateDistributedCheckTime();
//
//	FGsItem* item = nullptr;
//	for (uint8 i = 0; i < storageData->_distributionDetailList.Num(); ++i)
//	{
//		item = storageData->_distributionDetailList[i]->_distributedItem;
//		item->SetIsNew(false);
//	}
//
//	const TArray<UUserWidget*>& entries = _listView->GetDisplayedEntryWidgets();
//	for (uint8 i = 0; i < entries.Num(); ++i)
//	{
//		UGsUIGuildStorageEntryDistributionDetail* entry = Cast<UGsUIGuildStorageEntryDistributionDetail>(entries[i]);
//		entry->InvalidateIcon();
//	}
//
//	Super::Hide();
//}