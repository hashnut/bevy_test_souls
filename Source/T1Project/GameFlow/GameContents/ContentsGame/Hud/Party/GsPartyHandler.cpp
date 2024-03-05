#include "GsPartyHandler.h"

#include "Party/GsPartyMemberInfo.h"
#include "Party/GsPartyInviterInfo.h"
#include "Party/GsPartyFunc.h"

#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDPartyTargetButton.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContentDungeon.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"

#include "ActorEx/GsCharacterBase.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/HUD/GsHUDLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "DataSchema/Skill/GsSchemaSkillSet.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Public/Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendService.h"

#include "GameFramework/PlayerController.h"


// 생성 응답
void FGsPartyHandler::RecvPartyCreate(PartyDropOwnershipType In_type)
{
	FGsPartyMemberInfo* myInfo = MakeMyInfo(0, true);
	GSParty()->RecvPartyCreate(myInfo, In_type);
	

	// 모두 갱신
	GSParty()->UpdateMemberUI(true);
	// not update skill target(only me)
	
	// side tab 갱신
	GSParty()->UpdateHUDSideTab();
	GSParty()->UpdateHUDPartyInfo();
	
	// 1P, 2P... 표시때문에 갱신
	UpdateNameplate(myInfo->GetGameId());

	// update show party target button
	UpdatePartyTargetButtonHUDMode();
}

// 내정보 만들기
FGsPartyMemberInfo* FGsPartyHandler::MakeMyInfo(int In_slotId, bool In_isLeader)
{
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return nullptr;
	}
	FGsGameObjectDataLocalPlayer* playerData =
		local->GetCastData<FGsGameObjectDataLocalPlayer>();

	if (playerData == nullptr)
	{
		return nullptr;
	}
	int64 localGameId = playerData->GetGameId();
	FString localName = playerData->GetName();
	int localLevel = playerData->GetLevel();
	CreatureWeaponType weaponType = playerData->GetCurrentWeaponType();

	UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(local);
	const FGsStatInfo* hpInfo = localPlayer->GetCreatureStatInfoByType(StatType::HP);
	if (hpInfo == nullptr)
	{
		return nullptr;
	}
	const FGsStatInfo* shieldInfo = localPlayer->GetCreatureStatInfoByType(StatType::HP_SHIELD);
	if (shieldInfo == nullptr)
	{
		return nullptr;
	}
	const FGsStatInfo* mpInfo = localPlayer->GetCreatureStatInfoByType(StatType::MP);
	if (mpInfo == nullptr)
	{
		return nullptr;
	}

	int nowHP = hpInfo->GetStatValue();
	int maxHP = hpInfo->GetMaxStatValue();

	int nowMP = mpInfo->GetStatValue();
	int maxMP = mpInfo->GetMaxStatValue();

	int currentChannel = GGameData()->GetCurrentChannel();
	int currentLevel = GLevel()->GetCurrentLevelId();
	FVector currentPos = localPlayer->GetLocation();

	WorldId localNowMapWorldId = GGameData()->GetLoadWorldId();
	WorldId localHomeWorldId = GGameData()->GetUserData()->mHomeWorldId;

	FGsPartyMemberInfo* myInfo =
		new FGsPartyMemberInfo(
			localGameId, In_isLeader, localName, localLevel,
			nowHP, maxHP, shieldInfo->GetStatValue(), nowMP, maxMP, In_slotId,
			weaponType, currentChannel, currentLevel, currentPos, true, false, localNowMapWorldId, localHomeWorldId);

	return myInfo;
}


