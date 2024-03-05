// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Window/Fairy/GsSelectedPanelInterface.h"
#include "GsUIPageCostumeAppearance.generated.h"

class UGsSwitcherButton;
class UGsButton;
class UScrollBox;
class UPanelWidget;
class UGsDynamicPanelSlotHelper;
class UGsUICostumeAppearanceDetail;

struct FGsCostumeData;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPageCostumeAppearance : public UUserWidget, public IGsSelectedPanelInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listCostume;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _listPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeNormal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeMagic;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeRare;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeEpic;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeUnique;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeLegend;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeAll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _emptyPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInit;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChange;


	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeAppearanceDetail* _equipCostume;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeAppearanceDetail* _hairAppearance;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeAppearanceDetail* _bodyAppearance;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	FGsToggleGroup _toggleGroupGrade;
	TMap<int32, CostumeGrade> _tabGradeIndex;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnGradeTabChanged(int32 index);

	UFUNCTION()
	void OnCreateCostumeItemEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshCostumeItemEntry(int32 index, UWidget* InEntry);

	UFUNCTION()
	void OnInitializeAppearance();

	UFUNCTION()
	void OnSetApparance();

	void OnSelectedAppearanceItem(CostumeId id);

	void OnFixHairCostume(bool set);
	void OnFixBodyCostume(bool set);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void BeginDestroy() override;

	virtual void InitializeTab(int32 tabIndex) override;
	virtual void OnSelected() override;
	virtual void OnUnselected() override;

public:
	void OnUpdateItemList(TArray<CostumeId>& arr) override;
	
private:
	void SetRefreshCostumeItems(CostumeGrade grade);
	void UpdateItemSelection(CostumeId selectId);

private:
	TArray<TWeakPtr<FGsCostumeData>> _costumeGradeList;

	int32 _gradeTab = -1;
	CostumeId _selectedCostumeId = INVALID_COSTUME_ID;
	CostumeId _selectedReservedCostumeId = INVALID_COSTUME_ID;
	CreatureGenderType _genderType{ CreatureGenderType::ALL };
	CreatureWeaponType _weaponType{ CreatureWeaponType::ALL };
};
