// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITooltipDesc.h"
#include "Components/TextBlock.h"
#include "Slate/Public/Widgets/Text/STextBlock.h"


void UGsUITooltipDesc::NativeConstruct()
{
	Super::NativeConstruct();

	//_lifeTime = 0.f;
	_elapsedTime = 0.f;
	_needRefresh = 0;

	SetDefaultColor();
}

void UGsUITooltipDesc::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (0 < _lifeTime && true == IsOpen())
	{
		_elapsedTime += InDeltaTime;

		if (_elapsedTime >= _lifeTime)
		{
			Close();
		}
	}

	if (_needRefresh > 1)
	{
		--_needRefresh;
	}
	else if (_needRefresh == 1)
	{
		OnInvalidatePosition.ExecuteIfBound();
		_needRefresh = 0;
	}
}

void UGsUITooltipDesc::Open()
{
	Super::Open();

	_elapsedTime = 0.f;
}

void UGsUITooltipDesc::Close()
{
	_elapsedTime = 0.f;

	Super::Close();
}

void UGsUITooltipDesc::SetDescText(const FText& InText)
{
	_textBlockDesc->SetText(InText);
}

void UGsUITooltipDesc::SetDescColor(const FLinearColor& InColor)
{
	_textBlockDesc->SetColorAndOpacity(InColor);
}

void UGsUITooltipDesc::SetDefaultColor()
{
	_textBlockDesc->SetColorAndOpacity(_colorDefaultDesc);
}

void UGsUITooltipDesc::ToggleOpenClose()
{
	if (IsOpen())
	{
		Close();
	}
	else
	{
		Open();
	}
}

void UGsUITooltipDesc::SetLifeTime(float InLifeTime /*= 0.f*/)
{
	_lifeTime = InLifeTime;
}

void UGsUITooltipDesc::SetWrapTextOption(float InWrapTextAt)
{
	TSharedPtr<SWidget> swidget = _textBlockDesc->GetCachedWidget();
	if (swidget.IsValid())
	{
		STextBlock* stextBlock = static_cast<STextBlock*>(swidget.Get());
		stextBlock->SetWrapTextAt(InWrapTextAt);		
	}

	_textBlockDesc->SetAutoWrapText(true);
}

void UGsUITooltipDesc::SetIsNeedRefresh(bool InIsNeedRefresh)
{
	const int32 NEED_TICK = 2;
	_needRefresh = NEED_TICK;
}

FVector2D UGsUITooltipDesc::GetTextBlockSize()
{
	return _textBlockDesc->GetCachedGeometry().GetLocalSize();
}