#include "GsUIPartySettingMemberItem.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIControlLib/Define/EGsIconType.h"

#include "UI/UIContent/Factory/GsIconFactory.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Party/GsPartyFunc.h"
#include "Party/GsPartyMemberInfo.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendService.h"

#include "DataSchema/GameObject/GsSchemaWeaponUIInfo.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "ContentsServer/GsContentsServerHelper.h"

#include "UTIL/GsText.h"

#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/ProgressBar.h"

#include "Slate/Public/Widgets/Layout/Anchors.h"
#include "SlateCore/Public/Layout/Margin.h"

#include "Components/Image.h"

#include "Core/Public/Delegates/Delegate.h"
#include "Core/Public/Misc/DateTime.h"
#include "Core/Public/Misc/Timespan.h"

void UGsUIPartySettingMemberItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnLeave->OnClicked.AddDynamic(this, &UGsUIPartySettingMemberItem::OnClickLeave);
	_btnKick->OnClicked.AddDynamic(this, &UGsUIPartySettingMemberItem::OnClickKick);
	_btnChangeLeader->OnClicked.AddDynamic(this, &UGsUIPartySettingMemberItem::OnClickChangeLeader);
	_btnAddFriendImLeader->OnClicked.AddDynamic(this, &UGsUIPartySettingMemberItem::OnClickAddFriend);
	_btnAddFriendImNotLeader->OnClicked.AddDynamic(this, &UGsUIPartySettingMemberItem::OnClickAddFriend);
	_btnInvite->OnClicked.AddDynamic(this, &UGsUIPartySettingMemberItem::OnClickInvite);
	
}

// 나가기(나자신)
void UGsUIPartySettingMemberItem::OnClickLeave()
{
	FGsPartyFunc::OpenLeavePopup();
}

// 추방
void UGsUIPartySettingMemberItem::OnClickKick()
{
	FText messageFormat;
	FText::FindText(TEXT("PartyText"), TEXT("PartyKickOut_desc"), messageFormat);

	FGsUIHelper::PopupYesNo(FText::Format(messageFormat, _textName), [this](bool bYes)
		{
			if (bYes)
			{
				// 서버 전송
				FGsNetSendService::SendPartyKick(_partySlotId);
			}
		});
}

// 리더 변경
void UGsUIPartySettingMemberItem::OnClickChangeLeader()
{
	FText messageFormat;
	FText::FindText(TEXT("PartyText"), TEXT("PartyLeaderDelegation_desc"), messageFormat);

	FGsUIHelper::PopupYesNo(FText::Format(messageFormat, _textName), [this](bool bYes)
		{
			if (bYes)
			{
				// 서버 전송
				FGsNetSendService::SendPartyChangeLeader(_partySlotId);
			}
		});
}

// 친구 추가
void UGsUIPartySettingMemberItem::OnClickAddFriend()
{	
	FGsUIMsgParamBuddy param = FGsUIMsgParamBuddy(0, _oldName);
	GMessage()->GetUI().SendMessage(MessageUI::BUDDY_ADD, &param);
}


// 초대
void UGsUIPartySettingMemberItem::OnClickInvite()
{
	// 내가 리더가 아니면 아무것도 하지 않는다
	if (GSParty()->AmILeader() == false)
	{
		return;
	}

	GUI()->Open(TEXT("PopupPartyInvite"));
}

