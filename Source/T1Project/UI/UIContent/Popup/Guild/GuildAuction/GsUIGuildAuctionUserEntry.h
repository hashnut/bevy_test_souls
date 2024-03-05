// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIGuildAuctionUserEntry.generated.h"

class UTextBlock;
/**
 * 기사단 경매 참여자 팝업(GsUIPopupGuildAuctionUser)의 참여자 정보
 */
UCLASS()
class T1PROJECT_API UGsUIGuildAuctionUserEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockUserName;

public:
	void SetData(const FString& InUserName);
};