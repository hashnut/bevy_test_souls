#pragma once

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Seal/GsSealSlot.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Window/GsUIWindowSeal.h"

#include "GsUISealSlotListEntry.generated.h"

class UGsButton;
class UGsUIVFX;
class UTextBlock;
class UWidgetSwitcher;

UCLASS()
class UGsUISealSlotListItem final : public UObject
{
	friend class UGsUISealSlotListEntry;

	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClickSlotLockButton, const int32 /*InSlotIndex*/, const EGsSealRerollUnlockTarget /*InUnlockTarget*/)

private:
	int32 _slotIndex;

public:
	SealId _sealNodeId = INVALID_SEAL_ID;
	SealSlotId _sealSlotId = INVALID_SEAL_SLOT_ID;
	bool _activated = false;
	FText _header;
	FText _title;
	SealSlotColor _sealColor;
	bool _unlockStat = true;
	bool _unlockColor = true;
	bool _canRerollLock = true;
	FOnClickSlotLockButton _onClickUnlockStat;
	FOnClickSlotLockButton _onClickUnlockColor;

public:
	void SetSlotIndex(const int32 InSlotIndex);
	void ResetData();
	int32 GetCurrentSealRerollLockCount() const;
};

/**
 * 봉인 슬롯 리스트
 */
UCLASS()
class UGsUISealSlotListEntry final : public UUserWidget, public IGsUIWindowSealEvent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _headerTxt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _titleTxt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _colorSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _unlockStatBtn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _unlockStatSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _unlockColorBtn;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _unlockColorSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _casting;

	TWeakObjectPtr<UGsUISealSlotListItem> _listItem;

protected:
	void NativeOnInitialized() final;

public:
	void Invalidate(const bool InClearEffect);
	void SetListItem(TWeakObjectPtr<UGsUISealSlotListItem> InSealSlotListItem);

private:
	UGsUISealSlotListItem* GetListItem() const;

protected:
	UFUNCTION()
	void OnClickLockStatBtn();

	UFUNCTION()
	void OnClickLockColorBtn();

public:
	UFUNCTION()
	void OnUIEvent(const FGsUIWindowSealEventParameters& InEventParameters) final;
};
