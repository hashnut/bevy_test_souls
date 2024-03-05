// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITrayTickerUnlock.generated.h"


class UTextBlock;
class UImage;
class UCanvasPanel;
/**
 * ÄÁÅÙÃ÷ ¾ð¶ô ¿¬Ãâ Æ¼Ä¿
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerUnlock : public UGsUITray
{
	GENERATED_BODY()	

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockContentsName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockContentsDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageContentsIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panelIcon;

protected:
	bool _isClosing = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	void SetText(const FText& InContentsName);
	void SetText(const FText& InContentsName, const FText& InContentsDesc);
	void SetImage(const FSoftObjectPath& InIconPath);

protected:
	bool SetReservedData();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayOpenAnimation();
};
