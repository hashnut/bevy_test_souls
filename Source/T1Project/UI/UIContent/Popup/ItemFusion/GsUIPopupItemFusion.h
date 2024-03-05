#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../UI/UILib/Base/GsUIPopup.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupItemFusion.generated.h"


UCLASS()
class T1PROJECT_API UGsUIPopupItemFusion : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsWrapBoxInventoryIconSelector* _iconSrcItemSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsWrapBoxInventoryIconSelector* _iconDestItemSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsWrapBoxInventoryIconSelector* _iconFusionItemSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textSrcItemName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textDestItemName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textFusionItemName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelSelectedStatGuide;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherRefineSelectorPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSrcRefineInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSrcRefineInfo;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgSelectedSrcRefineImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UVerticalBox* _verticalSrcRefineStatSet;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _listSrcRefineStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryRefineSrcStatEntry;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherDestRefineInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnDestRefineInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgSelectedDestRefineImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UVerticalBox* _verticalDestRefineStatSet;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _listDestRefineStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryRefineDestStatEntry;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textGuide;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencyButton* _btnCurrency;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIInventoryCommon* _invenUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _animationEffectPanel;

protected:
	// 2023/10/10 PKT - Fusion Handler Logic..
	class FGsItemFusionHandler* _fusionHandler = nullptr;

	// 2023/10/10 PKT - Invnetory Logic
	class FGsInventoryLogicItemFusion* _inventoryData = nullptr;

	// 2023/10/10 PKT - Animation's
	class UWidgetAnimation* _idleAnimation = nullptr;
	class UWidgetAnimation* _FusionAnimation = nullptr;

	// 2023/10/10 PKT - 합성 Animation 종료 시점에 호출
	FWidgetAnimationDynamicEvent _fusionAnimationFinishDelegate;

public:
	virtual void BeginDestroy() override;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void OnInputCancel() override;

	void PlayEffect(UWidgetAnimation* InAnimaiton, bool InIsLoop);
	void StopEffect(UWidgetAnimation* InAnimaiton);
	bool IsPlayEffect(UWidgetAnimation* InAnimaiton);

	class UGsUIIconItemInventory* GetIcon(class UGsWrapBoxInventoryIconSelector* InIconSelector);

	void FillIconWithItemData(class UGsWrapBoxInventoryIconSelector* OutIconSelector, class UTextBlock* OutText, ItemDBId InItemDBId);
	void FillStatUIWithItemData(class FGsItemEquipment* InItemEquipmentData, int32 InIndex, class UGsUIItemFusionRefineStat* OutWidget);

protected:
	UFUNCTION()
	void OnClickedTryFusion();

	UFUNCTION()
	void OnClickedClose();	

	UFUNCTION()
	void OnClickedInventory(uint64 InItemDBId);

	UFUNCTION()
	void OnRefreshSrcRefineStatEntry(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnRefreshDestRefineStatEntry(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickedSrcRefine();

	UFUNCTION()
	void OnClickedDestRefine();

	void OnSelectedMaterialItemIcon(class UGsUIIconItem& InIcon);

public:
	UFUNCTION()
	void OnFusionAnimationFinish();

public:
	void SetData(class FGsItemFusionHandler* InItemFusionHandler);

	void Invalidate();

	void ActionFusionEffect(const ItemDBId InFusionItemDBId);
};
