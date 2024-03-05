
#pragma once

/**
* file		GsUIWindowTitleSystem.h
* @brief	칭호 시스템 관련 Window - 기존에 WindowTitle이라는 명이 있어서 뒤에 System을 붙였음.
* @author	PKT
* @date		2022/01/19
**/

#include "CoreMinimal.h"
#include "Title/ETitleEnum.h"
#include "Title/GsTitleFilter.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "GsUIWindowTitleSystem.generated.h"

class USizeBox;
class UGsButton;
class UListView;
class UTextBlock;
class UScrollBox;
class UGsPoolUObject;
class UGsToggleButton;
class UWidgetSwitcher;
class UGsSwitcherButton;
class UGsUIRedDotDefault;
class UGsUITitleEquipSlotEffect;
class UGsDynamicPanelSlotHelper;

UCLASS()
class T1PROJECT_API UGsUIWindowTitleSystem : public UGsUIWindow
{
	GENERATED_BODY()

	// 2021/11/23 PKT - invalid main category value
	const static int32 INVALID_CATEGORY_INDEX = -1;

	// 2021/11/23 PKT - invalid sub category value
	const static int32 DEFAULT_SUB_CATEGORY_INDEX = 0;

	// 2021/11/23 PKT - invalid viewtype value
	const static int32 DEFAULT_VIEWTYPE_INDEX = 0;

	using FilterSet = TArray<TPair<StatType, bool>>;

	/************************************************************************/
	/* BP Bind                                                              */
	/************************************************************************/
protected:
	// 2021/11/23 PKT - 뒤로 가기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	// 2021/11/23 PKT - 닫기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	// 2022/01/19 PKT - 메인 카테고리 앞
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnFront;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMainCategoryFrontOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMainCategoryFrontOn;

	// 2022/01/19 PKT - 메인 카테고리 앞 RedDot
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _reddotFrontRedDot;

	// 2022/01/19 PKT - 메인 카테고리 뒤
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMainCategoryBackOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMainCategoryBackOn;

	// 2022/01/19 PKT - 메인 카테고리 뒤 RedDot
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _reddotBackRedDot;

	/**
	 * Sub Category Dynamic Helper
	 */
	// 2021/08/03 PKT - sub tab ..
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperSubBtnTab;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfSubBtnTabEntry;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxSubCategory;

	/**
	 * List View 
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherTitleList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _listViewTitle;

	/**
	 * View Filter
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	USizeBox* _sizeBoxFilterGroup;

	// 2022/01/19 PKT - 전체
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _btnViewTypeAll;

	// 2022/01/19 PKT - 보유
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _btnViewTypeAcquire;

	// 2022/01/19 PKT - 미보유
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _btnViewTypeDotHave;

	// 2022/01/19 PKT - 스탯 필터
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnStatFilter;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITitleEquipSlotEffect* _equipSlotEffect;

	/**
	 * 칭호 컬렉션 정보
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSymbol;

	// 2022/01/21 PKT - 칭호 보유 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textRetentionStatusCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCollection;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _reddotCollection;

	// 2022/01/05 PKT - collection list Data Entry Buffer
	UPROPERTY()
	UGsPoolUObject* _poolTitleData;

	// 2022/01/19 PKT - main category toggle
	FGsToggleGroup _toggleGroupMainTabs;

	// 2022/01/19 PKT - sub category toggle
	FGsToggleGroup _toggleGroupSubTabs;

	// 2022/01/19 PKT - sub category toggle
	FGsToggleGroup _toggleGroupViewTypeBtns;

	// 2022/01/24 PKT - Filter
	TArray<TPair<StatType, bool>> _statFilterSet;

	ETitleMainCategory _lastMainTab = ETitleMainCategory::FORNT;
	int32 _lastSubCategory = DEFAULT_SUB_CATEGORY_INDEX;
	int32 _lastViewTypeBtn = DEFAULT_VIEWTYPE_INDEX;

	ETitleMainCategory _prevMainCategory = ETitleMainCategory::MAX;
	int32 _prevSubCategory = INVALID_CATEGORY_INDEX;

	/************************************************************************/
	/* override                                                             */
	/************************************************************************/
public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool OnBack() override;

	bool SyncContentState() override;

protected:
	void MakeViewList();

	void MakeTitleView();

	void ChangedVisibleWidget(ETitleMainCategory InMainCategory);

	TArray<TSharedPtr<IGsTitleFilterConditionBase>> MakeViewListFilters();

	void CheckOutTitleList(ETitleMainCategory InPrevMainCategory, int32 InPrevSubCategory);

	/************************************************************************/
	/* Refresh                                                              */
	/************************************************************************/
protected:
	// 2022/01/21 PKT - 컬렉션 상태 갱신
	void RefreshCollection();

	// 2022/01/24 PKT - 칭호 리스트 갱신
	void RefreshTitleViewList(TitleId InTitleId);

	// 2022/01/24 PKT - Category RedDot 갱신
	void RefreshCategoryRedDot();

	// 2022/01/24 PKT - 장착 정보 갱신
	void RefreshEquip(bool InIsShowEffect);

	/************************************************************************/
	/* bind 이벤트                                                           */
	/************************************************************************/
private:
	// 2022/01/19 PKT - Clicked Main Category
	void OnSelectedBtnMainCategory(int32 InIndex);

	// 2022/01/19 PKT - Clicked Sub Category
	void OnSelectedBtnSubCategory(int32 InIndex);

	// 2022/01/19 PKT - Clicked View Type
	void OnSelectedBtnViewType(int32 InIndex);

	UFUNCTION()
	void OnRefreshEntrySubBtnTab(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnTitleStateChagned(int32 InTitleId, bool IsEquip);

	UFUNCTION()
	void OnClickedStatFilter();

	UFUNCTION()
	void OnClickedOpenCollection();

	UFUNCTION()
	void OnApplyStatFilter(const TArray<StatType>& InData);

	/************************************************************************/
	/* Invalid                                                              */
	/************************************************************************/
public:
	void InvalidCollection();

	void InvalidTitle(TitleId InTitleId);

	void InvalidEquip();

	void InvalidCategoryRedDot();
};