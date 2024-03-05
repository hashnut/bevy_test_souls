// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/ItemEnchant/GsUIItemEnchantGroup.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Message/MessageParam/GsEnchantMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UILib/Manager/GsTooltipManager.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemIngredient.h"
#include "Item/GsItemManager.h"
#include "Currency/GsCurrencyHelper.h"
#include "Net/GsNetSendServiceItem.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Popup/ItemEnchant/GsUIQuickEnchantGroup.h"
#include "UI/UIContent/Common/Stat/GsUIChangeStatGroup.h"
#include "UI/UIContent/Common/Item/GsUIItemChangeSkill.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "GsUIChangeItemPropertyEntry.h"
#include "GsUIEnchantRateListEntry.h"
#include "GsUIEnchantPropTitleEntry.h"



void UGsUIItemEnchantGroup::BeginDestroy()
{
	if (nullptr != _targetIconSlotHelper)
	{
		_targetIconSlotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIItemEnchantGroup::OnRefreshTargetIcon);
		_targetIconSlotHelper->Clear();
		_targetIconSlotHelper = nullptr;
	}
	if (nullptr != _materialIconSlotHelper)
	{
		_materialIconSlotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIItemEnchantGroup::OnRefreshMaterialIcon);
		_materialIconSlotHelper->Clear();
		_materialIconSlotHelper = nullptr;
	}

	if (nullptr != _statListPanelSlotHelper)
	{
		_statListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIItemEnchantGroup::DefaultStatListRefresh);
		_statListPanelSlotHelper = nullptr;
	}
	if (nullptr != _enchantPropStatListPanelSlotHelper)
	{
		_enchantPropStatListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIItemEnchantGroup::EnchantPropListRefresh);
		_enchantPropStatListPanelSlotHelper = nullptr;
	}
	if (nullptr != _enchantSkillListPanelSlotHelper)
	{
		_enchantSkillListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIItemEnchantGroup::EnchantSkillListRefresh);
		_enchantSkillListPanelSlotHelper = nullptr;
	}
	if (nullptr != _expectRateListPanelSlotHelper)
	{
		_expectRateListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIItemEnchantGroup::ExpectRateListRefresh);
		_expectRateListPanelSlotHelper = nullptr;
	}
	if (nullptr != _enchantPropTitlePanelSlotHelper)
	{
		_enchantPropTitlePanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIItemEnchantGroup::EnchantPropTitleRefresh);
		_enchantPropTitlePanelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIItemEnchantGroup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _targetIconSlotHelper)
	{
		_targetIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_targetIconSlotHelper->Initialize(_targetItemIconSelector);
		_targetIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIItemEnchantGroup::OnRefreshTargetIcon);
	}
	if (nullptr == _materialIconSlotHelper)
	{
		_materialIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_materialIconSlotHelper->Initialize(_materialItemIconSelector);
		_materialIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIItemEnchantGroup::OnRefreshMaterialIcon);
	}
	if (nullptr == _statListPanelSlotHelper)
	{
		_statListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_statListPanelSlotHelper->Initialize(_statListEntryWidgetClass, _defaultStatListRoot);
		_statListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIItemEnchantGroup::DefaultStatListRefresh);
	}
	if (nullptr == _enchantPropStatListPanelSlotHelper)
	{
		_enchantPropStatListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_enchantPropStatListPanelSlotHelper->Initialize(_enchantPropStatListEntryWidget, _enchantPropStatListRoot);
		_enchantPropStatListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIItemEnchantGroup::EnchantPropListRefresh);
	}
	if (nullptr == _enchantSkillListPanelSlotHelper)
	{
		_enchantSkillListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_enchantSkillListPanelSlotHelper->Initialize(_enchantSkillListEntryWidget, _enchantSkillListRoot);
		_enchantSkillListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIItemEnchantGroup::EnchantSkillListRefresh);
	}
	if (nullptr == _expectRateListPanelSlotHelper)
	{
		_expectRateListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_expectRateListPanelSlotHelper->Initialize(_expectRateListEntryWidget, _expectRateListRoot);
		_expectRateListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIItemEnchantGroup::ExpectRateListRefresh);
	}
	if (nullptr == _enchantPropTitlePanelSlotHelper)
	{
		_enchantPropTitlePanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_enchantPropTitlePanelSlotHelper->Initialize(_enchantPropTitleEntryWidget, _enchantPropStatListRoot);
		_enchantPropTitlePanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIItemEnchantGroup::EnchantPropTitleRefresh);
	}

	_btnTargetItem->OnClicked.AddDynamic(this, &UGsUIItemEnchantGroup::OnclickTargetItemSlot);
	_btnMaterialItem->OnClicked.AddDynamic(this, &UGsUIItemEnchantGroup::OnClickMaterialItemSlot);
	_btnStopEnchant->OnClicked.AddDynamic(this, &UGsUIItemEnchantGroup::OnclickStopEnchant);
	_btnRateInfo->OnClicked.AddDynamic(this, &UGsUIItemEnchantGroup::OnclickRateInfo);
	_btnToolTipClose->OnClicked.AddDynamic(this, &UGsUIItemEnchantGroup::OnclickCloseRateInfo);
	_btnCurrency->OnClickCurrencyButton.BindUObject(this, &UGsUIItemEnchantGroup::OnclickEnchant);
	
	_quickEnchantBtn1->OnClicked.AddDynamic(this, &UGsUIItemEnchantGroup::OnclickQuickEnchant1);
	_quickEnchantBtn2->OnClicked.AddDynamic(this, &UGsUIItemEnchantGroup::OnclickQuickEnchant2);
	_quickEnchantBtn3->OnClicked.AddDynamic(this, &UGsUIItemEnchantGroup::OnclickQuickEnchant3);

}

void UGsUIItemEnchantGroup::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == _mainSlotsafeEnchantEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_mainSlotsafeEnchantEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUIItemEnchantGroup::BindSafeEnchantEffectAnimFinishedEvent);
	}

	_quickEnchantBtnSwitcher1->SetActiveWidgetIndex(1);
	_quickEnchantBtnSwitcher2->SetActiveWidgetIndex(1);
	_quickEnchantBtnSwitcher3->SetActiveWidgetIndex(1);

	//_statListPanelSlotHelper->AllClearChildren();
	//_enchantPropStatListPanelSlotHelper->AllClearChildren();

	_displayDefaultStatDataList.Empty();
	_displayPropertyStatDataList.Empty();
	//_displaySkillDataMap->Empty();

	_expectEnchantLevel = 0;
	_isAutoEnchant = false;

	FText findText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Button_Enchant"), findText);
	_btnCurrency->SetButtonName(findText);
	_btnCurrency->SetOnClickLockTime(0.5f);

	OnclickCloseRateInfo();
}

