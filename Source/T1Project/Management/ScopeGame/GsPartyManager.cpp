#include "GsPartyManager.h"

#include "Party/GsPartyMemberInfo.h"
#include "Party/GsPartyInviteeInfo.h"
#include "Party/GsPartyInviteBanInfo.h"
#include "Party/GsPartyFunc.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendService.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"

#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"

#include "Option/GsGameUserSettings.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "UI/UIContent/Popup/GsUIPopupPartySetting.h"

#include "Public/Shared/Shared/SharedInclude/SharedTypes.h"


// 초기화
void FGsPartyManager::Initialize()
{
	_isInParty = false;
}

// 정리
void FGsPartyManager::Finalize()
{
	ClearData();

	if (_mapInviteBanInfo.Num() != 0)
	{
		for (auto banInfo : _mapInviteBanInfo)
		{
			if (banInfo.Value == nullptr)
			{
				continue;
			}

			delete banInfo.Value;
		}
		_mapInviteBanInfo.Empty();
	}
}

// 갱신
void FGsPartyManager::Update(float In_delta)
{

	TArray<int> arrDeleteSlotId;
	for (FGsPartyInviteeInfo* info : _arrPartyInviteeInfo)
	{
		if (info == nullptr)
		{
			continue;
		}
		info->Update();

		if (info->GetIsTimmerStarted() == false)
		{
			arrDeleteSlotId.Add(info->GetPartySlotId());
		}
	}

	if (arrDeleteSlotId.Num() != 0)
	{
		for (int slotId :arrDeleteSlotId)
		{
			RemovePartyInviteeInfo(slotId);
		}

		// 하나면 단일 갱신
		if (arrDeleteSlotId.Num() == 1)
		{
			UpdateMemberUI(false, true, arrDeleteSlotId[0]);
		}
		// 복수개면 모두 갱신
		else
		{
			UpdateMemberUI(true);
		}

	}
	
	// 밴 정보 갱신
	if (_mapInviteBanInfo.Num() != 0)
	{
		// 삭제할 정보
		TArray<FString> arrayDelInfo;

		for (auto banInfo: _mapInviteBanInfo) 
		{
			if (banInfo.Value == nullptr)
			{
				continue;
			}

			if (banInfo.Value->Update() == false)
			{
				arrayDelInfo.Add(banInfo.Key);
			}
		}

		// 삭제할꺼 삭제
		for (FString& delName : arrayDelInfo)
		{
			DelInviteBanInfo(delName);
		}
	}
}


// 데이터 클리어
void FGsPartyManager::ClearData()
{
	if (_arrPartyMemberInfo.Num() != 0)
	{
		for (FGsPartyMemberInfo* info : _arrPartyMemberInfo)
		{
			if (info == nullptr)
			{
				continue;
			}
			delete info;
		}
		_arrPartyMemberInfo.Empty();
	}
	if (_arrPartyInviteeInfo.Num() != 0)
	{
		for (FGsPartyInviteeInfo* info : _arrPartyInviteeInfo)
		{
			if (info == nullptr)
			{
				continue;
			}
			delete info;
		}
		_arrPartyInviteeInfo.Empty();
	}	

	_isInParty = false;

	_partyDropOwnershipType = PartyDropOwnershipType::RANDOM;

	_inviterInfo.ClearInviter();
}


// 생성 응답
void FGsPartyManager::RecvPartyCreate(FGsPartyMemberInfo* In_myInfo,
	PartyDropOwnershipType In_type)
{
	_partyDropOwnershipType = In_type;
	_leaderSlotId = 0;
	_mySlotId = 0;

	_isInParty = true;

	// 내가 만들어서
	// 내가 방장임
	if (nullptr == In_myInfo)
	{
		return;
	}

	_arrPartyMemberInfo.Add(In_myInfo);

}

