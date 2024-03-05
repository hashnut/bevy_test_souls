#include "GsUIPopupPreset.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsPresetManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"

#include "Net/GsNetSendServiceWorld.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"

#include "UI/UIContent/Popup/Preset/GsUIPresetCopy.h"
#include "UI/UIContent/Popup/Preset/GsUIPresetCostume.h"
#include "UI/UIContent/Popup/Preset/GsUIPresetFairy.h"
#include "UI/UIContent/Popup/Preset/GsUIPresetSkill.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Popup/GsUIPopupCommonInput.h"

#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemEquipment.h"

#include "GameObject/Stat/GsStatHelper.h"

#include "Shared/Shared/SharedInclude/SharedDefine.h"


void UGsUIPopupPreset::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupPreset::OnClickClose);
	_btnCopy->OnClicked.AddDynamic(this, &UGsUIPopupPreset::OnClickCopy);
	_btnEditNameFirst->OnClicked.AddDynamic(this, &UGsUIPopupPreset::OnClickEditNameFirst);
	_btnEditNameSecond->OnClicked.AddDynamic(this, &UGsUIPopupPreset::OnClickEditNameSecond);
	_btnEditNameThird->OnClicked.AddDynamic(this, &UGsUIPopupPreset::OnClickEditNameThird);

	_btnPresetFirst->OnClicked.AddDynamic(this, &UGsUIPopupPreset::OnClickPresetFirst);
	_btnPresetSecond->OnClicked.AddDynamic(this, &UGsUIPopupPreset::OnClickPresetSecond);
	_btnPresetThird->OnClicked.AddDynamic(this, &UGsUIPopupPreset::OnClickPresetThird);

	_switcherButtonFirst->OnClicked.AddDynamic(this, &UGsUIPopupPreset::OnClickPageFirst);
	_switcherButtonSecond->OnClicked.AddDynamic(this, &UGsUIPopupPreset::OnClickPageSecond);

	_iconSlotHelperFirst = NewObject<UGsDynamicIconSlotHelper>(this);
	if (nullptr != _iconSlotHelperFirst)
	{
		_iconSlotHelperFirst->Initialize(_iconSelectorFirst);
		_iconSlotHelperFirst->OnCreateIcon.AddDynamic(this, &UGsUIPopupPreset::OnCreateIcon);
		_iconSlotHelperFirst->OnRefreshIcon.AddDynamic(this, &UGsUIPopupPreset::OnRefreshIconFirst);
	}
	_iconSlotHelperSecond = NewObject<UGsDynamicIconSlotHelper>(this);
	if (nullptr != _iconSlotHelperSecond)
	{
		_iconSlotHelperSecond->Initialize(_iconSelectorSecond);
		_iconSlotHelperSecond->OnCreateIcon.AddDynamic(this, &UGsUIPopupPreset::OnCreateIcon);
		_iconSlotHelperSecond->OnRefreshIcon.AddDynamic(this, &UGsUIPopupPreset::OnRefreshIconSecond);
	}

	_equipSlotScrollBox->SetAnimateWheelScrolling(false);
	_equipSlotScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::Never);
	_equipSlotScrollBox->SetAllowOverscroll(false);

	for (int32 i = 1; i <= MAX_COSTUME_SKILL_SLOT_ID; ++i)
	{
		// C_Skill_1 ~ 
		FString widgetName = TEXT("C_Skill_");
		widgetName.AppendInt(i);

		if (UGsUIPresetSkill* slot = Cast<UGsUIPresetSkill>(GetWidgetFromName(*widgetName)))
		{
			slot->SetSlotId(i);
			_uiSkillMap.Emplace(i, slot);
		}
	}
}