void UGsUIItemEnchantGroup::NativeDestruct()
{
	if (true == OnClickMainIconSlot.IsBound())
	{
		OnClickMainIconSlot.Clear();
	}

	_displayDefaultStatDataList.Empty();
	_displayPropertyStatDataList.Empty();
	_expectRateInfoList.Empty();

	if (_displaySkillDataMap.IsSet())
	{
		_displaySkillDataMap->Empty();
		_displaySkillDataMap.Reset();
	}

	_quickEnchantValueMap.Empty();
	_propTitleInfoList.Empty();

	_mainSlotsafeEnchantEffect->HideVFX();

	_targetItemIconSelector->RemoveAllChildren();
	_materialItemIconSelector->RemoveAllChildren();

	_statListPanelSlotHelper->AllClearChildren();
	_enchantPropStatListPanelSlotHelper->AllClearChildren();
	_enchantPropTitlePanelSlotHelper->AllClearChildren();

	_targetItemDbid = 0;
	_ingredientItemDbid = 0;

	OnclickCloseRateInfo();
	
	Super::NativeDestruct();
}

void UGsUIItemEnchantGroup::CloseEnchant()
{
	_isAutoEnchant = false;
	_expectEnchantLevel = 0;
	_targetItemDbid = 0;
	_ingredientItemDbid = 0;

	_mainSlotsafeEnchantEffect->HideVFX();
	OnclickCloseRateInfo();
}

void UGsUIItemEnchantGroup::SetEnchantInfo(const ItemDBId InTargetItemDbid, const EGsItemEnchantActionType InEnchantActionType)
{
	_ingredientItemDbid = GetFirstIngredient(InTargetItemDbid);
	SetEnchantItemDbid(InTargetItemDbid);
}

void UGsUIItemEnchantGroup::InvalidateByQuickEnchant()
{
	UGsItemManager* itemMgr = GItem();
	TWeakPtr<FGsItem> findTargetItem = itemMgr->FindItem(_targetItemDbid, ItemType::EQUIP);
	TWeakPtr<FGsItem> findItemIngredient = itemMgr->FindItem(_ingredientItemDbid, ItemType::INGREDIENT);

	SetEnchantGuideText(findTargetItem, findItemIngredient);
	SetEnchantBtnInfo();
}

void UGsUIItemEnchantGroup::SetEnchantItemDbid(const ItemDBId InTargetItemDbid)
{
	TWeakPtr<FGsItem> findItem = GItem()->FindItem(InTargetItemDbid);
	if (findItem.IsValid())
	{
		_targetItemDbid = true == findItem.Pin()->IsEquipment() ? InTargetItemDbid : _targetItemDbid;
		_ingredientItemDbid = false == findItem.Pin()->IsEquipment() ? InTargetItemDbid : _ingredientItemDbid;
	}

	InvalidateAll();
}

void UGsUIItemEnchantGroup::SetInputItemDbid(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid)
{
	_targetItemDbid = InTargetItemDbid;
	_ingredientItemDbid = InIngredientItemDbid;

	InvalidateAll();
}

void UGsUIItemEnchantGroup::InvalidateAll()
{
	UGsItemManager* itemMgr = GItem();
	FGsItemEnchantManager* enchantMgr = GSItemEnchant();

	// itemiconselector
	_targetIconSlotHelper->RefreshAll(1);
	_materialIconSlotHelper->RefreshAll(1);

	TWeakPtr<FGsItem> findTargetItem = itemMgr->FindItem(_targetItemDbid, ItemType::EQUIP);
	TWeakPtr<FGsItem> findItemIngredient = itemMgr->FindItem(_ingredientItemDbid, ItemType::INGREDIENT);

	// guide switcher
	DisableEnchantReasoneType type = CheckEnchantStateType(findTargetItem, findItemIngredient);
	bool state = (findTargetItem.IsValid() && findItemIngredient.IsValid() && DisableEnchantReasoneType::NoneType == type);
	_contentsSwitcher->SetActiveWidgetIndex(state ? 0 : 1); // 0 display , 1 guidetext

	SetEquipItemInfo(findTargetItem);
	_enchantGuideText->SetVisibility(state ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_btnCurrency->SetIsEnabled(state);
	if (state)
	{
		_enchantDisplayStatSwitcher->SetActiveWidgetIndex(findTargetItem.Pin()->IsCollectableType() ? 1 : 0);

		SetExpectInfo(findTargetItem, findItemIngredient, enchantMgr);

		// quickenchant set
		SetQuickEnchantInfo(findTargetItem, findItemIngredient);
		
		// default stat list
		// enchant property stat list	
		SetDefaultStatInfo(findTargetItem, findItemIngredient, enchantMgr);
		SetEnchantPropertyStatInfo(findTargetItem, findItemIngredient, enchantMgr);
		SetEnchantSkillInfo(findTargetItem, findItemIngredient, enchantMgr);

		SetEnchantGuideText(findTargetItem, findItemIngredient);

		SetEnchantBtnInfo();
		InvalidateBtnState(_isAutoEnchant ? EnchantBtnType::BtnStopEnchantType : EnchantBtnType::BtnEnchantType);

		InvalidateRateInfo(findTargetItem, findItemIngredient);
	}
	else
	{
		OnclickCloseRateInfo();
		FText findText;
		if (0 >= _targetItemDbid || 0 >= _ingredientItemDbid)
		{
			FText::FindText(TEXT("ItemUIText"), 0 >= _targetItemDbid ?
				TEXT("ItemUi_Enchant_Notice_PickItem") : TEXT("ItemUi_Enchant_Notice_PickScroll"), findText);
		}
		else
		{
			FTextKey key = TEXT("ItemUi_Notice_CannotUse");
			switch (type)
			{
			case UGsUIItemEnchantGroup::ItemLockType:
				key = TEXT("ItemUi_Use_Notice_Locked");
				break;
			case UGsUIItemEnchantGroup::ItemMaxLevelType:
				key = TEXT("ItemUi_Enchant_Notice_MaxLevel");
				break;
			case UGsUIItemEnchantGroup::CantUseCursedType:
				key = TEXT("ItemUi_Enchant_Notice_CursedToLevel0");
				break;
			}
			FText::FindText(TEXT("ItemUIText"), key, findText);
		}
		_guideText->SetText(findText);
	}
}

void UGsUIItemEnchantGroup::InvalidateBtnState(const EnchantBtnType InEnchantBtnType)
{
	_confirmBtnSwitcher->SetActiveWidgetIndex((uint8)InEnchantBtnType);
}

const UGsUIItemEnchantGroup::DisableEnchantReasoneType UGsUIItemEnchantGroup::CheckEnchantStateType(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData)
{
	if (InEquipData.IsValid() && InIngredientData.IsValid())
	{
		if (InEquipData.Pin()->GetLock() || InIngredientData.Pin()->GetLock())
		{
			return DisableEnchantReasoneType::ItemLockType;
		}
		if ((false == InIngredientData.Pin()->IsCursedEnchantScrollItem()) && (InEquipData.Pin()->GetLevel() >= InEquipData.Pin()->GetMaxLevel()))
		{
			return DisableEnchantReasoneType::ItemMaxLevelType;
		}
		if ((true == InIngredientData.Pin()->IsCursedEnchantScrollItem()) && (InEquipData.Pin()->GetMinLevel() >= InEquipData.Pin()->GetLevel()))
		{
			return DisableEnchantReasoneType::CantUseCursedType;
		}
	}
	return DisableEnchantReasoneType::NoneType;
}

void UGsUIItemEnchantGroup::SetEquipItemInfo(TWeakPtr<FGsItem> InEquipData)
{
	_itemInfoAreaPanel->SetVisibility(InEquipData.IsValid() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	if (InEquipData.IsValid())
	{
		if (FGsItemEquipment* equipData = InEquipData.Pin()->GetCastItem<FGsItemEquipment>())
		{
			FString resultString;
			FText markText;
			FText::FindText(TEXT("MarkFormatingText"), TEXT("Plus"), markText);
			resultString = FString::Format(TEXT("{0}{1} {2}"), { *markText.ToString() , FString::FormatAsNumber(equipData->GetLevel()), *equipData->GetName().ToString() });
			_itemNameText->SetText(FText::FromString(resultString));
			_itemNameText->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(equipData->GetGradeType()));
			_toolTipItemNameText->SetText(FText::FromString(resultString));
			_toolTipItemNameText->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(equipData->GetGradeType()));
			_enchantShieldCountText->SetText(FText::AsNumber(equipData->GetEnchantShieldCount()));
		}
	}
}

