// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIPopupBMShopProductBuy.h"
#include "T1Project.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "GsUIBMShopProductDetail.h"
#include "GsUIBMShopProductTextEntry.h"
#include "GsUIBMShopProvideGroup.h"
#include "GsUIBMShopBuyPrice.h"
#include "GsUIBMShopBuyCalculator.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/BMShop/GsUIBMShopContractGuide.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "GameObject/Define/EBMShopProductDetailPopupType.h"
#include "Item/GsItem.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

const TCHAR* UGsUIPopupBMShopProductBuy::ASSET_NAME = TEXT("PopupBMShopProductBuy");

void UGsUIPopupBMShopProductBuy::BeginDestroy()
{
	if (nullptr != _textListPanelSlotHelper)
	{
		_textListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupBMShopProductBuy::OnRefreshText);
		_textListPanelSlotHelper = nullptr;
	}
	if (nullptr != _provideGroupPanelSlotHelper)
	{
		_provideGroupPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupBMShopProductBuy::OnRefreshItemGroup);
		_provideGroupPanelSlotHelper = nullptr;
	}


	Super::BeginDestroy();
}

void UGsUIPopupBMShopProductBuy::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnProbability->OnClicked.AddDynamic(this, &UGsUIPopupBMShopProductBuy::OnProbability);
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupBMShopProductBuy::OnOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupBMShopProductBuy::OnCancel);
	_btnCoupon->OnClicked.AddDynamic(this, &UGsUIPopupBMShopProductBuy::OnCoupon);

	if (nullptr == _textListPanelSlotHelper)
	{
		_textListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_textListPanelSlotHelper->Initialize(_textListEntryWidgetClass, _detailTextRootPanel);
		_textListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupBMShopProductBuy::OnRefreshText);
	}
	if (nullptr == _provideGroupPanelSlotHelper)
	{
		_provideGroupPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_provideGroupPanelSlotHelper->Initialize(_provideGroupEntryWidgetClass, _provideGroupRootPanel);
		_provideGroupPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupBMShopProductBuy::OnRefreshItemGroup);
	}
}

void UGsUIPopupBMShopProductBuy::NativeConstruct()
{
	Super::NativeConstruct();

	_currentAmountCount = 1;
	_productID = 0;

	_cashRebateInfo.mCurrencyType = CurrencyType::NONE;
	_cashRebateInfo.mCurrency = 0;

	_btnCoupon->SetVisibility(ESlateVisibility::Collapsed);

	_bmShopMsgDelegate = GMessage()->GetBMShop().AddUObject(FGsBMShopSalesTimeOver::MESSAGE_ID, this, &UGsUIPopupBMShopProductBuy::OnFinishedRemainTime);
}

void UGsUIPopupBMShopProductBuy::NativeDestruct()
{
	GMessage()->GetBMShop().Remove(_bmShopMsgDelegate);
	GBMShop()->SetReserveCouponItemDBID(0);

	_displayStringMap.Empty();
	_rewardItemDataList.Empty();

	_provideGroupPanelSlotHelper->RefreshAll(0);
	_textListPanelSlotHelper->RefreshAll(0);

	Super::NativeDestruct();
}

void UGsUIPopupBMShopProductBuy::OnFinishedRemainTime(FGsSharedParam& InParamData)
{
	const FGsBMShopSalesTimeOver* param = InParamData.GetParam<const FGsBMShopSalesTimeOver>();
	if (nullptr == param)
		return;

	if (_productID != param->_productID)
		return;

	FGsBMShopProductData* productData = GBMShop()->FindProduct(param->_productID);
	if (nullptr == productData)
		return;

	SetProductBuyInfo(productData);
}

