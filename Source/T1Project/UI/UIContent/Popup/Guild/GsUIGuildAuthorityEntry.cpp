// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildAuthorityEntry.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "Guild/GsGuildHelper.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"


//----------------------------------------------------------------------------------------------------------------------
void UGsUIGuildAuthorityEntry::FGsAuthoritySlot::InitCheckBox()
{
	_checkBoxUneditable = Cast<UGsCheckBox>(_switcher->GetChildAt(0));
	_checkBoxEditable = Cast<UGsCheckBox>(_switcher->GetChildAt(1));
}

bool UGsUIGuildAuthorityEntry::FGsAuthoritySlot::IsChecked() const
{
	return _checkBoxEditable->IsChecked();
}

void UGsUIGuildAuthorityEntry::FGsAuthoritySlot::SetIsEditable(bool bInIsEditable)
{
	_switcher->SetActiveWidgetIndex((bInIsEditable) ? 1 : 0);
}

void UGsUIGuildAuthorityEntry::FGsAuthoritySlot::SetIsChecked(bool bInIsChecked)
{
	_checkBoxUneditable->SetIsChecked(bInIsChecked);
	_checkBoxEditable->SetIsChecked(bInIsChecked);
}
//----------------------------------------------------------------------------------------------------------------------
void UGsUIGuildAuthorityEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotList.Empty();

	_slotList.Emplace(FGsAuthoritySlot(_switcher0, GUILD_RIGHTS_LEVEL_MEMBER_MANAGEMENT));
	_slotList.Emplace(FGsAuthoritySlot(_switcher1, GUILD_RIGHTS_LEVEL_POST_MANAGEMENT));
	_slotList.Emplace(FGsAuthoritySlot(_switcher2, GUILD_RIGHTS_LEVEL_BUILDING_MANAGEMENT));
	_slotList.Emplace(FGsAuthoritySlot(_switcher3, GUILD_RIGHTS_LEVEL_DIPLOMACY));
	_slotList.Emplace(FGsAuthoritySlot(_switcher4, GUILD_RIGHTS_LEVEL_GUILD_STORE));
	_slotList.Emplace(FGsAuthoritySlot(_switcher5, GUILD_RIGHTS_LEVEL_GUILD_CAMP));
};

void UGsUIGuildAuthorityEntry::SetData(GuildMemberGradeType InType, GuildRightsLevel InAuthority)
{
	_memberGrade = InType;

	// 등급이름
	FText textGrade;
	FGsGuildHelper::GetMemberGradeText(InType, textGrade);
	_textBlockGrade->SetText(textGrade);
	
	// 0:단원관리, 1:게시글관리, 2.:운영, 3:외교, 4:창고, 5:수호탑관리
	SetCheckBox(0, FGsGuildHelper::CanManageMember(InAuthority));
	SetCheckBox(1, FGsGuildHelper::CanManagePost(InAuthority));
	SetCheckBox(2, FGsGuildHelper::CanManageBuilding(InAuthority));
	SetCheckBox(3, FGsGuildHelper::CanDiplomacy(InAuthority));
	SetCheckBox(4, FGsGuildHelper::CanDistribute(InAuthority));
	SetCheckBox(5, FGsGuildHelper::CanManageCamp(InAuthority));	

	// 길마 버튼은 편집 못하게 막음
	if (GuildMemberGradeType::CAPTAIN == InType)
	{
		for (FGsAuthoritySlot& slot : _slotList)
		{
			slot._checkBoxEditable->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		for (FGsAuthoritySlot& slot : _slotList)
		{
			slot._checkBoxEditable->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UGsUIGuildAuthorityEntry::SetEditableMode(bool bInEditable)
{
	for (FGsAuthoritySlot& slot : _slotList)
	{
		slot.SetIsEditable(bInEditable);
	}
}

GuildRightsLevel UGsUIGuildAuthorityEntry::GetAuthority()
{
	// 마스터는 모든권한으로 예외처리
	if (GuildMemberGradeType::CAPTAIN == _memberGrade)
	{
		return GUILD_RIGHTS_LEVEL_MASTER;
	}

	GuildRightsLevel authority = 0;

	for (FGsAuthoritySlot& slot : _slotList)
	{
		if (slot.IsChecked())
		{
			authority |= slot._authorityFlag;
		}
	}

	return authority;
}

void UGsUIGuildAuthorityEntry::SetCheckBox(int32 InIndex, bool bIsChecked)
{
	if (_slotList.IsValidIndex(InIndex))
	{
		_slotList[InIndex].SetIsChecked(bIsChecked);
	}
}
