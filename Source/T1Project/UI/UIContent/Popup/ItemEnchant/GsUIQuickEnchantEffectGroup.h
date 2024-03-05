// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Popup/ItemEnchant/GsUIQuickEnchantGroup.h"
#include "GsUIQuickEnchantEffectGroup.generated.h"

class UCanvasPanel;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIQuickEnchantEffectGroup : public UGsUIQuickEnchantGroup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _enchantSuccessEffectPanel;


protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
};
