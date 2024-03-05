// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIChatGroupPageInvitation.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildGroupChatData.h"
#include "Guild/GsGuildHelper.h"
#include "UI/UIContent/Popup/Chat/GsUIChatGroupInvitationData.h"
#include "UI/UIContent/Popup/Chat/GsUIChatGroupInvitaionEntry.h"
#include "UI/UIControlLib/ContentWidget/GsScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Net/GsNetSendServiceGuild.h"

void UGsUIChatGroupPageInvitation::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_invitationList->InitializeData();
	_invitationList->OnEntryWidgetGenerated().AddLambda([this](UUserObjectScrollEntry& entry) {
		if (UGsUIChatGroupInvitaionEntry* invitation = Cast<UGsUIChatGroupInvitaionEntry>(&entry))
		{
			invitation->OnResponseInvitation.BindUObject(this, &UGsUIChatGroupPageInvitation::OnResponseInvitation);
		}
		});

	_invitationList->OnEntryWidgetReleased().AddLambda([](UUserObjectScrollEntry& entry) {
		if (UGsUIChatGroupInvitaionEntry* invitation = Cast<UGsUIChatGroupInvitaionEntry>(&entry))
		{
			invitation->OnResponseInvitation.Unbind();
		}
		});
}

void UGsUIChatGroupPageInvitation::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIChatGroupPageInvitation::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIChatGroupPageInvitation::InitializeData()
{
	_invitations.Reset();
	GGuild()->GetGuildGroupChatInfo()->GetInvitationsToGroupChat(_invitations);

	TArray<UGsUIChatGroupInvitationData*> listItems;
	for (auto weakItem : _invitations)
	{
		if (UGsUIChatGroupInvitationData* listItem = NewObject<UGsUIChatGroupInvitationData>())
		{
			listItem->SetItemData(weakItem);
			listItems.Add(listItem);
		}
	}
	_invitationList->SetListItems(listItems);

	RefreshUI();
}

void UGsUIChatGroupPageInvitation::RemoveItem(GuildGroupChatRoomId roomId)
{
	_invitations.RemoveAll([roomId](auto e) { return e.IsValid() && e.Pin()->GetRoomId() == roomId; });

	const TArray<UObject*> listItems = _invitationList->GetListItems();

	if (auto item = listItems.FindByPredicate([](auto e) { 
		if (auto data = Cast<UGsUIChatGroupInvitationData>(e)) return data->GetItemData().IsValid() == false; return false; }))
	{
		_invitationList->RemoveItem(*item);
	}

	RefreshUI();
}

void UGsUIChatGroupPageInvitation::OnResponseInvitation(GuildGroupChatRoomId roomId, bool accepted)
{
	if (accepted)
	{
		FGsNetSendServiceGuild::SendReqGuildGroupChatInvitationAccept(roomId);
	}
	else
	{
		FGsNetSendServiceGuild::SendReqGuildGroupChatInvitationReject(roomId);
	}
}

void UGsUIChatGroupPageInvitation::RefreshUI()
{
	_textInvitedCount = FText::FromString(FString::Format(TEXT("{0}/{1}"), { _invitations.Num(), 50}));
	_selectList->SetActiveWidgetIndex(_invitations.Num() > 0 ? 0 : 1);
}
