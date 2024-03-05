// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUINpcShopProductEntry.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

#include "Paper2D/Classes/PaperSprite.h"

#include "T1Project.h"

#include "Item/GsItemManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"

#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemShop.h"

#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsNpcShopManager.h"

#include "Currency/GsCurrencyHelper.h"

#include "NpcShop/GsNpcShopHelper.h"


void UGsUINpcShopProductEntry::BeginDestroy()
{
	if (nullptr != _iconSlotHelper)
	{
		_iconSlotHelper->Clear();
		_iconSlotHelper = nullptr;
	}

	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	Super::BeginDestroy();
}

void UGsUINpcShopProductEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUINpcShopProductEntry::OnRefreshIconWrapBox);
		_iconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUINpcShopProductEntry::OnCreateIconWrapBox);
	}

	_btnSelectSlot->OnClicked.AddDynamic(this, &UGsUINpcShopProductEntry::OnClickSelect);
}

void UGsUINpcShopProductEntry::NativeDestruct()
{
	OnHide();

	Super::NativeDestruct();
}

void UGsUINpcShopProductEntry::OnShow()
{
	// UGsDynamicPanelSlotHelper로 관리되어 수행
	// OnShow -> SetItemInfo

	_isSelected = false;
	_slotIndex = 0;
	_slotSelectionSwitcher->SetActiveWidgetIndex(_isSelected);
}

void UGsUINpcShopProductEntry::OnHide()
{
	// UGsDynamicPanelSlotHelper로 관리되어 수행

	if (true == OnSelectProductItemDelegate.IsBound())
	{
		OnSelectProductItemDelegate.Clear();
	}

	_shopProduct = nullptr;
}

void UGsUINpcShopProductEntry::SetItemInfo(FGsNpcShopItemInfo* InShopInfo, uint8 InSlotIndex, NpcFunctionType InShopType)
{
	if (nullptr == InShopInfo)
		return;

	const FGsSchemaItemCommon* commonItemData = FGsItem::GetTableDataByID(InShopInfo->_itemTID);
	if (nullptr == commonItemData)
		return;

	_slotIndex = InSlotIndex;
	_shopProduct = InShopInfo;
	_shopNpcFunctionType = InShopType;
	
	_itemNameText->SetText(commonItemData->name);
	_itemPriceText->SetText(FText::AsNumber(_shopProduct->GetCurrencyAmount(_shopNpcFunctionType)));

	if (CostType::CURRENCY == InShopInfo->_costType)
	{
		SetCurrencyIconImage(InShopInfo->_currencyType);
	}
	else if (CostType::ITEM == InShopInfo->_costType)
	{
		SetCurrencyIconImageByItem(InShopInfo->_currencyItemTID);
	}

	InvalidateItem();
}

void UGsUINpcShopProductEntry::InvalidateItem()
{
	CheckState();
	_iconSlotHelper->RefreshAll(1);
}

void UGsUINpcShopProductEntry::CheckState()
{
	if (nullptr == _shopProduct)
	{
		_dimmedBg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		return;
	}

	EGsNotAllowedBuyReason type = EGsNotAllowedBuyReason::NONE;
	if (true == FGsNpcShopHelper::IsAllowedBuyItem(_shopProduct, type))
	{	// 구매 가능한 경우
		SetRenewalTimeText(false);
		_dimmedBg->SetVisibility(ESlateVisibility::Collapsed);
		_levelLimitText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{	// 구매 불가능한 경우
		_dimmedBg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// 제한 유형에 따른 제한 정보 표시
		_levelLimitText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SetLimitText(type);

		// 갱신 유형에 따른 갱신 정보 표시
		if (EGsNotAllowedBuyReason::SOLD_OUT == type)
		{
			SetRenewalTimeText(true, _shopProduct->_pcLimitResetType);
		}
		else
		{
			SetRenewalTimeText(false);
		}
	}
}

void UGsUINpcShopProductEntry::SetCurrencyIconImage(const CurrencyType InType)
{
	UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(FGsCurrencyHelper::GetCurrencyIconPath(InType));
	_currencyImage->SetBrushFromAtlasInterface(spriteImg);
}

void UGsUINpcShopProductEntry::SetCurrencyIconImageByItem(int32 InItemTID)
{
	const UGsTableItemCommon* table = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (table)
	{
		const FGsSchemaItemCommon* row;
		if (table->FindRowById(InItemTID, row))
		{
			UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(row->icon);
			_currencyImage->SetBrushFromAtlasInterface(spriteImg);
		}
	}
}

void UGsUINpcShopProductEntry::SetSlotSelection(const bool InIsSelected)
{
	_isSelected = InIsSelected;

	// 0 : normal , 1 : selected
	_slotSelectionSwitcher->SetActiveWidgetIndex(_isSelected);
}

void UGsUINpcShopProductEntry::OnClickSelect()
{
	if (nullptr == _shopProduct)
		return;

	if (true == OnSelectProductItemDelegate.IsBound())
	{
		OnSelectProductItemDelegate.Broadcast(this);
	}
}

void UGsUINpcShopProductEntry::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	UGsUIIconItemShop* itemIcon = Cast<UGsUIIconItemShop>(&InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if (nullptr == itemIcon->GetItemIconData())
		return;

	OnClickSelect();
	// 아이템 상세창 open / close
	//FGsItemDetailOpenMessageParam param(itemIcon->GetItemIconData(),true,true, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
	//GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
}

void UGsUINpcShopProductEntry::OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon)
{
	UGsUIIconItemShop* itemIcon = Cast<UGsUIIconItemShop>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}
	
	itemIcon->SetNotOwnItem(_shopProduct->_itemTID, 1);
	itemIcon->SetItemShopInfo(_shopProduct);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
	itemIcon->SetItemCollectionMarking(itemIcon->GetItemIconData(), true);

	if (false == itemIcon->OnFinishedLongPressEvent.IsBound())
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUINpcShopProductEntry::OnLongPressIcon);
	}

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUINpcShopProductEntry::OnClickIconSlot);
	}
}

