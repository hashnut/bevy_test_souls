// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIPopupKeywordVote.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "DataSchema/Social/GsSchemaSocialKeyword.h"
#include "Management/ScopeGame/GsUserProfileManager.h"
#include "Net/GsNetSendService.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/UserProfile/GsUIKeywordVoteListEntry.h"


void UGsUIPopupKeywordVote::BeginDestroy()
{
	if (nullptr != _slotHelperKeyword)
	{
		_slotHelperKeyword->OnCreateEntry.RemoveDynamic(this, &UGsUIPopupKeywordVote::KeywordListCreate);
		_slotHelperKeyword->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupKeywordVote::KeywordListRefresh);
		_slotHelperKeyword = nullptr;
	}

	if (_btnOk)
	{
		_btnOk->OnClicked.RemoveDynamic(this, &UGsUIPopupKeywordVote::OnClickOk);
	}
	if (_btnCancel)
	{
		_btnCancel->OnClicked.RemoveDynamic(this, &UGsUIPopupKeywordVote::OnClickCancel);
	}

	Super::BeginDestroy();
}

void UGsUIPopupKeywordVote::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupKeywordVote::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupKeywordVote::OnClickCancel);

	_toggleGroupKeyword.OnSlotClicked.BindUObject(this, &UGsUIPopupKeywordVote::OnClickSlotKeyword);
	_toggleGroupKeyword.OnSelectChanged.BindUObject(this, &UGsUIPopupKeywordVote::OnSelectChangedKeyword);

	if (nullptr == _slotHelperKeyword)
	{
		_slotHelperKeyword = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperKeyword->Initialize(_subClassOfKeyword, _scrollKeyword);
		_slotHelperKeyword->OnCreateEntry.AddDynamic(this, &UGsUIPopupKeywordVote::KeywordListCreate);
		_slotHelperKeyword->OnRefreshEntry.AddDynamic(this, &UGsUIPopupKeywordVote::KeywordListRefresh);
	}
}

void UGsUIPopupKeywordVote::NativeConstruct()
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

	// 설명 텍스트
	FText findText;
	FText::FindText(TEXT("SocialText"), TEXT("keyword_popup_OthersProfile"), findText);
	findText = FText::Format(findText, FText::FromString(GSUserProfile()->GetTargetUserName()));
	_textBlockDesc->SetText(findText);

	// 키워드 목록 생성
	_toggleGroupKeyword.Clear();

	_slotHelperKeyword->RefreshAll(_keywordList.Num() + 1);

	_selectedKeywordId = GSUserProfile()->GetSelectedKeyword();
	int32 selectedIndex = 0;
	for (int32 i = 0; i < _keywordList.Num(); ++i)
	{
		if (_keywordList[i]->id == _selectedKeywordId)
		{
			selectedIndex = i + 1;
			break;
		}
	}
	_toggleGroupKeyword.SetSelectedIndex(selectedIndex, true);
}

void UGsUIPopupKeywordVote::NativeDestruct()
{
	_toggleGroupKeyword.Clear();

	Super::NativeDestruct();
}

void UGsUIPopupKeywordVote::UpdateMainKeywordCount()
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

void UGsUIPopupKeywordVote::OnClickSlotKeyword(int32 InIndex, bool bIsSelected)
{
	_toggleGroupKeyword.SetSelectedIndex(InIndex);
}

void UGsUIPopupKeywordVote::OnSelectChangedKeyword(int32 InIndex)
{
	GSUserProfile()->SetKeywordCountMinus(_selectedKeywordId);

	if (InIndex > 0)
	{
		_selectedKeywordId = GetKeywordId(InIndex - 1);
		GSUserProfile()->SetKeywordCountPlus(_selectedKeywordId);
	}
	else
	{
		_selectedKeywordId = 0;
	}

	UpdateMainKeywordCount();

	_slotHelperKeyword->RefreshAll(_keywordList.Num() + 1);
}

uint64 UGsUIPopupKeywordVote::GetKeywordId(int32 InIndex)
{
	if (_keywordList.Num() > InIndex)
	{
		return _keywordList[InIndex]->id;
	}
	return 0;
}

void UGsUIPopupKeywordVote::KeywordListCreate(UWidget* InEntry)
{
	UGsUIKeywordVoteListEntry* entry = Cast<UGsUIKeywordVoteListEntry>(InEntry);
	if (nullptr == entry)
		return;

	_toggleGroupKeyword.AddToggleWidget(entry->GetToggleGroupEntry());
	entry->SetToggleGroup(&_toggleGroupKeyword);
}

void UGsUIPopupKeywordVote::KeywordListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIKeywordVoteListEntry* entry = Cast<UGsUIKeywordVoteListEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (InIndex <= 0)
	{
		entry->SetSwitcherIndex(2);
		entry->SetIsSelected(0 == _selectedKeywordId);
		return;
	}

	int32 listIndex = InIndex - 1;
	
	uint64 keywordId = GetKeywordId(listIndex);
	uint64 keywordCount = GSUserProfile()->GetKeywordCount(keywordId);

	entry->SetSwitcherIndex((_mainKeywordCount > 0 && keywordCount == _mainKeywordCount) ? 0 : 1);
	entry->SetData(false, _keywordList[listIndex]->icon, _keywordList[listIndex]->keywordText, keywordCount);
	entry->SetIsSelected(keywordId == _selectedKeywordId);
}

void UGsUIPopupKeywordVote::OnClickOk()
{
	if (false == GSUserProfile()->IsMyProfile())
	{
		FGsNetSendService::SendReqUserProfileKeywordUpdate(GSUserProfile()->GetTargetUserDBID(), _selectedKeywordId);
	}

	Close();
}

void UGsUIPopupKeywordVote::OnClickCancel()
{
	Close();
}

void UGsUIPopupKeywordVote::OnInputCancel()
{
	OnClickCancel();
}