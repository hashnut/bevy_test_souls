// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/ItemEnchant/GsUIItemRefineGroup.h"
#include "Item/GsItem.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Message/MessageParam/GsEnchantMessageParam.h"
#include "UI/UILib/Manager/GsTooltipManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsItemEnchantManager.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemIngredient.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemStat.h"
#include "Net/GsNetSendServiceItem.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Popup/ItemEnchant/GsUIPopupItemRefineStatList.h"
#include "GsUIRefineOptionStatListEntry.h"
#include "Currency/GsCurrencyHelper.h"

//#include "UI/UIContent/Popup/ItemEnchant/GsUIEnchantRateListEntry.h"

#include "T1Project.h"

void UGsUIItemRefineGroup::BeginDestroy()
{
	if (nullptr != _targetIconSlotHelper)
	{
		_targetIconSlotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIItemRefineGroup::OnRefreshTargetIcon);
		_targetIconSlotHelper->Clear();
		_targetIconSlotHelper = nullptr;
	}
	if (nullptr != _materialIconSlotHelper)
	{
		_materialIconSlotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIItemRefineGroup::OnRefreshIngredientIcon);
		_materialIconSlotHelper->Clear();
		_materialIconSlotHelper = nullptr;
	}

	if (nullptr != _statListPanelSlotHelper)
	{
		_statListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIItemRefineGroup::RefineStatListRefresh);
		_statListPanelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIItemRefineGroup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _targetIconSlotHelper)
	{
		_targetIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_targetIconSlotHelper->Initialize(_targetItemIconSelector);
		_targetIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIItemRefineGroup::OnRefreshTargetIcon);
	}
	if (nullptr == _materialIconSlotHelper)
	{
		_materialIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_materialIconSlotHelper->Initialize(_materialItemIconSelector);
		_materialIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIItemRefineGroup::OnRefreshIngredientIcon);
	}
	if (nullptr == _statListPanelSlotHelper)
	{
		_statListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_statListPanelSlotHelper->Initialize(_statListEntryWidgetClass, _refineStatRootPanel);
		_statListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIItemRefineGroup::RefineStatListRefresh);
	}

	_btnTargetItem->OnClicked.AddDynamic(this, &UGsUIItemRefineGroup::OnclickTargetItemSlot);
	_btnMaterialItem->OnClicked.AddDynamic(this, &UGsUIItemRefineGroup::OnClickMaterialItemSlot);
	_btnCurrency->OnClickCurrencyButton.BindUObject(this, &UGsUIItemRefineGroup::OnclickRefine);
	_btnRefineOptionInfo->OnClicked.AddDynamic(this, &UGsUIItemRefineGroup::OnclickRefineOptionList);


}

void UGsUIItemRefineGroup::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == _mainSlotsafeEnchantEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_mainSlotsafeEnchantEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUIItemRefineGroup::BindSafeEnchantEffectAnimFinishedEvent);
	}

	_statListPanelSlotHelper->AllClearChildren();
	_btnCurrency->SetOnClickLockTime(0.5f);

	FText findText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Button_Refine"), findText);
	_btnCurrency->SetButtonName(findText);
	_btnCurrency->SetData(CurrencyType::GOLD, 0);

	_isRefine = false;
}

void UGsUIItemRefineGroup::NativeDestruct()
{
	if (true == OnClickRefineMainIconSlot.IsBound())
	{
		OnClickRefineMainIconSlot.Clear();
	}

	CloseExpectOptionPopup();

	_mainSlotsafeEnchantEffect->HideVFX();

	_targetItemIconSelector->RemoveAllChildren();
	_materialItemIconSelector->RemoveAllChildren();

	_targetItemDbid = 0;
	_ingredientItemDbid = 0;
	_refineStatDisplayDataMap.Empty();

	Super::NativeDestruct();
}

void UGsUIItemRefineGroup::SetRefineInfo(const ItemDBId InTargetItemDbid)
{
	TWeakPtr<FGsItem> findItem = GItem()->FindItem(InTargetItemDbid);
	if (findItem.IsValid())
	{
		_targetItemDbid = true == findItem.Pin()->IsEquipment() ? InTargetItemDbid : _targetItemDbid;
		_ingredientItemDbid = false == findItem.Pin()->IsEquipment() ? InTargetItemDbid : _ingredientItemDbid;
	}

	InvalidateAll();
}

