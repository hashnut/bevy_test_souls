
#include "GsUIPopupItemCollectionAlternateList.h"
#include "GsUIAlternateItemEntry.h"
#include "../Item/Collection/GsItemCollectionCondition.h"
#include "../Item/Collection/GsItemCollectionField.h"
#include "../Management/ScopeGame/GsItemCollectionManager.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UMG/Public/Components/ScrollBox.h"




void UGsUIPopupItemCollectionAlternateList::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIPopupItemCollectionAlternateList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperItemList = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperItemList->Initialize(_subClassOfItem, _scrollBoxItemList);
	_slotHelperItemList->OnRefreshEntry.AddDynamic(this, &UGsUIPopupItemCollectionAlternateList::OnRefreshItemList);
	
	_btnApply->OnClicked.AddDynamic(this, &UGsUIPopupItemCollectionAlternateList::OnClickApply);
}

void UGsUIPopupItemCollectionAlternateList::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIPopupItemCollectionAlternateList::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIPopupItemCollectionAlternateList::OnRefreshItemList(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIAlternateItemEntry> item = Cast<UGsUIAlternateItemEntry>(InEntry);
	if (false == item.IsValid() || nullptr == _data)
	{
		return;
	}

	item->SetData(_data, InIndex);
	
	if (false == item->OnClicked.IsBoundToObject(this))
	{
		item->OnClicked.BindUObject(this, &UGsUIPopupItemCollectionAlternateList::OnSelectedDelegate);
	}
	
}

void UGsUIPopupItemCollectionAlternateList::OnSelectedDelegate(UUserWidget* InEntry)
{
	TWeakObjectPtr<UGsUIAlternateItemEntry> item = Cast<UGsUIAlternateItemEntry>(InEntry);
	if (false == item.IsValid())
	{
		return;
	}

	SetSelected(item->GetData());
}

void UGsUIPopupItemCollectionAlternateList::OnClickApply()
{
	if (_data && _selectedMaterialId != INVALID_ITEM_ID)
	{
		const FGsItemCollectionField* field = GSItemCollection()->FindFieldByConditionId(_data->Id());
		if (field)
		{
			GSItemCollection()->FixedConditionEntryMaterialInfo(field->Id(), _data->Id(), _selectedMaterialId);
		}		
	}

	Close();
}

void UGsUIPopupItemCollectionAlternateList::SetSelected(ItemId InMaterialId)
{
	_selectedMaterialId = InMaterialId;

	for (int32 idx = 0; idx < _slotHelperItemList->GetItemCount(); ++idx)
	{
		TWeakObjectPtr<UGsUIAlternateItemEntry> item = Cast<UGsUIAlternateItemEntry>(_slotHelperItemList->GetEntry(idx));
		if (false == item.IsValid())
		{
			return;
		}

		item->SetIsSelected(InMaterialId == item->GetData());
	}
}

void UGsUIPopupItemCollectionAlternateList::SetData(const FGsItemCollectionCondition* InData)
{
	_data = InData;
	_selectedMaterialId = INVALID_ITEM_ID;

	const int32 maxCount = InData->MaterialAll().Num();
	_slotHelperItemList->RefreshAll(maxCount);

	SetSelected(InData->EntryMaterialInfo().Key);
}