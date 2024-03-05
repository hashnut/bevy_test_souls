#include "GsUIPopupTrade.h"

#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"

#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Common/Inventory/GsUIInventoryCommon.h"
#include "UI/UIContent/Common/Inventory/GsInventoryLogicTrade.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/ScopeGame/GsPersonalTradingManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/GsMessageHolder.h"

#include "Item/GsItemEquipment.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Currency/GsCurrencyHelper.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"


void UGsUIPopupTrade::BeginDestroy()
{
	if (_inventoryLogic)
	{
		delete _inventoryLogic;
		_inventoryLogic = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupTrade::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupTrade::OnClickClose);
	_btnLock->OnClicked.AddDynamic(this, &UGsUIPopupTrade::OnClickLock);
	_btnConfirm->OnClicked.AddDynamic(this, &UGsUIPopupTrade::OnClickConfirm);
	_btnDiaLocal->OnClicked.AddDynamic(this, &UGsUIPopupTrade::OnClickDiaLocal);


	_iconSlotHelperTarget = NewObject<UGsDynamicIconSlotHelper>(this);
	if (nullptr != _iconSlotHelperTarget)
	{
		_iconSlotHelperTarget->Initialize(_iconSelectorTarget);
		_iconSlotHelperTarget->OnCreateIcon.AddDynamic(this, &UGsUIPopupTrade::OnCreateIcon);
		_iconSlotHelperTarget->OnRefreshIcon.AddDynamic(this, &UGsUIPopupTrade::OnRefreshIconRemote);
	}
	_iconSlotHelperLocal = NewObject<UGsDynamicIconSlotHelper>(this);
	if (nullptr != _iconSlotHelperLocal)
	{
		_iconSlotHelperLocal->Initialize(_iconSelectorLocal);
		_iconSlotHelperLocal->OnCreateIcon.AddDynamic(this, &UGsUIPopupTrade::OnCreateIcon);
		_iconSlotHelperLocal->OnRefreshIcon.AddDynamic(this, &UGsUIPopupTrade::OnRefreshIconLocal);
	}

	_inventoryLogic = new FGsInventoryLogicTrade();
}

void UGsUIPopupTrade::NativeConstruct()
{
	Super::NativeConstruct();
	RegisterMessages();

	FGsPersonalTradingManager* tradingManager = GSPersonalTrading();

	// 인벤토리
	_inventory->SetData(_inventoryLogic);
	_inventory->InvalidateData();

	// 상단 상대 유저 이름
	_textBlockUserNameTarget->SetText(FText::FromString(tradingManager->GetRemoteUserName()));

	// 거래 수수료 : 내가 열면 수수료 존재, 상대가 열면 수수료 없음
	CurrencyDeltaPair tradeChargeData = tradingManager->GetMyTradeChargeData();
	_slotTradeCharge->SetData(tradeChargeData.mCurrencyType, tradeChargeData.mCurrencyDelta);

	// 일일 거래 가능 횟수
	int32 remainDailyTradeNumber = tradingManager->GetLocalRemainDailyTradeNumber();
	int32 dailyTradeNumberMax = tradingManager->GetDailyTradeNumberMax();
	FText dailyTradeText;
	FGsUIStringHelper::GetTwoValueSlashedText(remainDailyTradeNumber, dailyTradeNumberMax, dailyTradeText);
	_textBlockRemainDailyCount->SetText(dailyTradeText);

	InvalidateTarget();
	InvalidateLocal();
}

void UGsUIPopupTrade::NativeDestruct()
{
	UnregisterMessages();
	if (FGsPersonalTradingManager* tradeManager = GSPersonalTrading())
	{
		tradeManager->ClearPersonalTrading();
	}
	Super::NativeDestruct();
}

void UGsUIPopupTrade::OnInputCancel()
{
	OnClickClose();
}

