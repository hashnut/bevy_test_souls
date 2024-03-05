// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIPopupCalculator.h"

#include "Components/TextBlock.h"

#include "Management/ScopeGame/GsInventoryManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Runtime/UMG/Public/Components/SlateWrapperTypes.h"


void UGsUIPopupCalculator::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnMinus->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickMinus);
	_btnPlus->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickPlus);
	_btnNumPad1->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickNumPad1);
	_btnNumPad2->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickNumPad2);
	_btnNumPad3->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickNumPad3);
	_btnNumPad4->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickNumPad4);
	_btnNumPad5->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickNumPad5);
	_btnNumPad6->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickNumPad6);
	_btnNumPad7->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickNumPad7);
	_btnNumPad8->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickNumPad8);
	_btnNumPad9->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickNumPad9);
	_btnNumPad0->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickNumPad0);
	_btnClear->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickClear);
	_btnEraseBack->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickRemove);
	_btnAdd10->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickAdd10);
	_btnAdd100->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickAdd100);
	_btnAdd1000->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickAdd1000);
	_btnAddMax->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickAddMax);
	_btnMax70->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickMaxFirst);
	_btnMax90->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickMaxSecond);
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickCancel);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupCalculator::OnClickClose);
}

void UGsUIPopupCalculator::NativeConstruct()
{
	Super::NativeConstruct();

	// 현재 이미 사용중인 부분들에서는 Dimmed 가 꺼져 있는 상태로 구현되어 있으므로, 초기화 시에는 항상 끄도록 설정함.
	SetDimmed(false);

	_num = 0;

	InvalidateNumText();

	_titleText->SetVisibility(ESlateVisibility::Hidden);
	_btnMax70->SetVisibility(ESlateVisibility::Hidden);
	_btnMax90->SetVisibility(ESlateVisibility::Hidden);

	_weightPenalty1Phase = GSInventory()->GetOwnedItemPenaltyValueByType(WeightPenaltyGrade::FIRST);
	_weightPenalty2Phase = GSInventory()->GetOwnedItemPenaltyValueByType(WeightPenaltyGrade::SECOND);
	
	FString maxFirstStr = FString::Printf(TEXT("%.2f"), (float)_weightPenalty1Phase - 0.01f);
	FString maxSecondStr = FString::Printf(TEXT("%.2f"), (float)_weightPenalty2Phase - 0.01f);

	_textBlockFirst->SetText(FText::FromString(maxFirstStr));
	_textBlockSecond->SetText(FText::FromString(maxSecondStr));
}

void UGsUIPopupCalculator::NativeDestruct()
{
	if (OnNativeDestructDelegate.IsBound())
	{
		OnNativeDestructDelegate.Broadcast();
		OnNativeDestructDelegate.Clear();
	}
	if (OnFinishCalculateDelegate.IsBound())
	{
		OnFinishCalculateDelegate.Clear();
	}
	if (OnCancelCalculateDelegate.IsBound())
	{
		OnCancelCalculateDelegate.Clear();
	}
	if (OnResetCalculateDelegate.IsBound())
	{
		OnResetCalculateDelegate.Clear();
	}

	_buttonCallback = nullptr;	

	Super::NativeDestruct();
}

void UGsUIPopupCalculator::InvalidateNumText()
{
	_numString = FString::FromInt(_num);
	_numText->SetText(FText::FromString(_numString));
}

void UGsUIPopupCalculator::SetMaximumCount(int64 InMaximumCnt)
{
	_maximumCount = InMaximumCnt;
}

void UGsUIPopupCalculator::SetDefaultCount(int64 InDefaultCnt)
{
	_num = InDefaultCnt;
}

