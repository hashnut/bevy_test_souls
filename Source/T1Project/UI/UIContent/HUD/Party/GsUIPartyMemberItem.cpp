#include "GsUIPartyMemberItem.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Factory/GsIconFactory.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupChatUserInfo.h"
#include "UI/UIContent/HUD/Party/GsUIPartyDrawMenu.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"

#include "Party/GsPartyFunc.h"
#include "Party/GsPartyMemberInfo.h"
#include "Party/GsPartyDefine.h"

#include "Map/GsMapFunc.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendService.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "DataSchema/GameObject/GsSchemaWeaponUIInfo.h"
#include "DataSchema/Map/GsSchemaMapData.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "ContentsServer/GsContentsServerHelper.h"

#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "UTIL/GsUIUtil.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsText.h"

#include "Currency/GsCurrencyHelper.h"
#include "Currency/GsCostPackage.h"

#include "Core/Public/Delegates/Delegate.h"

#include "Components/Image.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/ProgressBar.h"

#include "Paper2D/Classes/PaperSprite.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"

#include "Sequence/GsSequencePlayer.h"
#include "Guild/GsGuildHelper.h"


void UGsUIPartyMemberItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnDrawMenu->OnClicked.AddDynamic(this, &UGsUIPartyMemberItem::OnClickDrawMenu);
	_btnInvite->OnClicked.AddDynamic(this, &UGsUIPartyMemberItem::OnClickInvite);

	_drawMenu->_callbackChangeLeader = [this]()
	{
		OnClickChangeLeader();
	};
	_drawMenu->_callbackKick = [this]()
	{
		OnClickKick();
	};
	_drawMenu->_callbackChannelMove = [this]()
	{
		OnClickChannelMove();
	};
	_drawMenu->_callbackUserInfo = [this]()
	{
		OnClickUserInfo();
	};
}
void UGsUIPartyMemberItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 비었을때
	if (_isEmpty == true)
	{
		return;
	}
	// 초대 대기중
	if (_memberItemType == EGsPartyMemberItemType::InviteProgress)
	{
		float rate = GetInviteWaitTimeRate();
		_progressBarInviteWait->SetPercent(rate);
	}
	// 멤버일때
	else
	{
		UpdateChangedInfo();
	}
}

// 드로우 메뉴 열기
void UGsUIPartyMemberItem::OnClickDrawMenu()
{
	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);

	if (_drawMenu->IsOpen() == true)
	{
		CloseDrawMenu();
		return;
	}
	if (true == OpenDrawMenu())
	{
		_drawMenu->Open();
	}
}

// open menu
bool UGsUIPartyMemberItem::OpenDrawMenu()
{
	EGsUIPartyHUDDrawMenuSwitchType switchIndex = EGsUIPartyHUDDrawMenuSwitchType::NotUse;

	if (_memberItemType == EGsPartyMemberItemType::Me)
	{
		switchIndex = EGsUIPartyHUDDrawMenuSwitchType::NotUse;
	}
	else if (_memberItemType == EGsPartyMemberItemType::Member_ImLeader)
	{
		switchIndex = EGsUIPartyHUDDrawMenuSwitchType::Open_MemberImLeader;
	}
	else if (_memberItemType == EGsPartyMemberItemType::Member_ImNotLeader)
	{
		// im member - target member
		if (_isLeader == false)
		{
			switchIndex = EGsUIPartyHUDDrawMenuSwitchType::Open_MemberImNotLeader;
		}
		// im member - target leader
		else
		{
			switchIndex = EGsUIPartyHUDDrawMenuSwitchType::Open_LeaderImNotLeader;
		}
	}

	if (switchIndex == EGsUIPartyHUDDrawMenuSwitchType::NotUse)
	{
		return false;
	}

	_drawMenu->SetType(switchIndex);

	return true;
}

