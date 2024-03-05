// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIComboBox.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/ComboBox/GsUIComboBoxEntry.h"


void UGsUIComboBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_panelSlotHelper.Initialize(_panelList, true);
	_panelSlotHelper.OnRefreshEntry.BindUObject(this, &UGsUIComboBox::OnRefreshEntry);

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIComboBox::OnClickSlot);
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIComboBox::OnClickListEntry);
}

void UGsUIComboBox::NativeConstruct()
{
	Super::NativeConstruct();
	
	ShowList(false);
}

void UGsUIComboBox::NativeDestruct()
{
	Clear();

	Super::NativeDestruct();
}

void UGsUIComboBox::Clear()
{
	_nameList.Empty();
	_toggleGroup.Clear();
}

void UGsUIComboBox::StartAddMenu()
{
	_nameList.Empty();
}

void UGsUIComboBox::AddMenu(const FText& InNameText)
{
	_nameList.Emplace(InNameText);
}

void UGsUIComboBox::FinishAddMenu()
{
	_panelSlotHelper.RefreshAll(_nameList.Num());
	
	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_panelList, false);
}

void UGsUIComboBox::SetSelectedIndex(int32 InIndex)
{
	_toggleGroup.SetSelectedIndex(InIndex);

	if (_nameList.IsValidIndex(InIndex))
	{
		_textBlockSelected->SetText(_nameList[InIndex]);
	}
}

void UGsUIComboBox::ShowList(bool bInShow)
{
	_panelList->SetVisibility((bInShow) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIComboBox::OnClickSlot()
{
	ShowList((_panelList->IsVisible()) ? false : true);
}

void UGsUIComboBox::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _nameList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIComboBoxEntry* entry = Cast<UGsUIComboBoxEntry>(InEntry))
	{
		entry->SetNameText(_nameList[InIndex]);
	}
}

void UGsUIComboBox::OnClickListEntry(int32 InIndex)
{
	if (false == _nameList.IsValidIndex(InIndex))
	{
		return;
	}

	OnSelectComboBox.ExecuteIfBound(InIndex);

	_textBlockSelected->SetText(_nameList[InIndex]);

	ShowList(false);
}