void UGsUIItemRefineGroup::SetInputItemDbid(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid)
{
	_targetItemDbid = InTargetItemDbid;
	_ingredientItemDbid = InIngredientItemDbid;

	InvalidateAll();
}

void UGsUIItemRefineGroup::InvalidateAll()
{
	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
		return;

	TWeakPtr<FGsItem> findTargetItem = itemMgr->FindItem(_targetItemDbid, ItemType::EQUIP);
	TWeakPtr<FGsItem> findItemIngredient = itemMgr->FindItem(_ingredientItemDbid, ItemType::INGREDIENT);

	_targetItemDbid = findTargetItem.IsValid() ? _targetItemDbid : INVALID_ITEM_DB_ID;
	_ingredientItemDbid = findItemIngredient.IsValid() ? _ingredientItemDbid : INVALID_ITEM_DB_ID;

	// 아이템 아이콘 갱신
	_targetIconSlotHelper->RefreshAll(1);
	_materialIconSlotHelper->RefreshAll(1);

	const int32 SHOW_STAT_LIST = 0;
	const int32 HIDE_STAT_LIST = 1;

	FGsItemEquipment* equipData = nullptr;
	bool canRefine = false;
	FText guideText;
	int32 contentsSwitcherIndex = HIDE_STAT_LIST;
	do 
	{
		if (false == findTargetItem.IsValid())
		{
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_PickItem"), guideText);
			break;
		}

		equipData = findTargetItem.Pin()->GetCastItem<FGsItemEquipment>();
		if (nullptr == equipData)
		{
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_PickItem"), guideText);
			break;
		}

		TArray<int32> optionIdList;
		equipData->GetRefineOptionIdList(optionIdList);
		if (0 >= optionIdList.Num())
		{
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Refine_CanNotRefine"), guideText);
			break;
		}

		canRefine = true;

		if (false == UpdateRefineOptionList(findTargetItem))
		{
			FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_Refinement_NeedRefine"), guideText);
			break;
		}

		contentsSwitcherIndex = SHOW_STAT_LIST;

	} while (0);

	_contentsSwitcher->SetActiveWidgetIndex(contentsSwitcherIndex);
	if (SHOW_STAT_LIST == contentsSwitcherIndex)
	{
		_statListPanelSlotHelper->RefreshAll(_refineStatDisplayDataMap.Num());
	}
	else
	{
		_guideText->SetText(guideText);
	}
	
	if (nullptr == equipData)
	{
		_refineOptionTextAreaRootPanel->SetVisibility(ESlateVisibility::Collapsed);
		_itemInfoAreaPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_refineOptionTextAreaRootPanel->SetVisibility(canRefine ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		// 아이템 이름
		_itemInfoAreaPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FString resultString;
		FText markText;
		FText::FindText(TEXT("MarkFormatingText"), TEXT("Plus"), markText);
		resultString = FString::Format(TEXT("{0}{1} {2}"), { *markText.ToString() , FString::FormatAsNumber(equipData->GetLevel()), *equipData->GetName().ToString() });
		_itemNameText->SetText(FText::FromString(resultString));
		_itemNameText->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(equipData->GetGradeType()));
	}
	
	bool isEnableRefine = nullptr != equipData && findItemIngredient.IsValid() && canRefine;
	_btnCurrency->SetIsEnabled(isEnableRefine);
	if (isEnableRefine)
	{
		_btnCurrency->SetData(equipData->GetRefineOptionCostType(), equipData->GetRefineOptionCostValue());
		_btnCurrency->UpdateAmountTextColorLocalPlayer();
		_btnRefineOptionInfo->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_btnCurrency->SetData(CurrencyType::GOLD, 0);
		_btnRefineOptionInfo->SetVisibility(ESlateVisibility::Hidden);
	}

	// 재련 옵션 팝업 갱신
	FGsOpenExpectRefineOption paramData(_targetItemDbid, _ingredientItemDbid);
	GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::INVALIDATE_EXPECT_REFINE_OPTION, &paramData);
}

