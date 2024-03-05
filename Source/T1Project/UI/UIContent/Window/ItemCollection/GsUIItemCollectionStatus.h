#pragma once
#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIItemCollectionStatus.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;

UCLASS()
class T1PROJECT_API UGsUIItemCollectionStatus : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 2023/1/9 PKT - 필드 전체
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textFieldCompletedCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textFieldCompletedRatio;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textFieldProgressingCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textFieldProgressingRatio;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textFieldIncompleteCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textFieldIncompleteRatio;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textConditionCompletedCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textConditionCompletedRatio;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textConditionIncompleteCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textConditionIncompleteRatio;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelApplyStatList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfApplyStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherApplyStatList;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperApplyStat;

	TArray<TPair<FText, FText>> _textPassivitySet;

public:
	void BeginDestroy();
	
	void SetData();
protected:
	void NativeOnInitialized();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
};