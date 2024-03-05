// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIIconItem.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"

#include "DataSchema/GsSchemaCurrencyData.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Manager/GsTooltipManager.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"

#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "UMG/Public/Blueprint/SlateBlueprintLibrary.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Engine/Classes/Engine/GameViewportClient.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Currency/GsCurrencyHelper.h"

#include "T1Project.h"


void UGsUIIconItem::NativeOnInitialized()
{
	_itemSlotButton->OnClicked.AddDynamic(this, &UGsUIIconItem::OnClickItemIcon);

	SetItemCount(0);

	Super::NativeOnInitialized();
}

void UGsUIIconItem::NativeConstruct()
{
	Super::NativeConstruct();

	// icon slot Effect Widget (없을수도 있음)
	_slotEffectWidget = Cast<UGsUIItemIconSlotEffect>(GetWidgetFromName(TEXT("_iconSlotEffectWidget")));
}

void UGsUIIconItem::NativeDestruct()
{
	_nameToolTipFlag = false;

	CloseNameTooltip();
	_notOwnItem.Reset();
	_itemDBID = INVALID_ITEM_DB_ID;
	_currencyType = CurrencyType::NONE;

	OnClickSlot.Clear();
	OnFinishedLongPressEvent.Clear();

	if (true == _itemSlotButton->_onFinishedLongPressGauge.IsBoundToObject(this) && true == _longPressDelegate.IsValid())
	{
		_itemSlotButton->_onFinishedLongPressGauge.Remove(_longPressDelegate);
		_longPressDelegate.Reset();
	}

	//_itemSlotButton->OnChangeLongPress(false);

	if (nullptr != _slotEffectWidget)
	{
		_slotEffectWidget = nullptr;
	}

	Super::NativeDestruct();
}

void UGsUIIconItem::SetEmptyIcon()
{
	Super::SetEmptyIcon();

	CloseNameTooltip();

	_nameToolTipFlag = false;
	_isHoverMode = false;

	_notOwnItem.Reset();
	_tableId = 0;
	_itemDBID = INVALID_ITEM_DB_ID;
	_currencyType = CurrencyType::NONE;

	if (OnClickSlot.IsBound())
	{
		OnClickSlot.Clear();
	}
	if (OnFinishedLongPressEvent.IsBound())
	{
		OnFinishedLongPressEvent.Clear();
	}

	_lastSoftObjectPath = FSoftObjectPath();

	SetItemCount(0);
	SetItemEnchantLevel(0);
	SetItemSlotEffect(EGsItemIconSlotEffectType::None);
	SetSlotEffectMagicalForge(nullptr);

	if (true == _itemSlotButton->_onFinishedLongPressGauge.IsBoundToObject(this) && true == _longPressDelegate.IsValid())
	{
		_itemSlotButton->_onFinishedLongPressGauge.Remove(_longPressDelegate);
		_longPressDelegate.Reset();
	}
	_itemSlotButton->OnChangeLongPress(false);
}

void UGsUIIconItem::OnFinishedLongPress()
{
	//GSLOG(Warning, TEXT("[UGsUIIconItem::OnFinishedLongPress]"));
	CloseNameTooltip();

	if (OnFinishedLongPressEvent.IsBound())
	{
		OnFinishedLongPressEvent.Broadcast(*this);
		return;
	}

	FGsItem* itemData = GetItemIconData();
	if (_currencyType == CurrencyType::NONE && nullptr != itemData)
	{
		/*
		// 제거해야할 코드임 - 상세창 출력은 각 컨텐츠에서 OnFinishedLongPressEvent 이벤트 바인딩하여 출력해야함
		FGsItemDetailOpenMessageParam param(itemData, false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
		*/
	}
	else if (_currencyType != CurrencyType::NONE && nullptr == itemData)
	{
		//FGsCurrencyDetailOpenMessageParam param(_currencyType, false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
		//GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_CURRENCY, &param);
	}
	else
	{
		GSLOG(Warning, TEXT("_currencyType == %d, nullptr != %p"), _currencyType, itemData);
		_itemSlotButton->OnChangeLongPress(false);
	}
}

