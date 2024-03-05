// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayEnchantFairyProgress.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyItem.h"


void UGsUITrayEnchantFairyProgress::NativeConstruct()
{
	SetVisibility(ESlateVisibility::Visible);

	Super::NativeConstruct();

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUITrayEnchantFairyProgress::OnClickBlock);

	if (false == _fXEnchant->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_fXEnchant->OnVFXAnimationFinished.BindUObject(this, &UGsUITrayEnchantFairyProgress::FinishedAnim);
	}
	_lockMaxTime = 0.f;
}

void UGsUITrayEnchantFairyProgress::NativeDestruct()
{
	_btnConfirm->OnClicked.RemoveDynamic(this, &UGsUITrayEnchantFairyProgress::OnClickBlock);
	SetVisibility(ESlateVisibility::Collapsed);

	Super::NativeDestruct();
}

void UGsUITrayEnchantFairyProgress::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_lockMaxTime > 0.f)
	{
		_lockMaxTime -= InDeltaTime;

		if (_lockMaxTime <= 0.f)
		{
			_lockMaxTime = 0.f;
			_isFinished = true;
		}
	}
}

void UGsUITrayEnchantFairyProgress::InitializeData(EffectInitData* initParam)
{
	Super::InitializeData(initParam);

	_isUseTimeout = false;
	if (EnchantProgressEffectInitData* waitInitData = static_cast<EnchantProgressEffectInitData*>(initParam))
	{
		_lockMaxTime = waitInitData->_lockTime;
		_isUseTimeout = true;
	}
}

void UGsUITrayEnchantFairyProgress::FinishedAnim()
{
	if(!_isUseTimeout)
		_lockMaxTime = 0.3f;
	//SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUITrayEnchantFairyProgress::OnClickBlock()
{
}