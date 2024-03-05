// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIWindowExchangeCostume.generated.h"

class UGsUISummonPortraitBase;
class UGsUICostumeDetail;
class UTextBlock;
class UGsButton;
class UPanelWidget;
class UWidgetSwitcher;
class UScrollBox;

class UGsDynamicPanelSlotHelper;

class FGsItem;
struct FGsCostumeData;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowExchangeCostume : public UGsUIWindow
{
	GENERATED_BODY()

protected:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISummonPortraitBase* _portrait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _costumeName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _costumeGrade;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _collectionButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _flavorButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeDetail* _costumeDetail;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherCostumeList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listCostume;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _listPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDetailInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnProbabilityInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _emptyPanel;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

private:
	const FGsItem* _selectedItem{ nullptr };
	TWeakPtr<FGsCostumeData> _selectData;
	TArray<TPair<TWeakPtr<FGsCostumeData>, int64>> _exchangeList;

	CreatureGenderType _genderType{ CreatureGenderType::ALL };

	//CostumeId _selectedCostumeId{ INVALID_COSTUME_ID };

protected:
	UFUNCTION(BlueprintCallable)
	void OnClickPopupSelect();

	UFUNCTION(BlueprintCallable)
	void OnClickPopupCostumeDetail();

	UFUNCTION(BlueprintCallable)
	void OnClickPopupProbability();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnOpenCollection();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnOpenFlavorText();

	UFUNCTION()
	void OnCreateCostumeItemEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshCostumeItemEntry(int32 index, UWidget* InEntry);

	UFUNCTION(BlueprintCallable)
	void OnClickExchange();

	void OnSelectedCostumeItem(CostumeId id);

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool OnBack() override;

private:
	void SetInitExchangeInfo();

public:
	void SetCostumeExchangeTicketInfo(const FGsItem* item);
	void SetSelectCostume(TWeakPtr<FGsCostumeData> selectData, const TArray<TPair<TWeakPtr<FGsCostumeData>, uint64>>& probabilityList);
	void OnAckExchangedCostume();

};