void UGsUIIconItem::SetOnChangeDisplayDetailWndType(UseDisplayDetailWndType InUseType)
{
	switch (InUseType)
	{
	case UseDisplayDetailWndType::LongPressType:
		_itemSlotButton->OnChangeLongPress(true);
		break;
	case UseDisplayDetailWndType::NotUseDetailWndType:
	case UseDisplayDetailWndType::OnClickType:
		_itemSlotButton->OnChangeLongPress(false);
		break;
	}
	_eDisplayDetailWndType = InUseType;
}

void UGsUIIconItem::SetIconImage(const FSoftObjectPath& InPath)
{
	if (_lastSoftObjectPath == InPath)
		return;

	// 로딩되기 전에는 이미지를 가리기 위함
	SetIconImageSprite(nullptr);

	_lastSoftObjectPath = InPath;

	//uint64 tableId = _tableId;
	FSoftObjectPath objectPath = _lastSoftObjectPath;

	// 로드되는 사이 이 객체가 어떻게 될지 모르므로 TWeakObjectPtr 사용
	TWeakObjectPtr<UGsUIIconItem> weakThis(this);
	FGsUIHelper::AsyncLoadSpriteFromPath(InPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, objectPath](UObject* inRes)
		{
			if (weakThis.IsValid())
			{
				// 로드 끝난후라 LoadSpriteFromPath 시 새로 로드하지 않고, 로드된 항목을 찾음
				if (UGsUIIconItem* strongThis = weakThis.Get())
				{
					//if (tableId == strongThis->GetTableId() || objectPath == strongThis->GetLastSoftObjectPath())
					if (objectPath == strongThis->GetLastSoftObjectPath())
					{
						strongThis->SetIconImageSprite(Cast<UPaperSprite>(inRes));
					}
				}
			}
			else
			{
				GSLOG(Warning, TEXT("AsyncLoad UGsUIIconItem::SetIconImage invalid pointer"));
			}
		}));
}

void UGsUIIconItem::OnClickItemIcon()
{
	//GSLOG(Warning, TEXT("[UGsUIIconItem::OnClickItemIcon]"));
	if (OnClickSlot.IsBound())
	{
		OnClickSlot.Broadcast(*this);
	}

	OpenNameTooltip();
}