void UGsUIPopupTrade::RegisterMessages()
{
	FGsMessageHolder* msg = GMessage();

	_msgTradeHandlerList.Emplace(msg->GetPersonalTrading().AddUObject(MessageContentPersoncalTrading::INVALIDATE_TARGET_STATE,
		this, &UGsUIPopupTrade::InvalidateTarget));
	_msgTradeHandlerList.Emplace(msg->GetPersonalTrading().AddUObject(MessageContentPersoncalTrading::INVALIDATE_TARGET_ITEM,
		this, &UGsUIPopupTrade::InvalidateTarget));
	_msgTradeHandlerList.Emplace(msg->GetPersonalTrading().AddUObject(MessageContentPersoncalTrading::INVALIDATE_TARGET_DIA,
		this, &UGsUIPopupTrade::InvalidateTarget));
	_msgTradeHandlerList.Emplace(msg->GetPersonalTrading().AddUObject(MessageContentPersoncalTrading::INVALIDATE_LOCAL_STATE,
		this, &UGsUIPopupTrade::InvalidateLocal));
	_msgTradeHandlerList.Emplace(msg->GetPersonalTrading().AddUObject(MessageContentPersoncalTrading::INVALIDATE_LOCAL_ITEM,
		this, &UGsUIPopupTrade::InvalidateInventory));
	_msgTradeHandlerList.Emplace(msg->GetPersonalTrading().AddUObject(MessageContentPersoncalTrading::INVALIDATE_LOCAL_DIA,
		this, &UGsUIPopupTrade::InvalidateLocal));
	_msgTradeHandlerList.Emplace(msg->GetPersonalTrading().AddUObject(MessageContentPersoncalTrading::CLOSE,
		this, &UGsUIPopupTrade::OnClose));
	_msgTradeHandlerList.Emplace(msg->GetPersonalTrading().AddUObject(MessageContentPersoncalTrading::CLOSE_POPUP_CHAT,
		this, &UGsUIPopupTrade::OnCloseTradeChat));
	_msgTradeHandlerList.Emplace(msg->GetPersonalTrading().AddUObject(MessageContentPersoncalTrading::CLOSE_ABORT,
		this, &UGsUIPopupTrade::OnClickClose));

	if (FGsGameFlowManager* mode = GMode())
	{
		if (FGsGameFlowGame* gameFlow = mode->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				contents->OnEnterStateEvent().AddUObject(this, &UGsUIPopupTrade::OnContentsEnterStateEvent);
			}
		}
	}
}

void UGsUIPopupTrade::UnregisterMessages()
{
	FGsMessageHolder* msg = GMessage();

	for (TPair<MessageContentPersoncalTrading, FDelegateHandle>& iter : _msgTradeHandlerList)
	{
		msg->GetPersonalTrading().Remove(iter);
	}
	_msgTradeHandlerList.Empty();

	if (FGsGameFlowManager* mode = GMode())
	{
		if (FGsGameFlowGame* gameFlow = mode->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				contents->OnEnterStateEvent().RemoveAll(this);
			}
		}
	}
}

void UGsUIPopupTrade::InvalidateAllInternal()
{
	InvalidateTarget();
	InvalidateLocal();
	InvalidateInventory();
}

void UGsUIPopupTrade::InvalidateTarget()
{
	FGsPersonalTradingManager* tradingManager = GSPersonalTrading();
	if (nullptr == tradingManager)
		return;

	int32 slotCountMax = tradingManager->GetSlotCountMax();

	// 상대 정보 갱신
	PersonalTradingState localState = tradingManager->GetLocalPersonalTradingState();
	PersonalTradingState targetState = tradingManager->GetRemotePersonalTradingState();
	switch (targetState)
	{
	case PersonalTradingState::LOCK:
		// 잠금
		_switcherStateTarget->SetActiveWidgetIndex(1);
		_panelDiaTarget->SetIsEnabled(false);
		break;
	case PersonalTradingState::CONFIRM:
		// 거래 수락
		_switcherStateTarget->SetActiveWidgetIndex(2);
		_panelDiaTarget->SetIsEnabled(false);
		break;
	default:
		// 준비중
		_switcherStateTarget->SetActiveWidgetIndex(0);
		_panelDiaTarget->SetIsEnabled(true);
		break;
	}

	_iconSlotHelperTarget->RefreshAll(slotCountMax);
	_slotDiaTarget->SetData(CurrencyType::DIA, tradingManager->GetRemoteDiaAmount());

	_btnLock->SetSwitcherIndex((localState == PersonalTradingState::TRADING) ? 0 : 1);
	_btnLock->SetIsEnabled((localState == PersonalTradingState::CONFIRM) ? false : true);

	_btnConfirm->SetSwitcherIndex((localState == PersonalTradingState::CONFIRM) ? 1 : 0);
	_btnConfirm->SetIsEnabled((localState == PersonalTradingState::TRADING || targetState == PersonalTradingState::TRADING) ? false : true);
}

