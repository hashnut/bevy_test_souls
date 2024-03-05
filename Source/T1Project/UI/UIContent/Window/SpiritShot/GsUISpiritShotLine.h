// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUISpiritShotLine.generated.h"

class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUISpiritShotLine : public UUserWidget
{
	GENERATED_BODY()


	/************************************************************************/
	/* 완료 연출용 위젯                                                     */
	/************************************************************************/
public:
	/** 이미 완료한 라인들과 마지막 라인의 이펙트 효과 분기를 위한 위젯 스위쳐 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _lineEffectSwitcher;


	/************************************************************************/
	/* 연출 분기                                                            */
	/************************************************************************/
public:
	/** 라인이 최전선이 아니라, 이미 지나간 라인이라는 효과를 준다 */
	void SetLineEffect(bool InIsLast = false);
};