bool UGsUIItemRefineGroup::UpdateRefineOptionList(TWeakPtr<FGsItem> InTargetItem)
{
	_refineStatDisplayDataMap.Empty();
	FGsItemEquipment* equipItemData = InTargetItem.Pin()->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipItemData)
		return false;
	
	// 일반 스텟 먼저 출력하기 위해 선 작업
	// 현재 적용중인 제련 스텟옵션들
	TArray<FGsItemStat*> refineOptionStatList;
	if (false == equipItemData->GetAllRefineOptionStatList(refineOptionStatList))
		return false;

	// 스텟타입,<최소값,최대값>
	TMap<StatType, TPair<int32, int32>> statRangeMap;
	// 제련석과 빛나는 제련석의 옵션 수치는 동일하다.
	equipItemData->GetAllExpectRefineOptionStatList(ItemCategorySub::OPTION_STONE, statRangeMap);
	
	uint8 index = 0;
	for (FGsItemStat* iter : refineOptionStatList)
	{
		if(nullptr == iter)
			continue;

		StatType type = iter->GetStatType();
		int32 value = iter->GetStatOriValue();

		if (false == statRangeMap.Contains(type))
			continue;

		const FLinearColor& refineColor = FGsUIColorHelper::GetColorItemRefineOption(value, statRangeMap[type].Key, statRangeMap[type].Value);
		//TMap<uint8, TPair<TPair<StatType, int32>, const FLinearColor>>

		TPair<StatType, int32> statPairData = TPair<StatType, int32>(type, value);
		_refineStatDisplayDataMap.Emplace(index, TPair<TPair<StatType, int32>, const FLinearColor>(statPairData, refineColor));
		++index;
	}

	// 일반 스텟 출력 이후 바로 보너스 스텟 출력을 위한 처리
	// BonusStat
	// StatType , value
	TPair<StatType, int32> bonusStatData;
	if (equipItemData->GetEquipItemRefineBonusStatInfo(bonusStatData))
	{
		// statType, Value min / max 
		TMap<StatType, TPair<int32, int32>> bonusStatExpectDataMap;
		equipItemData->GetAllExpectBonusRefineOptionStatList(bonusStatExpectDataMap);

		StatType type = bonusStatData.Key;
		int32 statValue = bonusStatData.Value;

		int32 bonusStatMinValue = MAX_STAT_VALUE;
		int32 bonusStatMaxValue = MIN_STAT_VALUE;
		if (bonusStatExpectDataMap.Contains(type))
		{
			const TPair<int32, int32>& bonusStatExpectData = bonusStatExpectDataMap[type];
			bonusStatMinValue = bonusStatExpectData.Key; // min
			bonusStatMaxValue = bonusStatExpectData.Value; // max
		}
		const FLinearColor& refineColor = FGsUIColorHelper::GetColorItemRefineOption(statValue, bonusStatMinValue, bonusStatMaxValue);
		TPair<StatType, int32> statPairData = TPair<StatType, int32>(type, statValue);
		_refineStatDisplayDataMap.Emplace(index, TPair<TPair<StatType, int32>, const FLinearColor>(statPairData, refineColor));
	}
	return true;
}

void UGsUIItemRefineGroup::OnClickInventory(TWeakPtr<FGsItem> InItem)
{
	if (InItem.Pin()->GetDBID() == _targetItemDbid || InItem.Pin()->GetDBID() == _ingredientItemDbid)
	{
		return;
	}

	_targetItemDbid = true == InItem.Pin()->IsEquipment() ? InItem.Pin()->GetDBID() : _targetItemDbid;
	_ingredientItemDbid = false == InItem.Pin()->IsEquipment() ? InItem.Pin()->GetDBID() : _ingredientItemDbid;

	InvalidateAll();
}

void UGsUIItemRefineGroup::UpdateRefineOptionResultInfo()
{
	_isRefine = false;
	TWeakPtr<FGsItem> findItemIngredient = GItem()->FindItem(_ingredientItemDbid, ItemType::INGREDIENT);
	_ingredientItemDbid = findItemIngredient.IsValid() ? _ingredientItemDbid : 0;

	InvalidateAll();
	GSItemEnchant()->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::Refine_Success);
}

void UGsUIItemRefineGroup::RemoveItemInfo(const ItemDBId InRemoveItemDbid)
{

}

bool UGsUIItemRefineGroup::UpdateItemInfo(const ItemDBId InTargetItemDbid)
{
	if (_targetItemDbid == InTargetItemDbid || _ingredientItemDbid == InTargetItemDbid)
	{
		if (_isRefine)
			return true;

		SetRefineInfo(InTargetItemDbid);
		return true;
	}
	return false;
}

