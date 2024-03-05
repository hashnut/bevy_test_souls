#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUITitleCollectionEntry.generated.h"


class UGsUIVFX;
class UTextBlock;
class UVerticalBox;
class UWidgetSwitcher;
class UGsUIRedDotDefault;
class FGsTitleRewardData;
class UGsDynamicPanelSlotHelper;


UCLASS()
class T1PROJECT_API UGsUITitleCollectionEntry : public UUserWidget
{
	GENERATED_BODY()

private:
	DECLARE_DELEGATE(FOnFinishedVFX);

	const static int32 ACTIVE_WIDGET_INDEX = 0;
	const static int32 INACTIVE_WIDGET_INDEX = 1;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _vfxChangedEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textStatCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSymbol;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBoxStat;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperStatData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfStatEntry;

	const FGsTitleRewardData* _data = nullptr;

	FOnFinishedVFX _OnFinishedVFX;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void FinishedVFX();
	void SetActivate(bool InIsActive);

private:
	UFUNCTION()
	void OnRefreshStatEntry(int32 InIndex, class UWidget* InEntry);

public:
	void SetData(const class FGsTitleRewardData* InData, bool InIsConfirm);

	void PlayEffect(bool InIsActive);

	const FGsTitleRewardData* GetData() const;

	FOnFinishedVFX& OnFinishedVFX();
};
