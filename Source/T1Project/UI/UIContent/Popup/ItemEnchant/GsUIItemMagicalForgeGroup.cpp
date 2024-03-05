// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIItemMagicalForgeGroup.h"

#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"

#include "Currency/GsCurrencyHelper.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Item/GsSchemaItemEffectMagicalForge.h"
#include "DataSchema/Item/GsSchemaItemEquipConfig.h"
#include "DataSchema/EffectText/GsSchemaEffectTextPassivity.h"

#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"

#include "UI/UIContent/Popup/ItemEnchant/GsUIItemMagicalForgeEntry.h"
#include "UI/UIContent/Popup/ItemEnchant/GsUIEnchantRateListEntry.h"
#include "UI/UIContent/Popup/ItemEnchant/GsUIPopupMagicalForgeChange.h"

#include "UI/UIContent/Define/EGsUIColorType.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"

#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UILib/Manager/GsTooltipManager.h"

#include "UTIL/GsTableUtil.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsItemEnchantManager.h"
#include "Management/ScopeGame/GsEffectTextManager.h"

#include "Message/MessageParam/GsEnchantMessageParam.h"

#include "Net/GsNetSendServiceItem.h"

#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemIngredient.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemHelper.h"


void UGsUIItemMagicalForgeGroup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _targetIconSlotHelper)
	{
		_targetIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_targetIconSlotHelper->Initialize(_targetItemIconSelector);
		_targetIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIItemMagicalForgeGroup::OnRefreshTargetIcon);
	}
	if (nullptr == _ingredientIconSlotHelper)
	{
		_ingredientIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_ingredientIconSlotHelper->Initialize(_ingredientItemIconSelector);
		_ingredientIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIItemMagicalForgeGroup::OnRefreshIngredientIcon);
	}

	if (_btnTargetItem)
	{
		_btnTargetItem->OnClicked.AddDynamic(this, &UGsUIItemMagicalForgeGroup::OnClickTargetItem);
	}
	if (_btnIngredientItem)
	{
		_btnIngredientItem->OnClicked.AddDynamic(this, &UGsUIItemMagicalForgeGroup::OnClickIngredientItem);
	}
	if (_btnProbability)
	{
		_btnProbability->OnClicked.AddDynamic(this, &UGsUIItemMagicalForgeGroup::OnClickProbability);
	}
	if (_btnProbabilityPanel)
	{
		_btnProbabilityPanel->OnClicked.AddDynamic(this, &UGsUIItemMagicalForgeGroup::OnClickProbabilityPanel);
	}
	if (_btnEnchant)
	{
		_btnEnchant->OnClickCurrencyButton.BindUObject(this, &UGsUIItemMagicalForgeGroup::OnClickEnchant);
	}
}

void UGsUIItemMagicalForgeGroup::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == _mainSlotsafeEnchantEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_mainSlotsafeEnchantEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUIItemMagicalForgeGroup::OnEnchantEffectAnimationFinished);
	}

	FText findText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_MagicalForge_Button_MagicalForge"), findText);
	_btnEnchant->SetButtonName(findText);
	_btnEnchant->SetOnClickLockTime(0.5f);
	SetCurrency(0);

	OnClickProbabilityPanel(); // 확률 정보 닫기

	FText findTextCondition;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_MagicalForge_MagicalCondition"), findTextCondition);
	_entryCondition->SetTitleText(findTextCondition);

	FText findTextEffect;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_MagicalForge_MagicalEffect"), findTextEffect);
	_entryEffect->SetTitleText(findTextEffect);
}

void UGsUIItemMagicalForgeGroup::NativeDestruct()
{
	if (OnClickMagicalForgeIconSlot.IsBound())
	{
		OnClickMagicalForgeIconSlot.Clear();
	}

	_mainSlotsafeEnchantEffect->HideVFX();

	_targetItemIconSelector->RemoveAllChildren();
	_ingredientItemIconSelector->RemoveAllChildren();

	_targetItemDBId = INVALID_ITEM_DB_ID;
	_ingredientItemDBId = INVALID_ITEM_DB_ID;

	OnClickProbabilityPanel(); // 확률 정보 닫기

	Super::NativeDestruct();
}