// 데이터 세팅
void UGsUIPartySettingMemberItem::SetData(int In_slotId, 
	bool In_isEmpty,
	const FString& In_name/*=FString("")*/,
	EGsPartyMemberItemType In_itemType,
	CreatureWeaponType In_weaponType,
	bool In_isLeader,
	int In_level,
	int In_channelId,
	WorldId In_homeWorldId)
{	
	// 타입이 바뀐거면 다시 세팅하자
	if (In_itemType != _memberItemType)
	{
		ClearOldData();
	}

	SetIsEmpty(In_isEmpty, In_isLeader);
	if (In_isEmpty == true)
	{
		_partySlotId = In_slotId;
		return;
	}

	SetPartySlotId(In_slotId);

	SetName(In_name, In_homeWorldId);

	SetMemberItemType(In_itemType);

	SetIsLeader(In_isLeader);

	// 레벨 텍스트 세팅
	SetLevel(In_level);

	SetChannel(In_channelId);

	CreatureWeaponType weaponType = (In_level == INVALID_LEVEL) ? CreatureWeaponType::ALL : In_weaponType;
	SetWeaponType(weaponType);


	FGsPartyMemberInfo* info =
		GSParty()->FindPartyMemberInfo(_partySlotId);
	// 전체 세팅 했으므로 dirty 클리어
	if (info != nullptr)
	{
		info->ClearPopupSettingDirtyInfo();
	}
}
// 무기 아이콘 만들기
void UGsUIPartySettingMemberItem::MakeWeaponIcon(CreatureWeaponType In_weaponType)
{
	if (In_weaponType == CreatureWeaponType::NONE)
	{
		return;
	}
	_loadedIcon = FGsIconFactory::CreateIcon(EGsIconType::ICON_PARTY_WEAPON_TYPE);
	if (nullptr == _loadedIcon ||
		nullptr == _iconImageRoot)
	{
		return;
	}
	
	const FGsSchemaWeaponUIInfo* tableData = FGsPartyFunc::GetWeaponUIInfo(In_weaponType);
	if (tableData == nullptr)
	{
		return;
	}	
	_loadedIcon->SetIconImage(tableData->_weaponTypeImagePath);
	
	UCanvasPanelSlot* canvasPanelSlot = _iconImageRoot->AddChildToCanvas(_loadedIcon);
	if (nullptr == canvasPanelSlot)
	{
		return;
	}
	
	canvasPanelSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
	canvasPanelSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));	
}
// 리소스 해제
void UGsUIPartySettingMemberItem::ReleaseWeaponIcon()
{
	// 이미 만든게 있으면 해제
	if (_loadedIcon != nullptr)
	{
		FGsIconFactory::ReleaseIcon(_loadedIcon);
		_loadedIcon = nullptr;
	}
}
// 프로그래스바 시작
void UGsUIPartySettingMemberItem::StartProgressBar(int64 In_startTime)
{
	_startTime = In_startTime;
}

void UGsUIPartySettingMemberItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isEmpty == true)
	{
		return;
	}
	if(_memberItemType == EGsPartyMemberItemType::InviteProgress)
	{
		float rate = GetInviteWaitTimeRate();
		_progressBarInviteWait->SetPercent(rate);
	}
	// 멤버 일때
	else
	{
		UpdateChangedInfo();
	}
}

// 초대 대기 남은시간 rate 값 구하기
float UGsUIPartySettingMemberItem::GetInviteWaitTimeRate()
{
	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _startTime;

	static float maxTimeTick = 
		GData()->GetGlobalData()->_partyInviteWaitSecond * ETimespan::TicksPerSecond;

	float rate = diffTick / maxTimeTick;
	return rate;
}

// info 변경되었는지 체크(정보 개별 수정)
void UGsUIPartySettingMemberItem::UpdateChangedInfo()
{
	FGsPartyMemberInfo* info =
		GSParty()->FindPartyMemberInfo(_partySlotId);
	if (info == nullptr ||
		info->IsChangedPopupSettingInfo() == false)
	{
		return;
	}

	// 레벨 변경
	if (info->IsChangedPopupSettingInfo(EGsPartyMemberInfoType::Level) == true)
	{
		SetLevel(info->GetLevel());
	}
	// 무기 변경
	if (info->IsChangedPopupSettingInfo(EGsPartyMemberInfoType::WeaponType) == true)
	{
		CreatureWeaponType weaponType = (info->GetLevel() == INVALID_LEVEL) ? CreatureWeaponType::ALL : info->GetWeaponType();
		SetWeaponType(weaponType);
	}

	info->ClearPopupSettingDirtyInfo();
}


// 레벨 세팅
void UGsUIPartySettingMemberItem::SetLevel(int In_level)
{
	if (In_level == _oldLevel)
	{
		return;
	}

	_oldLevel = In_level;

	// 레벨 텍스트 세팅
	const FString levelText = (In_level == -1 || (In_level == INVALID_LEVEL)) ?
		FString("Lv.??") : FString::Printf(TEXT("Lv.%d"), In_level);
	_textLevel = FText::FromString(levelText);
}