// 초대 응답
void FGsPartyManager::RecvPartyInvite(int In_partySlotId, const FString& In_name, WorldId In_homeWorldId)
{
	FGsPartyInviteeInfo* inviteeInfo = FindPartyInviteeInfo(In_partySlotId);
	if (inviteeInfo == nullptr)
	{
		inviteeInfo = new FGsPartyInviteeInfo(
			In_name,In_partySlotId, In_homeWorldId);
		inviteeInfo->StartTimer();
		_arrPartyInviteeInfo.Add(inviteeInfo);
	}
	else
	{
		inviteeInfo->SetData(In_name, In_partySlotId, In_homeWorldId);
		inviteeInfo->StartTimer();
	}
	// 단일 갱신
	UpdateMemberUI(false, true,In_partySlotId);

	// 여기는 내가 초대 보낸거에 대한 서버 응답
	// 원래는 여기서 ban 정보 추가했는데
	// 수락시에는 카운트가 초기화 되어야 하므로
	// fail 패킷으로 변경		  
}
// 초대 대상 찾기
FGsPartyInviteeInfo* FGsPartyManager::FindPartyInviteeInfo(int In_partySlotId)
{
	for (FGsPartyInviteeInfo* iter : _arrPartyInviteeInfo)
	{
		if (iter == nullptr)
		{
			continue;
		}
		if (iter->GetPartySlotId() == In_partySlotId)
		{
			return iter;
		}
	}
	return nullptr;
}
int FGsPartyManager::FindPartyInviteeIndexBySlotId(int In_partySlotId)
{
	int findIndex = -1;
	for (int i = 0; i<  _arrPartyInviteeInfo.Num(); ++i)
	{
		FGsPartyInviteeInfo* info = _arrPartyInviteeInfo[i];
		if (info == nullptr)
		{
			continue;
		}
		if (info->GetPartySlotId() == In_partySlotId)
		{
			findIndex = i;
			break;
		}
	}
	return findIndex;
}

// 멤버 찾기
FGsPartyMemberInfo* FGsPartyManager::FindPartyMemberInfo(int In_partySlotId)
{
	for (FGsPartyMemberInfo* iter: _arrPartyMemberInfo)
	{
		if (iter == nullptr)
		{
			continue;
		}
		if (iter->GetPartySlotId() == In_partySlotId)
		{
			return iter;
		}
	}

	return nullptr;
}

// find array index by slot id
int FGsPartyManager::FindPartyMemberIndexBySlotId(int In_partySlotId)
{
	int findIndex = -1;
	for (int i = 0; i < _arrPartyMemberInfo.Num(); ++i)
	{
		FGsPartyMemberInfo* info = _arrPartyMemberInfo[i];
		if (info == nullptr)
		{
			continue;
		}

		if (info->GetPartySlotId() == In_partySlotId)
		{
			findIndex = i;
			break;
		}
	}
	return findIndex;
}

// 초대 받음
void FGsPartyManager::RecvPartyInvited(const FString& In_name, int In_level, int In_channelId, WorldId In_homeWorldId)
{
	FGsUnlockManager* unlockManager = GSUnlock();
	if (unlockManager == nullptr)
		return;

	if (false == unlockManager->IsContentsUnlock(EGsUnlockContentsType::PARTY_CREATE_OR_JOIN, true))
	{
		FGsNetSendService::SendPartyConfirmInvitation(false);
		return;
	}

	_inviterInfo.SetData(In_name, In_level, In_channelId, In_homeWorldId);
	_inviterInfo.StartTimer();

	// 자동 수락 옵션 켜져있으면 바로 ok
	bool isAutoPartyAccept = false;
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		const float isAutoPartyAcceptFloat =
			gameUserSettings->GetAutoSetting(EGsOptionAuto::IS_AUTO_PARTY_ACCEPT);

		isAutoPartyAccept = (isAutoPartyAcceptFloat == 0.0f) ? false : true;
	}

	if (isAutoPartyAccept == false)
	{
		UGsUIManager* uiManager = GUI();
		if (uiManager == nullptr)
		{
			return;
		}

		uiManager->Open(TEXT("PopupPartyInviteAccept"));
	}
	else
	{
		FGsNetSendService::SendPartyConfirmInvitation(true);
	}
}

