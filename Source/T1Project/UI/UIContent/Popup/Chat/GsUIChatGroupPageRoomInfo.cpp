// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/Chat/GsUIChatGroupPageRoomInfo.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Guild/GsGuildGroupChatData.h"
#include "Guild/GsGuildHelper.h"
#include "UI/UIControlLib/ContentWidget/GsScrollBox.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Menu/GsUIDropDownMenuEntry.h"
#include "UI/UIContent/Popup/Chat/GsUIDropDownMenuChat.h"
#include "UI/UIContent/Popup/Chat/GsUIChatGroupGuildData.h"
#include "UI/UIContent/Popup/Chat/GsUIChatGroupMemberEntry.h"
#include "UI/UIContent/Popup/GsUIPopupCommonInput.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "Net/GsNetSendServiceGuild.h"

const FName NamePopupInvitationChatRoom = TEXT("PopupInvitationChatRoom");
const FName NamePopupLeaveChatRoom = TEXT("PopupLeaveChatRoom");

struct MenuDataGuildGrade : public IMenuData
{
	GuildMemberGradeType _grade;
	MenuDataGuildGrade(GuildMemberGradeType grade) : _grade(grade) {};
};

ENUM_RANGE_BY_FIRST_AND_LAST(GuildMemberGradeType, GuildMemberGradeType::KNIGHT, GuildMemberGradeType::CAPTAIN)
void UGsUIChatGroupPageRoomInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toggleGroupTab.AddToggleWidget(_tabBtnGroupChat);
	_toggleGroupTab.AddToggleWidget(_tabBtnInvite);

	_toggleGroupTab.OnSelectChanged.BindUObject(this, &UGsUIChatGroupPageRoomInfo::OnTabChanged);
	_toggleGroupTab.SetSelectedIndex(0);

	_btnLeave->OnClicked.AddDynamic(this, &UGsUIChatGroupPageRoomInfo::OnClickLeave);
	_btnInvite->OnClicked.AddDynamic(this, &UGsUIChatGroupPageRoomInfo::OnClickInvite);

	_chatGroupTargetGrade->OnSelectedEntry.BindUObject(this, &UGsUIChatGroupPageRoomInfo::OnGradeSelected);

	TWeakObjectPtr<UGsUIChatGroupPageRoomInfo> thiz = this;

	_memberGuildList->InitializeData();
	_memberGuildList->OnEntryWidgetGenerated().AddLambda([thiz](UUserObjectScrollEntry& entry) {
		if (UGsUIChatGroupMemberEntry* memberGuild = Cast<UGsUIChatGroupMemberEntry>(&entry))
		{
			if (thiz.IsValid())
			{
				thiz->SetEntryFunction(memberGuild);
			}
		}
		});

	_memberGuildList->OnEntryWidgetReleased().AddLambda([](UUserObjectScrollEntry& entry) {
		if (UGsUIChatGroupMemberEntry* memberGuild = Cast<UGsUIChatGroupMemberEntry>(&entry))
		{
			memberGuild->OnEntrustMaster.Unbind();
			memberGuild->OnKickOut.Unbind();
			memberGuild->OnCancelInvitation.Unbind();
		}
		});
}

void UGsUIChatGroupPageRoomInfo::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto guildData = GGuild()->GetMyGuildData())
	{
		_myGuildDBId = guildData->_guildDBId;
	}

	int count = static_cast<int>(GuildMemberGradeType::MAX) - static_cast<int>(GuildMemberGradeType::KNIGHT);
	_chatGroupTargetGrade->InitializeMenu(count, -1);

	int index = 0;
	for (auto grade : TEnumRange<GuildMemberGradeType>())
	{
		FText gradeText;
		if (FGsGuildHelper::GetMemberGradeText(grade, gradeText))
		{
			_chatGroupTargetGrade->SetMenuEntryName(index, gradeText);
			_chatGroupTargetGrade->SetMenuEntryOptionData(index, MakeShared<MenuDataGuildGrade>(grade));
			index++;
		}
	}
}