void UGsUIPopupCalculator::SetDimmed(const bool InDimmed)
{
	_dimmed->SetVisibility(InDimmed ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUIPopupCalculator::SetMinimumCount(int64 InMinimumCnt)
{
	_minimumCount = InMinimumCnt;
}

// true : buyMode , false : sellMode
void UGsUIPopupCalculator::SetNpcShopCalcInfo(bool IsBuyMode)
{
	_isAddMode = IsBuyMode;
	_titleText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FText findText;
	FTextKey textKey = true == IsBuyMode ? TEXT("BuyMode_NeedTextID") : TEXT("SellMode_NeedTextID");
	FText::FindText(TEXT("NpcShopText"), textKey, findText);
	_titleText->SetText(findText);
}

void UGsUIPopupCalculator::SetTitleText(FText InText)
{
	_titleText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_titleText->SetText(InText);
}

void UGsUIPopupCalculator::SetResetCount()
{
	OnClickClear();
}

void UGsUIPopupCalculator::OnClickMinus()
{
	if (false == IsMinimum())
	{
		_num -= 1;
		_touched = true;
	}
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickPlus()
{
	if (false == IsMaximum())
	{
		_num += 1;
		_touched = true;
	}
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickNumPad1()
{
	AppendNum(1);
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickNumPad2()
{
	AppendNum(2);
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickNumPad3()
{
	AppendNum(3);
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickNumPad4()
{
	AppendNum(4);
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickNumPad5()
{
	AppendNum(5);
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickNumPad6()
{
	AppendNum(6);
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickNumPad7()
{
	AppendNum(7);
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickNumPad8()
{
	AppendNum(8);
	InvalidateNumText();
}


void UGsUIPopupCalculator::OnClickNumPad9()
{
	AppendNum(9);
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickNumPad0()
{
	AppendNum(0);
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickClear()
{
	_num = _minimumCount;
	_touched = false;
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickRemove()
{
	RemoveEnd();
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickAdd10()
{
	AddedNum(10);
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickAdd100()
{
	AddedNum(100);
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickAdd1000()
{
	AddedNum(1000);
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickAddMax()
{
	_num = _maximumCount;
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickOk()
{
	if (true == OnFinishCalculateDelegate.IsBound())
	{
		OnFinishCalculateDelegate.Broadcast(_num);
	}

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(true);
	}

	Close();
}

void UGsUIPopupCalculator::OnClickCancel()
{
	OnClickClose();
}

void UGsUIPopupCalculator::OnClickClose()
{
	if (true == OnCancelCalculateDelegate.IsBound())
	{
		OnCancelCalculateDelegate.Broadcast();
	}

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}

	Close();
}

void UGsUIPopupCalculator::OnClickMaxFirst()
{
	_touched = true;
	_num = GetMaximumCount(_weightPenalty1Phase, FTextKey(TEXT("Notice_Aleardy70Over")), FTextKey(TEXT("Notice_Aleardy70Under")));
	InvalidateNumText();
}

void UGsUIPopupCalculator::OnClickMaxSecond()
{
	_touched = true;
	_num = GetMaximumCount(_weightPenalty2Phase, FTextKey(TEXT("Notice_Aleardy90Over")), FTextKey(TEXT("Notice_Aleardy90Under")));
	InvalidateNumText();
}

bool UGsUIPopupCalculator::IsMaximum()
{
	if (_maximumCount <= _num)
	{
		_num = _maximumCount;
		return true;
	}

	return false;
}

bool UGsUIPopupCalculator::IsMinimum()
{
	if (_minimumCount >= _num)
	{
		_num = _minimumCount;
		return true;
	}

	return false;
}

void UGsUIPopupCalculator::AppendNum(uint32 InAppendNum)
{
	if (false == _touched)
	{
		_num = 0;
		_numString = FString::FromInt(_num);
		_touched = true;
	}

	if (true == IsMaximum())
	{
		return;
	}

	_numString.AppendInt(InAppendNum);
	_num = FCString::Atoi(*_numString);
	CheckMinMaxValue();
}

void UGsUIPopupCalculator::RemoveEnd()
{
	_touched = true;

	if (true == IsMinimum())
	{
		_touched = false;
		return;
	}

	if (1 >= _numString.Len())
	{
		_touched = false;
		_num = _minimumCount;
		return;
	}
	
	_numString.RemoveAt(_numString.Len() - 1);
	_num = FCString::Atoi(*_numString);

	if (CheckMinMaxValue())
	{
		_touched = false;
	}
}

void UGsUIPopupCalculator::AddedNum(int64 InAddedNum)
{
	_touched = true;

	if (true == IsMaximum())
	{
		return;
	}

	_num += InAddedNum;
	CheckMinMaxValue();
}

bool UGsUIPopupCalculator::CheckMinMaxValue()
{
	return (true == IsMinimum()) || (true == IsMaximum());
}

int64 UGsUIPopupCalculator::GetMaximumCount(int32 InPenaltyValue, const FTextKey& InOverKey, const FTextKey& InUnderKey)
{
	if (_itemWeight <= 0)
	{
		return _maximumCount;
	}

	int32 targetWeight = _maxWeight * InPenaltyValue / 100;

	if (_isAddMode && _currentWeight >= targetWeight)
	{
		FText findText;
		FText::FindText(TEXT("DepotText"), InOverKey, findText);
		findText = FText::Format(findText, InPenaltyValue);
		FGsUIHelper::TrayMessageTicker(findText);
		return 0;
	}

	if (!_isAddMode && _currentWeight < targetWeight)
	{
		FText findText;
		FText::FindText(TEXT("DepotText"), InUnderKey, findText);
		findText = FText::Format(findText, InPenaltyValue);
		FGsUIHelper::TrayMessageTicker(findText);
		return 0;
	}

	int32 diffWeight = _isAddMode ? (targetWeight - _currentWeight) : (_currentWeight - targetWeight);
	int64 divCount = diffWeight / _itemWeight;

	int64 outMaxCount = 0;
	if (_isAddMode)
	{
		outMaxCount = (diffWeight % _itemWeight == 0) ? divCount - 1 : divCount;
	}
	else
	{
		outMaxCount = divCount + 1;
	}

	if (_maximumCount < outMaxCount)
	{
		outMaxCount = _maximumCount;
	}

	if (outMaxCount == 0)
	{
		FText findText;
		FText::FindText(TEXT("DepotText"), TEXT("Notice_AlreadyThatValue"), findText);
		findText = FText::Format(findText, InPenaltyValue);
		FGsUIHelper::TrayMessageTicker(findText);
	}
	return outMaxCount;
}

void UGsUIPopupCalculator::SetTouchState(bool InState)
{
	_touched = InState;
}

void UGsUIPopupCalculator::SetWeight(int32 InItemWeight, int32 InMaxWeight, int32 InCurrentWeight)
{
	_itemWeight = InItemWeight;
	_maxWeight = InMaxWeight;
	_currentWeight = InCurrentWeight;

	_btnMax70->SetVisibility(ESlateVisibility::Visible);
	_btnMax90->SetVisibility(ESlateVisibility::Visible);
}

void UGsUIPopupCalculator::SetMode(bool InIsAddMode)
{
	_isAddMode = InIsAddMode;
}

void UGsUIPopupCalculator::SetButtonCallBack(TFunction<void(bool)> InCallback)
{
	_buttonCallback = InCallback;
}

void UGsUIPopupCalculator::OnInputOk()
{
	OnClickOk();
}

void UGsUIPopupCalculator::OnInputCancel()
{
	OnClickClose();
}