void UGsUIPopupPreset::NativeConstruct()
{
	Super::NativeConstruct();
	RegisterMessages();

	_infoSelectedPresetId = GSPreset()->GetCurrentPresetId();

	// 좌우 번갈아가며 저장
	TArray<EGsEquipSlotType> firstEquipSlotTypeList;
	firstEquipSlotTypeList.Emplace(EGsEquipSlotType::HELMET_TYPE);
	firstEquipSlotTypeList.Emplace(EGsEquipSlotType::WEAPON_TYPE);
	firstEquipSlotTypeList.Emplace(EGsEquipSlotType::ARMOR_TYPE);
	firstEquipSlotTypeList.Emplace(EGsEquipSlotType::BELT_TYPE);
	firstEquipSlotTypeList.Emplace(EGsEquipSlotType::SHOES_TYPE);
	firstEquipSlotTypeList.Emplace(EGsEquipSlotType::GLOVE_TYPE);
	firstEquipSlotTypeList.Emplace(EGsEquipSlotType::NECKLACE_TYPE);
	firstEquipSlotTypeList.Emplace(EGsEquipSlotType::SCARF_TYPE);
	firstEquipSlotTypeList.Emplace(EGsEquipSlotType::EAR_RING_LEFT_TYPE);
	firstEquipSlotTypeList.Emplace(EGsEquipSlotType::EAR_RING_RIGHT_TYPE);
	firstEquipSlotTypeList.Emplace(EGsEquipSlotType::RING_LEFT_TYPE);
	firstEquipSlotTypeList.Emplace(EGsEquipSlotType::RING_RIGHT_TYPE);
	_equipSlotTypeList.Emplace(EquipPageType::FIRST, firstEquipSlotTypeList);

	TArray<EGsEquipSlotType> secondEquipSlotTypeList;
	secondEquipSlotTypeList.Emplace(EGsEquipSlotType::CHARM_LEFT_TYPE);
	secondEquipSlotTypeList.Emplace(EGsEquipSlotType::CHARM_RIGHT_TYPE);
	secondEquipSlotTypeList.Emplace(EGsEquipSlotType::BRACELET_LEFT_TYPE);
	secondEquipSlotTypeList.Emplace(EGsEquipSlotType::BRACELET_RIGHT_TYPE);
	secondEquipSlotTypeList.Emplace(EGsEquipSlotType::DURAL_LEFT_TYPE);
	secondEquipSlotTypeList.Emplace(EGsEquipSlotType::SLOT_MAX);
	secondEquipSlotTypeList.Emplace(EGsEquipSlotType::MARBLE_LEFT_TYPE);
	secondEquipSlotTypeList.Emplace(EGsEquipSlotType::MARBLE_RIGHT_TYPE);
	secondEquipSlotTypeList.Emplace(EGsEquipSlotType::SLOT_MAX);
	secondEquipSlotTypeList.Emplace(EGsEquipSlotType::SLOT_MAX);
	secondEquipSlotTypeList.Emplace(EGsEquipSlotType::SLOT_MAX);
	secondEquipSlotTypeList.Emplace(EGsEquipSlotType::SLOT_MAX);
	_equipSlotTypeList.Emplace(EquipPageType::SECOND, secondEquipSlotTypeList);

	_currentEquipPageType = EquipPageType::FIRST;

	// 스크롤 설정
	_equipSlotScrollBox->OnUserScrolled.AddDynamic(this, &UGsUIPopupPreset::OnChangeMainScrollEvent);
	OnClickPageFirst();


	InvalidateAllInternal();
}

void UGsUIPopupPreset::NativeDestruct()
{
	_infoSelectedPresetId = INVALID_PRESET_ID;

	_panelCopy->Hide();

	_equipSlotTypeList.Empty();
	_equipSlotScrollBox->OnUserScrolled.RemoveDynamic(this, &UGsUIPopupPreset::OnChangeMainScrollEvent);

	UnregisterMessages();
	Super::NativeDestruct();
}

void UGsUIPopupPreset::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isScrolling && false == _equipSlotScrollBox->HasMouseCapture())
	{
		float scrollSizeValue = _equipSlotScrollBox->GetScrollOffsetOfEnd() / 2.f;
		if (scrollSizeValue >= _equipSlotScrollBox->GetScrollOffset())
		{
			OnClickPageFirst();
		}
		else
		{
			OnClickPageSecond();
		}
	}
}

void UGsUIPopupPreset::OnInputCancel()
{
	Close();
}

