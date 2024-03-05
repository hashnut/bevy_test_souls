#include "GsUIItemDetailInfo.h"

#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/TileView.h"
#include "Components/SizeBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/Spacer.h"

#include "Currency/GsCurrencyHelper.h"

#include "Data/GsDataContainManager.h"

#include "DataSchema/GsSchemaCurrencyData.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Item/GsSchemaItemMaterialData.h"
#include "DataSchema/Item/GsSchemaItemExpiredTime.h"

#include "GameObject/Stat/GsStatHelper.h"

#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemConsumable.h"
#include "Item/GsItemConsumableBox.h"
#include "Item/GsItemIngredient.h"
#include "Item/GsItemStat.h"
#include "Item/GsItemHelper.h"
#include "Item/Data/GsItemTableStruct.h"
#include "Item/Collection/GsItemCollectionCondition.h"

#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGame/GsUtilizingContentsManager.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Management/ScopeGame/GsFairyManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Skill/GsSkillHelper.h"

#include "T1Project.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

#include "UI/UIContent/Common/Item/GsUIItemStatListEntry.h"
#include "UI/UIContent/Common/Item/GsUIItemSetStatListEntry.h"
#include "UI/UIContent/Common/Item/GsUIItemSetItemListEntry.h"
#include "UI/UIContent/Common/Item/GsUIItemSkillOptionListEntry.h"
#include "UI/UIContent/Common/Item/GsUIItemDescListEntry.h"
#include "UI/UIContent/Common/Item/GsUITooltipWeaponSkillInfo.h"
#include "UI/UIContent/Common/Item/GsUIItemCollectionMaterialLevel.h"

#include "UI/UIContent/Define/EGsUIColorType.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"

#include "UI/UIContent/Common/Item/GsUIItemSupplyIconEntry.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "UTIL/GsUIUtil.h"


const int32 INVALID_CURRENCY_AMOUNT = -1;

void UGsUIItemDetailInfo::BeginDestroy()
{
	if (_slotHelperStatList)
	{
		_slotHelperStatList->OnRefreshEntry.RemoveDynamic(this, &UGsUIItemDetailInfo::OnRefreshEntryStatList);
	}
	_slotHelperStatList = nullptr;

	if (_slotHelperSuppDesc)
	{
		_slotHelperSuppDesc->OnRefreshEntry.RemoveDynamic(this, &UGsUIItemDetailInfo::OnRefreshEntrySuppDesc);
	}
	_slotHelperSuppDesc = nullptr;

	if (_slotHelperMaterialLevelSet)
	{
		_slotHelperMaterialLevelSet->OnRefreshEntry.RemoveDynamic(this, &UGsUIItemDetailInfo::OnRefreshEntryCollectionMaterialLevel);
		
		_slotHelperMaterialLevelSet = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIItemDetailInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// 리스트 채우기
	GetAllChildListEntry<UGsUIItemStatListEntry>(_panelEnchantPropListRoot, _enchantPropSlotList);
	GetAllChildListEntry<UGsUIItemStatListEntry>(_panelRefineListRoot, _refineSlotList);
	GetAllChildListEntry<UGsUIItemSetStatListEntry>(_panelSetStatListRoot, _setStatSlotList);
	GetAllChildListEntry<UGsUIItemSetItemListEntry>(_panelSetItemListRoot, _setItemSlotList);
	GetAllChildListEntry<UGsUIItemSkillOptionListEntry>(_skillOptionListRoot, _skillOptionSlotList);

	_toggleGroupEquipTab.OnSelectChanged.BindUObject(this, &UGsUIItemDetailInfo::OnSelectEquipTab);
	_toggleGroupEquipTab.AddToggleWidgetByParentPanel(_panelEquipTabListRoot);

	_slotHelperStatList = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperStatList->Initialize(_entryClassStatWidget, _panelStatListRoot);
	_slotHelperStatList->OnRefreshEntry.AddDynamic(this, &UGsUIItemDetailInfo::OnRefreshEntryStatList);

	_slotHelperSuppDesc = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperSuppDesc->Initialize(_entryClassSuppWidget, _panelSuppDescListRoot);
	_slotHelperSuppDesc->OnRefreshEntry.AddDynamic(this, &UGsUIItemDetailInfo::OnRefreshEntrySuppDesc);

	_slotHelperMaterialLevelSet = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperMaterialLevelSet->Initialize(_subClassOfMaterialLevelEntry, _horizontalBoxMaterialLevelSet);
	_slotHelperMaterialLevelSet->OnRefreshEntry.AddDynamic(this, &UGsUIItemDetailInfo::OnRefreshEntryCollectionMaterialLevel);

	_btnEnchantMinus->OnClicked.AddDynamic(this, &UGsUIItemDetailInfo::OnClickEnchantMinus);
	_btnEnchantPlus->OnClicked.AddDynamic(this, &UGsUIItemDetailInfo::OnClickEnchantPlus);

	//_supplyTileViewPool = NewObject<UGsUObjectPool>(this);
	//_supplyTileViewPool->InitPool<UGsTileView>();
}

void UGsUIItemDetailInfo::NativeConstruct()
{
	Super::NativeConstruct();

	_suppPeriodRootPanel->SetVisibility(ESlateVisibility::Collapsed);
	_suppExpiredTimeRootPanel->SetVisibility(ESlateVisibility::Collapsed);

	_periodRootPanel->SetVisibility(ESlateVisibility::Collapsed);
	_expiredTimeRootPanel->SetVisibility(ESlateVisibility::Collapsed);

	// 세트효과 탭 클릭후 아이템 상세창 닫고 다시 열 때, '기본 정보' 탭이 열리도록 설정 (C2URWQ-3909)
	_toggleGroupEquipTab.SetSelectedIndex(0);
	//_switcherEquipPage->SetActiveWidgetIndex(0);

	_isRunningExpiredTimeFlag = false;

	_verticalBoxGuildCollectionEquip->SetVisibility(ESlateVisibility::Collapsed);
	_verticalBoxGuildCollectionSupplies->SetVisibility(ESlateVisibility::Collapsed);

	_panelMagicalForge->SetVisibility(ESlateVisibility::Collapsed);

	UGsSupplyManager* SupplyManager = GSSupply();
	FGsUtilizingContentsManager* utilizingContentsMgr = GSUtilizingContents();
	UGsFairyManager* fairyManager = GSFairy();

	if (nullptr == SupplyManager || nullptr == utilizingContentsMgr || nullptr == fairyManager)
	{
		GSLOG(Error, TEXT("nullptr == SupplyManager || nullptr == utilizingContentsMgr"));
	}
	else
	{
		SupplyManager->RegisterHyperLinkDelegate(_rbAcquire);
		SupplyManager->RegisterHyperLinkDelegate(_rbSuppAcquire);
		utilizingContentsMgr->RegisterHyperLinkDelegate(_rbUtilize);
		utilizingContentsMgr->RegisterHyperLinkDelegate(_rbSuppUtilize);
		fairyManager->RegisterHyperLinkDelegate(_rbSummonInfo);
	}

	_collectionMarkingDataList.Empty();
	
	_displayedRewardId = INVALID_REWARD_ID;
}

void UGsUIItemDetailInfo::NativeDestruct()
{
	if (_displayedItem.IsValid())
	{
		_displayedItem.Reset();
	}

	if (_compareItem.IsValid())
	{
		_compareItem.Reset();
	}

	Super::NativeDestruct();
}

void UGsUIItemDetailInfo::SetCompareData(const FGsItem* InItem, const FGsItem* InCompareItem)
{
	if (nullptr == InItem || nullptr == InCompareItem)
		return;

	if (false == InItem->IsEquipment() || false == InCompareItem->IsEquipment())
		return;

	_displayedItem = MakeShareable(FGsItem::Create(InItem));
	_compareItem = MakeShareable(FGsItem::Create(InCompareItem));
	_isCompareMode = true;

	_switcherItemType->SetActiveWidgetIndex(0);
	const FGsItemEquipment* equipData = static_cast<const FGsItemEquipment*>(InItem);
	const FGsItemEquipment* compareEquipData = static_cast<const FGsItemEquipment*>(InCompareItem);

	SetUIRefineOption(equipData);
	SetUIMaterial(equipData);
	SetUIEquipWeight(equipData);
	SetUIPeriod(InItem);
	SetUIMagicalForge(InItem);
	SetUIEnchant(equipData);
	InvalidateEnchantPanel(); // 스탯, 특성, 스킬 정보 갱신
	//SetUIItemCompareStat(equipData, compareEquipData);
	//SetUIEnchantProperty(equipData);
	//SetSkillOptionData(equipData);

	if (equipData->IsSetItem())
	{
		_panelEquipTabRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		SetUISetStatInfo(equipData);
		SetUISetItemInfo(equipData);
	}
	else
	{
		_toggleGroupEquipTab.SetSelectedIndex(0);
		_panelEquipTabRoot->SetVisibility(ESlateVisibility::Collapsed);
	}

	SetUIAcquire(InItem->MakeSupplyHyperLink());
	SetUIUtilize(InItem->MakeUtilizeHyperLink());
}

void UGsUIItemDetailInfo::SetWeaponSkillInfo(FText&& InTitleText, FText&& InDescText, bool InIsPassive)
{
	if (nullptr == _tooltipWeaponSkillInfo)
	{
		GSLOG(Error, TEXT("nullptr == _tooltipWeaponSkillInfo"));
		return;
	}

	_tooltipWeaponSkillInfo->SetData(MoveTemp(InTitleText), MoveTemp(InDescText), InIsPassive);
	_tooltipWeaponSkillInfo->SetTitleColor(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_VALID));
	_tooltipWeaponSkillInfo->Open();
}

