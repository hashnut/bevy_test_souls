#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Message/GsMessageContents.h"

#include "GameFlow/GameContents/GsContentsMode.h"

#include "GsUIPopupTrade.generated.h"


class UTextBlock;
class UGsButton;
class UGsSwitcherButton;
class UWidgetSwitcher;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UPanelWidget;
class UGsUICurrencySlot;
class UGsUIIconBase;
class UGsUIIconItem;
class UGsUIInventoryCommon;
class FGsInventoryLogicTrade;
struct IGsMessageParam;
/**
 * 개인 거래 팝업 UI
 */
UCLASS()
class T1PROJECT_API UGsUIPopupTrade : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	// 상단 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	// 상대 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockUserNameTarget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherStateTarget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelectorTarget;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelperTarget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelDiaTarget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _slotDiaTarget;

	// 내 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherStateLocal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelectorLocal;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelperLocal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelDiaLocal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDiaLocal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _slotDiaLocal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRemainDailyCount;

	// 하단 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _slotTradeCharge;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnLock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnConfirm;
	
	// 우측 인벤토리
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIInventoryCommon* _inventory;
	FGsInventoryLogicTrade* _inventoryLogic = nullptr;

protected:
	TArray<TPair<MessageContentPersoncalTrading, FDelegateHandle>> _msgTradeHandlerList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputCancel() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

protected:
	virtual void InvalidateAllInternal() override;
	void InvalidateTarget();
	void InvalidateLocal();
	void InvalidateInventory();

	void OnInvalidateUI(const IGsMessageParam* InParam);

protected:
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnClickLock();
	UFUNCTION()
	void OnClickConfirm();
	UFUNCTION()
	void OnClickDiaLocal();
	UFUNCTION()
	void OnClose();
	UFUNCTION()
	void OnCloseTradeChat();

	void OnFinishCalculate(int64 InValue);

	UFUNCTION()
	void OnCreateIcon(UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIconRemote(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIconLocal(int32 InIndex, UGsUIIconBase* InIcon);

	void OnClickIconTarget(UGsUIIconItem& InIcon);
	void OnClickIconLocal(UGsUIIconItem& InIcon);
	void OnLongPressIcon(UGsUIIconItem& InIcon);

	void OnContentsEnterStateEvent(FGsContentsMode::InGame InStateType);
};