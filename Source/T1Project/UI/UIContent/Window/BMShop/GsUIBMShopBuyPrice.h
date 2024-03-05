#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIBMShopBuyPrice.generated.h"

class UGsUICurrencySlot;
class UPanelWidget;
class UWidget;

UCLASS()
class UGsUIBMShopBuyPrice final : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _totalAmount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _ownedAmount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _discountAmount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _discountRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidget* _ownedArea;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetCouponPriceData(const CurrencyType InType, const Currency InOriAmount, const Currency InDiscountAmount);
	void SetCouponPriceData(const uint32 InItemTid, const Currency InOriAmount, const Currency InDiscountAmount);

private:
	void SetDiscount(const CurrencyType InType, const Currency InDisCountAmount);
	void SetDiscount(const uint32 InItemTid, const Currency InDisCountAmount);

public:
	void SetData(const CurrencyType InType, const Currency InTotalAmount);
	void SetData(const int64 InItemTid, const Currency InAmount);

	void SetOwnedAmountData(const CurrencyType InType);
	void SetOwnedAmountData(const uint32 InItemTid);
	void UpdateCostState();
	void UpdateDiscountCostState(Currency InAmount);

	Currency GetDiscountAmount() const;
};
