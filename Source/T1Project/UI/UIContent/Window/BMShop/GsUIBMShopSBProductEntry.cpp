// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIBMShopSBProductEntry.h"
#include "T1Project.h"
#include "SBoxPanel.h"
#include "UTIL/GsTimeUtil.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "GsUIBMShopProductEntry.h"
#include "GsUIBMShopSmallBanner.h"
#include "Components/TextBlock.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "GsUIBMShopProductDetail.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "UI/UIContent/Common/GsUISwipeCircle.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"



void UGsUIBMShopSBProductEntry::BeginDestroy()
{
	if (nullptr != _productListPanelSlotHelper)
	{
		_productListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIBMShopSBProductEntry::ProductListRefresh);
		_productListPanelSlotHelper = nullptr;
	}
	if (nullptr != _smallBannerListPanelSlotHelper)
	{
		_smallBannerListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIBMShopSBProductEntry::SmallBannerListRefresh);
		_smallBannerListPanelSlotHelper = nullptr;
	}
	if (nullptr != _swipeCirclePanelSlotHelper)
	{
		_swipeCirclePanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIBMShopSBProductEntry::SwipeCircleListRefresh);
		_swipeCirclePanelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIBMShopSBProductEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _productListPanelSlotHelper)
	{
		_productListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_productListPanelSlotHelper->Initialize(_productListEntryWidgetClass, _productRootPanel);
		_productListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIBMShopSBProductEntry::ProductListRefresh);
	}
	if (nullptr == _smallBannerListPanelSlotHelper)
	{
		_smallBannerListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_smallBannerListPanelSlotHelper->Initialize(_smallBannerListEntryWidgetClass, _bannerRootPanel);
		_smallBannerListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIBMShopSBProductEntry::SmallBannerListRefresh);
	}
	if (nullptr == _swipeCirclePanelSlotHelper)
	{
		_swipeCirclePanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_swipeCirclePanelSlotHelper->Initialize(_swipeCircleEntryWidgetClass, _swipeCircleRoot);
		_swipeCirclePanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIBMShopSBProductEntry::SwipeCircleListRefresh);
	}

	_bannerScrollBox->OnUserScrolled.AddDynamic(this, &UGsUIBMShopSBProductEntry::OnChangeScrollEvent);

	// 버튼 클릭텀을 무조건 1초로 설정해둠
	// 슬롯간 swap 할 시간을 위하여
	_btnLeft->OnClicked.AddDynamic(this, &UGsUIBMShopSBProductEntry::OnBtnLeft);
	_btnLeft->_clickLockSecond = 1.0f;

	_btnRight->OnClicked.AddDynamic(this, &UGsUIBMShopSBProductEntry::OnBtnRight);
	_btnRight->_clickLockSecond = 1.0f;

	_timer = NewObject<UGsSimpleTimer>();
	float min;
	float max;
	_curve->GetTimeRange(min, max);
	_timer->SetMaxTime(max);

	_timer->Stop();
	_timer->Reset();
}

void UGsUIBMShopSBProductEntry::OnShow()
{
	_curPage = 0;
	_focusingSlotIndex = 0;
	_autoScrollTimeCheck = 0.f;
	_widthSize = 0.f;
	_isScrolling = false;
	_isUseScrolling = true;
	_pagePosMap.Empty();
	_bannerIDList.Empty();
	_productIDList.Empty();
	_autoMoveDirType = AutoMoveDirType::MoveNoneType;

	_bannerScrollBox->ScrollToStart();
}

void UGsUIBMShopSBProductEntry::OnHide()
{
}

void UGsUIBMShopSBProductEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 스크롤 하다가 릴리즈 한 상태
	if (_isUseScrolling)
	{
		if (false == _bannerScrollBox->HasMouseCapture() && true == _isScrolling)
		{
			// 다음틱에서..
			if (false == _timer->CanTick())
			{
				_timer->Reset();
				_timer->Start();
				return;
			}
			if (_timer->IsFinish())
			{
				_timer->Stop();
			}

			_timer->Tick(InDeltaTime);
			if (_curve)
			{
				float curveValue = _curve->GetFloatValue(_timer->GetCurrentTime());
				AutoScroll(curveValue);
			}
		}

		// banner 자동 스크롤
		if (false == _isScrolling)
		{
			if (_autoScrollDurationTime >= _autoScrollTimeCheck)
			{
				_autoScrollTimeCheck += InDeltaTime;
			}
			else
			{
				_isScrolling = true;
				_autoScrollTimeCheck = 0.f;
				_autoMoveDirType = AutoMoveDirType::MoveRightType;			
			}
		}
	}
}

void UGsUIBMShopSBProductEntry::TimeAutoScroll(float InCurveValue)
{
	if (_pagePosMap.Contains(_focusingSlotIndex + 1))
	{
		_autoMoveDirType = AutoMoveDirType::MoveRightType;
	}	
}

