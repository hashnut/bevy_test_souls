#include "GsUIMaterialSetGainInfoCraftTargetListEntry.h"

#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "Currency/GsCostPackage.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Item/Collection/GsItemCollectionCondition.h"
#include "Item/Craft/GsCraftTarget.h"
#include "Item/GsItemManager.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemCollectionSlot.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"

void UGsUIMaterialSetGainInfoCraftTargetListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _targetSlotHelper)
	{
		_targetSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_targetSlotHelper->Initialize(_targetIconSelector);
		_targetSlotHelper->OnRefreshIcon.AddUniqueDynamic(this, &UGsUIMaterialSetGainInfoCraftTargetListEntry::OnRefreshIconTarget);
	}

	if (nullptr == _materialSlotHelper)
	{
		_materialSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_materialSlotHelper->Initialize(_materialIconSelector);
		_materialSlotHelper->OnRefreshIcon.AddUniqueDynamic(this, &UGsUIMaterialSetGainInfoCraftTargetListEntry::OnRefreshIconMaterial);
	}

	_btnCraft->OnClicked.AddUniqueDynamic(this, &UGsUIMaterialSetGainInfoCraftTargetListEntry::OnClickedCraft);
}

void UGsUIMaterialSetGainInfoCraftTargetListEntry::NativeDestruct()
{
	_targetIconSelector->RemoveAllChildren();
	_materialIconSelector->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsUIMaterialSetGainInfoCraftTargetListEntry::SetParameters(TWeakPtr<Parameters> InParamerters)
{
	constexpr int32 DETAIL_INFO_INDEX_MATERIALS = 0;
	constexpr int32 DETAIL_INFO_INDEX_COLLECTED = 1;
	constexpr int32 DETAIL_INFO_INDEX_NO_CRAFT = 2;

	_parameters = MoveTemp(InParamerters);

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const FGsItemCollectionCondition* ItemCollectionCondition = _parameters.IsValid() ? _parameters.Pin()->_collectionCondition : nullptr;
	if (nullptr == ItemCollectionCondition)
	{
		_craftId = INVALID_CRAFT_ID;
		_itemId = INVALID_ITEM_ID;
		_itemLevel = INVALID_LEVEL;
		_collectionCompleted = false;
	}
	else
	{
		const TPair<ItemId, Level>& EntryMaterialInfo = ItemCollectionCondition->EntryMaterialInfo();
		_itemId = EntryMaterialInfo.Key;
		_itemLevel = EntryMaterialInfo.Value;
		_craftId = CraftManager->FindAvaialbleCraftId(_itemId);
		_collectionCompleted = ItemCollectionCondition->ConditionCount() <= ItemCollectionCondition->RegisterCount();
	}

	// 대상 아이콘
	_targetSlotHelper->RefreshAll(1);

	int32 DetailInfoIndex = DETAIL_INFO_INDEX_NO_CRAFT;
	const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(_craftId);
	if (_collectionCompleted)
	{
		// 컬렉션 완료 상태
		DetailInfoIndex = DETAIL_INFO_INDEX_COLLECTED;
	}
	else if (nullptr == CraftTarget || !CraftTarget->CanDisplay())
	{
		// 제작 불가 상태
		DetailInfoIndex = DETAIL_INFO_INDEX_NO_CRAFT;
	}
	else
	{
		// 제작 가능 상태
		DetailInfoIndex = DETAIL_INFO_INDEX_MATERIALS;
		_materialSlotHelper->RefreshAll(CraftTarget->GetCostPackage()->GetCostElementsCount());
	}

	_detailInfoSwitcher->SetActiveWidgetIndex(DetailInfoIndex);

	const bool NavigateToCraft = (nullptr != CraftTarget) && CraftTarget->CanDisplay();
	_btnCraft->SetIsEnabled(NavigateToCraft);
}

void UGsUIMaterialSetGainInfoCraftTargetListEntry::OnRefreshIconTarget(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemCollectionSlot* Icon = Cast<UGsUIIconItemCollectionSlot>(InIcon);
	if (nullptr == Icon)
	{
		GSLOG(Error, TEXT("nullptr == Icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	const FGsItemCollectionCondition* ItemCollectionCondition = _parameters.IsValid() ? _parameters.Pin()->PopCollectionCondition() : nullptr;
	if (nullptr == ItemCollectionCondition)
	{
		GSLOG(Error, TEXT("nullptr == ItemCollectionCondition, InIndex:%d"), InIndex);
		return;
	}

	Icon->SetData(*ItemCollectionCondition);

	Icon->OnFinishedLongPressEvent.RemoveAll(this);
	Icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIMaterialSetGainInfoCraftTargetListEntry::OnFinishedLongPressEventItemIcon);

	// 대상 이름
	const FGsSchemaItemCommon* ItemCommon = UGsItemManager::GetItemTableDataByTID(_itemId);
	if (nullptr == ItemCommon)
	{
		GSLOG(Error, TEXT("nullptr == ItemCommon"));
	}
	else
	{
		const FText CollectionMaterialName = (1 < _itemLevel) ? FText::Format(NSLOCTEXT("GsCollection", "GsCollectionMaterialItemNameFormat", "+{0} {1}"), _itemLevel, ItemCommon->name) : ItemCommon->name;
		_txtTitle->SetText(CollectionMaterialName);
		_txtTitle->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGradeByItemId(_itemId));
	}
}

void UGsUIMaterialSetGainInfoCraftTargetListEntry::OnRefreshIconMaterial(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* Icon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == Icon)
	{
		GSLOG(Error, TEXT("nullptr == Icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(_craftId);
	if (nullptr == CraftTarget || !CraftTarget->CanDisplay())
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget || !CraftTarget->CanDisplay()"));
		return;
	}

	ItemId MaterialItemId = INVALID_ITEM_ID;
	TSharedRef<const FGsCostPackage> CostPackage = CraftTarget->GetCostPackage();
	if (!CostPackage->IsValidCostElementsIndex(InIndex))
	{
		GSLOG(Error, TEXT("!CostPackage->IsValidCostElementsIndex(%d)"), InIndex);
		return;
	}

	const FGsCostElement* CostElement = CostPackage->GetCostElementAt(InIndex);
	if (nullptr == CostElement)
	{
		GSLOG(Error, TEXT("nullptr == CostElement, InIndex:%d"), InIndex);
		return;
	}

	if (CostType::CURRENCY == CostElement->_costType)
	{
		// 작은 아이콘에는 테그 출력 필요 없으므로, 0개로 강제 설정
		Icon->SetCurrency(CostElement->GetCurrencyType(), 0);
	}
	else if (CostType::ITEM == CostElement->_costType)
	{
		MaterialItemId = CostElement->GetItemId();
		Icon->SetNotOwnItem(MaterialItemId, 1);
	}

	// 롱프레스 비활성화, 레드닷 제거, 이름 제거 등
	Icon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::NotUseDetailWndType);
	Icon->SetRedDot(false);
	Icon->SetItemNameVisibility(false);
	Icon->SetDimmedImage(!CostElement->PlayerHasEnough());
}

void UGsUIMaterialSetGainInfoCraftTargetListEntry::OnClickedCraft()
{
	const UGsSupplyManager* SupplyManager = GSSupply();
	if (nullptr == SupplyManager)
	{
		GSLOG(Error, TEXT("nullptr == SupplyManager"));
		return;
	}

	SupplyManager->TryToNavigateItemId(EGsUnlockContentsType::CRAFT, _itemId);
}

void UGsUIMaterialSetGainInfoCraftTargetListEntry::OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
