// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIGuildQuestPointEffect.generated.h"

class UPanelWidget;

/**
 * 기사단 열성 포인트 연출 
 */
UCLASS()
class T1PROJECT_API UGsUIGuildQuestPointEffect : public UUserWidget
{
	GENERATED_BODY()

public:
	enum class EGsGuildQuestPointState
	{
		LOCKED = 0,
		ACTIVE,
		BOOST,
	};

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelActive;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelBoost;

public:
	void SetPointState(EGsGuildQuestPointState InState);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationBoost();

	UFUNCTION(BlueprintCallable)
	void OnFinishAnimationBoost();
};