void UGsUIItemEnchantGroup::SetExpectInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData, FGsItemEnchantManager* InEnchantMgr)
{
	if (InEquipData.IsValid() && InIngredientData.IsValid() && InEnchantMgr)
	{
		FGsItemEquipment* equipData = InEquipData.Pin()->GetCastItem<FGsItemEquipment>();
		FGsItemIngredient* ingredientData = InIngredientData.Pin()->GetCastItem<FGsItemIngredient>();
		if (equipData && ingredientData)
		{
			// enchant level area
			// deltaValue 값이 curLevel + @ 했을때 맥스레벨 넘어가면 해당 수치만큼 차감 해서 deltaValue 에 넣어줘야함
			int32 daltaValue = InEnchantMgr->GetEnchantResultDeltaValue(ingredientData);
			ItemEnchantType enchantType = ingredientData->GetItemEnchantType();

			FText markText;
			FText swungDashText;
			FText::FindText(TEXT("MarkFormatingText"), TEXT("Plus"), markText);
			FText::FindText(TEXT("MarkFormatingText"), TEXT("SwungDash"), swungDashText);

			// 일반 , 저주 - +5 +6
			// 축 - +5 ~ +7
			int32 expectLevel = equipData->GetLevel() + daltaValue;
			FString curLvString = FString::Format(TEXT("{0}{1}"), { *markText.ToString(), equipData->GetLevel() });
			FString expectLvString;
			if (ItemEnchantType::BLESSED_ENCHANT == enchantType)
			{
				if (equipData->GetMaxLevel() < expectLevel)
				{
					int32 result = expectLevel - equipData->GetMaxLevel();
					expectLvString = FString::Format(TEXT("{0}{1}"), { *markText.ToString(), equipData->GetLevel() + result });
				}
				else
				{
					expectLvString = FString::Format(TEXT("{0}{1} {2} {3}"), { *markText.ToString(), equipData->GetLevel() + 1 , *swungDashText.ToString(), equipData->GetLevel() + daltaValue });
				}
			}
			else
			{
				expectLvString = FString::Format(TEXT("{0}{1}"), { *markText.ToString(), equipData->GetLevel() + daltaValue });
			}
			_currentLevelText->SetText(FText::FromString(curLvString));
			_expectLevelText->SetText(FText::FromString(expectLvString));
			_expectLevelText->SetColorAndOpacity(FGsUIColorHelper::GetColor(ItemEnchantType::CURSED_ENCHANT == enchantType ?
				EGsUIColorType::ENCHANT_CURSED_EXPECT_VALUE : EGsUIColorType::ENCHANT_EXPECT_VALUE));
		}
	}
}

void UGsUIItemEnchantGroup::SetEnchantGuideText(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData)
{
	if (InEquipData.IsValid() && InIngredientData.IsValid())
	{
		FGsItemEquipment* equipData = InEquipData.Pin()->GetCastItem<FGsItemEquipment>();
		FGsItemIngredient* ingredientData = InIngredientData.Pin()->GetCastItem<FGsItemIngredient>();
		if (equipData && ingredientData)
		{
			QuickEnchantSelectType type = GetCurrenctQuickSelectType();
			if (QuickEnchantSelectType::Risk == type)
			{
				FText findText;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_EquipDestroy"), findText);
				_enchantGuideText->SetText(findText);
				_toolTipGuideText->SetText(findText);
				_enchantGuideText->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::ENCHANT_WARNING_DESTROY));
				_toolTipGuideText->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::ENCHANT_WARNING_DESTROY));
			}
			else
			{
				FGsItemEnchantManager* enchantMgr = GSItemEnchant();
				float failRate = enchantMgr->GetEnchantItemRate(ItemEnchantResult::ENCHANT_FAILED, equipData->GetEnchantRateId() , equipData->GetLevel());
				float preservedRate = enchantMgr->GetEnchantItemRate(ItemEnchantResult::ENCHANT_PRESERVED, equipData->GetEnchantRateId(), equipData->GetLevel());
				float downRate = enchantMgr->GetEnchantItemRate(ItemEnchantResult::ENCHANT_FAILED_DOWN, equipData->GetEnchantRateId(), equipData->GetLevel());

				FText resultText;
				FText findDestroyText;
				FText findSafeText;
				FText preservedText;
				FText downText;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_EquipDestroy"), findDestroyText);
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_EquipSafe"), findSafeText);
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_EnchantLevelDown"), downText);
				FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_Common_AlertPreservation"), preservedText);

				// 1차 실패 확률이 있다면 파괴 텍스트 출력
				// 2차 유지 확률이 있다면 유지 텍스트 출력
				// 3차 실패도 없고 유지도 없다면 안전 강화
				// 저주 주문서류 들은 무조건 안전강화 
				bool isSafeEnchant = true;
				if(ingredientData->IsCursedEnchantScrollItem())
				{
					resultText = findSafeText;
				}
				else if (0.f < failRate)
				{
					isSafeEnchant = false;
					resultText = findDestroyText;
				}
				else if (0.f < downRate)
				{
					resultText = downText;
				}
				else if (0.f < preservedRate)
				{
					resultText = preservedText;
				}
				else
				{
					resultText = findSafeText;
				}
				_enchantGuideText->SetText(resultText);
				_toolTipGuideText->SetText(resultText);
				_enchantGuideText->SetColorAndOpacity(FGsUIColorHelper::GetColor(isSafeEnchant ? EGsUIColorType::ENCHANT_WARNING_NORMAL : EGsUIColorType::ENCHANT_WARNING_DESTROY));
				_toolTipGuideText->SetColorAndOpacity(FGsUIColorHelper::GetColor(isSafeEnchant ? EGsUIColorType::ENCHANT_WARNING_NORMAL : EGsUIColorType::ENCHANT_WARNING_DESTROY));
			}
		}
	}
}

void UGsUIItemEnchantGroup::SetEnchantBtnInfo()
{
	_btnCurrency->SetData(CurrencyType::GOLD, GetNeedEnchantCost());
	_btnCurrency->UpdateAmountTextColorLocalPlayer();
	//uint64 curValue = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
	//_btnCurrency->SetIsEnabled(curValue >= GetNeedEnchantCost());
}

void UGsUIItemEnchantGroup::InvalidateCurrencyInfo()
{
	_btnCurrency->UpdateAmountTextColorLocalPlayer();
}

