#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "GsUIPopupCeiling.generated.h"

class UTextBlock;
class UPanelWidget;
class URichTextBlock;
class UGsButton;
class UGsSwitcherButton;
class UGsHorizontalBoxIconSelector;
class UGsUIIconBase;
class UGsUIFairyComposeRes;
class UGsDynamicIconSlotHelper;

struct FGsSchemaItemCommon;

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCeiling : public UGsUIPopup
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnCeilingStart, uint32, uint8);
	DECLARE_DELEGATE(FOnCeilingCenceled);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUITab", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeEpic;

	UPROPERTY(BlueprintReadOnly, Category = "GsUITab", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeUnique;

	UPROPERTY(BlueprintReadOnly, Category = "GsUITab", meta = (BindWidget))
	UGsSwitcherButton* _btnGradeLegend;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _ceilingPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIFairyComposeRes* _ItemCeilingResult;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textceilingRate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPrev;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNext;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textLayer;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _resourceIconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FLinearColor _colorItemName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FLinearColor _colorItemAmount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemRequireCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _ceilingDesc1;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textCeilingDesc2;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCeilingInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSummon;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;

	FGsToggleGroup _toggleGroupGrade;

public:
	FOnCeilingStart OnCelingStart;
	FOnCeilingCenceled OnCelingCanceled;

protected:
	virtual bool OnBack() override;
	virtual void OnInputCancel() override;

protected:
	int32 _selectIndex{ 0 };
	uint8 _selectLayer{ 0 };

	int32 _ownItemCount{ 0 };
	int32 _usableOwnCount{ 0 };
	int32 _maxCeilingAmount{ 0 };
	TArray<int> _needCelingItemAmount;

	bool _runCeiling{ false };

protected:
	const FGsSchemaItemCommon* _ceilingMaterial{ nullptr };

	TArray<TWeakObjectPtr<UGsUIWidgetBase>> _childwidgets;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnTabGradeChanged(int32 index);

	UFUNCTION(BlueprintCallable)
	void OnClickClose();

	UFUNCTION(BlueprintCallable)
	void OnClickSummon();

	UFUNCTION()
	void OnRefreshMaterialIcon(int32 InIndex, UGsUIIconBase* InIcon);

	UFUNCTION()
	void OnPrevLayer();

	UFUNCTION()
	void OnNextLayer();

	void OnLongPressIcon(UGsUIIconItem& InIcon);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void OnSelectedTab(int32 index);
	virtual void OnStartSummonCeiling() { _runCeiling = true;  }

protected:
	void SetSelectLayer(uint8 layer);

public:
	virtual void BeginDestroy() override;
	virtual void OnCeilingResult(bool success);
};

