// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISampleScroll.h"

#include "Components/ScrollBox.h"
#include "Components/ListView.h"
#include "Components/TileView.h"
#include "Components/TreeView.h"
#include "Components/PanelWidget.h"

#include "UI/Sample/Scroll/GsSampleListItem.h"
#include "UI/Sample/Scroll/GsSampleTreeItem.h"
#include "UI/Sample/Scroll/GsUISampleEntry.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"


void UGsUISampleScroll::BeginDestroy()
{
	if (_dynamicSlotHelper)
	{
		_dynamicSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUISampleScroll::OnRefreshEntryDynamic);
	}
	_dynamicSlotHelper = nullptr;

	_staticSlotHelper.OnRefreshEntry.Unbind();

	Super::BeginDestroy();
}

void UGsUISampleScroll::NativeOnInitialized()
{
	_dynamicSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_dynamicSlotHelper->Initialize(_scrollBoxEntryClass, _panelDynamic);
	_dynamicSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUISampleScroll::OnRefreshEntryDynamic);

	_staticSlotHelper.Initialize(_panelStatic, true);
	_staticSlotHelper.OnRefreshEntry.BindUObject(this, &UGsUISampleScroll::OnRefreshEntryStatic);

	Super::NativeOnInitialized();
}

void UGsUISampleScroll::NativeConstruct()
{
	Super::NativeConstruct();

	SetStaticData(10);
	SetDynamicData(10);
	SetListViewData(10);
	SetTileViewData(10);
	SetTreeViewData();
}

void UGsUISampleScroll::NativeDestruct()
{
	_dataListDynamic.Empty();
	_dataListStatic.Empty();
	_listItemList.Empty();
	_tileItemList.Empty();
	_treeRootItemList.Empty();

	Super::NativeDestruct();
}

void UGsUISampleScroll::SetStaticData(int32 InDataCount)
{
	// 데이터를 만들어도 배치해 둔 슬롯 이상 표시 못함
	_dataListStatic.Empty();

	for (int32 i = 0; i < InDataCount; ++i)
	{
		_dataListStatic.Emplace(FGsSampleData(i));
	}

	_staticSlotHelper.RefreshAll(_dataListStatic.Num());
}

void UGsUISampleScroll::SetDynamicData(int32 InDataCount)
{
	_dataListDynamic.Empty();

	for (int32 i = 0; i < InDataCount; ++i)
	{
		_dataListDynamic.Emplace(FGsSampleData(i));
	}

	_dynamicSlotHelper->RefreshAll(_dataListDynamic.Num());
}

void UGsUISampleScroll::SetListViewData(int32 InDataCount)
{
	_listItemList.Empty();

	// 예제 데이터(ListItem) 만들기
	for (int32 i = 0; i < InDataCount; ++i)
	{
		// ListView용 ListItem 생성
		UGsSampleListItem* listItem = NewObject<UGsSampleListItem>(this);
		if (listItem)
		{
			listItem->SetItemId(i);
		}

		_listItemList.Emplace(listItem);
	}

	_listView->SetListItems(_listItemList);
}

void UGsUISampleScroll::SetTileViewData(int32 InDataCount)
{
	_tileItemList.Empty();

	// 예제 데이터(ListItem) 만들기
	for (int32 i = 0; i < InDataCount; ++i)
	{
		// ListView용 ListItem 생성
		UGsSampleListItem* listItem = NewObject<UGsSampleListItem>(this);
		if (listItem)
		{
			listItem->SetItemId(i);
		}

		_tileItemList.Emplace(listItem);
	}

	_tileView->SetListItems(_tileItemList);
}

void UGsUISampleScroll::SetTreeViewData()
{
	for (int32 i = 0; i < 10; ++i)
	{
		int id = i + 1;

		UGsSampleTreeItem* item = NewObject<UGsSampleTreeItem>(this);
		item->SetItemId(id);

		for (int32 j = 0; j < 3; ++j)
		{
			UGsSampleTreeItem* childItem = NewObject<UGsSampleTreeItem>(this);
			childItem->SetItemId(id * 10 + j);
			item->AddChild(childItem);
		}

		_treeRootItemList.Add(item);

		_treeView->SetOnGetItemChildren<UGsSampleTreeItem>(item, &UGsSampleTreeItem::OnGetChildren);
	}

	_treeView->SetListItems(_treeRootItemList);
}

void UGsUISampleScroll::OnRefreshEntryDynamic(int32 InIndex, UWidget* InEntry)
{
	if (UGsUISampleEntry* entry = Cast<UGsUISampleEntry>(InEntry))
	{
		if (_dataListDynamic.IsValidIndex(InIndex))
		{
			entry->SetListItem(&_dataListDynamic[InIndex]);
		}
	}
}

void UGsUISampleScroll::OnRefreshEntryStatic(int32 InIndex, UWidget* InEntry)
{
	if (UGsUISampleEntry* entry = Cast<UGsUISampleEntry>(InEntry))
	{
		if (_dataListStatic.IsValidIndex(InIndex))
		{
			entry->SetListItem(&_dataListStatic[InIndex]);
		}
	}
}

void UGsUISampleScroll::SetDataCountStatic(int32 InDataCount)
{
	SetStaticData(InDataCount);
	_staticSlotHelper.RefreshAll(InDataCount);
}

void UGsUISampleScroll::SetDataCountDynamic(int32 InDataCount)
{
	SetDynamicData(InDataCount);
	_dynamicSlotHelper->RefreshAll(InDataCount);
}

void UGsUISampleScroll::SetDataCountListView(int32 InDataCount)
{
	SetListViewData(InDataCount);
	_listView->SetListItems(_listItemList);
}

void UGsUISampleScroll::SetDataCountTileView(int32 InDataCount)
{
	SetTileViewData(InDataCount);
	_tileView->SetListItems(_tileItemList);
}