// 초대 수락
void FGsPartyHandler::RecvPartyJoined(PD::GC::PKT_GC_PARTY_JOINED_READ& In_packet,
	TMap<int64, bool> In_mapSpawnedInfo)
{
	int mySlotId = In_packet.MySlotIndex();
	FGsPartyMemberInfo* myInfo = MakeMyInfo(mySlotId, false);

	GSParty()->RecvPartyJoined(In_packet, In_mapSpawnedInfo, myInfo);

	// 전체 갱신
	GSParty()->UpdateMemberUI(true);
	GSParty()->UpdateSkillTargetUI(true);

	// side tab 갱신
	GSParty()->UpdateHUDSideTab();
	GSParty()->UpdateHUDPartyInfo();
	// 미니맵 전송 파람
	FGsUIMapPartyMemberParam param;

	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return;
	}

	FGsTargetHandlerBase* targetHandler = local->GetTargetHandler();
	if (targetHandler == nullptr)
	{
		return;
	}

	UGsGameObjectBase* targetObj = targetHandler->GetTarget();
	int64 targetGameId = -1;
	if (targetObj != nullptr)
	{
		targetGameId = targetObj->GetGameId();
	}
	

	for (PD::GC::PKT_GC_PARTY_JOINED_READ::PartyMemberInfoListIterator iter =
		In_packet.GetFirstPartyMemberInfoListIterator();
		iter != In_packet.GetLastPartyMemberInfoListIterator();
		++iter)
	{
		int64 gameId = iter->GameId();
		// 미니맵 정보 추가
		param._gameId.Add(gameId);

		// 추가된 파티원중 타겟이 있다면 타겟에서 삭제
		if (targetGameId == gameId)
		{
			targetHandler->ClearTarget();
		}
	}

	// 네임플레이트 갱신
	UpdateNameplate(local->GetGameId());

	for (int64& gameId : param._gameId)
	{
		UpdateNameplate(gameId);
	}	

	// 파티원 미니맵 추가
	param._isAdd = true;
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_PARTY_MEMBER, &param);

	GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_PARTY_MEMBER, &param);

	// update show party target button
	UpdatePartyTargetButtonHUDMode();
}

void FGsPartyHandler::UpdateNameplate(int64 InGameId)
{
	UGsGameObjectBase* playerBase = GSGameObject()->FindObject(EGsGameObjectType::Player, InGameId);
	if (nullptr == playerBase)
	{
		return;
	}

	UGsGameObjectPlayer* player = Cast<UGsGameObjectPlayer>(playerBase);
	if (nullptr == player)
	{
		return;
	}

	player->UpdateBillboard(EGsBillboardUpdateType::All);
}

void FGsPartyHandler::Init()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
				&FGsPartyHandler::OnLocalPlayerSpawnComplete));

		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_REVIVE, this,
				&FGsPartyHandler::OnLocalPlayerSpawnComplete));

		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::COMMON_DESPAWN, this,
				&FGsPartyHandler::OnCommonDespawn));

		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::COMMON_SPAWN, this,
				&FGsPartyHandler::OnCommonSpawn));

		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::COMMON_DIE, this,
				&FGsPartyHandler::OnCommonDie));
	}
}

void FGsPartyHandler::Close()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_gameObjectMsg.Num() != 0)
		{
			for (auto iter : _gameObjectMsg)
			{
				msg->GetGameObject().Remove(iter);
			}
			_gameObjectMsg.Empty();
		}
	}
}

