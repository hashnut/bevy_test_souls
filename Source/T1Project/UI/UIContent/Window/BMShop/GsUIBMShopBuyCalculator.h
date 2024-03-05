#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIBMShopBuyCalculator.generated.h"

class UTextBlock;
class UGsButton;
class UGsUIBMShopBuyPrice;
class FGsBMShopCostInfo;
class FGsBMShopProductData;

const int32 ADD_VALUE = 5;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeCountDelegate, const uint16);

UCLASS()
class UGsUIBMShopBuyCalculator final : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBMShopBuyPrice* _currentCost;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnReset;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAddAmountFive;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAddAmountTen;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMin;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPlus;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _currentAmountText;

private:
	uint16 _currentCount = 0;
	FGsBMShopCostInfo* _costInfo = nullptr;
	int32 _maximumCount;

public:
	FOnChangeCountDelegate _onChangeCountDelegate;

protected:
	void NativeOnInitialized() final;
	void NativeConstruct() final;
	void NativeDestruct() final;

public:
	void SetData(const FGsBMShopProductData* InProductData, const int32 InMaximumCount);

private:
	void UpdateData();

protected:
	UFUNCTION()
	void OnClickReset();
	UFUNCTION()
	void OnClickAddFive();
	UFUNCTION()
	void OnClickAddTen();
	UFUNCTION()
	void OnClickMin();
	UFUNCTION()
	void OnClickPlus();
};
