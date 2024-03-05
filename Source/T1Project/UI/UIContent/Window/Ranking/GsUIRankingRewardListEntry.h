// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIRankingRewardListEntry.generated.h"

class UTextBlock;
class UImage;

/**
 * 랭킹 보상 정보 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUIRankingRewardListEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGrade;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockReward;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageBadge;

public:
	void SetData(FText InGradeText, FText InRewardText, const FSoftObjectPath* InIconPath = nullptr);
};
