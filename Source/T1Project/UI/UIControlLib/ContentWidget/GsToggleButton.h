// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Styling/SlateColor.h"
#include "Styling/SlateTypes.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsToggleButton.generated.h"

class USlateWidgetStyleAsset;

/**
 * 토글 버튼. 
 * - 버튼 하위에 이미지, 텍스트가 있을 경우 지정된 컬러 적용.
 * - 선택시, 선택해제 시 함께 끄고 켤 위젯 등록기능.
 * - 주의: 클릭한다고 해서 _bIsSelected를 변경하지 않음. 외부에서 SetSelected 시켜줘야 함. 
 *        이렇게 구현된 이유는, Select 실패 등에 상황에 대응하기 위함임.
 *		  FOnToggleButtonSelected 이벤트도 클릭되면 무조건 호출되지 않고, SetSelected 시 호출.
 */
UCLASS()
class T1PROJECT_API UGsToggleButton : public UGsButton, public IGsToggleGroupEntry
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToggleButtonSelected, bool, bIsSelected);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIToggleButton")
	FLinearColor _checkedColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIToggleButton")
	FLinearColor _uncheckedColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIToggleButton")
	FButtonStyle _uncheckedWidgetStyle;

protected:
	bool _bIsFirstSetSelected = true;
	bool _bIsSelected = false;

public:
	UPROPERTY(BlueprintAssignable, Category = "GsUIControl")
	FOnToggleButtonSelected OnToggleButtonSelected;

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

	void SetIsSelectedNoEvent(bool bInIsSelected);

protected:
	void RecursiveSetColor(class UPanelWidget* InPanel, const FLinearColor& InColor);

	// SetStyle에서 WidgetStyle에 값을 덮어쓰는데, 이것을 막기 위해 따로 구현
	void SetStyleEx(const FButtonStyle& InStyle);

	virtual void SlateHandleReleasedEx() override;
};
