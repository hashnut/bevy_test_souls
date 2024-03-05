// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/ItemEnchant/GsUIItemMultiEnchantGroup.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemIngredient.h"
#include "Item/GsItemManager.h"
#include "GsUIMultiEnchantCountEntry.h"
#include "GsUIMultiEnchantEquipItemEntry.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Message/MessageParam/GsEnchantMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsItemEnchantManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "UI/UILib/Manager/GsTooltipManager.h"
#include "Currency/GsCurrencyHelper.h"
#include "Net/GsNetSendServiceItem.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"



void UGsUIItemMultiEnchantGroup::BeginDestroy()
{
	if (nullptr != _ingredientIconSlotHelper)
	{
		_ingredientIconSlotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIItemMultiEnchantGroup::OnRefreshTargetIcon);
		_ingredientIconSlotHelper->Clear();
		_ingredientIconSlotHelper = nullptr;
	}
	if (nullptr != _expectCountListPanelSlotHelper)
	{
		_expectCountListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIItemMultiEnchantGroup::EnchantCountListRefresh);
		_expectCountListPanelSlotHelper = nullptr;
	}
	if (nullptr != _equipItemListPanelSlotHelper)
	{
		_equipItemListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIItemMultiEnchantGroup::EnchantEquipItemListRefresh);
		_equipItemListPanelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIItemMultiEnchantGroup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _ingredientIconSlotHelper)
	{
		_ingredientIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_ingredientIconSlotHelper->Initialize(_ingredientItemIconSelector);
		_ingredientIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIItemMultiEnchantGroup::OnRefreshTargetIcon);
	}
	if (nullptr == _expectCountListPanelSlotHelper)
	{
		_expectCountListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_expectCountListPanelSlotHelper->Initialize(_expectCountListEntryWidgetClass, _enchantExpectCountListRoot);
		_expectCountListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIItemMultiEnchantGroup::EnchantCountListRefresh);
	}
	if (nullptr == _equipItemListPanelSlotHelper)
	{
		_equipItemListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_equipItemListPanelSlotHelper->Initialize(_equipItemListEntryWidgetClass, _equipItemListRoot);
		_equipItemListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIItemMultiEnchantGroup::EnchantEquipItemListRefresh);
	}

	_btnStopEnchant->OnClicked.AddDynamic(this, &UGsUIItemMultiEnchantGroup::OnclickStopEnchant);
	_btnDetailInfo->OnPressed.AddDynamic(this, &UGsUIItemMultiEnchantGroup::OnclickDetailInfo);
	_btnCurrency->OnClickCurrencyButton.BindUObject(this, &UGsUIItemMultiEnchantGroup::OnclickEnchant);
}

void UGsUIItemMultiEnchantGroup::NativeConstruct()
{
	Super::NativeConstruct();

	_ingredientItemIconSelector->RemoveAllChildren();

	_btnSwitcher->SetActiveWidgetIndex(0);
	_selectedExpectEnchantCount = -1;
	_expectCountListPanelSlotHelper->AllClearChildren();
	_equipItemListPanelSlotHelper->AllClearChildren();

	_inputCheckbox->SetIsSelected(false);
	_inputCheckbox->OnCheckStateChangedEx.AddDynamic(this, &UGsUIItemMultiEnchantGroup::OnChangeCheckbox);

	FText findText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Button_Enchant"), findText);
	_btnCurrency->SetButtonName(findText);

	_isAutoEnchant = false;
	_isSendingPacket = false;
	_reserveRemoveItemDbid = 0;

	if (_tooltipDescWidget)
	{
		FText textDesc;
		FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_EnchantMulti_Tooltip"), textDesc);
		_tooltipDescWidget->SetWrapTextOption(800.f); // 글자가 길어져 화면 밖으로 못나가게 강제 줄바꿈 설정
		_tooltipDescWidget->SetDescText(textDesc);
		// 2023/4/26 PKT - https://jira.com2us.com/jira/browse/C2URWQ-4019 툴팁 기존 변경으로 시간값 주석
		//_tooltipDescWidget->SetLifeTime(3.f);
		_tooltipDescWidget->Close();
	}
}

void UGsUIItemMultiEnchantGroup::NativeDestruct()
{
	if (true == OnEquipItemSlot.IsBound())
	{
		OnEquipItemSlot.Clear();
	}

	_packetEquipItemDbidList.Empty();
	_packetIngredientItemDbidList.Empty();
	_packetTargetLevel = 0;
	_equipSlotEffectInfoMap.Empty();
	_targetItemDbidList.Empty();
	_ingredientDataMap.Empty();
	_expectEnchantStateList.Empty();

	_ingredientItemIconSelector->RemoveAllChildren();
	_inputCheckbox->OnCheckStateChangedEx.RemoveDynamic(this, &UGsUIItemMultiEnchantGroup::OnChangeCheckbox);

	GetWorld()->GetTimerManager().ClearTimer(_equipSlotWaitEffectTimer);

	Super::NativeDestruct();
}

