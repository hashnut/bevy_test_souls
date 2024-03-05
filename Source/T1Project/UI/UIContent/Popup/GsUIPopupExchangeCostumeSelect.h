// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Costume/GsCostumeData.h"
#include "GsUIPopupExchangeCostumeSelect.generated.h"

class UImage;
class UScrollBox;
class UGsSwitcherButton;
class UGsDynamicPanelSlotHelper;
class UGsUISkillDetailInfo;
class UGsUICostumeDetail;

class UTextBlock;
class UGsButton;
class UVerticalBox;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupExchangeCostumeSelect : public UGsUIPopup
{
	GENERATED_BODY()

	using Probability = TPair<TWeakPtr<FGsCostumeData>, uint64>;
	using CallbackFunctionType = TFunction<void(TWeakPtr<FGsCostumeData>, const TArray<Probability>&)>;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

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

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	FGsToggleGroup _toggleGroupGrade;

	TMap<int32, CostumeGrade> _tabGradeIndex;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _costumeName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _costumeGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeDetail* _costumeDetail;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _typePanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

private:
	ItemId _selectItemId{ INVALID_ITEM_ID };
	TMap<CostumeGrade, TArray<TWeakPtr<FGsCostumeData>>> _excludeGradeList; // 아이템 Effect에 포함된 코스튬 리스트 제외 목록
	TMap<CostumeGrade, TArray<Probability>> _exchangeEnableList;	// 선택된 코스튬 grade 별로 changible Costume 리스트 및 확률

	TMap<CostumeGrade, UGsSwitcherButton*> _gradeTabs;	// 변동성 탭버튼 정보
	TArray<TWeakPtr<FGsCostumeData>> _costumeGradeList;	// 선택된 탭의 코스튬 리스트

	CreatureGenderType _genderType{ CreatureGenderType::ALL };
	CostumeId _selectedReservedCostumeId{ INVALID_COSTUME_ID };
	CallbackFunctionType _callbackSelection;

	enum { Selection, Detail } _popupType{ Selection };

	int32 _gradeTab = -1;
	TWeakPtr<FGsCostumeData> _selectedCostume;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnGradeTabChanged(int32 index);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickSelectd();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();

	UFUNCTION()
	void OnCreateCostumeItemEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshCostumeItemEntry(int32 index, UWidget* InEntry);

	void OnSelectedCostumeItem(CostumeId id);

	virtual void OnInputCancel() override;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void BeginDestroy() override;

private:
	void SetRefreshCostumeItems(CostumeGrade grade);
	void MakeExchangeCostumeInfoByItemId();

public:
	struct PopupInitData
	{
		ItemId itemId{ 0 };
		CostumeId selectCostume{ INVALID_COSTUME_ID };
		bool isSelectionWindow{ true };
	};

	void InitializeData(PopupInitData* initParam, CallbackFunctionType cbSelection = nullptr);
	
};