// 초대 수락(내가 수락해서 들어감)
void FGsPartyManager::RecvPartyJoined(PD::GC::PKT_GC_PARTY_JOINED_READ& In_packet,
	TMap<int64, bool> In_mapSpawnedInfo,
	FGsPartyMemberInfo* In_myInfo)
{	
	// 기존 데이터 삭제
	ClearData();

	_isInParty = true;
	_leaderSlotId = In_packet.LeaderSlotIndex();
	_mySlotId = In_packet.MySlotIndex();

	_partyDropOwnershipType = In_packet.DropOwnershipType();

	for (PD::GC::PKT_GC_PARTY_JOINED_READ::PartyMemberInfoListIterator iter =
		In_packet.GetFirstPartyMemberInfoListIterator();
		iter != In_packet.GetLastPartyMemberInfoListIterator();
		++iter)
	{		
		int slotIndex = iter->SlotIndex();
		bool isLeader = (_leaderSlotId == slotIndex)?true: false;
			
		int64 gameId = iter->GameId();

		bool isSpawned =
			In_mapSpawnedInfo.FindRef(gameId);
		FGsPartyMemberInfo* newInfo =
			new FGsPartyMemberInfo(gameId, isLeader,
				iter->Name(),
				iter->Level(),
				iter->Hp(), iter->MaxHp(), iter->HpShield(),
				iter->Mp(), iter->MaxMp(),
				slotIndex,
				iter->WeaponType(),
				iter->ChannelId(),
				iter->MapId(),
				iter->Pos(),
				isSpawned,
				iter->IsReady(),
				// not ready server data(world id)
				// 서버에서 world id 주기로 변경(시련의 전당에서 파티 join 후 바로 따라가기 버튼 활성화 이슈)
				// https://jira.com2us.com/jira/browse/C2URWQ-6207
				iter->MemberWorldId(),
				iter->MemberHomeWorldId());
		_arrPartyMemberInfo.Add(newInfo);
	}

	if (In_myInfo != nullptr)
	{
		_arrPartyMemberInfo.Add(In_myInfo);
	}
}




// 멤버 정보 넣기
void FGsPartyManager::SetPartyMembmerInfo(int64 In_gameId, const FString& In_name,
	int In_level, int In_nowHP, int In_maxHP, int In_nowShield, int In_nowMP, int In_maxMP,
	int In_partySlotId, CreatureWeaponType In_weaponType, int In_channelId,
	int In_mapId, const FVector& In_pos,
	bool In_isSpawned, WorldId In_worldId, WorldId In_homeWorldId)
{
	bool isLeader = (_leaderSlotId == In_partySlotId) ? true : false;

	FGsPartyMemberInfo* memberInfo = FindPartyMemberInfo(In_partySlotId);
	if (memberInfo == nullptr)
	{
		memberInfo =
			new FGsPartyMemberInfo(In_gameId, isLeader, In_name,
				In_level, In_nowHP, In_maxHP, In_nowShield, In_nowMP, In_maxMP,
				In_partySlotId, In_weaponType, In_channelId,
				In_mapId, In_pos, In_isSpawned, false, In_worldId, In_homeWorldId);

		_arrPartyMemberInfo.Add(memberInfo);
	}
	else
	{	// 2022/06/13 PKT - 업데이트 패킷 이면 준비 상태는 이전 상태 그대로 세팅
		bool isReady = memberInfo->GetComplatedReady();

		memberInfo->SetData(In_gameId, isLeader, In_name,
			In_level, In_nowHP, In_maxHP, In_nowShield, In_nowMP, In_maxMP,
			In_partySlotId, In_weaponType, In_channelId,
			In_mapId, In_pos, In_isSpawned, isReady, In_worldId, In_homeWorldId);
	}

	// 밴정보에서 삭제(파티원 되었으므로)
	DelInviteBanInfo(In_name);
}

