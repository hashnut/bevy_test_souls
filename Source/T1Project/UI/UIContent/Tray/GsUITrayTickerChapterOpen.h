// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITrayTickerChapterOpen.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerChapterOpen : public UGsUITray
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE(FOnFinished);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _chapterIndexText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _chapterTitleText;	

public:
	FOnFinished _delegateOnFinished;

public:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnExitAnimation();

	void SetData(int32 inIndex, const FText& InText);
};
