// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayRepairWait.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUITrayRepairWait::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnConfirm)
	{
		_btnConfirm->OnClicked.AddDynamic(this, &UGsUITrayRepairWait::OnClickConfirm);
	}
}

void UGsUITrayRepairWait::NativeDestruct()
{
	_confirmCallback = nullptr;
	_currentTime = 0.f;
	_lockMaxTime = 0.f;

	Super::NativeDestruct();
}

void UGsUITrayRepairWait::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	_currentTime += InDeltaTime;
}

bool UGsUITrayRepairWait::IsPossibleOnClick() const
{
	return (_currentTime >= _lockMaxTime);
}

void UGsUITrayRepairWait::SetData(float InBtnLockTime, TFunction<void()> InConfirmCallback)
{
	_lockMaxTime = InBtnLockTime;
	_confirmCallback = InConfirmCallback;
}

void UGsUITrayRepairWait::OnClickConfirm()
{
	if (false == IsPossibleOnClick())
		return;

	if (nullptr != _confirmCallback)
	{
		// 중복 재클릭이 안되도록 임시로 막음
		_currentTime = 0.f;
		_lockMaxTime = 10.0f;

		_confirmCallback();
	}
	else
	{
		Close();
	}
}