void UGsUIItemMultiEnchantGroup::SetMultiEnchantInfo(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDBId)
{
	_targetItemDbidList.Empty();
	_ingredientDataMap.Empty();

	if (UGsItemManager* itemManager = GItem())
	{
		TWeakPtr<FGsItem> findEquipItemData = itemManager->FindItem(InTargetItemDbid, ItemType::EQUIP);
		if (findEquipItemData.IsValid())
		{
			_targetItemDbidList.Emplace(InTargetItemDbid);
		}

		TWeakPtr<FGsItem> findIngredientData = itemManager->FindItem(InIngredientItemDBId, ItemType::INGREDIENT);
		if (findIngredientData.IsValid())
		{
			if (FGsItemIngredient* ingredientItem = findIngredientData.Pin()->GetCastItem<FGsItemIngredient>())
			{
				if (findEquipItemData.IsValid())
				{
					if (ingredientItem->IsMatchedEnchantTargetItem(findEquipItemData.Pin()->GetSubCategory()))
					{
						_ingredientDataMap.Emplace(findIngredientData.Pin()->GetDBID(),
							TPair<ItemId, ItemAmount>(findIngredientData.Pin()->GetTID(), findIngredientData.Pin()->GetAmount()));
					}
				}
				else
				{
					_ingredientDataMap.Emplace(findIngredientData.Pin()->GetDBID(),
						TPair<ItemId, ItemAmount>(findIngredientData.Pin()->GetTID(), findIngredientData.Pin()->GetAmount()));
				}
			}
		}
	}
	
	InvalidateAll();
}

bool UGsUIItemMultiEnchantGroup::IsUpdateInfo(const ItemDBId InItemDbid)
{
	return INDEX_NONE != _targetItemDbidList.Find(InItemDbid) || _ingredientDataMap.Contains(InItemDbid);
}

void UGsUIItemMultiEnchantGroup::RemoveItemInfo(const ItemDBId InRemoveItemDbid)
{
	bool isNeedValidation = false;
	isNeedValidation = (false == _isAutoEnchant) && (INDEX_NONE != _targetItemDbidList.Find(InRemoveItemDbid) || _ingredientDataMap.Contains(InRemoveItemDbid));
	if (isNeedValidation)
	{
		if (false == _isAutoEnchant)
		{
			if (INDEX_NONE != _targetItemDbidList.Find(InRemoveItemDbid))
			{
				_targetItemDbidList.Remove(InRemoveItemDbid);
			}
			else if (_ingredientDataMap.Contains(InRemoveItemDbid))
			{
				_ingredientDataMap.Remove(InRemoveItemDbid);
			}

			InvalidateAll();
			if (OnEquipItemSlot.IsBound())
			{
				OnEquipItemSlot.Broadcast();
			}
		}
		else
		{
			_reserveRemoveItemDbid = InRemoveItemDbid;
		}
	}
}

void UGsUIItemMultiEnchantGroup::UpdateMultiEnchantInfo(const ItemDBId InItemDbid)
{
	if (_isAutoEnchant)
		return;

	if (INDEX_NONE != _targetItemDbidList.Find(InItemDbid) || _ingredientDataMap.Contains(InItemDbid))
	{
		TWeakPtr<FGsItem> findItem = GItem()->FindItem(InItemDbid);
		if (findItem.IsValid())
		{
			if (findItem.Pin()->GetLock())
			{
				if (_ingredientDataMap.Contains(InItemDbid))
				{
					_ingredientDataMap.Remove(InItemDbid);
				}
				else
				{
					_targetItemDbidList.Remove(InItemDbid);
				}
			}
			else
			{
				if (_ingredientDataMap.Contains(InItemDbid))
				{
					TPair<ItemId, ItemAmount>& data = _ingredientDataMap[InItemDbid];
					data.Value = findItem.Pin()->GetAmount();
				}
			}

			InvalidateAll();
			if (OnEquipItemSlot.IsBound())
			{
				OnEquipItemSlot.Broadcast();
			}
		}
	}
}

void UGsUIItemMultiEnchantGroup::OnClickInventory(TWeakPtr<FGsItem> InInvenItemData)
{
	if (InInvenItemData.Pin()->IsEquipment())
	{
		if (INDEX_NONE != _targetItemDbidList.Find(InInvenItemData.Pin()->GetDBID()))
		{
			_targetItemDbidList.Remove(InInvenItemData.Pin()->GetDBID());
		}
		else
		{
			if (EQUIPITEM_SLOT_NUM <= _targetItemDbidList.Num())
				return;

			_targetItemDbidList.Emplace(InInvenItemData.Pin()->GetDBID());
		}
	}
	else
	{
		if(_ingredientDataMap.Contains(InInvenItemData.Pin()->GetDBID()))
		{
			if (_inputCheckbox->GetIsSelected())
			{
				_ingredientDataMap.Empty();
			}
			else
			{
				_ingredientDataMap.Remove(InInvenItemData.Pin()->GetDBID());
			}
		}
		else
		{
			bool isReset = false;
			UGsItemManager* itemMgr = GItem();
			FGsItemIngredient* invenIngredientItem = InInvenItemData.Pin()->GetCastItem<FGsItemIngredient>();
			for (const TPair<ItemDBId, TPair<ItemId, ItemAmount>> iter : _ingredientDataMap)
			{
				TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter.Key,ItemType::INGREDIENT);
				if (findItem.IsValid())
				{
					FGsItemIngredient* ingredientItem = findItem.Pin()->GetCastItem<FGsItemIngredient>();
					if (false == ingredientItem->IsMatchedEnchantScrollItem(invenIngredientItem->FindFirstEffectId()))
					{
						isReset = true;
						break;
					}
					/*
					if (findItem.Pin()->GetSubCategory() != InInvenItemData.Pin()->GetSubCategory())
					{
						isReset = true;
						break;
					}
					*/
				}
			}
			if (isReset)
			{
				_ingredientDataMap.Empty();
			}

			// 일괄선택 선택되어있으면 일괄로 움직임 - 선택/해지 모두
			if (_inputCheckbox->GetIsSelected())
			{
				AddAllIngredientItems(InInvenItemData);
			}
			else
			{
				_ingredientDataMap.Emplace(InInvenItemData.Pin()->GetDBID(),
					TPair<ItemId, ItemAmount>(InInvenItemData.Pin()->GetTID(), InInvenItemData.Pin()->GetAmount()));
			}
		}
	}
	InvalidateAll();
}

