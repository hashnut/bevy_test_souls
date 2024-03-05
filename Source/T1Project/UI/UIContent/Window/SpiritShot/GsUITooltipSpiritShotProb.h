// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITooltip.h"
#include "GsUITooltipSpiritShotProb.generated.h"


class UTextBlock;

/**
 * 잠식 확률 등을
 */
UCLASS()
class T1PROJECT_API UGsUITooltipSpiritShotProb : public UGsUITooltip
{
	GENERATED_BODY()
	
	/************************************************************************/
	/* 필요 위젯                                                            */
	/************************************************************************/
protected:

	/** 툴팁 설명 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _descText;

	/** 최소 성공 확률 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _minSuccessRate;

	/** 최대 성공 확률 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _maxSuccessRate;

	/** 하락 확률 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _fallRate;

	/************************************************************************/
	/* Setter                                                               */
	/************************************************************************/
public:
	void InvalidateAll(FText InDescText, FText&& InMinRate, FText&& InMaxRate, FText&& InFallRate);
};
