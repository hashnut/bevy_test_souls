// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "GsUIAutoDepositEntry.generated.h"


class UTextBlock;
class UGsButton;
class UImage;
class UPanelWidget;

/**
 * 창고 자동 보관 설정 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUIAutoDepositEntry : public UUserWidget, public IGsEntryInterface, public IGsToggleGroupEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGroupName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class USlider* _sliderGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UProgressBar* _progressBarGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUITooltipDesc* _tooltipDesc;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UImage* _imgSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsToggleButton* _btnSlot;

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeAutoDepositGrade, int32, ItemGrade);
	FOnChangeAutoDepositGrade OnChangeAutoDepositGrade;

private:
	ItemGrade _minGrade = ItemGrade::NONE;
	ItemGrade _maxGrade = ItemGrade::NONE;
	ItemGrade _grade = ItemGrade::NONE;

	bool _bIsSelected = false;

	int32 _prevSliderValue = -1;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetIcon(const FSoftObjectPath& InIconPath);
	void SetGroupName(const FText& InGroupName);
	void SetGradeRange(const ItemGrade InMinGrade, const ItemGrade InMaxGrade);
	void SetGrade(ItemGrade InGrade, bool InUseUserSetting);
	void SetTooltipText(const FText& InTooltip);

	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override { return _bIsSelected; }

public:
	UFUNCTION()
	void OnClickSlot();
	UFUNCTION()
	void OnClickTooltip();
	UFUNCTION()
	void OnChangeSliderValue(float InChangeValue);

};