// 초대 거절
void FGsPartyManager::RecvPartyRefuseInvitation()
{
	// 기존 데이터 삭제
	ClearData();

	// 초대 팝업 닫기

	UGsUIManager* uiManager = GUI();
	if (uiManager == nullptr)
	{
		return;
	}

	uiManager->CloseByKeyName(TEXT("PopupPartyInviteAccept"));

}
// 초대 실패
void FGsPartyManager::RecvPartyInviteFailed(int In_slotId)
{
	FGsPartyInviteeInfo* removeTargetInfo = FindPartyInviteeInfo(In_slotId);
	if (removeTargetInfo == nullptr)
	{
		return;
	}
	// ticker show
	FGsPartyFunc::ShowTickerInviteFailed();

	// 초대 밴 정보 추가
	AddInviteBanInfo(removeTargetInfo->GetName());

	RemovePartyInviteeInfo(In_slotId);
	// 단일 갱신
	UpdateMemberUI(false, false,In_slotId);
}

// 초대 대상 삭제
void FGsPartyManager::RemovePartyInviteeInfo(int In_partySlotId)
{
	int findIndex =  FindPartyInviteeIndexBySlotId(In_partySlotId);
	if (findIndex == -1)
	{
		return;
	}

	delete _arrPartyInviteeInfo[findIndex];
	_arrPartyInviteeInfo.RemoveAt(findIndex);
}
// 멤버 삭제
void FGsPartyManager::RemovePartyMemberInfo(int In_partySlotId)
{
	int findIndex = FindPartyMemberIndexBySlotId(In_partySlotId);
	if (findIndex == -1)
	{
		return;
	}

	delete _arrPartyMemberInfo[findIndex];
	_arrPartyMemberInfo.RemoveAt(findIndex);
}


// 리더 변경
void FGsPartyManager::LeaderChange(int In_leaderSlotId, bool In_forceShowTicker)
{
	int oldLeaderSlotId = _leaderSlotId;
	_leaderSlotId = In_leaderSlotId;
	for (FGsPartyMemberInfo* iter : _arrPartyMemberInfo)
	{
		if (iter == nullptr)
		{
			continue;
		}
		iter->LeaderChange(_leaderSlotId);
	}

	// return: not changed && not forced 
	if (In_forceShowTicker == false &&
		oldLeaderSlotId == In_leaderSlotId)
	{
		return;
	}

	FGsPartyMemberInfo* leaderInfo =  FindPartyMemberInfo(_leaderSlotId);
	if (leaderInfo == nullptr)
	{
		return;
	}

	// show chat text
	// https://jira.com2us.com/jira/browse/CHR-14141
	FGsPartyFunc::ShowChatLeaderChange(leaderInfo->GetName());
}

// 리더 변경
void FGsPartyManager::RecvChangeLeader(int In_leaderSlotId)
{
	LeaderChange(In_leaderSlotId, true);
	// 모두 갱신
	UpdateMemberUI(true);
}

// 전리품 분배룰 변경
void FGsPartyManager::RecvChangeDropOwnershipType(PartyDropOwnershipType In_type)
{
	_partyDropOwnershipType = In_type;

	// side tab 갱신
	UpdateHUDSideTab();

	// https://jira.com2us.com/jira/browse/CHR-14141
	FGsPartyFunc::ShowTickerChangeDropOwnershipType(_partyDropOwnershipType);
}

void FGsPartyManager::RecvChangedReadyState(int32 In_partySlotId, bool In_isReady)
{
	FGsPartyMemberInfo* memberInfo = FindPartyMemberInfo(In_partySlotId);
	if (nullptr == memberInfo)
	{
		GSLOG(Error, TEXT("nullptr == memberInfo"));
		return;
	}

	// 2022/06/13 PKT - 준비 상태 갱신
	memberInfo->SetComplatedReady(In_isReady);

	// 2022/06/13 PKT - 변경 된 파티원 정보 갱신(단일 갱신)
	UpdateMemberUI(false, false, In_partySlotId);
}

