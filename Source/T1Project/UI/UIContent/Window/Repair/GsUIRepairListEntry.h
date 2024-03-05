// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIRepairListEntry.generated.h"


class UImage;
class UTextBlock;
class UGsButton;
class UGsHorizontalBoxIconSelector;
class UGsUIIconItemInventory;
class FGsItem;
struct FGsSchemaItemCommon;
/**
 * 합성강화 복구 시스템
 * 장비 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUIRepairListEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
	
	DECLARE_DELEGATE_OneParam(FOnClickedItemSlot, UUserWidget*);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageFocus;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY()
	UGsUIIconItemInventory* _uiIcon;

public:
	FOnClickedItemSlot OnClickedItemSlot;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnClickItemSlot();

public:
	void SetData(const FGsSchemaItemCommon* InItemTableData, Level InItemLevel, EnchantBonusIndex InEnchantBonusIndex, ItemDBId InItemDBId);
	void SetIsSelected(bool InIsSelected);

	const FGsItem* GetItemData();
};
