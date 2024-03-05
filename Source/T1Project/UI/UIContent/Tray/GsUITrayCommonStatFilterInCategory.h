#pragma once

/**
* file		GsUITrayCommonStatFilterInCategory.h
* @brief	Category가 존재하는 StatFilter
*			아이템 컬렉션 / 칭호 시스템 등에 사용 됨.
* @author	PKT
* @date		2022/02/10
**/

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "GsUITrayCommonStatFilterInCategory.generated.h"


class UWidget;
class UGsButton;
class UScrollBox;
class UTextBlock;
class UPanelWidget;
class UWidgetSwitcher;
class UEditableTextBox;
class UGsDynamicPanelSlotHelper;


UCLASS()
class T1PROJECT_API UGsUITrayCommonStatFilterInCategory : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnApply;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnReset;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSerachTextReset;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _textSearchStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherStatList;

	/**
	 * 2022/02/10 PKT - 카테고리 관련..
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxTabButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _categoryTabButtonbEntry;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperCategoryBtnTab;

	/**
	 * 2022/02/10 PKT - Stat
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxStatSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _statUnitEntry;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelCheckBoxStatSet;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperCheckBoxSet;


protected:
	// 2022/02/10 PKT - Category Toggle Group
	FGsToggleGroup _toggleGroupCategory;

	TArray<StatType> _contentsStatAllSet;

	TArray<StatType> _selectedStatSet;

	TArray<TPair<StatType,FText>> _viewListStatItemSet;
	
	// 2024/1/17 PKT - 검색
	FString _textSerachWord;

public:
	TFunction<void(TArray<StatType>&)> _callbackApply;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/************************************************************************/
	/* Bind                                                                 */
	/************************************************************************/
protected:
	// 2022/02/11 PKT - 닫기(취소)
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickedBtnClose();

	// 2022/02/11 PKT - 적용
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickedBtnApply();

	// 2022/02/11 PKT - 초기화
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickedBtnReset();

	// 2021/07/16 PKT - 검색 초기화
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickedBtnSerachClear();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod);

	UFUNCTION()
	void OnTextChangedMsg(const FText& InText);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshEntryTabButton(int32 InIndex, UWidget* InEntry);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnCreateEntryCheckBox(UWidget* InEntry);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshEntryCheckBox(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickedCheckBox(bool bIsChecked, int32 InIndexInGroup);

	void OnSelectedCategory(int32 InIndex);

	/************************************************************************/
	/* Refresh                                                              */
	/************************************************************************/
	void MakeViewList();

protected:
	class FGsStatFilterHandler* GetHandler() const;

public:
	void SetData(const TArray<StatType>& InContentsAllStatSet, const TArray<StatType>& InSelectedStatSet, TFunction<void(TArray<StatType>&)> InCallback);
};