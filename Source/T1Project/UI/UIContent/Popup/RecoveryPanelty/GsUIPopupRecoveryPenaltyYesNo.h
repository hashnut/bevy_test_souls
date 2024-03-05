// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "GsUIPopupRecoveryPenalty.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupRecoveryPenaltyYesNo.generated.h"

/**
 * 부활 팝업
 */

USTRUCT()
struct FGsRestoreInfo
{
	GENERATED_BODY()

public:
	DeathPenaltyId penaltyId;
	CurrencyType currencyType;
	int32 cost;
};

UCLASS()
class T1PROJECT_API UGsUIPopupRecoveryPenaltyYesNo : public UGsUIPopup
{
	GENERATED_BODY()

private:
	EGsPenaltyType _penaltyType = EGsPenaltyType::Item;
	CurrencyType _currencyType = CurrencyType::DIA;
	Currency _playerCurrencyAmount = 0;
	Currency _currentCost = 0;
	TArray<DeathPenaltyExp> _expPenalty;
	TArray<DeathPenaltyItem> _itemPenalty;
	TWeakObjectPtr<UGsUIPopupRecoveryPenalty> _parents;
	TArray<FGsRestoreInfo> _restoreList;	
	FText _itemRecoveryText;
	FText _expRecoveryText;

	//Toggle
public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _penaltyText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _penaltyDetailText;
	UPROPERTY(meta = (BindWidget))
		class UGsButton* _recoveryButton;
	UPROPERTY(meta = (BindWidget))
		class UGsButton* _cancelButton;
	UPROPERTY(meta = (BindWidget))
		class UGsUICurrencySlot* _currentCurrencySlot;
	UPROPERTY(meta = (BindWidget))
		class UGsUICurrencySlot* _costCurrencySlot;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void Invalid();
	void InvalidCost();

public:
	virtual void OnInputCancel() override;

public:
	void Clear();
	void SetItemPenalty(class UGsUIPopupRecoveryPenalty* inPerents, TArray<DeathPenaltyItem>& inList);
	void SetExpPenalty(class UGsUIPopupRecoveryPenalty* inPerents, TArray<DeathPenaltyExp>& inList);
	UFUNCTION()
		void OnClickRecovery();
	UFUNCTION()
		void OnCancel();
	int32 GetCost(DeathPenaltyItem& inItem, CurrencyType inCurrency);
	int32 GetCost(DeathPenaltyExp& inItem, CurrencyType inCurrency);
	CurrencyType GetCurrencyType(EGsPenaltyCurrencyType inCurrencyType);
};
