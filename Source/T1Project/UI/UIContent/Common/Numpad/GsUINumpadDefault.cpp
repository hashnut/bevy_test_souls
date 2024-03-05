// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUINumpadDefault.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUINumpadDefault::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnPlus->OnClicked.AddDynamic(this, &UGsUINumpadDefault::OnClickPlus);
	_btnMinus->OnClicked.AddDynamic(this, &UGsUINumpadDefault::OnClickMinus);
	_btnBack->OnClicked.AddDynamic(this, &UGsUINumpadDefault::OnClickBack);
	_btnMax->OnClicked.AddDynamic(this, &UGsUINumpadDefault::OnClickMax);
}

void UGsUINumpadDefault::SetMaxNumber(int32 InMaxNumber)
{
	_maxNumber = InMaxNumber;
}

void UGsUINumpadDefault::SetNumber(int32 InNumber, bool InIsFirst)
{
	_bFirstFocused = InIsFirst;
	SetNumberInter(InNumber);
}

void UGsUINumpadDefault::SetNumberInter(int32 InNumber)
{
	_currNumber = FMath::Clamp(InNumber, 0, _maxNumber);
	_textBlockNumber->SetText(FText::AsNumber(_currNumber));

	_onChangeNumber.ExecuteIfBound(_currNumber);
}

void UGsUINumpadDefault::OnClickNumber(int32 InNumber)
{
	if (_bFirstFocused)
	{
		// 기존에 있던 값을 덮어씀
		_bFirstFocused = false;
		SetNumberInter(InNumber);
	}
	else
	{
		// 기존값을 한자릿수 올리고 뒤에 더함
		SetNumberInter(_currNumber * 10 + InNumber);
	}
}

void UGsUINumpadDefault::OnClickNumberAdd(int32 InNumber)
{
	SetNumberInter(_currNumber + InNumber);
}

void UGsUINumpadDefault::OnClickClear()
{
	_bFirstFocused = true;
	SetNumberInter(0);
}

void UGsUINumpadDefault::OnClickPlus()
{
	SetNumberInter(_currNumber + 1);
}

void UGsUINumpadDefault::OnClickMinus()
{
	SetNumberInter(_currNumber - 1);
}

void UGsUINumpadDefault::OnClickBack()
{
	// 뒤에 한자릿수 제거	
	
	// 이렇게 계산하면 99999999 -> 10000000 가 되어서 사용하지 않음
	//int32 num2 = FMath::FloorToInt(_currNumber * 0.1f);

	double num = FMath::FloorToDouble(_currNumber * 0.1);
	SetNumberInter(static_cast<int32>(num));
}

void UGsUINumpadDefault::OnClickMax()
{
	SetNumberInter(_maxNumber);
}
