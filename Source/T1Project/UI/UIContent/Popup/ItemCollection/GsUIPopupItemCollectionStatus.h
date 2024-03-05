#pragma once

#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupItemCollectionStatus.generated.h"

class UGsButton;
class UProgressBar;
class UTextBlock;
class UGsDynamicPanelSlotHelper;
class UWidgetSwitcher;

/**
 * 아이템 컬렉션 현황 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupItemCollectionStatus : public UGsUIPopup
{
	GENERATED_BODY()

public:
	struct PopupInitData
	{
		int32 _itemCollectionCompletedCount = 0;
		int32 _itemCollectionProgressingCount = 0;
		int32 _itemCollectionIncompleteCount = 0;

		int32 _itemCollectionItemSlotCompletedCount = 0;
		int32 _itemCollectionItemSlotIncompleteCount = 0;
	};
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressCollectionCompleted;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressCollectionProgressing;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCollectionCompletedCount;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCollectionCompletedRatio;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCollectionProgressingCount;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCollectionProgressingRatio;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCollectionIncompleteCount;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCollectionIncompleteRatio;


	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressItemCompleted;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textItemCompletedCount;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textItemCompletedRatio;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textItemIncompleteCount;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textItemIncompleteRatio;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelApplyStatList;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperApplyStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfApplyStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherApplyStatList;	

	TArray<TPair<FText, FText>> _applyEffectTextList;

public:
	void BeginDestroy() override;
protected:
	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnClickOk();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshEntryApplyStat(int32 InIndex, UWidget* InEntry);
};