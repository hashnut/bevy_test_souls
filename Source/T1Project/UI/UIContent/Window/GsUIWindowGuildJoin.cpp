// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowGuildJoin.h"
#include "Components/EditableTextBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Guild/GsUIGuildListEntry.h"
#include "UI/UIContent/Window/GsUIWindowGuildStart.h"
#include "UI/UIContent/Common/ComboBox/GsUIComboBox.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Guild/GsGuildData.h"
#include "T1Project.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "UTIL/GsText.h"


void UGsUIWindowGuildJoin::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowGuildJoin::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIWindowGuildJoin::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelList);
	_slotHelper->SetEmptyListPanel(_panelEmptyList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowGuildJoin::OnRefreshEntry);

	_toggleGroupTab.AddToggleWidgetByParentPanel(_panelTab);
	_toggleGroupTab.OnSelectChanged.BindUObject(this, &UGsUIWindowGuildJoin::OnChangeTab);

	_comboBoxFilter->OnSelectComboBox.BindUObject(this, &UGsUIWindowGuildJoin::OnSelectComboBox);

	_inputBoxSearch->OnTextChanged.AddDynamic(this, &UGsUIWindowGuildJoin::OnTextChangedSearch);

	_btnSearch->OnClicked.AddDynamic(this, &UGsUIWindowGuildJoin::OnClickSearch);
	_btnUpdate->OnClicked.AddDynamic(this, &UGsUIWindowGuildJoin::OnClickUpdateList);

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowGuildJoin::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowGuildJoin::OnClickCloseAllStack);
}

void UGsUIWindowGuildJoin::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();
	
	FText textFilter0;
	FText::FindText(TEXT("GuildText"), TEXT("GuildJoinType_All"), textFilter0); // TEXT: 전체
	FText textFilter1;
	FText::FindText(TEXT("GuildText"), TEXT("GuildJoinType_AtOnce"), textFilter1); // TEXT: 즉시 가입
	FText textFilter2;
	FText::FindText(TEXT("GuildText"), TEXT("GuildJoinType_Application"), textFilter2); // TEXT: 가입 신청

	_comboBoxFilter->StartAddMenu();
	_comboBoxFilter->AddMenu(textFilter0);
	_comboBoxFilter->AddMenu(textFilter1);
	_comboBoxFilter->AddMenu(textFilter2);
	_comboBoxFilter->FinishAddMenu();
	_comboBoxFilter->SetSelectedIndex(0);	

	_toggleGroupTab.SetSelectedIndex(0, true);

	if (IsStackRecoverFlag())
	{
		// 이전 탭으로 복구
		_toggleGroupTab.SetSelectedIndex(_prevTabIndex, true);
	}
}

void UGsUIWindowGuildJoin::NativeDestruct()
{
	_prevTabIndex = _toggleGroupTab.GetSelectedIndex();
	_currTabIndex = 0;
	_currComboBoxIndex = 0;

	UnbindMessages();

	Super::NativeDestruct();
}

bool UGsUIWindowGuildJoin::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsGuild))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_GUILD);

		// 정해진 로직으로 열어야해서...
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("WindowGuildStart"));
		if (widget.IsValid())
		{
			if (UGsUIWindowGuildStart* windowStart = Cast<UGsUIWindowGuildStart>(widget.Get()))
			{
				windowStart->StartOpenJoin();
			}
		}

		return true;
	}

	return false;
}
void UGsUIWindowGuildJoin::BindMessages()
{
	MGuild& msgGuild = GMessage()->GetGuild();
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_GUILD_LIST,
		this, &UGsUIWindowGuildJoin::OnReceiveGuildList));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_JOIN_WAITING_GUILD_LIST,
		this, &UGsUIWindowGuildJoin::OnReceiveJoinWaitingGuildList));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::INVALIDATE_GUILD_JOIN_LIST,
		this, &UGsUIWindowGuildJoin::OnInvalidateGuildJoinList));
}

void UGsUIWindowGuildJoin::UnbindMessages()
{
	MGuild& msgGuild = GMessage()->GetGuild();
	for (MsgGuildHandle& handle : _msgGuildHandleList)
	{
		msgGuild.Remove(handle);
	}
	_msgGuildHandleList.Empty();
}

