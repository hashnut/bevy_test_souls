#pragma once

#include "CoreMinimal.h"
#include "GsUIPopupMonsterBookExpItem.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPopupMonsterBookUsedItem.generated.h"

class UGsButton;
class UPanelWidget;
class UGsUITooltipDesc;
class UGsDynamicPanelSlotHelper;

UCLASS()
class T1PROJECT_API UGsUIPopupMonsterBookUsedItem : public UGsUIPopupMonsterBookExpItem
{
	GENERATED_BODY()

	enum class OPTION_FLAG
	{
		PRIORITY = 1 << 0,
		BOOKGRADE = 1 << 1,
	};

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _panelBtnCheckBoxGroupHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _checkGroupBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _checkBoxEntryClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _panelBtnSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _btnHorizontalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _btnSlotEntryClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipWithLifeTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnApply;

protected:
	FGsToggleGroup _btnTogglePriority;

	uint64 _savedOption = 0;

public:
	// 2022/12/27 PKT - 상태를 관리자에게 알릴 필요가 있을 경우 호출 됨.
	DECLARE_DELEGATE_OneParam(FOnSavedOptionDelegate, uint64);
	FOnSavedOptionDelegate OnSavedOptionDelegate;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void ImportOption(uint64 InParam);
	void ExportOption(uint64& OutParam);

	bool IsSelectedCheckBox();
	uint64 SetDefaultCheckBox();

protected:
	UFUNCTION()
	void OnCancel();

	UFUNCTION()
	void OnApply();

	UFUNCTION()
	void OnRefreshCheckBox(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnRefreshTabButton(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickedCheckBox(bool bIsChecked, int32 InIndexInGroup);

	UFUNCTION()
	void OnClickedTooltip();
	
public:
	void SetData(uint64 InParam = 0);	

	virtual void Invalidate() override;

protected:
	virtual void OnInputCancel() override;
};
