#include "GsUIPopupSortCandidateGoods.h"

#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "Currency/GsCostPackage.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "SortCandidateGoods/GsUICandidateGoodsListItem.h"
#include "T1Project.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

void UGsUIPopupSortCandidateGoods::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _slotHelper)
	{
		_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelper->Initialize(_subClassOfList, _scrollList);
		_slotHelper->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIPopupSortCandidateGoods::OnRefreshEntryListItem);
	}

	_btnCancel->OnClicked.AddUniqueDynamic(this, &UGsUIPopupSortCandidateGoods::OnClickedCancel);
	_btnOk->OnClicked.AddUniqueDynamic(this, &UGsUIPopupSortCandidateGoods::OnClickedOk);
}

void UGsUIPopupSortCandidateGoods::NativeDestruct()
{
	// _scrollList->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsUIPopupSortCandidateGoods::SetParameters(Parameters InParameters)
{
	_listParameters.Empty();

	_parameters = MoveTemp(InParameters);

	if (!_parameters._costPackage.IsValid())
	{
		GSLOG(Error, TEXT("!_parameters._costPackage.IsValid()"));
		return;
	}

	const FGsCostElement* CostElement = _parameters._costPackage.Pin()->GetCostElementAt(_parameters._costElementIndex);
	if (nullptr == CostElement)
	{
		GSLOG(Error, TEXT("nullptr == CostElement, InIndex:%d"), _parameters._costElementIndex);
		return;
	}

	const UGsSupplyManager* SupplyManager = GSSupply();
	if (nullptr == SupplyManager)
	{
		GSLOG(Error, TEXT("nullptr == SupplyManager"));
		return;
	}

	const TArray<TWeakPtr<const FGsCostElement::FGsCostElementItem>> AllCandidateItems = CostElement->GetAllCandidateItems();
	check(AllCandidateItems.Num() < TNumericLimits<uint8>::Max());
	for (uint8 i = 0; i < AllCandidateItems.Num(); ++i)
	{
		if (!AllCandidateItems[i].IsValid())
		{
			GSLOG(Error, TEXT("!AllCandidateItems[%d].IsValid()"), i);
			continue;
		}

		TSharedPtr<const FGsCostElement::FGsCostElementItem> CandidateItem = AllCandidateItems[i].Pin();

		_listParameters.Add(MakeShared<UGsUICandidateGoodsListItem::Parameters>(UGsUICandidateGoodsListItem::Parameters {
			i,
			CandidateItem->_costItemId,
			CandidateItem->GetItemLevel(),
			CandidateItem->PlayerHasAmount(),
			UGsUICandidateGoodsListItem::Parameters::INVALID_SORT_ORDER,
			UGsSupplyManager::EGsNavigationStatus::Movable == SupplyManager->GetNavigationStatus(EGsUnlockContentsType::CRAFT, CandidateItem->_costItemId),
			UGsUICandidateGoodsListItem::FGsCandidateGoodsListItemDelegate::CreateUObject(this, &UGsUIPopupSortCandidateGoods::OnClickedListItemCraft),
			UGsUICandidateGoodsListItem::FGsCandidateGoodsListItemDelegate::CreateUObject(this, &UGsUIPopupSortCandidateGoods::OnClickedListItemSort)
		}));
	}

	Invalidate();
}

void UGsUIPopupSortCandidateGoods::Invalidate()
{
	_slotHelper->RefreshAll(_listParameters.Num());

	if (!_parameters._costPackage.IsValid())
	{
		GSLOG(Warning, TEXT("!_parameters._costPackage.IsValid()"));
		return;
	}

	// 갯수
	TSharedPtr<const FGsCostPackage> CostPackage = _parameters._costPackage.Pin();
	const FGsCostElement* CostElement = CostPackage->GetCostElementAt(_parameters._costElementIndex);
	const uint64 AvailableAmount = FMath::Min(CostElement->PlayerHasAmountMax(), CostElement->GetCostAmount());
	_txtCount->SetText(FText::FromString(FString::Printf(TEXT("%lld / %lld"), AvailableAmount, CostElement->GetCostAmount())));
}

int32 UGsUIPopupSortCandidateGoods::GetCurrentMaxSortOrder() const
{
	int32 NextSmallestSortOrder = -1;

	for (uint8 i = 0; i < _listParameters.Num(); ++i)
	{
		TSharedRef<UGsUICandidateGoodsListItem::Parameters> CurrentParameterItem = _listParameters[i];
		if (!CurrentParameterItem->IsValidSortOrder())
		{
			continue;
		}

		if (NextSmallestSortOrder < CurrentParameterItem->_sortOrder)
		{
			NextSmallestSortOrder = CurrentParameterItem->_sortOrder;
		}
	}

	return NextSmallestSortOrder;
}

void UGsUIPopupSortCandidateGoods::OnRefreshEntryListItem(int32 InIndex, UWidget* InEntry)
{
	UGsUICandidateGoodsListItem* ListItem = Cast<UGsUICandidateGoodsListItem>(InEntry);
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem, *InEntry->GetName():%s"), *InEntry->GetName());
		return;
	}

	if (!_listParameters.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_listParameters.IsValidIndex(InIndex), InIndex:%d"), InIndex);
		return;
	}

	ListItem->SetParameters(_listParameters[InIndex]);
}

