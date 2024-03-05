// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildAuthority.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Popup/Guild/GsUIGuildAuthorityEntry.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildHelper.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Net/GsNetSendServiceGuild.h"


void UGsUIPopupGuildAuthority::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnChange->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuthority::OnClickChange);
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuthority::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuthority::OnClickCancel);
}

void UGsUIPopupGuildAuthority::NativeConstruct()
{
	Super::NativeConstruct();

	_msgHandleGuild = GMessage()->GetGuild().AddUObject(MessageContentGuild::RECEIVE_CHANGE_AUTHORITY,
		this, &UGsUIPopupGuildAuthority::OnReceiveChangeAuthority);

	_entryList.Empty();
	TArray<UWidget*> childList =_panelListRoot->GetAllChildren();
	for (int32 i = 0; i < childList.Num(); ++i)
	{
		_entryList.Emplace(Cast<UGsUIGuildAuthorityEntry>(childList[i]));
	}

	SetEditableMode(false);

	InvalidateData();
}

void UGsUIPopupGuildAuthority::NativeDestruct()
{
	GMessage()->GetGuild().Remove(_msgHandleGuild);

	_entryList.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupGuildAuthority::OnInputOk()
{
	TArray<GuildRightsLevel> authorityList;

	// 순서가 중요하다 주의
	for (int32 i = 0; i < _entryList.Num(); ++i)
	{
		authorityList.Emplace(_entryList[i]->GetAuthority());
	}

	GGuild()->SendRequestChangeAuthority(authorityList);

	SetEditableMode(false);
}

void UGsUIPopupGuildAuthority::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildAuthority::OnClickChange()
{
	// TODO: 내 등급 체크
	SetEditableMode(true);
}

void UGsUIPopupGuildAuthority::OnReceiveChangeAuthority(const IGsMessageParam*)
{
	InvalidateData();
}

void UGsUIPopupGuildAuthority::OnClickOk()
{
	OnInputOk();
}

void UGsUIPopupGuildAuthority::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupGuildAuthority::InvalidateData()
{
	FGsGuildManager* guildMgr = GGuild();
	
	// 순서가 중요하다 주의
	uint8 gradeIndex = static_cast<uint8>(GuildMemberGradeType::MAX) - 1;
	for (int32 i = 0; i < _entryList.Num(); ++i)
	{
		if (_entryList[i])
		{
			GuildMemberGradeType memberGrade = static_cast<GuildMemberGradeType>(gradeIndex);
			GuildRightsLevel authority = guildMgr->GetAuthorityByMemberGrade(memberGrade);
						
			_entryList[i]->SetData(memberGrade, authority);
			
			--gradeIndex;
		}
	}
}

void UGsUIPopupGuildAuthority::SetEditableMode(bool bInEditable)
{
	_bIsEditableMode = bInEditable;

	if (_bIsEditableMode)
	{
		_btnChange->SetVisibility(ESlateVisibility::Collapsed);
		_btnOk->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_btnChange->SetVisibility(ESlateVisibility::Visible);
		_btnOk->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int32 i = 0; i < _entryList.Num(); ++i)
	{
		if (_entryList[i])
		{
			_entryList[i]->SetEditableMode(bInEditable);
		}
	}
}
