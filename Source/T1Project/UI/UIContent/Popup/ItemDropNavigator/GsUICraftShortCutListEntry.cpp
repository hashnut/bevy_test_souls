#include "GsUICraftShortCutListEntry.h"

#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "Currency/GsCostPackage.h"
#include "Item/Craft/GsCraftTarget.h"
#include "Item/Craft/GsCraftTargetObject.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUICraftShortCutListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _targetSlotHelper)
	{
		_targetSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_targetSlotHelper->Initialize(_targetIconSelector);
		_targetSlotHelper->OnRefreshIcon.AddUniqueDynamic(this, &UGsUICraftShortCutListEntry::OnRefreshIconTarget);
	}

	if (nullptr == _materialSlotHelper)
	{
		_materialSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_materialSlotHelper->Initialize(_materialIconSelector);
		_materialSlotHelper->OnRefreshIcon.AddUniqueDynamic(this, &UGsUICraftShortCutListEntry::OnRefreshIconMaterial);
	}

	_moveBtn->OnClicked.AddUniqueDynamic(this, &UGsUICraftShortCutListEntry::OnClickedMoveBtn);
}

void UGsUICraftShortCutListEntry::NativeDestruct()
{
	_targetIconSelector->RemoveAllChildren();
	_materialIconSelector->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsUICraftShortCutListEntry::NativeOnListItemObjectSet(UObject* InListItemObject)
{
	InvalidateListItem(InListItemObject);
}

void UGsUICraftShortCutListEntry::InvalidateListItem(const UObject* InListItemObject /*= nullptr*/)
{
	if (nullptr == InListItemObject)
	{
		InListItemObject = GetListItem();
	}

	const UGsUICraftShortCutListItem* ListItem = Cast<UGsUICraftShortCutListItem>(InListItemObject);
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(ListItem->_craftId);
	if (nullptr == CraftTarget)
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget"));
		return;
	}

	_targetSlotHelper->RefreshAll(1);
	_materialSlotHelper->RefreshAll(CraftTarget->GetMaterialNum());
}

void UGsUICraftShortCutListEntry::OnRefreshIconTarget(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* Icon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == Icon)
	{
		GSLOG(Error, TEXT("nullptr == Icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	const UGsUICraftShortCutListItem* ListItem = GetListItem<UGsUICraftShortCutListItem>();
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(ListItem->_craftId);
	if (nullptr == CraftTarget)
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget"));
		return;
	}

	const FGsCraftTargetObject* CraftTargetObject = CraftTarget->GetDefaultCraftTargetObject();
	if (nullptr == CraftTargetObject)
	{
		GSLOG(Error, TEXT("nullptr == CraftTargetObject"));
		return;
	}

	if (CostType::ITEM == CraftTargetObject->GetCostType())
	{
		Icon->SetNotOwnItem(CraftTargetObject->GetItemId(), 0);
	}
	else
	{
		Icon->SetCurrency(CraftTargetObject->GetCurrencyType(), 0);
	}

	_txtTitle->SetText(CraftTarget->GetCraftName());
	FSlateColor TitleColor = (CostType::ITEM == CraftTargetObject->GetCostType()) ? FGsUIColorHelper::GetColorItemGradeByItemId(CraftTargetObject->GetItemId()) : FGsUIColorHelper::GetColorItemGrade(ItemGrade::NONE);
	_txtTitle->SetColorAndOpacity(TitleColor);

	// 롱프레스 비활성화, 레드닷 제거, 이름 제거 등
	Icon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::NotUseDetailWndType);
	Icon->SetRedDot(false);
	Icon->SetItemNameVisibility(false);
}

void UGsUICraftShortCutListEntry::OnRefreshIconMaterial(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* Icon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == Icon)
	{
		GSLOG(Error, TEXT("nullptr == Icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	const UGsUICraftShortCutListItem* ListItem = GetListItem<UGsUICraftShortCutListItem>();
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(ListItem->_craftId);
	if (nullptr == CraftTarget)
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget"));
		return;
	}

	const TSharedRef<const FGsCostPackage> CostPackage = CraftTarget->GetCostPackage();
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
		Icon->SetNotOwnItem(CostElement->GetItemId(), 0);
	}

	// 롱프레스 비활성화, 레드닷 제거, 이름 제거 등
	Icon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::NotUseDetailWndType);
	Icon->SetRedDot(false);
	Icon->SetItemNameVisibility(false);
	Icon->SetDimmedImage(!CostElement->PlayerHasEnough());
}

void UGsUICraftShortCutListEntry::OnClickedMoveBtn()
{
	const UGsUICraftShortCutListItem* ListItem = Cast<UGsUICraftShortCutListItem>(GetListItem());
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	ListItem->_onClickMoveBtn.ExecuteIfBound(*ListItem);
}
