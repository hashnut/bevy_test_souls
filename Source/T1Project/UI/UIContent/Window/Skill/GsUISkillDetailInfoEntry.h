// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUISkillDetailInfoEntry.generated.h"

/**
 * 스킬 상세정보 UI
 */
UCLASS()
class T1PROJECT_API UGsUISkillDetailInfoEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockTitle;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockDesc;

public:
	void SetTitle(const FText& InText);
	void SetDesc(const FText& InText);
};
