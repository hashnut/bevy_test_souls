// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "GsUIReuseScroll.generated.h"


class UVerticalBox;
class UScrollBox;
class UHorizontalBox;
class UGsSimpleTimer;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;
/**
 * 무한 스크롤 아이콘 슬롯 부모 클래스
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIReuseScroll : public UUserWidget
{
	GENERATED_BODY()

public:
	enum ScrollDircetionType : uint8
	{
		Scroll_None = 0,
		Scroll_Return_Left,
		Scroll_Return_Right,
		Scroll_Left,
		Scroll_Right,
	};

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsIconSlot")
	EGsIconSlotType _iconSlotType = EGsIconSlotType::SImpleType;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsIconSlot")
	EGsIconSlotCountType _maxSlotPageType = EGsIconSlotCountType::One;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsIconSlot")
	EGsIconSlotCountType _maxSlotCountType = EGsIconSlotCountType::One;

	// auto로 이동되는 영역 값
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsIconSlot")
	float _autoScrollValue = 0.f;

	// 슬롯 아이콘 리스트 동적 생성 및 관리
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TArray<TSubclassOf<UUserWidget>> _entryWidgetClassList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	
	// 0 : on , 1 : off 
protected:
	UPROPERTY()
	TArray<UWidgetSwitcher*> _switcherPanelList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _firstPageSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _secondPageSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _thirdPageSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _fourthPageSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _fifthPageSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _sixthPageSwitcher;

public:
	UPROPERTY(EditDefaultsOnly, Category = "CurveData")
	UCurveFloat* _curve;
	UPROPERTY()
	UGsSimpleTimer* _timer;

private:
	bool _isScrolling = false;
	bool _isAutoScrolling = false;
	bool _isWindosInputAutoScroll = false;
	float _endScrollOffset = 0.f;
	
	TMap<EGsIconSlotCountType, float> _pageIndexPosMap;

protected:
	EGsIconSlotCountType _curPageSwitcherType = EGsIconSlotCountType::One;
	EGsIconSlotCountType _reserveTargetPageType = EGsIconSlotCountType::One;
	EGsIconSlotCountType _eSlotPageIndexType = EGsIconSlotCountType::One;
	ScrollDircetionType _eScrollDirectionType = Scroll_None;

protected:
	virtual void InvalidateSwitcher();

protected:
	EGsIconSlotCountType GetCurrentPageType() { return _curPageSwitcherType; }
	float GetSlotPos(EGsIconSlotCountType InCurSlotPageType);

private:	
	void CreateEachPagePosition();
	void CheckTargetSlot();
	float MoveTargetPagePos(bool InNextPage);
	float GetSlotCenterPos(EGsIconSlotCountType InCurSlotPageType);	
	
	EGsIconSlotCountType GetSwitcherPageType(bool InNextType);
	EGsIconSlotCountType GetPageType(bool InNextType);
	EGsIconSlotCountType GetPageType(EGsIconSlotCountType InType , bool InNextType);

protected:
	void WindowskeyInputLeft();
	void WindowskeyInputRight();

private:
	void AutoScroll(float InCurveData);
	void MoveNextScroll(float InCurveData);
	void MoveBeforeScroll(float InCurveData);
	void MoveRetrunLeftScroll(float InCurveData);
	void MoveRetrunRightScroll(float InCurveData);

private:
	void OnFinishedTargetPos();
	void CheckPageCondition();

protected:
	virtual void SetSlotMove(int32 InMoveTargetPageIndex, int32 InDestPageIndex) {}

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;	
	virtual void NativeDestruct() override;

public:
	virtual void UpdateManagerTick(float InDeltaTime);

public:
	UFUNCTION()
	virtual void OnChangeMainScrollEvent(float InCurOffset);
};

