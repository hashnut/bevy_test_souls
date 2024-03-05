// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "GsUIGuildClearReward.generated.h"

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIGuildClearReward : public UGsUIWindow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _titleTextBlock;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _moneyTextBlock;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _rewardListView;

public:
	void SetData(){}
};