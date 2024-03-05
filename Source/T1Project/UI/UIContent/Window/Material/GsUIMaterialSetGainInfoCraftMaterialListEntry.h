#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUIMaterialSetGainInfoCraftMaterialListEntry.generated.h"

class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
class UGsUIIconBase;
class UGsUIIconItem;
class UTextBlock;

UCLASS()
class UGsUIMaterialSetGainInfoCraftMaterialListEntry final : public UUserWidget, public IGsEntryInterface
{

	GENERATED_BODY()

public:
	struct Parameters final
	{
		CostType _costType;
		CurrencyType _currencyType;
		ItemId _itemId;
		Level _itemLevel;
		FText _name;
		uint64 _requireCount;
		uint64 _ownedCount;
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	UGsDynamicIconSlotHelper* _slotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtTitle;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtDesc;

private:
	Parameters _parameters;

protected:
	void NativeOnInitialized() final;

	void NativeDestruct() final;

	/************************************************************************/
	/* setter																*/
	/************************************************************************/

public:
	void SetParameters(Parameters InParamerters);

	/************************************************************************/
	/* event																*/
	/************************************************************************/

private:
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon);

	void OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon);

};
