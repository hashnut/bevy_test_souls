#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Currency/GsCostPackage.h"
#include "GsUIMaterialSetGainInfoCraftMaterialListEntry.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIMaterialSetGainInfoCraftMaterialList.generated.h"

class FGsCraftTarget;
class FGsItemCollectionCondition;
class UGsDynamicPanelSlotHelper;
class UPanelWidget;
class UScrollBox;
class UWidget;
class UWidgetSwitcher;

UCLASS()
class UGsUIMaterialSetGainInfoCraftMaterialList final : public UUserWidget
{

	GENERATED_BODY()

public:
	struct Parameters final
	{

		TSharedPtr<const FGsCostPackage> _costPackage;

	};

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scroll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootList;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	UGsDynamicPanelSlotHelper* _slotHelper;

private:
	Parameters _parameters;

	/************************************************************************/
	/* overrides															*/
	/************************************************************************/

protected:
	void NativeOnInitialized() final;

	void NativeConstruct() final;

	void NativeDestruct() final;

	/************************************************************************/
	/* behavior                                                             */
	/************************************************************************/

private:
	void ResetAllDatas();

	/************************************************************************/
	/* setter																*/
	/************************************************************************/

public:
	void SetParameters(Parameters InParameters);

	/************************************************************************/
	/* event                                                                */
	/************************************************************************/

private:

	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);

};
