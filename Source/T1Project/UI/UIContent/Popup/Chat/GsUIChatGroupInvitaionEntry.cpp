// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIChatGroupInvitaionEntry.h"
#include "UI/UIContent/Popup/Chat/GsUIChatGroupInvitationData.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void UGsUIChatGroupInvitaionEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnAccept->OnClicked.AddDynamic(this, &UGsUIChatGroupInvitaionEntry::OnClickAccept);
	_btnReject->OnClicked.AddDynamic(this, &UGsUIChatGroupInvitaionEntry::OnClickReject);
}

void UGsUIChatGroupInvitaionEntry::NativeConstruct()
{
	Super::NativeConstruct();

	TWeakObjectPtr<UGsUIChatGroupInvitaionEntry> thiz = this;
	_btnSelect->SetToggleCallback([thiz](int32 index, bool select) {
		if (thiz.IsValid()) thiz->SetIsSelected(select);
		});
}

void UGsUIChatGroupInvitaionEntry::NativeDestruct()
{
	_btnSelect->SetToggleCallback(nullptr);

	Super::NativeDestruct();
}

void UGsUIChatGroupInvitaionEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	_data = Cast<UGsUIChatGroupInvitationData>(ListItemObject); // casting 실패이면 null로 저장

	RefreshUI();
}

void UGsUIChatGroupInvitaionEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	Super::NativeOnItemSelectionChanged(bIsSelected);

	_btnSelect->SetIsSelected(bIsSelected);
}

void UGsUIChatGroupInvitaionEntry::OnClickAccept()
{
	if (auto data = Cast<UGsUIChatGroupInvitationData>(_data))
	{
		OnResponseInvitation.ExecuteIfBound(data->GetGroupChatRoomId(), true);
	}
}

void UGsUIChatGroupInvitaionEntry::OnClickReject()
{
	if (auto data = Cast<UGsUIChatGroupInvitationData>(_data))
	{
		OnResponseInvitation.ExecuteIfBound(data->GetGroupChatRoomId(), false);
	}
}

void UGsUIChatGroupInvitaionEntry::RefreshUI()
{
	_chatRoomName = FText::GetEmpty();
	_guildName = FText::GetEmpty();
	_guildMasterName = FText::GetEmpty();

	if (auto data = Cast<UGsUIChatGroupInvitationData>(_data))
	{
		_chatRoomName = data->GetGroupChatRoomName();
		_guildName = data->GetGuildName();
		_guildMasterName = data->GetGuildMasterName();

		if (data->GetEmblemId() != INVALID_GUILD_EMBLEM_ID)
		{
			if (const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(data->GetEmblemId()))
			{
				if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(emblemData->iconPathBig))
				{
					_emblem.SetResourceObject(loadObject);
				}
			}
		}
		_btnSelect->SetIsSelected(false);
	}
}
