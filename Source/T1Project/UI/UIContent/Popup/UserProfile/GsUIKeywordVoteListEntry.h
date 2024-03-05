// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIKeywordVoteListEntry.generated.h"

class UGsUIRadioButton;
class UImage;
class UTextBlock;
class UWidgetSwitcher;
class IGsToggleGroupEntry;
class FGsToggleGroup;
/**
 * 유저 프로필 키워드 투표 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUIKeywordVoteListEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRadioButton* _radioButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageIconMain;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNameMain;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCountMain;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageIconNormal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNameNormal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCountNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherKeyword; // 최다키워드(0), 일반(1), 미선택(2)

protected:
	int32 _switcherIndex = 2;
	bool _isLoaded = false;
	FGsToggleGroup* _toggleGroup;

public:
	void SetSwitcherIndex(int32 InIndex);
	void SetData(bool InIsMine, const FSoftObjectPath& InIconPath, const FText& InKeywordName, uint64 InKeywordCount);
	void SetIsSelected(bool InIsSelected);
	void SetToggleGroup(FGsToggleGroup* InToggleGroup);

protected:
	void SetIcon(const FSoftObjectPath& InIconPath, UImage* InImage);
	void SetName(const FText& InKeywordName, UTextBlock* InTextBlock);
	void SetCount(uint64 InKeywordCount, UTextBlock* InTextBlock);

protected:
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);

public:
	IGsToggleGroupEntry* GetToggleGroupEntry();
};
