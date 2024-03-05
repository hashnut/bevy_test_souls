#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "UI/UIContent/Define/GsUIContentDefine.h"

#include "GsUIPlayerEquipInfo.generated.h"


class UGsSwitcherButton;
class UGsDynamicIconSlotHelper;
class UGsButton;
class UGsUIIconBase;
class UGsUIIconItem;
class UWidgetAnimation;

UCLASS()
class UGsUIPlayerEquipInfo : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsSwitcherButton* _btnEquipFirst;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsSwitcherButton* _btnEquipSecond;

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UPanelWidget* _panelPage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelPageFirst;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelPageSecond;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelIconRootLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelIconRootRight;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelIconRootBMLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelIconRootBMRight;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animMoveToFirst;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animMoveToSecond;

protected:
	TArray<EGsEquipSlotType> _equipSlotList;
	EGsEquipSlotType _refreshSlotType = EGsEquipSlotType::SLOT_MAX;

	UPROPERTY()
	TArray<UGsDynamicIconSlotHelper*> _iconSlotHelperList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void FindIconSelecter(UPanelWidget* InPanel, OUT TArray<UGsDynamicIconSlotHelper*>& OutIconSlotHelperList);

protected:
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon);

	void OnClickIcon(UGsUIIconItem& InIcon);
	void OnLongPressIcon(UGsUIIconItem& InIcon);

	UFUNCTION()
	void OnClickPageFirst();
	UFUNCTION()
	void OnClickPageSecond();

public:
	void InvalidateSlotAll();
	void InvalidateSlot(EGsEquipSlotType InSlotType);
};