// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h" 

#include "UI/UILib/Base/GsUIWindow.h"

#include "GsUIWindowMultiLevelRank.generated.h"


class UGsButton;
class UGsSwitcherButton;
class UTextBlock;
class UScrollBox;
class UGsDynamicPanelSlotHelper;
class UWidgetSwitcher;
class UImage;

struct FGsMultiLevelRankSlotData;
/**
 * ½Â±Þ ½Ã½ºÅÛ Window Class
 */
UCLASS()
class T1PROJECT_API UGsUIWindowMultiLevelRank : public UGsUIWindow
{
	GENERATED_BODY()

	/************************************************************************/
	/* BP Bind                                                              */
	/************************************************************************/
protected:
	/**
	 * Common Button( Back / Close ) 
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	/**
	 * ÇÏ´Ü »óÈ² ¾È³» ¹®±¸
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherInfoImage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageInfo;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInfo;

	/**
	 * ¼­¾à º¯°æ Button / ´©Àû È¹µæ ½ºÅÈ Button
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPledgeChange;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAcquiredStat;

	/**
	 * ½Â±Þ ·©Å© ½½·Ô Scroll Box
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _mainScrollBox;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperRank;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfRankEntry;


	/************************************************************************/
	/* ÀÏ¹Ý º¯¼ö                                                            */
	/************************************************************************/	
	TWeakObjectPtr<UUserWidget> _focusSlotWidget;	// current rank slot focus data

	TArray<int32> _slotRankList; // ½Â±Þ ½½·Ô(GsSchemaMultiLevelRank.rank) + ¼­¾à ½½·Ô(0)

	/************************************************************************/
	/* override                                                             */
	/************************************************************************/
public:
	void BeginDestroy() override;

protected:
	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	virtual bool SyncContentState() override;

	/************************************************************************/
	/* Entry Initialized									                */
	/************************************************************************/
	UFUNCTION()
	void OnInitializedRankEntry(int32 InIndex, class UWidget* InEntry);

	void InitializeEntryAll();
	void InitializeFocus();

	/************************************************************************/
	/* Button Clicked                                                       */
	/************************************************************************/
	UFUNCTION()
	void OnClickedRankSlot(UUserWidget* InWidget);

	UFUNCTION()
	void OnClickedPledgeChange();

	UFUNCTION()
	void OnClickedAcquiredStat();

	UFUNCTION()
	void OnClickedInfo();

	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
public:
	void ChangeFocusedSlot(MultiLevelRank InRank);

protected:
	void ForceAutoScroll(TWeakObjectPtr<UWidget> InWidget);
	void SetIsSelected(TWeakObjectPtr<UWidget> InWidget, bool IsSelected);
	void SetFocusToForce(const FGsMultiLevelRankSlotData* InFocusData);

	/************************************************************************/
	/* Invalidate...														*/
	/************************************************************************/
public:
	void InitializeSlotAll();
	void InvalidateSlotAll();
	void InvalidateInfo();

	void InvalidateQuest(MultiLevelRank InRank);
	

	/************************************************************************/
	/* Æ©Åä¸®¾ó																*/
	/************************************************************************/
protected:
	bool _bIsTutorial = false;

public:
	DECLARE_DELEGATE_OneParam(FOnTutorialListEvent, UWidget*);
	FOnTutorialListEvent OnTutorialListEvent;

public:
	void SetTutorial(bool bIsTutorial);

protected:
	bool IsTutorial() const { return _bIsTutorial; }
	void CheckTutorialTargetList();

	UFUNCTION()
	void OnScrollMain(float InCurrentOffset);
};