void UGsUIBMShopSBProductEntry::SetProduct(TArray<BMProductId> InProductList, int32 InFocusingProductID)
{
	_productIDList = InProductList;

	_bannerIDList.Empty();
	TArray<FGsBMShopProductData*> productDataList;
	FGsBMShopManager* bmShopMgr = GBMShop();
	for (const BMProductId iter : _productIDList)
	{
		FGsBMShopProductData* data = bmShopMgr->FindProduct(iter);
		if(nullptr == data)
			continue;

		if(0 >= data->GetBannerDisplayOrder())
			continue;

		productDataList.Emplace(data);
	}

	// banner 정렬
	productDataList.Sort([](FGsBMShopProductData& dataA, FGsBMShopProductData& dataB)
		{
			return dataA.GetBannerDisplayOrder() > dataB.GetBannerDisplayOrder();
		}
	);

	for (const FGsBMShopProductData* iter : productDataList)
	{
		_bannerIDList.Emplace(iter->GetProductID());
	}

	_swipeCirclePanelSlotHelper->RefreshAll(_bannerIDList.Num());

	if (2 == _bannerIDList.Num())
	{
		if (_bannerIDList.IsValidIndex(0) || _bannerIDList.IsValidIndex(1))
		{
			_bannerIDList.Emplace(_bannerIDList[0]);
			_bannerIDList.Emplace(_bannerIDList[1]);
		}
	}
	_isUseScrolling = 1 < _bannerIDList.Num();
	_btnLeft->SetVisibility(1 < _bannerIDList.Num() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_btnRight->SetVisibility(1 < _bannerIDList.Num() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	_smallBannerListPanelSlotHelper->RefreshAll(_bannerIDList.Num());
	_productListPanelSlotHelper->RefreshAll(_productIDList.Num());
	
	if (0 < InFocusingProductID)
	{
		FocusingProduct(InFocusingProductID);
	}
	else 
	{
		SwapSlot();
	}
}

void UGsUIBMShopSBProductEntry::FocusingProduct(const BMProductId InProductID)
{
	int32 index = 0;
	for (BMProductId iter : _bannerIDList)
	{
		if (InProductID != iter)
		{
			++index;
			continue;
		}

		if (_pagePosMap.Contains(index))
		{
			_curPage = index;
			_focusingSlotIndex = index;
			_bannerScrollBox->SetScrollOffset(_pagePosMap[index]);
		}

		SwapSlot();
		OnChangeSwipeCircle();

		break;
	}
}

void UGsUIBMShopSBProductEntry::InvalidateProduct()
{
	/*
	SetProductName();
	SetSaleRemainTime();
	SetSaleCurrencyInfo();
	*/
	_productListPanelSlotHelper->RefreshAll(_productIDList.Num());
}

void UGsUIBMShopSBProductEntry::OnChangeScrollEvent(float InCurOffset)
{
	//GSLOG(Warning, TEXT("[UGsUIBMShopSBProductEntry::OnChangeScrollEvent] - InCurOffset : %f"), InCurOffset);
	if (false == _isUseScrolling)
		return;

	if (true == _bannerScrollBox->HasMouseCapture())
	{
		_isScrolling = true;
		_timer->Stop();
		_autoScrollTimeCheck = 0.f;

		CheckCurPos();
	}
	else
	{
		float pagePos = _pagePosMap[_focusingSlotIndex];		
		float curveValue = _curve->GetFloatValue(_timer->GetCurrentTime());
		AutoScroll(curveValue);
		if (false == _isScrolling)
		{
			// targetPos 로 강제로 set 처리한다
			float targetPos = _pagePosMap[_focusingSlotIndex];
			_bannerScrollBox->SetScrollOffset(targetPos);
		}	
	}
}

void UGsUIBMShopSBProductEntry::CheckCurPos()
{
	if (_pagePosMap.Contains(_focusingSlotIndex))
	{
		float curOffset = _bannerScrollBox->GetScrollOffset();
		float pagePos = _pagePosMap[_focusingSlotIndex];
		if (pagePos + (_widthSize / 2.f) <= curOffset) // 우측으로 이동
		{
			if (_pagePosMap.Contains(_focusingSlotIndex + 1))
			{
				_autoMoveDirType = AutoMoveDirType::MoveRightType;
				//GSLOG(Error, TEXT("[CheckCurPos] - move right"));
			}
		}
		else if (pagePos - (_widthSize / 2.f) >= curOffset) // 좌측으로 이동
		{
			if (_pagePosMap.Contains(_focusingSlotIndex - 1))
			{
				_autoMoveDirType = AutoMoveDirType::MoveLeftType;
				//GSLOG(Error, TEXT("[CheckCurPos] - move left"));
			}
		}
		else // 원복
		{
			if (pagePos < curOffset)
			{
				_autoMoveDirType = AutoMoveDirType::MoveReturnLeftType;
				//GSLOG(Error, TEXT("[CheckCurPos] - move MoveReturnLeftType"));
			}
			else
			{
				_autoMoveDirType = AutoMoveDirType::MoveReturnRightType;
				//GSLOG(Error, TEXT("[CheckCurPos] - move MoveReturnRightType"));
			}
		}
	}
}

void UGsUIBMShopSBProductEntry::MoveAutoScroll(float InCurveValue, float InGoalPos, AutoMoveDirType InMoveDirType, uint8 InTargetSlotIndex)
{
	//_bannerScrollBox->SetIsEnabled(false);
	float curOffset = _bannerScrollBox->GetScrollOffset();
	switch (InMoveDirType)
	{
	case AutoMoveDirType::MoveLeftType:
	case AutoMoveDirType::MoveReturnLeftType:
	{
		if (curOffset > InGoalPos)
		{
			_bannerScrollBox->SetScrollOffset(curOffset - InCurveValue);
		}
		else
		{
			_focusingSlotIndex = InTargetSlotIndex;
			_bannerScrollBox->SetScrollOffset(InGoalPos);
			//_bannerScrollBox->SetIsEnabled(true);
			_isScrolling = false;
			_autoMoveDirType = AutoMoveDirType::MoveNoneType;
			if (AutoMoveDirType::MoveLeftType == InMoveDirType)
			{
				_curPage = 0 >= _curPage ? _swipeCirclePanelSlotHelper->GetChildrenCount() - 1 : _curPage - 1;
			}
		}
		break;
	}
	case AutoMoveDirType::MoveRightType:
	case AutoMoveDirType::MoveReturnRightType:
	{
		if (curOffset < InGoalPos)
		{
			_bannerScrollBox->SetScrollOffset(curOffset + InCurveValue);
		}
		else
		{
			_focusingSlotIndex = InTargetSlotIndex;
			_bannerScrollBox->SetScrollOffset(InGoalPos);
			//_bannerScrollBox->SetIsEnabled(true);
			_isScrolling = false;
			_autoMoveDirType = AutoMoveDirType::MoveNoneType;
			if (AutoMoveDirType::MoveRightType == InMoveDirType)
			{
				_curPage = _swipeCirclePanelSlotHelper->GetChildrenCount() - 1 <= _curPage ? 0 : _curPage + 1;
			}
		}
		break;
	}

	default:
		_bannerScrollBox->SetScrollOffset(InGoalPos);
		//_bannerScrollBox->SetIsEnabled(true);
		_isScrolling = false;
		_focusingSlotIndex = InTargetSlotIndex;
		_autoMoveDirType = AutoMoveDirType::MoveNoneType;
		break;
	}

	if (AutoMoveDirType::MoveNoneType == _autoMoveDirType)
	{
		SwapSlot();
		OnChangeSwipeCircle();
		_timer->Stop();
	}
}

void UGsUIBMShopSBProductEntry::AutoScroll(float InCurveValue)
{
	if (_pagePosMap.Contains(_focusingSlotIndex))
	{
		switch (_autoMoveDirType)
		{
		case AutoMoveDirType::MoveLeftType:
		{
			if (_pagePosMap.Contains(_focusingSlotIndex - 1))
			{
				MoveAutoScroll(InCurveValue, _pagePosMap[_focusingSlotIndex - 1], AutoMoveDirType::MoveLeftType, _focusingSlotIndex - 1);
			}
			break;
		}
		case AutoMoveDirType::MoveRightType:
		{
			if (_pagePosMap.Contains(_focusingSlotIndex + 1))
			{
				MoveAutoScroll(InCurveValue, _pagePosMap[_focusingSlotIndex + 1], AutoMoveDirType::MoveRightType, _focusingSlotIndex + 1);
			}
			break;
		}
		case AutoMoveDirType::MoveReturnLeftType:
			MoveAutoScroll(InCurveValue, _pagePosMap[_focusingSlotIndex], AutoMoveDirType::MoveReturnLeftType, _focusingSlotIndex);
			break;
		case AutoMoveDirType::MoveReturnRightType:
			MoveAutoScroll(InCurveValue, _pagePosMap[_focusingSlotIndex], AutoMoveDirType::MoveReturnRightType, _focusingSlotIndex);
			break;
		}
	}
}

void UGsUIBMShopSBProductEntry::OnChangeSwipeCircle()
{
	_swipeCirclePanelSlotHelper->RefreshAll(_swipeCirclePanelSlotHelper->GetChildrenCount());
}

void UGsUIBMShopSBProductEntry::SwapSlot()
{
	if (2 > _bannerIDList.Num())
		return;

	if (2 == _bannerIDList.Num())
	{
	}
	else
	{
		if (_pagePosMap.Contains(0) && _pagePosMap.Contains(_bannerIDList.Num() - 1))
		{
			float curOffset = _bannerScrollBox->GetScrollOffset();
			float firstPos = _pagePosMap[0] + (_widthSize / 2.f);
			float lastPos = _pagePosMap[_bannerIDList.Num() - 1] - (_widthSize / 2.f);

			if (firstPos > curOffset)
			{
				SetSlotMove(_bannerIDList.Num() - 1, 0);
				_bannerScrollBox->SetScrollOffset(_pagePosMap[1]);
				_focusingSlotIndex = 1;
			}
			else if (lastPos < curOffset)
			{
				SetSlotMove(0, _bannerIDList.Num() - 1);
				_bannerScrollBox->SetScrollOffset(_pagePosMap[_bannerIDList.Num() - 2]);
				_focusingSlotIndex = _bannerIDList.Num() - 2;
			}
		}
	}
}

void UGsUIBMShopSBProductEntry::ProductListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopProductEntry* entry = Cast<UGsUIBMShopProductEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _productIDList.IsValidIndex(InIndex))
		return;

	entry->SetProductID(_productIDList[InIndex]);

	const TArray<UPanelSlot*>& slots = _productRootPanel->GetSlots();
	if (UPanelSlot* slot = slots[InIndex])
	{
		if (UGridSlot* gridSlot = Cast<UGridSlot>(slot))
		{
			gridSlot->SetRow(0);
			gridSlot->SetColumn(InIndex);
		}
	}
}

