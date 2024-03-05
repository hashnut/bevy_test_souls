// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUILineGraphPoints.generated.h"

struct FGsLineGraphData;
class UPanelWidget;
class UGsButton;
class UGsUITooltipDesc;

/**
 * 꺾은선 그래프의 포인트 부분(버튼) - 필요시 구현
 */
UCLASS()
class T1PROJECT_API UGsUILineGraphPoints : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootButtons;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltip;

protected:
	UPROPERTY(Transient)
	TArray<UGsButton*> _btnList;

	const FGsLineGraphData* _data = nullptr;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	void SetData(const FGsLineGraphData* InData);

	UFUNCTION(BlueprintCallable)
	void OnClickPoint(int32 InIndex);
};