// 로컬 스폰 끝
void FGsPartyHandler::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	// https://jira.com2us.com/jira/browse/C2URWQ-3653
	GSParty()->UpdateHUDSideTab();

	// 파티원이 아니면 리턴
	if (GSParty()->GetIsInParty() == false)
	{
		return;
	}

	const TArray<FGsPartyMemberInfo* > arrMembmerInfo = 
		GSParty()->GetAllPartyMemberInfo();

	// 파티원이 1명 이하면 리턴(나만 있는거니까)
	if (arrMembmerInfo.Num() <= 1)
	{
		return;
	}

	FGsUIMapPartyMemberParam param;
	param._isAdd = true;

	// check server id(with local server id)
	WorldId localNowMapWorldId = GGameData()->GetLoadWorldId();

	for (FGsPartyMemberInfo* iter : arrMembmerInfo)
	{
		if (nullptr == iter)
		{
			continue;
		}

		// 내정본 추가 안한다
		if (GSParty()->IsMySlotId(iter->GetPartySlotId()) == true)
		{
			continue;
		}

		UGsGameObjectBase* findObj =
			GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, iter->GetGameId());
		// gameobject가 있으면 거기서 만듬
		if (findObj != nullptr)
		{
			continue;
		}

		WorldId partyMemberWorldId = iter->GetWorldId();

		// 1. set data
		// 2. diff local
		if (partyMemberWorldId != 0 &&
			partyMemberWorldId != localNowMapWorldId)
		{
			// not make party icon
			continue;
		}

		param._gameId.Add(iter->GetGameId());
	}


	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_PARTY_MEMBER, &param);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_PARTY_MEMBER, &param);
	
	UpdateSharedIdState();
}

// 멤버 추가(초대 수락)
void FGsPartyHandler::RecvPartyMemberJoined(PD::GC::PKT_GC_PARTY_MEMBER_JOINED_READ& In_packet,
	bool In_isSpawned)
{
	int64 gameId = In_packet.GameId();
	int slotId = In_packet.SlotIndex();
		
	GSParty()->SetPartyMembmerInfo(gameId,
		In_packet.Name(),
		In_packet.Level(),
		In_packet.Hp(), In_packet.MaxHp(), In_packet.HpShield(),
		In_packet.Mp(), In_packet.MaxMp(),
		slotId, In_packet.WeaponType(),
		In_packet.ChannelId(), In_packet.MapId(), In_packet.Pos(), In_isSpawned, 
		// not ready server data(world id)
		0, 
		In_packet.HomeWorldId());

	// 추가된 파티원이 타겟이라면 타겟에서 삭제
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return;
	}

	FGsTargetHandlerBase* targetHandler = local->GetTargetHandler();
	if (targetHandler == nullptr)
	{
		return;
	}

	UGsGameObjectBase* targetObj = targetHandler->GetTarget();
	int64 targetGameId = -1;
	if (targetObj != nullptr)
	{
		targetGameId = targetObj->GetGameId();
	}

	if (gameId == targetGameId)
	{
		targetHandler->ClearTarget();
	}

	// 2022/07/27 PKT -  파티 멤버 추가
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ADD_PARTY_MEMBER, nullptr);
	// 초대 대기 정보 삭제
	GSParty()->RemovePartyInviteeInfo(slotId);
	// 단일 갱신
	GSParty()->UpdateMemberUI(false, true, slotId);
	GSParty()->UpdateSkillTargetUI(true);
	// side tab 갱신
	GSParty()->UpdateHUDSideTab();
	GSParty()->UpdateHUDPartyInfo();
	// 네임플레이트 갱신
	UpdateNameplate(gameId);

	// 파티원 미니맵 추가
	FGsUIMapPartyMemberParam param;
	param._isAdd = true;
	param._gameId.Add(gameId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_PARTY_MEMBER, &param);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_PARTY_MEMBER, &param);
}