void UGsUIPopupSortCandidateGoods::OnClickedListItemCraft(const int32 InIndex)
{
	if (!_listParameters.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_listParameters.IsValidIndex(InIndex), InIndex:%d"), InIndex);
		return;
	}

	// close 이후에 값이 날아갈 수 있으므로 캐쉬.
	const ItemId CurrentItemId = _listParameters[InIndex]->_itemId;

	Close();

	const UGsSupplyManager* SupplyManager = GSSupply();
	if (nullptr == SupplyManager)
	{
		GSLOG(Error, TEXT("nullptr == SupplyManager"));
		return;
	}

	if (!SupplyManager->TryToNavigateItemId(EGsUnlockContentsType::CRAFT, CurrentItemId))
	{
		GSLOG(Error, TEXT("!SupplyManager->TryToNavigate(EGsUnlockContentsType::Craft, %d)"), CurrentItemId);
	}
}

void UGsUIPopupSortCandidateGoods::OnClickedListItemSort(const int32 InIndex)
{
	if (!_listParameters.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_listParameters.IsValidIndex(InIndex), InIndex:%d"), InIndex);
		return;
	}

	TSharedRef<UGsUICandidateGoodsListItem::Parameters> ListItemParameters = _listParameters[InIndex];
	if (ListItemParameters->IsValidSortOrder())
	{
		// already setted sort order
		return;
	}

	// sort order 가 이미 할당된 요소이므로 스킵
	ListItemParameters->_sortOrder = GetCurrentMaxSortOrder() + 1;
}

void UGsUIPopupSortCandidateGoods::OnClickedCancel()
{
	Close();
}

void UGsUIPopupSortCandidateGoods::OnClickedOk()
{
	if (!_parameters._costPackage.IsValid())
	{
		GSLOG(Warning, TEXT("!_parameters._costPackage.IsValid()"));
	}
	else
	{
		TSharedPtr<const FGsCostPackage> CostPackage = _parameters._costPackage.Pin();
		const FGsCostElement* CostElement = CostPackage->GetCostElementAt(_parameters._costElementIndex);
		if (nullptr == CostElement)
		{
			GSLOG(Error, TEXT("nullptr == CostElement"));
		}
		else
		{
			for (TSharedRef<UGsUICandidateGoodsListItem::Parameters> ListParameter : _listParameters)
			{
				const FGsCostElement::FGsCostElementBase* CurrentCandidate = CostElement->GetCandidateAt(ListParameter->_index);
				if (nullptr == CurrentCandidate)
				{
					GSLOG(Error, TEXT("nullptr == CurrentCandidate, ListParameter->_index:%d, ListParameter->_itemId:%d"), ListParameter->_index, ListParameter->_itemId);
				}
				else
				{
					CurrentCandidate->_priority = ListParameter->_sortOrder;
				}
			}

			// 사용자 정렬로 변경
			const_cast<FGsCostElement*>(CostElement)->SortCandidates(EGsSortItemDisposal::UserSpecific);
		}
	}

	_parameters._onClickedOk.ExecuteIfBound();

	Close();
}

UGsUIPopupSortCandidateGoods* UGsUIPopupSortCandidateGoods::OpenThis(Parameters InParameters)
{
	UGsUIManager* UIManager = GUI();
	TWeakObjectPtr<UGsUIWidgetBase> WidgetBase = UIManager->OpenAndGetWidget(TEXT("PopupSortCandidateGoods"));
	if (!WidgetBase.IsValid())
	{
		GSLOG(Error, TEXT("WidgetBase.IsValid()"));
		return nullptr;
	}

	UGsUIPopupSortCandidateGoods* PopupSortCandidateGoods = Cast<UGsUIPopupSortCandidateGoods>(WidgetBase.Get());
	if (nullptr == PopupSortCandidateGoods)
	{
		GSLOG(Error, TEXT("nullptr == PopupSortCandidateGoods"));
		return nullptr;
	}

	PopupSortCandidateGoods->SetParameters(MoveTemp(InParameters));
	return PopupSortCandidateGoods;
}

void UGsUIPopupSortCandidateGoods::OnInputCancel()
{
	OnClickedCancel();
}