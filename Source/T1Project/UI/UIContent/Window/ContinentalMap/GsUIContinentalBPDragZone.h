#pragma once

#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "SlateCore/Public/Layout/Geometry.h"

#include "GsUIContinentalBPDragZone.generated.h"

/*
* 대륙 bp 드래그 존(대륙 bp에서 드래그 하니까 최상단 캔버스 패널(해상도 크기)만 커버가 됨)
*/

class UCanvasPanel;

UCLASS()
class UGsUIContinentalBPDragZone : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnBPTouchEvent, const FVector2D&);

public:
	FOnBPTouchEvent OnBPTouchMoved;
	FOnBPTouchEvent OnBPTouchStarted;
	FOnBPTouchEvent OnBPTouchEnd;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _sizeCanvas;

protected:
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	// get
public:
	// 사이즈 캔버스 접근
	UCanvasPanel* GetSizeCanvas()
	{
		return _sizeCanvas;
	}
};