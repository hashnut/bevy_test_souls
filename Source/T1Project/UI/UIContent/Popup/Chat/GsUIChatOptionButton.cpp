// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIChatOptionButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

void UGsUIChatOptionButton::SetIsSelected(bool bInIsSelected)
{
	_btnOption->SetIsSelected(bInIsSelected);
}

bool UGsUIChatOptionButton::GetIsSelected() const
{
	return _btnOption->GetIsSelected();
}

void UGsUIChatOptionButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIChatOptionButton::SetIndexInGroup(uint64 key)
{
	_btnOption->SetIndexInGroup(100);
	_keyId = key;
}

void UGsUIChatOptionButton::SetToggleCallback(TFunction<void(uint64, bool)> InToggleCallback)
{
	_toggleCallback = InToggleCallback;
	if (_toggleCallback)
	{
		TWeakObjectPtr<UGsUIChatOptionButton> thiz = this;

		_btnOption->SetToggleCallback([thiz](int32 key, bool select) {
			if (thiz.IsValid()) thiz->_toggleCallback(thiz->GetIndexInGroup(), select);
			});
	}
	else
	{
		_btnOption->SetToggleCallback(nullptr);
	}
}
