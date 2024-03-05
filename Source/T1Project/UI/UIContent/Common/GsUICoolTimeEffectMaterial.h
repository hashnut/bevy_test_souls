// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/GsUICoolTimeEffect.h"
#include "GsUICoolTimeEffectMaterial.generated.h"

class UImage;

/**
 * 쿨타임 흐르는 연출 UI
 */
UCLASS()
class T1PROJECT_API UGsUICoolTimeEffectMaterial : public UGsUICoolTimeEffect
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgProgress;

protected:
	virtual void NativeConstruct() override;
	
public:
	virtual void SetProgress(float InRate) override;
};
