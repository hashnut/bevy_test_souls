// Copyright Epic Games, Inc. All Rights Reserved.

#include "SImageSequence.h"
#include "Engine/Texture2D.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SSpacer.h"


void SImageSequence::Construct(const SImageSequence::FArguments& InDeclaration, TArray<UTexture2D*>& InImageList, float InInterval)
{	
	_interval = InInterval;
	SetBrushList(InImageList);

	this->ChildSlot
		[
			SAssignNew(Image, SImage)
		];

	if (0 < _cleanupBrushList.Num())
	{
		Image->SetImage(_cleanupBrushList[0]->GetSlateBrush());
	}
}

int32 SImageSequence::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, 
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	_totalDeltaTime += Args.GetDeltaTime();

	if (_interval <= _totalDeltaTime)
	{
		if (0 < _cleanupBrushList.Num())
		{
			++_imageIndex;

			if (_cleanupBrushList.Num() <= _imageIndex ||
				false == _cleanupBrushList.IsValidIndex(_imageIndex))
			{
				_imageIndex = 0;
			}

			Image->SetImage(_cleanupBrushList[_imageIndex].IsValid() ?
				_cleanupBrushList[_imageIndex]->GetSlateBrush() : nullptr);
		}
		_totalDeltaTime = 0.f;
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void SImageSequence::SetBrushList(TArray<UTexture2D*>& InImageList)
{
	if (0 < InImageList.Num())
	{
		_cleanupBrushList.Empty();
		_imageIndex = 0;

		for (int i = 0; i < InImageList.Num(); ++i)
		{
			if (UTexture2D* image = InImageList[i])
			{
				_cleanupBrushList.Add(FDeferredCleanupSlateBrush::CreateBrush(image,
					FVector2D(image->GetSurfaceWidth(), image->GetSurfaceHeight())));
			}			
		}
	}
}