// 멤버 갱신
void FGsPartyHandler::RecvPartyMemberUpdate(PD::GC::PKT_GC_PARTY_MEMBER_UPDATED_READ& In_packet,
	bool In_isSpawned)
{
	int64 gameId = In_packet.GameId();
	int slotId = In_packet.SlotIndex();
	int channelId = In_packet.ChannelId();
	int mapId = In_packet.MapId();

	WorldId partyMemberWorldId = In_packet.ServerId();
	// check server id(with local server id)
	WorldId localNowMapWorldId = GGameData()->GetLoadWorldId();

	bool isDiffrentWorldId = (partyMemberWorldId == localNowMapWorldId) ? false : true;

	FGsPartyMemberInfo* oldMemberInfo = GSParty()->FindPartyMemberInfo(slotId);

	EGsPartyIconChangeState iconChangeState = EGsPartyIconChangeState::NotChange;
	if (oldMemberInfo != nullptr)
	{
		// 채널이 바뀌었는지 체크
		bool isChannelChanged =
			(oldMemberInfo->GetChannelId() != channelId) ? true : false;

		// 맵이 이전정보와 다른가
		int oldMapId = oldMemberInfo->GetMapId();
		bool isMapChanged =
			(oldMapId != mapId) ? true : false;

		bool isDespawned = false;
		if (oldMemberInfo->GetIsDespawn() == true)
		{
			isDespawned = true;
			oldMemberInfo->SetIsDespawn(false);
		}

		if (isMapChanged == true)
		{
			int32 currentLevelId = 0;
			currentLevelId = GLevel()->GetCurrentLevelId();

			// 이전엔 현재맵이었다가 다른 맵으로 갔으면 삭제
			if (oldMapId == currentLevelId &&
				mapId != currentLevelId)
			{
				iconChangeState = EGsPartyIconChangeState::Del;
			}
			// 이전엔 다른맵이었다가 현재 맵으로 왔으면 추가
			else if (oldMapId != currentLevelId &&
				mapId == currentLevelId)
			{
				iconChangeState = EGsPartyIconChangeState::Add;
			}
			// 채널이 변경 되었다면
			else if (isChannelChanged == true)
			{
				iconChangeState = EGsPartyIconChangeState::Add;
			}
		}
		// 채널이 바뀌었던가 아니면 디스폰이었던가
		else if (isChannelChanged == true ||
			isDespawned == true)
		{
			iconChangeState = EGsPartyIconChangeState::Add;
		}
	}

	//https://jira.com2us.com/jira/browse/C2URWQ-663
	// not show other server gone party member
	if (iconChangeState != EGsPartyIconChangeState::Del &&
		isDiffrentWorldId == true)
	{
		if (oldMemberInfo != nullptr)
		{
			WorldId oldWorldId = oldMemberInfo->GetWorldId();

			if ((oldWorldId == 0) ||
				(oldWorldId != partyMemberWorldId))				
			{
				iconChangeState = EGsPartyIconChangeState::Del;
			}
			else
			{
				iconChangeState = EGsPartyIconChangeState::NotChange;
			}
		}
		else
		{
			iconChangeState = EGsPartyIconChangeState::Del;
		}
	}


	GSParty()->SetPartyMembmerInfo(gameId,
		In_packet.Name(),
		In_packet.Level(),
		In_packet.Hp(), In_packet.MaxHp(), In_packet.HpShield(),
		In_packet.Mp(), In_packet.MaxMp(),
		slotId, In_packet.WeaponType(),
		channelId, mapId,
		In_packet.Pos(),
		In_isSpawned, 
		partyMemberWorldId,
		In_packet.HomeWorldId());

	// 단일 갱신
	GSParty()->UpdateMemberUI(false, false, slotId);
	GSParty()->UpdateSkillTargetUI(false, slotId);
	// 네임 플레이트 갱신
	UpdateNameplate(gameId);

	if (iconChangeState != EGsPartyIconChangeState::NotChange)
	{
		// 파티원 미니맵 갱신(아이콘 갱신)
		FGsUIMapPartyMemberParam param;
		param._isAdd = (iconChangeState == EGsPartyIconChangeState::Add) ?
			true : false;
		param._gameId.Add(gameId);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_PARTY_MEMBER, &param);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_PARTY_MEMBER, &param);
	}
}

