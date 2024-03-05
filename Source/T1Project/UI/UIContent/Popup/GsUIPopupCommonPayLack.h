// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIPopupCommonPayLack.generated.h"

class UGsUICurrencySlot;
class URichTextBlock;
class UGsButton;
struct FGsSchemaCurrencyData;
/**
 * 재화 부족 알림 및 구매 유도 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCommonPayLack : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	URichTextBlock* _richTextDesc;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _currencySlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _space;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	CurrencyType _currencyType = CurrencyType::NONE;
	ItemId _itemId = INVALID_ITEM_ID;

	bool _bUseLink = false;

	// T: Yes, F: No
	TFunction<void(bool)> _buttonCallback;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

protected:
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();

public:
	void SetData(CurrencyType InCurrencyType, Currency InLackAmount, bool bInUseLink, TFunction<void(bool)> InCallback = nullptr);
	void SetItemData(ItemId InItemId, ItemAmount InLackAmount, bool bInUseLink, TFunction<void(bool)> InCallback = nullptr);
};
