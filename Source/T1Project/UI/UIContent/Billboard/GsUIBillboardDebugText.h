// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIBillboardDebugText.generated.h"

class UTextBlock;
class UCanvasPanel;
class UVerticalBox;
class UImage;
//--------------------------------------------------------------
// SHIPPING 빌드가 아닐 때 NPC 의 정보를 출력하기 위해 사용한다.
//--------------------------------------------------------------

const FSlateColor WHITE = FSlateColor(FColor::White);
const FSlateColor RED = FSlateColor(FColor::Red);

/**
 *
 */

UCLASS()
class T1PROJECT_API UGsUIBillboardDebugText : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "GsUIBillboardDebugText", meta = (BindWidget))
	UCanvasPanel* _canvasPanel;

	UPROPERTY(BlueprintReadWrite, Category = "GsUIBillboardDebugText", meta = (BindWidget))
	UVerticalBox* _verticalBox;

	UPROPERTY(BlueprintReadWrite, Category = "GsUIBillboardDebugText", meta = (BindWidget))
	UImage* _backgroundImg;

	UPROPERTY()
	UObject* _fontFamily = nullptr;

#if	!UE_BUILD_SHIPPING

private:
	// UPROPERTY 를 쓰지 말아야 하는 이유는 당연히 알 것이다...
	//UPROPERTY(BlueprintReadWrite, Category = "GsUIBillboardDebugText")
	TArray<UTextBlock*> _textBlockList;

	TArray<FString> _debugInfoList;


	bool _isInitialized = false;

public:

	void InitializeDebugTextInfo(TArray<FString>&& InDebugTextArray);
	void SetDebugTextInfo(TArray<FString>&& InDebugTextArray);

	void SetInitializedFlag() { _isInitialized = false; }
#endif
};

