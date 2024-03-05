// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUILineGraphLines.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameStatistics/GsLineGraphData.h"


void UGsUILineGraphLines::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bHasScriptImplementedPaint = true;
}

void UGsUILineGraphLines::NativeDestruct()
{
	_data = nullptr;

	Super::NativeDestruct();
}

int32 UGsUILineGraphLines::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
	int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// 주의: UserWidget의 구현부를 가져와 수정했으므로, Super::NativePaint를 부르지 않는다
	if (bHasScriptImplementedPaint)
	{
		FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
		OnPaint(Context);

		if (_data)
		{
			UWidgetBlueprintLibrary::DrawLines(Context, _data->_gridList, _colorGrid);
			UWidgetBlueprintLibrary::DrawLines(Context, _data->_pointList, _colorLine);
		}

		return FMath::Max(LayerId, Context.MaxLayer);
	}

	return LayerId;
}

void UGsUILineGraphLines::SetData(const FGsLineGraphData* InData)
{
	_data = InData;
}