void UGsUIItemMagicalForgeGroup::ResetContents()
{
	_targetItemDBId = INVALID_ITEM_DB_ID;
	_ingredientItemDBId = INVALID_ITEM_DB_ID;

	_mainSlotsafeEnchantEffect->HideVFX();

	OnClickProbabilityPanel();
}

void UGsUIItemMagicalForgeGroup::SetItemDBId(const ItemDBId InTargetItemDBId, const ItemDBId InIngredientItemDBId)
{
	SetTargetItemDBId(InTargetItemDBId);
	SetIngredientItemDBId(InIngredientItemDBId);

	InvalidateAllInternal();
}

void UGsUIItemMagicalForgeGroup::SetTargetItemDBId(const ItemDBId InItemDBId)
{
	// 이미 선택중인 아이템일 경우 반응하지 않는다.
	if (InItemDBId == _targetItemDBId)
		return;

	ItemDBId selectedItemDBId = _targetItemDBId;

	// 등록할 수 있는 아이템인지 확인한다.
	if (CanRegisterEquipment(InItemDBId, true))
	{
		// 등록 가능하다면 무조건 등록된다.
		selectedItemDBId = InItemDBId;

		// 재료가 선택되어있는 경우
		if (INVALID_ITEM_DB_ID != _ingredientItemDBId)
		{
			// 선택한 장비에 부여할 수 있는 재료가 아닌 경우
			if (false == FGsItemHelper::IsMatchedMagicalForge(selectedItemDBId, _ingredientItemDBId))
			{
				// 재료를 해제시킨다.
				_ingredientItemDBId = INVALID_ITEM_DB_ID;
			}
		}
	}

	_targetItemDBId = selectedItemDBId;
}

void UGsUIItemMagicalForgeGroup::SetIngredientItemDBId(const ItemDBId InItemDBId)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	// 이미 선택중인 아이템일 경우 반응하지 않는다.
	if (InItemDBId == _ingredientItemDBId)
		return;

	ItemDBId selectedItemDBId = _ingredientItemDBId;

	// 등록할 수 있는 아이템인지 확인한다.
	if (CanRegisterIngredient(InItemDBId))
	{
		// 등록 가능하다면 무조건 등록된다.
		selectedItemDBId = InItemDBId;

		// 장비가 선택되어있는 경우
		if (INVALID_ITEM_DB_ID != _targetItemDBId)
		{
			// 선택한 재료를 부여할 수 있는 장비가 아닌 경우
			if (false == FGsItemHelper::IsMatchedMagicalForge(_targetItemDBId, selectedItemDBId))
			{
				// 장비를 해제시킨다.
				_targetItemDBId = INVALID_ITEM_DB_ID;
			}
		}
	}

	_ingredientItemDBId = InItemDBId;
}

bool UGsUIItemMagicalForgeGroup::IsPlayingAnim()
{
	return _mainSlotsafeEnchantEffect->IsPlayingAnimation();
}

void UGsUIItemMagicalForgeGroup::UpdateMagicalForgeResultInfo(ItemDBId InTargetItemDBId, ItemMagicalForgeResult InResultType)
{
	_isInProgress = false;
	
	InvalidateAllInternal();

	ItemEnchantResult resultType = (InResultType == ItemMagicalForgeResult::MAGICAL_FORGE_SUCCEEDED) ? ItemEnchantResult::ENCHANT_SUCCEEDED : ItemEnchantResult::ENCHANT_PRESERVED;
	FGsShowEnchantEffect paramData(InTargetItemDBId, _ingredientItemDBId, resultType, true);
	GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::SHOW_MAGICALFORGE_EFFECT, &paramData);
}

void UGsUIItemMagicalForgeGroup::InvalidateAll()
{
	InvalidateAllInternal();
}

void UGsUIItemMagicalForgeGroup::InvalidateCurrencyInfo()
{
	_btnEnchant->UpdateAmountTextColorLocalPlayer();
}