// 슬롯 정보 구하기
IGsPartySlotInfo* FGsPartyManager::GetPartySlotInfo(int In_partySlotId)
{
	IGsPartySlotInfo* findInfo = nullptr;
	// 1. 멤버에서 찾는다
	findInfo = FindPartyMemberInfo(In_partySlotId);
	if (findInfo != nullptr)
	{
		return findInfo;
	}
	// 2. 초대 대기에서 찾는다 
	findInfo = FindPartyInviteeInfo(In_partySlotId);
	if (findInfo != nullptr)
	{
		return findInfo;
	}
	// 3. 없으면 nullptr 리턴
	return nullptr;
}

// 파티 설정창 열기
void FGsPartyManager::OpenPartySettingUI()
{
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}
	TWeakObjectPtr<UGsUIWidgetBase> widget =
		uiMgr->OpenAndGetWidget(TEXT("PopupPartySetting"));

	if (widget.IsValid() == false)
	{
		return;
	}
	UGsUIPopupPartySetting* popupPartySetting = Cast<UGsUIPopupPartySetting>(widget.Get());

	if (popupPartySetting == nullptr)
	{
		return;
	}

}

// ui 멤버 갱신
void FGsPartyManager::UpdateMemberUI(bool In_isAll, bool In_isDrawMenuInit,int In_slotId)
{
	// 파티 설정 멤버 갱신은 파티 설장창이 열려있어야 한다
	UGsUIManager* uiManager = GUI();
	if (uiManager == nullptr)
	{
		return;
	}
	_isCloseDrawMenu = In_isDrawMenuInit;

	bool isPopupPartySettingOpened = uiManager->IsActiveWidget(TEXT("PopupPartySetting"));
	if (In_isAll == true)
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_ALL_LIST_CHANGE, nullptr);
		if (isPopupPartySettingOpened == true)
		{
			GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_SETTING_ALL_LIST_CHANGE, nullptr);
		}
	}
	else
	{
		FGsPrimitiveInt32 param(In_slotId);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_ONE_LIST_CHANGE, &param);
		if (isPopupPartySettingOpened == true)
		{
			GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_SETTING_ONE_LIST_CHANGE, &param);
		}
	}
	
}
void FGsPartyManager::UpdateSkillTargetUI(bool In_isAll, int In_slotId)
{
	UGsUIManager* uiManager = GUI();
	if (uiManager == nullptr)
	{
		return;
	}

	if (In_isAll == true)
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_SKILL_TARGET_ALL_LIST_CHANGE, nullptr);
	}
	else
	{
		FGsPrimitiveInt32 param(In_slotId);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_SKILL_TARGET_ONE_LIST_CHANGE, &param);
	}
}
// ui HUD side 탭 갱신
void FGsPartyManager::UpdateHUDSideTab()
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_SIDE_TAB_UPDATE, nullptr);
}

// ui 파티 정보 갱신
void FGsPartyManager::UpdateHUDPartyInfo()
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_INFO_UPDATE, nullptr);
}

