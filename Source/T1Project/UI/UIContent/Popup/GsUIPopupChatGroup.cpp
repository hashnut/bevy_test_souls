// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupChatGroup.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsScrollBox.h"
#include "UI/UIContent/Popup/Chat/GsUIChatGroupPageRoomInfo.h"
#include "UI/UIContent/Popup/Chat/GsUIChatGroupPageInvitation.h"
#include "UI/UIContent/Popup/Chat/GsUIChatGroupRoomData.h"
#include "UI/UIContent/Popup/Chat/GsUIChatRoomListEntry.h"
#include "UI/UIContent/Popup/GsUIPopupCommonInput.h"
#include "Net/GsNetSendServiceGuild.h"
#include "../Message/MessageParam/GsUIMessageParam.h"

FName NamePopupCreateRoom = TEXT("PopupCommonInput");
void UGsUIPopupChatGroup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TWeakObjectPtr<UGsUIPopupChatGroup> thiz = this;

	_chatRoomList->InitializeData();
	_chatRoomList->OnEntryWidgetGenerated().AddLambda([thiz](UUserObjectScrollEntry& entry) {
			if (UGsUIChatRoomListEntry* roomEntry = Cast<UGsUIChatRoomListEntry>(&entry))
			{
				if (thiz.IsValid())
				{
					thiz->SetEntryFunction(roomEntry);
				}
			}
		});

	_chatRoomList->OnEntryWidgetReleased().AddLambda([](UUserObjectScrollEntry& entry) {
			if (UGsUIChatRoomListEntry* roomEntry = Cast<UGsUIChatRoomListEntry>(&entry))
			{
				roomEntry->OnRoomCreate.Unbind();
				roomEntry->OnRecvInvitation.Unbind();
				roomEntry->OnSetRoomInfo.Unbind();
				roomEntry->OnEditRoomInfo.Unbind();
			}
		});
}

void UGsUIPopupChatGroup::NativeConstruct()
{
	Super::NativeConstruct();

	FGsMessageHolder* msgHolder = GMessage();

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_LIST, this, &UGsUIPopupChatGroup::OnUpdateChatGroupInfo));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_ADD, this, &UGsUIPopupChatGroup::OnUpdateChatGroupInfo));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_INVALIDATE, this, &UGsUIPopupChatGroup::OnUpdateChatGroupInfo));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_REMOVE, this, &UGsUIPopupChatGroup::OnUpdateChatGroupInfo));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_INVITATION_LIST, this, &UGsUIPopupChatGroup::OnUpdateChatGroupInvitaion));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_INVITATION_REMOVE, this, &UGsUIPopupChatGroup::OnRemoveChatGroupInvitaion));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_MEMBER_LIST, this, &UGsUIPopupChatGroup::OnUpdateChatGroupMemberInfo));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_MEMBER_REMOVE, this, &UGsUIPopupChatGroup::OnUpdateChatGroupMemberInfo));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_GUEST_LIST, this, &UGsUIPopupChatGroup::OnUpdateChatGroupGuestInfo));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_GUEST_REMOVE, this, &UGsUIPopupChatGroup::OnUpdateChatGroupGuestInfo));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_ROOMNAME_CHANGED, this, &UGsUIPopupChatGroup::OnUpdateChatGroupNameChanged));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_ROOMMASTER_ENTRUSTED, this, &UGsUIPopupChatGroup::OnUpdateChatGroupEntrusted));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_ROOMAUTHGRADE_CHANGED, this, &UGsUIPopupChatGroup::OnUpdateChatGroupAuthGrade));

	_detailSelect->SetActiveWidgetIndex(0);
	_roomList.Reset();

	int maxCount = GGuild()->GetGuildGroupChatInfo()->GetAllGroupRooms(_roomList);
	int curCount = _roomList.FilterByPredicate([](auto e) { return e.IsValid() && e.Pin()->IsValidGroup(); }).Num();
	_textRoomCount = FText::FromString(FString::Format(TEXT("{0}/{1}"), { curCount, maxCount }));

	TArray<UGsUIChatGroupRoomData*> listItems;
	for (auto weakItem : _roomList)
	{
		if (UGsUIChatGroupRoomData* listItem = NewObject<UGsUIChatGroupRoomData>())
		{
			listItem->SetItemData(weakItem);
			listItems.Add(listItem);
		}
	}
	_chatRoomList->SetListItems(listItems);

	_selectList->SetActiveWidgetIndex(listItems.Num() > 0 ? 0 : 1);

	_detailSelect->SetActiveWidgetIndex(0);
	_roomInfo->InitializeData(nullptr);
}

