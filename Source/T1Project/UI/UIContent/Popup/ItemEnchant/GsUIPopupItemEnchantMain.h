// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "Unlock/GsUnlockDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageContents.h"
#include "GsUIPopupItemEnchantMain.generated.h"

class FGsItem;

class UImage;
class UTextBlock;
class UGsButton;
class UCanvasPanel;
class FGsToggleGroup;
class UWidgetSwitcher;
class UGsSwitcherButton;
class UGsUICurrencyButton;
class UGsUIItemEnchantGroup;
class UGsUIItemMultiEnchantGroup;
class UGsUIItemRefineGroup;
class UGsUIItemMagicalForgeGroup;
class UGsUIInventoryCommon;
class FGsInventoryLogicEnchantInven;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupItemEnchantMain : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIInventoryCommon* _invenUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemEnchantGroup* _singleEnchantUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemMultiEnchantGroup* _multiEnchantUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemRefineGroup* _refineOptionUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemMagicalForgeGroup* _magicalForgeUI;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _enchantTypeSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _enchantSwitcherBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _multiEnchantSwitcherBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _refineSwitcherBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _magicalForgeSwitcherBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _magicalForgeTabDeco;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _bgImage;

private:
	FGsToggleGroup _toggleGroupTab;

protected:
	FGsInventoryLogicEnchantInven* _invenLogic = nullptr;

private:
	EGsItemEnchantActionType _enchantActionType = EGsItemEnchantActionType::ACTION_MAX;
	TMap<EGsItemEnchantActionType, EGsUnlockContentsType> _unlockTypeMap;

private:
	TPair<MessageUserInfo, FDelegateHandle> _handlerUserInfo;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemDelegate;
	TArray<TPair<MessageContentEnchant, FDelegateHandle>>	_listItemEnchantDelegate;
	TArray<TPair<MessageContentUnlock, FDelegateHandle>> _listUnlockDelegate;


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;
	//virtual bool OnBack() override;

public:
	virtual bool GetPreventAutoClose() const override { return true; }

private:
	void BindMessages();
	void RemoveMessage();
	void InvalidateInvenDimmedState();
	void InvalidateInvenSelection();

private:
	void OnSelectInvenTab(int32 InIndex);
	void ResetContentsInfo();

protected:
	void UpdateSingleItemEnchantInfo(const struct IGsMessageParam* InMessageParam);
	void UpdateMultiItemEnchantInfo(const struct IGsMessageParam* InMessageParam);
	void UpdateRefineOptionEnchantInfo(const struct IGsMessageParam* InMessageParam);
	void UpdateMagicalForgeEnchantInfo(const struct IGsMessageParam* InMessageParam);

	void OnAddItemList(struct FGsItemMessageParamBase& InParam);
	void OnUpdateItem(struct FGsItemMessageParamBase& InParam);
	void OnRemoveItem(struct FGsItemMessageParamBase& InParam);
	void OnUpdateCurrency(uint64 InData);
	void OnContentsUnlock(const IGsMessageParam* InParam);

	void CheckUnlock();

private:
	void OnClickInventoryItem(uint64 InItemDBId);
	void OnClickEnchantMainSlot(const bool InTarget, const ItemDBId InItemDbid);
	void OnClickMultiEnchantEquipSlot();
	void OnClickMultiEnchantResetBtn();
	void OnClickMultiEnchantRegisterBtn();

public:
	void SetEnchantInfo(const ItemDBId InTargetItemDbid, const EGsItemEnchantActionType InEnchantActionType, const bool InIsEquipment);
	void UpdateEnchantInfo(const ItemDBId InTargetItemDbid, const EGsItemEnchantActionType InEnchantActionType);
	void SetAllIconDimmed(const bool InState);
	void SetBG(const bool InUseBg);

private:
	TArray<ItemDBId> GetSelectedItemDbidList(const EGsItemEnchantActionType InCurType);

	ItemEffectType GetIngredientEffectType(const EGsItemEnchantActionType InActionType);

private:
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnSelectedTab(int32 inIndex);
	UFUNCTION()
	void OnClickBlockedTab(int32 InIndex);


//----------------------------------------------------------------------------------------------------------------------
//Tutorial
public:
	DECLARE_DELEGATE_OneParam(FOnTutorialListEvent, UWidget* /*TargetWidget*/);
	FOnTutorialListEvent OnTutorialListEvent;

public:
	void SetTutorial(uint64 InItemDBId); // 스크롤 락걸기, 대상 위젯이 정렬되어 준비되면 OnTutorialListEvent로 가져가기
	void OnTutorialSettingComplete(UWidget* InTargetWidget);
};
