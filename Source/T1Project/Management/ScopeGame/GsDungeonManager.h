// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Dungeon/GsDungeonGroupBase.h"

#include "../GsScopeGame.h"
#include "../GsScopeHolder.h"

#include "Classes/GsManager.h"

#include "RedDot/GsRedDotCheck.h"

#include "../Dungeon/GsDungeonReserveWarpHandler.h"

#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"

#include "Message/GsMessageSystem.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContentDungeon.h"

#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"


class T1PROJECT_API FGsDungeonManager final : public IGsRedDotCheck, public IGsManager
{
private:
	
	// 2022/10/25 PKT - Dungeon Group Holder
	TArray<TSharedPtr<FGsDungeonGroupBase>> _dungeonGroupHolder;
	// 2022/10/25 PKT - message Holder
	uint32 _messageHolder = 0;
	// 2022/10/26 PKT - Warp Handler
	FGsDungeonReserveWarpHandler _warpHandler;
	// 2023/2/21 PKT - View Data
	mutable TArray<const FGsDungeonGroupBase*> _viewGroupSet;

	// 2022/10/25 PKT - 파티 던전 Config Data
	const struct FGsSchemaCoopPartyDungeonConfig* _configPartyOnly = nullptr;

private:
	// 2022/10/25 PKT - 캐릭터 스폰 후 메세지 받음
	MsgGameObjHandle _gameObjectDelegate;
	// 2022/06/09 PKT - Reset Event
	MsgSystemHandle _systemDelegate;

	// 2023/7/31 PKT - 시련의 전당 Unlock Contents
	TPair<MessageContentUnlock, FDelegateHandle> _unlockDelegate;

	// 2023/9/19 PKT - 각 컨텐츠 마다 던전 들의 Tab을 Hidden해야 할 때가 있어, 별도로 만련 해 둠.
	TMap<DungeonCategory, EGsUnlockContentsType> _hiddenMainTabWithUnlockContentsType;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

	// 2022/10/25 PKT - 던전 시간
	void SyncDungeonGroupTime(DungeonGroupId InDungeonGroupId, int32 InNormalTime, int32 InChargeTime);


public:
	// 2022/10/26 PKT - RedDot Interface	
	virtual bool CheckRedDot(bool bInUpdate = true) override;
	virtual bool CheckRedDot(DungeonCategory InCategoryType) const;	

private:	
	// 2022/10/25 PKT - Find Data By Group Id
	FGsDungeonGroupBase* FindGroupByGroupId(DungeonGroupId InDungeonGroupId) const;
	// 2022/10/25 PKT - Find Data By Group Id
	const FGsDungeonGroupBase* FindGroupByMapId(MapId InMapId) const;
	// 2022/10/26 PKT - Find Dungeon Data By MapId
	const FGsDungeonData* FindDungeonByMapId(MapId InMapId, const FGsDungeonGroupBase* InGroupData = nullptr) const;

	// 2022/10/25 PKT - Send Window Message
	void SendWindowMessage(MessageContentDungeon InType = MessageContentDungeon::INVALIDATE_DATA, struct IGsMessageParam* InMessageParm = nullptr);

	/**
	 * Event Delegate
	 */
public:
	// 2022/10/25 PKT - Local Player Spawn
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam*);	
	// 2022/10/25 PKT - Reset Contents
	void OnResetTimeContents(const struct IGsMessageParam* InParam);
	// 2022/06/09 PKT - Dungeon Data가 변경 되었을 시 호출
	void ChangedDataDelegate(DungeonGroupId InDungeonGroupId = INVALID_DUNGEON_GROUP_ID);

	/**
	 * Server Packet
	 */
public:
	// 2022/07/19 PKT - Dungeon Group List - 때에 따라서는 Notify 될 수도 있음. 
	void RecvDungeonGroupList(PD::SC::PKT_SC_ACK_DUNGEON_GROUP_LIST_READ& Packet);
	// 2022/10/25 PKT - Party Dungeon Entry Item List
	void RecvPartyEntryList(PD::GC::PKT_GC_ACK_COOP_PARTY_LIST_READ& Packet);
	// 2022/10/25 PKT - Party Dungeon Entered Count( 입장 시 보상 받을 수 있는 카운트 )
	void RecvPartyEnterdRewardCount(PD::SC::PKT_SC_ACK_PARTY_DUNGEON_INFO_READ& Packet);
	// 2022/10/25 PKT - 던전 시간 충전
	void RecvChargeTime(PD::SC::PKT_SC_DUNGEON_GROUP_UPDATE_READ& Packet);

public:
	// 2022/10/26 PKT - Warp 가능? ( Visible이 꺼져 있어도 Quest에서는 그 처리를 무시하고 워프해야 할 때가 있어 별도의 Flag를 준다. )
	bool IsPossibleWarpByMapId(MapId InMapId, bool InOnMessage = true);
	// 2022/10/26 PKT - 던전 입장 시도
	void TryToEnterDungeon(MapId InMapId);

	/**
	 * 2022/10/26 PKT - Window UI Info
	 */
	// 2023/2/21 PKT - Window 전용 Data
	int32 BuildViewGroup(DungeonCategory InType) const;
	// 2023/2/21 PKT - Window 전용 Data
	const TArray<const FGsDungeonGroupBase*> ViewGroupSet() const;
	// 2023/9/19 PKT - Main Tab Hidden
	bool IsHiddenWithMainTab(DungeonCategory InCategory, bool InShowTicker = false) const;


	// 2022/07/21 PKT - Get Dungeon Group Data
	const FGsDungeonGroupBase* FindGroupDataById(DungeonGroupId InDungeonGroupId) const;
	// 2022/07/25 PKT - 그룹 정보 찾기
	const FGsDungeonGroupBase* FindGroupDataByMapId(MapId InMapId) const;
	// 2022/07/25 PKT - 던전 정보 찾기
	const FGsDungeonData* FindDungeonDataByMapId(MapId InMapId) const;
	
	// 2023/2/21 PKT - 파티 던전 Config Data
	const struct FGsSchemaCoopPartyDungeonConfig* GetPartyDungeonConfig() const;

	/**
	 * 2022/10/26 PKT - Build Dungeon Group Data
	 */
private:
	void LoadConfigData();
	void BuildData();

	void OnContentsUnlock(const struct IGsMessageParam* InParam);
};

#define GSDungeon() UGsScopeHolder::GetGameManagerFType<FGsDungeonManager>(UGsScopeGame::EManagerType::Dungeon)