void UGsUIItemMultiEnchantGroup::UpdateEquipItemList()
{
	TArray<ItemDBId> removeList;
	const FGsItem* itemData = GetRegisterIngredientItemData();
	if (nullptr != itemData)
	{
		const FGsItemIngredient* ingredientData = static_cast<const FGsItemIngredient*>(itemData);
		if (nullptr == ingredientData)
			return;

		UGsItemManager* itemMgr = GItem();
		for (ItemDBId iter : _targetItemDbidList)
		{
			TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter, ItemType::EQUIP);
			if (findItem.IsValid())
			{
				if (false == ingredientData->IsMatchedEnchantTargetItem(findItem.Pin()->GetSubCategory()))
				{
					removeList.Emplace(iter);
				}
			}
		}
	}
	for (ItemDBId iter : removeList)
	{
		_targetItemDbidList.Remove(iter);
	}
}

void UGsUIItemMultiEnchantGroup::InvalidateAll()
{
	_ingredientIconSlotHelper->RefreshAll(1);

	UpdateEquipItemList();
	GSItemEnchant()->FindMultiEnchantExpectNumState(_targetItemDbidList, GetSelectedIngredientTID(), EXPECT_ENCHANT_NUM, _expectEnchantStateList);
	
	UpdateExpectEnchantCount();
	UpdateNeedIngredientCount();
	UpdateCurrencyBtnInfo();
	UpdateGuideText();

	_equipItemListPanelSlotHelper->RefreshAll(EQUIPITEM_SLOT_NUM);
	_expectCountListPanelSlotHelper->RefreshAll(EXPECT_ENCHANT_NUM);
}

void UGsUIItemMultiEnchantGroup::UpdateExpectEnchantCount()
{	
	_selectedExpectEnchantCount = (IsEnchantValidState() && IsValidQuickEnchantNum()) ? _selectedExpectEnchantCount : -1;
	FText markText;
	FText::FindText(TEXT("MarkFormatingText"), TEXT("Plus"), markText);
	FString result = FString::Format(TEXT("{0}{1}"), { *markText.ToString() , FString::FormatAsNumber(0 >= _selectedExpectEnchantCount ? 0 : _selectedExpectEnchantCount)});
	_expectEnchantCountText->SetText(FText::FromString(result));
}

void UGsUIItemMultiEnchantGroup::UpdateNeedIngredientCount()
{
	if (IsEnchantValidState())
	{
		const FGsItem* itemData = GetRegisterIngredientItemData();
		if (itemData)
		{
			ItemAmount count = GSItemEnchant()->GetMultiEnchantNeedScrollCount(_targetItemDbidList, itemData->IsCursedEnchantScrollItem(), 
				_selectedExpectEnchantCount);

			_needIngredientCountText->SetText(FText::FromString(FString::FromInt(count)));
			_needIngredientCountText->SetColorAndOpacity(FGsUIColorHelper::GetColor(count > GetSelectedIngredientTotalAmount() ?
				EGsUIColorType::DEFAULT_LACK : EGsUIColorType::DEFAULT_ENOUGH));

			return;
		}
	}
	_needIngredientCountText->SetText(FText::AsNumber(0));
	_needIngredientCountText->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_ENOUGH));

	//DEFAULT_ENOUGH UMETA(DisplayName = "DEFAULT_ENOUGH(기본 충분함 컬러)"),
	//DEFAULT_LACK UMETA(DisplayName = "DEFAULT_LACK(기본 부족함 컬러)"),
}

void UGsUIItemMultiEnchantGroup::UpdateCurrencyBtnInfo()
{
	if (IsEnchantValidState() && IsValidQuickEnchantNum() && 0 <= _selectedExpectEnchantCount)
	{
		Currency needCost = GetNeedEnchantCost();
		_btnCurrency->SetIsEnabled(needCost > 0);
		_btnCurrency->SetData(CurrencyType::GOLD, needCost);
		_btnCurrency->UpdateAmountTextColorLocalPlayer();
	}
	else
	{
		_btnCurrency->SetData(CurrencyType::GOLD, 0);
		_btnCurrency->SetIsEnabled(false);
	}
}

void UGsUIItemMultiEnchantGroup::UpdateGuideText()
{
	if (IsEnchantValidState() && IsValidQuickEnchantNum() && 0 <= _selectedExpectEnchantCount)
	{
		_enchantGuide->SetVisibility(ESlateVisibility::Visible);

		FText findDestroyText;
		FText findSafeText;
		FText preservedText;
		FText resultText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_EquipDestroy"), findDestroyText);
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_EquipSafe"), findSafeText);
		FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_Common_AlertPreservation"), preservedText);
		if (IsRegisterCursed())
		{
			_enchantGuide->SetText(findSafeText);
			_enchantGuide->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::ENCHANT_WARNING_NORMAL));
			
		}
		else
		{
			//  0 : safe / 1 : preserved / 2 : destroy
			uint8 result = GSItemEnchant()->IsRiskEnchant(_targetItemDbidList, _selectedExpectEnchantCount);
			switch (result)
			{
			case 0:
				resultText = findSafeText;
				break;
			case 1:
				resultText = preservedText;
				break;
			case 2:
				resultText = findDestroyText;
				break;

			default:
				resultText = findSafeText;
				break;
			}
			_enchantGuide->SetText(resultText);
			_enchantGuide->SetColorAndOpacity(FGsUIColorHelper::GetColor(2 == result ? EGsUIColorType::ENCHANT_WARNING_DESTROY : EGsUIColorType::ENCHANT_WARNING_NORMAL));
		}
	}
	else
	{
		_enchantGuide->SetVisibility(ESlateVisibility::Collapsed);
	}
}

