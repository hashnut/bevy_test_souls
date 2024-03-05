// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUICustomizeBase.generated.h"

struct FGsCustomizeUIData;
class FGsCustomizeHandler;

/**
 * 커스터마이징 메뉴 선택에 따라 바뀌는 메뉴들의 부모클래스
 */
UCLASS()
class T1PROJECT_API UGsUICustomizeBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	FGsCustomizeUIData* _selectedData = nullptr;
	FGsCustomizeHandler* _customizeHandler = nullptr;

protected:
	// 터치막기
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
public:
	virtual void Show() {}

public:
	// 0.0f ~ 1.0f 값을 0 ~ 100 값으로 전환
	int32 ConvertFloatToDataValue(float InValue);

	// 0 ~ 100 값을 0.0 ~ 1.0f 값으로 전환
	float ConvertIntToControlValue(int32 InValue);

public:
	void SetSelectedData(FGsCustomizeUIData* InData) { _selectedData = InData; }
	void SetCustomizeHandler(FGsCustomizeHandler* InCustomizeHandler) { _customizeHandler = InCustomizeHandler; }

	void GetBaseColorList(const TArray<int32>& InColorIndexList, TArray<FColor>& OutColorList);
};