// weapon type 변경
void UGsUIPartySettingMemberItem::SetWeaponType(CreatureWeaponType In_type)
{
	if (_oldWeaponType == In_type)
	{
		return;
	}

	_oldWeaponType = In_type;

	// 무기 타입 아이콘 클리어
	ReleaseWeaponIcon();

	// 무기 타입 아이콘 만들기
	MakeWeaponIcon(In_type);
}

// 이전 데이터 클리어
void UGsUIPartySettingMemberItem::ClearOldData()
{
	_oldName.Empty();
	_oldWeaponType = CreatureWeaponType::NONE;
	_oldLevel = -1;
	_oldChannelId = -1;

	_partySlotId = -1;

	_memberItemType = EGsPartyMemberItemType::None;

}

void UGsUIPartySettingMemberItem::OnShow()
{
	ClearOldData();
}

// party slot id 세팅
void UGsUIPartySettingMemberItem::SetPartySlotId(int In_partySlotId)
{
	// 이미 같으면 return
	if (_partySlotId == In_partySlotId)
	{
		return;
	}

	_partySlotId = In_partySlotId;
	// 슬롯 번호 텍스트 세팅
	// 0부터 시작해서 1더함
	int addId = In_partySlotId + 1;
	const FString slotText = FString::Printf(TEXT("%dP"), addId);
	_textSlotId = FText::FromString(slotText);
}

// 비어있는지 세팅
void UGsUIPartySettingMemberItem::SetIsEmpty(bool In_isEmpty, bool In_isLeader)
{
	_isEmpty = In_isEmpty;

	int emptyIndex = 0;
	if (In_isEmpty == true)
	{
		if (GSParty()->AmILeader() == true)
		{
			emptyIndex = 1;
		}
		else
		{
			emptyIndex = 2;
		}
	}

	// 기존과 다르면 스위칭
	if (_switcherEmpty->ActiveWidgetIndex != emptyIndex)
	{
		_switcherEmpty->SetActiveWidgetIndex(emptyIndex);
	}
}

// 이름 세팅
void UGsUIPartySettingMemberItem::SetName(const FString& In_name, WorldId In_homeWorldId)
{
	// 인터서버 필드면 prefix 붙인다	
	FString newName = FGsPartyFunc::GetPartyMemberName(In_name, In_homeWorldId);

	// 같으면 처리할 필요 없음
	if (newName.Equals(_oldName) == true)
	{
		return;
	}
	_oldName = newName;

	// 이름 텍스트 세팅
	_textName = FText::FromString(newName);
}

// 아이템 타입 세팅
void UGsUIPartySettingMemberItem::SetMemberItemType(EGsPartyMemberItemType In_itemType)
{
	// 이미 같은거면 리턴
	if (_memberItemType == In_itemType)
	{
		return;
	}

	_memberItemType = In_itemType;

	// 타입 세팅
	_switcherType->SetActiveWidgetIndex((int)In_itemType);

	// 초대 대기일때만 채널 캔버스 끄고 나머지는 살림
	ESlateVisibility channelVisible =
		(In_itemType == EGsPartyMemberItemType::InviteProgress) ?
		ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible;
	_channelCanvalPanel->SetVisibility(channelVisible);
}

// 리더인지 세팅
void UGsUIPartySettingMemberItem::SetIsLeader(bool In_isLeader)
{
	ESlateVisibility leaderVisibility =
		(In_isLeader) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;

	// 이미 같으면 리턴
	if (_imgLeader->GetVisibility() == leaderVisibility)
	{
		return;
	}
	// 리더 이미지 on, off
	_imgLeader->SetVisibility(leaderVisibility);
}

// 채널 세팅
void UGsUIPartySettingMemberItem::SetChannel(int In_channelId)
{
	if (_oldChannelId == In_channelId)
	{
		return;
	}

	_oldChannelId = In_channelId;

	// 채널 텍스트 세팅
	const FString channelText = (In_channelId == -1) ?
		FString("") : FString::Printf(TEXT("CH.%d"), In_channelId);
	_textChannel = FText::FromString(channelText);
}