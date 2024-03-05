// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildMemberListEntry.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Popup/GsUIPopupChatUserInfo.h"
#include "UI/UIContent/Popup/GsUIPopupGuildChangeMemberGrade.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "Guild/GsGuildMemberData.h"
#include "Guild/GsGuildHelper.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Net/GsNetSendServiceGuild.h"


void UGsUIGuildMemberListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIGuildMemberListEntry::OnClickSlot);
	_btnName->OnClicked.AddDynamic(this, &UGsUIGuildMemberListEntry::OnClickName);
	_btnAppointment->OnClicked.AddDynamic(this, &UGsUIGuildMemberListEntry::OnClickAppointment);
	_btnBan->OnClicked.AddDynamic(this, &UGsUIGuildMemberListEntry::OnClickBan);
}

void UGsUIGuildMemberListEntry::OnHide()
{
	_memberData = nullptr;
}

void UGsUIGuildMemberListEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_imgSelected->SetVisibility(_bIsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (_bIsSelected)
	{
		if (false == IsMyData())
		{
			// 권한이 있는 유저만 임명/추방 버튼 볼 수 있음
			uint32 myAuthority = GGuild()->GetAuthority();
			if (FGsGuildHelper::CanManageMember(myAuthority) ||
				FGsGuildHelper::CanManageMember(myAuthority))
			{
				SetUIManageMode(true);
				return;
			}
		}
	}

	SetUIManageMode(false);
}

void UGsUIGuildMemberListEntry::SetData(const FGsGuildMemberData* InData)
{
	_memberData = InData;

	_textBlockLevel->SetText(FText::FromString(FString::Format(TEXT("Lv.{0}"), { InData->_level })));
	_textBlockName->SetText(FText::FromString(InData->_userName));

	FText textGrade;
	if (false == FGsGuildHelper::GetMemberGradeText(InData->_memberGrade, textGrade))
	{
		textGrade = FText::GetEmpty();		
	}
	_textBlockGrade->SetText(textGrade);

	if (0 == InData->_lastLoginTimeOriginal)
	{
		// 접속중 표시
		_switcherState->SetActiveWidgetIndex(0);
	}
	else
	{
		// 시간 표시
		_switcherState->SetActiveWidgetIndex(1);

		FString strTime;
		FGsTimeStringHelper::GetElapsedTime(InData->_lastLoginTimeOriginal, strTime);
		_textBlockState->SetText(FText::FromString(strTime));
	}

	_textBlockWeeklyContribution->SetText(FText::AsNumber(InData->_weeklyContribution));
	_textBlockAccumContribution->SetText(FText::AsNumber(InData->_accumContribution));

	_imgAttendance->SetVisibility(InData->_attendance ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	SetIsSelected(_bIsSelected);
}

void UGsUIGuildMemberListEntry::OnClickSlot()
{
	// ToggleGroup에 콜백 전달. 스왑을 위해 현재 상태에서 바꿔서 전달
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

void UGsUIGuildMemberListEntry::OnClickName()
{
	if (IsMyData())
		return;

	if (_memberData)
	{
		OnClickUserName(_memberData->_userDBId, _btnName);
		TWeakObjectPtr<UGsUIPopupChatUserInfo> contextPopup = Cast<UGsUIPopupChatUserInfo>(GUI()->OpenAndGetWidget(TEXT("PopupChatUserInfo")));
		if (contextPopup.IsValid())
		{
			UGsUIPopupChatUserInfo::PopupInitData initData{ _memberData->_userDBId, _memberData->_userName, TEXT(""), true, EGsUserContentsOwner::GUILD_MEMBER };
			contextPopup->SetData(&initData, true);
		}
	}
}

void UGsUIGuildMemberListEntry::SetUIManageMode(bool bManageMode)
{
	_switcherManage->SetActiveWidgetIndex(bManageMode ? 1 : 0);

	if (bManageMode)
	{
		if (_memberData)
		{
			FGsGuildManager* guildMgr = GGuild();

			uint32 myAuthority = guildMgr->GetAuthority();
			GuildMemberGradeType myGrade = guildMgr->GetMemeberGrade();

			// 나보다 권한이 낮은 유저일 경우 권한체크하여 버튼 활성화
			if (myGrade > _memberData->_memberGrade)
			{
				_btnAppointment->SetIsEnabled(FGsGuildHelper::CanManageMember(myAuthority));
				_btnBan->SetIsEnabled(FGsGuildHelper::CanManageMember(myAuthority));
			}
			else
			{
				// 나보다 등급이 높거나 같은 유저는 불가
				_btnAppointment->SetIsEnabled(false);
				_btnBan->SetIsEnabled(false);
			}
		}
	}
}

void UGsUIGuildMemberListEntry::OnClickAppointment()
{
	if (nullptr == _memberData)
	{
		return;
	}

	if (IsMyData())
	{
		return;
	}

	FGsGuildManager* guildMgr = GGuild();

	// 등급 체크(나보다 낮은 등급만 가능)
	GuildMemberGradeType myGrade = guildMgr->GetMemeberGrade();
	if (myGrade <= _memberData->_memberGrade)
	{
		return;
	}

	// 권한 체크
	if (false == FGsGuildHelper::CanManageMember(guildMgr->GetAuthority()))
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildChangeMemberGrade"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildChangeMemberGrade* popup = Cast<UGsUIPopupGuildChangeMemberGrade>(widget.Get()))
		{
			popup->SetData(_memberData);
		}
	}
}

void UGsUIGuildMemberListEntry::OnClickBan()
{
	if (nullptr == _memberData)
	{
		return;
	}

	if (IsMyData())
	{
		return;
	}

	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::GUILD_MEMBER_BAN, true))
	{	// 2023/12/20 PKT - 그림자 전장에서는 멤버 추방을 할수가 없음.
		return;
	}

	FGsGuildManager* guildMgr = GGuild();

	// 등급 체크(나보다 낮은 등급만 가능)
	GuildMemberGradeType myGrade = guildMgr->GetMemeberGrade();
	if (myGrade <= _memberData->_memberGrade)
	{
		return;
	}

	// 권한 체크
	if (false == FGsGuildHelper::CanManageMember(guildMgr->GetAuthority()))
	{
		return;
	}

	// TEXT: {0} 님을 추방하시겠습니까?
	FText textFormat;
	FText textMsg;
	if (FText::FindText(TEXT("GuildText"), TEXT("UI_KickOut_Check"), textFormat))
	{
		textMsg = FText::Format(textFormat, FText::FromString(_memberData->_userName));
	}

	UserDBId userDBId = _memberData->_userDBId;

	FGsUIHelper::PopupYesNo(textMsg, [userDBId](bool bIsOk)
		{
			if (bIsOk)
			{
				FGsNetSendServiceGuild::SendReqBanMember(userDBId);
			}
		});
}

bool UGsUIGuildMemberListEntry::IsMyData() const
{
	if (_memberData)
	{
		if (const FGsNetUserData* userData = GGameData()->GetUserData())
		{
			if (_memberData->_userDBId == userData->mUserDBId)
			{
				return true;
			}
		}
	}
	
	return false;
}
