// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Components/CheckBox.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsCheckBox.generated.h"

/**
 * UGsCheckBox
 * : UCheckBox를 상속받아 기능을 확장함.
 * - 그룹관리 기능.
 * - On/Off 시 함께 껐다 켤 위젯 등록.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCheckBoxComponentStateChangedEx, int32, IndexInGroup, bool, bIsChecked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckBoxToggleGroupSelected, bool, bIsChecked );

UCLASS()
class T1PROJECT_API UGsCheckBox : public UCheckBox, public IGsToggleGroupEntry
{
	GENERATED_BODY()

protected:
	TArray<TWeakObjectPtr<UWidget>> _onVisibleList; // on시 ESlateVisibility::Visible, off시 Hidden
	TArray<TWeakObjectPtr<UWidget>> _onVisibleNoHitList; // on시 SelfHitTestInvisible, off시 Hidden
	TArray<TWeakObjectPtr<UWidget>> _offVisibleList; // on시 Hidden, off시 Visible
	TArray<TWeakObjectPtr<UWidget>> _offVisibleNoHitList; // on시 Hidden, off시 SelfHitTestInvisible

public:
	// IndexGroup
	UPROPERTY(BlueprintAssignable, Category = "GsUIControl")
	FOnCheckBoxComponentStateChangedEx OnCheckStateChangedEx;

	UPROPERTY(BlueprintAssignable, Category = "GsUIControl")
	FOnCheckBoxToggleGroupSelected OnToggleGroupSelected;

public:
	virtual void BeginDestroy() override;

protected:
	// UCheckBox override
	virtual TSharedRef<SWidget> RebuildWidget() override;

public:
	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

public:
	// SlateOnCheckStateChangedCallback이 상속이 안되어 대신받아 개조한 것
	void SlateOnCheckStateChangedCallbackEx(ECheckBoxState NewState);

	// on시 ESlateVisibility::Visible, off시 Hidden
	UFUNCTION(BlueprintCallable, Category = "GsUIControl")
	void AddWidgetToggleOnVisible(UWidget* InWidget);

	// on시 SelfHitTestInvisible, off시 Hidden
	UFUNCTION(BlueprintCallable, Category = "GsUIControl")
	void AddWidgetToggleOnVisibleNoHit(UWidget* InWidget);
	
	// on시 Hidden, off시 Visible
	UFUNCTION(BlueprintCallable, Category = "GsUIControl")
	void AddWidgetToggleOffVisible(UWidget* InWidget);

	// on시 Hidden, off시 SelfHitTestInvisible
	UFUNCTION(BlueprintCallable, Category = "GsUIControl")
	void AddWidgetToggleOffVisibleNoHit(UWidget* InWidget);

	UFUNCTION(BlueprintCallable, Category = "GsUIControl")
	int32 GetIndexInToggleGroup() const { return GetIndexInGroup(); }

private:
	void SetVisibilityTargetList(TArray<TWeakObjectPtr<UWidget>>& InList, ESlateVisibility InVisibility);
};
