// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "GsUIGuildAcquisitionEntry.generated.h"

class UTextBlock;
class UHorizontalBox;
class UGsDynamicPanelSlotHelper;
struct FGsSchemaGuildAcquisition;
/**
 * 기사단 획득 설정 Entry
 */
UCLASS()
class T1PROJECT_API UGsUIGuildAcquisitionEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	DECLARE_DELEGATE_TwoParams(FOnChangeOption, const FGsSchemaGuildAcquisition*, int32);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _panelOptionList;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

public:
	FOnChangeOption OnChangeOption;

protected:
	FGsToggleGroup _toggleGroup;

	const FGsSchemaGuildAcquisition* _data = nullptr;
	TArray<int32> _gradeList;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnRefreshEntryOption(int32 InIndex, class UWidget* InEntry);

	void OnChangedOption(int32 InIndex);

	void GetGradeText(int32 InGrade, OUT FText& OutGradeText);

public:
	void SetData(const FGsSchemaGuildAcquisition* InData, int32 InValue);

};
