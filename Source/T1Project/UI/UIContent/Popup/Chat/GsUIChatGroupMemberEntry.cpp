// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/Chat/GsUIChatGroupMemberEntry.h"
#include "UI/UIContent/Popup/Chat/GsUIChatGroupGuildData.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void UGsUIChatGroupMemberEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnEntrust->OnClicked.AddDynamic(this, &UGsUIChatGroupMemberEntry::OnClickEntrust);
	_btnKickOut->OnClicked.AddDynamic(this, &UGsUIChatGroupMemberEntry::OnClickKickOut);
	_btnCancelInvite->OnClicked.AddDynamic(this, &UGsUIChatGroupMemberEntry::OnClickInvitationCancel);
}

void UGsUIChatGroupMemberEntry::NativeConstruct()
{
	Super::NativeConstruct();
	TWeakObjectPtr<UGsUIChatGroupMemberEntry> thiz = this;
	_btnSelect->SetToggleCallback([thiz](int32 index, bool select) {
		if (thiz.IsValid()) thiz->SetIsSelected(select);
	});
}

void UGsUIChatGroupMemberEntry::NativeDestruct()
{
	_btnSelect->SetToggleCallback(nullptr);
	Super::NativeDestruct();
}

void UGsUIChatGroupMemberEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	_data = Cast<UGsUIChatGroupGuildData>(ListItemObject); // casting 실패이면 null로 저장

	RefreshUI();
}

void UGsUIChatGroupMemberEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	Super::NativeOnItemSelectionChanged(bIsSelected);

	_btnSelect->SetIsSelected(bIsSelected);
}

void UGsUIChatGroupMemberEntry::OnClickEntrust()
{
	if (auto data = Cast<UGsUIChatGroupGuildData>(_data))
	{
		OnEntrustMaster.ExecuteIfBound(data->GetGuildDBId());
	}
}

void UGsUIChatGroupMemberEntry::OnClickKickOut()
{
	if (auto data = Cast<UGsUIChatGroupGuildData>(_data))
	{
		OnKickOut.ExecuteIfBound(data->GetGuildDBId());
	}
}

void UGsUIChatGroupMemberEntry::OnClickInvitationCancel()
{
	if (auto data = Cast<UGsUIChatGroupGuildData>(_data))
	{
		OnCancelInvitation.ExecuteIfBound(data->GetGuildDBId());
	}
}

void UGsUIChatGroupMemberEntry::RefreshUI()
{
	_iconAdmin->SetVisibility(ESlateVisibility::Collapsed);

	_guildName = FText::GetEmpty();
	_guildMasterName = FText::GetEmpty();
	_authSelect->SetActiveWidgetIndex(1);
	_selectAction->SetVisibility(ESlateVisibility::Collapsed);
	_selectAction->SetActiveWidgetIndex(0);
	_btnSelect->SetIsEnabled(false);
	_btnEntrust->SetIsEnabled(false);
	_btnKickOut->SetIsEnabled(false);
	_btnCancelInvite->SetIsEnabled(false);

	if (auto data = Cast<UGsUIChatGroupGuildData>(_data))
	{
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

		bool isControlable = data->IsAuthorizedMember() && data->IsMasterGuild() == false;
		_authSelect->SetActiveWidgetIndex(isControlable ? 0 : 1);
		_btnSelect->SetIsEnabled(isControlable);
		_btnSelect->SetIsSelected(false);

		_selectAction->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_selectAction->SetActiveWidgetIndex(static_cast<int>(data->GetEntryState()));
		
		switch (data->GetEntryState())
		{
		case MemberState::Member	 : RefreshMemberUI(data, isControlable); break;
		case MemberState::InviteGuest: RefreshInvitationUI(data, isControlable); break;
		default: break;
		}
	}
}

void UGsUIChatGroupMemberEntry::RefreshMemberUI(UGsUIChatGroupGuildData* data, bool isControlable /*= false*/)
{
	_iconAdmin->SetVisibility(data->IsMasterGuild() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	_btnEntrust->SetIsEnabled(isControlable);
	_btnKickOut->SetIsEnabled(isControlable);
}

void UGsUIChatGroupMemberEntry::RefreshInvitationUI(UGsUIChatGroupGuildData* data, bool isControlable /*= false*/)
{
	_btnCancelInvite->SetIsEnabled(isControlable);
}
