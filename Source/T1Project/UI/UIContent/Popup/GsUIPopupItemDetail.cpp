// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupItemDetail.h"
#include "Item/GsItemEquipment.h"
#include "UTIL/GsItemUtil.h"
#include "T1Project.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "T1Project/UI/UIContent/Helper/GsUIHelper.h"
#include "T1Project/UI/UIContent/Common/Item/GsUIItemDefaultInfo.h"
#include "T1Project/UI/UIContent/Common/Item/GsUIItemDetailInfo.h"
#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
#include "UI/UIContent/Popup/GsUIPopupDeleteConfirm.h"
#include "UI/UILib/Manager/GsTooltipManager.h"

#include "Item/GsItemHelper.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsUserProfileManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildItemCollectionData.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsEnchantMessageParam.h"
#include "Message/MessageParam/GsInvenMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsItemCollectionMessageParam.h"
#include "Message/GsMessageContents.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"

void UGsUIPopupItemDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickClose);

	_btnUse->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickUseItem);
	
	_btnEnchant->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickEnchant);
	_btnDisableEnchant->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickDisableEnchant);

	_btnDelete->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickDelete);
	_btnDisableDelete->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickDisableDelete);

	_btnDecompose->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickItemDecompose);
	_btnDisableDecompose->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickDisableItemDecompose);

	_btnItemCollection->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickItemCollection);
	_btnDisableItemCollection->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickDisableItemCollection);

	_btnMarket->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickMarket);
	_btnDisableMarket->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickDisableMarket);

	_btnCompare->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickItemCompare);
	_btnDisableCompare->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickDisableItemCompare);

	_btnComparePrev->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickNextCompare);
	_btnCompareNext->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickNextCompare);

	_personItemCollection->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnShotcutPersonItemCollection);
	_guildItemCollection->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnShotcutGuildItemCollection);

	_btnItemFusion->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickItemFusion);
	_btnItemFusionDisable->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickItemFusion);

	_btnSeveralUse->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnClickSeveralUseItem);

	_itemDetailInfo->OnChangeEnchantCount.BindUObject(this, &UGsUIPopupItemDetail::OnChangeEnchantCount);
}

void UGsUIPopupItemDetail::NativeConstruct()
{
	Super::NativeConstruct();

	_buttonsGroup1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_buttonsGroup2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	_collectionSelect->SetVisibility(ESlateVisibility::Collapsed);

	// 2023/4/18 PKT - Flag 초기화
	_isItemCollectionMarking = false;

	if (UGsUIManager* uiManager = GUI())
	{
		if (UGsTooltipManager* tooltipManager = uiManager->GetTooltipManager())
		{
			// 다른 이름 툴팁 닫기
			tooltipManager->CloseNameTooltip();
		}
	}
}

void UGsUIPopupItemDetail::NativeDestruct()
{
	_isHideButtonsFlag = false;
	_isHideButtonsBottomFlag = false;
	_isHideLockButtonFlag = false;
	_isOpenCompare = false;

	if (OnGetItemDataDelegate.IsBound())
	{
		OnGetItemDataDelegate.Unbind();
	}
	if (OnCloseItemDetailDelegate.IsBound())
	{
		OnCloseItemDetailDelegate.Clear();
	}
	if (OnItemDetailCompareDelegate.IsBound())
	{
		OnItemDetailCompareDelegate.Clear();
	}
	if (OnItemDetailCompareNextDelegate.IsBound())
	{
		OnItemDetailCompareNextDelegate.Clear();
	}

	Super::NativeDestruct();
}

void UGsUIPopupItemDetail::OpenDeleteCalculatorUI()
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (nullptr != uiManager->OpenAndGetWidget(TEXT("PopupCalculator")))
		{
			UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(uiManager->OpenAndGetWidget(TEXT("PopupCalculator")));

			FText findText;
			FText::FindText(TEXT("ContextUIText"), TEXT("calculator_dump"), findText);
			popup->SetDimmed(true);
			popup->SetTitleText(findText);
			popup->SetMaximumCount(GetItemAmount());
			popup->SetMinimumCount(1);
			popup->SetDefaultCount(GetItemAmount());
			popup->InvalidateNumText();

			if (false == popup->OnFinishCalculateDelegate.IsBoundToObject(this))
			{
				popup->OnFinishCalculateDelegate.AddUObject(this, &UGsUIPopupItemDetail::OnConfirmItemDeleteStackCount);
			}
		}
	}
}

void UGsUIPopupItemDetail::OpenDeleteConfirmUI(ItemAmount InAmount)
{
	if (!IsValidItem())
	{
		GSLOG(Warning, TEXT("!IsValidItem()"));
		return;
	}

	if (!IsOwnedItem())
	{
		GSLOG(Warning, TEXT("!IsOwnedItem()"));
		return;
	}

	if (UGsUIManager* uiManager = GUI())
	{
		if (nullptr != uiManager->OpenAndGetWidget(TEXT("PopupDeleteConfirm")))
		{
			UGsUIPopupDeleteConfirm* popup = Cast<UGsUIPopupDeleteConfirm>(uiManager->OpenAndGetWidget(TEXT("PopupDeleteConfirm")));
			popup->SetDeleteItemInfo(GetDisPlayItemData(), InAmount);
		}
	}
}

