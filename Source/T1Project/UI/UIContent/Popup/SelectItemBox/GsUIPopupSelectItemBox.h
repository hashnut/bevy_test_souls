// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "DataSchema/Reward/GsSchemaRewardBoxData.h"
#include "GsUIPopupSelectItemBox.generated.h"


class UWrapBox;
class UGsButton;
class UPanelWidget;
class UTextBlock;
class UGsUISelectItemEntry;
class UGsDynamicPanelSlotHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupSelectItemBox : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirm;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWrapBox* _entryRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _calcRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _calcCountNum; // 1/100
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalcMin;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalcPlus;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalculator;

	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _itemListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _itemListPanelSlotHelper;

private:
	const FGsSchemaRewardBoxData* _boxData = nullptr;
	bool _isEnableCalc = false;
	int32 _selectedIndex = -1;
	uint64 _itemDBId = 0;
	uint32 _curCalcNum = 0;


protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


public:
	void SetItemList(const uint64 InItemDBID, const FGsSchemaRewardBoxData* InBoxData, const bool InIsSeveralUse);

private:
	void OnClickEntryWidget(uint8 InIndex);
	void InvalidateCalc();
	void OnConfirmSeveralUseCalculator(uint32 InCount);


protected:
	UFUNCTION()
	void OnConfrim();
	UFUNCTION()
	void OnCancel();
	UFUNCTION()
	void OnRefreshItemGroup(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnPlus();
	UFUNCTION()
	void OnMin();
	UFUNCTION()
	void OnCalculator();
	

};
