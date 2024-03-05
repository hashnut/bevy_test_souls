// Fill out your copyright notice in the Description page of Project Settings.

#include "GsRedDotChecker.h"
#include "Components/Image.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"


void FGsRedDotChecker::Initialize(UImage* InImgRedDot, TFunction<bool()> InRedDotFunc)
{
	_imgRedDot = InImgRedDot;
	_uiRedDot = nullptr;
	_redDotType = EGsIconRedDotType::NORMAL;
	_redDotCheckFunc = InRedDotFunc;

	SetIsRedDot(false);
}

void FGsRedDotChecker::Initialize(UGsUIRedDotBase* InRedDot, EGsIconRedDotType InType, TFunction<bool()> InRedDotFunc)
{
	_imgRedDot = nullptr;
	_uiRedDot = InRedDot;
	_redDotType = InType;
	_redDotCheckFunc = InRedDotFunc;

	SetIsRedDot(false);
}

void FGsRedDotChecker::SetIsRedDot(bool bIsRedDot)
{
	if (_imgRedDot.IsValid())
	{
		_imgRedDot->SetVisibility(bIsRedDot ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (_uiRedDot.IsValid())
	{
		_uiRedDot->SetRedDot(bIsRedDot, _redDotType);
	}
}

bool FGsRedDotChecker::UpdateRedDot()
{
	bool bResult = false;
	if (_redDotCheckFunc)
	{
		bResult = _redDotCheckFunc();
	}

	SetIsRedDot(bResult);

	return bResult;
}
