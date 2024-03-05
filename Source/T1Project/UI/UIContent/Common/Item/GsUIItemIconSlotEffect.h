// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsUIItemIconSlotEffect.generated.h"

class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIItemIconSlotEffect : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UWidgetSwitcher* _effectSwitcher;


protected:
	// UUserWidget override
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetSlotEffect(const EGsItemIconSlotEffectType InType);
	void SetSlotEffectByIndex(int32 InIndex);
};