void UGsUIPopupTrade::InvalidateLocal()
{
	FGsPersonalTradingManager* tradingManager = GSPersonalTrading();
	if (nullptr == tradingManager)
		return;

	int32 slotCountMax = tradingManager->GetSlotCountMax();

	// 내 정보 갱신
	PersonalTradingState localState = tradingManager->GetLocalPersonalTradingState();
	PersonalTradingState targetState = tradingManager->GetRemotePersonalTradingState();
	switch (localState)
	{
	case PersonalTradingState::LOCK:
		// 잠금
		_switcherStateLocal->SetActiveWidgetIndex(1);
		_panelDiaLocal->SetIsEnabled(false);
		break;
	case PersonalTradingState::CONFIRM:
		// 거래 수락
		_switcherStateLocal->SetActiveWidgetIndex(2);
		_panelDiaLocal->SetIsEnabled(false);
		break;
	default:
		// 준비중
		_switcherStateLocal->SetActiveWidgetIndex(0);
		_panelDiaLocal->SetIsEnabled(true);
		break;
	}
	_iconSlotHelperLocal->RefreshAll(slotCountMax);
	_slotDiaLocal->SetData(CurrencyType::DIA, tradingManager->GetLocalDiaAmount());
	
	_btnLock->SetSwitcherIndex((localState == PersonalTradingState::TRADING) ? 0 : 1);
	_btnLock->SetIsEnabled((localState == PersonalTradingState::CONFIRM) ? false : true);

	_btnConfirm->SetSwitcherIndex((localState == PersonalTradingState::CONFIRM) ? 1 : 0);
	_btnConfirm->SetIsEnabled((localState == PersonalTradingState::TRADING || targetState == PersonalTradingState::TRADING) ? false : true);
}

void UGsUIPopupTrade::InvalidateInventory()
{
	InvalidateLocal();

	_inventory->InvalidateData();
}

void UGsUIPopupTrade::OnInvalidateUI(const IGsMessageParam* InParam)
{
	InvalidateInventory();
}

void UGsUIPopupTrade::OnClickClose()
{
	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	if (tradeManager->TryReqPersonalTradingAbort())
	{
		tradeManager->SetCancelType(PersonalTradingCancelType::CLICK);
	}
}

void UGsUIPopupTrade::OnCloseTradeChat()
{
	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	if (tradeManager->TryReqPersonalTradingAbort())
	{
		tradeManager->SetCancelType(PersonalTradingCancelType::CLICK);
	}
}

void UGsUIPopupTrade::OnClickLock()
{
	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	const int32 BUTTON_LOCK = 0;
	if (_btnLock->GetSwitcherIndex() == BUTTON_LOCK)
	{
		// 거래 잠금 요청
		tradeManager->TryReqPersonalTradingLock();
	}
	else
	{
		// 거래 잠금 해제 요청
		tradeManager->TryReqPersonalTradingLockCancel();
	}
}

void UGsUIPopupTrade::OnClickConfirm()
{
	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	const int32 INDEX_CONFIRMED = 1;
	if (_btnConfirm->GetSwitcherIndex() == INDEX_CONFIRMED)
	{
		// 거래 확인 해제 요청
		tradeManager->TryReqPersonalTradingConfirmCancel();
	}
	else
	{
		// 거래 확인 요청
		tradeManager->TryReqPersonalTradingConfirm();
	}
}

void UGsUIPopupTrade::OnClickDiaLocal()
{
	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	if (tradeManager->GetLocalPersonalTradingState() != PersonalTradingState::TRADING)
		return;

	// 이미 요청중인지 확인
	if (tradeManager->IsRequestingDia())
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCalculator"));
	if (!widget.IsValid())
		return;

	UGsUIPopupCalculator* popupCalculator = Cast<UGsUIPopupCalculator>(widget.Get());
	if (nullptr == popupCalculator)
		return;

	Gem currentDiaAmount = tradeManager->GetLocalDiaAmount();
	CurrencyDeltaPair tradeChargeData = tradeManager->GetMyTradeChargeData();

	popupCalculator->SetDimmed(true);
	popupCalculator->SetResetCount();
	popupCalculator->SetMinimumCount(0);
	popupCalculator->SetMaximumCount(FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::DIA) - tradeChargeData.mCurrencyDelta);
	popupCalculator->SetDefaultCount(currentDiaAmount);
	popupCalculator->InvalidateNumText();

	if (!popupCalculator->OnFinishCalculateDelegate.IsBoundToObject(this))
	{
		popupCalculator->OnFinishCalculateDelegate.AddUObject(this, &UGsUIPopupTrade::OnFinishCalculate);
	}
}

void UGsUIPopupTrade::OnClose()
{
	Close();
}

void UGsUIPopupTrade::OnFinishCalculate(int64 InValue)
{
	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	Gem currentDiaAmount = tradeManager->GetLocalDiaAmount();
	if (currentDiaAmount != InValue)
	{
		Gem sendAmount = FMath::Min<Gem>(FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::DIA), InValue);
		tradeManager->ReqLocalDiaAmount(sendAmount);
	}
}

