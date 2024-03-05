#pragma once

#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIScanDragObject.generated.h"

class UGameViewportClient;
class APlayerController;
class UWidget;

UCLASS(BlueprintType)
class UGsUIScanDragObject : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _dragWidget;
private:
	bool _isTouchStart = false;
	bool _isDragOn = false;
	FVector2D _currentTouchPos;
	FVector2D _dragStartPos;

	float _squaredDragDiffVal = 0.0f;
public:
	TFunction<void(bool)> CallbackScanStart;
	TFunction<void()> CallbackTouchStart;
	TFunction<void()> CallbackTouchEnd;

	// virtual func
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual void NativeConstruct() override;

	// logic func
public:
	void SetDragWidgetPos(FVector2D In_localPos);
	// get, set
public:	
	void SetIsDragOn(bool In_isDragOn);

	
	FVector2D GetCurrentTouchPos() 
	{
		return _currentTouchPos;
	}

	void SetTestDragDiff(float In_val)
	{
		_squaredDragDiffVal = In_val;
	}
};