void UGsUINpcShopProductEntry::OnCreateIconWrapBox(class UGsUIIconBase* InIcon)
{
	UGsUIIconItemShop* itemIcon = Cast<UGsUIIconItemShop>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemCount(0);
}

void UGsUINpcShopProductEntry::SetAmountTextColor(const EGsUIColorType InColorType)
{
	_itemPriceText->SetColorAndOpacity(FGsUIColorHelper::GetColor(InColorType));
}

int32 UGsUINpcShopProductEntry::GetProductItemTID()
{
	if (nullptr == _shopProduct)
		return 0;

	return _shopProduct->_itemTID;
}

EventProductId UGsUINpcShopProductEntry::GetProductId()
{
	if (nullptr == _shopProduct)
		return INVALID_NPC_EVENT_SHOP_PRODUCT_ID;

	return _shopProduct->_productId;
}

uint8 UGsUINpcShopProductEntry::GetSlotIndex()
{
	return _slotIndex;
}

FGsNpcShopItemInfo* UGsUINpcShopProductEntry::GetShopProductInfo()
{
	return _shopProduct;
}

void UGsUINpcShopProductEntry::SetRenewalTimeText(bool InActive, ResetTimeTicket InResetType)
{
	if (true == InActive && ResetTimeTicket::RTT_NONE != InResetType)
	{
		_renewalTimeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FText resetTimeText;
		FGsNpcShopHelper::GetTextByRTT(InResetType, resetTimeText);
		_renewalTimeText->SetText(resetTimeText);
	}
	else
	{
		_renewalTimeText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FTextKey UGsUINpcShopProductEntry::GetLimitGuildMemberGradeTextKey(GuildMemberGradeType InGradeType)
{
	FTextKey outTextKey;
	switch (InGradeType)
	{
	case GuildMemberGradeType::KNIGHT:
		outTextKey = TEXT("Purchase_Restrict_Guild_Knight");
	break;
	case GuildMemberGradeType::FIRSTKNIGHT:
		outTextKey = TEXT("Purchase_Restrict_Guild_1stKnight");
	break;
	case GuildMemberGradeType::SECONDKNIGHT:
		outTextKey = TEXT("Purchase_Restrict_Guild_2ndKnight");
	break;
	case GuildMemberGradeType::SUBCAPTAIN:
		outTextKey = TEXT("Purchase_Restrict_Guild_subCaptain");
	break;
	case GuildMemberGradeType::CAPTAIN:
		outTextKey = TEXT("Purchase_Restrict_Guild_Captain");
	break;
	default:
		outTextKey = TEXT("");
	}

	return outTextKey;
}

void UGsUINpcShopProductEntry::SetLimitText(EGsNotAllowedBuyReason InReason)
{
	FText findText;
	switch (InReason)
	{
	case EGsNotAllowedBuyReason::UNDER_LIMIT_LEVEL:
	{
		// 레벨 n 이상
		FTextKey textKey = TEXT("Purchase_Restrict_Level");
		FText::FindText(TEXT("NpcEventShopText"), textKey, findText);
		findText = FText::Format(findText, _shopProduct->_pcLimitLevel);
	}
	break;
	case EGsNotAllowedBuyReason::UNDER_LIMIT_GUILD_LEVEL:
	{
		// 길드 레벨 n 이상
		FTextKey textKey = TEXT("Purchase_Restrict_GuildLevel");
		FText::FindText(TEXT("NpcEventShopText"), textKey, findText);
		findText = FText::Format(findText, _shopProduct->_pcLimitGuildLevel);
	}
	break;
	case EGsNotAllowedBuyReason::UNDER_LIMIT_GUILD_MEMBER_GRADE:
	{
		// <계급명> 이상
		FTextKey textKey = GetLimitGuildMemberGradeTextKey(_shopProduct->_pcLimitGuildMemberGrade);
		FText::FindText(TEXT("NpcEventShopText"), textKey, findText);
	}
	break;
	case EGsNotAllowedBuyReason::SOLD_OUT:
	{
		// 서버 제한 : 3/3 Or 캐릭터 제한 : 10/10
		FTextKey textKey;
		switch (_shopProduct->_pcLimitBuyerType)
		{
		case PurchaseLimitBuyerType::CHARACTER:
			textKey = TEXT("Purchase_Limit_Character");
			break;
		case PurchaseLimitBuyerType::SERVER:
			textKey = TEXT("Purchase_Limit_Server");
			break;
		case PurchaseLimitBuyerType::ACCOUNT:
			textKey = TEXT("Purchase_Limit_Account");
			break;
		default:
			textKey = TEXT("");
		}
		FText::FindText(TEXT("NpcEventShopText"), textKey, findText);
		FString tempStr = FString::Printf(TEXT("%s : %d/%d"), *(findText.ToString()), _shopProduct->_purchasedAmount, _shopProduct->_pcLimitBuyerAmount);
		findText = FText::FromString(tempStr);
	}
	break;
	}
	_levelLimitText->SetText(findText);
}

void UGsUINpcShopProductEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = false;
	option._bHideBG = false;
	option._bHideButtonsBottom = true;
	option._bUseLock = false;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), UGsUIPopupItemDetailMain::CenterPosType::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}