// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildApproveJoinEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Guild/GsGuildData.h"
#include "Guild/GsGuildHelper.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Net/GsNetSendServiceGuild.h"


void UGsUIGuildApproveJoinEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnApprove->OnClicked.AddDynamic(this, &UGsUIGuildApproveJoinEntry::OnClickApprove);
	_btnReject->OnClicked.AddDynamic(this, &UGsUIGuildApproveJoinEntry::OnClickReject);
}

void UGsUIGuildApproveJoinEntry::OnHide()
{
	_userDBId = 0;
}

void UGsUIGuildApproveJoinEntry::SetData(const FGsGuildJoinWaitingUser& InData)
{
	_userDBId = InData._userDBId;

	_textBlockLevel->SetText(FText::AsNumber(InData._level));
	_textBlockName->SetText(FText::FromString(InData._userName));
}

bool UGsUIGuildApproveJoinEntry::CheckAuthority()
{
	if (FGsGuildHelper::CanManageMember(GGuild()->GetAuthority()))
	{
		return true;
	}

	// TEXT: 권한이 없습니다.
	FText textMsg;
	FText::FindText(TEXT("GuildText"), TEXT("Alert_NoAuthority"), textMsg);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

	return false;
}

void UGsUIGuildApproveJoinEntry::OnClickApprove()
{
	if (CheckAuthority())
	{
		if (0 < _userDBId)
		{
			FGsNetSendServiceGuild::SendReqJoinApprove(_userDBId);
		}
	}
}

void UGsUIGuildApproveJoinEntry::OnClickReject()
{
	if (CheckAuthority())
	{
		if (0 < _userDBId)
		{
			FGsNetSendServiceGuild::SendReqJoinReject(_userDBId);
		}
	}
}