Currency UGsUIItemMultiEnchantGroup::GetNeedEnchantCost()
{
	Currency resultcost = 0;
	UGsItemManager* itemMgr = GItem();
	FGsItemEnchantManager* enchantMgr = GSItemEnchant();
	for (ItemDBId iter : _targetItemDbidList)
	{
		TWeakPtr<FGsItem> findItemData = itemMgr->FindItem(iter, ItemType::EQUIP);
		if (findItemData.IsValid() && findItemData.Pin()->GetCastItem<FGsItemEquipment>())
		{
			FGsItemEquipment* equipData = findItemData.Pin()->GetCastItem<FGsItemEquipment>();
			ItemAmount needAmount = enchantMgr->GetMultiEnchantNeedScrollCount(findItemData, IsRegisterCursed(), _selectedExpectEnchantCount);
			resultcost += equipData->GetEnchantCostValue() * needAmount;
		}
	}
	return resultcost;
}

void UGsUIItemMultiEnchantGroup::OnClickReset()
{
	if (_isAutoEnchant)
		return;

	if (0 < _targetItemDbidList.Num() || 0 < _ingredientDataMap.Num())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNo"));
		if (false == widget.IsValid())
		{
			return;
		}
		UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get());
		if (nullptr == popup)
		{
			return;
		}
		
		// TEXT: 등록된 아이템을 초기화 하시겠습니까
		FText textDesc;
		FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_EnchantMulti_Popup_Reset"), textDesc);
		popup->SetData(textDesc, [&, this](bool bIsOk)
			{
				if (bIsOk)
				{
					OnConfirmResetBtn();
				}
			});
	}
}

void UGsUIItemMultiEnchantGroup::OnConfirmResetBtn()
{
	_targetItemDbidList.Empty();
	_ingredientDataMap.Empty();
	InvalidateAll();

	if (OnEquipItemSlot.IsBound())
	{
		OnEquipItemSlot.Broadcast();
	}
}

void UGsUIItemMultiEnchantGroup::OnClickRegisterSameItem()
{
	AddSameEquipItems();
}

void UGsUIItemMultiEnchantGroup::OnclickEnchant()
{
	if (_isAutoEnchant || _isSendingPacket)
		return;

	if (IsEnchantValidState() && IsValidQuickEnchantNum() && 0 <= _selectedExpectEnchantCount)
	{
		if (UGsTooltipManager* tooltipManager = GUI()->GetTooltipManager())
		{
			// 다른 이름 툴팁 닫기
			tooltipManager->CloseNameTooltip();
		}

		// 재화 부족 팝업
		Currency cost = _btnCurrency->GetCurrencyAmount();
		if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::GOLD, cost, false))
		{
			Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
			FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, cost - myAmount);
			return;
		}

		FGsItemEnchantManager* enchantMgr = GSItemEnchant();
		// 주문서 수량 체크
		ItemAmount count = enchantMgr->GetMultiEnchantNeedScrollCount(_targetItemDbidList, IsRegisterCursed(),
			_selectedExpectEnchantCount);
		if (count > GetSelectedIngredientTotalAmount())
		{
			// 스크롤 갯수가 부족합니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_NotEnoughScroll"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		// 재화 체크
		uint64 ownedCurrency = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
		if (ownedCurrency < GetNeedEnchantCost())
		{
			// 돈이 없슴
			//FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, GetNeedEnchantCost() - ownedCurrency);
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUI_Notice_NotEnoughGold"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		for (const uint64 iter : _targetItemDbidList)
		{
			if (enchantMgr->IsNotAllowedMultiEnchantItem(iter))
			{
				// 아이템 특성 상, 다중강화가 안되는 아이템에 대해서는 별도 티커 처리
				FText findText;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_CannotMultiEnchant"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return;
			}
		}

		bool state = (false == IsRegisterCursed() && 2 == enchantMgr->IsRiskEnchant(_targetItemDbidList, _selectedExpectEnchantCount));
		if (state)
		{
			// 인벤슬롯 혹은 무게 여유가 없을때 처리
			if (false == enchantMgr->IsOkInvenCondition())
			{
				FText findText;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_cannot_try"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return;
			}

			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNo"));
			if (false == widget.IsValid())
			{
				return;
			}
			UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get());
			if (nullptr == popup)
			{
				return;
			}

			FText textDesc;
			FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_Common_Popup_AlertDestruction"), textDesc);
			popup->SetData(textDesc, [&, this](bool bIsOk)
				{
					if (bIsOk)
					{
						OnConfirmEnchant();
					}
				});
		}
		else
		{
			OnConfirmEnchant();
		}
	}
}

