// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIAgitRaidCountListEntry.generated.h"

class UWidgetSwitcher;

/**
 * 기사단 영지던전 공략던전의 공략 횟수
 */
UCLASS()
class UGsUIAgitRaidCountListEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherIcon;

public:
	void SetData(bool bIsActive);
};