void UGsUIChatGroupPageRoomInfo::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIChatGroupPageRoomInfo::InitializeData(TWeakPtr<FGsGuildGroupChatRoom> weakData)
{
	_curData = weakData;

	_tabBtnInvite->SetIsEnabled(false);
	_bottomInfo->SetVisibility(ESlateVisibility::Collapsed);
	_chatGroupTargetGrade->SetMenuPanelActive(false);

	_toggleGroupTab.SetSelectedIndex(0, true);
	_chatGroupTargetGrade->SetSelectMenuEntry(0);
	
	RefreshUI();
}

void UGsUIChatGroupPageRoomInfo::RefreshUI()
{
	_selectNoData->SetActiveWidgetIndex(_curData.IsValid() ? 1 : 0);
	_chatGroupTargetGrade->SetVisibility(_curData.IsValid() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (auto data = _curData.Pin())
	{
		_tabBtnInvite->SetIsEnabled(data->IsRoomMasterAuthorized());

		_bottomInfo->SetVisibility((data->IsRoomMasterAuthorized() || data->IsRoomLeaveAuthorized()) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		_btnLeave->SetIsEnabled(data->IsRoomLeaveAuthorized());
		_btnInvite->SetIsEnabled(data->IsRoomMasterAuthorized());

		auto selectIndex = static_cast<int>(data->GetChangingAuthGrade()) - static_cast<int>(GuildMemberGradeType::KNIGHT);
		_chatGroupTargetGrade->SetSelectMenuEntry(selectIndex);
		_chatGroupTargetGrade->SetMenuPanelActive(data->IsRoomMasterAuthorized());

		auto index = _tabPanel->GetActiveWidgetIndex();
		OnTabChanged(index);
	}
}

void UGsUIChatGroupPageRoomInfo::OnTabChanged(int32 index)
{
	_tabPanel->SetActiveWidgetIndex(index);

	if (index == 0)
	{
		OnSelectMemberTab();
	}
	else
	{
		OnSelectInvitationWaitingTab();
	}
}

bool UGsUIChatGroupPageRoomInfo::OnGradeSelected(int32 index, bool forced /*= false*/)
{
	if (auto data = _curData.Pin())
	{
		auto selectIndex = index + static_cast<int>(GuildMemberGradeType::KNIGHT);
		data->SetChangingAuthGrade(static_cast<GuildMemberGradeType>(selectIndex));
	}
	return true;
}

void UGsUIChatGroupPageRoomInfo::OnClickInvite()
{
	// 초대 창
	if (auto data = _curData.Pin())
	{
		if (data->GetMemberCount() >= GGuild()->GetMaxGroupRoomCount())
		{
			FText findText;
			FText::FindText(TEXT("ChatText"), TEXT("Ticker_ChatGroup_InviteFail1"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			return;
		}

		auto roomId = data->GetRoomId();
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(NamePopupInvitationChatRoom);
		if (widget.IsValid())
		{
			if (UGsUIPopupCommonInput* popup = Cast<UGsUIPopupCommonInput>(widget.Get()))
			{
				FText titleText;
				FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_InviteKnights"), titleText);
				FText descText;
				FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_InviteExplain"), descText);
				FText warningText;
				FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_InviteAnotherServer"), warningText);

				popup->SetTitleText(titleText);
				popup->SetDescText(descText);
				popup->SetWarningText(warningText);
				popup->SetMaxTextLength(MAX_GUILD_NAME_LEN + MAX_SERVER_NAME_LEN, true);
				popup->SetCallbackFunc([roomId](const FString& inviteGuldName) {

					TArray<const TCHAR*> delimArray = GSChat()->worldStartDelim;
					const auto worldStartDelim = delimArray.GetData();

					FText worldName;
					FString guildName;
					TArray<FString> strCheck;
					inviteGuldName.ParseIntoArray(strCheck, worldStartDelim, delimArray.Num(), true);
					if (strCheck.IsValidIndex(0))
					{
						guildName = strCheck[0];
						if (strCheck.IsValidIndex(1))
						{
							worldName = FText::FromString(strCheck[1]);
						}
					}
					auto myWorldId = GGuild()->GetMyGuildData()->GetWorldId();
					auto myGuildName = GGuild()->GetMyGuildData()->_guildName;
					auto targetWorldId = worldName.IsEmpty() ? myWorldId : FGsContentsServerHelper::GetWorldIdWithWorldPrefix(worldName);

					if (myGuildName.Compare(guildName) && targetWorldId == myWorldId)
					{
						return;
					}

					if (GGuild()->GetGuildGroupChatInfo()->IsMemberInGroup(roomId, guildName, targetWorldId))
					{
						FText findText;
						FText::FindText(TEXT("ChatText"), TEXT("Ticker_ChatGroup_InviteFail3"), findText);
						FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);

						return;
					}

					if (GGuild()->GetGuildGroupChatInfo()->IsInviteGuest(roomId, guildName, targetWorldId))
					{
						FText findText;
						FText::FindText(TEXT("ChatText"), TEXT("Ticker_ChatGroup_InviteFail4"), findText);
						FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);

						return;
					}


					FGsNetSendServiceGuild::SendReqGuildGroupChatInvite(roomId, guildName, targetWorldId);

					FText findText;
					FText::FindText(TEXT("ChatText"), TEXT("Ticker_ChatGroup_InviteSuccess"), findText);
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
				});
			}
		}
	}
}

void UGsUIChatGroupPageRoomInfo::OnClickLeave()
{
	// 나가기 창
	if (auto data = _curData.Pin())
	{
		auto roomId = data->GetRoomId();
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(NamePopupLeaveChatRoom);
		if (widget.IsValid())
		{
			if (UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get()))
			{
				FText Popup_ChatGroup_ExitOk;
				FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_ExitOk"), Popup_ChatGroup_ExitOk);
				FText groupTagName = FText::FromString(FString::Format(TEXT("<p>{0}</>"), { *data->GetGroupRoomName() }));

				FText desc = FText::Format(Popup_ChatGroup_ExitOk, groupTagName);

				popup->SetData(desc, [roomId](bool isOK) {
						if (isOK)
						{
							FGsNetSendServiceGuild::SendReqGuildGroupChatWithDraw(roomId);
						}
					});
			}
		}
	}
}

void UGsUIChatGroupPageRoomInfo::OnEntrustAuth(GuildDBId guildId)
{
	// 위임 창
	if (auto data = _curData.Pin())
	{
		if (auto guild = data->GetMember(guildId).Pin())
		{
			auto roomId = data->GetRoomId();
			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(NamePopupLeaveChatRoom);
			if (widget.IsValid())
			{
				if (UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get()))
				{
					FText Popup_ChatGroup_Entrust;
					FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_Entrust"), Popup_ChatGroup_Entrust);

					FText textPrefix = FGsContentsServerHelper::GetWorldIPrefixdWithWorldId(guild->_homePlanetWorldId.st.worldId);
					FText guildTagName = FText::FromString(FString::Format(TEXT("<p>{1}[{0}]</>"), { *textPrefix.ToString(), *(guild->_guildName) }));

					FText desc = FText::Format(Popup_ChatGroup_Entrust, guildTagName);

					popup->SetData(desc, [roomId, guildId](bool isOK) {
						if (isOK)
						{
							FGsNetSendServiceGuild::SendReqGuildGroupChatEntrustWithRoomMaster(roomId, guildId);
						}
						});
				}
			}
		}
	}
}