bool UGsUIItemMagicalForgeGroup::UpdateItemInfo(ItemDBId InItemDBId)
{
	if (_targetItemDBId == InItemDBId || _ingredientItemDBId == InItemDBId)
	{
		InvalidateAllInternal();
		return true;
	}

	return false;
}

void UGsUIItemMagicalForgeGroup::RemoveItemInfo(ItemDBId InItemDBId)
{
	if (_targetItemDBId == InItemDBId || _ingredientItemDBId == InItemDBId)
	{
		InvalidateAllInternal();
	}
}

void UGsUIItemMagicalForgeGroup::InvalidateAllInternal()
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	FGsItemEnchantManager* enchantManager = GSItemEnchant();
	if (nullptr == enchantManager)
		return;

	const FGsItemEquipment* equipData = nullptr;
	TWeakPtr<FGsItem> findTargetItem = itemManager->FindItem(_targetItemDBId, ItemType::EQUIP);
	if (findTargetItem.IsValid())
	{
		equipData = findTargetItem.Pin()->GetCastItem<FGsItemEquipment>();
	}
	else
	{
		// 아이템이 유효하지 않으므로 초기값으로 변경한다.
		_targetItemDBId = INVALID_ITEM_DB_ID;
	}

	const FGsItemIngredient* ingredientData = nullptr;
	TWeakPtr<FGsItem> findIngredientItem = itemManager->FindItem(_ingredientItemDBId, ItemType::INGREDIENT);
	if (findIngredientItem.IsValid())
	{
		ingredientData = findIngredientItem.Pin()->GetCastItem<FGsItemIngredient>();
	}
	else
	{
		// 아이템이 유효하지 않으므로 초기값으로 변경한다.
		_ingredientItemDBId = INVALID_ITEM_DB_ID;
	}

	// 아이템 아이콘 갱신
	_targetIconSlotHelper->RefreshAll(1);
	_ingredientIconSlotHelper->RefreshAll(1);

	bool isTargetItemSelected = (_targetItemDBId > 0) && (nullptr != equipData);
	bool isIngredientItemSelected = (_ingredientItemDBId > 0) && (nullptr != ingredientData);

	// 장비와 재료가 모두 꽂혔을 경우
	if (true == isTargetItemSelected && true == isIngredientItemSelected)
	{
		_textBlockItemName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SetEquipItemInfo(equipData);

		SetMagicalForgeExpectInfo(equipData, ingredientData);
	}
	// 장비만 꽂혔을 경우
	else if (true == isTargetItemSelected && false == isIngredientItemSelected)
	{
		_textBlockItemName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SetEquipItemInfo(equipData);
		
		FTextKey textKey(TEXT("ItemUi_MagicalForge_Notice_PickItem"));
		HideDetailInfo(textKey);

		SetMagicalForgeOff();
	}
	// 재료만 꽂혔을 경우
	else if (false == isTargetItemSelected && true == isIngredientItemSelected)
	{
		_textBlockItemName->SetVisibility(ESlateVisibility::Collapsed);

		FTextKey textKey(TEXT("ItemUi_MagicalForge_Notice_PickItem"));
		HideDetailInfo(textKey);
		
		SetMagicalForgeOff();
	}
	// 아무것도 꽂히지 않았을 경우
	else // (false == isTargetItemSelected && false == isIngredientItemSelected)
	{
		_textBlockItemName->SetVisibility(ESlateVisibility::Collapsed);

		FTextKey textKey(TEXT("ItemUi_MagicalForge_Notice_PickItem"));
		HideDetailInfo(textKey);

		SetMagicalForgeOff();
	}
}