Currency UGsUIItemEnchantGroup::GetNeedEnchantCost()
{
	Currency needcost = 0;
	TWeakPtr<FGsItem> findTargetItem = GItem()->FindItem(_targetItemDbid, ItemType::EQUIP);
	if (findTargetItem.IsValid() && findTargetItem.Pin()->GetCastItem<FGsItemEquipment>())
	{
		FGsItemEquipment* equipData = findTargetItem.Pin()->GetCastItem<FGsItemEquipment>();
		uint8 safeCount = equipData->GetSafeEnchantCount();
		int32 curLevel = equipData->GetLevel();
		ItemAmount needScrollItemCount = 0;
		switch (GetCurrenctQuickSelectType())
		{
		case UGsUIItemEnchantGroup::QuickEnchantSelectType::None:
			needScrollItemCount = 1;
			break;
		case UGsUIItemEnchantGroup::QuickEnchantSelectType::LowSafe:
			needScrollItemCount = (safeCount - 1) - curLevel;
			break;
		case UGsUIItemEnchantGroup::QuickEnchantSelectType::Safe:
			needScrollItemCount = safeCount - curLevel;
			break;
		case UGsUIItemEnchantGroup::QuickEnchantSelectType::Risk:
			needScrollItemCount = (safeCount + 1) - curLevel;
			break;
		}
		needcost = equipData->GetEnchantCostValue() * needScrollItemCount;
	}
	//_expectEnchantCount;
	return needcost;
}

ItemDBId UGsUIItemEnchantGroup::GetFirstIngredient(ItemDBId InTargetItemDBId)
{
	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
		return INVALID_ITEM_DB_ID;

	TArray<FGsItem*> ingredientItemList;
	TWeakPtr<FGsItem> findItem = itemMgr->FindItem(InTargetItemDBId);
	if (findItem.IsValid() && findItem.Pin()->IsEquipment())
	{
		TArray<TWeakPtr<FGsItem>> ingredientItems = itemMgr->FindItem(ItemType::INGREDIENT, ItemCategoryMain::SCROLL);
		for (TWeakPtr<FGsItem> iter : ingredientItems)
		{
			if (false == iter.IsValid())
				continue;

			FGsItemIngredient* ingredientItem = iter.Pin()->GetCastItem<FGsItemIngredient>();
			if (nullptr == ingredientItem)
				continue;

			if (ingredientItem->GetEffectType() != ItemEffectType::ITEM_ENCHANT)
				continue;

			if (ItemEnchantType::NORMAL_ENCHANT != ingredientItem->GetItemEnchantType())
				continue;

			if (false == ingredientItem->IsMatchedEnchantTargetItem(findItem.Pin()->GetSubCategory()))
				continue;

			ingredientItemList.Emplace(iter.Pin().Get());
		}
	}
	ingredientItemList.Sort([](FGsItem& itemA, FGsItem& itemB)
		{
			// 기간제 -> 귀속/비귀속
			if (itemB.IsPeriodItem() != itemA.IsPeriodItem())
			{
				return itemA.IsPeriodItem();
			}

			// 귀속 -> 비귀속
			if (itemB.IsTradable() != itemA.IsTradable())
			{
				return false == itemA.IsTradable();
			}

			// 아이템 보유 개수 (오름차순)
			if (itemB.GetAmount() != itemA.GetAmount())
			{
				return itemB.GetAmount() > itemA.GetAmount();
			}

			// 고유 비교 (ItemDBId 오름차순)
			return itemB.GetDBID() > itemA.GetDBID();
		}
	);

	if (0 >= ingredientItemList.Num())
		return INVALID_ITEM_DB_ID;
	
	return ingredientItemList[0]->GetDBID();
}

void UGsUIItemEnchantGroup::SetDefaultStatInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData, FGsItemEnchantManager* InEnchantMgr)
{
	if (InEquipData.Pin()->IsCollectableType())
	{
		_displayDefaultStatDataList.Empty();
		_statListPanelSlotHelper->RefreshAll(_displayDefaultStatDataList.Num());
		return;
	}

	if (InEnchantMgr)
	{
		InEnchantMgr->GetEnchantDefaultStatInfo(InEquipData, InIngredientData, _displayDefaultStatDataList);
	}
	_statListPanelSlotHelper->RefreshAll(_displayDefaultStatDataList.Num());
}