void UGsUIChatGroupPageRoomInfo::OnKickOutGuild(GuildDBId guildId)
{
	// 추방 창
	if (auto data = _curData.Pin())
	{
		if (auto guild = data->GetMember(guildId).Pin())
		{
			auto roomId = data->GetRoomId();
			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(NamePopupLeaveChatRoom);
			if (widget.IsValid())
			{
				if (UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get()))
				{
					FText Popup_ChatGroup_ExitOk;
					FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_Kick"), Popup_ChatGroup_ExitOk);

					FText textPrefix = FGsContentsServerHelper::GetWorldIPrefixdWithWorldId(guild->_homePlanetWorldId.st.worldId);
					FText guildTagName = FText::FromString(FString::Format(TEXT("<p>{1}[{0}]</>"), { *textPrefix.ToString(), *(guild->_guildName) }));

					FText desc = FText::Format(Popup_ChatGroup_ExitOk, guildTagName);

					popup->SetData(desc, [roomId, guildId](bool isOK) {
						if (isOK)
						{
							FGsNetSendServiceGuild::SendReqGuildGroupChatKickOut(roomId, guildId);
						}
						});
				}
			}
		}
	}
}

void UGsUIChatGroupPageRoomInfo::OnCancelInvitation(GuildDBId guildId)
{
	if (auto data = _curData.Pin())
	{
		FGsNetSendServiceGuild::SendReqGuildGroupChatCancelInvitation(data->GetRoomId(), guildId);
	}
	//바로 Send (packet lock)
}

