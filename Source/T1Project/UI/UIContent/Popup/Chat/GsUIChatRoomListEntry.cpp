// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIChatRoomListEntry.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Guild/GsGuildGroupChatData.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/Chat/GsUIChatGroupRoomData.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "Components/WidgetSwitcher.h"

void UGsUIChatRoomListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	_btnCreateRoom->OnClicked.AddDynamic(this, &UGsUIChatRoomListEntry::OnClickCreateRoom);
	_btnRecvInvitation->OnClicked.AddDynamic(this, &UGsUIChatRoomListEntry::OnClickRecvInvitation);
	_btnGroupEdit->OnClicked.AddDynamic(this, &UGsUIChatRoomListEntry::OnClickEditGroup);
	_btnRoomInfo->OnSwitcherButtonSelected.AddDynamic(this, &UGsUIChatRoomListEntry::OnClickReqInfoRoom);
}

void UGsUIChatRoomListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	TWeakObjectPtr<UGsUIChatRoomListEntry> thiz = this;
	_btnRoomInfo->SetToggleCallback([thiz](int32 index, bool select) {
			if(thiz.IsValid()) thiz->SetIsSelected(select);
		});
}

void UGsUIChatRoomListEntry::NativeDestruct()
{
	_btnRoomInfo->SetToggleCallback(nullptr);

	Super::NativeDestruct();
}

void UGsUIChatRoomListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	_data = Cast<UGsUIChatGroupRoomData>(ListItemObject); // casting 실패이면 null로 저장

	RefreshUI();
}

void UGsUIChatRoomListEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	Super::NativeOnItemSelectionChanged(bIsSelected);

	_btnRoomInfo->SetIsSelected(bIsSelected);
}

void UGsUIChatRoomListEntry::RefreshUI()
{
	_groupName = FText::GetEmpty();
	_memberCount = FText::GetEmpty();
	_switchGroup->SetActiveWidgetIndex(1);
	_btnRoomInfo->SetIsEnabled(false);
	_btnGroupEdit->SetVisibility(ESlateVisibility::Collapsed);
	_invitaionPanel->SetVisibility(ESlateVisibility::Collapsed);

	if (auto data = Cast<UGsUIChatGroupRoomData>(_data))
	{
		_switchGroup->SetActiveWidgetIndex(data->IsValidGroup() ? 0 : 1);

		_groupName = data->GetGroupRoomName();

		FText UI_ChatGroup_JoinNumber;
		FText::FindText(TEXT("ChatText"), TEXT("UI_ChatGroup_JoinNumber"), UI_ChatGroup_JoinNumber);

		_memberCount = FText::Format(UI_ChatGroup_JoinNumber, data->GetMemberCount(), GGuild()->GetMaxGroupGuildCount() );

		_btnRoomInfo->SetIsEnabled(data->IsValidGroup());
		_btnRoomInfo->SetIsSelected(false);

		bool editAuth = data->IsValidGroup() && data->IsRoomMasterAuthorized();
		//bool createAuth = !data->IsValidGroup() && data->IsRoomMasterAuthorized();
		bool invitedAuth = data->IsWaitingRoom() && data->IsRoomMasterAuthorized();

		//_btnGroupEdit->SetIsEnabled(editAuth);
		//_btnGroupEdit->SetVisibility(editAuth ? ESlateVisibility::Visible : ESlateVisibility::Collapsed );

		_newRoomRedDot->SetVisibility(data->IsNewGroupMember() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		_invitationRedDot->SetVisibility(invitedAuth && data->IsInvitations() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		_invitaionPanel->SetVisibility(invitedAuth && data->IsInvitations() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		_btnRecvInvitation->SetIsEnabled(invitedAuth && data->IsInvitations());
	}
}

void UGsUIChatRoomListEntry::OnClickCreateRoom()
{
	if (auto data = Cast<UGsUIChatGroupRoomData>(_data))
	{
		if (data->IsValidGroup() || !data->IsRoomMasterAuthorized())
		{
			//"기사단 외교 권한이 없어 개설할 수 없습니다.” 
			FText Ticker_ChatCreate_NoPower;
			FText::FindText(TEXT("ChatText"), TEXT("Ticker_ChatCreate_NoPower"), Ticker_ChatCreate_NoPower);

			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, Ticker_ChatCreate_NoPower);
			return;
		}

		OnRoomCreate.ExecuteIfBound();
	}
}

void UGsUIChatRoomListEntry::OnClickRecvInvitation()
{
	OnRecvInvitation.ExecuteIfBound();
}

void UGsUIChatRoomListEntry::OnClickReqInfoRoom(bool set)
{
	if (set)
	{
		if (auto data = Cast<UGsUIChatGroupRoomData>(_data))
		{
			OnSetRoomInfo.ExecuteIfBound(data->GetItemData());
		}
	}
}

void UGsUIChatRoomListEntry::OnClickEditGroup()
{
	if (auto data = Cast<UGsUIChatGroupRoomData>(_data))
	{
		OnEditRoomInfo.ExecuteIfBound(data->GetItemData());
	}
}

