// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"

#include "UI/UIContent/Common/Icon/GsUIIconItem.h"

#include "GsUIItemSupplyIconEntry.generated.h"

class UGsHorizontalBoxIconSelector;
class UTextBlock;
class UGsDynamicIconSlotHelper;
class UGsUIIconItemInventory;

UCLASS()
class T1PROJECT_API UGsUIItemSupplyIconItem : public UObject
{
	GENERATED_BODY()
	
public:
	struct Parameters
	{
		CostType _costType;
		CurrencyType _currencyType;
		ItemId _itemId;
		Level _itemLevel;
		int32 _amountMin;
		int32 _amountMax;
	};

private:
	Parameters _parameters;

public:
	Parameters GetParameters()
	{
		return _parameters;
	}

	void SetParameters(Parameters InParameters)
	{
		_parameters = MoveTemp(InParameters);
	}
};



/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIItemSupplyIconEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	/* 아이템 정보 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;

	/** 아이템 아이콘 */
	UPROPERTY()
	UGsUIIconItemInventory* _iconItemInventory;


protected:
	virtual void NativeOnInitialized() final;
	virtual void NativeConstruct() final;
	virtual void NativeDestruct() final;

	/** 실제 엔트리의 데이터 세팅 */
	void NativeOnListItemObjectSet(UObject* InItemData) final;

	void OnLongPressIcon(UGsUIIconItem& InIcon);
};
