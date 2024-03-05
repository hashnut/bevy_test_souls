// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "UI/Sample/GsUISampleScroll.h"
#include "GsUISampleEntry.generated.h"

/**
 * 스크롤 밑에 달리는 슬롯 UI 예제
 */
UCLASS()
class T1PROJECT_API UGsUISampleEntry : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	FText _testText;

public:
	void SetListItem(struct UGsUISampleScroll::FGsSampleData* InItem);
};
