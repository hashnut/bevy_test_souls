//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "UI/UIContent/Common/Numpad/TGsCalculateHelper.h"
//#include "UI/UIContent/Tray/Guild/GsUIGuildStoragePageBase.h"
//#include "GsUIGuildStoragePageDistribution.generated.h"
//
//
//class UGsSwitcherButton;
//class UWidgetSwitcher;
//class UGsHorizontalBoxIconSelector;
//class UGsButton;
//class UTextBlock;
//class UImage;
//class UGsUITrayGuildStorage;
//class UGsUIPopupCommonItemAction;
//class UGsGuildStorageDistributionListItem;
//class UGsUIIconItemInventory;
//struct FGsGuildStorageMemberData;
//struct IGsMessageParam;
///**
// * 기사단 창고 아이템 분배 페이지
// */
//UCLASS()
//class T1PROJECT_API UGsUIGuildStoragePageDistribution : public UGsUIGuildStoragePageBase
//{
//	GENERATED_BODY()
//
//public:
//	using FilterSortFunc = TFunction<void(int32)>;
//
//	enum EGsGuildStorageFilterType
//	{
//		None = -1,
//		Level = 0,
//		Name,
//		MemberGrade,
//		Contribution,
//		AccumContribution,
//	};
//
//	const int64 GUILD_STORAGE_DISTRIBUTION_AMOUNT_MIN = 1;
//
//protected:
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsSwitcherButton* _btnFilterLevel;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsSwitcherButton* _btnFilterName;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsSwitcherButton* _btnFilterMemberGrade;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsSwitcherButton* _btnFilterContribution;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsSwitcherButton* _btnFilterAccumContribution;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UWidgetSwitcher* _switcherDistribution;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsHorizontalBoxIconSelector* _iconSelector;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockItemName;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsButton* _btnMinus;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsButton* _btnPlus;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsButton* _btnCalcOpen;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockAmount;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockDistributionError;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UImage* _imageLine;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsButton* _btnDistribution;
//
//	UPROPERTY()
//	UGsUIIconItemInventory* _uiIcon;
//
//private:
//	UGsUITrayGuildStorage* _storageUI = nullptr;
//	UGsUIPopupCommonItemAction* _distributionUI = nullptr;
//
//	TArray<FilterSortFunc> _sortFuncList;
//	TArray<UGsSwitcherButton*> _filterList;
//	EGsGuildStorageFilterType _currFilter = EGsGuildStorageFilterType::AccumContribution;
//
//	TArray<UGsGuildStorageDistributionListItem*> _dataList;
//	int32 _selectedIndex = 0;
//	
//	TGsCalculateHelper<int64> _calculateHelper;
//	
//public:
//	virtual void BeginDestroy() override;
//
//private:
//	virtual void NativeOnInitialized() override;
//	virtual void NativeConstruct() override;
//	virtual void NativeDestruct() override;
//
//protected:
//	virtual void InvalidateAllInternal() override;
//	
//public:
//	void InvalidateDistribution();
//	void InvalidateDistributionItem();
//	void InvalidateCalculator();
//
//public:
//	virtual void Show() override;
//
//	void SetStorageUI(UGsUITrayGuildStorage* InStorageUI);
//	FGsGuildStorageMemberData* GetSelectedMemberData();
//
//	/************************************************************************/
//	/* 계산기        													    */
//	/************************************************************************/
//protected:
//	UFUNCTION()
//	void OnClickedCalcMinus1();
//	UFUNCTION()
//	void OnClickedCalcPlus1();
//	UFUNCTION()
//	void OnClickedCalcOpen();
//
//	void OnChangedCalculateHelper(int64 InNumber);
//	void OnFinishCalculatePopupCalculator(int64 InValue);
//
//	void OpenPopupCalculator();
//
//	/************************************************************************/
//	/* 분배	        													    */
//	/************************************************************************/
//protected:
//	UFUNCTION()
//	void OnClickDistribution();
//
//	bool CheckDistribution(int64 InAmount, OUT FText& OutError);
//
//	/************************************************************************/
//	/* 멤버 목록 정렬        													    */
//	/************************************************************************/
//protected:
//	UFUNCTION()
//	void OnClickFilterLevel();
//	UFUNCTION()
//	void OnClickFilterName();
//	UFUNCTION()
//	void OnClickFilterMemberGrade();
//	UFUNCTION()
//	void OnClickFilterContribution();
//	UFUNCTION()
//	void OnClickFilterAccumContribution();
//
//private:
//	void SetFilter(EGsGuildStorageFilterType InType, int32 InState);
//
//	void SortByLevel(int32 InState);
//	void SortByName(int32 InState);
//	void SortByMemberGrade(int32 InState); // 기본값
//	void SortByContribution(int32 InState);
//	void SortByAccumContribution(int32 InState);
//
//	// InState: 0: 없음, 1: 오름차순, 2: 내림차순
//	void SetUISelectFilter(EGsGuildStorageFilterType InType, int32 InState);
//
//	/************************************************************************/
//	/* 이벤트         													    */
//	/************************************************************************/
//public:
//	void OnChangeMyAuthority();
//
//	void OnClickedMember(UObject* InListItem);
//	void SetSelectedIndex(int32 InIndex, bool bInForced);
//	void InitializeCalculator();
//};
