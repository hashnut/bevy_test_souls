#include "GsUIBMShopBuyCalculator.h"

#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "T1Project.h"
#include "UI/UIContent/Window/BMShop/GsUIBMShopBuyPrice.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUIBMShopBuyCalculator::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnReset->OnClicked.AddDynamic(this, &UGsUIBMShopBuyCalculator::OnClickReset);
	_btnAddAmountFive->OnClicked.AddDynamic(this, &UGsUIBMShopBuyCalculator::OnClickAddFive);
	_btnAddAmountTen->OnClicked.AddDynamic(this, &UGsUIBMShopBuyCalculator::OnClickAddTen);
	_btnMin->OnClicked.AddDynamic(this, &UGsUIBMShopBuyCalculator::OnClickMin);
	_btnPlus->OnClicked.AddDynamic(this, &UGsUIBMShopBuyCalculator::OnClickPlus);
}

void UGsUIBMShopBuyCalculator::NativeConstruct()
{
	Super::NativeConstruct();

	_currentCount = 1;
	_maximumCount = 0;
	_costInfo = nullptr;

	_currentAmountText->SetText(FText::AsNumber(_currentCount));
}

void UGsUIBMShopBuyCalculator::NativeDestruct()
{
	if (_onChangeCountDelegate.IsBound())
	{
		_onChangeCountDelegate.Clear();
	}

	_costInfo = nullptr;

	Super::NativeDestruct();
}

// 0 == InMaximumCount 이면, 무제한 구매가 되게 해달라고 함
void UGsUIBMShopBuyCalculator::SetData(const FGsBMShopProductData* InProductData, const int32 InMaximumCount)
{
	if (nullptr == InProductData)
		return;

	if (0 > InMaximumCount)
	{
		GSLOG(Warning, TEXT("[UGsUIBMShopBuyCalculator::SetData] 0 > InMaximumCount - %d"), InMaximumCount);
		return;
	}

	FGsBMShopProductData* castData = const_cast<FGsBMShopProductData*>(InProductData);
	_costInfo = castData->GetProductCostInfo();
	if (nullptr == _costInfo)
		return;

	if (0 == InMaximumCount)
	{
		_maximumCount = InProductData->IsSlodOut() ? _currentCount : InMaximumCount;
	}
	else
	{
		_maximumCount = InMaximumCount;
	}

	UpdateData();

	if (InProductData->IsItemCurrencyType())
	{
		_currentCost->SetOwnedAmountData(InProductData->GetPurchaseCurrencyItemId());
	}
	else
	{
		_currentCost->SetOwnedAmountData(InProductData->GetPurchaseCurrencyType());
	}	
}

void UGsUIBMShopBuyCalculator::UpdateData()
{
	if (_costInfo)
	{
		if (_costInfo->IsItemCurrencyType())
		{
			_currentCost->SetData(_costInfo->GetCurrencyItemTid(), (_costInfo->GetAmount() * _currentCount));
		}
		else
		{
			_currentCost->SetData(_costInfo->GetCurrencyType(), (_costInfo->GetAmount() * _currentCount));
		}
		_currentCost->UpdateCostState();
		_currentAmountText->SetText(FText::AsNumber(_currentCount));

		if (true == _onChangeCountDelegate.IsBound())
		{
			_onChangeCountDelegate.Broadcast(_currentCount);
		}
	}
}

void UGsUIBMShopBuyCalculator::OnClickReset()
{
	_currentCount = 1;
	UpdateData();
}

void UGsUIBMShopBuyCalculator::OnClickAddFive()
{
	if (0 == _maximumCount) // 무제한 구매
	{
		_currentCount += ADD_VALUE;
		UpdateData();
	}
	else
	{
		if (_maximumCount <= (_currentCount + ADD_VALUE))
		{
			_currentCount = _maximumCount;
			UpdateData();
			return;
		}

		_currentCount += ADD_VALUE;
		UpdateData();
	}
}

void UGsUIBMShopBuyCalculator::OnClickAddTen()
{
	if (0 == _maximumCount) // 무제한 구매
	{
		_currentCount += (ADD_VALUE * 2);
		UpdateData();
	}
	else
	{
		if (_maximumCount <= (_currentCount + (ADD_VALUE * 2)))
		{
			_currentCount = _maximumCount;
			UpdateData();
			return;
		}

		_currentCount += (ADD_VALUE * 2);
		UpdateData();
	}
}

void UGsUIBMShopBuyCalculator::OnClickMin()
{
	if (1 >= _currentCount)
	{
		return;
	}

	_currentCount -= 1;
	UpdateData();
}

void UGsUIBMShopBuyCalculator::OnClickPlus()
{
	if (0 == _maximumCount) // 무제한 구매
	{
		_currentCount += 1;
		UpdateData();
	}
	else
	{
		if (_maximumCount <= (_currentCount + 1) )
		{
			_currentCount = _maximumCount;
			UpdateData();
			return;
		}

		_currentCount += 1;
		UpdateData();
	}
}
