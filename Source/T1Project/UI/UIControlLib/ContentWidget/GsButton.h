// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "GsButton.generated.h"

UENUM(BlueprintType)
enum class EGsButtonSoundType : uint8
{
	DEFAULT = 0 UMETA(DisplayName = "기본음"),
	NONE UMETA(DisplayName = "없음")
};

class UGsUIWidgetBase;
/**
 * 버튼에 추가로 필요한 기능을 제공하기 위해 상속받은 클래스
 */
UCLASS()
class T1PROJECT_API UGsButton : public UButton
{
	GENERATED_UCLASS_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnFinishedLongPressGauge);
	DECLARE_DELEGATE_TwoParams(FOnMouseButtonEventEx, const FGeometry&, const FPointerEvent&);

public:
	// 광클방지 로직을 위한 시간 값(초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIButton")
	float _clickLockSecond = 0.0f;

	// 클릭 시 보일 이펙트 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIButton")
	FSoftObjectPath _clickVFXPath;

	// 이펙트 ZOrder
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIButton")
	int32 _clickVFXZOrder = 1000;

	// Disable 시 적용 할 알파값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIButton", meta=(UIMin = "0", UIMax = "1"))
	float _disableAlpha = 1.f;

	// Disable 시 적용 할 밝기 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIButton", meta=(UIMin = "0", UIMax = "1"))
	float _disableBrightness = 1.f;

	// 기본 클릭음 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIButton")
	EGsButtonSoundType _pressSoundType = EGsButtonSoundType::DEFAULT;

	TWeakObjectPtr<class USoundBase> _pressSound;

public:
	// 롱프레스 사용 유무 체크
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIButton")
	bool _useLongPress = false;
	// 외부 롱프레스 사용할지
	// 월드맵개선: 몬스터 리스트 버튼 사이즈가 일반 아이콘보다 커서 어색하여 외부 프로그바 사용
	// https://jira.com2us.com/jira/browse/CHR-23756
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIButton")
	bool _useLongPressOtherProgressBar = false;

	// 롱프레스 GaugeBar ZOrder
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIButton")
	int32 _longPressGaugeBarZOrder = 1000;
	// 외부 롱프레스 시작
	TFunction<void(TFunction<void(bool)>)> _callbackOtherProgressBarStart;

public:
	FOnFinishedLongPressGauge _onFinishedLongPressGauge;

	FOnMouseButtonEventEx OnMouseButtonEventDownEx;
	FOnMouseButtonEventEx OnMouseButtonEventUpEx;
	FOnMouseButtonEventEx OnMouseButtonEventMoveEx;

protected:
	float _oriClickLockSecond = 0.f;
	double _lockEndTime = 0;
	double _lastClickTime = 0;
	FLinearColor _backupColor;
	FLinearColor _backupBgColor;
	bool _bIsFirst = true;

private:
	TWeakObjectPtr<UGsUIWidgetBase> _longPressObjPtr;
	FTimerHandle _timerHandle;
	bool _longPressState = false;
	bool _isPressedBtn = false;


protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

public:
	virtual void SynchronizeProperties() override;
	virtual void SetIsEnabled(bool bInIsEnabled) override;
	void SetIsEnabledGraphics(bool bInIsEnabled);

protected:
	virtual void SlateHandlePressedEx();
	virtual FReply SlateHandleClickedEx();
	virtual void SlateHandleReleasedEx();
	virtual void SlateHandleHoveredEx();
	virtual void SlateHandleUnhoveredEx();

protected:
	void OnMouseButtonDownEx(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	void OnMouseButtonUpEx(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	void OnMouseButtonMoveEx(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
		
	void OnMouseFocusLeave();

private:
	void LongPressedReleased();

public:
	void OnChangeLongPress(bool InState);
	void SetOnClickLockSec(float InSce) { _clickLockSecond = InSce; };

protected:
	void OnEndTimer();
	void OnFinishedLongPressTrayMessage(bool InIsFullGauge);

protected:
	bool IsTimeLocked();

public:
	void ClearLock() { _lockEndTime = 0; }
};
