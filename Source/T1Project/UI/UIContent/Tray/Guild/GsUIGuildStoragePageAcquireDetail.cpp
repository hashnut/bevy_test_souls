//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "GsUIGuildStoragePageAcquireDetail.h"
//#include "Guild/GsGuildStorageData.h"
//#include "Components/ListView.h"
//#include "Components/WidgetSwitcher.h"
//#include "Management/ScopeGlobal/GsGuildManager.h"
//#include "Item/GsItem.h"
//#include "UI/UIContent/Tray/Guild/GsGuildStorageAcquireDetailListItem.h"
//#include "UI/UIContent/Tray/Guild/GsUIGuildStorageEntryAcquireDetail.h"
//
//
//void UGsUIGuildStoragePageAcquireDetail::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	_dataList.Empty();
//}
//
//void UGsUIGuildStoragePageAcquireDetail::NativeDestruct()
//{
//	_dataList.Empty();
//	_listView->ClearListItems();
//
//	if (true == _isOpened)
//	{
//		FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData();
//		storageData->UpdateAcquiredCheckTime();
//	}
//
//	Super::NativeDestruct();
//}
//
//void UGsUIGuildStoragePageAcquireDetail::InvalidateAllInternal()
//{
//	// 획득 내역에 변화가 있다면 데이터 갱신
//	FGsGuildStorageData* myGuildStorageData = GGuild()->GetMyGuildStorageData();
//	if (true == myGuildStorageData->GetDirtyAcquired() || 0 == _dataList.Num())
//	{
//		_dataList.Empty();
//		uint8 count = 0;
//		TArray<FGsGuildStorageAcquireDetailData*>& acquiredList = myGuildStorageData->_acquireDetailList;
//		for (uint8 i = 0; i < acquiredList.Num() && count < myGuildStorageData->MaxAcquiredItemHistoryCount; ++i)
//		{
//			if (nullptr == acquiredList[i])
//				continue;
//
//			UGsGuildStorageAcquireDetailListItem* acquiredDetail = NewObject<UGsGuildStorageAcquireDetailListItem>(this);
//			acquiredDetail->SetData(acquiredList[i]);
//			_dataList.Emplace(acquiredDetail);
//			++count;
//		}
//	}
//
//	myGuildStorageData->SetDirtyAcquired(false);
//
//	// 설정
//	_listView->SetListItems(_dataList);
//	_listView->ScrollToTop();
//
//	_switcherList->SetActiveWidgetIndex((_dataList.Num() == 0) ? 1 : 0);
//}
//
//void UGsUIGuildStoragePageAcquireDetail::Show()
//{
//	Super::Show();
//
//	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
//	InvalidateAll();
//}
//
//void UGsUIGuildStoragePageAcquireDetail::Hide()
//{
//	FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData();
//	storageData->UpdateAcquiredCheckTime();
//
//	FGsItem* item = nullptr;
//	for (uint8 i = 0; i < storageData->_acquireDetailList.Num(); ++i)
//	{
//		item = storageData->_acquireDetailList[i]->_acquiredItem;
//		item->SetIsNew(false);
//	}
//
//	const TArray<UUserWidget*>& entries = _listView->GetDisplayedEntryWidgets();
//	for (uint8 i = 0; i < entries.Num(); ++i)
//	{
//		UGsUIGuildStorageEntryAcquireDetail* entry = Cast<UGsUIGuildStorageEntryAcquireDetail>(entries[i]);
//		entry->InvalidateIcon();
//	}
//
//	Super::Hide();
//}