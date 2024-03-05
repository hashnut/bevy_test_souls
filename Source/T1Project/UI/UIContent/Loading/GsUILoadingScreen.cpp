// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUILoadingScreen.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataSchema/Loading/GsSchemaLoadingScreen.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Classes/Engine/Texture2D.h"
#include "Management/ScopeGlobal/GsUIManager.h"


void UGsUILoadingScreen::RemoveFromParent()
{
	if (false == _bNotDestroy)
	{
		Super::RemoveFromParent();
	}
}

void UGsUILoadingScreen::NativeConstruct()
{
	Super::NativeConstruct();

	SetNotDestroy(true);
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (_bIsForUI)
	{
		GUI()->AddUIScaleFlag(EGsUIScaleFlags::UI_SCALE_LOADING);
	}
}

void UGsUILoadingScreen::NativeDestruct()
{
	if (_bIsForUI)
	{
		GUI()->RemoveUIScaleFlag(EGsUIScaleFlags::UI_SCALE_LOADING);
	}

	Super::NativeDestruct();
}

void UGsUILoadingScreen::Remove()
{
	SetNotDestroy(false);
	RemoveFromParent();
}

void UGsUILoadingScreen::SetData(const FGsSchemaLoadingScreen* InTable)
{
	if (UTexture2D* texture = FGsUIHelper::LoadTextureFromPath(InTable->imagePath))
	{
		_imgBG->SetColorAndOpacity(FLinearColor::White);
		_imgBG->SetBrushFromTexture(texture);
	}
	else
	{
		_imgBG->SetColorAndOpacity(FLinearColor::Black);
	}

	_textBlockTitle->SetText(InTable->titleText);
	_textBlockTitle->SetColorAndOpacity(FLinearColor(InTable->titleColor));

	_textBlockDesc->SetText(InTable->descText);
	_textBlockDesc->SetColorAndOpacity(FLinearColor(InTable->descColor));
}

void UGsUILoadingScreen::SetTextTooltip(const FText& InText, const FColor& InColor)
{
	_textBlockTooltip->SetText(InText);
	_textBlockTooltip->SetColorAndOpacity(FLinearColor(InColor));
}

void UGsUILoadingScreen::OnStartLoadingModule()
{
	// 로딩 모듈이 뜨면 보이지 않게 가림
	SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUILoadingScreen::OnEndLoadingModule()
{
	// 로딩 모듈이 끝나면 다시 보이게 켬
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