void UGsUIPopupBMShopProductBuy::SetProductBuyInfo(const FGsBMShopProductData* InProductData)
{
	if (nullptr == InProductData)
	{
		GSLOG(Warning, TEXT("[UGsUIPopupBMShopProductBuy::SetProductBuyInfo] InProductData is nullptr !!"));
		return;
	}
	_productID = InProductData->GetProductID();

	// 아이템 name
	_itemNameText->SetText(InProductData->GetProductDisplayName());

	// 아이템 이미지 정보들
	_productDetailInfo->SetProductData(InProductData);

	// 아이템 상세text 
	InProductData->GetAllActiveDisplayText(_displayStringMap);
	_textListPanelSlotHelper->RefreshAll(_displayStringMap.Num());

	// 하단 switcher 세팅
	SetBottomInfo(InProductData);

	// 우측 상품 그룹 구성
	SetItemGroup(InProductData);

	_btnOk->SetIsEnabled(!InProductData->IsSlodOut());
	if (InProductData->IsStepUpProduct())
	{
		FGsBMShopManager::StepUpState state = GBMShop()->GetStepUpProductState(_productID);
		if (FGsBMShopManager::StepUpState::State_IsLock == state)
		{
			_btnOk->SetIsEnabled(false);
		}
	}
}

void UGsUIPopupBMShopProductBuy::SetBottomInfo(const FGsBMShopProductData* InProductData)
{
	/*
	* CashType = 0,  // 경고문만
	PackageCashType, // 경고문만

	InGameCurrencyType,  // 계산기
	InGameCurrencyWithCalcType, // 계산기

	PackageInGameCurrencyType, // 계산기
	PackageInGameCurrencyWithCalcType,  // 계산기
	*/

	_contractGuide->SetVisibility(IsShowContractNotice() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	// 0 : 가격만 출력 , 1 : 계산기 및 가격 , 2 : 안내문
	switch (InProductData->GetPopupDetailType())
	{
	case EBMShopProductDetailPopupType::OnlyPriceType:
		SetOnlyPrice(InProductData);
		break;
	case EBMShopProductDetailPopupType::WithCalculatorType:
	default:
		SetWithCalculator(InProductData);
		break;
	}
}

void UGsUIPopupBMShopProductBuy::SetOnlyPrice(const FGsBMShopProductData* InProductData)
{
	_bottomSwitcher->SetActiveWidgetIndex(BottomType::OnlyPrice);

	if (0 < GBMShop()->GetReserveCouponItemDBID())
	{
		InvalidateCoupontPrice(InProductData);
	}
	else
	{
		if (InProductData->IsItemCurrencyType())
		{
			_priceOnly->SetData(InProductData->GetPurchaseCurrencyItemId(), InProductData->GetPurchasePrice());
			_priceOnly->SetOwnedAmountData(InProductData->GetPurchaseCurrencyItemId());
		}
		else
		{
			_priceOnly->SetData(InProductData->GetPurchaseCurrencyType(), InProductData->GetPurchasePrice());
			_priceOnly->SetOwnedAmountData(InProductData->GetPurchaseCurrencyType());
		}
		_priceOnly->UpdateCostState();
	}

	_btnCoupon->SetVisibility(InProductData->IsCouponProduct() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);	
}

void UGsUIPopupBMShopProductBuy::SetWithCalculator(const FGsBMShopProductData* InProductData)
{
	_bottomSwitcher->SetActiveWidgetIndex(BottomType::WithCalculator);

	// 현금성 아이템은 구매갯수 계산기랑 같이 출력하는 팝업창은 사용하면 안?무조건 단일구매만 가능함 / hive 지원안함)
	//if (true == InProductData->IsHiveProduct())
	//	return;

	// 최대 구매가능한 갯수
	// 최대 구매갯수는 _purchasedQuantiy - GetPurchaseQuantiyLimit() 만큼 계산기에서 조절 가능
	// InMaximumCount 파라미터가 0이면 무제한 구매가 가능토록 해달라고 요청함
	_calculator->SetData(InProductData, InProductData->GetMaxCountSinglePurchase());

	if (false == _calculator->_onChangeCountDelegate.IsBoundToObject(this))
	{
		_calculator->_onChangeCountDelegate.AddUObject(this, &UGsUIPopupBMShopProductBuy::OnChangeCalculatorCountEvent);
	}
}

void UGsUIPopupBMShopProductBuy::OnChangeCalculatorCountEvent(const uint16 InCount)
{
	_currentAmountCount = InCount;
	//GSLOG(Warning, TEXT("[UGsUIPopupBMShopProductBuy::OnChangeCalculatorCountEvent] - InCount : %d"), InCount);
}

void UGsUIPopupBMShopProductBuy::SetItemGroup(const FGsBMShopProductData* InProductData)
{
	FGsBMShopManager* shopMgr = GBMShop();
	shopMgr->FindProductGroupInfoByProductID(InProductData->GetProductID(), _rewardItemDataList);

	_cashRebateInfo.mCurrencyType = CurrencyType::NONE;
	_cashRebateInfo.mCurrency = 0;

	if (InProductData->IsCashRebate())
	{
		_cashRebateInfo.mCurrencyType = InProductData->GetRebateCurrencyType();
		_cashRebateInfo.mCurrency = InProductData->GetRebateAmount();
	}
	if (0 >= _rewardItemDataList.Num())
	{
		_groupItemListRootPanel->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_groupItemListRootPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_provideGroupPanelSlotHelper->RefreshAll(_rewardItemDataList.Num());
}

void UGsUIPopupBMShopProductBuy::OnRefreshText(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopProductTextEntry* entry = Cast<UGsUIBMShopProductTextEntry>(InEntry);
	if (nullptr == entry)
		return;

	uint8 index = 0;
	for (const auto& iter : _displayStringMap)
	{
		if (InIndex == index)
		{
			entry->SetTitleText(FText::FromString(iter.Key));
			entry->SetDescText(FText::FromString(iter.Value));

			break;
		}

		++index;
	}
}

void UGsUIPopupBMShopProductBuy::OnRefreshItemGroup(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopProvideGroup* entry = Cast<UGsUIBMShopProvideGroup>(InEntry);
	if (nullptr == entry)
		return;

	if (_rewardItemDataList.IsValidIndex(InIndex))
	{
		entry->SetCashRebateInfo(_cashRebateInfo);

		FGsRewardUnboxingData data = _rewardItemDataList[InIndex];
		entry->SetProvideItem(data, (_rewardItemDataList.Num() - 1) == InIndex);
	}
}

void UGsUIPopupBMShopProductBuy::OnOk()
{
	if (0 >= _productID || 0 >= _currentAmountCount)
	{
		// B1 BM상점 튜토리얼에서 구매 진행 중 실패할 경우 바로 종료
		if (FGsTutorialManager* mgr = GSTutorial())
		{
			if (mgr->IsActiveTutorial())
			{
				GMessage()->GetTutorial().SendMessage(MessageContentTutorial::FORCED_END_TUTORIAL, nullptr);
			}
		}
		return;
	}

	FGsBMShopManager* BMShopManager = GBMShop();
	TArray<BMProductIdAmountPair> pairList;
	pairList.Emplace(BMProductIdAmountPair(_productID, _currentAmountCount));
	TSharedRef<FGsBMShopCheckValidProduct> BMShopCheckValidProduct = MakeShared<FGsBMShopCheckValidProduct>(pairList, false, 0 < BMShopManager->GetReserveCouponItemDBID(), 0 < BMShopManager->GetReserveCouponItemDBID() ? _priceOnly->GetDiscountAmount() : _currentAmountCount);
	BMShopManager->StartPurchase(BMShopCheckValidProduct);
}

void UGsUIPopupBMShopProductBuy::OnCancel()
{
	Close();
}

void UGsUIPopupBMShopProductBuy::OnProbability()
{

}

void UGsUIPopupBMShopProductBuy::InvalidateCurrency()
{
	FGsBMShopProductData* data =  GBMShop()->FindProduct(_productID);
	if (nullptr == data)
		return;

	SetBottomInfo(data);
}

void UGsUIPopupBMShopProductBuy::InvalidateLevel()
{
	FGsBMShopProductData* data = GBMShop()->FindProduct(_productID);
	if (nullptr == data)
		return;

	SetProductBuyInfo(data);
}

void UGsUIPopupBMShopProductBuy::OnCoupon()
{
	if (0 >= _productID)
		return;

	MakeShared<FGsBMShopOpenCouponList>(_productID)->Send();
}

void UGsUIPopupBMShopProductBuy::OnInputOk()
{
	OnOk();
}

void UGsUIPopupBMShopProductBuy::OnInputCancel()
{
	OnCancel();
}

void UGsUIPopupBMShopProductBuy::InvalidateCoupontPrice(FGsBMShopProductData* InProductData, FGsItem* InCouponItemData)
{
	if (nullptr == InProductData)
		return;

	if (nullptr == InCouponItemData)
	{
		SetOnlyPrice(InProductData);
		return;
	}

	int32 price = FGsBMShopManager::GetDiscountPrice(InProductData, InCouponItemData);
	if (0 > price)
	{
		SetOnlyPrice(InProductData);
		return;
	}

	if (InProductData->IsItemCurrencyType())
	{
		_priceOnly->SetCouponPriceData(InProductData->GetPurchaseCurrencyItemId(), InProductData->GetPurchasePrice(), price);
		_priceOnly->SetOwnedAmountData(InProductData->GetPurchaseCurrencyItemId());
	}
	else
	{
		_priceOnly->SetCouponPriceData(InProductData->GetPurchaseCurrencyType(), InProductData->GetPurchasePrice(), price);
		_priceOnly->SetOwnedAmountData(InProductData->GetPurchaseCurrencyType());
	}
	_priceOnly->UpdateDiscountCostState(price);
}

void UGsUIPopupBMShopProductBuy::InvalidateCoupontPrice(const FGsBMShopProductData* InProductData)
{
	if (nullptr == InProductData)
		return;

	if (InProductData->IsItemCurrencyType())
	{
		_priceOnly->SetCouponPriceData(InProductData->GetPurchaseCurrencyItemId(), InProductData->GetPurchasePrice(), _priceOnly->GetDiscountAmount());
		_priceOnly->SetOwnedAmountData(InProductData->GetPurchaseCurrencyItemId());
	}
	else
	{
		_priceOnly->SetCouponPriceData(InProductData->GetPurchaseCurrencyType(), InProductData->GetPurchasePrice(), _priceOnly->GetDiscountAmount());
		_priceOnly->SetOwnedAmountData(InProductData->GetPurchaseCurrencyType());
	}
	_priceOnly->UpdateDiscountCostState(_priceOnly->GetDiscountAmount());
}

bool UGsUIPopupBMShopProductBuy::IsShowContractNotice() const
{
	const UGsGlobalConstant* GlobalConstant = GData()->GetGlobalData();
	if (nullptr == GlobalConstant)
	{
		GSLOG(Error, TEXT("nullptr == GlobalConstant"));
		return false;
	}

	if (!GlobalConstant->_showContractNoticeOnPurchasePopup)
	{
		return false;
	}

	const FGsBMShopProductData* BMShopProductData = GBMShop()->FindProduct(_productID);
	if (nullptr == BMShopProductData)
	{
		return false;
	}

	return CurrencyType::CASH == BMShopProductData->GetPurchaseCurrencyType();
}

void UGsUIPopupBMShopProductBuy::ExpiredCouponItem()
{
	FGsBMShopProductData* findData = GBMShop()->FindProduct(_productID);
	if (nullptr != findData)
	{
		SetOnlyPrice(findData);
	}
}