void UGsUIPopupPreset::RegisterMessages()
{
	FGsMessageHolder* msg = GMessage();

	_uiMsgHandlerList.Empty();
	_uiMsgHandlerList.Emplace(msg->GetUI().AddUObject(MessageUI::INVALIDATE_PRESET,
		this, &UGsUIPopupPreset::OnInvalidateAll));
	_uiMsgHandlerList.Emplace(msg->GetUI().AddUObject(MessageUI::INVALIDATE_PRESET_NAME,
		this, &UGsUIPopupPreset::OnInvalidatePresetName));

	_itemMsgHandlerList.Emplace(msg->GetItem().AddUObject(MessageItem::ADDITEMLIST,
		this, &UGsUIPopupPreset::OnInvalidateAll));
	_itemMsgHandlerList.Emplace(msg->GetItem().AddUObject(MessageItem::UPDATEITEM,
		this, &UGsUIPopupPreset::OnInvalidateAll));
	_itemMsgHandlerList.Emplace(msg->GetItem().AddUObject(MessageItem::REMOVEITEM,
		this, &UGsUIPopupPreset::OnInvalidateAll));

	_fairyMsgHandlerList.Emplace(msg->GetFairy().AddUObject(MessageFairy::UPDATED,
		this, &UGsUIPopupPreset::OnInvalidateAll));

	_costumeMsgHandlerList.Emplace(msg->GetCostume().AddUObject(MessageCostume::UPDATED,
		this, &UGsUIPopupPreset::OnInvalidateAll));

	_gameObjectMsgHandlerList.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT_AUTO,
		this, &UGsUIPopupPreset::InvalidateSkill));
}

void UGsUIPopupPreset::UnregisterMessages()
{
	FGsMessageHolder* msg = GMessage();

	for (MsgUIHandle& elemItem : _uiMsgHandlerList)
	{
		msg->GetUI().Remove(elemItem);
	}
	_uiMsgHandlerList.Empty();

	for (auto& elemItem : _itemMsgHandlerList)
	{
		msg->GetItem().Remove(elemItem);
	}
	_itemMsgHandlerList.Empty();

	for (auto& elemItem : _fairyMsgHandlerList)
	{
		msg->GetFairy().Remove(elemItem);
	}
	_fairyMsgHandlerList.Empty();

	for (auto& elemItem : _costumeMsgHandlerList)
	{
		msg->GetCostume().Remove(elemItem);
	}
	_costumeMsgHandlerList.Empty();
}

void UGsUIPopupPreset::OnClickClose()
{
	OnInputCancel();
}

void UGsUIPopupPreset::OnClickCopy()
{
	if (_panelCopy->IsOpened())
	{
		_panelCopy->Hide();
	}
	else
	{
		_panelCopy->Show();
	}
}

void UGsUIPopupPreset::OnClickEditName(PresetId InPresetId)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCommonInput"));
	if (widget.IsValid())
	{
		if (UGsUIPopupCommonInput* popupInput = Cast<UGsUIPopupCommonInput>(widget.Get()))
		{
			FText findText;
			FText::FindText(TEXT("PresetText"), TEXT("Preset_Copy_Textbox"), findText);
			FText presetName = FText::FromString(GSPreset()->GetPresetName(InPresetId));

			FText warningText;
			FText::FindText(TEXT("PresetText"), TEXT("Preset_Slot_NameChange_Rule"), warningText);
			
			popupInput->SetDescText(findText);
			popupInput->SetText(presetName);
			popupInput->SetWarningText(warningText);
			popupInput->SetMaxTextLength(MAX_CHAR_NAME_LEN, true);
			popupInput->SetCallbackFunc([InPresetId](const FString& InString)
				{
					FGsNetSendServiceWorld::SendReqUserPresetName(InPresetId, InString);
				});
		}
	}
}

void UGsUIPopupPreset::OnClickEditNameFirst()
{
	OnClickEditName(EGsPresetId::FIRST);
}

void UGsUIPopupPreset::OnClickEditNameSecond()
{
	OnClickEditName(EGsPresetId::SECOND);
}

void UGsUIPopupPreset::OnClickEditNameThird()
{
	OnClickEditName(EGsPresetId::THIRD);
}

void UGsUIPopupPreset::OnClickPresetInfo(PresetId InPresetId)
{
	if (_infoSelectedPresetId == InPresetId)
	{
		return;
	}

	_infoSelectedPresetId = InPresetId;

	InvalidateInfo();
}

void UGsUIPopupPreset::OnClickPresetFirst()
{
	OnClickPresetInfo(EGsPresetId::FIRST);
}

void UGsUIPopupPreset::OnClickPresetSecond()
{
	OnClickPresetInfo(EGsPresetId::SECOND);
}

void UGsUIPopupPreset::OnClickPresetThird()
{
	OnClickPresetInfo(EGsPresetId::THIRD);
}