// 파티 나감(내가)
void FGsPartyHandler::RecvPartyLeave()
{
	// 파티원 미니맵 삭제

	FGsUIMapPartyMemberParam param;
	param._isAdd = false;

	const TArray<FGsPartyMemberInfo* > arrPartyMemberInfo =
		GSParty()->GetAllPartyMemberInfo();

	for (FGsPartyMemberInfo* iter : arrPartyMemberInfo)
	{
		if (nullptr == iter)
		{
			continue;
		}
		param._gameId.Add(iter->GetGameId());
	}
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_PARTY_MEMBER, &param);

	// 2022/06/19 PKT - 파티가 깨졌다.
	GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_BREAKS_UP, &param);

	GSParty()->ClearData();

	// call after data changed
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_PARTY_MEMBER, &param);

	// 파티 세팅창 닫기
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}

	uiMgr->CloseByKeyName(TEXT("PopupPartySetting"));

	// 전체 갱신
	GSParty()->UpdateMemberUI(true);
	GSParty()->UpdateSkillTargetUI(true);
	// side tab 갱신
	GSParty()->UpdateHUDSideTab();
	GSParty()->UpdateHUDPartyInfo();
	// 네임플레이트 갱신
	for (int64& gameId : param._gameId)
	{
		UpdateNameplate(gameId);
	}

	// update show party target button
	UpdatePartyTargetButtonHUDMode();

	// clear shared party target
	ChangedSharePartyTargetId(-1);
}
// 파티원 나감
void FGsPartyHandler::RecvPartyMemberLeft(int In_leftSlotId, int In_leaderSlotId)
{
	// 파티원 미니맵 삭제
	FGsPartyMemberInfo* findInfo = GSParty()->FindPartyMemberInfo(In_leftSlotId);
	int64 targetGameId = -1;

	FGsUIMapPartyMemberParam param;
	if (findInfo != nullptr)
	{
		targetGameId = findInfo->GetGameId();

		param._isAdd = false;
		param._gameId.Add(targetGameId);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_PARTY_MEMBER, &param);
	}

	GSParty()->RemovePartyMemberInfo(In_leftSlotId);
	// https://jira.com2us.com/jira/browse/C2URWQ-3149
	// old server code
	// 1. GC_PARTY_MEMBER_LEFT(new leader slot id)
	
	// new server code
	// 1. GC_PARTY_MEMBER_LEFT(new leader slot id)
	// 2. GC_PARTY_LEADER_CHANGED(new leader slot id)
	// 
	// not need leader change chat show here
	//GSParty()->LeaderChange(In_leaderSlotId);

	if (param._gameId.Num() != 0)
	{
		// call after data changed
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_PARTY_MEMBER, &param);
	}

	// 모두 갱신
	GSParty()->UpdateMemberUI(true);
	GSParty()->UpdateSkillTargetUI(true);
	// side tab 갱신
	GSParty()->UpdateHUDSideTab();
	GSParty()->UpdateHUDPartyInfo();
	// 네임플레이트 갱신. 호출 타이밍 주의(RemovePartyMemberInfo 이후)
	// dont use deleted pointer-> garbage value
	// https://jira.com2us.com/jira/browse/C2URWQ-726	
	UpdateNameplate(targetGameId);
	
}
// 멤버중 하나 추방당함
void FGsPartyHandler::RecvPartyKick(int In_kickSlotId)
{
	// 파티원 미니맵 삭제
	FGsPartyMemberInfo* findInfo = GSParty()->FindPartyMemberInfo(In_kickSlotId);
	int64 targetGameId = -1;
	FGsUIMapPartyMemberParam param;
	if (findInfo != nullptr)
	{
		targetGameId = findInfo->GetGameId();
		
		param._isAdd = false;
		param._gameId.Add(targetGameId);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_PARTY_MEMBER, &param);

		// 티커 출력
		FGsPartyFunc::ShowTickerKicked(findInfo->GetName());
	}		

	GSParty()->RemovePartyMemberInfo(In_kickSlotId);

	if (param._gameId.Num() != 0)
	{
		// call after data changed
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_PARTY_MEMBER, &param);
	}

	// 단일 갱신
	GSParty()->UpdateMemberUI(false, false, In_kickSlotId);
	GSParty()->UpdateSkillTargetUI(true);
	// side tab 갱신
	GSParty()->UpdateHUDSideTab();
	GSParty()->UpdateHUDPartyInfo();
	// 네임플레이트 갱신. 호출 타이밍 주의(RemovePartyMemberInfo 이후)	
	// dont use deleted pointer-> garbage value
	// https://jira.com2us.com/jira/browse/C2URWQ-726
	UpdateNameplate(targetGameId);

}