void UGsUIItemEnchantGroup::SetEnchantPropertyStatInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData, FGsItemEnchantManager* InEnchantMgr)
{
	if (InEquipData.Pin()->IsCollectableType())
	{
		_displayPropertyStatDataList.Empty();
		_enchantPropStatListPanelSlotHelper->RefreshAll(_displayPropertyStatDataList.Num());
		return;
	}
	
	// 특성이름이 먼저 출력되어야 함
	// root 가 같음	
	// 순서 변경 금지
	_propTitleInfoList.Empty();
	FGsItemEquipment* equipData = InEquipData.Pin()->GetCastItem<FGsItemEquipment>();
	if (equipData)
	{
		FString titleName = equipData->GetBonusStatTitleName();
		if (false == titleName.IsEmpty())
		{
			FLinearColor color = FGsUIColorHelper::GetColorItemGrade(equipData->GetGradeType());
			_propTitleInfoList.Emplace(TPair<FText, FLinearColor>(FText::FromString(titleName), color));
		}
		_enchantPropTitlePanelSlotHelper->RefreshAll(_propTitleInfoList.Num());
		if (InEnchantMgr)
		{
			bool state = InEnchantMgr->GetEnchantPropertyStatInfo(equipData, InIngredientData, _displayPropertyStatDataList);
			_enchantPropStatRoot->SetVisibility(state ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}

	_enchantPropStatListPanelSlotHelper->RefreshAll(_displayPropertyStatDataList.Num() + _propTitleInfoList.Num());
}

void UGsUIItemEnchantGroup::SetEnchantSkillInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData, FGsItemEnchantManager* InEnchantMgr)
{
	// 이전 스킬 데이터가 있을 경우, 초기화를 해 준다
	if (_displaySkillDataMap.IsSet())
	{
		_displaySkillDataMap->Empty();
	}

	// 수집품의 경우, 비워준다
	if (InEquipData.Pin()->IsCollectableType())
	{
		if (_displaySkillDataMap.IsSet())
		{
			_displaySkillDataMap->Empty();
		}
		_enchantSkillListPanelSlotHelper->RefreshAll(0);
		return;
	}

	FGsItemEquipment* equipData = InEquipData.Pin()->GetCastItem<FGsItemEquipment>();
	if (equipData && InEnchantMgr)
	{
		bool state = InEnchantMgr->GetEnchantSkillInfo(equipData, InIngredientData, _displaySkillDataMap);

		_enchantSkillRoot->SetVisibility(state ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	// 만약 강화 단계에 따라 가질 수 있는 스킬의 갯수가 바뀐다면, 이쪽을 수정해야 한다
	int32 currentLevel = equipData->GetLevel();
	int32 targetLevel = 0;
	
	FGsItemIngredient* ingredientData = InIngredientData.Pin()->GetCastItem<FGsItemIngredient>();
	ItemEnchantType enchantType = ingredientData->GetItemEnchantType();
	if (ItemEnchantType::CURSED_ENCHANT == enchantType)
	{
		targetLevel = FMath::Clamp(currentLevel - 1, 0, equipData->GetMaxLevel());
	}
	else
	{
		targetLevel = FMath::Clamp(currentLevel + 1, 0, equipData->GetMaxLevel());
	}

	if (!_displaySkillDataMap.IsSet() || 
		(!_displaySkillDataMap->Contains(currentLevel) && !_displaySkillDataMap->Contains(targetLevel)))
	{
		_enchantSkillListPanelSlotHelper->RefreshAll(0);
	}
	else
	{
		int32 maxEntryNum = 0;
		if (_displaySkillDataMap->Contains(currentLevel))
			maxEntryNum = (*_displaySkillDataMap)[currentLevel].Num();
		if (_displaySkillDataMap->Contains(targetLevel))
			maxEntryNum = FMath::Max(maxEntryNum, (*_displaySkillDataMap)[targetLevel].Num());

		_enchantSkillListPanelSlotHelper->RefreshAll(maxEntryNum);
	}
}


void UGsUIItemEnchantGroup::SetQuickEnchantInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData)
{
	_quickEnchantValueMap.Empty();

	_quickEnchantBtnSwitcher1->SetActiveWidgetIndex(1);
	_quickEnchantBtnSwitcher2->SetActiveWidgetIndex(1);
	_quickEnchantBtnSwitcher3->SetActiveWidgetIndex(1);

	if (InEquipData.IsValid() && InIngredientData.IsValid())
	{
		if (ItemCategoryMain::WEAPON != InEquipData.Pin()->GetMainCategory() && ItemCategoryMain::ARMOR != InEquipData.Pin()->GetMainCategory())
		{
			_quickEnchantBtn1->SetVisibility(ESlateVisibility::Hidden);
			_quickEnchantBtn2->SetVisibility(ESlateVisibility::Hidden);
			_quickEnchantBtn3->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		FGsItemEquipment* equipData = InEquipData.Pin()->GetCastItem<FGsItemEquipment>();
		FGsItemIngredient* ingredientData = InIngredientData.Pin()->GetCastItem<FGsItemIngredient>();
		if (equipData && ingredientData)
		{
			FString resultText;
			FText markText;
			FText noramlText;
			FText destroyText;
			FText::FindText(TEXT("MarkFormatingText"), TEXT("Plus"), markText);
			FText::FindText(TEXT("UICommonText"), TEXT("Safe"), noramlText);
			FText::FindText(TEXT("UICommonText"), TEXT("Destroy"), destroyText);

			bool activeState = true;
			int32 safeEnchantCount = equipData->GetSafeEnchantCount();
			activeState = (ItemEnchantType::NORMAL_ENCHANT == ingredientData->GetItemEnchantType()) && (safeEnchantCount - 1 > equipData->GetLevel());
			_quickEnchantBtn1->SetVisibility(activeState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
			if (activeState)
			{
				resultText = FString::Format(TEXT("{0}{1}"), { markText.ToString(), equipData->GetSafeEnchantCount() - 1 });
				_quickEnchantGroupCheckText1->SetEnchantGoalNumText(FText::FromString(resultText), false);
				_quickEnchantGroupCheckText1->SetEnchantWarnText(noramlText, false);
				_quickEnchantGroupNormalText1->SetEnchantGoalNumText(FText::FromString(resultText), false);
				_quickEnchantGroupNormalText1->SetEnchantWarnText(noramlText, false);
				if (!_quickEnchantValueMap.Contains(QuickEnchantSelectType::LowSafe))
				{
					_quickEnchantValueMap.Emplace(QuickEnchantSelectType::LowSafe, equipData->GetSafeEnchantCount() - 1);
				}
			}

			activeState = (ItemEnchantType::NORMAL_ENCHANT == ingredientData->GetItemEnchantType()) && (safeEnchantCount > equipData->GetLevel());
			_quickEnchantBtn2->SetVisibility(activeState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
			if (activeState)
			{
				resultText = FString::Format(TEXT("{0}{1}"), { markText.ToString(), equipData->GetSafeEnchantCount() });
				_quickEnchantGroupCheckText2->SetEnchantGoalNumText(FText::FromString(resultText), false);
				_quickEnchantGroupCheckText2->SetEnchantWarnText(noramlText, false);
				_quickEnchantGroupNormalText2->SetEnchantGoalNumText(FText::FromString(resultText), false);
				_quickEnchantGroupNormalText2->SetEnchantWarnText(noramlText, false);
				if (!_quickEnchantValueMap.Contains(QuickEnchantSelectType::Safe))
				{
					_quickEnchantValueMap.Emplace(QuickEnchantSelectType::Safe, equipData->GetSafeEnchantCount());
				}
			}

			activeState = (ItemEnchantType::NORMAL_ENCHANT == ingredientData->GetItemEnchantType()) && (safeEnchantCount + 1 > equipData->GetLevel());
			_quickEnchantBtn3->SetVisibility(activeState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
			if (activeState)
			{
				resultText = FString::Format(TEXT("{0}{1}"), { markText.ToString(), equipData->GetSafeEnchantCount() + 1 });
				_quickEnchantGroupCheckText3->SetEnchantGoalNumText(FText::FromString(resultText), true);
				_quickEnchantGroupCheckText3->SetEnchantWarnText(destroyText, true);
				_quickEnchantGroupNormalText3->SetEnchantGoalNumText(FText::FromString(resultText), true);
				_quickEnchantGroupNormalText3->SetEnchantWarnText(destroyText, true);
				if (!_quickEnchantValueMap.Contains(QuickEnchantSelectType::Risk))
				{
					_quickEnchantValueMap.Emplace(QuickEnchantSelectType::Risk, equipData->GetSafeEnchantCount() + 1);
				}
			}
		}
	}
}

bool UGsUIItemEnchantGroup::UpdateItemInfo(const ItemDBId InTargetItemDbid)
{
	if (_targetItemDbid == InTargetItemDbid || _ingredientItemDbid == InTargetItemDbid)
	{
		SetEnchantItemDbid(InTargetItemDbid);
		return true;
	}
	return false;
}

void UGsUIItemEnchantGroup::RemoveItemInfo(const ItemDBId InRemoveItemDbid)
{

}

void UGsUIItemEnchantGroup::OnclickTargetItemSlot()
{
	if (OnClickMainIconSlot.IsBound())
	{
		OnClickMainIconSlot.Broadcast(true, _targetItemDbid);
	}
}

void UGsUIItemEnchantGroup::OnClickMaterialItemSlot()
{
	if (OnClickMainIconSlot.IsBound())
	{
		OnClickMainIconSlot.Broadcast(false, _ingredientItemDbid);
	}
}

void UGsUIItemEnchantGroup::DefaultStatListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIChangeStatGroup* entry = Cast<UGsUIChangeStatGroup>(InEntry);
	if (nullptr == entry)
		return;

	if (_displayDefaultStatDataList.IsValidIndex(InIndex))
	{
		FGsItemEnchantManager::DisplayDefaultStatData& statData = _displayDefaultStatDataList[InIndex];
		FText statNameText;
		FGsStatHelper::GetDisPlayStatName(statData._type, statNameText);

		FString resultString;
		FText curStatValueText;
		FText minExpectStatValueText;
		FText maxExpectStatValueText;
		FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statData._type, statData._currentValue, curStatValueText);
		FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statData._type, statData._minExpectValue, minExpectStatValueText);
		if (0 < statData._maxExpectValue)
		{
			FText swungDashText;
			FText::FindText(TEXT("MarkFormatingText"), TEXT("SwungDash"), swungDashText);

			FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statData._type, statData._maxExpectValue, maxExpectStatValueText);
			resultString = FString::Format(TEXT("{0} {1} {2}"), { *minExpectStatValueText.ToString(), *swungDashText.ToString() , *maxExpectStatValueText.ToString()});
		}

		entry->SetStatGroupUIPanel(true);
		entry->SetStatName(statNameText);
		entry->SetCurrentStatValue(curStatValueText);
		entry->SetExpectStatValue(true == resultString.IsEmpty() ? minExpectStatValueText : FText::FromString(resultString));
		entry->SetExpectValueColor(FGsUIColorHelper::GetColor(true == statData._isCursedScroll ?
			EGsUIColorType::ENCHANT_CURSED_EXPECT_VALUE : EGsUIColorType::ENCHANT_EXPECT_VALUE));
	}
}

void UGsUIItemEnchantGroup::EnchantSkillListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIItemChangeSkill* entry = Cast<UGsUIItemChangeSkill>(InEntry);
	if (nullptr == entry)
	{
		GSLOG(Error, TEXT("nullptr == entry"));
		return;
	}
	
	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
		GSLOG(Error, TEXT("nullptr == itemMgr"));
		return;
	}

	TWeakPtr<FGsItem> findEqupiItem = itemMgr->FindItem(_targetItemDbid, ItemType::EQUIP);
	if (false == findEqupiItem.IsValid())
	{
		GSLOG(Error, TEXT("false == findEqupiItem.IsValid()"));
		return;
	}

	FGsItemEquipment* equipData = findEqupiItem.Pin()->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipData)
	{
		GSLOG(Error, TEXT("nullptr == equipData"));
		return;
	}

	TWeakPtr<FGsItem> findIngredientData = itemMgr->FindItem(_ingredientItemDbid, ItemType::INGREDIENT);
	if (false == findIngredientData.IsValid())
		return;

	FGsItemIngredient* ingredientData = findIngredientData.Pin()->GetCastItem<FGsItemIngredient>();
	if (nullptr == ingredientData)
		return;

	// 현재 강화 단계를 체크한다
	int32 curLevel = equipData->GetLevel();

	// 목표 강화 단계를 체크한다
	int32 targetLevel = 0;
	ItemEnchantType enchantType = ingredientData->GetItemEnchantType();
	if (ItemEnchantType::CURSED_ENCHANT == enchantType)
	{
		targetLevel = FMath::Clamp(curLevel - 1, 0, equipData->GetMaxLevel());
	}
	else
	{
		targetLevel = FMath::Clamp(curLevel + 1, 0, equipData->GetMaxLevel());
	}

	// 각 Entry 의 항목을 (현재 강화 단계 / 목표 강화 단계) 설명으로 채운다
	// 현재 스킬의 경우 curLevel 보다 작거나 같은 녀석을,
	// 다음 스킬의 경우 nextLevel 에 해당하는 녀석이 있는지만 체크한다.

	// 현재 레벨에서 적용할 수 있는 스킬의 최대 레벨
	Level curMaxLevel = 0;
	for (auto& pair : *_displaySkillDataMap)
	{
		if (pair.Key > curLevel)
			continue;

		curMaxLevel = curMaxLevel < pair.Key ? pair.Key : curMaxLevel;
	}

	TArray<FText>* curSkillTextList = _displaySkillDataMap->Find(curMaxLevel);
	TArray<FText>* nextSkillTextList = _displaySkillDataMap->Find(targetLevel);

	// "강화 스킬 없음" 표시
	FText noSkillText;
	FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_EnchantSkill_EmptySkillId"), noSkillText);

	// 현재 스킬에 대한 표시
	if (curSkillTextList && curSkillTextList->IsValidIndex(InIndex))
	{
		entry->SetCurSkillName((*curSkillTextList)[InIndex]);
	}
	else
	{
		entry->SetCurSkillName(noSkillText);
	}

	// 현재 스킬 설명의 텍스트 컬러는 무조건 기본 색상
	//if (equipData->GetSafeEnchantCount() < curLevel)
	//{
	//	entry->SetCurSkillNameColor(FGsUIColorHelper::GetColor(EGsUIColorType::ENCHANT_WARNING_DESTROY));
	//}

	// 다음 스킬에 대한 표시
	if (!nextSkillTextList || !nextSkillTextList->IsValidIndex(InIndex))
	{
		entry->ShowNextSkillArea(false);
		return;
	}

	// 만약 다음 스킬이 현재 스킬과 똑같은 값이면 표시 X
	// 갱신할 스킬이 없을 경우, 화살표 부분 부분부터 표시를 해 주지 않는다
	if (curSkillTextList && curSkillTextList->IsValidIndex(InIndex) &&
		(*curSkillTextList)[InIndex].CompareTo((*nextSkillTextList)[InIndex]) == 0)
	{
		entry->ShowNextSkillArea(false);
	}
	else
	{
		entry->ShowNextSkillArea(true);

		entry->SetNextSkillName((*nextSkillTextList)[InIndex]);

		if (ItemEnchantType::CURSED_ENCHANT == enchantType)
			entry->SetNextSkillNameColor(FGsUIColorHelper::GetColor(EGsUIColorType::ENCHANT_CURSED_EXPECT_VALUE));
		else
			entry->SetNextSkillNameColor(FGsUIColorHelper::GetColor(EGsUIColorType::ENCHANT_WARNING_DESTROY));
	}
}