bool UGsUIPopupItemDetail::IsValidItem() const
{
	if (nullptr == GetDisPlayItemData())
		return false;

	const FGsItem* findData = GetDisPlayItemData();
	return FGsItem::IsValidItem(findData);
}

bool UGsUIPopupItemDetail::IsOwnedItem() const
{
	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return false;
	}

	return CheckOwnedItem();
}

// 창고와 인벤토리 외에 추가적으로 체크가 필요하면 추가하면됌
bool UGsUIPopupItemDetail::CheckOwnedItem() const
{
	if (false == IsValidItem())
		return false;

	ItemStorageType type = UGsItemUtil::FindItemStorageType(GetDisPlayItemData()->GetDBID());
	return (ItemStorageType::INVENTORY == type || ItemStorageType::DEPOT == type);
}

bool UGsUIPopupItemDetail::CanEnchantImmediately() const
{
	return IsOwnedItem() && GetDisPlayItemData()->IsEquipment() && !GetDisPlayItemData()->GetLock() && (false == GSUserProfile()->IsExistProfile()) && GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::ITEM_ENCHANT);
}

bool UGsUIPopupItemDetail::CanDeleteImmediately() const
{
	return IsOwnedItem() && !GetDisPlayItemData()->IsEquipped() && !GetDisPlayItemData()->GetLock() && GetDisPlayItemData()->IsCanDelete() && !GetDisPlayItemData()->IsPresetEquipped();
}

bool UGsUIPopupItemDetail::CanDecomposeImmediately() const
{
	return IsOwnedItem() && GetDisPlayItemData()->IsDecomposable() && !GetDisPlayItemData()->IsEquipped() && !GetDisPlayItemData()->GetLock() && !GetDisPlayItemData()->IsCashItem() && !GetDisPlayItemData()->IsPresetEquipped();
}

bool UGsUIPopupItemDetail::CanItemFusionImmediately() const
{
	bool isCanItemFusion = false;

	do 
	{
		FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
		if (nullptr == gameFlow)
		{
			break;
		}

		FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
		if (nullptr == contents)
		{
			break;
		}

		FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
		if (nullptr == hud)
		{
			break;
		}

		isCanItemFusion = (nullptr == hud->GetItemFusionHandler()) ? false : hud->GetItemFusionHandler()->IsSuitableForSrcItem(GetDisPlayItemData()->GetDBID());

	} while (0);

	return isCanItemFusion;
}

bool UGsUIPopupItemDetail::CanUseMarket() const
{
	// 내가 가진 아이템이 아니어도 거래소 접근해서 정보를 볼 수 있음. IsOwnedItem 체크 등 제거
	return (GetDisPlayItemData()->GetTradableItemIdInGroup() != INVALID_ITEM_ID && 
			GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MARKET));
}

bool UGsUIPopupItemDetail::IsExpiredTimeItem() const
{
	return IsOwnedItem() && GetDisPlayItemData()->IsExpiredTime();
}

ItemCategoryMain UGsUIPopupItemDetail::GetItemCategoryMain() const
{
	return IsValidItem() ? GetDisPlayItemData()->GetMainCategory() : ItemCategoryMain::NONE;
}

ItemType UGsUIPopupItemDetail::GetItemType() const
{
	return IsValidItem() ? GetDisPlayItemData()->GetType() : ItemType::NONE;
}

ItemCategorySub UGsUIPopupItemDetail::GetItemCategorySub() const
{
	return IsValidItem() ? GetDisPlayItemData()->GetSubCategory() : ItemCategorySub::NONE;
}

ItemDBId UGsUIPopupItemDetail::GetItemDBId() const
{
	return IsValidItem() ? GetDisPlayItemData()->GetDBID() : INVALID_ITEM_DB_ID;
}

ItemAmount UGsUIPopupItemDetail::GetItemAmount() const
{
	if (!IsValidItem())
	{
		GSLOG(Error, TEXT("!IsValidItem()"));
		return 0;
	}

	return GetDisPlayItemData()->GetAmount();
}

FText UGsUIPopupItemDetail::GetItemActionText() const
{
	if (!IsValidItem())
	{
		return FText::GetEmpty();
	}

	FTextKey key;
	key.Reset();
	if (true == GetDisPlayItemData()->IsUseItem())
	{
		key = TEXT("ItemIconUse");
	}
	else
	{
		if (GetDisPlayItemData()->IsEquipment())
		{
			key = false == GetDisPlayItemData()->IsEquipped() ? TEXT("ItemIconEquip") : TEXT("ItemIconUnequip");
		}
	}
	
	if (false == key.IsEmpty())
	{
		FText text;
		if (FText::FindText(TEXT("UICommonText"), key, text))
		{
			return text;
		}
	}
	return FText::GetEmpty();
}