// 추방 당함
void FGsPartyHandler::RecvPartyKicked()
{
	// 파티원 미니맵 삭제

	FGsUIMapPartyMemberParam param;
	param._isAdd = false;

	const TArray<FGsPartyMemberInfo* > arrPartyMemberInfo =
		GSParty()->GetAllPartyMemberInfo();

	for (FGsPartyMemberInfo* iter : arrPartyMemberInfo)
	{
		if (nullptr == iter)
		{
			continue;
		}
		param._gameId.Add(iter->GetGameId());
	}
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_PARTY_MEMBER, &param);

	// 내가 추방당한거 출력
	if (FGsPartyMemberInfo* memberInfo = GSParty()->FindMyPartyMemberInfo())
	{
		FGsPartyFunc::ShowTickerKicked(memberInfo->GetName());
	}

	// 2022/06/30 PKT - 리더이면서 파티 던전 상태 중이였다면 별도 처리 해야 함.(파티 정보가 초기화 되기 전에 처리 해야 함!!)
	GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_KICK, &param);

	GSParty()->ClearData();

	// call after data changed
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_PARTY_MEMBER, &param);

	// 파티 세팅창 닫기
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}

	uiMgr->CloseByKeyName(TEXT("PopupPartySetting"));

	// 전체 갱신
	GSParty()->UpdateMemberUI(true);
	GSParty()->UpdateSkillTargetUI(true);
	// side tab 갱신
	GSParty()->UpdateHUDSideTab();
	GSParty()->UpdateHUDPartyInfo();
	// 네임플레이트 생신
	for (int64& gameId : param._gameId)
	{
		UpdateNameplate(gameId);
	}

	// update show party target button
	UpdatePartyTargetButtonHUDMode();

	// clear shared party target
	ChangedSharePartyTargetId(-1);
}

// 초대 거절 처리
void FGsPartyHandler::CancelInvite()
{
	FGsPartyInviterInfo info = GSParty()->GetPartyInviteInfo();
	if (info.GetIsInviterExist() == false)
	{
		return;
	}

	// 서버에 거절 처리 요청
	FGsNetSendService::SendPartyConfirmInvitation(false);
}

void FGsPartyHandler::RecvPartyMemberReadyState(PD::GC::PKT_GC_NOTIFY_COOP_PARTY_READY_STATUS_READ& InPacket)
{	// 2022/06/13 PKT - 파티 원들에 대한 준비 상태
	FGsPartyManager* partyManager = GSParty();
	if (partyManager)
	{
		partyManager->RecvChangedReadyState(InPacket.SlotIndex(), InPacket.IsReady());
	}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_INFO_UPDATE, nullptr);
}

void FGsPartyHandler::RecvPartyReadyState(PD::GC::PKT_GC_ACK_COOP_PARTY_READY_READ& InPacket)
{	// 2022/06/13 PKT - 내 준비 상태에 대한 응답
	FGsPartyManager* partyManager = GSParty();
	FGsPartyMemberInfo* myInfo = (nullptr != partyManager) ? partyManager->FindMyPartyMemberInfo() : nullptr;
	if (myInfo)
	{
		int32 partySlotId = myInfo->GetPartySlotId();
		partyManager->RecvChangedReadyState(partySlotId, InPacket.IsReady());
	}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_INFO_UPDATE, nullptr);
}