void UGsUIPopupPreset::InvalidateInfo()
{
	FGsPresetManager* presetManager = GSPreset();
	if (nullptr == presetManager)
		return;

	const int32 SLOT_INDEX_SELECTED = _btnPresetFirst->_slotIndexSelected;
	const int32 SLOT_INDEX_UN_SELECTED = _btnPresetFirst->_slotIndexUnselected;
	_btnPresetFirst->SetSwitcherIndex((_infoSelectedPresetId == EGsPresetId::FIRST) ? SLOT_INDEX_SELECTED : SLOT_INDEX_UN_SELECTED);
	_btnPresetSecond->SetSwitcherIndex((_infoSelectedPresetId == EGsPresetId::SECOND) ? SLOT_INDEX_SELECTED : SLOT_INDEX_UN_SELECTED);
	_btnPresetThird->SetSwitcherIndex((_infoSelectedPresetId == EGsPresetId::THIRD) ? SLOT_INDEX_SELECTED : SLOT_INDEX_UN_SELECTED);

	_uiCostume->SetCostume(presetManager->GetCostumeId(_infoSelectedPresetId));
	_uiFairy->SetFairy(presetManager->GetFairyId(_infoSelectedPresetId));

	InvalidateEquip();
	InvalidateSkill();
}

void UGsUIPopupPreset::InvalidatePresetName()
{
	FGsPresetManager* presetManager = GSPreset();
	if (nullptr == presetManager)
	{
		return;
	}

	_textFirst = FText::FromString(presetManager->GetPresetName(EGsPresetId::FIRST));
	_textSecond = FText::FromString(presetManager->GetPresetName(EGsPresetId::SECOND));
	_textThird = FText::FromString(presetManager->GetPresetName(EGsPresetId::THIRD));
}

void UGsUIPopupPreset::InvalidateEquip()
{
	_iconSlotHelperFirst->RefreshAll(_equipSlotTypeList[EquipPageType::FIRST].Num());
	_iconSlotHelperSecond->RefreshAll(_equipSlotTypeList[EquipPageType::SECOND].Num());
}

void UGsUIPopupPreset::InvalidateSkill(const IGsMessageParam* InParam)
{
	ItemDBId weaponItemDBId = GItem()->GetEquipedItemDBIDBySlotType(_infoSelectedPresetId, EGsEquipSlotType::WEAPON_TYPE);
	CreatureWeaponType weaponType = CreatureWeaponType::NONE;

	TWeakPtr<FGsItem> weaponItem = GItem()->FindItem(weaponItemDBId, ItemType::EQUIP);
	if (weaponItem.IsValid())
	{
		const FGsItemEquipment* equipItem = weaponItem.Pin()->GetCastItem<FGsItemEquipment>();
		weaponType = equipItem->GetCreatureWeaponType();
	}

	const TMap<SkillSlotId, const FGsSkill*>* skillSlotData = GSkill()->GetSkillSlotMapByPresetId(weaponType,_infoSelectedPresetId);
	for (auto& uiSkill : _uiSkillMap)
	{
		UGsUIPresetSkill* uiSkillSlot = uiSkill.Value;
		const FGsSkill* skillData = nullptr;
		if (nullptr != skillSlotData && (*skillSlotData).Contains(uiSkill.Key))
		{
			skillData = (*skillSlotData)[uiSkill.Key];
		}
		
		uiSkillSlot->SetSkill(skillData, _infoSelectedPresetId);
	}
}

void UGsUIPopupPreset::InvalidateAllInternal()
{
	InvalidateInfo();
	InvalidatePresetName();

	_panelCopy->InvalidateCopy();
	_panelCopy->InvalidatePresetName();
}

void UGsUIPopupPreset::OnInvalidateAll(const IGsMessageParam* InParam)
{
	InvalidateAll();
}

void UGsUIPopupPreset::OnInvalidateAll(FGsItemMessageParamBase& InParam)
{
	InvalidateAll();
}

void UGsUIPopupPreset::OnInvalidatePresetName(const IGsMessageParam* InParam /*= nullptr*/)
{
	InvalidatePresetName();
	_panelCopy->InvalidatePresetName();
}

// 장비 UI 관련 함수
void UGsUIPopupPreset::OnCreateIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemCount(0);
	itemIcon->SetCoolTimeBg(false);
}

void UGsUIPopupPreset::OnRefreshIconFirst(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIcon(InIndex, InIcon, EquipPageType::FIRST);
}

void UGsUIPopupPreset::OnRefreshIconSecond(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIcon(InIndex, InIcon, EquipPageType::SECOND);
}