void UGsUIItemMagicalForgeGroup::SetEquipItemInfo(const FGsItemEquipment* InEquipData)
{
	if (nullptr == InEquipData)
		return;

	FString strItemName;
	strItemName = FString::Format(TEXT("+{0} {1}"), { FString::FormatAsNumber(InEquipData->GetLevel()), *InEquipData->GetName().ToString() });
	FText textItemName = FText::FromString(strItemName);
	const FLinearColor& itemGradeColor = FGsUIColorHelper::GetColorItemGrade(InEquipData->GetGradeType());
	_textBlockItemName->SetText(textItemName);
	_textBlockItemName->SetColorAndOpacity(itemGradeColor);
}

void UGsUIItemMagicalForgeGroup::SetMagicalForgeExpectInfo(const FGsItemEquipment* InEquipData, const FGsItemIngredient* InIngredientData)
{
	if (nullptr == InEquipData)
		return;

	if (nullptr == InIngredientData)
		return;

	// 유효하지 않은 재료의 EffectId
	if (0 == InIngredientData->FindFirstEffectId())
		return;

	const FGsSchemaItemEffectMagicalForge* currentMagicalForge = UGsTableUtil::FindRowById<UGsTableItemEffectMagicalForge, FGsSchemaItemEffectMagicalForge, int32>(InEquipData->GetMagicalForgeData().mMagicalForgeId);
	const FGsSchemaItemEffectMagicalForge* expectMagicalForge = UGsTableUtil::FindRowById<UGsTableItemEffectMagicalForge, FGsSchemaItemEffectMagicalForge, int32>(InIngredientData->FindFirstEffectId());
	
	Level expectLevel = InEquipData->GetMagicalForgeData().mLevel + 1;

	// 동일한 효과에 대한 마법 부여 : 강화
	if (InEquipData->GetMagicalForgeData().mMagicalForgeId == InIngredientData->FindFirstEffectId())
	{
		bool isExistNextLevel = false;
		for (const FGsSchemaItemMagicalForgeData& magicalForgeLevelData : expectMagicalForge->levelGroupList)
		{
			if (magicalForgeLevelData.level == expectLevel)
			{
				isExistNextLevel = true;
				break;
			}
		}

		// 다음 레벨의 효과정보가 없는 것을 더 이상 강화할 수 없는 상태로 판단
		if (false == isExistNextLevel)
		{
			FTextKey textKey(TEXT("ItemUi_MagicalForge_Notice_MagicalGradeMax"));
			HideDetailInfo(textKey);

			SetMagicalForgeOff();
			return;
		}
	}
	// 다른 효과에 대한 마법 부여 : 변경
	else
	{
		// 1레벨로 변경이 된다.
		expectLevel = 1;
	}
	
	Level currentLevel = InEquipData->GetMagicalForgeData().mLevel;

	// 효과 노출 공용 처리
	_contentsSwitcher->SetActiveWidgetIndex(SHOW_DETAIL_INFO);
	SetCurrentEffectInfo(currentMagicalForge, currentLevel);
	SetExpectEffectInfo(expectMagicalForge, expectLevel);
	SetMagicalForgeOn();
}

void UGsUIItemMagicalForgeGroup::SetCurrentEffectInfo(const FGsSchemaItemEffectMagicalForge* InMagicalForgeData, Level InLevel)
{
	const FGsSchemaPassivitySet* passivityData = nullptr;

	// 표시할 효과 탐색
	if (nullptr != InMagicalForgeData)
	{
		for (const FGsSchemaItemMagicalForgeData& magicalForgeLevelData : InMagicalForgeData->levelGroupList)
		{
			if (magicalForgeLevelData.level == InLevel)
			{
				passivityData = magicalForgeLevelData.passivityId.GetRow();
			}
		}
	}
	
	FText nameText;
	FLinearColor nameColor = FGsUIColorHelper::GetColorItemGrade(ItemGrade::NORMAL);
	FText conditionText;
	FText effectText;

	if (nullptr == passivityData)
	{
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_MagicalForge_Nothing"), nameText);
		conditionText = FText::FromString(TEXT("-"));
		effectText = FText::FromString(TEXT("-"));
	}
	else
	{
		// 효과 이름 및 단계
		GetNameText(passivityData->nameText, InLevel, nameText);
		nameColor = FGsUIColorHelper::GetColorItemGrade(InMagicalForgeData->grade);

		GetConditionAndEffectText(passivityData->id, conditionText, effectText);
	}

	_textBlockCurrentName->SetText(nameText);
	_textBlockCurrentName->SetColorAndOpacity(nameColor);

	_entryCondition->SetCurrentText(conditionText);
	_entryEffect->SetCurrentText(effectText);
}

