// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/Control/GsStaticPanelSlotHelper.h"
#include "GsUIComboBox.generated.h"


/**
 * 기본적인 텍스트 출력 콤보박스. 리스트 동적 생성.
 */
UCLASS()
class T1PROJECT_API UGsUIComboBox : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnSelectComboBox, int32 /*Index*/);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelList;

protected:
	FGsStaticPanelSlotHelper _panelSlotHelper;
	FGsToggleGroup _toggleGroup;
	TArray<FText> _nameList;

public:
	FOnSelectComboBox OnSelectComboBox;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void Clear();
	void StartAddMenu();
	void AddMenu(const FText& InNameText);
	void FinishAddMenu();
	void SetSelectedIndex(int32 InIndex);

protected:
	void ShowList(bool bInShow);

	UFUNCTION()
	void OnClickSlot();

	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	void OnClickListEntry(int32 InIndex);
};