void UGsUIBMShopSBProductEntry::SmallBannerListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopSmallBanner* entry = Cast<UGsUIBMShopSmallBanner>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _bannerIDList.IsValidIndex(InIndex))
		return;

	if (false == _pagePosMap.Contains(InIndex))
	{
		_widthSize = entry->GetWidthSize();
		_pagePosMap.Emplace(InIndex, _widthSize * InIndex);
	}

	if (false == entry->OnClickSmallBanner.IsBoundToObject(this))
	{
		entry->OnClickSmallBanner.AddUObject(this, &UGsUIBMShopSBProductEntry::OnClickSmallBanner);
	}

	entry->SetProductID(_bannerIDList[InIndex]);
}

void UGsUIBMShopSBProductEntry::OnClickSmallBanner(BMProductId InProductID)
{
	//GSLOG(Warning, TEXT("[UGsUIBMShopSBProductEntry::OnClickSmallBanner] - InProductID : %d"), InProductID);
}

// 버튼을 통한 이동은 자동이동 연출이 없다고 함
void UGsUIBMShopSBProductEntry::OnBtnLeft()
{
	if (_isScrolling)
		return;

	if (0 < _focusingSlotIndex)
	{
		int32 goalIndex = _focusingSlotIndex - 1;
		if (_pagePosMap.Contains(goalIndex))
		{
			_bannerScrollBox->SetScrollOffset(_pagePosMap[goalIndex]);
			_curPage = 0 >= _curPage ? _swipeCirclePanelSlotHelper->GetChildrenCount() - 1 : _curPage - 1;
			_focusingSlotIndex = goalIndex;
			_autoScrollTimeCheck = 0.f;
		}
	}

	SwapSlot();
	OnChangeSwipeCircle();
}

