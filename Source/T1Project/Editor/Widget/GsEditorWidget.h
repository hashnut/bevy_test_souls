// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsEditorWidget.generated.h"

class UTextBlock;
class UImage;
//----------------------------------------------------------------
// 에디터 모드일때 레벨 뷰포트에서 액터의 설명을 위해 사용한다. 
//----------------------------------------------------------------

UCLASS()
class T1PROJECT_API UGsEditorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "GsEditorWidget", meta = (BindWidget))
		UTextBlock* TitleText;
	UPROPERTY(BlueprintReadWrite, Category = "GsEditorWidget", meta = (BindWidget))
		UImage* TitleImg;
public:
	UPROPERTY(BlueprintReadWrite, Category = "GsEditorWidget", meta = (BindWidget))
		UTextBlock* DescriptionText;
	UPROPERTY(BlueprintReadWrite, Category = "GsEditorWidget", meta = (BindWidget))
		UImage* DescImg;	

public:
	void SetTitleText(FString inTtile);
	void SetTitleImgColor(FColor inColor);
	void SetDescriptionText(FString inTtile);
	void SetDescriptionImgColor(FColor inColor);
};
