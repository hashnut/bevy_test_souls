#include "GsUIPopupCollectionStatFilter.h"

#include "GameObject/Stat/GsStatHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "ItemCollection/GsUIItemCollectionStatTypeCheckbox.h"

void UGsUIPopupCollectionStatFilter::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPopupCollectionStatFilter::OnCreateEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupCollectionStatFilter::OnRefreshEntry);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupCollectionStatFilter::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIItemCollectionStatTypeCheckbox>())
		{
			child->RemoveFromParent();
		}
	}

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIPopupCollectionStatFilter::OnCreateEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupCollectionStatFilter::OnRefreshEntry);

	_checkboxAll->_onClickDelegate.BindUObject(this, &UGsUIPopupCollectionStatFilter::OnClickAll);
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupCollectionStatFilter::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupCollectionStatFilter::OnClickCancel);
}

void UGsUIPopupCollectionStatFilter::InitializeData(UGsUIPopupCollectionStatFilter::PopupInitData* initParam)
{
	_onClosed = nullptr;
	_onChanged = nullptr;

	_curStatMenu.Empty();

	if (initParam)
	{
		_curStatMenu.Append(initParam->_sortStatMenu);
		_onClosed = initParam->_cbClosed;
		_onChanged = initParam->_cbChanged;
		_scrollBoxHelper->RefreshAll(_curStatMenu.Num());
	}
}

void UGsUIPopupCollectionStatFilter::OnCreateEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIItemCollectionStatTypeCheckbox>(InEntry))
	{
		int32 index = _listPanel->GetChildIndex(InEntry);
		entry->_onClickDelegate.BindLambda([index, this]() {
				OnClickStatTypeCheckbox(index);
		});
	}
}

void UGsUIPopupCollectionStatFilter::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIItemCollectionStatTypeCheckbox>(InEntry))
	{
		if (_curStatMenu.IsValidIndex(InIndex))
		{	
			auto statVal = _curStatMenu[InIndex];

			FText StatName;
			if (FGsStatHelper::GetDisPlayStatName(statVal.Key, StatName))
			{
				entry->SetTitle(StatName);
			}
			entry->SetState(statVal.Value ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		}
	}
}
void UGsUIPopupCollectionStatFilter::OnClickAll()
{
	const ECheckBoxState CheckboxState = _checkboxAll->GetState();

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (auto item = Cast<UGsUIItemCollectionStatTypeCheckbox>(child))
		{
			item->SetState(CheckboxState);
		}
	}

	for (auto& e : _curStatMenu)
	{
		e.Value = CheckboxState == ECheckBoxState::Checked;
		if(_onChanged) _onChanged(e);
	}
}

void UGsUIPopupCollectionStatFilter::OnClickStatTypeCheckbox(int32 index)
{
	if (_curStatMenu.IsValidIndex(index))
	{
		if (auto entry = Cast<UGsUIItemCollectionStatTypeCheckbox>(_scrollBoxHelper->GetEntry(index)))
		{
			_curStatMenu[index].Value = entry->GetState() == ECheckBoxState::Checked;
			if(_onChanged) _onChanged(_curStatMenu[index]);
		}
	}
}

void UGsUIPopupCollectionStatFilter::OnClickOk()
{
	if(_onClosed) _onClosed(_curStatMenu);
	Close();
}

void UGsUIPopupCollectionStatFilter::OnClickCancel()
{
	Close();
}

void UGsUIPopupCollectionStatFilter::OnInputCancel()
{
	OnClickCancel();
}