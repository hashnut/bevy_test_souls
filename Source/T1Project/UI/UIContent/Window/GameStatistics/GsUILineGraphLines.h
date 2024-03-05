// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUILineGraphLines.generated.h"

struct FGsLineGraphData;

/**
 * 꺾은선 그래프의 라인 부분
 */
UCLASS()
class T1PROJECT_API UGsUILineGraphLines : public UUserWidget
{
	GENERATED_BODY()

protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUILineGraph")
	FLinearColor _colorLine;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUILineGraph")
	FLinearColor _colorGrid;

protected:
	const FGsLineGraphData* _data = nullptr;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
		int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

public:
	void SetData(const FGsLineGraphData* InData);
};