void FGsPartyManager::OnChangedStat(int64 In_gameId, const FGsStatInfo& In_statInfo)
{
	FGsPartyMemberInfo* info = FindPartyMemberInfoByGameId(In_gameId);
	if (nullptr == info)
	{
		return;
	}

	if (In_statInfo.GetStatType() == StatType::HP)
	{
		info->SetHp(In_statInfo.GetStatValue(), In_statInfo.GetMaxStatValue());
	}
	else if(In_statInfo.GetStatType() == StatType::MP)
	{
		info->SetMp(In_statInfo.GetStatValue(), In_statInfo.GetMaxStatValue());
	}
	else if (In_statInfo.GetStatType() == StatType::HP_SHIELD)
	{
		info->SetShield(In_statInfo.GetStatValue());
	}
}
// 무기 변경
void FGsPartyManager::OnChangedWeapon(int64 In_gameId, CreatureWeaponType In_weaponType)
{
	FGsPartyMemberInfo* info = FindPartyMemberInfoByGameId(In_gameId);
	if (nullptr == info)
	{
		return;
	}

	info->SetWeaponType(In_weaponType);
}
// 해당 gameId 파티원인가
bool FGsPartyManager::IsInPartyGameId(int64 In_gameId)
{
	for (FGsPartyMemberInfo* iter : _arrPartyMemberInfo)
	{
		if (iter == nullptr)
		{
			continue;
		}
		if (iter->GetGameId() == In_gameId)
		{
			return true;
		}
	}
	return false;
}
// 파티원의 이름인가
bool FGsPartyManager::IsInPartyName(const FString& In_name)
{
	for (FGsPartyMemberInfo* iter : _arrPartyMemberInfo)
	{
		if (iter == nullptr)
		{
			continue;
		}
		if (iter->GetName() == In_name)
		{
			return true;
		}
	}
	return false;
}

// 초대중인 사람 이름인가
bool FGsPartyManager::IsPartyInviteeName(const FString& In_name)
{
	for (FGsPartyInviteeInfo* iter : _arrPartyInviteeInfo)
	{
		if (iter == nullptr)
		{
			continue;
		}
		if (iter->GetName() == In_name)
		{
			return true;
		}
	}
	return false;
}

// 게임 id로 멤버 찾기
FGsPartyMemberInfo* FGsPartyManager::FindPartyMemberInfoByGameId(int64 In_gameId)
{
	for (FGsPartyMemberInfo* iter : _arrPartyMemberInfo)
	{
		if (iter == nullptr)
		{
			continue;
		}
		if (iter->GetGameId() == In_gameId)
		{
			return iter;
		}
	}
	return nullptr;
}


// 레벨 변경
void FGsPartyManager::OnChangedLevel(int64 In_gameId, int In_newLevel)
{
	FGsPartyMemberInfo* info = FindPartyMemberInfoByGameId(In_gameId);
	if (nullptr == info)
	{
		return;
	}

	if (info->GetLevel() != INVALID_LEVEL)
	{
		info->SetLevel(In_newLevel);
	}
}

// 파티 멤버 인원 수
int FGsPartyManager::GetPartyMemberCount()
{
	int count = _arrPartyMemberInfo.Num();
	return count;
}

// 채널 변경
void FGsPartyManager::OnChangedMapIdAndChannel(int64 In_gameId, int In_newMapId ,int In_newChannelId)
{
	// 파티가 아니면 리턴
	if (GetIsInParty() == false)
	{
		return;
	}

	FGsPartyMemberInfo* info = FindPartyMemberInfoByGameId(In_gameId);
	if (nullptr == info)
	{
		return;
	}

	info->SetChannelId(In_newChannelId);
	info->SetMapId(In_newMapId);
}

// 디스폰 변경
void FGsPartyManager::OnDespawnChange(int64 In_gameId, bool In_isDespawn)
{
	// 파티가 아니면 리턴
	if (GetIsInParty() == false)
	{
		return;
	}

	FGsPartyMemberInfo* info = FindPartyMemberInfoByGameId(In_gameId);
	if (nullptr == info)
	{
		return;
	}

	info->SetIsDespawn(In_isDespawn);
}

