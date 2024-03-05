// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/ReuseIconSlot/GsUIReuseScroll.h"

#include "Message/GsMessageUI.h"
#include "Message/GsMessageInput.h"
#include "Message/GsMessageSystem.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "GsUIReuseScrollHUD.generated.h"

/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIReuseScrollHUD : public UGsUIReuseScroll
{
	GENERATED_BODY()

protected:
	// 상단 퀵슬롯 (확장)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _rootExpansion;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _spacerExpansion;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _firstPageRootPanelExpansion;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _secondPageRootPanelExpansion;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _thirdPageRootPanelExpansion;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _fourthPageRootPanelExpansion;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _fifthPageRootPanelExpansion;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _sixthPageRootPanelExpansion;

	// 하단 퀵슬롯 (기본)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _root;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _firstPageRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _secondPageRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _thirdPageRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _fourthPageRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _fifthPageRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _sixthPageRootPanel;

protected:
	UPROPERTY()
	TArray<UHorizontalBox*> _rootPanelList;

	UPROPERTY()
	TArray<UGsDynamicPanelSlotHelper*> _panelSlotHelperList;

	uint8 _slotIndex = 1;
	uint8 _maxSlotCount = 0;

public:
	virtual void BeginDestroy() override;

	virtual void UpdateManagerTick(float InDeltaTime);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void SetSlotMove(int32 InMoveTargetPageIndex, int32 InDestPageIndex) override;


public:
	void InvalidateAll();
	void InvalidateSlot(TArray<uint8>& InSlotIndexList);
	void InvalidateAllSlot();
	void InvalidateSlotList(TArray<uint8>& InSlotIndexList);
	void InvalidateAutoState(TArray<uint8>& InSlotIndexList);
	void OnChangeEditMode(const bool InIsEdit);
	void ResetSelection(const QuickSlotId InSlotID);
	void ResetAllSelection();
	void SetTargetPage(const EGsIconSlotCountType InTargetPageType);
	void SetSlotDimmed(const bool InState, TArray<ItemCategorySub> InDimmedTypeList);

	int32 GetFocusingPageIndex(const uint8 InSlotIndex);

protected:
	void OnTouchScreen(const IGsMessageParam* inParam);
	void OnOptionSaveCompleted(const IGsMessageParam*);
	void OnTouchRelease(const class FGsInputEventMsgBase& InMsg);


//----------------------------------------------------------------------------------------------------------------------
	// Tutorial
private:
	int32 _tutorialSlotIndex = -1;
	float _tutorialScrollOffset = 0.f;

private:
	TArray<MsgInputHandle> _inputMsgHandlerList;
	TArray<MsgInputHandle> _listInputDelegate;
	MsgSystemHandleArray _listSystemDelegate;
	TArray<TPair<MessageUI, FDelegateHandle>> _listUIDelegate;


	virtual void OnChangeMainScrollEvent(float InCurOffset) override;

	

	void CreateEntryReuseSlot();

private:
	void InitMsg();
	void RemoveMsg();

private:
	void InputKey(const uint8 InSlotNum);
	void InputKeyExpansion(const uint8 InSlotNum);
	void InputAutoMoveScroll(const uint8 InNum);

private:
	void OnWindowKeyMapping(const struct IGsMessageParam* InParam);

public:
	UFUNCTION()
	void OnCreateEntry(UWidget* InEntry);
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);

public:
	// 튜토리얼 시 현재 보고있는 페이지 강제 고정
	class UGsUIReuseItemIconSlotHud* SetTutorialTarget(int32 InSlotIndex);	
	bool IsTutorial() const { return (0 <= _tutorialSlotIndex) ? true : false; }
};
