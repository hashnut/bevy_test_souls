// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupRecoveryPenalty.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "GsRecoveryPenaltyListEntry.generated.h"

/**
 *
 */

 /**
 * RecoveryPanelty ListEntry에 전달할 파람
 * 코스트, 아이콘, 이름 정보 등 존재
 */

UCLASS()
class T1PROJECT_API UGsRecoveryPenaltyListEntryParam : public UObject
{
	GENERATED_BODY()

public:	
	TWeakObjectPtr<UGsUIPopupRecoveryPenalty> _parents;
	DeathPenaltyExp _expPenalty;
	DeathPenaltyItem _itemPenalty;

public:
	void Set(UGsUIPopupRecoveryPenalty* inParents, const DeathPenaltyExp& inExpPenalty)
	{
		_parents = inParents;
		_expPenalty = inExpPenalty;		
	}
	void Set(UGsUIPopupRecoveryPenalty* inParents, const DeathPenaltyItem& inItemPenalty)
	{
		_parents = inParents;
		_itemPenalty = inItemPenalty;
	}
};

UCLASS()
class T1PROJECT_API UGsRecoveryPenaltyListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	DeathPenaltyExp _expPenalty;
	DeathPenaltyItem _itemPenalty;
	TWeakObjectPtr<UGsUIPopupRecoveryPenalty> _parents;
	UPROPERTY()
		class UGsUIIconItemInventory* _uiIcon;

private:
	UPROPERTY(meta = (BindWidget))
		class UGsHorizontalBoxIconSelector* _iconSelector;
	UPROPERTY(meta = (BindWidget))
		class UImage* _expImg;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _penaltyText;
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* _condencyIconSwitcher;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _costText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _timeText;
	UPROPERTY(meta = (BindWidget))
		class UGsButton* _btnSlot;
	UPROPERTY(meta = (BindWidget))
		class UImage* _selectImg;

private:	
	UPROPERTY()
		class UGsDynamicIconSlotHelper* _iconHelper;
	FText _timeFormat;
	FText _lossExpAmountFormat;

private:
	FTimespan _timeSpan;
	FText _remainTimeText;
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(class UObject* inListItemObject) override;

private:
	void InvalidIcon();		
	void InvalidDetailInfo();

public:
	void InvalidCurrency();
	void InvalidRecoveryCost();
	void InvalidSelect();
	void InvalidTime();
	static int32 GetCost(const DeathPenaltyItem& inPenalty, const CurrencyType inCurrency);
	static int32 GetCost(const DeathPenaltyExp& inPenalty, const CurrencyType inCurrency);

private:
	UFUNCTION()
		void OnSelectPenalty();
		void OnLongPressIcon(class UGsUIIconItem& InIcon);
};