void UGsUIItemEnchantGroup::EnchantPropTitleRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIEnchantPropTitleEntry* entry = Cast<UGsUIEnchantPropTitleEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (_propTitleInfoList.IsValidIndex(InIndex))
	{
		TPair<FText,FLinearColor> data = _propTitleInfoList[InIndex];
		entry->SetTitleText(data.Key);
		entry->SetTitleColor(data.Value);
	}
}

void UGsUIItemEnchantGroup::EnchantPropListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIChangeItemPropertyEntry* entry = Cast<UGsUIChangeItemPropertyEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (0 < InIndex)
	{
		InIndex = 0 < _propTitleInfoList.Num() ? InIndex - 1 : InIndex;
	}
	
	if (_displayPropertyStatDataList.IsValidIndex(InIndex))
	{
		FGsItemEnchantManager::DisplayPropertyStatData& info = _displayPropertyStatDataList[InIndex];
		// TEXT: 강화
		FText textEnchant;
		FText::FindText(TEXT("UICommonText"), TEXT("Enchant"), textEnchant);

		//+{0}강화 성공시 획득"
		FText textGain;
		FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_EnchantSingle_EnchantBonusGain"), textGain);
		//+{0}강화 성공시 상실"
		FText textlose;
		FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_EnchantSingle_EnchantBonusLoss"), textlose);

		// 예: 공격력 10 (강화 +8)
		TPair<FText, FText> pairText;
		if (false == FGsStatHelper::GetDesplayStatNameAndValue(info._type, info._currentValue, pairText, false))
		{
			return;
		}

		// 예: 10 (강화 +8)
		FString strValueFinal = FString::Format(TEXT("{0} ({1} +{2})"),
			{ *(pairText.Value.ToString()), textEnchant.ToString(), info._earnStatLevel });
		
		FText textMsg = FText::Format(info._isCursed ? textlose : textGain, info._isCursed ? info._earnStatLevel - 1 : info._earnStatLevel);
		entry->SetDataText(pairText.Key, FText::FromString(strValueFinal));
		
		bool bIsOpen = info._currentLevel >= info._earnStatLevel;
		entry->SetChangeTextColor(FGsUIColorHelper::GetColor(bIsOpen ?
			EGsUIColorType::DEFAULT_STAT_COLOR : EGsUIColorType::DISABLE_STAT_COLOR));
		entry->SetExpectStatText(textMsg);

		int32 resultLevel = info._currentLevel + info._deltaScrollValue;
		if (info._isCursed)
		{
			entry->SetExpectTextVisible((bIsOpen) && (resultLevel < info._earnStatLevel));
		}
		else
		{
			entry->SetExpectTextVisible((!bIsOpen) && (resultLevel >= info._earnStatLevel));
		}
		entry->SetChangeExpectTextColor(info._isCursed);
	}
}