void UGsUIWindowGuildJoin::OnChangeTab(int32 InIndex)
{
	// 0: 길드 목록, 1: 내가 신청한 길드목록
	_currTabIndex = InIndex;

	// 길드 목록에서만 보여준다
	_panelOption->SetVisibility((InIndex == 0) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	_inputBoxSearch->SetText(FText::GetEmpty());

	// 요청 성공시엔 패킷받고 갱신, 아닐 때는 그냥 갱신
	if (0 == InIndex)
	{
		GGuild()->GetSendController().SendRequestGuildList();
	}
	else if (1 == InIndex)
	{
		GGuild()->GetSendController().SendRequestRequestedJoinGuildList();
	}

	InvalidateGuildList();
}

void UGsUIWindowGuildJoin::OnSelectComboBox(int32 InIndex)
{
	_currComboBoxIndex = InIndex;

	InvalidateGuildList();
}

void UGsUIWindowGuildJoin::OnClickSearch()
{
	FString strSearch = _inputBoxSearch->GetText().ToString();
	if (false == strSearch.IsEmpty())
	{
		if (false == GGuild()->GetSendController().SendRequestSearchGuild(strSearch, true))
		{
			// TEXT: 잠시 후 다시 시도해주세요.
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}
	}
}

void UGsUIWindowGuildJoin::OnTextChangedSearch(const FText& InText)
{
	FString text = InText.ToString();
	int32 length = FGsTextUtil::GetStringLengthNameType(text);

	if (MAX_GUILD_NAME_LEN < length)
	{
		text.RemoveAt(text.Len() - 1);

		_inputBoxSearch->SetText(FText::FromString(text));
	}
}

void UGsUIWindowGuildJoin::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _guildList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIGuildListEntry* entry = Cast<UGsUIGuildListEntry>(InEntry))
	{
		entry->SetData(_guildList[InIndex]);
	}
}

void UGsUIWindowGuildJoin::OnClickUpdateList()
{
	if (false == GGuild()->GetSendController().SendRequestRefreshGuildList())
	{
		// TEXT: 잠시 후 다시 시도해주세요.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}
}

void UGsUIWindowGuildJoin::OnReceiveGuildList(const IGsMessageParam*)
{
	if (0 == _toggleGroupTab.GetSelectedIndex())
	{
		InvalidateGuildList();
	}
}

void UGsUIWindowGuildJoin::OnReceiveJoinWaitingGuildList(const IGsMessageParam*)
{
	if (1 == _toggleGroupTab.GetSelectedIndex())
	{
		InvalidateGuildList();
	}
}

void UGsUIWindowGuildJoin::OnInvalidateGuildJoinList(const IGsMessageParam*)
{
	InvalidateGuildList();
}

void UGsUIWindowGuildJoin::InvalidateGuildList()
{
	_guildList.Empty();

	FGsGuildManager* guildMgr = GGuild();

	// 요청 성공시엔 패킷받고 갱신, 아닐 때는 그냥 갱신
	if (0 == _currTabIndex)
	{
		TArray<FGsGuildData*> tempList = GGuild()->GetJoinGuildList();

		GuildJoinType joinType = GuildJoinType::MAX;
		switch (_currComboBoxIndex)
		{
		case 1:
			joinType = GuildJoinType::IMMEDIATE;
			break;
		case 2:
			joinType = GuildJoinType::APPROVAL;
			break;
		}

		for (FGsGuildData* iter : tempList)
		{
			if (GuildJoinType::MAX == joinType)
			{
				_guildList.Emplace(iter);
			}
			else
			{
				if (iter->_joinType == joinType)
				{
					_guildList.Emplace(iter);
				}
			}
		}
	}
	else if (1 == _currTabIndex)
	{
		_guildList.Append(guildMgr->GetJoinWaitingGuildList());
	}

	SortGuildList();

	_slotHelper->RefreshAll(_guildList.Num());
}

void UGsUIWindowGuildJoin::SortGuildList()
{
	// 1순위 기사단 레벨 내림차순	
	_guildList.Sort([](const FGsGuildData& A, const FGsGuildData& B)
		{
			return (A._guildLevel >= B._guildLevel) ? true : false;
		});

	// 2순위 기획은 건물 레벨 총 합이었는데, 바뀔 확률이 높아서 구현 안하기로 함
}
