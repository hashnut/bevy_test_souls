// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "DataCenter/Public/Tutorial/GsTutorialEnumDefine.h"
#include "GsUITrayTutorial.generated.h"

class UWidget;
class UImage;
class UGsButton;
class USizeBox;
class UWidgetSwitcher;
class UPanelWidget;
class UGsUITutorialSubDialog;
struct FGsSchemaSubDialogCutData;
struct FGsSchemaTutorialActionUIInteraction;
class UGsUIIconItem;


/**
 * 튜토리얼에서 사용되는 UI
 */
UCLASS()
class T1PROJECT_API UGsUITrayTutorial : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsTutorial")
	FSoftObjectPath _clickVFXPath;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsTutorial")
	float _skipButtonActiveSecond = 5.f;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgTop;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgCenter;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	USizeBox* _sizeBoxTarget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _uiRectEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherArrow;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITutorialSubDialog* _dialogLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITutorialSubDialog* _dialogRight;

	// 광클 방지를 위해 EndInteraction되자마자 켜줌
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSkip;

protected:
	int32 _actionId = 0;
	EGsTutorialUIInteractionType _interactionType = EGsTutorialUIInteractionType::NONE;
	bool _bIsVisibleAfterInteraction = true;
	
	bool _bUseTarget = false;
	bool _bCheckClick = false;
	float _backupBtnLockSec = 0.f;

	// 대상 타게팅 딜레이 체크용
	bool _bIsTargetFound = false;
	float _targetingDelaySec = 0.f;
	float _targetingDelayRemainSec = 0.f;

	// 스킵 버튼 시간체크
	float _skipBtnActiveRemainSec = 0.f;

	// 갱신 체크용
	FVector2D _prevSizeBoxSize;
	FVector2D _prevSizeBoxPos;
	FVector2D _resetSize;

	TWeakObjectPtr<class UWidget> _targetWidget;
	TWeakObjectPtr<class UWidget> _visualWidget;
	
	FDelegateHandle _handleResize;
	FDelegateHandle _handleLongPressItemIcon;

public:
	static UWidget* RecursiveFindWidget(UUserWidget* InUserWidget, const FName& InRootName, const FName& InName);
	static UWidget* RecursiveFindWidget(UWidget* InWidget, const FName& InName);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_TRAY_CINEMATIC; }
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_TUTORIAL; }

public:
	// 타겟이 없을경우
	bool SetNoTargetData(int32 InActionId);
	// 타겟이 있을 경우
	bool SetTargetData(int32 InActionId, const FGsSchemaTutorialActionUIInteraction* InData, 
		UWidget* InTargetWidget = nullptr, UWidget* InVisualWidget = nullptr);	

	void SetSubDialog(const FGsSchemaSubDialogCutData* InData, EGsTutorialUIDialogDirection InDirection);

protected:	
	void SetArrow(EGsTutorialUIArrowDirection InDirection);
	void SetEffect(EGsTutorialUIEffectType InType);
	void SetVisibleTargetEffect(bool bVisible);
	void SetActiveSkipButton(bool bIsActive);

	void SetInteraction(EGsTutorialUIInteractionType InType, UWidget* InTargetWidget);
	void EndInteraction();
	void UnbindInteractionEvent();

	void SetCheckSize();
	void CalcSize();
	void ResetSize();

protected:
	UFUNCTION()
	void OnClickSkip();

	// 타게팅된 버튼에 바인딩하여 불림
	UFUNCTION()
	void OnClickTarget();	

	void OnClickRegion();

	void OnInteractionInterface();

	void OnLongPressItemIcon(class UGsUIIconItem& InIcon);
	void OnResizeViewport(class FViewport* InViewport, uint32 InNum);
};
