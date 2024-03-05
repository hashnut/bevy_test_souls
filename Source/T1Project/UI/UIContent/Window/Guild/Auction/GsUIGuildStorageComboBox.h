// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "GsUIGuildStorageComboBox.generated.h"


class UPanelWidget;
class UGsDynamicPanelSlotHelper;
/**
 * 기사단 정산의 각 페이지에 사용되는 콤보박스
 */
UCLASS()
class T1PROJECT_API UGsUIGuildStorageComboBox : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _btnMenu;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockMenu;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMain;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMainList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSub;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSubList;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperMain;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperSub;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryClassMain;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryClassSub;

public:
	DECLARE_DELEGATE(FOnClickComboMenu);
	DECLARE_DELEGATE_OneParam(FOnSelectComboBox, int32 /*Index*/);
	DECLARE_DELEGATE_TwoParams(FOnSelectComboBoxSide, int32 , int32);
	FOnClickComboMenu OnClickComboMenu;
	FOnSelectComboBox OnSelectComboBoxMain;
	FOnSelectComboBoxSide OnSelectComboBoxSub;

protected:
	FGsToggleGroup _toggleGroupMain;
	FGsToggleGroup _toggleGroupSub;

	FText _baseMenuText;
	
	TArray<FText> _mainMenuList;
	TMap<int32, TArray<FText>> _subMenuListMap; // Main Index, Sub List

	int32 _lastSelectedIndex = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void Clear();
	void StartAddMenu();
	void SetBaseText(const FText& InText);
	void AddMenu(const FText& InMenuText);
	void AddMenuWithSubMenu(const FText& InMenuText, const TArray<FText>& InSubMenuTextList);
	void FinishAddMenu();
	void SetSelectedIndex(int32 InMainIndex, int32 InSubIndex = 0);

	void Close();
	void ShowList(bool bInShow);
	void ShowSubList(bool bInShow, int32 InMainIndex = 0);

protected:
	void SetMainMenuText(int32 InIndex);
	void SetSubMenuText(int32 InIndex);

	UFUNCTION()
	void OnClickMenu();

	UFUNCTION()
	void OnRefreshMainEntry(int32 InIndex, class UWidget* InEntry);
	UFUNCTION()
	void OnRefreshSubEntry(int32 InIndex, class UWidget* InEntry);

	void OnClickListMainEntry(int32 InIndex, bool bInIsSelected);
	void OnClickListSubEntry(int32 InIndex, bool bInIsSelected);
	

};