void UGsUIItemMagicalForgeGroup::SetExpectEffectInfo(const FGsSchemaItemEffectMagicalForge* InMagicalForgeData, Level InLevel)
{
	const FGsSchemaPassivitySet* passivityData = nullptr;

	int32 probability = 0;
	Currency goldAmount = 0;
	// 표시할 효과 탐색
	if (nullptr != InMagicalForgeData)
	{
		for (const FGsSchemaItemMagicalForgeData& magicalForgeLevelData : InMagicalForgeData->levelGroupList)
		{
			if (magicalForgeLevelData.level == InLevel)
			{
				passivityData = magicalForgeLevelData.passivityId.GetRow();
				probability = magicalForgeLevelData.probability;
				goldAmount = magicalForgeLevelData.costGoldAmount;
			}
		}
	}

	FText nameText;
	FLinearColor nameColor = FGsUIColorHelper::GetColorItemGrade(ItemGrade::NORMAL);
	FText conditionText;
	FText effectText;

	if (nullptr == passivityData)
	{
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_MagicalForge_Nothing"), nameText);
		conditionText = FText::FromString(TEXT("-"));
		effectText = FText::FromString(TEXT("-"));
	}
	else
	{
		// 효과 이름 및 단계
		GetNameText(passivityData->nameText, InLevel, nameText);
		nameColor = FGsUIColorHelper::GetColorItemGrade(InMagicalForgeData->grade);

		GetConditionAndEffectText(passivityData->id, conditionText, effectText);
	}

	_textBlockExpectName->SetText(nameText);
	_textBlockExpectName->SetColorAndOpacity(nameColor);

	const FLinearColor& expectColor = FGsUIColorHelper::GetColor(EGsUIColorType::ENCHANT_EXPECT_VALUE);

	_entryCondition->SetExpectText(conditionText);
	_entryCondition->SetExpectColor(expectColor);

	_entryEffect->SetExpectText(effectText);
	_entryEffect->SetExpectColor(expectColor);

	// 확률 툴팁 정보
	const FText prevLevelText = FGsUIStringHelper::MakeFormatedText(TEXT("Lv.{0}"), InLevel - 1);
	const FText expectLevelText = FGsUIStringHelper::MakeFormatedText(TEXT("Lv.{0}"), InLevel);
	_textBlockItemNameProbability->SetText(nameText);
	_textBlockItemNameProbability->SetColorAndOpacity(nameColor);
	_uiProbabilityEntry->SetTitleText(ItemEnchantResult::ENCHANT_SUCCEEDED);
	_uiProbabilityEntry->SetCurrentText(prevLevelText);
	_uiProbabilityEntry->SetExpectText(expectLevelText);
	_uiProbabilityEntry->SetPercentText(probability);

	SetGuideText(probability == 10000);
	SetCurrency(goldAmount);
}

void UGsUIItemMagicalForgeGroup::SetGuideText(bool InIsSafe)
{
	FText findText;
	if (InIsSafe)
	{
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_MagicalForge_Safe"), findText);
	}
	else
	{
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_MagicalForge_NotSafe"), findText);
	}

	_panelGuideBottom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_textBlockGuideBottom->SetText(findText);
	_textBlockGuideProbability->SetText(findText);
}

void UGsUIItemMagicalForgeGroup::GetNameText(const FText& InNameText, Level InLevel, OUT FText& OutNameText)
{
	//FString strName = InNameText.ToString();
	//FText levelFormat;
	//FText::FindText(TEXT("FairyText"), TEXT("FairyUI_LevelFormat"), levelFormat);
	//FString strLevel = FText::Format(levelFormat, InLevel).ToString();

	//OutNameText = FText::FromString(strName + TEXT(" ") + strLevel);
	OutNameText = InNameText;
}