void UGsUIItemMultiEnchantGroup::OnConfirmEnchant()
{
	// valid check
	if (IsEnchantValidState() && IsValidQuickEnchantNum() && 0 <= _selectedExpectEnchantCount)
	{
		TArray<ItemDBId> equipItemDbidList;
		TArray<ItemDBIdAmountPair> ingredientItemDbidList;
		Level targetLevel = 0;
		float effectTime = 0.f;
		
		FGsItemEnchantManager* enchantMgr = GSItemEnchant();
		// 실제로 강화될 장비 아이템 별도 정리 필요
		if (enchantMgr->PrepareMultiEnchantPacketData(_targetItemDbidList, _ingredientDataMap, IsRegisterCursed(), _selectedExpectEnchantCount,
			equipItemDbidList, ingredientItemDbidList, targetLevel))
		{
			_equipSlotEffectInfoMap.Empty();
			for (int32 i = 0; i < _equipItemListPanelSlotHelper->GetChildrenCount(); ++i)
			{
				UWidget* findWidget = _equipItemListPanelSlotHelper->GetEntry(i);
				if (nullptr == findWidget)
					continue;

				UGsUIMultiEnchantEquipItemEntry* equipSlotWidget = Cast<UGsUIMultiEnchantEquipItemEntry>(findWidget);
				if (nullptr == equipSlotWidget)
					continue;

				if (INDEX_NONE != equipItemDbidList.Find(equipSlotWidget->GetItemDbid()))
				{
					effectTime = equipSlotWidget->SetEnchantWaitProcEffect(IsRegisterCursed(), _waitAnimSpeedValue);
					_equipSlotEffectInfoMap.Emplace(equipSlotWidget->GetItemDbid(), TPair<int32, bool>(equipSlotWidget->GetSlotIndex(), false));
				}
			}

			if (0 < equipItemDbidList.Num() && 0 < ingredientItemDbidList.Num())
			{
				_isAutoEnchant = true;
				_packetEquipItemDbidList = MoveTemp(equipItemDbidList);
				_packetIngredientItemDbidList = MoveTemp(ingredientItemDbidList);
				_packetTargetLevel = targetLevel;
				_btnSwitcher->SetActiveWidgetIndex(1);

				GetWorld()->GetTimerManager().SetTimer(_equipSlotWaitEffectTimer, this, &UGsUIItemMultiEnchantGroup::OnEndWaitingEffect, effectTime / _waitAnimSpeedValue /*waitAnimDuration + 0.1f*/);

				enchantMgr->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::MultiEnchant_Start, effectTime / _waitAnimSpeedValue);
			}
		}
		else
		{
			_isAutoEnchant = false;
			_packetEquipItemDbidList.Empty();
			_packetIngredientItemDbidList.Empty();
			_packetTargetLevel = 0;
			GetWorld()->GetTimerManager().ClearTimer(_equipSlotWaitEffectTimer);
			_equipItemListPanelSlotHelper->RefreshAll(EQUIPITEM_SLOT_NUM);
			_btnSwitcher->SetActiveWidgetIndex(0);
			enchantMgr->StopEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::MultiEnchant_Start);
		}
	}
}

void UGsUIItemMultiEnchantGroup::OnEndWaitingEffect()
{
	if (false == _isAutoEnchant)
	{
		GetWorld()->GetTimerManager().ClearTimer(_equipSlotWaitEffectTimer);
		return;
	}
	if (0 < _reserveRemoveItemDbid) // 강화 진행 중 다른 요인으로 아이템이 삭제 되는 경우
	{
		if (INDEX_NONE != _targetItemDbidList.Find(_reserveRemoveItemDbid))
		{
			_targetItemDbidList.Remove(_reserveRemoveItemDbid);
		}
		else if (_ingredientDataMap.Contains(_reserveRemoveItemDbid))
		{
			_ingredientDataMap.Remove(_reserveRemoveItemDbid);
		}
		_isSendingPacket = false;
		OnclickStopEnchant();
		InvalidateAll();
		_reserveRemoveItemDbid = 0;
		return;
	}

	FGsNetSendServiceItem::SendMultiEnchantItem(_packetIngredientItemDbidList, _packetEquipItemDbidList, _packetTargetLevel);
	_isSendingPacket = true;
}

void UGsUIItemMultiEnchantGroup::UpdateMultiEnchantResultInfo(TArray<ItemEnchantResultData> InEnchantResultList, TArray<ItemDBId> InUseIngredientItemDbisList)
{
	UGsItemManager* itemMgr = GItem();
	TArray<ItemDBId> removeList;
	for (TPair<ItemDBId, TPair<ItemId, ItemAmount>>& iter : _ingredientDataMap)
	{
		TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter.Key, ItemType::INGREDIENT);
		if (false == findItem.IsValid())
		{
			removeList.Emplace(iter.Key);
		}
		else
		{
			iter.Value.Value = findItem.Pin()->GetAmount();
		}
	}
	for (const ItemDBId iter : removeList)
	{
		_ingredientDataMap.Remove(iter);
	}

	for (const ItemEnchantResultData& iter : InEnchantResultList)
	{
		if (ItemEnchantResult::ENCHANT_FAILED_DESTROYED == iter.mItemEnchantResult)
		{
			_targetItemDbidList.Remove(iter.mItemDataEquip.mItemDBId);
		}

		if (_equipSlotEffectInfoMap.Contains(iter.mItemDataEquip.mItemDBId))
		{
			TPair<int32, bool> data = _equipSlotEffectInfoMap[iter.mItemDataEquip.mItemDBId];
			UWidget* findWidget = _equipItemListPanelSlotHelper->GetEntry(data.Key);
			if (nullptr != findWidget)
			{
				UGsUIMultiEnchantEquipItemEntry* equipSlotWidget = Cast<UGsUIMultiEnchantEquipItemEntry>(findWidget);
				if (nullptr != equipSlotWidget)
				{
					equipSlotWidget->OnEffect(iter.mItemEnchantResult, _resultAnimSpeedValue);
				}
			}
		}
	}
}

