// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"

#include "GsUIGuildAcquisitionOptionButton.generated.h"

class UGsButton;
class UTextBlock;
/**
 * 기사단 획득 설정 Entry 내 옵션 버튼
 */
UCLASS()
class T1PROJECT_API UGsUIGuildAcquisitionOptionButton : public UUserWidget, public IGsToggleGroupEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUnSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockOn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockOff;

	UPROPERTY()
	bool _bIsSelected = false;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnClickSelect();

	UFUNCTION()
	void OnClickUnSelect();

public:
	void SetText(const FText& InText);

	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;
};