void UGsUIItemEnchantGroup::OnclickQuickEnchant1()
{
	if (_mainSlotsafeEnchantEffect->IsPlayingAnimation() || _isAutoEnchant)
		return;

	int32 curIndex = _quickEnchantBtnSwitcher1->GetActiveWidgetIndex();
	_quickEnchantBtnSwitcher1->SetActiveWidgetIndex(0 == curIndex ? 1 : 0);

	_quickEnchantBtnSwitcher2->SetActiveWidgetIndex(1);
	_quickEnchantBtnSwitcher3->SetActiveWidgetIndex(1);

	InvalidateByQuickEnchant();
}

void UGsUIItemEnchantGroup::OnclickQuickEnchant2()
{
	if (_mainSlotsafeEnchantEffect->IsPlayingAnimation() || _isAutoEnchant)
		return;

	int32 curIndex = _quickEnchantBtnSwitcher2->GetActiveWidgetIndex();
	_quickEnchantBtnSwitcher2->SetActiveWidgetIndex(0 == curIndex ? 1 : 0);

	_quickEnchantBtnSwitcher1->SetActiveWidgetIndex(1);
	_quickEnchantBtnSwitcher3->SetActiveWidgetIndex(1);

	InvalidateByQuickEnchant();
}

void UGsUIItemEnchantGroup::OnclickQuickEnchant3()
{
	if (_mainSlotsafeEnchantEffect->IsPlayingAnimation() || _isAutoEnchant)
		return;

	int32 curIndex = _quickEnchantBtnSwitcher3->GetActiveWidgetIndex();
	_quickEnchantBtnSwitcher3->SetActiveWidgetIndex(0 == curIndex ? 1 : 0);

	_quickEnchantBtnSwitcher1->SetActiveWidgetIndex(1);
	_quickEnchantBtnSwitcher2->SetActiveWidgetIndex(1);

	InvalidateByQuickEnchant();
}

UGsUIItemEnchantGroup::QuickEnchantSelectType UGsUIItemEnchantGroup::GetCurrenctQuickSelectType()
{
	if (ESlateVisibility::Visible == _quickEnchantBtn1->GetVisibility())
	{
		int32 index = _quickEnchantBtnSwitcher1->GetActiveWidgetIndex();
		if (0 == index)
		{
			return QuickEnchantSelectType::LowSafe;
		}
	}
	if (ESlateVisibility::Visible == _quickEnchantBtn2->GetVisibility())
	{
		int32 index = _quickEnchantBtnSwitcher2->GetActiveWidgetIndex();
		if (0 == index)
		{
			return QuickEnchantSelectType::Safe;
		}
	}
	if (ESlateVisibility::Visible == _quickEnchantBtn3->GetVisibility())
	{
		int32 index = _quickEnchantBtnSwitcher3->GetActiveWidgetIndex();
		if (0 == index)
		{
			return QuickEnchantSelectType::Risk;
		}
	}

	return QuickEnchantSelectType::None;
}

void UGsUIItemEnchantGroup::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->ResetCoolTime();
	itemIcon->SetOwnedItem(_targetItemDbid);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameToolTipFlag(0 < _targetItemDbid);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
}

void UGsUIItemEnchantGroup::OnRefreshMaterialIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->ResetCoolTime();
	itemIcon->SetOwnedItem(_ingredientItemDbid);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameToolTipFlag(0 < _ingredientItemDbid);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
}

void UGsUIItemEnchantGroup::BindSafeEnchantEffectAnimFinishedEvent()
{
	if (0 >= _expectEnchantLevel)
		return;

	FGsItemEnchantManager* enchantMgr = GSItemEnchant();
	if (enchantMgr->IsRiskEnchant(_targetItemDbid, _ingredientItemDbid))
	{
		_isAutoEnchant = false;
		FGsShowEnchantEffect paramData(_targetItemDbid, _ingredientItemDbid, ItemEnchantResult::NONE, false);
		GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::SHOW_ENCHANT_EFFECT, &paramData);
		return;
	}

	FGsNetSendServiceItem::SendEnchantItem(_ingredientItemDbid, 1, _targetItemDbid);
	InvalidateBtnState(_isAutoEnchant ? EnchantBtnType::BtnStopEnchantType : EnchantBtnType::BtnEnchantType);
}

void UGsUIItemEnchantGroup::OnclickEnchant()
{
	if (_mainSlotsafeEnchantEffect->IsPlayingAnimation())
		return;

	// 강화 대기 or 결과 화면이 열려있다면 강화를 진행시키지 않는다.
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (uiManager->IsActiveWidget(TEXT("TrayEnchantItemWait")) ||
		uiManager->IsActiveWidget(TEXT("TrayEnchantResultSuccess")) ||
		uiManager->IsActiveWidget(TEXT("TrayEnchantResultBigSuccess")) ||
		uiManager->IsActiveWidget(TEXT("TrayEnchantResultFailed")) ||
		uiManager->IsActiveWidget(TEXT("TrayEnchantResultDestroy")) ||
		uiManager->IsActiveWidget(TEXT("TrayEnchantResultPreserved")) ||
		uiManager->IsActiveWidget(TEXT("TrayTickerRefineOptionResult")))
	{
		return;
	}

	if (UGsTooltipManager* tooltipManager = uiManager->GetTooltipManager())
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
	_isAutoEnchant = false;
	_expectEnchantLevel = 0;

	// 강화를 여러번 할 경우
	QuickEnchantSelectType enchantType = GetCurrenctQuickSelectType();

	if (_quickEnchantValueMap.Contains(enchantType))
	{
		_expectEnchantLevel = _quickEnchantValueMap[enchantType];
		_isAutoEnchant = true;
	}

	if (enchantMgr->IsValidSingleEnchant(_targetItemDbid, _ingredientItemDbid, _expectEnchantLevel))
	{
		// 장비 파괴 가능성이 있는 경우, 팝업을 띄워줌
		if (QuickEnchantSelectType::Risk == enchantType || enchantMgr->IsRiskEnchant(_targetItemDbid, _ingredientItemDbid))
		{
			// 인벤슬롯 혹은 무게 여유가 없을때 처리
			if (false == enchantMgr->IsOkInvenCondition())
			{
				FText findText;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_cannot_try"), findText);
				FGsUIHelper::TrayMessageTicker(findText);

				_isAutoEnchant = false;
				_expectEnchantLevel = 0;
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

			popup->SetData(textDesc, [enchantType = enchantType, this](bool bIsOk)
				{
					if (bIsOk)
					{
						OnConfirmEnchant();
					}
					else
					{
						if (QuickEnchantSelectType::Risk == enchantType)
						{
							ResetQuickEnchantButton();
						}
					}
				});
		}
		else
		{
			OnConfirmEnchant();
		}
	}
	else
	{
		_isAutoEnchant = false;
		_expectEnchantLevel = 0;
	}
}

void UGsUIItemEnchantGroup::OnConfirmEnchant()
{
	FGsItemEnchantManager* enchantMgr = GSItemEnchant();

	InvalidateBtnState(_isAutoEnchant ? EnchantBtnType::BtnStopEnchantType : EnchantBtnType::BtnEnchantType);
	_mainSlotsafeEnchantEffect->_playbackSpeed = 0.f >= _safeEnchantAnimSpeedValue ? 1.f : _safeEnchantAnimSpeedValue;
	_mainSlotsafeEnchantEffect->ShowVFX();
	enchantMgr->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::SingleEnchant_Safe, _safeEnchantAnimSpeedValue);
}

