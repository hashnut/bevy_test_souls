// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsSwitcherButton.generated.h"

class UWidgetSwitcher;

/**
 * 버튼 바로 아래 자손으로 Switcher를 가지는 형태
 * - FGsToggleGroup과 연계 시 0: 꺼짐. 1: 켜짐
  * - 주의: 클릭한다고 해서 _bIsSelected를 변경하지 않음. 외부에서 SetSelected 시켜줘야 함.
 *        이렇게 구현된 이유는, Select 실패 등에 상황에 대응하기 위함임.
 *		  FOnToggleButtonSelected 이벤트도 클릭되면 무조건 호출되지 않고, SetSelected 시 호출.
 */
UCLASS()
class T1PROJECT_API UGsSwitcherButton : public UGsButton, public IGsToggleGroupEntry
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitcherButtonSelected, bool, bIsSelected);

protected:
	bool _bIsFirstSetSelected = true;
	bool _bIsSelected = false;
	TWeakObjectPtr<class UWidgetSwitcher> _switcher;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIControl")
	int32 _slotIndexSelected = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIControl")
	int32 _slotIndexUnselected = 0;

public:
	UPROPERTY(BlueprintAssignable, Category = "GsUIControl")
	FOnSwitcherButtonSelected OnSwitcherButtonSelected;

public:
	virtual void BeginDestroy() override;

protected:
	// UButton override
	virtual TSharedRef<SWidget> RebuildWidget() override;

	// UGsButton override
	virtual FReply SlateHandleClickedEx() override;

public:
	// IGsToggleGroupEntry
	UFUNCTION(BlueprintCallable, Category = "GsUIControl")
	virtual void SetIsSelected(bool bInIsSelected) override;

	UFUNCTION(BlueprintCallable, Category = "GsUIControl")
	virtual bool GetIsSelected() const override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "GsUIControl")
	void SetSwitcherIndex(int32 InIndex);

	UFUNCTION(BlueprintCallable, Category = "GsUIControl")
	int32 GetSwitcherIndex() const;

	int32 GetNumWidgets();
protected:
	class UWidgetSwitcher* RecursiveFindSwitcher(class UPanelWidget* InPanel) const;

	virtual void SlateHandleReleasedEx() override;
};