void UGsUIPopupItemDetail::SetHideVisiblity(bool InState)
{
	SetVisibility(true == InState ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIPopupItemDetail::SetItemCollectionConditionItemOnly(bool InShowBtn, bool InEnchant, bool InChanged, bool InRegister, bool InIsBtnRegisterType)
{
	if (false == IsValidItem())
		return;

	const bool ownedState = IsOwnedItem();
	_isHideLockButtonFlag = !ownedState;
	_modeType = DisplayModeType::ItemCollectionMode;
		
	_itemDefaultInfo->SetData(GetDisPlayItemData(), true, _isHideLockButtonFlag);

	_buttonsGroup1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_buttonsGroup2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (false == InShowBtn)
	{
		_buttonsGroup2->SetVisibility(ESlateVisibility::Collapsed);
	}

	//  0 on , 1 off
	const static int32 ON_INDEX = 0;
	const static int32 OFF_INDEX = 1;
	_equipEnchantBtnSwitcher->SetActiveWidgetIndex(((true == InEnchant) && (CanEnchantImmediately())) ? 0 : 1);
	_itemCollectionBtnSwitcher->SetActiveWidgetIndex(ON_INDEX);
	_marketBtnSwitcher->SetActiveWidgetIndex(CanUseMarket() ? ON_INDEX : OFF_INDEX);
	
	// 0 itemDetail , 1 itemCompare , 2 ItemCollection
	_modeSwitcher->SetActiveWidgetIndex((uint8)_modeType);
	
	_isBtnRegisterType = InIsBtnRegisterType;
	
	//SetCollectionModeBtns(InChanged, InRegister);
}

void UGsUIPopupItemDetail::SetItemCollectionRegisterInfoText(const FText& InText)
{
	_itemDetailInfo->SetGuildItemCollectionRegisterInfo(InText);
}

void UGsUIPopupItemDetail::SetItemDefaultData(bool InHideButtons, DisplayModeType InModeType, bool InHideButtonsBottom, bool InUseLock, bool InIsItemCollectionMarking)
{
	if (false == IsValidItem())
		return;

	SetItemDefaultData(GetDisPlayItemData(), InHideButtons, InModeType, InHideButtonsBottom, InUseLock, InIsItemCollectionMarking);
}

void UGsUIPopupItemDetail::SetItemDefaultData(const FGsItem* InItem, bool InHideButtons, DisplayModeType InModeType /*= DisplayModeType::ItemDetailMode*/, bool InHideButtonsBottom /*= false*/, bool InUseLock /*= true*/, bool InIsItemCollectionMarking /*= false*/)
{
	if (nullptr == InItem)
		return;

	if (DisplayModeType::ItemCollectionMode == InModeType)
	{
		OnClickClose();
		return;
	}

	_isHideButtonsFlag = InHideButtons;
	_isHideButtonsBottomFlag = InHideButtonsBottom;
	_isItemCollectionMarking = InIsItemCollectionMarking;
	_isUseLock = InUseLock;

	const bool ownedState = IsOwnedItem();
	_isHideLockButtonFlag = !ownedState;
	_modeType = InModeType;

	_itemDefaultInfo->SetData(InItem, _isHideLockButtonFlag, _isUseLock, _isItemCollectionMarking);

	CheckBtnState();
}

void UGsUIPopupItemDetail::SetItemDetailData(bool InIsItemCollectionMarking /* = false */, bool InSkipUpdateDetailInfo /* = false */)
{
	if (InSkipUpdateDetailInfo)
		return;

	if (IsValidItem())
	{
		SetItemDetailData(GetDisPlayItemData(), InIsItemCollectionMarking, InSkipUpdateDetailInfo);
	}
}

void UGsUIPopupItemDetail::SetItemDetailData(const FGsItem* InItem, bool InIsItemCollectionMarking /*= false*/, bool InSkipUpdateDetailInfo /*= false*/)
{
	if (InSkipUpdateDetailInfo)
		return;

	if (nullptr == InItem)
		return;

	_isItemCollectionMarking = InIsItemCollectionMarking;

	_itemDetailInfo->SetData(InItem);
	_itemDetailInfo->SetItemCollectionMarking(InItem, _isItemCollectionMarking);
}

void UGsUIPopupItemDetail::SetCurrencyData(const CurrencyType InCurrencyType, const Currency InAmount, bool InHideButtons, DisplayModeType InModeType /*= DisplayModeType::ItemDetailMode*/)
{
	if (CurrencyType::NONE == InCurrencyType)
	{
		GSLOG(Warning, TEXT("CurrencyType::NONE == InCurrencyType"));
		return;
	}

	_isHideButtonsFlag = InHideButtons;
	_isHideButtonsBottomFlag = false;
	_isHideLockButtonFlag = true;
	_modeType = InModeType;

	_itemDefaultInfo->SetData(InCurrencyType, InAmount, _isItemCollectionMarking);

	CheckBtnState();

	_itemDetailInfo->SetData(InCurrencyType, InAmount);
	_itemDetailInfo->SetItemCollectionMarking(InCurrencyType, _isItemCollectionMarking);
}

void UGsUIPopupItemDetail::SetCompareValueVisibility(bool InState)
{
	_itemDetailInfo->SetCompareValueVisibility(InState);
}

void UGsUIPopupItemDetail::SetItemCompareDetailData(const FGsItem* InCompareItem)
{
	if (false == IsValidItem() || false == FGsItem::IsValidItem(InCompareItem))
		return;

	_itemDetailInfo->SetCompareData(GetDisPlayItemData(), InCompareItem);
}

void UGsUIPopupItemDetail::CheckBtnState()
{
	//  내가 소유하지 않은 아이템이 들어올 수 있음
	// 인벤에 있거나 창고에 있다면 내가 보유한 아이템이라고 봄
	const bool isOwnedItem = IsOwnedItem();
	_buttonsGroup1->SetVisibility(true == _isHideButtonsFlag ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	bool isHideButtonsGroupTwo = (true == _isHideButtonsFlag) || (false == isOwnedItem) || _isHideButtonsBottomFlag;

	//_buttonsGroup2->SetVisibility((isHideButtonsGroupTwo) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	_buttonsGroup2->SetVisibility((isHideButtonsGroupTwo) ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	if (true == _isHideButtonsFlag)
	{
		return;
	}
	
	//  0 on , 1 off
	_equipEnchantBtnSwitcher->SetActiveWidgetIndex(CanEnchantImmediately() ? 0 : 1);
	
	// 2021/10/29 PKT - 길드/개인 아이템 컬렉션 체크 
	bool isOnItemCollection = (false == GSUserProfile()->IsExistProfile()) &&
		((GSItemCollection()->IsValidMaterial(GetDisPlayItemData()) && GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::ITEM_COLLECTION)) ||
		GGuildItemCollection()->IsItemAvailableForItemCollection(GetDisPlayItemData()));
	_itemCollectionBtnSwitcher->SetActiveWidgetIndex(isOnItemCollection ? 0 : 1);
	
	// 거래소
	_marketBtnSwitcher->SetActiveWidgetIndex(CanUseMarket() ? 0 : 1);

	// 0 normal , 1 on compare , 2 disable compare
	if (!IsValidItem() || false == GetDisPlayItemData()->IsEquipment() || DisplayModeType::ItemCompareMode == _modeType)
	{
		_compareBtnSwitcher->SetActiveWidgetIndex(2);
	}
	else
	{	
		if (true == _isOpenCompare)
		{
			_compareBtnSwitcher->SetActiveWidgetIndex(1);
		}
		else if (GetDisPlayItemData()->IsEquipped())
		{
			_compareBtnSwitcher->SetActiveWidgetIndex(2);
		}
		else if(true == GetDisPlayItemData()->IsEquipment())
		{
			UGsItemManager* itemMgr = GItem();
			const FGsItemEquipment* equipItem = static_cast<const FGsItemEquipment*>(GetDisPlayItemData());
			if (nullptr == equipItem)
				return;

			bool isMultiSlot = itemMgr->IsMultiSlotItem(GetItemCategorySub());
			if (true == isMultiSlot)
			{
				// 멀티슬롯이면 해당 슬롯들에 아무 아이템이나 장착되어있으면 compare normal
				bool isEquipped = itemMgr->HasAnyEquippedMultiSlot(GetDisPlayItemData()->GetSubCategory());
				_compareBtnSwitcher->SetActiveWidgetIndex(true == isEquipped ? 0 : 2);
				
			}
			else
			{
				EGsEquipSlotType slotType = itemMgr->GetEquipSlotType(GetDisPlayItemData()->GetSubCategory(),0);
				TWeakPtr<FGsItem> equipped = itemMgr->GetPlayerEquipedItemBySlotType(slotType);
				_compareBtnSwitcher->SetActiveWidgetIndex(false == equipped.IsValid() ? 2 : 0);
			}
		}
	}
	
	// 0 itemDetail , 1 itemCompare , 2 ItemCollection
	_modeSwitcher->SetActiveWidgetIndex((uint8)_modeType); 

	switch (_modeType)
	{
	case UGsUIPopupItemDetail::ItemDetailMode:
		SetDetailModeBtns();
		_btnUse->SetVisibility(IsActiveEnableBtnState() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_btnUse->SetIsEnabled(IsEnableUseBtnState());
		_btnSeveralUse->SetVisibility(IsActiveSeveralBtnState() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		break;
	case UGsUIPopupItemDetail::ItemCompareMode:
		SetCompareModeBtns();
		_btnUse->SetVisibility(IsActiveEnableBtnState() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_btnUse->SetIsEnabled(IsEnableUseBtnState());
		_btnSeveralUse->SetVisibility(IsActiveSeveralBtnState() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		break;
	case UGsUIPopupItemDetail::ItemCollectionMode:
		//SetCollectionModeBtns(true,true);		
		break;
	default:
		_btnUse->SetVisibility(IsActiveEnableBtnState() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_btnUse->SetIsEnabled(IsEnableUseBtnState());
		_btnSeveralUse->SetVisibility(IsActiveSeveralBtnState() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		break;
	}	
}

void UGsUIPopupItemDetail::SetDetailModeBtns()
{
	//  0 on , 1 off
	_equipDeleteBtnSwitcher->SetActiveWidgetIndex(CanDeleteImmediately() ? 0 : 1);
	_equipDecomposeBtnSwitcher->SetActiveWidgetIndex(CanDecomposeImmediately() ? 0 : 1);
	_equipItemFusionSwitcher->SetActiveWidgetIndex(CanItemFusionImmediately() ? 0 : 1);

	// 사용 , 장착 , 해지 
	_textUseBtn->SetText(GetItemActionText());
}

void UGsUIPopupItemDetail::SetCompareModeBtns()
{
	if (!IsValidItem())
	{
		return;
	}

	if (false == GetDisPlayItemData()->IsEquipment())
		return;

	const FGsItemEquipment* equipItem = static_cast<const FGsItemEquipment*>(GetDisPlayItemData());
	if (nullptr == equipItem)
		return;

	const bool isMultiSlot = GItem()->IsMultiSlotItem(GetItemCategorySub());
	_btnComparePrev->SetVisibility(false == isMultiSlot ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	_btnCompareNext->SetVisibility(false == isMultiSlot ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	_textEquipped->SetVisibility(true == equipItem->IsEquipped() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_textItemNameCompare->SetVisibility(false == isMultiSlot ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	if (true == isMultiSlot)
	{
		FText findText;
		if (true == FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_EquipSlot"), findText))
		{
			FText resultText = findText.Format(findText, 0 == equipItem->GetCurrentPresetSlotIndex() ? 1 : 2);
			_textItemNameCompare->SetText(resultText);
		}
	}
}

void UGsUIPopupItemDetail::SetCollectionModeBtns(bool InChanged, bool InRegister)
{
	//// 2022/06/24 PKT - Item Collection  에서 일괄 버튼이 나오던 버그 수정
	//_btnSeveralUse->SetVisibility(ESlateVisibility::Hidden);

	//if (false == _isBtnRegisterType)
	//{
	//	_modeSwitcher->SetActiveWidgetIndex((uint8)DisplayModeType::ItemDetailMode);
	//	_btnUse->SetIsEnabled(false);
	//	return;
	//}
	//_btnChangeCollectionItem->SetIsEnabled(InChanged);
	//_btnRegisterCollection->SetIsEnabled(InRegister);

	//if (false == InChanged && false == InRegister)
	//	return;

	//if (false == _btnRegisterCollection->OnClicked.IsBound())
	//{
	//	_btnRegisterCollection->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnRegisterCollection);		
	//}

	//if (false == _btnChangeCollectionItem->OnClicked.IsBound())
	//{
	//	_btnChangeCollectionItem->OnClicked.AddDynamic(this, &UGsUIPopupItemDetail::OnShiftCollectionItem);
	//}
}

void UGsUIPopupItemDetail::UpdateItemData(const uint64 InItemDBID, bool InSkipUpdateDetailInfo)
{
	if (GetItemDBId() == InItemDBID)
	{
		SetItemDefaultData(_isHideButtonsFlag, _modeType, _isHideButtonsBottomFlag, _isUseLock, _isItemCollectionMarking);
		SetItemDetailData(_isItemCollectionMarking, InSkipUpdateDetailInfo);
	}
}

bool UGsUIPopupItemDetail::IsRemoveItem(const uint64 InItemDBID)
{
	if (false == IsValidItem())
		return true;

	return GetItemDBId() == InItemDBID;
}

void UGsUIPopupItemDetail::SetCompareItemEnchantCount(int32 InCount)
{
	_itemDetailInfo->SetComareItemEnchantCount(InCount);
}

bool UGsUIPopupItemDetail::IsEnableUseBtnState()
{
	if (ItemType::CURRENCY == GetItemType())
	{
		return false;
	}

	const ItemCategorySub Category = GetItemCategorySub();
	switch (Category)
	{
		// 제작 전용 재료 아이템
	case ItemCategorySub::INGREDIENT_CRAFT:
		return false;
		// 2021/12/22 PKT - 몬스터 도감(경험치아이템) 전용 아이템
	case ItemCategorySub::MONSTER_KNOWLEDGE_GAIN_EXP:
		return false;
	}

	// 시간이 만료 되었는데 장착중인 경우라면
	// 해지할 수 있도록 버튼을 Enable 처리 해야함
	if (true == IsExpiredTimeItem())
	{
		return GetDisPlayItemData()->IsEquipped();
	}

	return true;
}

bool UGsUIPopupItemDetail::IsActiveEnableBtnState()
{
	if (nullptr == GetDisPlayItemData())
		return false;

	if (GetDisPlayItemData()->IsIngredient() && false == GetDisPlayItemData()->IsUseItem())
	{
		return false;
	}
	else if (GetDisPlayItemData()->IsCurrency())
	{
		return false;
	}
	else if (GetDisPlayItemData()->IsCollectableType())
	{
		return false;
	}
	
	return true;
}

bool UGsUIPopupItemDetail::IsActiveSeveralBtnState()
{
	if (nullptr != GetDisPlayItemData())
	{
		if (ItemCategorySub::REWARD_BOX != GetDisPlayItemData()->GetSubCategory())
		{
			return false;
		}
		if (1 >= GetDisPlayItemData()->GetAmount())
		{
			return false;
		}
		if (true == IsExpiredTimeItem())
		{
			return false;
		}
	}

	return true;
}

void UGsUIPopupItemDetail::OnClickUseItem()
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	if (true == IsValidItem())
	{
		if (const FGsItem* itemInfo = GetDisPlayItemData())
		{
			// 소환 상품이면 retry 정보 여기서 세팅한다
			switch (itemInfo->GetSubCategory())
			{
			case ItemCategorySub::SUMMON_FAIRY:
			case ItemCategorySub::SUMMON_COSTUME:
			{
				if (FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler())
				{
					// 퀵슬롯 1, 2번에 등록한뒤(1: 페어리, 2: 코스튬)
					// 1, 2 연속해서 누르면
					// 1번(페어리) 성공, 2번(코스튬) 실패 
					// 하지만 다시뽑기 데이터는 2번이 저장되어서
					// 두번째는 코스튬뽑게됨
					// 유효하지 않으면 저장 안하게 처리
					//	https://jira.com2us.com/jira/browse/C2URWQ-5204	
					summonHandler->SetRetryDataByItemInfo(itemInfo);
				}
				break;
			}
			case ItemCategorySub::REWARD_BOX:
			{
				bool isPickOneType = FGsItemHelper::IsPickOneTypeRewardItem(itemInfo);
				// 선택형 보상 상자는 아이템을 바로 사용하지 않고 아이템 선택 팝업을 출력한다.
				if (isPickOneType)
				{
					if (itemManager->TryUseItem(itemInfo, 0, true))
					{
						// 아이템 단일 선택 팝업 출력
						int32 rewardId = itemInfo->GetPickOneTypeRewardID();
						struct FGsOpenPickOneBox paramData(itemInfo->GetDBID(), rewardId, false);
						GMessage()->GetItemContents().SendMessage(MessageContentItem::PICKONEBOX_OPEN, &paramData);

						OnClickClose();
					}
					return;
				}
				// 일반 보상 상자
				else
				{
					// 바로 아이템을 사용한다.
				}
				break;
			}
			}
		}

		itemManager->TryUseItem(GetDisPlayItemData());
	}

	OnClickClose();
}

void UGsUIPopupItemDetail::OnClickSeveralUseItem()
{
	if (false == IsValidItem())
	{
		OnClickClose();
		return;
	}

	if (false == GItem()->TryUseItem(GetDisPlayItemData(), 0, true))
		return;

	if (const FGsItem* itemInfo = GetDisPlayItemData())
	{
		// pick one reward box item
		if (0 < itemInfo->GetPickOneTypeRewardID())
		{
			struct FGsOpenPickOneBox paramData(itemInfo->GetDBID(), itemInfo->GetPickOneTypeRewardID(), true);
			GMessage()->GetItemContents().SendMessage(MessageContentItem::PICKONEBOX_OPEN, &paramData);
			OnClickClose();
		}
		else
		{
			// 계산기 출력
			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCalculator"));
			if (widget.IsValid())
			{
				UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(widget.Get());
				if (nullptr == popup)
					return;

				uint64 itemDBID = itemInfo->GetDBID();
				FText findText;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Button_UseAll"), findText);
				popup->SetDimmed(true);
				popup->SetTitleText(findText);
				popup->SetDefaultCount(itemInfo->GetAmount());
				popup->SetMinimumCount(1);
				popup->SetMaximumCount(itemInfo->GetAmount());
				popup->InvalidateNumText();
				popup->SetButtonCallBack([&, this, popup, itemDBID](bool state)
					{
						if (state)
						{
							OnConfirmSeveralUseCalculator(itemDBID, popup->GetCurrentNum());
						}
					});
			}
		}
	}
}

void UGsUIPopupItemDetail::OnConfirmSeveralUseCalculator(uint64 InItemDBID, ItemAmount InCount)
{
	if (0 >= InItemDBID || 0 >= InCount)
		return;

	GItem()->TryUseItem(InItemDBID, InCount);
}

void UGsUIPopupItemDetail::OnClickItemCompare()
{
	if (!IsValidItem() || false == GetDisPlayItemData()->IsEquipment())
		return;

	if (OnItemDetailCompareDelegate.IsBound())
	{
		OnItemDetailCompareDelegate.Broadcast();
		return;
	}

	struct FGsItemDetailCompareOpenMsgParam paramData(const_cast<FGsItem*>(GetDisPlayItemData()), _isHideButtonsFlag, true, false, _isHideButtonsBottomFlag, _isUseLock);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_COMPARE, &paramData);
}

void UGsUIPopupItemDetail::OnClickDisableItemCompare()
{
	if (DisplayModeType::ItemCompareMode == _modeType)
	{
		return;
	}

	FText findText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Compare_Notice_Cannot"), findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}

void UGsUIPopupItemDetail::OnClickDelete()
{
	/*
	if (true == GItem()->GetInvalidUserStatusflag())
	{
		// 스턴 상태일때 불가능 합니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}
	*/

	if (!IsValidItem())
		return;

	const ItemAmount amountCnt = GetItemAmount();
	if (1 < amountCnt)
	{
		OpenDeleteCalculatorUI();
	}
	else
	{
		OpenDeleteConfirmUI(amountCnt);
	}
}

void UGsUIPopupItemDetail::OnClickItemDecompose()
{
	if (IsValidItem())
	{
		struct FGsItemDecomposeSingle paramData(GetItemDBId());
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DECOMPOSE_SINGLE, &paramData);
	}
}

void UGsUIPopupItemDetail::OnClickDisableItemDecompose()
{
	/*
	FTextKey textKey = TEXT("");
	if (true == _selectedItem->IsCashItem())
	{
		textKey = TEXT("");
	}
	else if (true == _selectedItem->GetLock())
	{
		textKey = TEXT("");
	}
	else if (true == _selectedItem->IsEquipped())
	{
		textKey = TEXT("");
	}
	*/

	// 분해할 수 없습니다
	FText findText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Decompose_Notice_Cannot"), findText);
	FGsUIHelper::TrayMessageTicker(findText);
}

void UGsUIPopupItemDetail::OnClickItemCollection()
{
	if (IsValidItem())
	{
		if (_collectionSelect->GetVisibility() == ESlateVisibility::Collapsed)
		{
			bool isOnPersonItemCollection = GSItemCollection()->IsValidMaterial(GetDisPlayItemData());
			bool isOnGuildItemCollection = GGuildItemCollection()->IsItemAvailableForItemCollection(GetDisPlayItemData());
			if (isOnPersonItemCollection && isOnGuildItemCollection)
			{
				_collectionSelect->SetVisibility(ESlateVisibility::Visible);
			}
			else if (isOnPersonItemCollection)
			{
				OnShotcutPersonItemCollection();
			}
			else if (isOnGuildItemCollection)
			{
				OnShotcutGuildItemCollection();
			}
		}	
		else
		{
			_collectionSelect->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UGsUIPopupItemDetail::OnClickDisableItemCollection()
{
	if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::ITEM_COLLECTION))
	{
		// 컬렉션 안됨.
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Collection_Notice_Cannot"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}
	else
	{
		GSUnlock()->ShowLockTicker(EGsUnlockContentsType::ITEM_COLLECTION);
	}
}

void UGsUIPopupItemDetail::OnClickMarket()
{
	if (IsValidItem())
	{
#ifdef RESTORE_INVENTORY
		if (UGsInventoryManager* inventoryMgr = GSInventory())
		{
			if (inventoryMgr->CheckIsHUDState() &&
				inventoryMgr->IsInventoryOpen())
			{
				inventoryMgr->SetRestoreItemDBId(GetItemDBId());
			}
		}
#endif
		// ejrrb10 | TID 가 아니라, ItemGroupId 에 대해 거래 가능 아이템을 조회한다
		ItemId tradableItemId = GetDisPlayItemData()->GetTradableItemIdInGroup();

		FGsItemDetailOpenMarketParam param(tradableItemId, FGsItemDetailOpenMarketParam::EGsMarketSearchOption::DEFAULT);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_OPEN_MARKET, &param);

		OnClickClose();
	}
}

void UGsUIPopupItemDetail::OnClickDisableMarket()
{
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MARKET))
	{
		GSUnlock()->ShowLockTicker(EGsUnlockContentsType::MARKET);
		return;
	}

	FText findText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Market_Notice_Cannot"), findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}

void UGsUIPopupItemDetail::OnClickDisableDelete()
{
	if (IsValidItem())
	{
		FText findText;
		const FGsItem* itemData = GetDisPlayItemData();
		if (itemData->IsEquipped() || itemData->IsPresetEquipped())
		{
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Delete_Notice_Equiped"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			return;
		}
		if (itemData->GetLock())
		{
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Delete_Notice_Locked"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			return;
		}
	}
}

void UGsUIPopupItemDetail::OnClickDisableEnchant()
{
	FText findText;
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::ITEM_ENCHANT))
	{
		GSUnlock()->ShowLockTicker(EGsUnlockContentsType::ITEM_ENCHANT);
		return;
	}

	if (IsValidItem())
	{
		if (GetDisPlayItemData()->IsDepotItem())
		{
			// 창고에 보관중인 아이템은 강화할 수 없습니다.
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_Storage"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}
		else if (GetDisPlayItemData()->GetLock())
		{
			// 잠긴 아이템은 강화할 수 없습니다
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_Locked"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}
		else
		{
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_Cannot"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}
	}
}

void UGsUIPopupItemDetail::OnClickDisableItemUse()
{
	// 티커 메세지 출력
	// 어떤 이유로 사용할 수 없는지 이유 판단 코드 필요
	FString temp = "Cant Use Item_NeedTextID";
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FText::FromString(temp));

	/*
	// 잠긴 아이템은 사용할 수 없습니다
	FText findText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Use_Notice_Locked"), findText);
	FGsUIHelper::TrayMessageTicker(findText);
	*/
}

void UGsUIPopupItemDetail::OnClickEnchant()
{
	/*
	if (true == GItem()->GetInvalidUserStatusflag())
	{
		// 스턴 상태일때 불가능 합니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}
	*/

	if (IsValidItem())
	{
		FGsOpenItemEnchant paramData(GetItemDBId(), true , true/*DisplayModeType::ItemCollectionMode == _modeType*/);
		GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::OPEN, &paramData);

		/*
		
		if (DisplayModeType::ItemCollectionMode == _modeType)
		{	
			GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::OPEN_COLLECTION, &paramData);
		}
		else
		{
			GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::OPEN, &paramData);

		}
		*/

		OnClickClose();
	}
}

void UGsUIPopupItemDetail::OnConfirmItemDeleteStackCount(ItemAmount InCount)
{
	//GSLOG(Warning, TEXT("UGsUIPopupItemDetail::ConfirmItemStackCount - InCount : %d"), InCount);
	if (0 < InCount)
	{
		OpenDeleteConfirmUI(InCount);
	}
}

// 슬롯 1이 왼쪽 , 2 오른쪽 (캐릭터 상세창에 슬롯 기준)
// 슬롯1 ,슬롯2에 모두 착용중이라면 디폴트로 1을 출력함
// 둘중에 한곳에만 착용중이라면 착용중인 슬롯을 보여줌
// 둘다 착용중이 아니라면 별도 티커 출력해야 할듯
void UGsUIPopupItemDetail::OnClickNextCompare()
{
	if (!IsValidItem())
	{
		return;
	}

	const FGsItemEquipment* equipItem = static_cast<const FGsItemEquipment*>(GetDisPlayItemData());
	if (nullptr == equipItem)
		return;

	uint8 slotIndex = equipItem->GetCurrentPresetSlotIndex();
	EGsEquipSlotType slotType = GItem()->GetEquipSlotType(GetItemCategorySub(), 0 == slotIndex ? 1 : 0);
	if (OnItemDetailCompareNextDelegate.IsBound())
	{
		OnItemDetailCompareNextDelegate.Broadcast(slotType, _isHideButtonsFlag, _isHideButtonsBottomFlag, _isUseLock);
	}
}

void UGsUIPopupItemDetail::OnClickItemFusion()
{
	if (!IsValidItem())
	{
		return;
	}

	if (FGsMessageHolder* msgHolder = GMessage())
	{
		FGsPrimitiveUInt64 param(GetDisPlayItemData()->GetDBID());
		msgHolder->GetItemFusion().SendMessage(MessageContentsFusion::UI_OPEN, &param);
	}
}

void UGsUIPopupItemDetail::OnRegisterCollection()
{
	FGsItemItemCollectionRegisterParam Param(GetItemDBId());
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_REGISTER_ITEM_COLLECTION, &Param);
}

void UGsUIPopupItemDetail::OnShiftCollectionItem()
{
	const FGsItem* item = GetDisPlayItemData();
	ItemId itemId = (nullptr != item) ? item->GetTID() : INVALID_ITEM_ID;
	FGsItemItemCollectionShiftParam Param(GetItemDBId(), itemId);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_SHIFT_ITEM_COLLECTION, &Param);
}

void UGsUIPopupItemDetail::OnShotcutPersonItemCollection()
{
	if (false == IsValidItem())
	{
		return;
	}

	if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::ITEM_COLLECTION))
	{
#ifdef RESTORE_INVENTORY
		if (UGsInventoryManager* inventoryMgr = GSInventory())
		{
			if (inventoryMgr->CheckIsHUDState() &&
				inventoryMgr->IsInventoryOpen())
			{
				inventoryMgr->SetRestoreItemDBId(GetItemDBId());
			}
		}
#endif

		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_ITEM_COLLECTION);

		const FGsPrimitiveInt32 param(GetDisPlayItemData()->GetTID());
		GMessage()->GetItemCollection().SendMessage(MessageContentItemCollection::SEARCH_MATERIAL, &param);
	}
	else
	{
		GSUnlock()->ShowLockTicker(EGsUnlockContentsType::ITEM_COLLECTION);
	}
}


void UGsUIPopupItemDetail::OnShotcutGuildItemCollection()
{
	if (false == IsValidItem())
	{
		return;
	}

	if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::GUILD))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_GUILD);

		const FGsItemCollectionMessageOpenParam param(GetDisPlayItemData()->GetTID(), GetDisPlayItemData()->GetName());
		GMessage()->GetGuild().SendMessage(MessageContentGuild::ITEM_COLLECTION_OPEN_SHORTCUT, &param);
	}
	else
	{
		GSUnlock()->ShowLockTicker(EGsUnlockContentsType::GUILD);
	}
}

void UGsUIPopupItemDetail::OnChangeEnchantCount(int32 InCount)
{
	OnChangeOtherItemEnchantCount.ExecuteIfBound(InCount);
}

void UGsUIPopupItemDetail::OnClickClose()
{
	if (OnCloseItemDetailDelegate.IsBound())
	{
		OnCloseItemDetailDelegate.Broadcast();
	}
}

void UGsUIPopupItemDetail::SetDisplayDetailMode(DisplayModeType InModeType)
{
	_modeType = InModeType;
}

const FGsItem* UGsUIPopupItemDetail::GetDisPlayItemData() const
{
	if (true == OnGetItemDataDelegate.IsBound())
	{
		const FGsItem* itemData = OnGetItemDataDelegate.Execute();
		return itemData;
	}
	return nullptr;
}
