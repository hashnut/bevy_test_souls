// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/GsUIPopupCommonPayIncrease.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUIPopupCommonPayIncrease::OnClickYes()
{
	_isCallEventProcessFinish = false;

	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(true, true, _pastLevel + _curLevel);
	}
}

void UGsUIPopupCommonPayIncrease::OnClickNo()
{
	_isCallEventProcessFinish = true;

	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false, false, _pastLevel + _curLevel);
	}
}

void UGsUIPopupCommonPayIncrease::OnClickMinus()
{
	--_curLevel;
	RefreshUI();
}

void UGsUIPopupCommonPayIncrease::OnClickPlus()
{
	++_curLevel;
	RefreshUI();
}

void UGsUIPopupCommonPayIncrease::NativeDestruct()
{
	_buttonCallback = nullptr;

	if (nullptr != _destructCallback)
	{
		_destructCallback(_isCallEventProcessFinish);
		_destructCallback = nullptr;
	}

	Super::NativeDestruct();
}

void UGsUIPopupCommonPayIncrease::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupCommonPayIncrease::OnClickYes);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupCommonPayIncrease::OnClickNo);
	_btnPlus->OnClicked.AddDynamic(this, &UGsUIPopupCommonPayIncrease::OnClickPlus);
	_btnMin->OnClicked.AddDynamic(this, &UGsUIPopupCommonPayIncrease::OnClickMinus);
}

void UGsUIPopupCommonPayIncrease::OnInputCancel()
{
	OnClickNo();
}