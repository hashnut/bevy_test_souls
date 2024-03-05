#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageItem.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../Message/MessageParam/GsItemMessageParam.h"
#include "GsUIPopupMonsterBookExpItem.generated.h"


class UWidget;
class UGsButton;
class UGsWrapBoxInventoryIconSelector;
class UGsDynamicIconSlotHelper;
class UTextBlock;
class FGsItem;
struct FGsItemMessageParamBase;


UCLASS()
class T1PROJECT_API UGsUIPopupMonsterBookExpItem : public UGsUIPopup
{
	GENERATED_BODY()

	const static int32 LAGRAGE_AMOUNT = 10;

	/************************************************************************/
	/* Bind                                                                 */
	/************************************************************************/
protected:
	// 2021/12/06 PKT - 초기화 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalcReset;

	// 2021/12/06 PKT - 최대치 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCalcMax;

	// 2021/12/06 PKT - 수량 차감
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDecrease;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDecreaseLarge;

	// 2021/12/06 PKT - 수량 증가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnIncrease;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnIncreaseLarge;

	// 2021/12/06 PKT - 현재 수량
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCurrentAnount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _deemedCalc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textDecreaseLarge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textIncreaseLarge;

	// 2021/12/06 PKT - Icon Item Invnetory M
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxInventoryIconSelector* _iconSelector;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;

protected:
	const static int32 UNSELECTED_INDEX = -1;
	int32 _selectedIndex = UNSELECTED_INDEX;

	const static int32 EMPTY_AMOUNT = 0;
	const static int32 DEFAULT_MIN_AMOUNT = 1;
	int32 _amountUsed = EMPTY_AMOUNT;

private:
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemActionDelegates;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


private:
	void OnAddItem(FGsItemMessageParamBase& InParam);
	void OnUpdateItem(FGsItemMessageParamBase& InParam);
	void OnRemoveItem(FGsItemMessageParamBase& InParam);

	UFUNCTION()
	void OnCreateItemIcon(class UGsUIIconBase* InIcon);

	UFUNCTION()
	void OnRefreshItemIcon(int32 InIndex, class UGsUIIconBase* InIcon);

	UFUNCTION()
	void OnClickedReset();

	UFUNCTION()
	void OnClickedMax();

	UFUNCTION()
	void OnClickedDecrease();

	UFUNCTION()
	void OnClickedIncrease();

	UFUNCTION()
	void OnClickedLargeDecrease();

	UFUNCTION()
	void OnClickedLargeIncrease();

	void OnSelectedItemIcon(class UGsUIIconItem& InIcon);
	void OnLongPressedItemIcon(class UGsUIIconItem& InIcon);

	void SetSelectedItem(int32 InIndex);

protected:
	virtual void OnBtnClickedChangedAmount() {}

	virtual void InvalidateAmount(int32 InAmountUsed);

public:
	virtual void Invalidate();
};
