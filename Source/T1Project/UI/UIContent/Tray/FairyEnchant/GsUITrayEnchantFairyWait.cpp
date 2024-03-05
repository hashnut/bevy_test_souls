// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayEnchantFairyWait.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUITrayEnchantFairyWait::NativeConstruct()
{
	Super::NativeConstruct();
	
	_btnConfirm->OnClicked.AddDynamic(this, &UGsUITrayEnchantFairyWait::OnClickConfirm);
	_btnConfirm->SetIsEnabled(false);

}

void UGsUITrayEnchantFairyWait::NativeDestruct()
{
	_btnConfirm->OnClicked.RemoveDynamic(this, &UGsUITrayEnchantFairyWait::OnClickConfirm);
	_lockMaxTime = 0.f;

	Super::NativeDestruct();
}

void UGsUITrayEnchantFairyWait::OnClickConfirm()
{
	_isFinished = true;
}

void UGsUITrayEnchantFairyWait::InitializeData(EffectInitData* initParam)
{
	if (EnchantWaitEffectInitData* waitInitData = static_cast<EnchantWaitEffectInitData *>(initParam))
	{
		_lockMaxTime = waitInitData->_lockTime;
		_useButton = waitInitData->_useButton;

		if(_useButton && _lockMaxTime == 0.f)
			 _btnConfirm->SetIsEnabled(_useButton);
		else _btnConfirm->SetIsEnabled(false);
	}
	Super::InitializeData(initParam);
}

void UGsUITrayEnchantFairyWait::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (_lockMaxTime > 0.f)
	{
		_lockMaxTime -= InDeltaTime;

		if (_lockMaxTime <= 0.f)
		{
			_lockMaxTime = 0.f;
			if (_useButton)
				_btnConfirm->SetIsEnabled(true);
			else OnClickConfirm();
		}
	}
	Super::NativeTick(MyGeometry, InDeltaTime);
}