// 스폰
void FGsPartyManager::OnSpawn(int64 In_gameId)
{
	// 파티가 아니면 리턴
	if (GetIsInParty() == false)
	{
		return;
	}
	FGsPartyMemberInfo* info = FindPartyMemberInfoByGameId(In_gameId);
	if (nullptr == info)
	{
		return;
	}

	info->SetIsSpawned(true);
}
// 디스폰
void FGsPartyManager::OnDespawn(int64 In_gameId)
{
	// 파티가 아니면 리턴
	if (GetIsInParty() == false)
	{
		return;
	}
	FGsPartyMemberInfo* info = FindPartyMemberInfoByGameId(In_gameId);
	if (nullptr == info)
	{
		return;
	}

	info->SetIsSpawned(false);
}
// 초대 밴 정보 추가
void FGsPartyManager::AddInviteBanInfo(const FString& In_name)
{
	FGsPartyInviteBanInfo* findInfo = _mapInviteBanInfo.FindRef(In_name);
	// 기존에 없으면 추가
	if (findInfo == nullptr)
	{
		findInfo = new FGsPartyInviteBanInfo(In_name);
		_mapInviteBanInfo.Add(In_name , findInfo);
	}
	// 있다면 시간 추가
	else
	{
		findInfo->AddInviteTime();
	}
}

// 초대 밴 정보 삭제
void FGsPartyManager::DelInviteBanInfo(const FString& In_name)
{
	FGsPartyInviteBanInfo* delInfo =
		_mapInviteBanInfo.FindRef(In_name);

	if (delInfo != nullptr)
	{
		delete delInfo;
	}

	_mapInviteBanInfo.Remove(In_name);
}

// 초대 밴 대상인지
// 저대상한테 너무 많이 보냄(60초내에 4번 이상)
bool FGsPartyManager::IsInviteBanTarget(const FString& In_name)
{
	FGsPartyInviteBanInfo* findInfo = _mapInviteBanInfo.FindRef(In_name);

	if (findInfo == nullptr)
	{
		return false;
	}

	return findInfo->GetIsBan();
}

// 파티장으로 워프 가능한지
// 1. (다른맵||다른 채널)
// 2. not other world id with leader
EGsPartyWarpToLeaderResult FGsPartyManager::IsPossibleWarpToLeader(bool& Out_diffMap)
{
	if (GetIsInParty() == false ||
		AmILeader() == true)
	{
		return EGsPartyWarpToLeaderResult::NoData;
	}

	FGsPartyMemberInfo* leaderInfo = FindLeaderPartyMemberInfo();
	if (leaderInfo == nullptr)
	{
		return EGsPartyWarpToLeaderResult::NoData;
	}

	FGsPartyMemberInfo* myInfo = FindMyPartyMemberInfo();
	if (myInfo == nullptr)
	{
		return EGsPartyWarpToLeaderResult::NoData;
	}
	bool isSameMapId = (leaderInfo->GetMapId() == myInfo->GetMapId()) ? true : false;
	// 다른맵인지(같은 맵인지 반대)
	Out_diffMap = !isSameMapId;


	// 1. check world id
	WorldId leaderWorldId = leaderInfo->GetWorldId();
	WorldId localNowMapWorldId = GGameData()->GetLoadWorldId();

	// 1. set data
	// 2. diff local
	if (leaderWorldId != 0 &&
		leaderWorldId != localNowMapWorldId)
	{
		return EGsPartyWarpToLeaderResult::DifferentWorldId;
	}


	// 리더와 같은맵, 같은 채널이면 할필요 없음
	if (leaderInfo->GetChannelId() == myInfo->GetChannelId() &&
		isSameMapId)
	{
		return EGsPartyWarpToLeaderResult::SameChannelIdAndSameMapeId;
	}

	return EGsPartyWarpToLeaderResult::WarpPossible;
}

// 내 파티 정보 가져오기
FGsPartyMemberInfo* FGsPartyManager::FindMyPartyMemberInfo()
{
	return FindPartyMemberInfo(_mySlotId);
}

// 리더 파티 정보 가져오기
FGsPartyMemberInfo* FGsPartyManager::FindLeaderPartyMemberInfo()
{
	return FindPartyMemberInfo(_leaderSlotId);;
}