void UGsUIItemMultiEnchantGroup::OnFinishedEquipSlotEffect(const int32 InSlotIndex, const ItemDBId InItemDbid)
{
	if (_equipSlotEffectInfoMap.Contains(InItemDbid))
	{
		TPair<int32, bool>& data = _equipSlotEffectInfoMap[InItemDbid];
		data.Value = true;
	}

	bool isAllFinished = true;
	for (TPair<ItemDBId, TPair<int32, bool>>& iter : _equipSlotEffectInfoMap)
	{
		if (false == iter.Value.Value)
		{
			isAllFinished = false;
			break;
		}
	}
	if (isAllFinished)
	{
		ContinueEnchant();
	}
}

// 더이상 강화할 장착 아이템이 있는지 체크
// 있으면 OnConfirmEnchant() 호출
// 없으면 end 처리
void UGsUIItemMultiEnchantGroup::ContinueEnchant()
{
	// 필요한것만 갱신
	_equipSlotEffectInfoMap.Empty();
	FGsItemEnchantManager* enchantMgr = GSItemEnchant();
	_ingredientIconSlotHelper->RefreshAll(1);
	enchantMgr->FindMultiEnchantExpectNumState(_targetItemDbidList, GetSelectedIngredientTID(), EXPECT_ENCHANT_NUM, _expectEnchantStateList);
	_expectCountListPanelSlotHelper->RefreshAll(EXPECT_ENCHANT_NUM);
	UpdateExpectEnchantCount();
	UpdateNeedIngredientCount();
	UpdateCurrencyBtnInfo();
	_isSendingPacket = false;
	
	if (enchantMgr->IsContinueEnchant(_targetItemDbidList, IsRegisterCursed(), _selectedExpectEnchantCount))
	{
		if (0 != _btnSwitcher->GetActiveWidgetIndex())
		{
			bool isEnoughEnchant = true;
			// 주문서 수량 체크
			ItemAmount count = enchantMgr->GetMultiEnchantNeedScrollCount(_targetItemDbidList, IsRegisterCursed(),
				_selectedExpectEnchantCount);
			if (count > GetSelectedIngredientTotalAmount())
			{
				// 스크롤 갯수가 부족합니다
				FText findText;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_NotEnoughScroll"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				isEnoughEnchant = false;
			}

			// 재화 체크
			uint64 ownedCurrency = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
			if (ownedCurrency < GetNeedEnchantCost())
			{
				// 돈이 없슴
				//FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, GetNeedEnchantCost() - ownedCurrency);
				FText findText;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUI_Notice_NotEnoughGold"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				isEnoughEnchant = false;
			}

			if (isEnoughEnchant)
			{
				OnConfirmEnchant();
				return;
			}
		}
	}

	_isAutoEnchant = false;
	_packetEquipItemDbidList.Empty();
	_packetIngredientItemDbidList.Empty();
	_packetTargetLevel = 0;
	GetWorld()->GetTimerManager().ClearTimer(_equipSlotWaitEffectTimer);
	_equipItemListPanelSlotHelper->RefreshAll(EQUIPITEM_SLOT_NUM);
	_btnSwitcher->SetActiveWidgetIndex(0);
}

void UGsUIItemMultiEnchantGroup::OnclickStopEnchant()
{
	_btnSwitcher->SetActiveWidgetIndex(0);
	if (_isSendingPacket) // 이미 패킷을 보낸후 라면 버튼 상태만 변경하고 갱신처리까지 강제로 진행함
	{
		//GSLOG(Error, TEXT("123123323132313231323132312312313"));
	}
	else // 패킷 보내기 전 이면 취소 처리 가능
	{
		_isAutoEnchant = false;
		_packetEquipItemDbidList.Empty();
		_packetIngredientItemDbidList.Empty();
		_packetTargetLevel = 0;
		GetWorld()->GetTimerManager().ClearTimer(_equipSlotWaitEffectTimer);
		_equipItemListPanelSlotHelper->RefreshAll(EQUIPITEM_SLOT_NUM);

		// 장착 슬롯들 이펙트 취소 처리
	}
}

void UGsUIItemMultiEnchantGroup::OnChangeCheckbox(int32 InIndexGroup, bool InIsChecked)
{
	if (_isAutoEnchant)
	{
		_inputCheckbox->SetIsChecked(!InIsChecked);
		return;
	}

	 const FGsItem* registerItemData = GetRegisterIngredientItemData();
	 if (nullptr == registerItemData)
		 return;

	if (InIsChecked)
	{
		AddAllIngredientItems();
		_ingredientIconSlotHelper->RefreshAll(1);
		UpdateNeedIngredientCount();
		UpdateCurrencyBtnInfo();
		if (OnEquipItemSlot.IsBound())
		{
			OnEquipItemSlot.Broadcast();
		}
	}
}

void UGsUIItemMultiEnchantGroup::OnClickExpectNum(const uint8 InIndex, const bool InState)
{
	if (_isAutoEnchant)
		return;

	_selectedExpectEnchantCount = InIndex;
	UpdateExpectEnchantCount();
	UpdateNeedIngredientCount();
	UpdateCurrencyBtnInfo();
	UpdateGuideText();
	_expectCountListPanelSlotHelper->RefreshAll(EXPECT_ENCHANT_NUM);
	_equipItemListPanelSlotHelper->RefreshAll(EQUIPITEM_SLOT_NUM);
}

void UGsUIItemMultiEnchantGroup::OnClickEquipItemSlot(const int32 InIndex, const ItemDBId InItemDbid)
{
	if (_isAutoEnchant)
		return;

	if (0 < InItemDbid)
	{
		if (INDEX_NONE != _targetItemDbidList.Find(InItemDbid))
		{
			_targetItemDbidList.Remove(InItemDbid);
		}
		InvalidateAll();

		if (OnEquipItemSlot.IsBound())
		{
			OnEquipItemSlot.Broadcast();
		}
	}
}