void UGsUIPopupPreset::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon, EquipPageType InType)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	if (_equipSlotTypeList.Contains(InType))
	{
		const TArray<EGsEquipSlotType>& slotTypes = _equipSlotTypeList[InType];
		if (slotTypes.IsValidIndex(InIndex))
		{
			EGsEquipSlotType slotType = slotTypes[InIndex];
			ItemDBId itemDBId = GItem()->GetEquipedItemDBIDBySlotType(_infoSelectedPresetId, slotType);
			if (INVALID_ITEM_DB_ID == itemDBId)
			{
				itemIcon->SetEmptyIcon();
				itemIcon->SetActiveIconBg(false);
				return;
			}

			if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
			{
				itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupPreset::OnLongPressIcon);
			}

			if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
			{
				itemIcon->OnClickSlot.AddUObject(this, &UGsUIPopupPreset::OnClickIcon);
			}

			itemIcon->SetOwnedItem(itemDBId);

			itemIcon->SetActiveIconBg(true);
			itemIcon->SetItemNameVisibility(false);
			itemIcon->SetSlotEquipedImage(false);
			itemIcon->SetPresetImage(false);
			itemIcon->SetRedDot(false);

			UGsGameUserSettings* userSetting = GGameUserSettings();
			if (nullptr == userSetting)
				return;

			uint8 value = userSetting->GetCombatSetting(EGsOptionCombat::INVENTROY_LONGPRESS_ENABLE);
			bool isUseLongPress = 0 == value ? false : true;

			itemIcon->SetItemNameToolTipHoverMode(true);
			itemIcon->SetItemNameToolTipFlag(isUseLongPress);
			itemIcon->SetOnChangeDisplayDetailWndType(true == isUseLongPress ?
				UGsUIIconItem::UseDisplayDetailWndType::LongPressType : UGsUIIconItem::UseDisplayDetailWndType::OnClickType);
		}
	}
	else
	{
		itemIcon->SetEmptyIcon();
		itemIcon->SetActiveIconBg(false);
	}
}

void UGsUIPopupPreset::OnClickIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	UGsGameUserSettings* userSetting = GGameUserSettings();
	if (nullptr == userSetting)
		return;

	uint8 value = userSetting->GetCombatSetting(EGsOptionCombat::INVENTROY_LONGPRESS_ENABLE);
	bool isUseLongPress = 0 == value ? false : true;
	if (false == isUseLongPress)
	{
		// 아이템 상세창 open
		FGsItem* item = itemIcon->GetItemIconData();
		FGsItemDetailOpenMessageParam param(item, false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos, true);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
	}
}

void UGsUIPopupPreset::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
		return;

	// 아이템 상세창 open
	FGsItem* item = itemIcon->GetItemIconData();
	FGsItemDetailOpenMessageParam param(item, false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos, true);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);


}

void UGsUIPopupPreset::OnChangeMainScrollEvent(float InCurOffset)
{
	if (false == _equipSlotScrollBox->HasMouseCapture())
	{
		float scrollSizeValue = _equipSlotScrollBox->GetScrollOffsetOfEnd() / 2.f;
		if (scrollSizeValue >= InCurOffset)
		{
			OnClickPageFirst();
		}
		else if (scrollSizeValue < InCurOffset)
		{
			OnClickPageSecond();
		}
	}

	_isScrolling = true;
}

void UGsUIPopupPreset::OnClickPageFirst()
{
	_switcherButtonFirst->SetSwitcherIndex(0);
	_switcherButtonSecond->SetSwitcherIndex(1);

	_equipSlotScrollBox->EndInertialScrolling();
	TWeakObjectPtr<UWidget> findWidget = _panelPage->GetChildAt(0);
	_equipSlotScrollBox->ScrollWidgetIntoView(findWidget.Get(), true, EDescendantScrollDestination::Center);

	_isScrolling = false;
}

void UGsUIPopupPreset::OnClickPageSecond()
{
	_switcherButtonFirst->SetSwitcherIndex(1);
	_switcherButtonSecond->SetSwitcherIndex(0);

	_equipSlotScrollBox->EndInertialScrolling();
	TWeakObjectPtr<UWidget> findWidget = _panelPage->GetChildAt(1);
	_equipSlotScrollBox->ScrollWidgetIntoView(findWidget.Get(), true, EDescendantScrollDestination::Center);

	_isScrolling = false;
}
