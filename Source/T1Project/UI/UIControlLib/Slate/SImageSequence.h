// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

class UTexture2D;
class FDeferredCleanupSlateBrush;
 
/*
 * 로딩 중 이미지 애니메이션을 하기 위한 UI
 * - AsyncLoaidngScreen 플러그인 코드 참고하여 구현(https://github.com/truong-bui/AsyncLoadingScreen)
 * - OnPaint에서 일정 간격으로 이미지를 번갈아 가며 출력
 */
class SImageSequence : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SImageSequence) {}

	SLATE_END_ARGS()

	/**
	 * Construct this widget
	 *
	 * @param	InDeclaration	The declaration data for this widget
	 */
	void Construct(const SImageSequence::FArguments& InDeclaration, TArray<UTexture2D*>& InImageList, float InInterval);

private:
	TSharedPtr<SImage> Image;

protected:
	TArray<TSharedPtr<FDeferredCleanupSlateBrush>> _cleanupBrushList;

	// Current image sequence index
	mutable int32 _imageIndex = 0;
	// Current total delta time
	mutable float _totalDeltaTime = 0.f;

	//Time in second to update the images, the smaller value the faster of the animation. A zero value will update the images every frame.
	float _interval = 0.05f;

public:
	// SWidget overrides
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

public:
	void SetBrushList(TArray<UTexture2D*>& InImageList);
};
