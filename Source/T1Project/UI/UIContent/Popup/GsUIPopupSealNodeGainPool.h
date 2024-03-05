#pragma once

#include "UI/UILib/Base/GsUIPopup.h"

#include "Seal/GsUISealNodeGainPoolListEntry.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIPopupSealNodeGainPool.generated.h"

class UGsButton;
class UGsDynamicPanelSlotHelper;
class UGsSwitcherButton;
class UGsUISealNodeGainPoolListEntry;
class UPanelWidget;
class UScrollBox;
class UTextBlock;
class UWidget;
class UWidgetSwitcher;

/**
 * 봉인슬롯에서 획득 가능한 색상과 스탯 목록을 출력하는 팝업
 */
UCLASS()
class UGsUIPopupSealNodeGainPool final : public UGsUIPopup
{
	GENERATED_BODY()

public:
	struct Parameters final
	{
		SealId _sealNodeId;
		Level _sealNodeLevel;
		Level _displayLevel;
	};

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UGsUISealNodeGainPoolListEntry> _subClassOfList;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _titleTxt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _titleSubTxt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _currentLevelSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _currentLevelTxt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _currentLevelSelectedTxt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _prevSwitcherBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _nextSwitcherBtn;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _colorListPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _statListPanel;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _colorListSlotHelper;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _statListSlotHelper;

private:
	/** 표 출력 데이터 */
	TMap<Level, TPair<TArray<TPair<SealSlotColor, int32>>, TArray<TTuple<StatType, StatValue, int32>>>> _sealRerollGainsSharePerLevel;
	TRange<Level> _displayLevelRange;

	/** 리스트 출력에 사용할 파라미터 목록 */
	TArray<UGsUISealNodeGainPoolListEntry::Parameters> _colorListItems;
	TArray<UGsUISealNodeGainPoolListEntry::Parameters> _statListItems;

	Parameters _parameters;

protected:
	void NativeOnInitialized() final;

	void NativeDestruct() final;

public:
	void SetParameters(Parameters InParameters);

private:
	void ResetUI();
	void Invalidate();

protected:
	UFUNCTION()
	void OnRefreshEntryColorList(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnRefreshEntryStatList(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickCloseBtn();
	
	UFUNCTION()
	void OnClickPrevBtn();
	
	UFUNCTION()
	void OnClickNextBtn();

public:
	static UGsUIPopupSealNodeGainPool* OpenThis(Parameters InParameters);

protected:
	void OnInputCancel() final;
};