void UGsUIPopupTrade::OnCreateIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemCount(0);
	itemIcon->SetCoolTimeBg(false);
}

void UGsUIPopupTrade::OnRefreshIconRemote(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory * itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	ItemDBId itemDBId = tradeManager->GetRemoteItemDBId(InIndex);
	const ItemData* itemData = nullptr;
	const ItemDataEquip* itemDataEquip = tradeManager->GetRemoteItemDataEquip(itemDBId);
	if (nullptr == itemDataEquip)
	{
		itemData = tradeManager->GetRemoteItemData(itemDBId);
	}
	if (nullptr == itemData && nullptr == itemDataEquip)
	{
		// 빈 아이콘 설정.
		itemIcon->SetEmptyIcon();
		return;
	}

	if (nullptr != itemDataEquip)
	{
		itemIcon->SetNotOwnItem(itemDataEquip->mTemplateId, itemDataEquip->mItemAmount, itemDataEquip->mLevel, itemDataEquip->mEnchantShield, itemDataEquip->mEnchantBonusIndex);
		FGsItem* iconItemData = itemIcon->GetItemIconData();
		FGsItemEquipment* itemIconEquipData = iconItemData->GetCastItem<FGsItemEquipment>();
		itemIconEquipData->UpdateBy(itemDataEquip);
		itemIconEquipData->SetDBID(INVALID_ITEM_DB_ID);
		itemIcon->SetSlotEffectMagicalForge(iconItemData);
	}
	else if (nullptr != itemData)
	{
		itemIcon->SetNotOwnItem(itemData->mTemplateId, itemData->mItemAmount);
	}
	
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetSlotEquipedImage(false);
	itemIcon->SetPresetImage(false);
	itemIcon->SetRedDot(false);
	itemIcon->SetItemNameToolTipHoverMode(true);
	itemIcon->SetItemNameToolTipFlag(true);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::OnClickType);
	itemIcon->SetVisibility(ESlateVisibility::Visible);

	itemIcon->SetDimmedImage(GSPersonalTrading()->GetRemotePersonalTradingState() != PersonalTradingState::TRADING);

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupTrade::OnLongPressIcon);
	}

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIPopupTrade::OnClickIconTarget);
	}
}

void UGsUIPopupTrade::OnRefreshIconLocal(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	const TArray<TPair<ItemDBId, ItemAmount>>& itemDataList = GSPersonalTrading()->GetLocalItemDataList();
	if (itemDataList.IsValidIndex(InIndex))
	{
		itemIcon->SetOwnedItem(itemDataList[InIndex].Key);
		itemIcon->SetItemCount(itemDataList[InIndex].Value);

		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameVisibility(false);
		itemIcon->SetSlotEquipedImage(false);
		itemIcon->SetPresetImage(false);
		itemIcon->SetRedDot(false);
		itemIcon->SetItemNameToolTipHoverMode(true);
		itemIcon->SetItemNameToolTipFlag(true);
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
		itemIcon->SetVisibility(ESlateVisibility::Visible);

		itemIcon->SetDimmedImage(GSPersonalTrading()->GetLocalPersonalTradingState() != PersonalTradingState::TRADING);

		if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupTrade::OnLongPressIcon);
		}

		if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
		{
			itemIcon->OnClickSlot.AddUObject(this, &UGsUIPopupTrade::OnClickIconLocal);
		}
	}
	else
	{
		// 빈 아이콘 설정.
		itemIcon->SetEmptyIcon();
	}
}

void UGsUIPopupTrade::OnClickIconTarget(UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParam param(InIcon.GetItemIconData(), false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos, false, false);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
}

void UGsUIPopupTrade::OnClickIconLocal(UGsUIIconItem& InIcon)
{
	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	if (tradeManager->GetLocalPersonalTradingState() != PersonalTradingState::TRADING)
		return;

	// 이미 요청중인지 확인
	if (tradeManager->IsRequestingItem(InIcon.GetDBID()))
		return;

	if (InIcon.GetDBID() == INVALID_ITEM_DB_ID)
		return;

	// 등록 해제
	tradeManager->ReqLocalItemUpdate(InIcon.GetDBID(), 0);
}

void UGsUIPopupTrade::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParam param(InIcon.GetItemIconData(), true, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos, false, false);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
}

void UGsUIPopupTrade::OnContentsEnterStateEvent(FGsContentsMode::InGame InStateType)
{
	if (InStateType != FGsContentsMode::InGame::ContentsHud)
	{
		OnClickClose();
	}
}