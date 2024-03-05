#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Item/Craft/GsCraftTargetObject.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUICraftExpectedListItem.generated.h"

class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
class UGsUIIconBase;
class UGsUIIconItem;
class UTextBlock;
class UWidgetSwitcher;

/**
 * 아이템 제작 예상 목록 아이템
 */
UCLASS()
class UGsUICraftExpectedListItem : public UUserWidget
{
	
	GENERATED_BODY()

public:
	struct Parameters
	{
		CraftResult _craftResult;
		FGsCraftTargetObject _craftTargetObject;
		ItemAmount _count; //타입 수정 : longlong -> int32 android package error
		int32	   _enchant;
	};

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GsUI", meta = (BindWidget))
	TMap<CraftResult, int32> _switcherResultTypeMap;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherResultType;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtTitle;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _slotHelperIcon;

	Parameters _parameters;

protected:
	void NativeOnInitialized() override;

	void NativeDestruct() override;

	/************************************************************************/
	/* setter																*/
	/************************************************************************/
	
public:
	void SetParameters(Parameters InParameters);

	/************************************************************************/
	/* event																*/
	/************************************************************************/

private:
	UFUNCTION()
	void OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon);
	
	void OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon);
};
