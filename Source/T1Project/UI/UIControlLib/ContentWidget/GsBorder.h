// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "GsBorder.generated.h"

class UGsUIWidgetBase;
/**
 * 버튼에 추가로 필요한 기능을 제공하기 위해 상속받은 클래스
 */
UCLASS()
class T1PROJECT_API UGsBorder : public UBorder
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnFinishedLongPressBorder);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsBorder")
	bool _useLongPress = false;

	// 롱프레스 GaugeBar ZOrder
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsBorder")
	int32 _longPressGaugeBarZOrder = 1000;
	
	// 롱프레스 중 움직이면 취소시키는데, press로부터의 DistSquared 값 체크
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsBorder")
	float _moveCheckValue = 0.1f;

protected:
	TWeakObjectPtr<UGsUIWidgetBase> _longPressObjPtr;
	TWeakObjectPtr<UWidget> _logPressBarTarget;
	FTimerHandle _timerHandle;	
	FVector2D _pressPos;
	bool _bIsPressed = false;
	bool _bIsLongPressed = false;

public:
	FOnFinishedLongPressBorder OnFinishedLongPress;

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	FReply HandleMouseButtonDownEx(const FGeometry& Geometry, const FPointerEvent& MouseEvent);
	FReply HandleMouseButtonUpEx(const FGeometry& Geometry, const FPointerEvent& MouseEvent);
	FReply HandleMouseMoveEx(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

public:
	// 롱프레스 바를 붙일 대상
	void SetLongpressBarTarget(UWidget* InTarget) { _logPressBarTarget = InTarget; }
	// 직전에 롱프레스 성공시에만 true. Press 되면 풀림
	// 롱프레스 성공 시 클릭막기 등의 로직에 활용
	bool IsLongPressed() const { return _bIsLongPressed; }
	void ResetLongPress();

protected:
	void OnEndTimer();
	void OnFinishedLongPressTrayMessage(bool InIsFullGauge);
};