void FGsPartyHandler::OnClickPartyTargetButton(bool In_isActive)
{
	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (localBase == nullptr)
	{
		return;
	}
	FGsTargetHandlerLocalPlayer* targetHandler = localBase->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	if (targetHandler == nullptr)
	{
		return;
	}


	// 1. i am leader
	if (true == GSParty()->AmILeader())
	{
		if (In_isActive == true)
		{
			// sever send
			FGsNetSendServiceWorld::SendPartyTargetShareCancel();
		}
		else
		{
			// server send

			// find last target
			UGsGameObjectBase* lastTarget = targetHandler->GetLastTarget();
			if (lastTarget == nullptr)
			{
				return;
			}
			int64 gameId = lastTarget->GetGameId();

			//https://jira.com2us.com/jira/browse/C2URWQ-1764
			// check no party member
			if (true == GSParty()->IsInPartyGameId(gameId))
			{
				FGsPartyFunc::ShowTickerPartyMemberTarget();
				return;
			}

			FGsNetSendServiceWorld::SendPartyTargetShare(gameId);
		}
	}
	// 2. i am member
	else
	{
		// empty shared target
		if (In_isActive == false)
		{
			// team member try share(how dare!!)
			FGsPartyFunc::ShowTickerTryShareByPartyMember();
		}
		// attack shared target
		else
		{
			int64 sharedTargetId = GSParty()->GetSharedPartyTargetId();
			if (sharedTargetId == -1)
			{
				return;
			}
			UGsGameObjectBase* findTarget =
				GSGameObject()->FindObject(EGsGameObjectType::Creature, sharedTargetId);

			if (findTarget == nullptr)
			{
				return;
			}

			targetHandler->SetTarget(findTarget, false, FGsTargetHandlerBase::SelectTargetFilter::LockOn);
			UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>();
			if (local == nullptr)
			{
				return;
			}
			// 락온일경우 스킬실행
			FGsSkillHandlerLocalPlayer* skillHandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>();
			if (skillHandler == nullptr)
			{
				return;
			}

			const FGsSchemaSkillSet* firstSkillData = skillHandler->GetFirstComboData();
			if (firstSkillData == nullptr)
			{
				return;
			}

			skillHandler->OnSkill(firstSkillData->id);
		}
	}
}

// server recv party target
void FGsPartyHandler::RecvPartyTargetClick(bool In_isActive, int64 In_targetServerId)
{
	ChangedSharePartyTargetId(In_targetServerId);
	UpdateSharedIdState();
}
void FGsPartyHandler::UpdateSharedIdState(UGsGameObjectBase* In_findObj)
{
	int64 sharedTargetId = GSParty()->GetSharedPartyTargetId();
	// none shared
	bool isShared = (sharedTargetId == -1) ? false : true;

	UGsGameObjectBase* findTarget = In_findObj;
	if (sharedTargetId != -1 && 
		In_findObj == nullptr)
	{
		findTarget =
			GSGameObject()->FindObject(EGsGameObjectType::Creature, sharedTargetId);		
	}

	// not exist target-> share fail
	if (findTarget == nullptr)
	{
		isShared = false;
	}
	else
	{		
		// dead state(is zero hp)
		if (findTarget->IsZeroHP() == true)
		{
			isShared = false;
		}		
	}

	
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("HUDMain"), true);
	if (widget.IsValid())
	{
		UGsUIHUDFrameMain* hudWidget = Cast<UGsUIHUDFrameMain>(widget.Get());
		if (hudWidget)
		{
			if (UGsUIHUDPartyTargetButton* partyTargetButton = hudWidget->GetPartyTargetButton())
			{
				partyTargetButton->SetIsActive(isShared);

				if (isShared == true)
				{
					FString targetName;
					if (findTarget != nullptr)
					{
						if (FGsGameObjectData* data = findTarget->GetData())
						{
							targetName = data->GetName();
						}
					}
					partyTargetButton->SetTargetName(targetName);
				}
			}
		}
	}
}

