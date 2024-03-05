#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "UI/UIContent/Define/GsUIContentDefine.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Message/GsMessageFairy.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageCostume.h"
#include "Message/GsMessageGameObject.h"

#include "GsUIPopupPreset.generated.h"

/*
	프리셋 정보 팝업
*/

class UGsButton;
class UGsSwitcherButton;
class UImage;
class UTextBlock;
class UGsUICostumeItem;
class UGsUIFairyItem;
class UGsUIPresetCopy;
class UGsUIPresetCostume;
class UGsUIPresetFairy;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UScrollBox;
class UGsUIIconItem;
class UGsUIPresetSkill;
struct IGsMessageParam;
struct FGsItemMessageParamBase;

UCLASS()
class T1PROJECT_API UGsUIPopupPreset : public UGsUIPopup
{
	GENERATED_BODY()

private:
	enum class EquipPageType
	{
		FIRST,
		SECOND,

		MAX,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCopy;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEditNameFirst;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEditNameSecond;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEditNameThird;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnPresetFirst;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnPresetSecond;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnPresetThird;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textFirst;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textSecond;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textThird;

	// 페어리, 코스튬
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPresetCostume* _uiCostume;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPresetFairy* _uiFairy;

	// 장착 장비 정보 - GsUIPlayerEquipInfo
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsSwitcherButton* _switcherButtonFirst;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsSwitcherButton* _switcherButtonSecond;

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UPanelWidget* _panelPage;

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelectorFirst;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelperFirst;

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelectorSecond;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelperSecond;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _equipSlotScrollBox;

	// 스킬 슬롯 정보
	UPROPERTY()
	TMap<int32, UGsUIPresetSkill*> _uiSkillMap;		// <SkillSlotId, UI>

	// 복사 UI
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPresetCopy* _panelCopy;

protected:
	// 메시지핸들러
	MsgUIHandleArray _uiMsgHandlerList;
	TArray<TPair<MessageItem, FDelegateHandle>> _itemMsgHandlerList;
	TArray<TPair<MessageFairy, FDelegateHandle>> _fairyMsgHandlerList;
	TArray<TPair<MessageCostume, FDelegateHandle>> _costumeMsgHandlerList;
	MsgGameObjHandleArray _gameObjectMsgHandlerList;
	
	// 프리셋 UI 정보
	PresetId _infoSelectedPresetId = INVALID_PRESET_ID;

	// 장비
	EquipPageType _currentEquipPageType = EquipPageType::FIRST;
	TMap<EquipPageType, TArray<EGsEquipSlotType>> _equipSlotTypeList;
	bool _isScrolling = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void OnInputCancel() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

protected:
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnClickCopy();

	void OnClickEditName(PresetId InPresetId);
	UFUNCTION()
	void OnClickEditNameFirst();
	UFUNCTION()
	void OnClickEditNameSecond();
	UFUNCTION()
	void OnClickEditNameThird();

	void OnClickPresetInfo(PresetId InPresetId);
	UFUNCTION()
	void OnClickPresetFirst();
	UFUNCTION()
	void OnClickPresetSecond();
	UFUNCTION()
	void OnClickPresetThird();

protected:
	void InvalidateInfo();
	void InvalidatePresetName();
	void InvalidateEquip();
	void InvalidateSkill(const IGsMessageParam* InParam = nullptr);

	virtual void InvalidateAllInternal() override;

protected:
	void OnInvalidateAll(const IGsMessageParam* InParam);
	void OnInvalidateAll(FGsItemMessageParamBase& InParam);
	void OnInvalidatePresetName(const IGsMessageParam* InParam = nullptr);

	// 장비 UI 관련 함수
protected:
	UFUNCTION()
	void OnCreateIcon(UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIconFirst(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIconSecond(int32 InIndex, UGsUIIconBase* InIcon);
	void OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon, EquipPageType InType);
	void OnClickIcon(UGsUIIconItem& InIcon);
	void OnLongPressIcon(UGsUIIconItem& InIcon);
	UFUNCTION()
	void OnChangeMainScrollEvent(float InCurOffset);
	UFUNCTION()
	void OnClickPageFirst();
	UFUNCTION()
	void OnClickPageSecond();
};