// 리더 변경
void UGsUIPartyMemberItem::OnClickChangeLeader()
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
// 강퇴
void UGsUIPartyMemberItem::OnClickKick()
{
	// 이미 보냈으면 return
	if (_isSendKick == true)
	{
		return;
	}

	FText messageFormat;
	FText::FindText(TEXT("PartyText"), TEXT("PartyKickOut_desc"), messageFormat);

	FGsUIHelper::PopupYesNo(FText::Format(messageFormat, _textName), [this](bool bYes)
		{
			if (bYes)
			{
				// 서버 전송
				_isSendKick = true;
				FGsNetSendService::SendPartyKick(_partySlotId);
			}
		});
}
// 초대
void UGsUIPartyMemberItem::OnClickInvite()
{
	// 내가 리더가 아니면 아무것도 하지 않는다
	if (GSParty()->AmILeader() == false)
	{
		return;
	}

	GUI()->Open(TEXT("PopupPartyInvite"));

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

// 채널 이동
void UGsUIPartyMemberItem::OnClickChannelMove()
{
	// 1. 내가 리더면 할필요 없다(파티장에게로 가는거라서)
	if (GSParty()->AmILeader() == true)
	{
		return;
	}

	// 2. 파티장에게 이동이 막혔는지 확인
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::PARTY_WARP_TO_LEADER, true))
	{
		return;
	}

	// 3. 개인 이벤트 채널에서는 막음
	// 싱글 필드도 막음
	if (GLevel()->IsMapType(MapType::INSTANCE_SINGLE) == true ||
		GLevel()->IsMapType(MapType::SINGLE_FIELD) == true)
	{
		// 대상의 위치로 이동할 수 없습니다
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyChannelFail"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM,findText);
		return;
	}

	// 4. local이  상태이상이면 못보냄
	if (true == FGsMapFunc::IsLocalWarpImpossible())
	{
		// 대상의 위치로 이동할 수 없습니다
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyChannelFail"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM,findText);
		return;
	}

	bool isDiffMap = false;
	// 5. 방장과 같은 채널 && 같은 맵이면 안함	
	EGsPartyWarpToLeaderResult result = GSParty()->IsPossibleWarpToLeader(isDiffMap);
	if (result != EGsPartyWarpToLeaderResult::WarpPossible)
	{
		if(result == EGsPartyWarpToLeaderResult::SameChannelIdAndSameMapeId )
		{ 
			// 이미 동일한 위치입니다
			FText findText;
			FText::FindText(TEXT("PartyText"), TEXT("PartyChannelEqual"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
		}
		else if (result == EGsPartyWarpToLeaderResult::DifferentWorldId)
		{
			// 대상의 위치로 이동할 수 없습니다
			FText findText;
			FText::FindText(TEXT("PartyText"), TEXT("PartyChannelFail"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}
		return;
	}

	// 6. 채널 쿨타임인지 체크
	FGsGameDataManager* gameDataMgr = GGameData();
	if (gameDataMgr == nullptr)
	{
		return;
	}
	float remainSecond = gameDataMgr->GetChangeChannelRemainTime();
	// 0 초과면 쿨있다
	if (remainSecond > 0.0f)
	{
		// {0}초 후에 채널 이동 할 수 있습니다.		
		FText findText;
		FText::FindText(TEXT("UICommonText"), TEXT("ChannelDescRemainTime"), findText);
		FGsUIHelper::TrayMessageTicker(FText::Format(findText, (int)remainSecond));
		return;
	}

	// 7. 시네마틱 플레이중이면 이동하지 않기
	// https://jira.com2us.com/jira/browse/C2URWQ-5782
	if (true == UGsSequencePlayer::IsSequencePlaying())
	{
		FText findText;
		FText::FindText(TEXT("NetText"), TEXT("WARP_ERROR_CINEMATIC_CANCLE"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	// 다른 맵이 아니라면 바로 요청
	if (isDiffMap == false)
	{
		// 대화 연출 send 후 응답 대기중에는 워프 하지 말기
		if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
		{
			return;
		}


		// 서버에 요청
		FGsNetSendServiceWorld::SendWarpToPartyLeader();
	}
	// 다른맵이라면 재화 소모 확인 팝업띄움
	else
	{
		// 파티장 맵id
		FGsPartyMemberInfo* leaderInfo =
			GSParty()->FindLeaderPartyMemberInfo();

		if (leaderInfo == nullptr)
		{
			return;
		}

		int leaderMapId  = leaderInfo->GetMapId();

		if (FGsGuildHelper::IsAgitMap(leaderMapId))
		{
			// 대상의 위치로 이동할 수 없습니다
			FText findText;
			FText::FindText(TEXT("PartyText"), TEXT("PartyChannelFail"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);

			return;
		}

		CurrencyType currencyType = CurrencyType::GOLD;
		int useCost = 0;
		const FGsSchemaMapData* mapTbl = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(leaderMapId);
		const FGsSchemaMapBalance* mapBalanceData = mapTbl->balanceId.GetRow();
		if (mapBalanceData != nullptr)
		{
			currencyType =
				mapBalanceData->cost.costCurrencyType;
			useCost = mapBalanceData->cost.costCurrencyAmount;
		}

		GSWorldMap()->ReserveWarpMoveToPartyLeader(
			mapTbl->MapStringId.ToString(),
			FGsCostPackage::CreateSimple(currencyType, useCost));
	}
}

void UGsUIPartyMemberItem::OnClickUserInfo()
{
	FGsPartyMemberInfo* info = GSParty()->FindPartyMemberInfo(_partySlotId);
	if (nullptr == info)
	{
		return;
	}

	TWeakObjectPtr<UGsUIPopupChatUserInfo> contextPopup = Cast<UGsUIPopupChatUserInfo>(GUI()->OpenAndGetWidget(TEXT("PopupChatUserInfo")));
	if (info && contextPopup.IsValid())
	{
		UGsUIPopupChatUserInfo::PopupInitData initData{ 0, info->GetName(), TEXT(""), true, EGsUserContentsOwner::PARTY_MEMBER, info->GetHomeWorldId() };
		contextPopup->SetData(&initData, false);		
	}
}

// 데이터 세팅
void UGsUIPartyMemberItem::SetData(int In_slotId,
	bool In_isEmpty,
	const FString& In_name,
	EGsPartyMemberItemType In_itemType,
	CreatureWeaponType In_weaponType,
	bool In_isLeader,
	int In_level,
	int In_currHp,
	int In_maxHp,
	int In_currShield,
	float In_mpRate,
	bool In_isCloseDrawMenu,
	bool In_isSpawned,
	bool In_isReady,
	WorldId In_homeWorldId)
{
	// 타입이 바뀐거면 다시 세팅하자
	if (In_itemType != _memberItemType)
	{
		ClearOldData();
	}

	_isEmpty = In_isEmpty;
	_memberItemType = In_itemType;

	EGsUIPartyHUDMemberSwitchType switchType = EGsUIPartyHUDMemberSwitchType::Empty_LocalIsMember;
	if (In_isEmpty == false)
	{
		if (In_itemType == EGsPartyMemberItemType::InviteProgress)
		{
			switchType = EGsUIPartyHUDMemberSwitchType::InviteProgress;
		}
		else
		{
			switchType = EGsUIPartyHUDMemberSwitchType::Member;
		}
	}
	else
	{
		if (GSParty()->AmILeader() == true)
		{
			switchType = EGsUIPartyHUDMemberSwitchType::Empty_LocalIsLeader;
		}
	}
	int switchIndex = (int)switchType;
	// 기존과 다르면 스위칭 하기
	if (_switcherType->ActiveWidgetIndex != switchIndex)
	{
		_switcherType->SetActiveWidgetIndex(switchIndex);
	}

	// 비어 있으면 더이상 할거 없다
	if (switchType == EGsUIPartyHUDMemberSwitchType::Empty_LocalIsLeader ||
		switchType == EGsUIPartyHUDMemberSwitchType::Empty_LocalIsMember
		)
	{
		_partySlotId = In_slotId;
		return;
	}
	// 초대대기면 이름만 넣는다
	else if (switchType == EGsUIPartyHUDMemberSwitchType::InviteProgress)
	{
		// 이름 텍스트 세팅
		SetName(In_name, In_homeWorldId);
		_partySlotId = In_slotId;
		return;
	}

	SetPartySlotId(In_slotId);

	// 이름 텍스트 세팅
	SetName(In_name, In_homeWorldId);
	
	SetIsLeader(In_isLeader);

	// 레벨 세팅
	SetLevel(In_level);

	// hp 세팅	
	SetHP(In_currHp, In_maxHp, false);
	SetShield(In_currShield, false);
	UpdateHpShieldBar();
	 
	// mp 세팅
	SetMpRate(In_mpRate);
	// 무기 변경
	CreatureWeaponType weaponType = (In_level == INVALID_LEVEL) ? CreatureWeaponType::ALL : In_weaponType;
	SetWeaponType(weaponType);

	if (In_isCloseDrawMenu == true)
	{
		CloseDrawMenu();
	}

	SetIsSpawned(In_isSpawned);

	SetIsReaddy(In_isReady);

	ESlateVisibility drawMenuVisibility =
		(In_itemType == EGsPartyMemberItemType::Me) ? ESlateVisibility::Hidden: ESlateVisibility::Visible;

	_btnDrawMenu->SetVisibility(drawMenuVisibility);

	if (GSParty()->IsMySlotId(In_slotId))
	{
		UpdateAbnormalityConfuse();
	}
	else
	{
		SetIsConfused(false);
	}
}

// 무기 아이콘 만들기
void UGsUIPartyMemberItem::MakeWeaponIcon(CreatureWeaponType In_weaponType)
{
	if (In_weaponType == CreatureWeaponType::NONE)
	{
		return;
	}

	const FGsSchemaWeaponUIInfo* tableData = FGsPartyFunc::GetWeaponUIInfo(In_weaponType);
	if (tableData == nullptr)
	{
		return;
	}

	UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(tableData->_weaponTypeImagePath);
	_imgWeapon->SetBrushFromAtlasInterface(spriteImg);

}


// 초대 대기 남은시간 rate 값 구하기
float UGsUIPartyMemberItem::GetInviteWaitTimeRate()
{
	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _startTime;

	static float maxTimeTick = 
		GData()->GetGlobalData()->_partyInviteWaitSecond * ETimespan::TicksPerSecond;

	float rate = diffTick / maxTimeTick;
	return rate;
}
// 프로그래스바 시작
void UGsUIPartyMemberItem::StartProgressBar(int64 In_startTime)
{
	_startTime = In_startTime;
}
// 드로우 메뉴 닫기
void UGsUIPartyMemberItem::CloseDrawMenu()
{
	_drawMenu->Close();
}

// info 변경되었는지 체크(정보 개별 수정)
void UGsUIPartyMemberItem::UpdateChangedInfo()
{
	FGsPartyMemberInfo* info =
		GSParty()->FindPartyMemberInfo(_partySlotId);
	if (info == nullptr ||
		info->IsChangedInfo() == false)
	{
		return;
	}
	
	// 레벨 변경
	if (info->IsChangedInfo(EGsPartyMemberInfoType::Level) == true)
	{
		SetLevel(info->GetLevel());
	}
	// hp 변경
	if (info->IsChangedInfo(EGsPartyMemberInfoType::HP) == true)
	{
		SetHP(info->GetNowHp(), info->GetMaxHp(), true);
	}
	// mp 변경
	if (info->IsChangedInfo(EGsPartyMemberInfoType::MP) == true)
	{
		SetMpRate(info->GetMpRate());
	}
	if (info->IsChangedInfo(EGsPartyMemberInfoType::HP_Shield) == true)
	{
		SetShield(info->GetNowShield(), true);
	}
	// 무기 변경
	if (info->IsChangedInfo(EGsPartyMemberInfoType::WeaponType) == true)
	{
		CreatureWeaponType weaponType = (info->GetLevel() == INVALID_LEVEL) ? CreatureWeaponType::ALL : info->GetWeaponType();
		SetWeaponType(weaponType);
	}
	// 스폰 변경
	if (info->IsChangedInfo(EGsPartyMemberInfoType::Spawn) == true)
	{
		SetIsSpawned(info->GetIsSpawned());
	}
	// 컨퓨즈 상태이상 변경
	if (info->IsChangedInfo(EGsPartyMemberInfoType::Confuse) == true)
	{
		UpdateAbnormalityConfuse();
	}

	info->ClearDirtyInfo();
}
// 레벨 세팅
void UGsUIPartyMemberItem::SetLevel(int In_level)
{
	if (_oldLevel == In_level)
	{
		return;
	}
	_oldLevel = In_level;

	// 레벨 텍스트 세팅
	const FString levelText = (In_level == -1) || (In_level == INVALID_LEVEL) ?
		FString("Lv.??") : FString::Printf(TEXT("Lv.%d"), In_level);
	_textLevel = FText::FromString(levelText);
}

// mp rate 세팅
void UGsUIPartyMemberItem::SetMpRate(float In_mpRate)
{
	if (_oldMpRate == In_mpRate)
	{
		return;
	}
	_oldMpRate = In_mpRate;
	// mp 세팅
	_progressBarMp->SetPercent(In_mpRate);
}

void UGsUIPartyMemberItem::SetHP(int32 InCurrent, int32 InMax, bool bInUpdateBar)
{
	_currHp = InCurrent;
	_maxHp = InMax;

	if (bInUpdateBar)
	{
		UpdateHpShieldBar();
	}

	// hp zero is dead
	ESlateVisibility imageDeadVisibility = (_currHp == 0) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;

	_imgDead->SetVisibility(imageDeadVisibility);
}

void UGsUIPartyMemberItem::SetShield(int32 InCurrent, bool bInUpdateBar)
{
	_currShield = InCurrent;

	if (bInUpdateBar)
	{
		UpdateHpShieldBar();
	}
}

void UGsUIPartyMemberItem::UpdateHpShieldBar()
{
	UGsUIUtil::SetHPShieldProgressBar(_progressBarHp, _progressBarShieldFront, _progressBarShieldBack,
		_currHp, _maxHp, _currShield);

}

// weapon type 변경
void UGsUIPartyMemberItem::SetWeaponType(CreatureWeaponType In_type)
{
	if (_oldWeaponType == In_type)
	{
		return;
	}

	_oldWeaponType = In_type;

	// 무기 타입 아이콘 만들기
	MakeWeaponIcon(In_type);
}

// 이전 데이터 클리어
void UGsUIPartyMemberItem::ClearOldData()
{
	_oldName.Empty();
	_oldWeaponType = CreatureWeaponType::NONE;
	_oldLevel = -1;
	//_oldHpRate = -1.0f;
	_oldMpRate = -1.0f;

	_currHp = 0;
	_maxHp = 0;
	_currShield = 0;
	
	_partySlotId = -1;
	_memberItemType = EGsPartyMemberItemType::None;

	// 추방 초기화
	_isSendKick = false;
}

void UGsUIPartyMemberItem::NativeConstruct()
{
	_drawMenu->Close();

	Super::NativeConstruct();
	ClearOldData();
}
// 이름 세팅
void UGsUIPartyMemberItem::SetName(const FString& In_name, WorldId In_homeWorldId)
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

// party slot id 세팅
void UGsUIPartyMemberItem::SetPartySlotId(int In_partySlotId)
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

// 리더인지 세팅
void UGsUIPartyMemberItem::SetIsLeader(bool In_isLeader)
{
	_isLeader = In_isLeader;

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
// 스폰 되어있는가
void UGsUIPartyMemberItem::SetIsSpawned(bool In_isSpawned)
{
	// 디스폰 이미지 처리
	// 스폰 되어있으면 hide
	// 디스폰 되어있으면 show
	ESlateVisibility despawnVisibility =
		(In_isSpawned) ? ESlateVisibility::Hidden:ESlateVisibility::SelfHitTestInvisible;

	// 이미 같으면 리턴
	if (_widgetDespawn->GetVisibility() == despawnVisibility)
	{
		return;
	}
	// 디스폰 이미지 on, off
	_widgetDespawn->SetVisibility(despawnVisibility);
}

// 2022/06/17 PKT - 준비 상태
void UGsUIPartyMemberItem::SetIsReaddy(bool In_isReady)
{
	_imgReady->SetVisibility(In_isReady ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIPartyMemberItem::UpdateAbnormalityConfuse()
{
	bool bIsConfused = false;
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (FGsAbnormalityHandlerBase* abnormalityHandler = local->GetAbnormalityHandler())
		{
			bIsConfused = abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::UTIL_CONFUSE);
		}
	}

	SetIsConfused(bIsConfused);
}

void UGsUIPartyMemberItem::SetIsConfused(bool bIsConfused)
{
	_panelRootBar->SetVisibility(bIsConfused ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
}