void UGsUIIconItem::SetItemCount(int64 InCount)
{
	_amount = InCount;
	if (nullptr != _itemCountText)
	{
		bool isShowingText = 1 < _amount ? true : false;
		_itemCountText->SetVisibility(true == isShowingText ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

		if (true == isShowingText)
		{
			FText amountText;
			FGsUIStringHelper::GetItemAmountShortText(_amount, amountText);
			_itemCountText->SetText(amountText);
		}
	}
}

// 2021/12/24 PKT - 특정 컨텐츠(몬스터 도감)에서 소유 하고 있는 아이템 수량을 0~1값으로 출력해야 하는 상황이 있음.(Data는 바꾸지 않고, 표기만 바꿈)
void UGsUIIconItem::SetItemCountEX(int64 InCount, bool InForcedShowText)
{
	if (nullptr != _itemCountText)
	{
		_itemCountText->SetText(FText::AsNumber(FMath::Max<int64>(InCount, 0)));

		if (InForcedShowText == true)
		{
			_itemCountText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void UGsUIIconItem::SetItemCountMinMax(int64 InMinValue, int64 InMaxValue)
{
	if (InMinValue == InMaxValue)
	{
		SetItemCount(InMaxValue);
		return;
	}

	_itemCountText->SetVisibility(ESlateVisibility::Visible);

	FText markText;
	FText::FindText(TEXT("MarkFormatingText"), TEXT("SwungDash"), markText);

	FString minString;
	FString maxString;
	FGsUIStringHelper::GetItemAmountShortString(InMinValue, minString);
	FGsUIStringHelper::GetItemAmountShortString(InMaxValue, maxString);

	FString resultStr = minString + markText.ToString() + maxString;
	_itemCountText->SetText(FText::FromString(resultStr));
}

void UGsUIIconItem::SetAllOwnedItemCountDisplay()
{
	FGsItem* itemData = GetItemIconData();
	if (nullptr == itemData)
		return;

	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
		return;

	const int32 MILLION = 1000000;
	ItemAmount allAmount = itemMgr->GetAllItemAmountByTypes(itemData->GetType(), itemData->GetMainCategory(), itemData->GetSubCategory());

	if (MILLION <= allAmount)
	{
		FText amountText;
		FGsUIStringHelper::GetItemAmountShortText(MILLION - 1, amountText);
		FText plusText;
		FText::FindText(TEXT("MarkFormatingText"), TEXT("Plus"), plusText);
		FString strAmount = amountText.ToString() + plusText.ToString();
		_itemCountText->SetVisibility(ESlateVisibility::HitTestInvisible);
		_itemCountText->SetText(FText::FromString(strAmount));
	}
	else
	{
		SetItemCount(allAmount);
	}
}

void UGsUIIconItem::SetOwnedItemCountByItemTid()
{
	FGsItem* itemData = GetItemIconData();
	if (nullptr == itemData)
		return;

	ItemAmount count = itemData->GetMaxStackCount() > GItem()->GetAllItemCountByTID(itemData->GetTID()) ?
		GItem()->GetAllItemCountByTID(itemData->GetTID()) : itemData->GetMaxStackCount();

	bool isShowingMark = GItem()->IsOverMaxStackCountByTID(itemData->GetTID());
	if (true == isShowingMark)
	{
		FText markText;
		FText::FindText(TEXT("MarkFormatingText"), TEXT("Plus"), markText);
		FString resultStr = FString::FromInt(count) + markText.ToString();
		_itemCountText->SetVisibility(ESlateVisibility::HitTestInvisible);
		_itemCountText->SetText(FText::FromString(resultStr));
		return;
	}

	SetItemCount(count);
}

void UGsUIIconItem::SetItemEnchantLevel(int32 InLevel)
{
	if (nullptr != _enchantNum)
	{
		bool isShowingText = 1 <= InLevel ? true : false;
		_enchantNum->SetVisibility(true == isShowingText ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

		if (true == isShowingText)
		{
			FString resultString;
			FText markText;
			FText::FindText(TEXT("MarkFormatingText"), TEXT("Plus"), markText);
			resultString = markText.ToString() + FString::FormatAsNumber(InLevel);
			_enchantNum->SetText(FText::FromString(resultString));

			SetEnchantTextColor();
		}
	}
}

CostType UGsUIIconItem::GetCostType() const
{
	FGsItem* itemData = GetItemIconData();
	if (_currencyType == CurrencyType::NONE && nullptr != itemData)
	{
		return CostType::ITEM;
	}
	else if (_currencyType != CurrencyType::NONE && nullptr == itemData)
	{
		return CostType::CURRENCY;
	}

	return CostType::MAX;
}

void UGsUIIconItem::SetItemCountTextVisibility(ESlateVisibility InVisibility)
{
	_itemCountText->SetVisibility(InVisibility);
}

void UGsUIIconItem::SetEnchantNumTextVisibility(ESlateVisibility InVisibility)
{
	_enchantNum->SetVisibility(InVisibility);
}

void UGsUIIconItem::SetEnchantTextColor()
{
	FGsItem* itemData = GetItemIconData();
	if (nullptr == itemData)
		return;

	_enchantNum->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(itemData->GetGradeType()));
}

void UGsUIIconItem::SetEquipmentInfo(FGsItem* InItemData, const uint8 InEnchantShieldNum, const uint8 InBonusStatIndex)
{
	if (nullptr == InItemData || nullptr == InItemData->GetCastItem<FGsItemEquipment>())
		return;

	FGsItemEquipment* equipData = InItemData->GetCastItem<FGsItemEquipment>();
	equipData->SetEnchantShieldCount(InEnchantShieldNum);
	equipData->SetEnchantBonusIndex(InBonusStatIndex);
}

void UGsUIIconItem::SetNotOwnItem(const uint32 InTID, const ItemAmount InAmount, const int32 InEnchantLevel, const uint8 InEnchantShieldCount, const uint8 InEnchantBonusIndex)
{
	if (_itemDBID != INVALID_ITEM_DB_ID || _tableId != InTID)
	{
		CloseNameTooltip();
	}

	_notOwnItem.Reset();
	_tableId = InTID;
	_itemDBID = INVALID_ITEM_DB_ID;
	_currencyType = CurrencyType::NONE;

	if (0 >= InTID)
	{
		SetEmptyIcon();
		return;
	}

	FGsItem* newItem = FGsItem::Create(InTID, InAmount);
	if (nullptr == newItem)
	{
		SetEmptyIcon();
		return;
	}
	newItem->UpdateTableData(InTID);
	newItem->SetLevel(InEnchantLevel);
	if (newItem->IsEquipment())
	{
		SetEquipmentInfo(newItem,InEnchantShieldCount, InEnchantBonusIndex);
	}
	
	_notOwnItem = MakeShareable(newItem);
	FGsItem* itemData = GetItemIconData();
	if (nullptr == itemData)
	{
		SetEmptyIcon();
		return;
	}

	SetVisibility(ESlateVisibility::Visible);

	SetItemCount(InAmount);
	SetItemEnchantLevel(itemData->GetLevel());
	SetItemGrade(newItem->GetGradeType());
	SetItemSlotEffect(newItem->GetIconSlotEffectType());
	SetSlotEffectMagicalForge(nullptr);

	if (nullptr != itemData->GetIconPath())
	{
		SetIconImage(*itemData->GetIconPath());
	}
	
	if (false == _itemSlotButton->_onFinishedLongPressGauge.IsBoundToObject(this))
	{
		_longPressDelegate = _itemSlotButton->_onFinishedLongPressGauge.AddUObject(this, &UGsUIIconItem::OnFinishedLongPress);
	}
	_itemSlotButton->OnChangeLongPress(true);
}

void UGsUIIconItem::SetOwnedItem(const uint64 InItemDBID, const FindItemStorageType InStorageType)
{
	if (_itemDBID != InItemDBID)
	{
		CloseNameTooltip();
	}

	_eFindItemStorageType = InStorageType;
	_itemDBID = InItemDBID;
	_currencyType = CurrencyType::NONE;
	if (0 >= _itemDBID)
	{
		SetEmptyIcon();
		return;
	}

	FGsItem* itemData = GetItemIconData();
	if (nullptr == itemData)
	{
		SetEmptyIcon();
		return;
	}

	_tableId = itemData->GetTID();

	SetVisibility(ESlateVisibility::Visible);
	
	SetItemCount(itemData->GetAmount());
	SetItemEnchantLevel(itemData->GetLevel());
	SetItemGrade(itemData->GetGradeType());
	SetItemSlotEffect(itemData->GetIconSlotEffectType());
	SetSlotEffectMagicalForge(itemData);

	if (nullptr != itemData->GetIconPath())
	{
		SetIconImage(*itemData->GetIconPath());
	}

	if (false == _itemSlotButton->_onFinishedLongPressGauge.IsBoundToObject(this))
	{
		_longPressDelegate = _itemSlotButton->_onFinishedLongPressGauge.AddUObject(this, &UGsUIIconItem::OnFinishedLongPress);
	}
	_itemSlotButton->OnChangeLongPress(true);
}

FGsItem* UGsUIIconItem::GetItemIconData() const
{
	if (_itemDBID == INVALID_ITEM_DB_ID)
	{
		if (false == _notOwnItem.IsValid())
		{
			return nullptr;
		}
		return _notOwnItem.Get();
	}

	if (FindItemStorageType::Storage_Inventory == _eFindItemStorageType)
	{
		TWeakPtr<FGsItem> findItem = GItem()->FindItem(_itemDBID);
		if (false == findItem.IsValid())
		{
			return nullptr;
		}
		return findItem.Pin().Get();
	}
	else if (FindItemStorageType::Storage_Depot == _eFindItemStorageType)
	{
		TWeakPtr<FGsItem> findItem = GSDepot()->FindDepotItemByDBID(_itemDBID);
		if (false == findItem.IsValid())
		{
			return nullptr;
		}
		return findItem.Pin().Get();
	}
	else
	{
		TWeakPtr<FGsItem> findItem = GItem()->FindItem(_itemDBID);
		if (false == findItem.IsValid())
		{
			findItem = GSDepot()->FindDepotItemByDBID(_itemDBID);
		}
		return findItem.Pin().Get();
	}

	return nullptr;
}

void UGsUIIconItem::SetCurrency(const CurrencyType InCurrencyType, const Currency InMinAmount, const Currency InMaxAmount/* = MIN_ITEM_AMOUNT*/)
{
	_notOwnItem.Reset();
	_tableId = static_cast<uint64>(InCurrencyType);
	_itemDBID = INVALID_ITEM_DB_ID;
	_currencyType = InCurrencyType;

	if (CurrencyType::NONE == _currencyType)
	{
		GSLOG(Error, TEXT("CurrencyType::NONE == _currencyType"));
		return;
	}

	SetVisibility(ESlateVisibility::Visible);

	if (InMaxAmount == MIN_ITEM_AMOUNT)
		SetItemCount(InMinAmount);
	else
		SetItemCountMinMax(InMinAmount, InMaxAmount);

	SetItemEnchantLevel(0);
	SetItemGrade(ItemGrade::NORMAL);
	SetItemSlotEffect(EGsItemIconSlotEffectType::None);

	SetIconImage(FGsCurrencyHelper::GetCurrencyIconPath(InCurrencyType));

	if (false == _itemSlotButton->_onFinishedLongPressGauge.IsBoundToObject(this))
	{
		_longPressDelegate = _itemSlotButton->_onFinishedLongPressGauge.AddUObject(this, &UGsUIIconItem::OnFinishedLongPress);
	}
	_itemSlotButton->OnChangeLongPress(true);
}

CurrencyType UGsUIIconItem::GetCurrencyType() const
{
	return _currencyType;
}

Currency UGsUIIconItem::GetAmount() const
{
	return _amount;
}

const FGsSchemaCurrencyData* UGsUIIconItem::GetCurrencyTable() const
{
	return FGsCurrencyHelper::GetCurrencyData(_currencyType);
}

void UGsUIIconItem::SetExpPercent(double InExpPercent)
{
	_notOwnItem.Reset();

	_tableId = 0;
	_itemDBID = INVALID_ITEM_DB_ID;
	_currencyType = CurrencyType::NONE;

	SetVisibility(ESlateVisibility::Visible);

	FString strExp = FString::Printf(TEXT("%.2lf%%"), InExpPercent);
	_itemCountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_itemCountText->SetText(FText::FromString(strExp));

	SetItemEnchantLevel(0);
	SetItemGrade(ItemGrade::NORMAL);
	SetItemSlotEffect(EGsItemIconSlotEffectType::None);

	FSoftObjectPath pathExpIcon = GPath()->GetObjectPath(EGsPathType::Asset, TEXT("Sprite_ExpIcon"));
	SetIconImage(pathExpIcon);

	// 롱프레스 제거
	if (_itemSlotButton->_onFinishedLongPressGauge.IsBoundToObject(this))
	{
		_itemSlotButton->_onFinishedLongPressGauge.Remove(_longPressDelegate);
	}
	_itemSlotButton->OnChangeLongPress(false);
}

void UGsUIIconItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
#if WITH_EDITOR || PLATFORM_WINDOWS
	if (_isHoverMode)
	{
		OpenNameTooltip();
		if (_nameToolTipObjPtr.IsValid())
		{
			_nameToolTipObjPtr->SetHoverMode(true);
		}
	}
#endif //WITH_EDITOR
}

void UGsUIIconItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
#if WITH_EDITOR || PLATFORM_WINDOWS
	if (_isHoverMode)
	{
		CloseNameTooltip();
	}
#endif //WITH_EDITOR
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UGsUIIconItem::OnFinishedItemNameToolTip()
{
	CloseNameTooltip();
}

void UGsUIIconItem::OpenNameTooltip()
{
	if (false == _nameToolTipFlag)
		return;

	if (_nameToolTipObjPtr.IsValid())
		return;

	FText tooltipText;
	ItemGrade tooltipColorGrade;

	// 아이템
	if (_currencyType == CurrencyType::NONE)
	{
		FGsItem* itemData = GetItemIconData();
		if (false == FGsItem::IsValidItem(itemData))
			return;

		tooltipText = itemData->GetName();
		tooltipColorGrade = itemData->GetGradeType();
	}
	// 재화
	else
	{
		tooltipText = FGsCurrencyHelper::GetCurrencyLocalizedText(_currencyType);
		tooltipColorGrade = ItemGrade::NORMAL;
	}

	if (UGsTooltipManager* tooltipManager = GUI()->GetTooltipManager())
	{
		// 다른 이름 툴팁 닫기
		tooltipManager->CloseNameTooltip();

		// 이름 툴팁 추가 및 설정
		_nameToolTipObjPtr = tooltipManager->OpenNameTooltip(this, tooltipText, tooltipColorGrade);
		if (UGsUITrayItemNameToolTip* nameToolTip = _nameToolTipObjPtr.Get())
		{
			if (false == nameToolTip->_onFinishedDelegate.IsBoundToObject(this))
			{
				nameToolTip->_onFinishedDelegate.AddUObject(this, &UGsUIIconItem::OnFinishedItemNameToolTip);
			}
		}
	}
}

uint8 UGsUIIconItem::GetGradeNumByGradeType(ItemGrade InItemGrade)
{
	switch (InItemGrade)
	{
	case ItemGrade::NORMAL:
		return 0;
	case ItemGrade::MAGIC:
		return 1;
	case ItemGrade::RARE:
		return 2;
	case ItemGrade::EPIC:
		return 3;
	case ItemGrade::UNIQUE:
		return 4;
	case ItemGrade::LEGEND:
		return 5;
	case ItemGrade::ETERNAL:
		return 6;
	}

	return 0;
}

bool UGsUIIconItem::DefaultItemDetailOpener(UGsUIIconItem& InIcon, const bool InShowBg, bool In_isShowCollectionShow)
{
	FGsMessageHolder* MessageHolder = GMessage();
	const CostType CurrentCostType = InIcon.GetCostType();

	if (nullptr != MessageHolder && CostType::CURRENCY == CurrentCostType)
	{
		const CurrencyType CurrentCurrencyType = InIcon.GetCurrencyType();
		if (CurrentCurrencyType == CurrencyType::NONE)
		{
			GSLOG(Error, TEXT("CurrentCurrencyType == CurrencyType::NONE"));
		}
		else
		{
			FGsCurrencyDetailOpenMessageParam MessageParam(CurrentCurrencyType, InIcon.GetAmount(), true, !InShowBg, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
			MessageHolder->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_CURRENCY, &MessageParam);
			return true;
		}
	}
	else if (nullptr != MessageHolder && CostType::ITEM == CurrentCostType)
	{
		FGsItem* item = InIcon.GetItemIconData();
		if (nullptr == item)
		{
			GSLOG(Error, TEXT("nullptr == item"));
		}
		else
		{
			bool isHideBG = !InShowBg;
			if (In_isShowCollectionShow == true)
			{
				FGsItemDetailOpenMessageParamEX::DetailOption option;
				option._bHideButtons = false;
				option._bHideBG = isHideBG;
				option._bHideButtonsBottom = true;
				option._bUseLock = false;
				option._bUseCollectionMarking = true;

				FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), UGsUIPopupItemDetailMain::CenterPosType::CenterPos, option);
				GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
			}
			else
			{ 
				FGsItemDetailOpenMessageParam param(item, false, isHideBG, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
				GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
			}
			return true;
		}
	}
	else
	{
		GSLOG(Error, TEXT("MessageHolder:%p, CurrentCostType:%d"), MessageHolder, CurrentCostType);
	}

	return false;
}

void UGsUIIconItem::SetOnClickLockSec(float InSec)
{
	if (_itemSlotButton)
	{
		_itemSlotButton->SetOnClickLockSec(0.f > InSec ? 0.f : InSec);
	}
}