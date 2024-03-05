// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "GsUIRedDotMulti.generated.h"

/**
 * 레드닷 여러 타입이 있는 경우, 우선순위에 의해 하나만 표시함
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIRedDotMulti : public UGsUIRedDotBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockNumber;

protected:
	TArray<EGsIconRedDotType> _activeList;

public:
	virtual void SetRedDot(bool bIsRedDot, EGsIconRedDotType InType = EGsIconRedDotType::NORMAL, int32 InNumber = 0) override;

protected:
	void CheckPriority();
};