void UGsUIItemRefineGroup::CloseRefineOption()
{
	CloseExpectOptionPopup();

	/*
	if (true == OnClickRefineMainIconSlot.IsBound())
	{
		OnClickRefineMainIconSlot.Clear();
	}
	*/

	_mainSlotsafeEnchantEffect->HideVFX();

	_targetItemIconSelector->RemoveAllChildren();
	_materialItemIconSelector->RemoveAllChildren();

	_isRefine = false;
	_targetItemDbid = 0;
	_ingredientItemDbid = 0;
	_refineStatDisplayDataMap.Empty();
}

void UGsUIItemRefineGroup::InvalidateCurrencyInfo()
{
	if (false == _btnCurrency->GetIsEnabled())
	{
		_btnCurrency->SetData(CurrencyType::GOLD, 0);
		return;
	}

	_btnCurrency->UpdateAmountTextColorLocalPlayer();
}

void UGsUIItemRefineGroup::OnclickRefine()
{
	if (false == IsPlayingAnim())
	{
		// 재화 부족 팝업
		Currency cost = _btnCurrency->GetCurrencyAmount();
		if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::GOLD, cost, false))
		{
			Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
			FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, cost - myAmount);
			return;
		}

		FGsItemEnchantManager* enchantMgr = GSItemEnchant();
		if (enchantMgr->IsValidRefine(_targetItemDbid, _ingredientItemDbid))
		{
			if (UGsTooltipManager* tooltipManager = GUI()->GetTooltipManager())
			{
				// 다른 이름 툴팁 닫기
				tooltipManager->CloseNameTooltip();
			}

			_isRefine = true;
			_mainSlotsafeEnchantEffect->_playbackSpeed = 0.f >= _refineOptionAnimSpeedValue ? 1.f : _refineOptionAnimSpeedValue;
			_mainSlotsafeEnchantEffect->ShowVFX();
			GSItemEnchant()->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::Refine_Start, _refineOptionAnimSpeedValue);
		}
	}
}

void UGsUIItemRefineGroup::BindSafeEnchantEffectAnimFinishedEvent()
{
	FGsItemEnchantManager* enchantMgr = GSItemEnchant();
	if (nullptr == enchantMgr)
		return;

	if (enchantMgr->IsValidRefine(_targetItemDbid, _ingredientItemDbid))
	{
		FGsNetSendServiceItem::SendRefineOptionItem(_targetItemDbid, _ingredientItemDbid);
	}
}

void UGsUIItemRefineGroup::OnclickTargetItemSlot()
{
	if (OnClickRefineMainIconSlot.IsBound())
	{
		OnClickRefineMainIconSlot.Broadcast(true, _targetItemDbid);
	}
}

void UGsUIItemRefineGroup::OnClickMaterialItemSlot()
{
	if (OnClickRefineMainIconSlot.IsBound())
	{
		OnClickRefineMainIconSlot.Broadcast(false, _ingredientItemDbid);
	}
}

void UGsUIItemRefineGroup::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	//itemIcon->ResetCoolTime();
	itemIcon->SetOwnedItem(_targetItemDbid);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameToolTipFlag(0 < _targetItemDbid);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
}

void UGsUIItemRefineGroup::OnRefreshIngredientIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	//itemIcon->ResetCoolTime();
	itemIcon->SetOwnedItem(_ingredientItemDbid);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameToolTipFlag(0 < _ingredientItemDbid);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
}

void UGsUIItemRefineGroup::RefineStatListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIRefineOptionStatListEntry* entry = Cast<UGsUIRefineOptionStatListEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (_refineStatDisplayDataMap.Contains(InIndex))
	{
		TPair<TPair<StatType, int32>, const FLinearColor>& data = _refineStatDisplayDataMap[InIndex];
		TPair<StatType, int32>& statDataInfo = data.Key;
		StatType type = statDataInfo.Key;
		int32 value = statDataInfo.Value;
		const FLinearColor colorData = data.Value;

		FText statNameText;
		FGsStatHelper::GetDisPlayStatName(type, statNameText);

		FText curStatValueText;
		FGsStatHelper::GetDisPlayStatValueWithMarkFormat(type, value, curStatValueText);

		entry->SetData(InIndex, statNameText, curStatValueText, colorData);
	}
}

void UGsUIItemRefineGroup::OnclickRefineOptionList()
{
	FGsOpenExpectRefineOption paramData(_targetItemDbid, _ingredientItemDbid);
	GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::OPEN_EXPECT_REFINE_OPTION, &paramData);
}

void UGsUIItemRefineGroup::CloseExpectOptionPopup()
{
	GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::CLOSE_EXPECT_REFINE_OPTION, nullptr);
}