// 버튼을 통한 이동은 자동이동 연출이 없다고 함
void UGsUIBMShopSBProductEntry::OnBtnRight()
{
	if (_isScrolling)
		return;

	if (_focusingSlotIndex < _bannerIDList.Num() - 1)
	{
		int32 goalIndex = _focusingSlotIndex + 1;
		if (_pagePosMap.Contains(goalIndex))
		{
			_bannerScrollBox->SetScrollOffset(_pagePosMap[goalIndex]);
			_curPage = _swipeCirclePanelSlotHelper->GetChildrenCount() - 1 <= _curPage ? 0 : _curPage + 1;
			_focusingSlotIndex = goalIndex;
			_autoScrollTimeCheck = 0.f;
		}
	}

	SwapSlot();
	OnChangeSwipeCircle();
}

void UGsUIBMShopSBProductEntry::SwipeCircleListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUISwipeCircle* entry = Cast<UGsUISwipeCircle>(InEntry);
	if (nullptr == entry)
		return;

	entry->SetCircleActive(_curPage == InIndex);
}

void UGsUIBMShopSBProductEntry::ResetBannerChild()
{
	if (nullptr != _smallBannerListPanelSlotHelper)
	{
		_smallBannerListPanelSlotHelper->AllClearChildren();
	}
}


void UGsUIBMShopSBProductEntry::SetSlotMove(int32 InMoveTargetPageIndex, int32 InDestPageIndex)
{
	TSharedRef<class SWidget> sharedRef = _bannerRootPanel->TakeWidget();
	class SHorizontalBox* horizontalBox = static_cast<class SHorizontalBox*>(&sharedRef.Get());
	if (nullptr == horizontalBox)
	{
		check(horizontalBox);
		return;
	}

	FChildren* children = horizontalBox->GetChildren();
	TPanelChildren<SHorizontalBox::FSlot>* PanelChildren = static_cast<TPanelChildren<SHorizontalBox::FSlot>*>(children);
	PanelChildren->Move(InMoveTargetPageIndex, InDestPageIndex);
}