void UGsUIPopupChatGroup::NativeDestruct()
{
	for (auto weakItem : _roomList)
	{
		if (auto room = weakItem.Pin())
		{
			if (room->GetAuthorizedGrade() != room->GetChangingAuthGrade())
			{
				FGsNetSendServiceGuild::SendReqGuildGroupChatChangeAuthorizedGrade(room->GetRoomId(), room->GetChangingAuthGrade());
			}
		}
	}

	for (auto& e : _chatMessageHandlers)
		GMessage()->GetChat().Remove(e);

	_chatMessageHandlers.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupChatGroup::OnUpdateChatGroupInfo(const IGsMessageParam* param)
{
	_detailSelect->SetActiveWidgetIndex(0);
	_roomList.Reset();

	GuildGroupChatRoomId roomId = INVALID_GUILD_GROUP_CHAT_ROOM_ID;
	if (auto selectData = _selectData.Pin())
	{
		roomId = selectData->GetRoomId();
	}

	int maxCount = GGuild()->GetGuildGroupChatInfo()->GetAllGroupRooms(_roomList);
	int curCount = _roomList.FilterByPredicate([](auto e) { return e.IsValid() && e.Pin()->IsValidGroup(); }).Num();
	_textRoomCount = FText::FromString(FString::Format(TEXT("{0}/{1}"), { curCount, maxCount }));

	auto index = _roomList.IndexOfByPredicate([roomId](auto e) { return e.IsValid() && e.Pin()->GetRoomId() == roomId; });

	TArray<UGsUIChatGroupRoomData*> listItems;
	for (auto weakItem : _roomList)
	{
		if (UGsUIChatGroupRoomData* listItem = NewObject<UGsUIChatGroupRoomData>())
		{
			listItem->SetItemData(weakItem);
			listItems.Add(listItem);
		}
	}
	_chatRoomList->SetListItems(listItems);
	if (index != INDEX_NONE)
	{
		auto curIndex = _detailSelect->GetActiveWidgetIndex();

		_chatRoomList->SetSelectIndex(index);
		_detailSelect->SetActiveWidgetIndex(curIndex);
	}
}

void UGsUIPopupChatGroup::OnUpdateChatGroupMemberInfo(const IGsMessageParam* param)
{
	_roomInfo->RefreshUI();
}

void UGsUIPopupChatGroup::OnUpdateChatGroupGuestInfo(const IGsMessageParam* param)
{
	_roomInfo->RefreshUI();
}

void UGsUIPopupChatGroup::OnUpdateChatGroupInvitaion(const IGsMessageParam* param)
{
	_invitaionList->InitializeData();
}

void UGsUIPopupChatGroup::OnRemoveChatGroupInvitaion(const IGsMessageParam* param)
{
	if (const FGsUIMsgParamChatGroup* chatParam = param ? param->Cast<const FGsUIMsgParamChatGroup>() : nullptr)
	{
		_invitaionList->RemoveItem(chatParam->_roomId);
	}
	else
	{
		_invitaionList->InitializeData();
	}
}

void UGsUIPopupChatGroup::OnUpdateChatGroupEntrusted(const IGsMessageParam* param)
{
	_roomInfo->RefreshUI();
}

void UGsUIPopupChatGroup::OnUpdateChatGroupNameChanged(const IGsMessageParam* param)
{
	_chatRoomList->RefreshAll();
}

void UGsUIPopupChatGroup::OnUpdateChatGroupAuthGrade(const IGsMessageParam* param)
{
	_roomInfo->RefreshUI();
}

void UGsUIPopupChatGroup::OnCloseExtra()
{
	Close();
}

void UGsUIPopupChatGroup::OnCreateChatGroup()
{
	// 만들기 창
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(NamePopupCreateRoom);
	if (widget.IsValid())
	{
		if (UGsUIPopupCommonInput* popup = Cast<UGsUIPopupCommonInput>(widget.Get()))
		{
			FText title;
			FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_Create"), title);

			FText desc;
			FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_RoomName"), desc);

			FText hint;
			FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_Input"), hint);

			popup->SetDescText(title);
			popup->SetWarningText(desc);
			popup->SetHintText(hint);
			popup->SetMaxTextLength(MAX_GUILD_NAME_LEN, true);
			popup->SetCallbackFunc([](const FString& roomName){
				FGsNetSendServiceGuild::SendReqGuildGroupChatRoomCreate(roomName);
			});
		}
	}
}

void UGsUIPopupChatGroup::OnPopupInvitationList()
{
	_detailSelect->SetActiveWidgetIndex(1);
	_invitaionList->InitializeData();
}

void UGsUIPopupChatGroup::OnReqRoomInfo(TWeakPtr<FGsGuildGroupChatRoom> data)
{
	_selectData = data;

	_detailSelect->SetActiveWidgetIndex(0);
	_roomInfo->InitializeData(data);
}

void UGsUIPopupChatGroup::OnEditRoomName(TWeakPtr<FGsGuildGroupChatRoom> data)
{
	FString roomName;
	GuildGroupChatRoomId roomId = INVALID_GUILD_GROUP_CHAT_ROOM_ID;
	if (auto roomData = data.Pin())
	{
		roomId = roomData->GetRoomId();
		roomName = roomData->GetGroupRoomName();
	}

	if (roomId == INVALID_GUILD_GROUP_CHAT_ROOM_ID) return;

	// edit 창
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(NamePopupCreateRoom);
	if (widget.IsValid())
	{
		if (UGsUIPopupCommonInput* popup = Cast<UGsUIPopupCommonInput>(widget.Get()))
		{
			FText title;
			FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_ChatRoomChange"), title);

			FText desc;
			FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_RoomName"), desc);

			FText hint;
			FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_Input"), hint);

			popup->SetDescText(title);
			popup->SetText(FText::FromString(roomName));
			popup->SetWarningText(desc);
			popup->SetHintText(hint);
			popup->SetMaxTextLength(MAX_GUILD_NAME_LEN, true);
			popup->SetCallbackFunc([](const FString& roomName) {
				//FGsNetSendServiceGuild::SendReqGuildGroupChatRoomCreate(roomName);
			});
		}
	}
}

void UGsUIPopupChatGroup::SetEntryFunction(UGsUIChatRoomListEntry* roomEntry)
{
	roomEntry->OnRoomCreate.BindUObject(this, &UGsUIPopupChatGroup::OnCreateChatGroup);
	roomEntry->OnRecvInvitation.BindUObject(this, &UGsUIPopupChatGroup::OnPopupInvitationList);
	roomEntry->OnSetRoomInfo.BindUObject(this, &UGsUIPopupChatGroup::OnReqRoomInfo);
	roomEntry->OnEditRoomInfo.BindUObject(this, &UGsUIPopupChatGroup::OnEditRoomName);
}

void UGsUIPopupChatGroup::ResetEntryFunction(UGsUIChatRoomListEntry* roomEntry)
{
	roomEntry->OnRoomCreate.Unbind();
	roomEntry->OnRecvInvitation.Unbind();
	roomEntry->OnSetRoomInfo.Unbind();
	roomEntry->OnEditRoomInfo.Unbind();
}