void FGsPartyHandler::UpdatePartyTargetButtonHUDMode()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return;
	}
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("HUDMain"), true);
	if (widget.IsValid())
	{
		UGsUIHUDFrameMain* hudWidget = Cast<UGsUIHUDFrameMain>(widget.Get());
		if (hudWidget)
		{
			if (UGsUIHUDPartyTargetButton* partyTargetButton = hudWidget->GetPartyTargetButton())
			{
				partyTargetButton->SetHUDMode(uiManager->GetHUDMode());
			}
		}
	}
}
// common despawn message
void FGsPartyHandler::OnCommonDespawn(const IGsMessageParam* inParam)
{
	// return if not in party 
	if (false == GSParty()->GetIsInParty())
	{
		return;
	}

	// return if  no shared id
	int64 sharedTargetId = GSParty()->GetSharedPartyTargetId();
	if (sharedTargetId == -1)
	{
		return;
	}

	const FGsGameObjectMessageParam* castParam = inParam->Cast<const FGsGameObjectMessageParam>();
	// 대상이 같으면 클리어
	if (UGsGameObjectBase* owner = castParam->_paramOwner)
	{
		if (FGsGameObjectData* data = owner->GetData())
		{
			if (data->GetGameId() == sharedTargetId)
			{
				// despawn not clear
				UpdateSharedIdState();
			}
		}
	}
}

void FGsPartyHandler::ChangedSharePartyTargetId(int64 In_id)
{
	int64 oldSharedTargetId = GSParty()->GetSharedPartyTargetId();

	UGsGameObjectBase* oldFindTarget =
		GSGameObject()->FindObject(EGsGameObjectType::Creature, oldSharedTargetId);

	if (oldFindTarget != nullptr)
	{
		oldFindTarget->SetPartySharedTargetActive(false);
	}

	GSParty()->SetSharedPartyTargetId(In_id);

	UGsGameObjectBase* newFindTarget =
		GSGameObject()->FindObject(EGsGameObjectType::Creature, In_id);

	if (newFindTarget != nullptr)
	{
		newFindTarget->SetPartySharedTargetActive(true);
	}
}

// common spawn message
void FGsPartyHandler::OnCommonSpawn(const IGsMessageParam* inParam)
{
	// return if not in party 
	if (false == GSParty()->GetIsInParty())
	{
		return;
	}

	// return if  no shared id
	int64 sharedTargetId = GSParty()->GetSharedPartyTargetId();
	if (sharedTargetId == -1)
	{
		return;
	}

	const FGsGameObjectMessageParam* castParam = inParam->Cast<const FGsGameObjectMessageParam>();
	if (UGsGameObjectBase* owner = castParam->_paramOwner)
	{
		if (FGsGameObjectData* data = owner->GetData())
		{
			if (data->GetGameId() == sharedTargetId)
			{
				UpdateSharedIdState(owner);
			}
		}
	}
}

void FGsPartyHandler::OnCommonDie(const IGsMessageParam* inParam)
{
	// return if not in party 
	if (false == GSParty()->GetIsInParty())
	{
		return;
	}

	// return if  no shared id
	int64 sharedTargetId = GSParty()->GetSharedPartyTargetId();
	if (sharedTargetId == -1)
	{
		return;
	}

	const FGsGameObjectMessageParam* castParam = inParam->Cast<const FGsGameObjectMessageParam>();
	// 대상이 같으면 클리어
	if (UGsGameObjectBase* owner = castParam->_paramOwner)
	{
		if (FGsGameObjectData* data = owner->GetData())
		{
			if (data->GetGameId() == sharedTargetId)
			{
				UpdateSharedIdState();
			}
		}
	}
}

void FGsPartyHandler::RecvClearInviterInfo()
{
	// 2023/7/17 PKT - 초대자 정보 초기화
	GSParty()->GetPartyInviteInfo().ClearInviter();
}

// 초대 수락 실패
void FGsPartyHandler::RecvPartyJoinedFailed()
{
	GSParty()->ClearData();
}