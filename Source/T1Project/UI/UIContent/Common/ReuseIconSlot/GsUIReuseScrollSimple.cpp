//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "UI/UIContent/Common/ReuseIconSlot/GsUIReuseScrollSimple.h"
//#include "T1Project.h"
//
//
//
//
//void UGsUIReuseScrollSimple::NativeOnInitialized()
//{
//	Super::NativeOnInitialized();
//
//}
//
//void UGsUIReuseScrollSimple::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//
//}
//
//void UGsUIReuseScrollSimple::SetDefaultPage()
//{
//	SetDefaultPos();
//}
//
//void UGsUIReuseScrollSimple::SetSlotAutoFocusing(const int32 InSlotIndex)
//{
//	if (0 > InSlotIndex)
//		return;
//
//	uint8 pageIndex = 0;
//	int32 result = InSlotIndex + 1;
//	while (0 < result)
//	{
//		result -= (uint8)_maxSlotCountType;
//		if (0 < result)
//		{
//			++pageIndex;
//		}
//	}
//
//	EGsIconSlotCountType convertType = pageIndex >= (uint8)EGsIconSlotCountType::MaxSlotType ? 
//		EGsIconSlotCountType::One : (EGsIconSlotCountType)pageIndex;
//
//	EGsIconSlotCountType type = GetCurrentPageType();
//	if (type != convertType)
//	{
//		AutoFocusingScroll(convertType);
//	}
//
//#if WITH_EDITOR
//	GSLOG(Error, TEXT("[UGsUIReuseScrollSimple::SetSlotAutoFocusing] - pageIndex : %d"), pageIndex);
//#endif
//}
//
//void UGsUIReuseScrollSimple::NativeDestruct()
//{
//	Super::NativeDestruct();
//}
//
//void UGsUIReuseScrollSimple::InvalidateAllSlot()
//{
//	Super::InvalidateAllSlot();
//
//}
//
//void UGsUIReuseScrollSimple::InvalidateSlotList(TArray<uint8>& InSlotIndexList)
//{
//	Super::InvalidateSlotList(InSlotIndexList);
//
//}
//
//void UGsUIReuseScrollSimple::InvalidateSwitcher()
//{
//	Super::InvalidateSwitcher();
//
//}
//
//void UGsUIReuseScrollSimple::InvalidateAll()
//{
//	InvalidateAllSlot();
//}
//
//void UGsUIReuseScrollSimple::InvalidateAll(TArray<FGsNpcShopQuickSlotItemInfo*> InNpcShopSlotInfo)
//{
//	bool isEmpty = 0 >= InNpcShopSlotInfo.Num();
//	int32 index = 0;
//	for (auto& iter : _slotInfoMap)
//	{
//		if (isEmpty)
//		{
//			iter.Value->_itemDBID = 0;
//			iter.Value->_itemTID = 0;
//			iter.Value->_itemAmount = 0;
//		}
//		else
//		{
//			if (InNpcShopSlotInfo.IsValidIndex(index))
//			{
//				FGsNpcShopQuickSlotItemInfo* data = InNpcShopSlotInfo[index];
//				if (nullptr != data)
//				{
//					iter.Value->_itemDBID = data->_itemDBID;
//					iter.Value->_itemTID = data->_itemTID;
//					iter.Value->_itemAmount = data->_itemAmount;
//				}
//
//				++index;
//			}
//			else
//			{
//				iter.Value->_itemDBID = 0;
//				iter.Value->_itemTID = 0;
//				iter.Value->_itemAmount = 0;
//			}
//		}
//	}
//
//	InvalidateAllSlot();
//}