#pragma once

#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupGuildItemCollectionStatus.generated.h"

class UGsButton;
class UGsDynamicPanelSlotHelper;
class UWidgetSwitcher;

/**
* file		GsUIPopupGuildItemCollectionStatus.h
* @brief	길드 아이템 콜렉션 스탯 현황
* @author	PKT
* @date		2021/10/05
**/

UCLASS()
class T1PROJECT_API UGsUIPopupGuildItemCollectionStatus : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

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

public:

	void InvalidateAll();
};