void UGsUIChatGroupPageRoomInfo::OnSelectMemberTab()
{
	_selectList->SetActiveWidgetIndex(0);
	_memberGuildList->ClearListItems();
	if (auto data = _curData.Pin())
	{
		bool ctrlMember = data->IsRoomMasterAuthorized();

		TArray<TWeakPtr<FGsGuildGroupChatMemberGuild>> members;
		int count = data->GetMemberList(members);

		_textMemberCount = FText::FromString(FString::Format(TEXT("{0}/{1}"), { count, GGuild()->GetMaxGroupGuildCount() }));

		TArray<UGsUIChatGroupGuildData*> listItems;
		for (auto weakItem : members)
		{
			if (UGsUIChatGroupGuildData* listItem = NewObject<UGsUIChatGroupGuildData>())
			{
				listItem->SetEntryState(MemberState::Member);
				listItem->SetMasterGuildDiplomat(ctrlMember);
				listItem->SetItemData(weakItem);
				listItems.Add(listItem);
			}
		}
		_memberGuildList->SetListItems(listItems);
	}
}

void UGsUIChatGroupPageRoomInfo::OnSelectInvitationWaitingTab()
{
	_memberGuildList->ClearListItems();
	if (auto data = _curData.Pin())
	{
		bool ctrlMember = data->IsRoomMasterAuthorized();

		TArray<TWeakPtr<FGsGuildGroupChatMemberGuild>> guests;
		int count = data->GetInviteGuestList(guests);

		_textInvitingCount = FText::FromString(FString::Format(TEXT("{0}/{1}"), { count,  GGuild()->GetMaxGroupInvitationCount()}));

		TArray<UGsUIChatGroupGuildData*> listItems;
		for (auto weakItem : guests)
		{
			if (UGsUIChatGroupGuildData* listItem = NewObject<UGsUIChatGroupGuildData>())
			{
				listItem->SetEntryState(MemberState::InviteGuest);
				listItem->SetMasterGuildDiplomat(ctrlMember);
				listItem->SetItemData(weakItem);
				listItems.Add(listItem);
			}
		}
		_memberGuildList->SetListItems(listItems);

		_selectList->SetActiveWidgetIndex(listItems.Num() > 0 ? 0 : 1);
	}
}

void UGsUIChatGroupPageRoomInfo::SetEntryFunction(UGsUIChatGroupMemberEntry* entry)
{
	entry->OnEntrustMaster.BindUObject(this, &UGsUIChatGroupPageRoomInfo::OnEntrustAuth);
	entry->OnKickOut.BindUObject(this, &UGsUIChatGroupPageRoomInfo::OnKickOutGuild);
	entry->OnCancelInvitation.BindUObject(this, &UGsUIChatGroupPageRoomInfo::OnCancelInvitation);
}