void UGsUIItemEnchantGroup::AutoEnchantProc()
{
	FGsItemEnchantManager* enchantMgr = GSItemEnchant();
	if (enchantMgr->IsValidSingleEnchant(_targetItemDbid, _ingredientItemDbid, _expectEnchantLevel))
	{
		_mainSlotsafeEnchantEffect->_playbackSpeed = 0.f >= _safeEnchantAnimSpeedValue ? 1.f : _safeEnchantAnimSpeedValue;
		_mainSlotsafeEnchantEffect->ShowVFX();
		enchantMgr->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::SingleEnchant_Safe, _safeEnchantAnimSpeedValue);
	}
	else
	{
		OnclickStopEnchant();
	}
}

void UGsUIItemEnchantGroup::OnclickStopEnchant()
{
	_isAutoEnchant = false;
	_expectEnchantLevel = -1;
	_mainSlotsafeEnchantEffect->HideVFX();
	InvalidateBtnState(EnchantBtnType::BtnEnchantType);
}

void UGsUIItemEnchantGroup::UpdateEnchantResultInfo(const ItemDBId IntargetItemDbid, const ItemDBId InIngredientItemDbid, const ItemEnchantResult InResultType)
{
	SetInputItemDbid(ItemEnchantResult::ENCHANT_FAILED_DESTROYED == InResultType ? 0 : IntargetItemDbid, InIngredientItemDbid);

	TWeakPtr<FGsItem> findTargetItem = GItem()->FindItem(IntargetItemDbid, ItemType::EQUIP);
	TWeakPtr<FGsItem> findIngredientItem = GItem()->FindItem(InIngredientItemDbid, ItemType::INGREDIENT);
	if (findTargetItem.IsValid() && findIngredientItem.IsValid())
	{
		ItemAmount autoEnchantCheck = _expectEnchantLevel - findTargetItem.Pin()->GetLevel();
		if (0 < autoEnchantCheck && false == findIngredientItem.Pin()->IsCursedEnchantScrollItem() 
			&& InResultType != ItemEnchantResult::ENCHANT_FAILED_SHIELDED && InResultType != ItemEnchantResult::ENCHANT_PRESERVED
			&& InResultType != ItemEnchantResult::ENCHANT_FAILED_DOWN)
		{
			SetSelectQuinckEnchant(findTargetItem, findIngredientItem);
			AutoEnchantProc();
			return;
		}
		else
		{
			if (_isAutoEnchant)
			{
				OnclickStopEnchant();
			}
		}
	}
	else
	{
		OnclickStopEnchant();
	}

	FGsShowEnchantEffect paramData(IntargetItemDbid, InIngredientItemDbid, InResultType, true);
	GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::SHOW_ENCHANT_EFFECT, &paramData);
}

void UGsUIItemEnchantGroup::SetSelectQuinckEnchant(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData)
{
	QuickEnchantSelectType type = QuickEnchantSelectType::None;
	for (const TPair<QuickEnchantSelectType, uint8> iter : _quickEnchantValueMap)
	{
		if (iter.Value == _expectEnchantLevel)
		{
			type = iter.Key;
			break;
		}
	}

	_quickEnchantBtnSwitcher1->SetActiveWidgetIndex(QuickEnchantSelectType::LowSafe == type ? 0 : 1);
	_quickEnchantBtnSwitcher2->SetActiveWidgetIndex(QuickEnchantSelectType::Safe == type ? 0 : 1);
	_quickEnchantBtnSwitcher3->SetActiveWidgetIndex(QuickEnchantSelectType::Risk == type ? 0 : 1);
	SetEnchantGuideText(InEquipData, InIngredientData);
}

void UGsUIItemEnchantGroup::ExpectRateListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIEnchantRateListEntry* entry = Cast<UGsUIEnchantRateListEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (_expectRateInfoList.IsValidIndex(InIndex))
	{
		FGsItemEnchantManager::DisplayRateInfo info = _expectRateInfoList[InIndex];
		entry->SetData(info);
		return;
	}
}

void UGsUIItemEnchantGroup::OnclickRateInfo()
{
	if (ESlateVisibility::Collapsed != _panelRateToolTip->GetVisibility())
	{
		_panelRateToolTip->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	if (0 >= _targetItemDbid || 0 >= _ingredientItemDbid)
	{
		_panelRateToolTip->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	UGsItemManager* itemMgr = GItem();
	TWeakPtr<FGsItem> findEquipItem = itemMgr->FindItem(_targetItemDbid, ItemType::EQUIP);
	TWeakPtr<FGsItem> findingredientItem = itemMgr->FindItem(_ingredientItemDbid, ItemType::INGREDIENT);
	if (false == findEquipItem.IsValid() || false == findingredientItem.IsValid())
		return;

	_panelRateToolTip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	GSItemEnchant()->GetEnchantExpectRateInfo(findEquipItem, findingredientItem, _expectRateInfoList);
	_expectRateListPanelSlotHelper->RefreshAll(_expectRateInfoList.Num());
}

void UGsUIItemEnchantGroup::InvalidateRateInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData)
{
	if (ESlateVisibility::Collapsed == _panelRateToolTip->GetVisibility())
	{
		return;
	}

	GSItemEnchant()->GetEnchantExpectRateInfo(InEquipData, InIngredientData, _expectRateInfoList);
	_expectRateListPanelSlotHelper->RefreshAll(_expectRateInfoList.Num());
}

void UGsUIItemEnchantGroup::OnclickCloseRateInfo()
{
	_panelRateToolTip->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIItemEnchantGroup::ResetQuickEnchantButton()
{
	_isAutoEnchant = false;
	_expectEnchantLevel = 0;

	_quickEnchantBtnSwitcher1->SetActiveWidgetIndex(1);
	_quickEnchantBtnSwitcher2->SetActiveWidgetIndex(1);
	_quickEnchantBtnSwitcher3->SetActiveWidgetIndex(1);

	InvalidateByQuickEnchant();
}