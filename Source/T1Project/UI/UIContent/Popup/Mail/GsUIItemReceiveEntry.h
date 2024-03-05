// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIItemReceiveEntry.generated.h"

class UGsUIIconItem;
class UGsUIIconBase;
class UGsHorizontalBoxIconSelector;
class UGsUIIconItemReward;
class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIItemReceiveEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _nameText;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:
	UGsUIIconItemReward* GetIcon();
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

public:
	void SetCurrencyData(CurrencyType InType, Currency InAmount);
	void SetItemData(const uint32 InTID, const ItemAmount InAmount);
	void SetEquipItemData(const uint32 InTID, const ItemAmount InAmount, const int32 InEnchantLevel = 0, const uint8 InEnchantShieldCount = UNCONFIRMED_ENCHANT_SHIELD, const uint8 InEnchantBonusIndex = UNCONFIRMED_ENCHANT_BONUS_INDEX);

	const FText& GetItemName(const uint32 InTID) const;
	FText GetCurrencyName(CurrencyType InType) const;
};
