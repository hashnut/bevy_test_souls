// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITrayBlockUI.generated.h"

/**
 * 화면 블록 UI. 네트워크 지연시 주로 활용.
 *	- ZOrder 높음
 *  - 로딩 중 파괴되지 않음
 */
UCLASS()
class T1PROJECT_API UGsUITrayBlockUI : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UBorder* _borderBg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (MultiLine = true))
	FText _contentText;

protected:
	const float SHOW_DELAY_TIME = 1.f;
	float _showRemainTime = 0.f;
	bool _bHideUI = false;

protected:
	// UUserWidget override. 
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 터치 이벤트 막기 처리
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	// UGsUIWidgetBase override.
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_RECONNECT; }
	virtual bool IsNotDestroyedDuringLoading() const override { return true; }

public:
	void Show();
	void Hide();
	void SetTextMessage(const FText& InText);
	void SetHideUI(bool bInHideUI);

protected:
	void SetTextDefault();

	// 디버깅용 로그 출력
protected:
	UFUNCTION()
	FEventReply OnBorderMouseButtonUp(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
};
