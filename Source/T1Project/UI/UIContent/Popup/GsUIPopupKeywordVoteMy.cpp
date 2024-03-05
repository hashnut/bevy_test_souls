// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIPopupKeywordVoteMy.h"
#include "Components/ScrollBox.h"
#include "DataSchema/Social/GsSchemaSocialKeyword.h"
#include "Management/ScopeGame/GsUserProfileManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/UserProfile/GsUIKeywordVoteListEntry.h"


void UGsUIPopupKeywordVoteMy::BeginDestroy()
{
	if (nullptr != _slotHelperKeyword)
	{
		_slotHelperKeyword->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupKeywordVoteMy::KeywordListRefresh);
		_slotHelperKeyword = nullptr;
	}

	if (_btnOk)
	{
		_btnOk->OnClicked.RemoveDynamic(this, &UGsUIPopupKeywordVoteMy::OnClickOk);
	}

	Super::BeginDestroy();
}

void UGsUIPopupKeywordVoteMy::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupKeywordVoteMy::OnClickOk);

	if (nullptr == _slotHelperKeyword)
	{
		_slotHelperKeyword = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperKeyword->Initialize(_subClassOfKeyword, _scrollKeyword);
		_slotHelperKeyword->OnRefreshEntry.AddDynamic(this, &UGsUIPopupKeywordVoteMy::KeywordListRefresh);
	}
}

void UGsUIPopupKeywordVoteMy::NativeConstruct()
{
	Super::NativeConstruct();

	const UGsTableSocialKeyword* table = Cast<UGsTableSocialKeyword>(FGsSchemaSocialKeyword::GetStaticTable());
	if (nullptr == table)
		return;

	_keywordList.Empty();
	if (table->GetAllRows<FGsSchemaSocialKeyword>(_keywordList))
	{
		_keywordList.Sort([](const FGsSchemaSocialKeyword& itemA, const FGsSchemaSocialKeyword& itemB)
			{
				return itemA.displayOrder < itemB.displayOrder;
			}
		);
	}
	UpdateMainKeywordCount();

	_slotHelperKeyword->RefreshAll(_keywordList.Num());
}

void UGsUIPopupKeywordVoteMy::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIPopupKeywordVoteMy::UpdateMainKeywordCount()
{
	_mainKeywordCount = 0;

	for (const FGsSchemaSocialKeyword* keywordData : _keywordList)
	{
		uint64 keywordCount = GSUserProfile()->GetKeywordCount(keywordData->id);
		if (keywordCount > _mainKeywordCount)
		{
			_mainKeywordCount = keywordCount;
		}
	}
}

uint64 UGsUIPopupKeywordVoteMy::GetKeywordId(int32 InIndex)
{
	if (_keywordList.Num() > InIndex)
	{
		return _keywordList[InIndex]->id;
	}
	return 0;
}

void UGsUIPopupKeywordVoteMy::KeywordListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIKeywordVoteListEntry* entry = Cast<UGsUIKeywordVoteListEntry>(InEntry);
	if (nullptr == entry)
		return;

	uint64 keywordId = GetKeywordId(InIndex);
	uint64 keywordCount = GSUserProfile()->GetKeywordCount(keywordId);

	entry->SetSwitcherIndex((_mainKeywordCount > 0 && keywordCount == _mainKeywordCount) ? 0 : 1);
	entry->SetData(true, _keywordList[InIndex]->icon, _keywordList[InIndex]->keywordText, keywordCount);
}

void UGsUIPopupKeywordVoteMy::OnClickOk()
{
	Close();
}

void UGsUIPopupKeywordVoteMy::OnInputCancel()
{
	Close();
}