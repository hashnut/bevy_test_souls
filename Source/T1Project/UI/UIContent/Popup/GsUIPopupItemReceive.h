// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "../Mail/GsMailInfo.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupItemReceive.generated.h"

class UGsButton;
class FGsMailManager;
class UGsDynamicPanelSlotHelper;
class UScrollBox;
struct FGsMailAttachment;
struct ItemDataCreated;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupItemReceive : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	// 확인 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnOk;	

	// 동적 슬롯 관리
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _itemScrollBox;

	// 아이콘 + 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _itemSlotClass;

protected:
	TMap<uint32, Currency> _currencyList;
	TMap<uint32, ItemAmount> _itemList;
	TArray<const ItemDataCreated*> _itemEquipList;
	TArray<TSharedPtr<FGsMailAttachment>> _mailItemEquipList;	

protected:
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;

	// UObject override
	virtual void BeginDestroy() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual	void OnClickOk();

	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

private:
	void Clear();

public:
	void SetData(IN TMap<uint32, Currency>& inCurrencyList, IN TMap<uint32, ItemAmount>& inItemList);
	void SetData(IN TArray<TSharedPtr<FGsMailAttachment>>& inMailDataList);
	void SetData(IN TArray<ItemDataCreated>& itemDBIdAmountPairList, IN TArray<CurrencyPair>& inCurrencyPairList);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