void UGsUIItemDetailInfo::SetData(const FGsItem* InItem)
{
	if (nullptr == InItem)
		return;

	_displayedItem = MakeShareable(FGsItem::Create(InItem));
	_isCompareMode = false;

	if (true == InItem->IsEquipment())
	{
		_displayedRewardId = INVALID_REWARD_ID;

		_switcherItemType->SetActiveWidgetIndex(0);
		const FGsItemEquipment* equipItem = static_cast<const FGsItemEquipment*>(InItem);
		if (nullptr != equipItem)
		{
			SetUIRefineOption(equipItem);
			SetUIMaterial(equipItem);
			SetUIEquipWeight(equipItem);
			SetUIPeriod(InItem);
			SetUIMagicalForge(InItem);
			SetUIEnchant(equipItem);

			InvalidateEnchantPanel(); // 스탯, 특성, 스킬 정보 갱신
			//SetUIItemStat(equipItem);
			//SetUIEnchantProperty(equipItem);
			//SetSkillOptionData(equipItem);

			if (equipItem->IsSetItem())
			{
				_panelEquipTabRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				SetUISetStatInfo(equipItem);
				SetUISetItemInfo(equipItem);
			}
			else
			{
				_toggleGroupEquipTab.SetSelectedIndex(0);
				_panelEquipTabRoot->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			// 끄기처리
			SetUIItemStat(nullptr);
			SetUIEnchantProperty(nullptr);
			SetUIRefineOption(nullptr);
			SetUIMaterial(nullptr);
			SetUIEquipWeight(nullptr);
			SetSkillOptionData(nullptr);
			SetUIMagicalForge(nullptr);
			SetUIEnchant(nullptr);
		}
	}
	else
	{
		_switcherItemType->SetActiveWidgetIndex(1);
		SetUISuppliesInfo(InItem);
		if (InItem->IsConsumable() ||InItem->IsIngredient())
		{
			SetUIConsumableInfo(InItem);
		}
		else
		{
			// 끄기처리
			SetUIConsumableInfo(nullptr);
		}
	}

	SetUIAcquire(InItem->MakeSupplyHyperLink());
	SetUIUtilize(InItem->MakeUtilizeHyperLink());
}

void UGsUIItemDetailInfo::SetData(const CurrencyType InCurrencyType, const int64 InAmount)
{
	if (CurrencyType::NONE == InCurrencyType)
	{
		GSLOG(Error, TEXT("CurrencyType::NONE == InCurrencyType"));
		return;
	}

	_switcherItemType->SetActiveWidgetIndex(1);

	// 끄기처리
	SetUISuppliesInfo(nullptr);

	SetUIConsumableInfo(nullptr);

	SetUICurrencyInfo(InCurrencyType, InAmount);
}

bool UGsUIItemDetailInfo::isExistCompareStat(TArray<FGsItemStat*> InCompareStatDataList, StatType InStatType , OUT int32& outCompareIndex)
{
	for (uint8 i = 0; i < InCompareStatDataList.Num(); ++i)
	{
		if(nullptr == InCompareStatDataList[i])
			continue;

		if (InStatType == InCompareStatDataList[i]->GetStatType())
		{
			outCompareIndex = i;
			return true;
		}
	}

	return false;
}


bool UGsUIItemDetailInfo::GetCalcCompareData(FGsItemStat* InStatDataList, TArray<FGsItemStat*> InCompareStatDataList, OUT int32& outCalcValue)
{
	int32 indexNum = 0;
	FGsItemStat* resultCalcData = nullptr;
	if (true == isExistCompareStat(InCompareStatDataList, InStatDataList->GetStatType(), indexNum))
	{
		if (false == InCompareStatDataList.IsValidIndex(indexNum))
			return false;

		FGsItemStat* compareData = InCompareStatDataList[indexNum];
		if (nullptr == compareData)
			return false;

		int32 oriValue = InStatDataList->GetStatOriValue();
		int32 calcValue = InStatDataList->GetCalcStatValue();
		int32 compareOriValue = compareData->GetStatOriValue();
		int32 compareCalcValue = compareData->GetCalcStatValue();
		int32 resultValue = (oriValue + calcValue) - (compareOriValue + compareCalcValue);	
		outCalcValue = resultValue;
		return true;
	}

	return false;
}

void UGsUIItemDetailInfo::SetCompareValueVisibility(bool InState)
{
	_isCompareMode = InState;

	InvalidateEnchantPanel();
}

void UGsUIItemDetailInfo::SetComareItemEnchantCount(int32 InCount)
{
	if (false == _compareItem.IsValid())
		return;

	FGsItemEquipment* compareItemEquipment = _compareItem->GetCastItem<FGsItemEquipment>();
	if (nullptr == compareItemEquipment)
		return;

	compareItemEquipment->UpdateStat(InCount);
	compareItemEquipment->SetLevel(InCount);

	InvalidateEnchantPanel();
}

void UGsUIItemDetailInfo::SetGuildItemCollectionRegisterInfo(const FText& InText)
{
	const static int32 EQUIP_PAGE_INDEX = 0;
	const static int32 Supp_PAGE_INDEX = 1;
	if (0 == _switcherItemType->GetActiveWidgetIndex())
	{
		_richTextOnlyGuildItemCollectionRegisterInfoEquipment->SetText(InText);
		_richTextOnlyGuildItemCollectionRegisterInfoEquipment->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_verticalBoxGuildCollectionEquip->SetVisibility(ESlateVisibility::Visible);
	}
	else if (1 == _switcherItemType->GetActiveWidgetIndex())
	{
		_richTextOnlyGuildItemCollectionRegisterInfoSupplies->SetText(InText);
		_richTextOnlyGuildItemCollectionRegisterInfoSupplies->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_verticalBoxGuildCollectionSupplies->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGsUIItemDetailInfo::SetItemCollectionMarking(const FGsItem* InItem, bool InUseMarking)
{
	bool isMarking = (nullptr == InItem) ? false : InUseMarking;

	_collectionMarkingDataList.Empty();

	if (InItem && true == isMarking)
	{
		const auto& conditionSet = GSItemCollection()->FindConditionsByItemId(InItem->GetTID());
		for (const auto& condition : conditionSet)
		{
			const TArray<FGsItemCollectionCondition::CollectMaterialData> materialSet = condition->MaterialAll();
			const auto materialData = materialSet.FindByPredicate([InItem](const FGsItemCollectionCondition::CollectMaterialData& InData)
				{
					return InData.Key == InItem->GetTID();
				}
			);

			if (materialData)
			{
				// 2023/4/18 PKT - 강화 레벨
				// materialData->Value;

				ECollectionMaterialActionType actionType = condition->ActionType();
				// 2023/4/18 PKT - 착용 중인 아이템은 컬렉션 메니져에서 상태 체크 제외 시키기 때문에 여기서 상태를 다시 구한다.
				if (ECollectionMaterialActionType::Completed != actionType)
				{
					actionType = (materialData->Value == InItem->GetLevel()) ? ECollectionMaterialActionType::ImmediatePossible : ECollectionMaterialActionType::ConditionalPossible;					
				}

				auto findData = _collectionMarkingDataList.FindByPredicate([materialData](const TPair<Level, ECollectionMaterialActionType>& InData)
					{
						return (materialData->Value == InData.Key);
					}
				);

				if (nullptr == findData)
				{
					_collectionMarkingDataList.Emplace(TPair<Level, ECollectionMaterialActionType>(materialData->Value, actionType));
				}
				else if(actionType < findData->Value)
				{
					findData->Value = actionType;
				}
			}
		}
	}

	if (isMarking && 0 < _collectionMarkingDataList.Num())
	{	// 2023/4/18 PKT - Draw
		_panelCollectionInfoRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		_collectionMarkingDataList.Sort([](const TPair<Level, ECollectionMaterialActionType>& lhr, const TPair<Level, ECollectionMaterialActionType>& rhr)
			{
				return lhr.Key < rhr.Key;
			}
		);

		const int32 maxCount = _collectionMarkingDataList.Num();
		_slotHelperMaterialLevelSet->RefreshAll(maxCount);
	}
	else
	{
		// 2023/4/18 PKT - hidden
		_panelCollectionInfoRoot->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIItemDetailInfo::SetItemCollectionMarking(const CurrencyType InCurrencyType, bool InUseMarking)
{
	bool isMarking = (CurrencyType::NONE == InCurrencyType) ? false : InUseMarking;
	
	isMarking = false;
	/**
	 * // 2023/4/18 PKT - 현재 CurrencyType은 지원하지 않음. 무조건 False
	 */
	_panelCollectionInfoRoot->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIItemDetailInfo::GetExistExpectNewOrRemoveStat(TArray<FGsItemStat*> InStatList, TArray<FGsItemStat*> InCompareStatList, StatCompareResultType InType, OUT TMap<FGsItemStat*, TPair<StatCompareResultType, int32>>& outResultMap)
{
	for (uint8 i = 0; i < InCompareStatList.Num(); ++i)
	{
		bool findFlag = false;
		if(nullptr == InCompareStatList[i])
			continue;

		for (uint8 j = 0; j < InStatList.Num(); ++j)
		{
			if(nullptr == InStatList[j])
				continue;

			if (InCompareStatList[i]->GetStatType() != InStatList[j]->GetStatType())
				continue;

			findFlag = true;
		}

		if (false == findFlag)
		{
			outResultMap.Emplace(InCompareStatList[i], TPair<StatCompareResultType, int32>(InType, 0));
		}
	}
}

void UGsUIItemDetailInfo::GetDisplayedStatList(const FGsItemEquipment* InDisplayedItem)
{
	_displayedStatList.Empty();
	TArray<FGsItemStat*> statDataList = InDisplayedItem->GetCurrentItemStatValue();
	for (FGsItemStat* statData : statDataList)
	{
		if (nullptr == statData)
			continue;

		FText textName;
		FString strValue;
		bool bResult = FGsStatHelper::GetStatNameAndValueWithAdditionalValue(
			statData->GetStatType(), statData->GetStatOriValue(), statData->GetCalcStatValue(),
			textName, strValue);

		if (bResult)
		{
			_displayedStatList.Emplace(statData->GetStatType(), textName, FText::FromString(strValue), StatCompareResultType::NONE, 0);
		}
	}
}

void UGsUIItemDetailInfo::GetComparedStatList(const FGsItemEquipment* InDisplayedItem, const FGsItemEquipment* InComparedItem)
{
	_displayedStatList.Empty();

	if (nullptr == InDisplayedItem)
		return;

	if (nullptr == InComparedItem)
		return;

	TArray<FGsItemStat*> statDataList = InDisplayedItem->GetCurrentItemStatValue();
	TArray<FGsItemStat*> compareStatDataList = InComparedItem->GetCurrentItemStatValue();
	TMap<FGsItemStat*, TPair<StatCompareResultType, int32>> resultCompareStatDataMap;
	for (uint8 i = 0; i < statDataList.Num(); ++i)
	{
		if (nullptr == statDataList[i])
			continue;

		FGsItemStat* statData = statDataList[i];
		int32 clacValue = 0;
		if (true == GetCalcCompareData(statData, compareStatDataList, clacValue))
		{
			resultCompareStatDataMap.Emplace(statData, TPair<StatCompareResultType, int32>(StatCompareResultType::UPDATE, clacValue));
		}
	}

	GetExistExpectNewOrRemoveStat(compareStatDataList, statDataList, StatCompareResultType::ADD, resultCompareStatDataMap);
	GetExistExpectNewOrRemoveStat(statDataList, compareStatDataList, StatCompareResultType::REMOVE, resultCompareStatDataMap);

	for (const auto& iter : resultCompareStatDataMap)
	{
		FGsItemStat* statData = iter.Key;
		TPair<StatCompareResultType, int32> pairData = iter.Value;
		if (nullptr == statData)
			continue;

		FText textName;
		FString strValue;
		bool bResult = FGsStatHelper::GetStatNameAndValueWithAdditionalValue(
			statData->GetStatType(), statData->GetStatOriValue(), statData->GetCalcStatValue(),
			textName, strValue);

		if (bResult)
		{
			StatCompareResultType resultType = StatCompareResultType::UPDATE;
			int32 compareStatDeltaValue = 0;
			switch (pairData.Key)
			{
				// 값 변경만 있는 경우
			case StatCompareResultType::UPDATE:
			{
				resultType = StatCompareResultType::UPDATE;
				compareStatDeltaValue = pairData.Value;
			} break;
			// 신규 추가되는 옵션
			case StatCompareResultType::ADD:
			{
				resultType = StatCompareResultType::ADD;
			} break;
			// 제거되는 옵션
			case StatCompareResultType::REMOVE:
			{
				resultType = StatCompareResultType::REMOVE;
			} break;
			default:
				break;
			}
			_displayedStatList.Emplace(StatDisplayInfo(statData->GetStatType(), textName, FText::FromString(strValue), resultType, compareStatDeltaValue));
		}
	}
}

void UGsUIItemDetailInfo::SetUIItemStat(const FGsItemEquipment* InDisplayedItem, const FGsItemEquipment* InComparedItem)
{
	_compareVisibility = (nullptr != InComparedItem);

	if (nullptr == InDisplayedItem)
	{
		_panelStatRoot->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	// 아이템 설명
	FText descText = InDisplayedItem->GetToolTipDesc();
	if (descText.IsEmpty())
	{
		_panelTooltipDesc->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_panelTooltipDesc->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_tbStatTooltipName->SetText(descText);
	}

	// 아이템 스탯 효과 목록 -> _displayedStatList 로 저장
	if (_compareVisibility)
	{
		GetComparedStatList(InDisplayedItem, InComparedItem);
	}
	else
	{
		GetDisplayedStatList(InDisplayedItem);
	}

	if (_displayedStatList.Num() == 0)
	{
		_panelStatListRoot->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_panelStatListRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		_slotHelperStatList->RefreshAll(_displayedStatList.Num());
	}

	// 효과 정보 표시 결정
	if (false == descText.IsEmpty() || 0 < _displayedStatList.Num())
	{
		_panelStatRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_panelStatRoot->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (false == descText.IsEmpty() && 0 < _displayedStatList.Num())
	{
		_spacerTooltip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_spacerTooltip->SetVisibility(ESlateVisibility::Collapsed);
	}

	_displayedStatList.Empty();
}

void UGsUIItemDetailInfo::SetUIEnchantProperty(const FGsItemEquipment* InEquipItem)
{
	if (nullptr == InEquipItem || InEquipItem->IsCollectableType() || false == InEquipItem->IsUseEnchantBonusStat())
	{
		_panelEnchantPropRoot->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	if (UNCONFIRMED_ENCHANT_BONUS_INDEX == InEquipItem->GetEnchantBonusIndex())
	{
		_panelEnchantPropRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_panelEnchantPropListRoot->SetVisibility(ESlateVisibility::Collapsed);

		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("UI_Text_EnchantBonus_NotYetReceived"), findText);
		_tbEnchantPropName->SetText(findText); // 아이템 수령 후 특성을 확인할 수 있습니다.
		_tbEnchantPropName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(ItemGrade::NORMAL));
		return;
	}

	const FString strTitle = InEquipItem->GetBonusStatTitleName();
	if (strTitle.IsEmpty())
	{
		_panelEnchantPropRoot->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_panelEnchantPropRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_panelEnchantPropListRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 메인 이름, 컬러 세팅
	_tbEnchantPropName->SetText(FText::FromString(strTitle));
	_tbEnchantPropName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InEquipItem->GetGradeType()));

	// TEXT: 강화
	FText textEnchant;
	FText::FindText(TEXT("UICommonText"), TEXT("Enchant"), textEnchant);

	// 스탯 리스트 세팅
	TArray<int32> levelList = InEquipItem->FindAllBonusStatLevel();
	for (int32 i = 0; i < _enchantPropSlotList.Num(); ++i)
	{
		UGsUIItemStatListEntry* slot = _enchantPropSlotList[i];
		if (nullptr == slot)
		{
			continue;
		}

		if (levelList.IsValidIndex(i))
		{			
			int32 level = levelList[i];

			// 예: 공격력 10 (강화 +8)

			// 데이터상에는 statList 로, 여러개를 가질수 있는데, ui 상에는 하나밖에 없으므로 우선은 하나만 세팅함
			// 추후 ui 상에서도 여러개를 가질수 있도록 표현해야함
			TArray<FGsItemStatData> statDataList = InEquipItem->FindBonusStatDataByLevel(level);
			if (0 < statDataList.Num())
			{
				FGsItemStatData& statData = statDataList[0];
				if (StatType::NONE == statData.type)
				{
					slot->SetVisibility(ESlateVisibility::Collapsed);
					continue;
				}

				TPair<FText, FText> pairText;
				if (false == FGsStatHelper::GetDesplayStatNameAndValue(statData.type, statData.value, pairText, false))
				{
					continue;
				}

				// 예: 10 (강화 +8)
				FString strValueFinal = FString::Format(TEXT("{0} ({1} +{2})"), 
					{ *(pairText.Value.ToString()), textEnchant.ToString(), level });				

				slot->SetVisibility(ESlateVisibility::HitTestInvisible);
				slot->SetTextName(pairText.Key);
				slot->SetTextValue(FText::FromString(strValueFinal));
				bool bIsOpen = (InEquipItem->GetLevel() >= level) ? true : false;
				slot->SetColorAllText(FGsUIColorHelper::GetColor(bIsOpen ?
					EGsUIColorType::DEFAULT_STAT_COLOR : EGsUIColorType::DISABLE_STAT_COLOR));

				continue;
			}
		}

		// 예외 상황에선 감춤
		slot->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIItemDetailInfo::SetUIRefineOption(const FGsItemEquipment* InEquipItem)
{
	if (nullptr == InEquipItem)
	{
		_panelRefineRoot->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	FGsItemEquipment* data = const_cast<FGsItemEquipment*>(InEquipItem);
	TArray<FGsItemStat*> statDataList;
	if (false == data->GetAllRefineOptionStatList(statDataList))
	{
		_panelRefineRoot->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	// 스텟타입,<최소값,최대값>
	TMap<StatType, TPair<int32, int32>> statRangeMap;
	// 제련석과 빛나는 제련석의 옵션 수치는 동일하다.
	data->GetAllExpectRefineOptionStatList(ItemCategorySub::OPTION_STONE, statRangeMap);

	_panelRefineRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 제련 스탯
	int32 lastSlotIndex = 0
;	for (int32 i = 0; i < _refineSlotList.Num(); ++i)
	{
		UGsUIItemStatListEntry* slot = _refineSlotList[i];
		if (nullptr == slot)
		{
			continue;
		}

		if (statDataList.IsValidIndex(i))
		{
			if (FGsItemStat* statData = statDataList[i])
			{
				// 예: 공격력 +10
				TPair<FText, FText> pairText;
				if (false == FGsStatHelper::GetDesplayStatNameAndValue(statData->GetStatType(), statData->GetStatOriValue(), pairText, true))
				{
					slot->SetVisibility(ESlateVisibility::Collapsed);
					continue;
				}

				slot->SetVisibility(ESlateVisibility::HitTestInvisible);
				slot->SetTextName(pairText.Key);
				slot->SetTextValue(pairText.Value);
				slot->SetColorAllText(FGsUIColorHelper::GetColorItemRefineOption(statData->GetStatOriValue(), 
					statRangeMap[statData->GetStatType()].Key, statRangeMap[statData->GetStatType()].Value));

				lastSlotIndex = i;
				continue;
			}
		}
		
		// 예외 상황에선 감춤
		slot->SetVisibility(ESlateVisibility::Collapsed);
	}

	// BonusStat. 남은 슬롯에 찍음
	bool isSetBonusData = false;
	TPair<StatType, int32> bonusStatData; // statType , value
	data->GetEquipItemRefineBonusStatInfo(bonusStatData);

	for (int32 i = lastSlotIndex + 1; i < _refineSlotList.Num(); ++i)
	{
		UGsUIItemStatListEntry* slot = _refineSlotList[i];
		if (nullptr == slot)
		{
			continue;
		}

		if (false == isSetBonusData)
		{
			StatType type = bonusStatData.Key;
			int32 statValue = bonusStatData.Value; // 만분율

			FText statName;
			FText valueText;
			FGsStatHelper::GetDisPlayStatName(type, statName);
			FGsStatHelper::GetDisPlayStatValueWithMarkFormat(type, statValue,valueText);

			// 만분율 값을 퍼센트로 변환하므로 value / 10000 * 100 = value * 0.01
			//FString strBonusValue = FString::Format(TEXT("{0}%"), {  FMath::FloorToInt(statValue * 0.01f) });

			slot->SetVisibility(ESlateVisibility::HitTestInvisible);
			slot->SetTextName(statName);
			//slot->SetTextValue(FText::FromString(strBonusValue));
			slot->SetTextValue(valueText);

			isSetBonusData = true;
			continue;
		}

		// 예외 상황에선 감춤
		slot->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIItemDetailInfo::SetSkillOptionData(const FGsItemEquipment* InEquipItem)
{
	if (nullptr == InEquipItem)
	{
		_skillOptionRootPanel->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	FGsItemEquipment* equipData = const_cast<FGsItemEquipment*>(InEquipItem);
	if (false == equipData->IsExistSkillOption())
	{
		_skillOptionRootPanel->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	_skillOptionRootPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	TArray<const FGsSchemaPassivitySet*> optionDataList = equipData->GetSkilOptionDataByLevel();
	bool isActiveOption = false;
	for (int32 i = 0; i < _skillOptionSlotList.Num(); ++i)
	{
		UGsUIItemSkillOptionListEntry* slot = _skillOptionSlotList[i];
		if (nullptr == slot)
		{
			continue;
		}
		
		if (optionDataList.IsValidIndex(i))
		{
			const FGsSchemaPassivitySet* skillData = optionDataList[i];
			if (nullptr != skillData)
			{
				slot->SetOptionText(skillData);
				slot->SetVisibility(ESlateVisibility::Visible);
				isActiveOption = true;
			}
			else
			{
				slot->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			slot->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (false == isActiveOption)
	{
		_skillOptionRootPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIItemDetailInfo::SetUIMagicalForge(const FGsItem* InItem)
{
	if (false == IsBranch(EGsGameClientBranchType::MAGICAL_FORGE))
	{
		_panelMagicalForge->SetVisibility(ESlateVisibility::Collapsed);
		_panelMagicalForgeSupp->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	bool isShowMagicalForge = false;
	do 
	{
		if (nullptr == InItem)
			break;

		// 장비 아이템
		if (InItem->IsEquipment())
		{
			const FGsItemEquipment* equipData = static_cast<const FGsItemEquipment*>(InItem);
			if (nullptr == equipData)
				break;

			// 개화된 아이템인가?
			if (false == equipData->GetMagicalForgeData().IsMagicalForge())
				break;

			// 효과 정보가 존재하는가?
			ItemGrade effectGrade = ItemGrade::NONE;
			const FGsSchemaPassivitySet* passivityData = FGsItemHelper::GetMagicalForgePassivitySetAndGrade(equipData->GetMagicalForgeData(), effectGrade);
			if (nullptr == passivityData)
				break;

			// 개화 정보를 표시할 수 있다.
			isShowMagicalForge = true;
			_panelMagicalForge->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			RegisterHyperLinkDelegateMagicalForge(*passivityData, effectGrade, _rbMagicalForge);
		}
		// 재료 아이템
		else if (InItem->IsIngredient())
		{
			const FGsItemIngredient* ingredientData = static_cast<const FGsItemIngredient*>(InItem);
			if (nullptr == ingredientData)
				break;

			// 개화 효과를 가진 아이템인가?
			if (ItemEffectType::MAGICAL_FORGE != ingredientData->GetEffectType())
				break;

			MagicalForgeId effectId = static_cast<MagicalForgeId>(ingredientData->FindFirstEffectId());

			// 효과 정보가 존재하는가?
			if (0 == effectId)
				break;

			// 효과 정보가 존재하는가? - 1레벨 고정 표시
			MagicalForgeData magicalForgeData(effectId);
			magicalForgeData.mLevel = 1;
			ItemGrade effectGrade = ItemGrade::NONE;
			const FGsSchemaPassivitySet* passivityData = FGsItemHelper::GetMagicalForgePassivitySetAndGrade(magicalForgeData, effectGrade);
			if (nullptr == passivityData)
				break;

			isShowMagicalForge = true;
			_panelMagicalForgeSupp->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			RegisterHyperLinkDelegateMagicalForge(*passivityData, effectGrade, _rbMagicalForgeSupp);
		}

	} while (0);

	if (false == isShowMagicalForge)
	{
		_panelMagicalForge->SetVisibility(ESlateVisibility::Collapsed);
		_panelMagicalForgeSupp->SetVisibility(ESlateVisibility::Collapsed);
		UnRegisterHyperLinkDelegateMagicalForge(_rbMagicalForge);
		UnRegisterHyperLinkDelegateMagicalForge(_rbMagicalForgeSupp);
	}
}

void UGsUIItemDetailInfo::SetUIEnchant(const FGsItemEquipment* InEquipItem)
{
	if (nullptr == InEquipItem)
	{
		// 장비가 아닐 경우 표시하지 않는다.
		_panelEnchant->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if (InEquipItem->IsCollectableType())
		{
			// 장비지만 강화할 수 없는 아이템
			_panelEnchant->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			// 장비지만 강화할 수 있는 아이템
			_panelEnchant->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			_displayedEnchantLevel = InEquipItem->GetLevel();
			_defaultEnchantLevel = InEquipItem->GetLevel();
		}
	}
}

void UGsUIItemDetailInfo::SetUIMaterial(const FGsItemEquipment* InEquipItem)
{
	if (nullptr == InEquipItem)
	{
		_tbMateraial->SetText(FText::GetEmpty());
		return;
	}

	const FGsSchemaItemMaterialData* tableMaterial =
		UGsTableUtil::FindRowById<UGsTableItemMaterialData, FGsSchemaItemMaterialData>(
			static_cast<int32>(InEquipItem->GetMaterialType()));

	if (nullptr == tableMaterial)
	{
		_tbMateraial->SetText(FText::GetEmpty());
		return;
	}

	_tbMateraial->SetText(tableMaterial->name);
}

void UGsUIItemDetailInfo::SetUIEquipWeight(const FGsItemEquipment* InEquipItem)
{
	if (nullptr == InEquipItem)
	{
		_tbWeight->SetText(FText::AsNumber(0));

		return;
	}

	SetUIWeightInter(InEquipItem->GetWeightPer(), _tbWeight);
}

void UGsUIItemDetailInfo::SetUISetStatInfo(const FGsItemEquipment* InEquipItem)
{
	uint8 setType = static_cast<uint8>(UGsItemManager::SetNumType::Set2);

	FText textFormat;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Equip_Set"), textFormat);

	// 세트 스탯정보
	FGsItemEquipment* data = const_cast<FGsItemEquipment*>(InEquipItem);

	// Set 갯수
	const uint8 SetItemCount = data->GetAllSetCount();

	for (int32 i = 0; i < _setStatSlotList.Num(); ++i)
	{
		UGsUIItemSetStatListEntry* slot = _setStatSlotList[i];

		if (slot)
		{
			if (SetItemCount <= i)
			{
				slot->SetVisibility(ESlateVisibility::Collapsed);
				continue;
			}

			const TArray<FGsItemStatData>* statDataList = nullptr;
			bool bIsOpen = data->GetSetItemStat(static_cast<UGsItemManager::SetNumType>(setType), statDataList);

			// 주의: UGsItemManager::SetNumType::Set2 의 값이 1이므로 1을 더해준다
			FText textTitle = FText::Format(textFormat, setType + 1);
			slot->SetTextTitle(textTitle);
			slot->SetStatData(statDataList);
			slot->SetOnOff(bIsOpen);

			++setType;
		}
	}
}

void UGsUIItemDetailInfo::SetUISetItemInfo(const FGsItemEquipment* InEquipItem)
{
	FGsItemEquipment* data = const_cast<FGsItemEquipment*>(InEquipItem);
	if (nullptr == data)
	{
		GSLOG(Error, TEXT("nullptr == data"));
		return;
	}
	TArray<TPair<bool, FText>> setPairDataList = data->GetItemNameTextAndOpenState();

	for (int32 i = 0; i < _setItemSlotList.Num(); ++i)
	{
		if (UGsUIItemSetItemListEntry* slot = _setItemSlotList[i])
		{
			if (setPairDataList.IsValidIndex(i))
			{
				TPair<bool, FText>& setPairData = setPairDataList[i];
				
				slot->SetOnOff(setPairData.Key);
				slot->SetTextName(setPairData.Value);
				slot->SetVisibility(ESlateVisibility::HitTestInvisible);

				continue;
			}

			slot->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UGsUIItemDetailInfo::SetUISuppliesInfo(const FGsItem* InItem)
{
	SetUIUtilize(FText::GetEmpty());
	SetUIDesc(InItem);	
	SetUIPeriod(InItem);

	// 개화 정보 표시
	SetUIMagicalForge(InItem);

	if (nullptr == InItem)
	{	
		SetUIAmount(INVALID_CURRENCY_AMOUNT);
		SetUIContents(FText::GetEmpty());
		_tbSuppWeight->SetText(FText::GetEmpty());
		_panelSuppWeight->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}	

	// 효과 표시
	SetUIEff(InItem);

	// 내용물 표시 (ConsumableBox 및 코스튬/페어리 소환권만 해당)
	// 먼저, ConsumableBox 아이템에 대하여 텍스트 대신 아이콘을 보여준다. 나중에는 TypeList 를 쓰는 게 좋을듯
	ItemCategorySub subCategory = InItem->GetSubCategory();
	if (subCategory == ItemCategorySub::REWARD_BOX)
	{
		SetUIContentsIcon(InItem);
	}
	else if (subCategory == ItemCategorySub::SUMMON_COSTUME ||
			 subCategory == ItemCategorySub::SUMMON_FAIRY)
	{
		SetUIContentsPopupLink(InItem);
		_displayedRewardId = INVALID_REWARD_ID;
	}
	else
	{
		SetUIContents(InItem->GetUIContentsText());
		_displayedRewardId = INVALID_REWARD_ID;
	}

	// 무게 표시
	SetUIWeightInter(InItem->GetWeightPer(), _tbSuppWeight);
	_panelSuppWeight->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 보유량 표시
	SetUIAmount(GItem()->GetAllItemCountByTID(InItem->GetTID()));
}

void UGsUIItemDetailInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (_isRunningExpiredTimeFlag)
	{
		if (false == _displayedItem.IsValid())
			return;

		SetUIPeriod(_displayedItem.Get());
	}

	Super::NativeTick(MyGeometry,InDeltaTime);
}

void UGsUIItemDetailInfo::SetUIConsumableInfo(const FGsItem* InItem)
{
	if (nullptr == InItem)
	{
		_panelSuppDurationRoot->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	FText durationText = FGsItemHelper::GetDurationText(InItem);
	// 지속시간 표시
	if (durationText.IsEmpty())
	{
		_panelSuppDurationRoot->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_panelSuppDurationRoot->SetVisibility(ESlateVisibility::HitTestInvisible);
		_tbSuppDuration->SetText(durationText);
	}
}

void UGsUIItemDetailInfo::SetUIEff(const FGsItem* InItem)
{
	if (nullptr == InItem)
	{
		_panelSuppEff->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		FText effectText;
		if (InItem->GetType() == ItemType::CONSUME)
		{
			effectText = FGsItemHelper::GetSupplyEffectText(InItem);
		}
		else
		{
			effectText = InItem->GetToolTipDesc();
		}

		if (effectText.IsEmpty())
		{
			_panelSuppEff->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			_panelSuppEff->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_tbSuppEff->SetText(effectText);
		}
	}
}

void UGsUIItemDetailInfo::ClearUIContents()
{
	_suppVerticalBox->ClearChildren();
	_rbSummonInfo->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIItemDetailInfo::SetUIContents(const FText& InTxt)
{
	ClearUIContents();

	if (InTxt.IsEmpty())
	{
		_panelSuppContents->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_panelSuppContents->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_tbSuppContents->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_tbSuppContents->SetText(InTxt);
	}
}

void UGsUIItemDetailInfo::SetUIContentsIcon(const FGsItem* InItem)
{
	RewardId rewardId = InItem->GetRewardId();
	if (_displayedRewardId != INVALID_REWARD_ID && _displayedRewardId == rewardId)
	{
		return;
	}
	_displayedRewardId = rewardId;

	// UVerticalBox : _suppVerticalBox 에 각 RewardBox 별로 파싱한 정보를 텍스트/TileView 쌍으로 넣을 것이다.
	// _tileViewForAdjustment 는 단순히 리스트 항목에 대한 정보를 받는 역할을 할 뿐이다.
	ClearUIContents();

	TArray<TPair<FString, TArray<FGsCurrencyItemIconData>>> descIconsPairList = 
		FGsItemConsumableBox::GetRewardDescItemsPairList(rewardId);

	if (descIconsPairList.Num() == 0)
	{
		_panelSuppContents->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	_panelSuppContents->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	// 이제 내용물에서, '공통 설명' 항목은 없을 것이다
	_tbSuppContents->SetVisibility(ESlateVisibility::Collapsed);

	for (int32 i = 0; i < descIconsPairList.Num(); ++i)
	{
		// 텍스트 블록
		URichTextBlock* descTextBlock = DuplicateObject(_tbSuppContents, _tbSuppContents->GetOuter(), MakeUniqueObjectName(this, UTextBlock::StaticClass(), FName(FString(TEXT("_tbSuppContents")))));

		descTextBlock->SetText(FText::FromString(descIconsPairList[i].Key));
		descTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
		_suppVerticalBox->AddChild(descTextBlock);

		// RANDOM_GROUP 에 대한 기획이 변경되어, 더는 동적으로 위젯을 생성하지 않아도 된다.
		//UTileView* iconTileView = nullptr;
		//USizeBox* sizeBoxForMaxHeight = nullptr;
		//if (descIconsPairList.Num() == 1)
		//{
		//	iconTileView = _tileViewForAdjustment;
		//	sizeBoxForMaxHeight = _sizeBoxForTileViewForMaxHeight;
		//}
		//else
		//{
		//	iconTileView = DuplicateObject(_tileViewForAdjustment, _tileViewForAdjustment->GetOuter(), MakeUniqueObjectName(this, UTileView::StaticClass(), FName(FString(TEXT("_tileViewForAdjustment")))));
		//	sizeBoxForMaxHeight = DuplicateObject(_sizeBoxForTileViewForMaxHeight, _sizeBoxForTileViewForMaxHeight->GetOuter(), MakeUniqueObjectName(this, USizeBox::StaticClass(), FName(FString(TEXT("_sizeBoxForTileViewForMaxHeight")))));
		//}
		//sizeBoxForMaxHeight->AddChild(iconTileView);

		TArray<UGsUIItemSupplyIconItem*> itemDataList;
		for (const FGsCurrencyItemIconData& iconData : descIconsPairList[i].Value)
		{
			UGsUIItemSupplyIconItem* itemData = NewObject<UGsUIItemSupplyIconItem>(this);
			itemData->SetParameters({
				iconData._costType,
				iconData._currencyType,
				iconData._itemId,
				iconData._itemLevel,
				iconData._amountMin,
				iconData._amountMax
				});
			itemDataList.Emplace(MoveTemp(itemData));
		}
		_tileViewForAdjustment->SetListItems(itemDataList);
		_tileViewForAdjustment->SetVisibility(ESlateVisibility::Visible);

		_suppVerticalBox->AddChild(_sizeBoxForTileViewForMaxHeight);
		_tileViewForAdjustment->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIItemDetailInfo::SetUIContentsPopupLink(const FGsItem* InItem)
{
	ClearUIContents();
	_panelSuppContents->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_tbSuppContents->SetVisibility(ESlateVisibility::Collapsed);
	_rbSummonInfo->SetVisibility(ESlateVisibility::Visible);
	_rbSummonInfo->SetText(InItem->MakeSummonPopupHyperLink());
}

void UGsUIItemDetailInfo::SetUIDesc(const FText& InTxt)
{	
	_panelSuppDescList->SetVisibility(ESlateVisibility::Collapsed);

	if (InTxt.IsEmpty())
	{
		_panelSuppDesc->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_panelSuppDesc->SetVisibility(ESlateVisibility::HitTestInvisible);
		_tbSuppDesc->SetText(InTxt);
	}
}

void UGsUIItemDetailInfo::SetUIDesc(const FGsItem* InItem)
{
	if (nullptr == InItem)
	{
		GSLOG(Error, TEXT("nullptr == InItem"));
		return;
	}

	_panelSuppDescList->SetVisibility(ESlateVisibility::Collapsed);
	_panelSuppDesc->SetVisibility(ESlateVisibility::Collapsed);

	// 스킬북일 경우 설명 표시
	if (InItem->GetMainCategory() == ItemCategoryMain::SKILLBOOK)
	{
		FText descText = InItem->GetToolTipDesc();
		if (false == descText.IsEmpty())
		{
			_panelSuppDesc->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_tbSuppDesc->SetText(descText);
		}

		FGsItem* itemNoConst = const_cast<FGsItem*>(InItem);
		if (const FGsItemConsumable* itemConsumable = itemNoConst->GetCastItem<FGsItemConsumable>())
		{
			_skillBookUserSkillSetIdList.Empty();
			if (itemConsumable->GetUserSkillSetIdList(_skillBookUserSkillSetIdList))
			{
				_panelSuppDescList->SetVisibility(ESlateVisibility::HitTestInvisible);
				_slotHelperSuppDesc->RefreshAll(_skillBookUserSkillSetIdList.Num());
			}
		}
	}
	else if (InItem->IsConsumable() && !InItem->GetToolTipDesc().IsEmpty())
	{
		_panelSuppDesc->SetVisibility(ESlateVisibility::HitTestInvisible);
		_tbSuppDesc->SetText(InItem->GetToolTipDesc());
	}
}

void UGsUIItemDetailInfo::SetUIAmount(const int64 InAmount)
{
	if (0 > InAmount)
	{
		_panelSuppAmount->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_panelSuppAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FText Txt;
		FGsUIStringHelper::GetAmountText(InAmount, MAX_uint64, Txt);
		_tbSuppAmount->SetText(Txt);
	}
}

void UGsUIItemDetailInfo::SetUICurrencyInfo(const CurrencyType InCurrencyType, const int64 InAmount)
{
	const FGsSchemaCurrencyData* CurrencyData = FGsCurrencyHelper::GetCurrencyData(InCurrencyType);
	if (nullptr == CurrencyData)
	{
		SetUIDesc(FText::GetEmpty());
		SetUIAmount(INVALID_CURRENCY_AMOUNT);
		SetUIAcquire(FText::GetEmpty());
		SetUIUtilize(FText::GetEmpty());
	}
	else
	{
		SetUIDesc(CurrencyData->desc);
		SetUIAmount(FMath::Max<int64>(FGsCurrencyHelper::GetCurrencyAmount(InCurrencyType), 0));
		SetUIAcquire(FGsCurrencyHelper::MakeSupplyHyperLink(InCurrencyType));
		SetUIUtilize(FGsCurrencyHelper::MakeUtilizeHyperLink(InCurrencyType));
	}

	SetUIEff(nullptr);
}

void UGsUIItemDetailInfo::SetUIWeightInter(float InValue, OUT UTextBlock* OutTextBlock)
{
	// weight 
	if (0.f >= InValue)
	{
		OutTextBlock->SetText(FText::AsNumber(0));
	}
	else
	{
		FString resultPerStr = FString::Printf(TEXT("%.2f"), InValue);
		OutTextBlock->SetText(FText::FromString(resultPerStr));
	}
}

void UGsUIItemDetailInfo::SetUIAcquire(const FText& InTxt)
{
	UPanelWidget* _panelTarget = nullptr;
	URichTextBlock* _rbTarget = nullptr;

	if (0 == _switcherItemType->GetActiveWidgetIndex())
	{
		// 장비
		_panelTarget = _panelAccquireRoot;
		_rbTarget = _rbAcquire;
	}
	else if (1 == _switcherItemType->GetActiveWidgetIndex())
	{
		// Supp
		_panelTarget = _panelSuppAcquireRoot;
		_rbTarget = _rbSuppAcquire;
	}

	if (nullptr != _panelTarget)
	{
		_panelTarget->SetVisibility(InTxt.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}

	if (nullptr != _rbTarget)
	{
		_rbTarget->SetText(InTxt);
	}
}

void UGsUIItemDetailInfo::SetUIUtilize(const FText& InTxt)
{
	UPanelWidget* _panelTarget = nullptr;
	URichTextBlock* _rbTarget = nullptr;

	if (0 == _switcherItemType->GetActiveWidgetIndex())
	{
		// 장비
		_panelTarget = _panelUtilizeRoot;
		_rbTarget = _rbUtilize;
	}
	else if (1 == _switcherItemType->GetActiveWidgetIndex())
	{
		// Supp
		_panelTarget = _panelSuppUtilizeRoot;
		_rbTarget = _rbSuppUtilize;
	}

	if (nullptr != _panelTarget)
	{
		_panelTarget->SetVisibility(InTxt.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}

	if (nullptr != _rbTarget)
	{
		_rbTarget->SetText(InTxt);
	}
}

void UGsUIItemDetailInfo::SetUIPeriod(const FGsItem* InItem)
{
	UTextBlock* textBlockPeriod = _suppPeriodText;
	UHorizontalBox* panelPeriod = _suppPeriodRootPanel;

	FText paramText;

	FDateTime expiredTime;
	FDateTime serverDataTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();

	ExpiredTimeType expiredTimeType = ExpiredTimeType::HIDE;
	do
	{
		if (nullptr == InItem)
			break;

		if (InItem->IsOtherUserItem())
			break;

		const FGsSchemaItemExpiredTime* expiredTimeData = InItem->GetExpriedTimeData();
		if (nullptr == expiredTimeData)
			break;

		if (InItem->IsEquipment())
		{
			textBlockPeriod = _periodText;
			panelPeriod = _periodRootPanel;
		}

		// 고정 유효 기간 아이템
		if (false == expiredTimeData->endTime.IsEmpty() && false == expiredTimeData->startTime.IsEmpty())
		{
			if (false == FDateTime::Parse(expiredTimeData->endTime, expiredTime))
				break;

			// 이미 만료되었는가?
			if (expiredTime <= serverDataTime)
			{
				expiredTimeType = ExpiredTimeType::EXPIRED;
				break;
			}

			FDateTime expiredClientDataTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(expiredTime);
			FString strTime;
			FGsTimeStringHelper::GetTimeStringNYMDHMS(expiredClientDataTime, strTime);
			paramText = FText::FromString(strTime);

			expiredTimeType = expiredTimeData->isAllowToDelete ? ExpiredTimeType::EXPIRED_DELETE : ExpiredTimeType::EXPIRED_NOT_DELETE;
			break;
		}

		// 수령 후 만료형 아이템
		if (0 < expiredTimeData->expiredTimeM)
		{
			// 수령하기 전인가?
			if (0 >= InItem->GetExpiredTime())
			{
				FTimespan remainTime(expiredTimeData->expiredTimeM * ETimespan::TicksPerMinute);
				FString strRemainTime;
				FGsTimeStringHelper::GetTimeStringFloor(remainTime, strRemainTime);
				paramText = FText::FromString(strRemainTime);

				expiredTimeType = expiredTimeData->isAllowToDelete ? ExpiredTimeType::RECEIVE_EXPIRED_DELETE : ExpiredTimeType::RECEIVE_EXPIRED_NOT_DELETE;
				break;
			}

			expiredTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(InItem->GetExpiredTime());
			if (expiredTime <= serverDataTime)
			{
				expiredTimeType = ExpiredTimeType::EXPIRED;
				break;
			}

			FTimespan remainTime = expiredTime - serverDataTime;
			// 1일 이상
			if (1 <= remainTime.GetDays())
			{
				FDateTime expiredClientDataTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(expiredTime);
				FString strTime;
				FGsTimeStringHelper::GetTimeStringNYMDHMS(expiredClientDataTime, strTime);
				paramText = FText::FromString(strTime);

				expiredTimeType = expiredTimeData->isAllowToDelete ? ExpiredTimeType::EXPIRED_DELETE : ExpiredTimeType::EXPIRED_NOT_DELETE;
			}
			// 1일 미만
			else
			{
				FString strRemainTime;
				FGsTimeStringHelper::GetTimeStringFloor(remainTime, strRemainTime);
				paramText = FText::FromString(strRemainTime);

				expiredTimeType = expiredTimeData->isAllowToDelete ? ExpiredTimeType::UNDER_DAY_DELETE : ExpiredTimeType::UNDER_DAY_NOT_DELETE;
			}
			break;
		}

	} while (0);

	switch (expiredTimeType)
	{
	case UGsUIItemDetailInfo::ExpiredTimeType::HIDE:
	{
		_isRunningExpiredTimeFlag = false;
		panelPeriod->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
	case UGsUIItemDetailInfo::ExpiredTimeType::EXPIRED:
	{
		_isRunningExpiredTimeFlag = false;
		panelPeriod->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UGsUIUtil::SetTextBlock(textBlockPeriod, TEXT("ItemUIText"), TEXT("ItemUi_Expired_End_03"));
		break;
	}
	case UGsUIItemDetailInfo::ExpiredTimeType::EXPIRED_DELETE:
	{
		_isRunningExpiredTimeFlag = true;
		panelPeriod->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UGsUIUtil::SetTextBlockOneParam(textBlockPeriod, TEXT("ItemUIText"), TEXT("ItemUi_Notice_Expired_Delete"), paramText);
		break;
	}
	case UGsUIItemDetailInfo::ExpiredTimeType::EXPIRED_NOT_DELETE:
	{
		_isRunningExpiredTimeFlag = true;
		panelPeriod->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UGsUIUtil::SetTextBlockOneParam(textBlockPeriod, TEXT("ItemUIText"), TEXT("ItemUi_Notice_Expired_Exceed"), paramText);
		break;
	}
	case UGsUIItemDetailInfo::ExpiredTimeType::RECEIVE_EXPIRED_DELETE:
	{
		_isRunningExpiredTimeFlag = true;
		panelPeriod->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UGsUIUtil::SetTextBlockOneParam(textBlockPeriod, TEXT("ItemUIText"), TEXT("UI_Text_NotCalculatedExpiredTime_isAllowedToDeleteTrue"), paramText);
		break;
	}
	case UGsUIItemDetailInfo::ExpiredTimeType::RECEIVE_EXPIRED_NOT_DELETE:
	{
		_isRunningExpiredTimeFlag = true;
		panelPeriod->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UGsUIUtil::SetTextBlockOneParam(textBlockPeriod, TEXT("ItemUIText"), TEXT("UI_Text_NotCalculatedExpiredTime_isAllowedToDeleteFALSE"), paramText);
		break;
	}
	case UGsUIItemDetailInfo::ExpiredTimeType::UNDER_DAY_DELETE:
	{
		_isRunningExpiredTimeFlag = true;
		panelPeriod->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UGsUIUtil::SetTextBlockOneParam(textBlockPeriod, TEXT("ItemUIText"), TEXT("ItemUi_Notice_Expired_UnderDayDelete"), paramText);
		break;
	}
	case UGsUIItemDetailInfo::ExpiredTimeType::UNDER_DAY_NOT_DELETE:
	{
		_isRunningExpiredTimeFlag = true;
		panelPeriod->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UGsUIUtil::SetTextBlockOneParam(textBlockPeriod, TEXT("ItemUIText"), TEXT("ItemUi_Notice_Expired_UnderDayExceed"), paramText);
		break;
	}
	default:
		break;
	}
}

void UGsUIItemDetailInfo::OnSelectEquipTab(int32 InIndex)
{
	_switcherEquipPage->SetActiveWidgetIndex(InIndex);
}

void UGsUIItemDetailInfo::RegisterHyperLinkDelegateMagicalForge(const FGsSchemaPassivitySet& InPassivitySet, ItemGrade InGrade, URichTextBlock* InTextBlock)
{
	URichTextBlockDecorator* Decorator = InTextBlock->GetDecoratorByClass(UGsRichTextBlockLinkDecorator::StaticClass());
	if (nullptr == Decorator)
	{
		GSLOG(Error, TEXT("nullptr == Decorator"));
		return;
	}

	UGsRichTextBlockLinkDecorator* GsDecorator = Cast<UGsRichTextBlockLinkDecorator>(Decorator);
	if (nullptr == GsDecorator)
	{
		GSLOG(Error, TEXT("nullptr == GsDecorator"));
		return;
	}

	TMap<FString, FString> ParamMap = {
		{TEXT("Title"), InPassivitySet.nameText.ToString()},
		{TEXT("Desc"), InPassivitySet.descText.ToString()}
	};

	FText gradeText = FText::FromString(ItemGradeEnumToString(InGrade));
	FText linkText = UGsRichTextBlockLinkDecorator::MakeCustomLinkText(gradeText, TEXT("MagicalForge"), InPassivitySet.nameText, ParamMap);

	InTextBlock->SetText(linkText);

	GsDecorator->AddUniqueOnClickDelegateId(GetUniqueID(), UGsRichTextBlockLinkDecorator::FGsOnClickRichTextBlockLinkDecoratorId::CreateUObject(this, &UGsUIItemDetailInfo::OnClickHyperCustomLinkId));
}

void UGsUIItemDetailInfo::UnRegisterHyperLinkDelegateMagicalForge(URichTextBlock* InTextBlock)
{
	URichTextBlockDecorator* Decorator = InTextBlock->GetDecoratorByClass(UGsRichTextBlockLinkDecorator::StaticClass());
	if (nullptr == Decorator)
	{
		GSLOG(Error, TEXT("nullptr == Decorator"));
		return;
	}

	UGsRichTextBlockLinkDecorator* GsDecorator = Cast<UGsRichTextBlockLinkDecorator>(Decorator);
	if (nullptr == GsDecorator)
	{
		GSLOG(Error, TEXT("nullptr == GsDecorator"));
		return;
	}

	GsDecorator->RemoveOnClickDelegate(GetUniqueID());
}

void UGsUIItemDetailInfo::InvalidateEnchantPanel()
{
	if (false == _displayedItem.IsValid())
		return;

	const FGsItemEquipment* itemEquipment = _displayedItem->GetCastItem<FGsItemEquipment>();
	if (nullptr == itemEquipment)
		return;

	bool isEnableMinus = _displayedItem->GetMinLevel() < _displayedEnchantLevel && (_displayedEnchantLevel == 1 || itemEquipment->IsExistEnchantLevel(_displayedEnchantLevel - 1));
	bool isEnablePlus = _displayedItem->GetMaxLevel() > _displayedEnchantLevel && itemEquipment->IsExistEnchantLevel(_displayedEnchantLevel + 1);

	_btnEnchantMinus->SetIsEnabled(isEnableMinus);
	_btnEnchantPlus->SetIsEnabled(isEnablePlus);

	EGsUIColorType colorType = (_displayedEnchantLevel == _defaultEnchantLevel) ? EGsUIColorType::DEFAULT_VALID :
		(_displayedEnchantLevel > _defaultEnchantLevel) ? EGsUIColorType::UP_COMPARE_STAT_COLOR : EGsUIColorType::DOWN_COMPARE_STAT_COLOR;
	_textBlockEnchant->SetText(FText::AsNumber(_displayedEnchantLevel));
	_textBlockEnchant->SetColorAndOpacity(FGsUIColorHelper::GetColor(colorType));

	FGsItemEquipment enchantedItem(_displayedItem->GetTableData());
	enchantedItem.UpdateBy(itemEquipment);
	enchantedItem.UpdateStat(_displayedEnchantLevel);
	enchantedItem.SetLevel(_displayedEnchantLevel);

	if (_isCompareMode)
	{
		if (false == _compareItem.IsValid())
			return;

		const FGsItemEquipment* compareItemEquipment = _compareItem->GetCastItem<FGsItemEquipment>();
		if (nullptr == itemEquipment)
			return;

		SetUIItemStat(&enchantedItem, compareItemEquipment);
	}
	else
	{
		if (_displayedEnchantLevel == _defaultEnchantLevel)
		{
			SetUIItemStat(itemEquipment);
		}
		else
		{
			SetUIItemStat(&enchantedItem, itemEquipment);
		}
	}
	
	SetUIEnchantProperty(&enchantedItem);
	SetSkillOptionData(&enchantedItem);
}

void UGsUIItemDetailInfo::OnRefreshEntryStatList(int32 InIndex, UWidget* InEntry)
{
	if (false == _displayedStatList.IsValidIndex(InIndex))
		return;

	UGsUIItemStatListEntry* entry = Cast<UGsUIItemStatListEntry>(InEntry);
	if (nullptr == entry)
		return;

	const StatDisplayInfo& statDisplayInfo = _displayedStatList[InIndex];
	entry->SetTextName(statDisplayInfo.statName);
	entry->SetTextValue(statDisplayInfo.statValue);
	entry->SetColorAllText(FGsUIColorHelper::GetColor(statDisplayInfo.resultType == StatCompareResultType::REMOVE ? EGsUIColorType::DISABLE_STAT_COLOR : EGsUIColorType::DEFAULT_STAT_COLOR));

	entry->SetCompareObjectVisibility(_compareVisibility);

	switch (statDisplayInfo.resultType)
	{
	case StatCompareResultType::UPDATE:
	{
		int32 deltaValue = statDisplayInfo.compareStatDeltaValue;
		
		if (0 == deltaValue)
		{
			entry->SetSameValueStat();
		}
		else
		{
			int32 absValue = fabs(deltaValue);
			FText resultValue;
			FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statDisplayInfo.statType, absValue, resultValue);

			entry->SetCompareArrowSwticher(0 <= deltaValue);
			entry->SetCompareValueText(resultValue, FGsUIColorHelper::GetColor(0 <= deltaValue ? EGsUIColorType::UP_COMPARE_STAT_COLOR : EGsUIColorType::DOWN_COMPARE_STAT_COLOR));
		}
	} break;
	case StatCompareResultType::ADD:
	{
		if (_compareVisibility)
		{
			entry->SetCompareExptectNewOrRemoveStatText(true, FGsUIColorHelper::GetColor(EGsUIColorType::UP_COMPARE_STAT_COLOR));
		}
	} break;
	case StatCompareResultType::REMOVE:
	{
		if (_compareVisibility)
		{
			entry->SetCompareExptectNewOrRemoveStatText(false, FGsUIColorHelper::GetColor(EGsUIColorType::DOWN_COMPARE_STAT_COLOR));
		}
	} break;
	default:
		break;
	}
	
}

void UGsUIItemDetailInfo::OnRefreshEntrySuppDesc(int32 InIndex, UWidget* InEntry)
{
	if (false == _skillBookUserSkillSetIdList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIItemDescListEntry* entry = Cast<UGsUIItemDescListEntry>(InEntry))
	{
		FText textSkillName;
		FText textSkillDesc;
		ItemGrade skillGrade = ItemGrade::NONE;
		if (FGsSkillHelper::GetSkillSimpleInfoByUserSkillSetId(_skillBookUserSkillSetIdList[InIndex], 
			textSkillName, textSkillDesc, skillGrade))
		{
			entry->SetTextTitle(textSkillName);
			entry->SetColorTitle(FGsUIColorHelper::GetColorItemGrade(skillGrade));
			entry->SetTextDesc(textSkillDesc);
		}
	}
}

void UGsUIItemDetailInfo::OnRefreshEntryCollectionMaterialLevel(int32 InIndex, UWidget* InEntry)
{
	if (false == _collectionMarkingDataList.IsValidIndex(InIndex))
	{
		return;
	}

	TWeakObjectPtr<UGsUIItemCollectionMaterialLevel> entry = Cast<UGsUIItemCollectionMaterialLevel>(InEntry);
	if (entry.IsValid())
	{
		TPair<Level, ECollectionMaterialActionType> data = _collectionMarkingDataList[InIndex];

		entry->SetData(data.Value, data.Key, 0 != InIndex);
	}
}

void UGsUIItemDetailInfo::OnClickHyperLinkId(const UGsRichTextBlockLinkDecorator::FGsOnClickParamId& InParamIdMap)
{
	// InParamIdMap 에 들어 있는 스킬 관련 설명 뽑아내기
	const FString* titleString = InParamIdMap.Value.Find(TEXT("Title"));
	if (nullptr == titleString)
	{
		GSLOG(Error, TEXT("nullptr == titleString"));
		return;
	}

	const FString* descString = InParamIdMap.Value.Find(TEXT("Desc"));
	if (nullptr == descString)
	{
		GSLOG(Error, TEXT("nullptr == descString"));
		return;
	}

	// 1. 메시지 방식 : 메시지를 보내는 방식은 ItemDetailInfo 가 여러 개 열려 있을 때, 해당 RichTextBlock 이
	// 속해 있는 팝업 창에서의 툴팁을 제대로 띄워주기 위해 UniqueId 와 스트링을 함께 보낸다.
	uint32 uniqueId = InParamIdMap.Key;

	FGsItemDetailSkillInfoMessage itemDetailSkillInfo(uniqueId, *titleString, *descString, false);

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetItemContentSkill().SendMessage(MessageContentItemSkill::DETAIL_OPEN_ITEM_SKILL, &itemDetailSkillInfo);
	}
}

void UGsUIItemDetailInfo::OnClickHyperCustomLinkId(const UGsRichTextBlockLinkDecorator::FGsOnClickParamId& InParamIdMap)
{
	// InParamIdMap 에 들어 있는 스킬 관련 설명 뽑아내기
	const FString* titleString = InParamIdMap.Value.Find(TEXT("Title"));
	if (nullptr == titleString)
	{
		GSLOG(Error, TEXT("nullptr == titleString"));
		return;
	}

	const FString* descString = InParamIdMap.Value.Find(TEXT("Desc"));
	if (nullptr == descString)
	{
		GSLOG(Error, TEXT("nullptr == descString"));
		return;
	}

	_tooltipWeaponSkillInfo->Close();
	_tooltipWeaponSkillInfoSupp->Close();

	ItemGrade itemGrade = ItemGrade::NORMAL;
	if (false == _displayedItem.IsValid())
		return;

	if (_displayedItem->IsEquipment())
	{
		const FGsItemEquipment* itemEquipment = _displayedItem->GetCastItem<FGsItemEquipment>();
		if (nullptr == itemEquipment)
			return;

		if (false == itemEquipment->GetMagicalForgeData().IsMagicalForge())
			return;

		const FGsSchemaPassivitySet* passivityData = FGsItemHelper::GetMagicalForgePassivitySetAndGrade(itemEquipment->GetMagicalForgeData(), itemGrade);

		_tooltipWeaponSkillInfo->SetData(Forward<FText>(FText::FromString(*titleString)), Forward<FText>(FText::FromString(*descString)), false);
		_tooltipWeaponSkillInfo->SetTitleColor(FGsUIColorHelper::GetColorItemGrade(itemGrade));
		_tooltipWeaponSkillInfo->Open();
	}
	else if (_displayedItem->IsIngredient())
	{
		const FGsItemIngredient* itemIngredient = _displayedItem->GetCastItem<FGsItemIngredient>();
		if (nullptr == itemIngredient)
			return;

		MagicalForgeId effectId = static_cast<MagicalForgeId>(itemIngredient->FindFirstEffectId());
		if (0 == effectId)
			return;

		MagicalForgeData magicalForgeData(effectId);
		magicalForgeData.mLevel = 1;
		const FGsSchemaPassivitySet* passivityData = FGsItemHelper::GetMagicalForgePassivitySetAndGrade(magicalForgeData, itemGrade);

		_tooltipWeaponSkillInfoSupp->SetData(Forward<FText>(FText::FromString(*titleString)), Forward<FText>(FText::FromString(*descString)), false);
		_tooltipWeaponSkillInfoSupp->SetTitleColor(FGsUIColorHelper::GetColorItemGrade(itemGrade));
		_tooltipWeaponSkillInfoSupp->Open();
	}
}

void UGsUIItemDetailInfo::OnClickEnchantMinus()
{
	--_displayedEnchantLevel;

	InvalidateEnchantPanel();

	OnChangeEnchantCount.ExecuteIfBound(_displayedEnchantLevel);
}

void UGsUIItemDetailInfo::OnClickEnchantPlus()
{
	++_displayedEnchantLevel;

	InvalidateEnchantPanel();

	OnChangeEnchantCount.ExecuteIfBound(_displayedEnchantLevel);
}
