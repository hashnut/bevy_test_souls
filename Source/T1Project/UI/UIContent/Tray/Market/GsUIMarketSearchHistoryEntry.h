// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIMarketSearchHistoryEntry.generated.h"

class UTextBlock;
class UGsButton;

/**
 * 거래소 검색 필터의 체크박스
 */
UCLASS()
class UGsUIMarketSearchHistoryEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
	
public:
	// 이벤트... 인자 하나에 선택인지 삭제인지 넣어보내자
	DECLARE_DELEGATE_TwoParams(FOnClickEvent, int32 /*InIndex*/, bool /*bIsRemove*/);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRemove;
	
protected:
	int32 _index = 0;
	
public:
	FOnClickEvent OnClickEvent;	
	
protected:
	virtual void NativeOnInitialized() override;	
	
public:
	void SetData(int32 InIndex, const FString& InData);	
	
protected:
	UFUNCTION()
	void OnClickSlot();
	UFUNCTION()
	void OnClickRemove();
};
