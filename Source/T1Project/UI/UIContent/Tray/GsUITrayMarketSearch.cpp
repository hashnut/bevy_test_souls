// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayMarketSearch.h"
#include "Components/PanelWidget.h"
#include "Components/EditableTextBox.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Tray/Market/GsUIMarketSearchHistoryEntry.h"
#include "Market/GsMarketItemGroupData.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Option/GsGameUserSettings.h"
#include "Net/GsNetSendServiceMarket.h"
#include "Message/MessageParam/GsMarketMessageParam.h"


void UGsUITrayMarketSearch::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUITrayMarketSearch::OnRefreshEntry);
	}
	_slotHelper = nullptr;
	
	Super::BeginDestroy();
}

void UGsUITrayMarketSearch::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnDelete->OnClicked.AddDynamic(this, &UGsUITrayMarketSearch::OnClickDelete);
	_btnOk->OnClicked.AddDynamic(this, &UGsUITrayMarketSearch::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUITrayMarketSearch::OnClickCancel);
	
	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelHistoryListRoot);
	_slotHelper->SetEmptyListPanel(_panelEmptyRoot);
	_slotHelper->OnCreateEntry.AddDynamic(this, &UGsUITrayMarketSearch::OnCreateEntry);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUITrayMarketSearch::OnRefreshEntry);
	
	_editableText->OnTextChanged.AddDynamic(this, &UGsUITrayMarketSearch::OnTextChanged);
}

void UGsUITrayMarketSearch::NativeConstruct()
{
	Super::NativeConstruct();
	
	_dataList.Empty();
	UGsGameUserSettings* settings = GGameUserSettings();
	if (settings)
	{
		_dataList.Append(settings->GetMarketSearchList());
	}

	_slotHelper->RefreshAll(_dataList.Num());

	// 열 때 이전 검색내역 초기화
	_editableText->SetText(FText::GetEmpty());
}

void UGsUITrayMarketSearch::NativeDestruct()
{
	Super::NativeDestruct();

	UGsGameUserSettings* settings = GGameUserSettings();
	if (settings)
	{
		settings->SaveUserSettings();
	}
}

void UGsUITrayMarketSearch::OnClickDelete()
{
	_editableText->SetText(FText::GetEmpty());
}

void UGsUITrayMarketSearch::OnClickOk()
{
	FString strName = _editableText->GetText().ToString();

	// GameUserSetting에 검색어 저장
	if (false == strName.IsEmpty())
	{
		UGsGameUserSettings* settings = GGameUserSettings();
		if (settings)
		{
			settings->SetSearchString(strName, true);
		}
	}
	
	FGsMarketMsgParamString param(strName);
	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_SEARCH, &param);

	Close();
}

void UGsUITrayMarketSearch::OnClickCancel()
{
	Close();
}

void UGsUITrayMarketSearch::OnCreateEntry(class UWidget* InEntry)
{
	if (UGsUIMarketSearchHistoryEntry* entry = Cast<UGsUIMarketSearchHistoryEntry>(InEntry))
	{
		entry->OnClickEvent.BindUObject(this, &UGsUITrayMarketSearch::OnClickListEntry);
	}
}

void UGsUITrayMarketSearch::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIMarketSearchHistoryEntry* entry = Cast<UGsUIMarketSearchHistoryEntry>(InEntry))
	{
		entry->SetData(InIndex, _dataList[InIndex]);
	}
}

void UGsUITrayMarketSearch::OnTextChanged(const FText& InText)
{
	FString strText = InText.ToString();	
	int32 length = strText.Len();

	// 지정 글자수 이상 입력하지 못하게 막음
	if (MAX_TEXT_LENGTH < length)
	{
		strText.RemoveAt(strText.Len() - 1);
		_editableText->SetText(FText::FromString(strText));
	}
}

void UGsUITrayMarketSearch::OnClickListEntry(int32 InIndex, bool bIsRemove)
{
	if (bIsRemove)
	{		
		if (_dataList.IsValidIndex(InIndex))
		{
			FString strName = _dataList[InIndex];

			UGsGameUserSettings* settings = GGameUserSettings();
			if (settings)
			{
				settings->SetSearchString(strName, false);
			}

			_dataList.RemoveAt(InIndex);
			_slotHelper->RefreshAll(_dataList.Num());
		}
	}
	else
	{
		if (_dataList.IsValidIndex(InIndex))
		{
			_editableText->SetText(FText::FromString(_dataList[InIndex]));
		}
	}
}