void UGsUIItemMultiEnchantGroup::CloseMultiEnchant()
{
	if (OnEquipItemSlot.IsBound())
	{
		OnEquipItemSlot.Clear();
	}
	GetWorld()->GetTimerManager().ClearTimer(_equipSlotWaitEffectTimer);

	_packetEquipItemDbidList.Empty();
	_packetIngredientItemDbidList.Empty();
	_packetTargetLevel = 0;
	_isSendingPacket = false;
	_isAutoEnchant = false;
	_reserveRemoveItemDbid = 0;
	_selectedExpectEnchantCount = -1;
	_targetItemDbidList.Empty();
	_ingredientDataMap.Empty();
	_expectEnchantStateList.Empty();
	_equipSlotEffectInfoMap.Empty();

	_inputCheckbox->SetIsSelected(false);
	_ingredientItemIconSelector->RemoveAllChildren();

	_equipItemListPanelSlotHelper->RefreshAll(0);
	_expectCountListPanelSlotHelper->RefreshAll(0);

	_btnSwitcher->SetActiveWidgetIndex(0);

	_tooltipDescWidget->Close();
}

TArray<ItemDBId> UGsUIItemMultiEnchantGroup::GetIngredientItemDbidList()
{
	TArray<ItemDBId> dbidList;
	for (const TPair<ItemDBId, TPair<ItemId, ItemAmount>> iter : _ingredientDataMap)
	{
		dbidList.Emplace(iter.Key);
	}
	return dbidList;
}

const ItemAmount UGsUIItemMultiEnchantGroup::GetSelectedIngredientTotalAmount()
{
	ItemAmount result = 0;
	for (const TPair<ItemDBId, TPair<ItemId, ItemAmount>> iter : _ingredientDataMap)
	{
		result += iter.Value.Value;
	}
	return result;
}

const ItemId UGsUIItemMultiEnchantGroup::GetSelectedIngredientTID()
{
	for (const TPair<ItemDBId, TPair<ItemId, ItemAmount>> iter : _ingredientDataMap)
	{
		return iter.Value.Key;
	}
	return 0;
}

void UGsUIItemMultiEnchantGroup::EnchantEquipItemListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIMultiEnchantEquipItemEntry* entry = Cast<UGsUIMultiEnchantEquipItemEntry>(InEntry);
	if (nullptr == entry)
		return;

	ItemDBId dbid = _targetItemDbidList.IsValidIndex(InIndex) ? _targetItemDbidList[InIndex] : 0;
	entry->SetData(InIndex, dbid, _selectedExpectEnchantCount, IsRegisterCursed());
	if (false == entry->OnClickEquipItem.IsBoundToObject(this))
	{
		entry->OnClickEquipItem.AddUObject(this, &UGsUIItemMultiEnchantGroup::OnClickEquipItemSlot);
	}
	if (false == entry->OnFinishedSlotEffect.IsBoundToObject(this))
	{
		entry->OnFinishedSlotEffect.AddUObject(this, &UGsUIItemMultiEnchantGroup::OnFinishedEquipSlotEffect);
	}
}

void UGsUIItemMultiEnchantGroup::EnchantCountListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIMultiEnchantCountEntry* entry = Cast<UGsUIMultiEnchantCountEntry>(InEntry);
	if (nullptr == entry)
		return;

	entry->SetData(IsRegisterCursed() ? InIndex : InIndex + 1, _expectEnchantStateList.IsValidIndex(InIndex) ? _expectEnchantStateList[InIndex] : false, 
		IsRegisterCursed() ? InIndex == _selectedExpectEnchantCount : (InIndex + 1) == _selectedExpectEnchantCount);
	if (false == entry->OnClickNum.IsBoundToObject(this))
	{
		entry->OnClickNum.AddUObject(this, &UGsUIItemMultiEnchantGroup::OnClickExpectNum);
	}
}

void UGsUIItemMultiEnchantGroup::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if (0 < _ingredientDataMap.Num())
	{
		itemIcon->ResetCoolTime();
		itemIcon->SetNotOwnItem(GetSelectedIngredientTID(), GetSelectedIngredientTotalAmount());
		itemIcon->SetItemNameVisibility(false);
		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameToolTipFlag(false);
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
	}
	else
	{
		itemIcon->SetEmptyIcon();
	}
}

const FGsItem* UGsUIItemMultiEnchantGroup::GetRegisterIngredientItemData()
{
	UGsUIIconBase* iconBase = _ingredientIconSlotHelper->GetIcon(0);
	if (nullptr != iconBase && Cast<UGsUIIconItemInventory>(iconBase))
	{
		UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(iconBase);
		FGsItem* itemiconData = itemIcon->GetItemIconData();
		return itemiconData;
	}

	return nullptr;
}

const bool UGsUIItemMultiEnchantGroup::IsRegisterCursed()
{
	UGsUIIconBase* iconBase = _ingredientIconSlotHelper->GetIcon(0);
	if (nullptr != iconBase && Cast<UGsUIIconItemInventory>(iconBase))
	{
		UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(iconBase);
		FGsItem* itemiconData = itemIcon->GetItemIconData();
		if (itemiconData)
		{
			return itemiconData->IsCursedEnchantScrollItem();
		}
	}
	return false;
}

bool UGsUIItemMultiEnchantGroup::IsEnchantValidState()
{
	return (nullptr != GetRegisterIngredientItemData() && 0 < _targetItemDbidList.Num());
}

