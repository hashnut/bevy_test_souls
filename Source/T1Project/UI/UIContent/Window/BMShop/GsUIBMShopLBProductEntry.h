// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIBMShopLBProductEntry.generated.h"

class UGsButton;
class UTextBlock;
class UPanelWidget;
class UHorizontalBox;
class UScrollBox;
class UGsSimpleTimer;
class UGsUISwipeCircle;
class UGsDynamicPanelSlotHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBMShopLBProductEntry : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

private:
	enum AutoMoveDirType : uint8
	{
		MoveNoneType = 0,
		MoveLeftType,
		MoveRightType,
		MoveReturnLeftType,
		MoveReturnRightType,

		MoveMaxType,
	};


protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _bannerRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _bannerScrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _swipeCircleRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRight;

protected:
	// large banner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _largeBannerListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _largeBannerListPanelSlotHelper;

	// swipe circle
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _swipeCircleEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _swipeCirclePanelSlotHelper;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CurveData")
	UCurveFloat* _curve;
	UPROPERTY()
	UGsSimpleTimer* _timer;

protected:
	UPROPERTY(EditAnywhere, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	float _autoScrollDurationTime = 4.f;


private:
	int32 _curPage = 0;
	int32 _focusingSlotIndex = 0;
	float _widthSize = 0.f;
	float _autoScrollTimeCheck = 0.f;
	bool _isUseScrolling = false;
	bool _isScrolling = false;
	TArray<BMProductId> _bannerIDList;
	TMap<uint8, float> _pagePosMap;
	AutoMoveDirType _autoMoveDirType = AutoMoveDirType::MoveNoneType;


protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetProduct(TArray<BMProductId> InProductList, int32 InFocusingProductID = 0);
	void InvalidateProduct();
	void ResetChild();

private:
	void OnChangeSwipeCircle();
	void CheckCurPos();
	void AutoScroll(float InCurveValue);
	void SwapSlot();
	void MoveAutoScroll(float InCurveValue, float InGoalPos, AutoMoveDirType InMoveDirType, uint8 InTargetSlotIndex);
	void SetSlotMove(int32 InMoveTargetPageIndex, int32 InDestPageIndex);
	void FocusingProduct(const BMProductId InProductID);

protected:
	UFUNCTION()
	void BannerListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnChangeScrollEvent(float InCurOffset);
	UFUNCTION()
	void SwipeCircleListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnBtnLeft();
	UFUNCTION()
	void OnBtnRight();

};
