// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIGuildAuctionResultEntry.generated.h"

class UTextBlock;
/**
 * 기사단 경매 정산 팝업(GsUIPopupGuildAuctionResult)의 유저 정보
 */
UCLASS()
class T1PROJECT_API UGsUIGuildAuctionResultEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockUserName;

public:
	void SetData(const FText& InGradeName, const FText& InUserName);
};