bool UGsUIItemMultiEnchantGroup::IsValidQuickEnchantNum()
{
	if (0 > _selectedExpectEnchantCount)
		return false;

	const FGsItem* itemData = GetRegisterIngredientItemData();
	if (itemData)
	{
		int32 index = _selectedExpectEnchantCount;
		index = itemData->IsCursedEnchantScrollItem() ? index : index - 1;
		if (_expectEnchantStateList.IsValidIndex(index))
		{
			return _expectEnchantStateList[index];
		}
	}
	return false;
}

void UGsUIItemMultiEnchantGroup::AddSameEquipItems()
{
	if (_isAutoEnchant)
		return;

	if (0 >= _targetItemDbidList.Num())
		return;

	UGsItemManager* itemMgr = GItem();
	TMap<ItemId, TArray<Level>> compareInfoMap;
	for (const ItemDBId iter : _targetItemDbidList)
	{
		TWeakPtr<FGsItem> findItemData = itemMgr->FindItem(iter, ItemType::EQUIP);
		if (findItemData.IsValid())
		{
			if (compareInfoMap.Contains(findItemData.Pin()->GetTID()))
			{
				TArray<Level>& levelList = compareInfoMap[findItemData.Pin()->GetTID()];
				if (INDEX_NONE == levelList.Find(findItemData.Pin()->GetLevel()))
				{
					levelList.Emplace(findItemData.Pin()->GetLevel());
				}
			}
			else
			{
				TArray<Level> list;
				list.Emplace(findItemData.Pin()->GetLevel());
				compareInfoMap.Emplace(findItemData.Pin()->GetTID(), list);
			}
		}
	}
	bool findData = false;
	for (TPair<ItemId, TArray<Level>> iter : compareInfoMap)
	{
		TArray<Level> levelList = iter.Value;
		TArray<TWeakPtr<FGsItem>> findItemList = itemMgr->FindByTID(iter.Key, ItemType::EQUIP);
		for (TWeakPtr<FGsItem> iterItem : findItemList)
		{
			if (INDEX_NONE != levelList.Find(iterItem.Pin()->GetLevel()))
			{
				if (false == _targetItemDbidList.Contains(iterItem.Pin()->GetDBID()))
				{
					if (EQUIPITEM_SLOT_NUM <= _targetItemDbidList.Num())
						break;

					_targetItemDbidList.Emplace(iterItem.Pin()->GetDBID());
					findData = true;
				}
			}
		}
	}
	if (findData)
	{
		itemMgr->SortItemListByDBID(0, _targetItemDbidList);
		InvalidateAll();
		if (OnEquipItemSlot.IsBound())
		{
			OnEquipItemSlot.Broadcast();
		}
	}
}

void UGsUIItemMultiEnchantGroup::AddAllIngredientItems(TWeakPtr<FGsItem> InInvenItem)
{
	if (false == InInvenItem.IsValid())
		return;

	FGsItemIngredient* ingredientItem = InInvenItem.Pin()->GetCastItem<FGsItemIngredient>();
	if (nullptr == ingredientItem)
		return;

	TArray<TWeakPtr<FGsItem>> findIngredientItems = GItem()->FindItem(ItemType::INGREDIENT, InInvenItem.Pin()->GetMainCategory(), InInvenItem.Pin()->GetSubCategory());
	for (TWeakPtr<FGsItem> iter : findIngredientItems)
	{
		if (true == iter.Pin()->GetLock())
			continue;

		FGsItemIngredient* ingredient = iter.Pin()->GetCastItem<FGsItemIngredient>();
		if (nullptr == ingredient)
			continue;

		if(false == ingredientItem->IsMatchedEnchantScrollItem(ingredient->FindFirstEffectId()))
			continue;

		if (false == _ingredientDataMap.Contains(iter.Pin()->GetDBID()))
		{
			_ingredientDataMap.Emplace(iter.Pin()->GetDBID(),
				TPair<ItemId, ItemAmount>(iter.Pin()->GetTID(), iter.Pin()->GetAmount()));
		}
	}
}

void UGsUIItemMultiEnchantGroup::AddAllIngredientItems()
{
	const FGsItem* registerItemData = GetRegisterIngredientItemData();
	if (nullptr == registerItemData)
		return;

	const FGsItemIngredient* ingredientItem = static_cast<const FGsItemIngredient*>(registerItemData);
	if (nullptr == ingredientItem)
		return;

	TArray<TWeakPtr<FGsItem>> findIngredientItems = GItem()->FindItem(ItemType::INGREDIENT, registerItemData->GetMainCategory(), registerItemData->GetSubCategory());
	for (TWeakPtr<FGsItem> iter : findIngredientItems)
	{
		if (true == iter.Pin()->GetLock())
			continue;

		FGsItemIngredient* ingredient = iter.Pin()->GetCastItem<FGsItemIngredient>();
		if (nullptr == ingredient)
			continue;

		if (false == ingredientItem->IsMatchedEnchantScrollItem(ingredient->FindFirstEffectId()))
			continue;

		if (false == _ingredientDataMap.Contains(iter.Pin()->GetDBID()))
		{
			_ingredientDataMap.Emplace(iter.Pin()->GetDBID(),
				TPair<ItemId, ItemAmount>(iter.Pin()->GetTID(), iter.Pin()->GetAmount()));
		}
	}
}

void UGsUIItemMultiEnchantGroup::InvalidateCurrencyInfo()
{
	_btnCurrency->UpdateAmountTextColorLocalPlayer();
}

void UGsUIItemMultiEnchantGroup::OnclickDetailInfo()
{
	_tooltipDescWidget->ToggleOpenClose();
}