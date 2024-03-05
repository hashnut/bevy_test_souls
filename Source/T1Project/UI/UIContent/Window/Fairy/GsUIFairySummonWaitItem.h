#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIFairySummonWaitItem.generated.h"

class UImage;
class UTextBlock;
class UGsButton;
class UPanelWidget;
class UScrollBox;
class UWidgetSwitcher;

class UGsUIFairyItem;
class UGsUICurrencyButton;
class UGsDynamicPanelSlotHelper;
class UGsUIPopupFairySkillTooltip;
class UGsUIFairySkillItem;

struct FGsFairySummonConfirmData;

UCLASS()
class T1PROJECT_API UGsUIFairySummonWaitItem : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
public:
	DECLARE_DELEGATE_OneParam(FOnSelected, TWeakPtr<FGsFairySummonConfirmData>);
	DECLARE_DELEGATE_OneParam(FOnSelectedCollection, FairyId);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIFairyItem* _fairyPortrait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _fairyName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _fairyLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnChange;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtRemainCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtRemainTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSummonConfirm;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnFindFairy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnViewCollection;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChangeInfo;

private:
	TWeakPtr<FGsFairySummonConfirmData> _data;
	bool _useCheck{ false };

public:
	FOnSelected OnSummonChange;
	FOnSelected OnSummonConfirm;
	FOnSelectedCollection OnCollectionPopup;
	FOnSelectedCollection OnFindFairyInfo;
	FOnSelected OnProbabilityPopup;
	FOnSelected OnTooltipSkillFairy;
	FOnSelected OnTooltipVehicleSkillFairy;

public:
	void BeginDestroy() override;

protected:
	void NativeOnInitialized() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickSummonChange();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickSummonConfirm();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickFindFairy();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickViewFairyCollection();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickFairyChangeInfo();

	void OnSelectedCollectionList(FairyId id);

	FSlateColor GetRemainAmountTextColor() const;
	FSlateColor GetRemainTimeTextColor() const;

public:
	void SetData(TWeakPtr<FGsFairySummonConfirmData> data)
	{
		_data = data;
	}
	void RefreshUI();
};