void UGsUIItemMagicalForgeGroup::GetConditionAndEffectText(int32 InPassivityId, OUT FText& OutConditionText, OUT FText& OutEffectText)
{
	FGsEffectTextManager* effectTextManager = GSEffectText();
	if (nullptr == effectTextManager)
		return;

	if (const FGsSchemaEffectTextPassivityEffect* effectTextData = GetEffectTextData(InPassivityId))
	{
		// 조건 및 확률
		FString strCondition;
		effectTextManager->GetStringPassivityCauseAndRate(effectTextData, strCondition);
		OutConditionText = FText::FromString(strCondition);

		// 발동 시 효과
		FString strEffect;
		effectTextManager->GetStringPassivityEffect(effectTextData, strEffect);
		OutEffectText = FText::FromString(strEffect);
	}
}

void UGsUIItemMagicalForgeGroup::HideDetailInfo(const FTextKey& InTextKey)
{
	_contentsSwitcher->SetActiveWidgetIndex(HIDE_DETAIL_INFO);

	FText findText;
	FText::FindText(TEXT("ItemUIText"), InTextKey, findText);
	_textBlockGuideMiddle->SetText(findText);

	_panelGuideBottom->SetVisibility(ESlateVisibility::Collapsed);

	OnClickProbabilityPanel();
}

void UGsUIItemMagicalForgeGroup::SetCurrency(Currency InCurrency)
{
	_btnEnchant->SetData(CurrencyType::GOLD, InCurrency);
	_btnEnchant->UpdateAmountTextColorLocalPlayer();
}

void UGsUIItemMagicalForgeGroup::SetMagicalForgeOn()
{
	_btnEnchant->SetIsEnabled(true);
}

void UGsUIItemMagicalForgeGroup::SetMagicalForgeOff()
{
	_btnEnchant->SetIsEnabled(false);
	SetCurrency(0);
}

bool UGsUIItemMagicalForgeGroup::CanRegisterEquipment(const ItemDBId InItemDBId, bool InIsShowTicker)
{
	const FGsSchemaItemEquipConfig* itemEquipConfig = UGsTableUtil::FindFirstRow<FGsSchemaItemEquipConfig>();
	if (nullptr == itemEquipConfig)
		return false;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return false;

	TWeakPtr<FGsItem> itemData = itemManager->FindItem(InItemDBId, ItemType::EQUIP);
	if (false == itemData.IsValid())
		return false;

	// ItemEquipConfig 에 등록된 등급 이상인가?
	bool isValidGrade = static_cast<uint8>(itemData.Pin()->GetGradeType()) >= static_cast<uint8>(itemEquipConfig->magicalForgeGrade);
	if (InIsShowTicker && false == isValidGrade)
	{
		// TEXT: 마법 부여가 불가능한 등급의 장비입니다.
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_MagicalForge_Notice_NotEnoughItemGrade"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
	}

	return isValidGrade;
}

bool UGsUIItemMagicalForgeGroup::CanRegisterIngredient(const ItemDBId InItemDBId)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return false;

	const FGsItemIngredient* ingredientData = nullptr;
	TWeakPtr<FGsItem> findIngredientItem = itemManager->FindItem(InItemDBId, ItemType::INGREDIENT);
	if (findIngredientItem.IsValid())
	{
		ingredientData = findIngredientItem.Pin()->GetCastItem<FGsItemIngredient>();
	}

	if (nullptr == ingredientData)
		return false;

	if (false == ingredientData->IsMagicalForgeItem())
		return false;

	return true;
}

const FGsSchemaEffectTextPassivityEffect* UGsUIItemMagicalForgeGroup::GetEffectTextData(int32 InPassivityId)
{
	const FGsSchemaEffectTextPassivity* effectTextData = UGsTableUtil::FindRowById<UGsTableEffectTextPassivity, FGsSchemaEffectTextPassivity>(InPassivityId);
	if (nullptr == effectTextData)
		return nullptr;

	if (effectTextData->effectList.IsValidIndex(0))
	{
		const FGsSchemaEffectTextPassivityEffect& effectTextDataDetail = effectTextData->effectList[0];
		return &effectTextDataDetail;
	}

	return nullptr;
}

