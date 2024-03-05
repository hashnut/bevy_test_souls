// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildDiplomacySearch.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/Guild/GsUIGuildDiplomacySearchedEntry.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildData.h"
#include "Guild/GsGuildDiplomacyData.h"
#include "Guild/GsGuildHelper.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "UTIL/GsText.h"


void UGsUIPopupGuildDiplomacySearch::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupGuildDiplomacySearch::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupGuildDiplomacySearch::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupGuildDiplomacySearch::OnRefreshEntry);

	_editableText->OnTextChanged.AddDynamic(this, &UGsUIPopupGuildDiplomacySearch::OnTextChanged);
	_btnSearch->OnClicked.AddDynamic(this, &UGsUIPopupGuildDiplomacySearch::OnClickSearch);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupGuildDiplomacySearch::OnClickCancel);
}

void UGsUIPopupGuildDiplomacySearch::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();

	_switcherList->SetActiveWidgetIndex(0);
	_slotHelper->RefreshAll(0);
}

void UGsUIPopupGuildDiplomacySearch::NativeDestruct()
{
	UnbindMessages();

	Super::NativeDestruct();
}

void UGsUIPopupGuildDiplomacySearch::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildDiplomacySearch::BindMessages()
{
	MGuild& msgGuild = GMessage()->GetGuild();
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_GUILD_LIST,
		this, &UGsUIPopupGuildDiplomacySearch::OnReceiveGuildList));
}

void UGsUIPopupGuildDiplomacySearch::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MGuild& msgGuild = msgMgr->GetGuild();
	for (MsgGuildHandle& handle : _msgGuildHandleList)
	{
		msgGuild.Remove(handle);
	}
	_msgGuildHandleList.Empty();
}

void UGsUIPopupGuildDiplomacySearch::SetData(bool bInIsAlly)
{
	_bIsAlly = bInIsAlly;

	FText textMsg;
	if (bInIsAlly)
	{
		// TEXT: 동맹을 요청할 기사단명을 입력하세요.
		FText::FindText(TEXT("GuildText"), TEXT("UI_Diplomacy_Search_Ally"), textMsg);
	}
	else
	{
		// TEXT: 적대를 요청할 기사단명을 입력하세요.
		FText::FindText(TEXT("GuildText"), TEXT("UI_Diplomacy_Search_Enemy"), textMsg);
	}

	_tbMessage->SetText(textMsg);

	_editableText->SetText(FText::GetEmpty());
	
	_slotHelper->RefreshAll(0);
	_scroll->ScrollToStart();
}

void UGsUIPopupGuildDiplomacySearch::OnTextChanged(const FText& InText)
{
	FString strText = InText.ToString();
	int32 length = FGsTextUtil::GetStringLengthNameType(strText);

	// 지정 글자수 이상 입력하지 못하게 막음
	if (MAX_GUILD_NAME_LEN < length)
	{
		strText.RemoveAt(strText.Len() - 1);
		_editableText->SetText(FText::FromString(strText));
	}
}

void UGsUIPopupGuildDiplomacySearch::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (_dataList.IsValidIndex(InIndex))
	{
		if (UGsUIGuildDiplomacySearchedEntry* entry = Cast<UGsUIGuildDiplomacySearchedEntry>(InEntry))
		{
			entry->SetData(_dataList[InIndex], _bIsAlly);
		}
	}
}

void UGsUIPopupGuildDiplomacySearch::OnClickSearch()
{
	FString strGuildName = _editableText->GetText().ToString();
	strGuildName.RemoveSpacesInline();

	// 기사단 이름 유효성 검사
	if (false == FGsGuildHelper::CheckDiplomacyGuildName(strGuildName, true, false))
	{
		return;
	}

	// 서버에 요청
	if (false == GGuild()->GetSendController().SendRequestSearchGuild(strGuildName, false))
	{
		// TEXT: 잠시 후 다시 시도해주세요.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}
}

void UGsUIPopupGuildDiplomacySearch::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupGuildDiplomacySearch::OnReceiveGuildList(const IGsMessageParam*)
{
	_dataList.Empty();
	
	if (const FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildData())
	{
		TArray<FGsGuildData*> guildList = GGuild()->GetJoinGuildList();

		// 내 길드, 이미 외교중(적대/동맹)인 길드 제거
		for (FGsGuildData* guild : guildList)
		{
			if (myGuildData->_guildDBId == guild->_guildDBId)
			{
				continue;
			}
			else if (myGuildData->IsExistInDiplomacyList(guild->_guildDBId))
			{
				continue;
			}

			_dataList.Emplace(guild);
		}
	}

	GuildDBId myGuildDBId = GGuild()->GetGuildDBId();	

	// 이름순 정렬
	_dataList.Sort([](const FGsGuildData& A, const FGsGuildData& B)
		{
			return (A._guildName <= B._guildName) ? true : false;
		});

	// 0: 빈 리스트, 1: 리스트
	_switcherList->SetActiveWidgetIndex((0 == _dataList.Num()) ? 0 : 1);
	_slotHelper->RefreshAll(_dataList.Num());
}
