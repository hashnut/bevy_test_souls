// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUITalkBubble.generated.h"

/**
 * 
 */

class UImage;
class UTextBlock;

UCLASS()
class T1PROJECT_API UGsUITalkBubble : public UUserWidget
{
	GENERATED_BODY()

protected:

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//class UImage* _bgImage; //9-sprite

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _talkText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _talkbox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _emoticon; //9-sprite

private:
	float _talkLimitWidth{ 300.f };

private:
	float ComputeDesiredSize(FText text) const;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetText(FText txt);
	void SetEmoticon(int32 emojiID);
};