void UGsUIItemMagicalForgeGroup::OnClickTargetItem()
{
	if (OnClickMagicalForgeIconSlot.IsBound())
	{
		OnClickMagicalForgeIconSlot.Broadcast(true, _targetItemDBId);
	}
}

void UGsUIItemMagicalForgeGroup::OnClickIngredientItem()
{
	if (OnClickMagicalForgeIconSlot.IsBound())
	{
		OnClickMagicalForgeIconSlot.Broadcast(false, _ingredientItemDBId);
	}
}

void UGsUIItemMagicalForgeGroup::OnClickEnchant()
{
	if (_mainSlotsafeEnchantEffect->IsPlayingAnimation())
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	// 다른 이름 툴팁 닫기
	if (UGsTooltipManager* tooltipManager = uiManager->GetTooltipManager())
	{
		tooltipManager->CloseNameTooltip();
	}

	// 재화 부족 팝업
	Currency cost = _btnEnchant->GetCurrencyAmount();
	if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::GOLD, cost, false))
	{
		Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
		FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, cost - myAmount);
		return;
	}

	bool isSameEffect = false;
	if (FGsItemHelper::CanMagicalForge(_targetItemDBId, _ingredientItemDBId, true, isSameEffect))
	{
		if (isSameEffect)
		{
			OnConfirmEnchant();
		}
		else
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupMagicalForgeChange"));
			if (widget.IsValid())
			{
				if (UGsUIPopupMagicalForgeChange* popup = Cast<UGsUIPopupMagicalForgeChange>(widget.Get()))
				{
					popup->SetCurrentText(_textBlockCurrentName->GetText(), _textBlockCurrentName->ColorAndOpacity);
					popup->SetExpectText(_textBlockExpectName->GetText(), _textBlockExpectName->ColorAndOpacity);
					popup->SetCallback([this](bool bIsOk)
						{
							if (bIsOk)
							{
								OnConfirmEnchant();
							}
						});
				}
			}
		}
	}
}

void UGsUIItemMagicalForgeGroup::OnClickProbability()
{
	if (0 >= _targetItemDBId || 0 >= _ingredientItemDBId)
	{
		_panelProbability->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	if (ESlateVisibility::Collapsed != _panelProbability->GetVisibility())
	{
		_panelProbability->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_panelProbability->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIItemMagicalForgeGroup::OnClickProbabilityPanel()
{
	_panelProbability->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIItemMagicalForgeGroup::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->ResetCoolTime();
	itemIcon->SetOwnedItem(_targetItemDBId);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameToolTipFlag(0 < _targetItemDBId);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
}

void UGsUIItemMagicalForgeGroup::OnRefreshIngredientIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->ResetCoolTime();
	itemIcon->SetOwnedItem(_ingredientItemDBId);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameToolTipFlag(0 < _ingredientItemDBId);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
}

void UGsUIItemMagicalForgeGroup::OnConfirmEnchant()
{
	_mainSlotsafeEnchantEffect->_playbackSpeed = 0.f >= _enchantAnimSpeedValue ? 1.f : _enchantAnimSpeedValue;
	_mainSlotsafeEnchantEffect->ShowVFX();

	if (FGsItemEnchantManager* enchantManager = GSItemEnchant())
	{
		enchantManager->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::MagicalForge, _enchantAnimSpeedValue);
	}	
}

void UGsUIItemMagicalForgeGroup::OnEnchantEffectAnimationFinished()
{
	if (0 >= _targetItemDBId || 0 >= _ingredientItemDBId)
		return;

	FGsShowEnchantEffect paramData(_targetItemDBId, _ingredientItemDBId, ItemEnchantResult::NONE, false);
	GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::SHOW_MAGICALFORGE_EFFECT, &paramData);
}
