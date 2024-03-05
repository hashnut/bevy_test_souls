// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetMessageHandlerWorld.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsOfflinePlayManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsBookmarkManager.h"
#include "Management/ScopeGame/GsMinimapManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsMailManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGame/GsNpcShopManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameStatisticsManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "Management/ScopeGame/GsArenaManager.h"
#include "Management/ScopeGame/GsPresetManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsPersonalTradingManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGame/GsBuddyManager.h"
#include "Management/ScopeGame/GsSummonManager.h"

#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataRemotePlayer.h"
#include "GameOBject/Quest/GsQuestHandler.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"

#include "Map/Minimap/MapIcon/GsMinimapIconHandler.h"
#include "Map/Bookmark/GsBookmarkShareInfo.h"
#include "Map/Bookmark/GsBookmarkFunc.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UIContent/Popup/GsUIPopupRevive.h"
#include "UI/UIContent/Tray/GsUITrayBookmarkShare.h"
#include "UI/UIContent/Tray/GsUITrayOfflinePlayResults.h"
#include "UI/UIContent/Tray/GsUITrayTickerMileageSummon.h"

#include "T1Project.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedUnlockEnum.h"

#include "Net/GsNetSendServiceWorld.h"

#include "GameFlow/Stage/GsStageMode.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateDungeon.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Party/GsPartyHandler.h"
#include "GameFlow/Stage/StageGame/GsStageGameBase.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/Stage/StageGame/GsStageGameDungeon.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerBookmarkStatController.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Sanctum/GsSanctumHandler.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "Option/GsServerOption.h"

#include "Item/GsItemManager.h"

#include "OfflinePlay/GsOfflinePlayData.h"

#include "AI/Data/GsAIDefine.h"
#include "AI/ContentsCondition/GsAIContentsConditionAutoReturn.h"
#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"

#include "Summon/GsSummonDefine.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Sequence/GsSequenceManager.h"

#include "Warp/GsWarpFunc.h"
#include "GameFlow/GameContents/ContentsGame/Dungeon/GsDungeonStateHandler.h"
#include "Message/GsMessageInvasion.h"
#include "Message/MessageParam/GsInvasionMessageParam.h"
#include "Message/GsMessageGameObject.h"
#include "Management/ScopeGame/GsEventManager.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Buddy/GsBuddyHandler.h"



FGsNetMessageHandlerWorld::~FGsNetMessageHandlerWorld()
{

}

void FGsNetMessageHandlerWorld::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& protocolWorld = inManager->GetProtocalWorld();

	InitializeMessageDelegateList(&protocolWorld);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_CHANNEL_LIST, 
		this, &FGsNetMessageHandlerWorld::ChannelListAck));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_SELECT_CHANNEL, 
		this, &FGsNetMessageHandlerWorld::SelectChannelAck));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_LEAVE_INSTANCE_SINGLE_MAP_FAILURE, 
		this, &FGsNetMessageHandlerWorld::LeaveInstanceSingleMapFailure));
	
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_BOOKMARK,
		this, &FGsNetMessageHandlerWorld::BookmarkAck));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_BOOKMARK_CREATE,
		this, &FGsNetMessageHandlerWorld::BookmarkCreate));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_BOOKMARK_UPDATE,
		this, &FGsNetMessageHandlerWorld::BookmarkUpdate));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_BOOKMARK_DELETE,
		this, &FGsNetMessageHandlerWorld::BookmarkDelete));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_BOOKMARK_WARP,
		this, &FGsNetMessageHandlerWorld::BookmarkTeleport));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_BOOKMARK_SHARE,
		this, &FGsNetMessageHandlerWorld::BookmarkShare));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_SHARED_BOOKMARK,
		this, &FGsNetMessageHandlerWorld::NotifySharedBookmark));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_SHARED_BOOKMARK_WARP,
		this, &FGsNetMessageHandlerWorld::SharedBookmarkWarp));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_BOOKMARK_REGISTER,
		this, &FGsNetMessageHandlerWorld::SharedBookmarkRegister));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_GAME_OPTION,
		this, &FGsNetMessageHandlerWorld::GameOptionAck));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_GAME_OPTION_SAVE,
		this, &FGsNetMessageHandlerWorld::GameOptionSaveAck));
	
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_PORTAL_WARP,
		this, &FGsNetMessageHandlerWorld::PortalWarp));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_REQ_WARP_WITH_DESPAWN_READY,
		this, &FGsNetMessageHandlerWorld::WarpWithDespawnReadyReq));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_WARP_WITH_DESPAWN_FAILURE,
		this, &FGsNetMessageHandlerWorld::WarpWithDespawnFailure));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_CLIENT_OPTION_SAVE,
		this, &FGsNetMessageHandlerWorld::OptionAck));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_TRY_INTERACT,
		this, &FGsNetMessageHandlerWorld::TryInteractAck));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_NPC_EVENT_SHOP_OPEN,
		this, &FGsNetMessageHandlerWorld::TryNpcEventShopOpenAck));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_ENTER_TERRITORY_EVENT_SHOP_DATA,
		this, &FGsNetMessageHandlerWorld::EnterTerritoryEventShopData));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_LEAVE_TERRITORY_EVENT_SHOP_DATA,
		this, &FGsNetMessageHandlerWorld::LeaveTerritoryEventShopData));

	//stat distribution
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_STAT_POINT,
		this, &FGsNetMessageHandlerWorld::UpdateRemainBonusStatPoint)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_BONUS_STAT,
		this, &FGsNetMessageHandlerWorld::UpdateBonusStat)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_BONUS_STAT_INCREASE_FAILURE,
		this, &FGsNetMessageHandlerWorld::AckBonusStatIncreaseFailure)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_BONUS_STAT_EXTEND_COUNT,
		this, &FGsNetMessageHandlerWorld::AckBonusStatExtendCount)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_USED_BONUS_STAT_ITEM_COUNT,
		this, &FGsNetMessageHandlerWorld::UpdateUsedBonusStatItemCount)
	);

	//사망 패널티
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_USER_DEATH,
		this, &FGsNetMessageHandlerWorld::UserDeath)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DEATH_PENALTY_EXP_LIST,
		this, &FGsNetMessageHandlerWorld::DeathPenaltyExpList)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DEATH_PENALTY_EXP_ADD,
		this, &FGsNetMessageHandlerWorld::AddDeathPenaltyExp)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DEATH_PENALTY_ITEM_LIST,
		this, &FGsNetMessageHandlerWorld::DeathPenaltyItemList)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DEATH_PENALTY_ITEM_ADD,
		this, &FGsNetMessageHandlerWorld::AddDeathPenaltyItem)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_DEATH_PENALTY_RESTORE,
		this, &FGsNetMessageHandlerWorld::AckDeathPenaltyRestore)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DEATH_PENALTY_DELETE,
		this, &FGsNetMessageHandlerWorld::DeathPenaltyDelete)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DEATH_PENALTY_FREE_COUNT,
		this, &FGsNetMessageHandlerWorld::DeathPenaltyFreeCount)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_EXP_RECOVERY_COUNT,
		this, &FGsNetMessageHandlerWorld::AckExpRecoveryCount)
	);

	//아르카스 축복
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ARCAS,
		this, &FGsNetMessageHandlerWorld::NotifyArcasBless)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_UPDATE_ARCAS_POINT,
		this, &FGsNetMessageHandlerWorld::UpdateBlessPoint)
	);

#ifdef R3_ARCAS_NOT_USE
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_UPDATE_ARCAS_BUFF,
		this, &FGsNetMessageHandlerWorld::UpdateUseBlessFlag)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_UPDATE_ARCAS_MEDITATION_REMAINTIME,
		this, &FGsNetMessageHandlerWorld::UpdateMeditationTime)
	);
#endif

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_MAIL_UNREAD,
		this, &FGsNetMessageHandlerWorld::NotifyMailUnread)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_MAIL_LIST,
		this, &FGsNetMessageHandlerWorld::MailListAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_MAIL_RECV,
		this, &FGsNetMessageHandlerWorld::MailRecvAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_MAIL_READ,
		this, &FGsNetMessageHandlerWorld::MailReadAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_UPDATE_PVP_ALIGNMENT_POINT,
		this, &FGsNetMessageHandlerWorld::UpdatePVPAlignmentPoint)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_DUNGEON_GROUP_LIST,
		this, &FGsNetMessageHandlerWorld::DungeonGroupListAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DUNGEON_GROUP_UPDATE,
		this, &FGsNetMessageHandlerWorld::DungeonGroupChargeNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DUNGEON_ENTER,
		this, &FGsNetMessageHandlerWorld::DungeonGroupEnter)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DUNGEON_LEAVE,
		this, &FGsNetMessageHandlerWorld::DungeonGroupLeave)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DUNGEON_INFO,
		this, &FGsNetMessageHandlerWorld::DungeonGroupInfo)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_PARTY_DUNGEON_INFO,
		this, &FGsNetMessageHandlerWorld::PartyDungeonInfo)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_PARTY_DUNGEON_INFO,
		this, &FGsNetMessageHandlerWorld::DungeonNotifyPatyDungeonStatus)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_PARTY_DUNGEON_END,
		this, &FGsNetMessageHandlerWorld::DungeonNotifyPatyDungeonResult)
	);	
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ELITEDUNGEON_PORTAL_INFO,
		this, &FGsNetMessageHandlerWorld::EliteDungeonPortalInfo)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_SPIRIT_SHOT_ON_OFF,
		this, &FGsNetMessageHandlerWorld::AckSpiritShotOnOff)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_USER_SPIRIT_SHOT_ACTIVATION,
		this, &FGsNetMessageHandlerWorld::UserSpiritShotActivation)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_USER_SPIRIT_SHOT_ACTIVATION,
		this, &FGsNetMessageHandlerWorld::UserSpiritShotActivation)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_OFF_LINE_MODE_RESULT,
		this, &FGsNetMessageHandlerWorld::NotifyOfflinePlayResult)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_OFF_LINE_TIME,
		this, &FGsNetMessageHandlerWorld::NotifyOfflinePlayRemainingTimeData)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_SERVER_OFF_LINE_CANCEL,
		this, &FGsNetMessageHandlerWorld::NotifyOfflinePlayCancel)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_OFF_LINE_READY,
		this, &FGsNetMessageHandlerWorld::AckOfflinePlayReadyState)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_OFF_LINE_START,
		this, &FGsNetMessageHandlerWorld::AckOfflinePlayStartPlay)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DEBUG_OFF_LINE_MODE,
		this, &FGsNetMessageHandlerWorld::AckOfflinePlayDebugMode)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_CONTENTS_STATE,
		this, &FGsNetMessageHandlerWorld::ContentsStatePacket)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_STATISTICS_INFO,
		this, &FGsNetMessageHandlerWorld::StatisticsInfoAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_STATISTICS_DAILY_INFO,
		this, &FGsNetMessageHandlerWorld::StatisticsDailyInfoAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_CONTENTS_LOCKED,
		this, &FGsNetMessageHandlerWorld::AckContentsLocked)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_FAIL_GUILD_CAMP_WARP,
		this, &FGsNetMessageHandlerWorld::WarpGuildCampFailure)
	);

	//침공전==========================================================================
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_INVADE_INFO,
		this, &FGsNetMessageHandlerWorld::AckInvadeInfo)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_INVADE_PORTAL_INFO,
		this, &FGsNetMessageHandlerWorld::AckSpawnedInvasionPortalInfo)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INVADE_ENTER,
		this, &FGsNetMessageHandlerWorld::InvadeEnter)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INVADE_LEAVE,
		this, &FGsNetMessageHandlerWorld::InvadeLeave)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INVADE_INFO,
		this, &FGsNetMessageHandlerWorld::InvadeInfo)
	);
	
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INVADE_PLAY_COUNT,
		this, &FGsNetMessageHandlerWorld::InvadeNumOfCurrentInvader)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INVADE_PORTAL_USER_COUNT,
		this, &FGsNetMessageHandlerWorld::InvadeNumOfInvaderViaPortal)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INVADE_START_ALARM,
		this, &FGsNetMessageHandlerWorld::NotifyStartInvasionTime)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INVADE_MINIMAP_INFO_UPDATE,
		this, &FGsNetMessageHandlerWorld::InvadeMinimapInfoUpdate)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_ACTIVE_INVADE_PORTAL_INFO_LIST,
		this, &FGsNetMessageHandlerWorld::AckActiveInvasionPortalList)
	);
	
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_TERRITORY_UPDATE,
		this, &FGsNetMessageHandlerWorld::TerritoryUpdate)
	);

	//침공전 방어자 침공 시작 알림 패킷 
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INVADE_START,
		this, &FGsNetMessageHandlerWorld::InvadeStart)
	);

	//침공전 방어자 침공 끝 알림 패킷 
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INVADE_FINISH,
		this, &FGsNetMessageHandlerWorld::InvadeFinish)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_TUTORIAL,
		this, &FGsNetMessageHandlerWorld::TutorialList)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_TUTORIAL_SAVE,
		this, &FGsNetMessageHandlerWorld::TutorialSaveAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_DUEL_INVITE,
		this, &FGsNetMessageHandlerWorld::AckDuelInvite)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_DUEL_INVITE_CANCEL,
		this, &FGsNetMessageHandlerWorld::AckDuelInviteCancel)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_DUEL_ACCEPT,
		this, &FGsNetMessageHandlerWorld::AckDuelAccept)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_DUEL_INVITE_CANCEL,
		this, &FGsNetMessageHandlerWorld::NotifyDuelInviteCancel)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_DUEL_INVITED,
		this, &FGsNetMessageHandlerWorld::NotifyDuelInvited)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_DUEL_INVITE_RESULT,
		this, &FGsNetMessageHandlerWorld::NotifyDuelInviteResult)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_DUEL_COUNT_START,
		this, &FGsNetMessageHandlerWorld::NotifyDuelCountStart)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_DUEL_START,
		this, &FGsNetMessageHandlerWorld::NotifyDuelStart)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_DUEL_RESULT,
		this, &FGsNetMessageHandlerWorld::NotifyDuelResult)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_DUEL_RECONNECT_INFO,
		this, &FGsNetMessageHandlerWorld::NotifyDuelReconnectInfo)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_PARTY_TARGET,
		this, &FGsNetMessageHandlerWorld::PartyTarget));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_PARTY_TARGET_CANCEL,
		this, &FGsNetMessageHandlerWorld::PartyTargetCancel));

	//bookmark
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_USER_STAT_BOOKMARK_LIST,
		this, &FGsNetMessageHandlerWorld::AckUserStatBookmarkList));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_USER_STAT_BOOKMARK_ADD,
		this, &FGsNetMessageHandlerWorld::AckUserStatBookmarkAdd));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_USER_STAT_BOOKMARK_DELETE,
		this, &FGsNetMessageHandlerWorld::AckUserStatBookmarkDelete));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_CHARACTER_SLOT_UNLOCK,
		this, &FGsNetMessageHandlerWorld::CharacterSlotUnLock));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_USER_PRESET_CHANGE,
		this, &FGsNetMessageHandlerWorld::AckUserPresetChange));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_USER_PRESET_COPY_TO,
		this, &FGsNetMessageHandlerWorld::AckUserPresetCopyTo));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_USER_PRESET_NAME,
		this, &FGsNetMessageHandlerWorld::AckUserPresetName));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_USER_PRESET_NAME_LIST,
		this, &FGsNetMessageHandlerWorld::NotifyUserPresetName));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_CLIENT_CINEMATIC_START,
		this, &FGsNetMessageHandlerWorld::AckCinematicStart));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_CLIENT_CINEMATIC_END,
		this, &FGsNetMessageHandlerWorld::AckCinematicEnd));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_PERSONAL_TRADING_TRY,
		this, &FGsNetMessageHandlerWorld::AckPersonalTradingTry));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_PERSONAL_TRADING_ACCEPT,
		this, &FGsNetMessageHandlerWorld::AckPersonalTradingAccept));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_PERSONAL_TRADING_CANCEL,
		this, &FGsNetMessageHandlerWorld::AckPersonalTradingCancel));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_PERSONAL_TRADING_ABORT,
		this, &FGsNetMessageHandlerWorld::AckPersonalTradingAbort));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_PERSONAL_TRADING_LOCK,
		this, &FGsNetMessageHandlerWorld::AckPersonalTradingLock));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_PERSONAL_TRADING_CONFIRM,
		this, &FGsNetMessageHandlerWorld::AckPersonalTradingConfirm));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INFORM_PERSONAL_TRADING,
		this, &FGsNetMessageHandlerWorld::NotifyPersonalTrading));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_PERSONAL_TRADING_PUT_ITEM,
		this, &FGsNetMessageHandlerWorld::AckPersonalTradingPutItem));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_PERSONAL_TRADING_PUT_DIA,
		this, &FGsNetMessageHandlerWorld::AckPersonalTradingPutDia));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INFORM_PERSONAL_TRADING_ITEM_UPDATE,
		this, &FGsNetMessageHandlerWorld::NotifyPersonalTradingItemUpdate));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INFORM_PERSONAL_TRADING_ITEM_EQUIP_UPDATE,
		this, &FGsNetMessageHandlerWorld::NotifyPersonalTradingItemEquipUpdate));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INFORM_PERSONAL_TRADING_ITEM_REMOVE,
		this, &FGsNetMessageHandlerWorld::NotifyPersonalTradingItemRemove));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_INFORM_PERSONAL_TRADING_DIA_UPDATE,
		this, &FGsNetMessageHandlerWorld::NotifyPersonalTradingDiaUpdate));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_FORCE_DAILY_TRADING_CNT_UPDATE,
		this, &FGsNetMessageHandlerWorld::CheatPersonalTradingDailyTradingCntUpdate));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_LATENCY,
		this, &FGsNetMessageHandlerWorld::AckLatency));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_USER_CONTEXT_UI_OTHER_SERVER,
		this, &FGsNetMessageHandlerWorld::AckUserContextOtherServer));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_FAIRY_SUMMON_MILEAGE,
		this, &FGsNetMessageHandlerWorld::AckFairySummonMileage));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_COSTUME_SUMMON_MILEAGE,
		this, &FGsNetMessageHandlerWorld::AckCostumeSummonMileage));	
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_UNIQUE_FAIRY_SUMMON_MILEAGE,
		this, &FGsNetMessageHandlerWorld::AckUniqueFairySummonMileage));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_UNIQUE_COSTUME_SUMMON_MILEAGE,
		this, &FGsNetMessageHandlerWorld::AckUniqueCostumeSummonMileage));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_UNIQUE_MILEAGE_OBTAINED_INFO,
		this, &FGsNetMessageHandlerWorld::UniqueMileageObtainedInfo));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_BUDDY_LOGIN,
		this, &FGsNetMessageHandlerWorld::BuddyLoginNotify));
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_BUDDY_LOGOUT,
		this, &FGsNetMessageHandlerWorld::BuddyLogoutNotify));
}

void FGsNetMessageHandlerWorld::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerWorld::ChannelListAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_CHANNEL_LIST_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_CHANNEL_LIST_READ*>(inPacket.Buffer))
	{
		GGameData()->Set(*pkt);
	}

	// 채널 리스트를 받았음을 알림 (채널변경 팝업 열기에 이용)
	GMessage()->GetUI().SendMessage(MessageUI::RECEIVE_CHANNEL_LIST, nullptr);
}

void FGsNetMessageHandlerWorld::SelectChannelAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ACK_SELECT_CHANNEL_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_SELECT_CHANNEL_READ*>(inPacket.Buffer))
	{
		// 실패 시 에러 팝업, 리스트 갱신 요청 후에 채널 팝업창을 갱신한다.
		if (PACKET_RESULT_SUCCESS != pkt->Result())
		{
			// 에러 상황은 아닌 메시지의 경우 티커로 출력
			PD::Result result = static_cast<PD::Result>(pkt->Result());
			if (PD::Result::SELECT_CHANNEL_ERROR_INVALID_STATUS == result ||
				PD::Result::SELECT_CHANNEL_ERROR_SAME_AREA_CHANNEL == result ||
				PD::Result::SELECT_CHANNEL_ERROR_IN_COMBAT == result ||
				PD::Result::SELECT_CHANNEL_ERROR_IN_SHUTTLE == result ||
				PD::Result::SELECT_CHANNEL_ERROR_CONGESTION_FULL == result)
			{
				FGsUIHelper::TraySectionMessageTickerNetError(result);
				return;
			}
			
			FGsUIHelper::PopupNetError(result);
			GGameData()->SendRequestChannelList();
			return;
		}

		// 채널이동 창을 닫음
		TWeakObjectPtr<UGsUIWidgetBase> popup = GUI()->GetWidgetByKey(TEXT("PopupChannelList"));
		if (popup.IsValid())
		{
			popup->Close();
		}

		// 채널이동 시 자동전투, 자동진행 끄기
		StopAuto();	
	}
}

void FGsNetMessageHandlerWorld::LeaveInstanceSingleMapFailure(const FGsNet::Packet & inPacket)
{
	// 실패시에만 오는 패킷 임에 주의할 것
	if (PD::SC::PKT_SC_ACK_LEAVE_INSTANCE_SINGLE_MAP_FAILURE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_LEAVE_INSTANCE_SINGLE_MAP_FAILURE_READ*>(inPacket.Buffer))
	{
		// 실패 시 에러 팝업
		if (PACKET_RESULT_SUCCESS != pkt->Result())
		{
			// 에러 상황은 아닌 메시지의 경우 티커로 출력
			PD::Result result = static_cast<PD::Result>(pkt->Result());
			if (PD::Result::WARP_ERROR_LEAVE_INSTANCE_SINGLE_DELAY_TIME == result)
			{
				FGsUIHelper::TraySectionMessageTickerNetError(result);
				return;
			}

			FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
			return;
		}
	}
}

void FGsNetMessageHandlerWorld::BookmarkAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_BOOKMARK_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_BOOKMARK_READ*>(inPacket.Buffer);

	if (nullptr == pkt)
	{
		return;
	}
	GGameData()->Set(*pkt);

	// 북마크 매니저가 살아있으면 세팅
	// 아니면 매니저 초기화 되는시점에 세팅
	GMessage()->GetUI().SendMessage(MessageUI::INIT_BOOK_MARK_ALL_INFO, nullptr);
}

void FGsNetMessageHandlerWorld::BookmarkCreate(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("BookmarkCreate")));

	PD::SC::PKT_SC_ACK_BOOKMARK_CREATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_BOOKMARK_CREATE_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}
		
	GSBookmark()->RecvBookmarkCreate(pkt->Id(), pkt->Title(),
		pkt->ConstRefPos(), pkt->MapId());

	// 미니맵 아이콘 추가
	GSMinimap()->GetIconHandler()->MakeBookmarkIcon(pkt->Id(), FVector2D(pkt->ConstRefPos()),
		pkt->MapId());
}

void FGsNetMessageHandlerWorld::BookmarkUpdate(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("BookmarkUpdate")));

	PD::SC::PKT_SC_ACK_BOOKMARK_UPDATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_BOOKMARK_UPDATE_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	GSBookmark()->RecvBookmarkUpdate(pkt->Id());
}

void FGsNetMessageHandlerWorld::BookmarkDelete(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("BookmarkDelete")));

	PD::SC::PKT_SC_ACK_BOOKMARK_DELETE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_BOOKMARK_DELETE_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	GSBookmark()->RecvBookmarkDelete(pkt->Id());

	// 미니맵 아이콘 삭제
	GSMinimap()->GetIconHandler()->DelBookmarkIcon(pkt->Id());
}

void FGsNetMessageHandlerWorld::BookmarkTeleport(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("BookmarkTeleport")));

	PD::SC::PKT_SC_ACK_BOOKMARK_WARP_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_BOOKMARK_WARP_READ*>(inPacket.Buffer);

	if (PACKET_RESULT_SUCCESS == pkt->Result())
	{
		return;
	}

	FText findText;
	switch (pkt->Result())
	{
	case PD::Result::BOOKMARK_ERROR_INVALID_STATUS:
		// 텔레포트 가능한 상태가 아님
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorTeleportInvalidState"), findText);
		break;
	}

	if (findText.IsEmpty())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}

void FGsNetMessageHandlerWorld::BookmarkShare(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("BookmarkShare")));

	PD::SC::PKT_SC_ACK_BOOKMARK_SHARE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_BOOKMARK_SHARE_READ*>(inPacket.Buffer);

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (false == FGsBookmarkFunc::ShowTickerBookmarkShareError(result))
		{
			FGsUIHelper::PopupNetError(result);
		}
		return;
	}

	// 공유 쿨타임 시작(60초 막기)
	GSBookmark()->StartShareCoolTime();
	// 북마크 공유 성공 티커 출력
	FGsBookmarkFunc::ShowTickerBookmarkShareSuccess();
}

void FGsNetMessageHandlerWorld::NotifySharedBookmark(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_SHARED_BOOKMARK_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_NOTIFY_SHARED_BOOKMARK_READ*>(inPacket.Buffer);

	// 튜토리얼 중 위치 공유 무시처리
	if (GSTutorial()->IsActiveTutorial())
	{
		return;
	}

	FString userName = pkt->UserName();
	int32 mapId = pkt->MapId();
	FVector pos = pkt->Pos();
	int32 channelId = pkt->ChannelId();
	GLocalization()->ChangeBannedSentence(pkt->Title(), [userName, mapId, pos, channelId](bool success, FString inText, FString outText)
		{
			FGsBookmarkShareInfo& shareInfo = GSBookmark()->GetShareInfo();
			shareInfo.SetShareInfo(userName, outText,
				mapId, pos, channelId);

			shareInfo.StartTimer();

			if (UGsUIManager* uiManager = GUI())
			{
				if (false == uiManager->IsActiveWidget(TEXT("TrayBookmarkShare")))
				{
					uiManager->OpenAndGetWidget(TEXT("TrayBookmarkShare"));
				}
				else
				{
					TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("TrayBookmarkShare"));
					if (widget.IsValid())
					{
						UGsUITrayBookmarkShare* trayBookMarkShare = Cast<UGsUITrayBookmarkShare>(widget.Get());
						if (nullptr == trayBookMarkShare)
						{
							return;
						}
						trayBookMarkShare->SetData();
					}
				}
			}
		});
}

void FGsNetMessageHandlerWorld::SharedBookmarkWarp(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("SharedBookmarkWarp")));

	PD::SC::PKT_SC_ACK_SHARED_BOOKMARK_WARP_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_SHARED_BOOKMARK_WARP_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (FGsBookmarkFunc::ShowTickerBookmarkShareWarpError(result) == false)
		{
			FGsUIHelper::PopupNetError(result);
		}
		return;
	}
}
void FGsNetMessageHandlerWorld::SharedBookmarkRegister(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("SharedBookmarkRegister")));

	PD::SC::PKT_SC_ACK_BOOKMARK_REGISTER_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_BOOKMARK_REGISTER_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	GSBookmark()->RecvBookmarkCreate(pkt->Id(), pkt->Title(),
		pkt->ConstRefPos(), pkt->MapId());

	// 미니맵 아이콘 추가
	GSMinimap()->GetIconHandler()->MakeBookmarkIcon(pkt->Id(), FVector2D(pkt->ConstRefPos()),
		pkt->MapId());
}

void FGsNetMessageHandlerWorld::GameOptionAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_GAME_OPTION_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_GAME_OPTION_READ*>(inPacket.Buffer);
	ApplyGameOption(pkt->ConstRefGameOption());

	// 정령탄 UI ON,OFF 저장
	GGameData()->Set(*pkt);
}

void FGsNetMessageHandlerWorld::GameOptionSaveAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_GAME_OPTION_SAVE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_GAME_OPTION_SAVE_READ*>(inPacket.Buffer);
	ApplyGameOption(pkt->ConstRefGameOption());
}

void FGsNetMessageHandlerWorld::ApplyGameOption(const GameOption& GameOptionData)
{
	FGsServerOption* serverOption = GOption()->GetServerOption();
	serverOption->ConvertToClientData(&GameOptionData);
	serverOption->SetUsePotionHpRate(GameOptionData.mAutoUsePotionInfo.mUsePotionHpRate);
	serverOption->SetisAutoUsePotion(GameOptionData.mAutoUsePotionInfo.mIsAutoUsePotion);

	serverOption->SetIsVisibleHelmet(GameOptionData.mViewHelmet);
	serverOption->SetOfflineOption(GameOptionData.mOffLineOptionInfo);
	serverOption->SetAutoAttackRange(GameOptionData.mCombatOptionInfo.mAutoAttackRange);
	serverOption->SetDropItemGrade(GameOptionData.mCombatOptionInfo.mAutoDropItemGrade);

	serverOption->SetSocialMediaInfoOption(GameOptionData.mSocialMediaInfo);
	serverOption->SetWorldMapFilter(GameOptionData.mWorldMapFilterInfo);
	serverOption->SetIsCounterAttackOn(GameOptionData.mCombatOptionInfo.mManualCombat.mCounterAttack);
	// serverOption->SetIsAutoSkillOn(GameOptionData.mCombatOptionInfo.mManualCombat.mAutoSkill);
	serverOption->SetAutoDropItemWeight(GameOptionData.mCombatOptionInfo.mAutoDropItemWeight);
	serverOption->SetScanOption(GameOptionData.mRadarOptionInfo);
	serverOption->SetPKBookOption(GameOptionData.mMonitoringTargetNotification);

	serverOption->SetPickupItemOption(GameOptionData.mOptionSelectTypeInfo);
	serverOption->SetAutoDecomposeOption(GameOptionData.mAutoDecompose);
	serverOption->SetPcRvo(GameOptionData.mPcRvo);

	// [B1] | ejrrb10 | 배열, 맵 형태의 녀석들은 iteration 으로 작업
	// TArray, TMap 의 경우 MoveTemp 를 사용할 수는 있으나, 매개변수가 const 로 잡혀 있어서 MoveTemp 대신 iteration 을 사용
	for (int i = 0; i < MAX_COMBAT_OPTION_COUNT; ++i)
	{
		serverOption->SetAutoCombatTargetOption(i, GameOptionData.mCombatOptionInfo.mAutoCombatTargetOptionList[i]);
	}

	for (int i = 0; i < MAX_COMBAT_OPTION_COUNT; ++i)
	{
		serverOption->SetManualCombatTargetOption(i, GameOptionData.mCombatOptionInfo.mManualCombatTargetOptionList[i]);
	}

	serverOption->SetHivePushNotification(GameOptionData.mHivePushNotification);
	serverOption->SetClientNotification(GameOptionData.mClientNotification);
	serverOption->SetLoginAlram(GameOptionData.mLoginAlram);
	serverOption->SetHiddenNameGetItem(GameOptionData.mHiddenNameGetItem);

	// 2021/10/26 PKT - AI에 서버 옵션 적용
	FGsAIManager* aiManager = GSAI();
	if (aiManager)
	{
		// 2021/10/26 PKT - 자동 사냥 범위 설정
		int32 attackRange = StaticCast<int32>(GameOptionData.mCombatOptionInfo.mAutoAttackRange);
		const FGsPrimitiveInt32 paramAttackRange(attackRange);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_RETURN_START_POS_TYPE, &paramAttackRange);

		// 2021/10/26 PKT - 줍기 옵션 변경(아이템 등급)
		int32 dropItemGrade = StaticCast<int32>(GameOptionData.mCombatOptionInfo.mAutoDropItemGrade);
		const FGsPrimitiveInt32 paramDropItemGrade(dropItemGrade);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_AUTO_LOOT_DROP_ITEM_GRADE, &paramDropItemGrade);
	}

	if (UGsGameObjectManager* objectManager = GSGameObject())
	{
		if (UGsGameObjectBase* gameObject = objectManager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* localPlayer = gameObject->CastGameObject<UGsGameObjectLocalPlayer>();
			localPlayer->CombatOptionUpdate();
			localPlayer->UpdateRVOAvoidance();
		}

		objectManager->LoadScanOption();
	}

	GMessage()->GetUI().SendMessage(MessageUI::OPTION_SAVE_TO_SERVER_COMPLETED, nullptr);

	if (UGsOptionManager* optionMgr = GOption())
	{
		optionMgr->SetServerOptionData();
	}
}

void FGsNetMessageHandlerWorld::StopAuto()
{
	// 퀘스트 자동진행 끄기
	if (UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>())
		{
			local->StopAuto();
		}
	}
}

// 포털 워프 응답
void FGsNetMessageHandlerWorld::PortalWarp(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_PORTAL_WARP_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_PORTAL_WARP_READ*>(inPacket.Buffer);

	// UI 블록 제거 
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("PortalWarp")));
	
	if (PACKET_RESULT_SUCCESS == pkt->Result())
	{
		// 워프 연출 블록걸기
		FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("WarpAnimEffect")));
		FGsUIHelper::SetBlockUISetting(FText::GetEmpty(), true);
		return;
	}	
	
	FText findText;
	switch (pkt->Result())
	{
	case PD::Result::DUNGEON_ERROR_NOT_ENOUGH_TIME:
		// 던전 - 충전 시간이 부족합니다
		FText::FindText(TEXT("UICommonText"), TEXT("Cannot_Teleport_TimeLimit"), findText);
		break;
	case PD::Result::WARP_ERROR_WORLD_BOSS_OPEN:		
		FText::FindText(WORLD_BOSS_TEXT_KEY, TEXT("Notice_NotEnterTime"), findText);
		break;
	case PD::Result::WARP_ERROR_WORLD_BOSS_ENOUGH_COST:		
		FText::FindText(WORLD_BOSS_TEXT_KEY, TEXT("Notice_NotEnoughCost"), findText);
		break;	
	case PD::Result::WARP_ERROR_WORLD_BOSS_ALREADY_ENTER:
		FText::FindText(TEXT("WorldBossText"), TEXT("Notice_AlreadyEnter"), findText);
		break;	
	case PD::Result::WARP_ERROR_WORLD_BOSS_INVALID_STATUS:
		FText::FindText(TEXT("WorldBossText"), TEXT("Notice_CanNotWarp"), findText);
		break;
	case PD::Result::PORTAL_WARP_ERROR_INVALID_STATUS:
		FText::FindText(TEXT("WorldMapText"), TEXT("ErrorCannotmove"), findText);
		break;
	case PD::Result::WARP_ERROR_UNLOCK:
		FText::FindText(TEXT("UICommonText"), TEXT("WarpFail_Alert"), findText);
		break;
	}

	if (findText.IsEmpty())
	{	// 2022/06/10 PKT - 이곳에 없는 메세지라면 서버 메세지에서 찾는다.
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}
	
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);		
}

// '디스폰을 수반한 워프' 준비 요청.
void FGsNetMessageHandlerWorld::WarpWithDespawnReadyReq(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("WarpWithDespawnReady")));

	if (FGsNetManager* netMgr = GNet())
	{
		netMgr->ClearPacketSendBlock();

		// 파티 요청 받은 상태에서 그냥 워프가면 거절할 틈이 없음(계속 파티 수락 대기중)
		// 파티 요청 받은게 있다면 거절 강제 처리
		FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
		if (nullptr == gameFlow)
		{
			GSLOG(Error, TEXT("nullptr == gameFlow"));
			return;
		}

		FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
		if (nullptr == contents)
		{
			GSLOG(Error, TEXT("nullptr == contents"));
			return;
		}

		FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
		if (nullptr == hud)
		{
			GSLOG(Error, TEXT("nullptr == hud"));
			return;
		}

		FGsPartyHandler* partyHandler = hud->GetPartyHandler();
		if (nullptr== partyHandler)
		{
			GSLOG(Error, TEXT("nullptr == partyHandler"));
			return;
		}

		partyHandler->CancelInvite();

		// 개인 거래 취소 처리
		GSPersonalTrading()->ClearPersonalTrading();

		// 대결 요청 취소 처리
		GSArena()->ClearArenaRequest();

		//Interaction 하던 도중 이였다면 인터렉션 취소
		GSInteraction()->SendInteractionCancel();

		// 절전 모드 닫기
		GMessage()->GetSaveBattery().SendMessage(MessageContentSaveBattery::CLOSE_UI, nullptr);
		
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetSendServiceWorld::SendAckWarpWithDespawnReady();
			netBase->SetPacketSendBlock(true);
		}

		// bgm 중지
		FGsPrimitiveUInt8 param(static_cast<uint8>(true));
		GMessage()->GetSound().SendMessage(MessageSound::PAUSE_BGM, &param);

		GMessage()->GetSystem().SendMessage(MessageSystem::WARP_WITH_DESPAWN_READY);
	}
}

// '디스폰을 수반한 워프' 실패.
void FGsNetMessageHandlerWorld::WarpWithDespawnFailure(const FGsNet::Packet& inPacket)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			netBase->SetPacketSendBlock(false);
		}
	}
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("WarpWithDespawnReady")));

	// 치명적인 오류이기 때문에 로비로 돌려보내는것이 맞을 것 같습니다
	FGsUIHelper::PopupNetError(static_cast<PD::Result>(PD::Result::WARP_ERROR_INVALID_STATUS));

	// 로그인화면으로 변경
	GMessage()->GetSystem().SendMessage(MessageSystem::BACKTO_LOGIN);
}

void FGsNetMessageHandlerWorld::OptionAck(const FGsNet::Packet & inPacket)
{
	PD::SC::PKT_SC_ACK_CLIENT_OPTION_SAVE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_CLIENT_OPTION_SAVE_READ*>(inPacket.Buffer);
	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}
}

void FGsNetMessageHandlerWorld::TryInteractAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_TRY_INTERACT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_TRY_INTERACT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	auto result = static_cast<PD::Result>(pkt->Result());
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("TRY_INTERACT Failed, Result = %d"), result);

		//result == PD::Result::INVADE_ERROR_ENTER_MAX_COUNT  -> PopUp
		if (result == PD::Result::INVADE_ERROR_INVALID_LEVEL ||
			result == PD::Result::INVADE_ERROR_NOT_ENOUGH_TIME)
		{
			FGsUIHelper::TraySectionMessageTickerNetError(result);
		}
		else 
		{
			// 성소점령전 중 발생할 수 있는 애러 코드 체크
			if (FGsSanctumHandler::ShowSanctumMessageTicker(pkt->Result()))
			{
				return;
			}

			FGsUIHelper::PopupNetError(result);
		}
		
		// 에러 패킷일때는 진행하면 안됨
		return;
	}

	FGsClientSpawnManager* clientSpawn = GSClientSpawn();
	SpawnId spawnId = pkt->InteractNpcSpawnId();
	int64 gameId = pkt->InteractNpcGameId();
	int64 findGameId = 0;
	if (clientSpawn->FindNpcGameIdBySpawnId(spawnId, gameId, findGameId) == true)
	{
		UGsGameObjectBase* object = GSGameObject()->FindObject(EGsGameObjectType::NonPlayer, findGameId);
		if (nullptr == object)
		{
			// NonPlayer 로 Find 하면 Camp 를 못찾음
			object = GSGameObject()->FindObject(EGsGameObjectType::Camp, findGameId);
		}

		if (nullptr == object)
		{
			// NonPlayer로 Find 하면 Sanctum 못찾음
			object = GSGameObject()->FindObject(EGsGameObjectType::Sanctum, findGameId);
		}

		GSInteraction()->RecvTryInteractAck(*pkt, object, findGameId);
	}
}

void FGsNetMessageHandlerWorld::TryNpcEventShopOpenAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_NPC_EVENT_SHOP_OPEN_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_NPC_EVENT_SHOP_OPEN_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("NPC_EVENT_SHOP_OPEN Failed, Result = %d"), result);
		return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
	}

	GSInteraction()->RecvNpcEventShopOpenAck(*pkt);
}
void FGsNetMessageHandlerWorld::EnterTerritoryEventShopData(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_ENTER_TERRITORY_EVENT_SHOP_DATA_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_NOTIFY_ENTER_TERRITORY_EVENT_SHOP_DATA_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GSInteraction()->RecvEnterTerritoryEventShopData(*pkt);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_NPCFUNCTION);
}
void FGsNetMessageHandlerWorld::LeaveTerritoryEventShopData(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_LEAVE_TERRITORY_EVENT_SHOP_DATA_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_NOTIFY_LEAVE_TERRITORY_EVENT_SHOP_DATA_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GSInteraction()->RecvLeaveTerritoryEventShopData(*pkt);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_NPCFUNCTION);
}
void FGsNetMessageHandlerWorld::AckBonusStatIncreaseFailure(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_BONUS_STAT_INCREASE_FAILURE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_BONUS_STAT_INCREASE_FAILURE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	int result = pkt->Result();
	FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
}

void FGsNetMessageHandlerWorld::UpdateBonusStat(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_BONUS_STAT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_BONUS_STAT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->Set(*pkt);
}

void FGsNetMessageHandlerWorld::AckBonusStatExtendCount(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_BONUS_STAT_EXTEND_COUNT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_BONUS_STAT_EXTEND_COUNT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}


	GGameData()->SetBonusStatExtendCount(pkt->ExtendedNumber());
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_BONUS_STAT_ITEM_COUNT, nullptr);
}

void FGsNetMessageHandlerWorld::UpdateUsedBonusStatItemCount(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_USED_BONUS_STAT_ITEM_COUNT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_USED_BONUS_STAT_ITEM_COUNT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->SetUsedBonusStatItemCount(pkt->UsedBonusStatItemCount());
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_BONUS_STAT_ITEM_COUNT, nullptr);
}

void FGsNetMessageHandlerWorld::UserDeath(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_USER_DEATH_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_USER_DEATH_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}	
	
	//플레이어 사망 정보 받고 
	//GsStateLocalPlayerDead(십자가 떨어지고) 진입 후 3초 뒤 팝업 창 띄워진다.
	GGameData()->Set(*pkt);	
}

void FGsNetMessageHandlerWorld::AckExpRecoveryCount(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_EXP_RECOVERY_COUNT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_EXP_RECOVERY_COUNT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->Set(*pkt);	
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PENALTY, nullptr);
}

void FGsNetMessageHandlerWorld::DeathPenaltyExpList(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_DEATH_PENALTY_EXP_LIST_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DEATH_PENALTY_EXP_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->Set(*pkt);	
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PENALTY, nullptr);
}

void FGsNetMessageHandlerWorld::AddDeathPenaltyExp(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_DEATH_PENALTY_EXP_ADD_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DEATH_PENALTY_EXP_ADD_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->Set(*pkt);
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PENALTY, nullptr);
}

void FGsNetMessageHandlerWorld::DeathPenaltyItemList(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_DEATH_PENALTY_ITEM_LIST_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DEATH_PENALTY_ITEM_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->Set(*pkt);
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PENALTY, nullptr);
}

void FGsNetMessageHandlerWorld::AddDeathPenaltyItem(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_DEATH_PENALTY_ITEM_ADD_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DEATH_PENALTY_ITEM_ADD_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->Set(*pkt);
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PENALTY, nullptr);
}

void FGsNetMessageHandlerWorld::AckDeathPenaltyRestore(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_DEATH_PENALTY_RESTORE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_DEATH_PENALTY_RESTORE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		switch (pkt->Result())
		{
		case PD::Result::DEATH_ERROR_INVALID_ID:
		{
			FText findText;
			FText::FindText(TEXT("ResurrectionText"), TEXT("Recovery_Notice_Expired"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}
			break;
		default:
		{
			FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		}
			break;
		}		

		return;
	}	
	
	//restore game data
	TArray<DeathPenaltyRestore> penaltyList;
	pkt->MakeDeathPenaltyRestoreVector<TArray<DeathPenaltyRestore>, DeathPenaltyRestore>(penaltyList);
	GGameData()->RemoveDeathPenaltyList(penaltyList);
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PENALTY, nullptr);
}

void FGsNetMessageHandlerWorld::DeathPenaltyFreeCount(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_DEATH_PENALTY_FREE_COUNT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DEATH_PENALTY_FREE_COUNT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->Set(*pkt);
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PENALTY, nullptr);
}

void FGsNetMessageHandlerWorld::DeathPenaltyDelete(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_DEATH_PENALTY_DELETE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DEATH_PENALTY_DELETE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	
	GGameData()->RemoveDeathPenaly(pkt->DeathPenaltyId());
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PENALTY, nullptr);
}

//레벨업을 하거나 보너스 스탯이 분배가 끝났을 때 오는 패킷
void FGsNetMessageHandlerWorld::UpdateRemainBonusStatPoint(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_STAT_POINT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_STAT_POINT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->Set(*pkt);

	
	FGsPlayerStatDistributionController* playerStatHandler = FGsPlayerStatHandler::GetInstance()->GetStatDistributionController();
	//남아있는 보너스 포인트 갱신
	playerStatHandler->UpdateStatPoint();

	//UI를 갱신하고 레드닷을 갱신해야한다.
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_STAT);
	bool isActive = pkt->StatPoint() > 0;
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::STAT, isActive);
}

void FGsNetMessageHandlerWorld::NotifyArcasBless(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ARCAS_READ*>(inPacket.Buffer))
	{
		GGameData()->Set(*pkt);

		GMessage()->GetUI().SendMessage(MessageUI::BLESS_ARCAS_POINT, nullptr);
	}
}

void FGsNetMessageHandlerWorld::UpdateBlessPoint(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_UPDATE_ARCAS_POINT_READ*>(inPacket.Buffer))
	{
		if (PACKET_RESULT_SUCCESS == pkt->Result())
		{
			GGameData()->Set(*pkt);
		}
		else
		{
			FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		}
		GMessage()->GetUI().SendMessage(MessageUI::BLESS_ARCAS_POINT, nullptr);
	}
}

#ifdef R3_ARCAS_NOT_USE
void FGsNetMessageHandlerWorld::UpdateMeditationTime(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_UPDATE_ARCAS_MEDITATION_REMAINTIME_READ*>(inPacket.Buffer))
	{
		if (PACKET_RESULT_SUCCESS == pkt->Result())
		{
			GGameData()->Set(*pkt);
		}
		else
		{
			FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		}
		GMessage()->GetUI().SendMessage(MessageUI::BLESS_ARCAS_POINT, nullptr);
	}
}

void FGsNetMessageHandlerWorld::UpdateUseBlessFlag(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_UPDATE_ARCAS_BUFF_READ*>(inPacket.Buffer))
	{
		if (PACKET_RESULT_SUCCESS == pkt->Result())
		{
			GGameData()->Set(*pkt);
		}
		else
		{
			FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		}
		GMessage()->GetUI().SendMessage(MessageUI::BLESS_ARCAS_POINT, nullptr);
	}
}
#endif

// 우편
void FGsNetMessageHandlerWorld::NotifyMailUnread(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[mail] NOTIFY_MAIL_UNREAD"));
#endif

	
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_NOTIFY_MAIL_UNREAD_READ*>(inPacket.Buffer))
	{
		GGameData()->Set(*pkt);
		//GMessage()->GetMail().SendMessage(MessageContentMail::NOTIFY_MAIL_UNREAD, nullptr);
	}
}

void FGsNetMessageHandlerWorld::MailListAck(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[mail] ACK_MAIL_LIST"));
#endif

	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_MAIL_LIST_READ*>(inPacket.Buffer))
	{
		int32_t result = pkt->Result();
		if (result != PACKET_RESULT_SUCCESS)
		{
			GSLOG(Log, TEXT("MAIL_LIST Failed, Result = %d"), result);
			return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		}
		
		GSMail()->Set(*pkt);
	}
}

void FGsNetMessageHandlerWorld::MailRecvAck(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[mail] ACK_MAIL_RECV"));
#endif

	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_MAIL_RECV_READ*>(inPacket.Buffer))
	{
		GSMail()->Set(*pkt);
	}
}

void FGsNetMessageHandlerWorld::MailReadAck(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[mail] ACK_MAIL_READ"));
#endif

	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_MAIL_READ_READ*>(inPacket.Buffer))
	{
		int32_t result = pkt->Result();
		if (result != PACKET_RESULT_SUCCESS)
		{
			GSLOG(Log, TEXT("MAIL_READ Failed, Result = %d"), result);
			return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		}

		GSMail()->Set(*pkt);
	}
}

void FGsNetMessageHandlerWorld::UpdatePVPAlignmentPoint(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_UPDATE_PVP_ALIGNMENT_POINT_READ*>(inPacket.Buffer))
	{
		GGameData()->Set(*pkt);
		
		//notify 
		GMessage()->GetUserBasicInfo().SendMessage(MessageUserInfo::PVP_POINT, pkt->PvpAlignmentPoint());
	}
}

void FGsNetMessageHandlerWorld::DungeonGroupListAck(const FGsNet::Packet& inPacket)
{
	// 2022/07/19 PKT - block ui
	FGsUIHelper::HideBlockUI();

#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] ACK_DUNGEON_GROUP_LIST_READ"));
#endif
	
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_DUNGEON_GROUP_LIST_READ*>(inPacket.Buffer))
	{
		GSDungeon()->RecvDungeonGroupList(*pkt);
	}

	GMessage()->GetDungeon().SendMessage(MessageContentDungeon::UPDATE_TIME, nullptr);	
}

void FGsNetMessageHandlerWorld::DungeonGroupChargeNotify(const FGsNet::Packet& inPacket)
{
	// BlockUI 닫기 
	FGsUIHelper::HideBlockUI();
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] ACK_DUNGEON_GROUP_CHARGE_READ"));
#endif

	PD::SC::PKT_SC_DUNGEON_GROUP_UPDATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DUNGEON_GROUP_UPDATE_READ*>(inPacket.Buffer);
	if (pkt)
	{
		GSDungeon()->RecvChargeTime(*pkt);

		FGsPrimitiveInt32 param(pkt->DungeonGroupId());
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::UPDATE_TIME, &param);
	}
}

void FGsNetMessageHandlerWorld::DungeonGroupEnter(const FGsNet::Packet& inPacket)
{
	// BlockUI 닫기 
	FGsUIHelper::HideBlockUI();
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] DUNGEON_ENTER_READ"));
#endif

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		GSLOG(Error, TEXT("nullptr == gameFlow"));
		return;
	}

	FGsStageManagerGame* stageMgr = gameFlow->GetStageManagerGame();
	if (nullptr == stageMgr)
	{
		GSLOG(Error, TEXT("nullptr == stageMgr"));
		return;
	}

	TSharedPtr<FGsStageGameBase> findStage = stageMgr->Find(FGsStageMode::Game::DUNGEON);
	if (findStage.IsValid())
	{
		FGsStageGameDungeon* stateGameDungeon = StaticCast<FGsStageGameDungeon*>(findStage.Get());
		if (stateGameDungeon)
		{
			if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_DUNGEON_ENTER_READ*>(inPacket.Buffer))
			{
				stateGameDungeon->RecvDungeonEnter(*pkt);
			}			
		}		
	}
}

void FGsNetMessageHandlerWorld::DungeonGroupLeave(const FGsNet::Packet& inPacket)
{
	// BlockUI 닫기 
	FGsUIHelper::HideBlockUI();
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] DUNGEON_ENTER_READ"));
#endif

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		GSLOG(Error, TEXT("nullptr == gameFlow"));
		return;
	}

	FGsStageManagerGame* stageMgr = gameFlow->GetStageManagerGame();
	if (nullptr == stageMgr)
	{
		GSLOG(Error, TEXT("nullptr == stageMgr"));
		return;
	}

	TSharedPtr<FGsStageGameBase> findStage = stageMgr->Find(FGsStageMode::Game::DUNGEON);
	if (findStage.IsValid())
	{
		FGsStageGameDungeon* stateGameDungeon = StaticCast<FGsStageGameDungeon*>(findStage.Get());
		if (stateGameDungeon)
		{
			if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_DUNGEON_LEAVE_READ*>(inPacket.Buffer))
			{
				stateGameDungeon->RecvDungeonExit(*pkt);
			}
		}
	}
}

void FGsNetMessageHandlerWorld::DungeonGroupInfo(const FGsNet::Packet& inPacket)
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		GSLOG(Error, TEXT("nullptr == gameFlow"));
		return;
	}

	FGsStageManagerGame* stageMgr = gameFlow->GetStageManagerGame();
	if (nullptr == stageMgr)
	{
		GSLOG(Error, TEXT("nullptr == stageMgr"));
		return;
	}

	TSharedPtr<FGsStageGameBase> findStage = stageMgr->Find(FGsStageMode::Game::DUNGEON);
	if (findStage.IsValid())
	{
		FGsStageGameDungeon* stateGameDungeon = StaticCast<FGsStageGameDungeon*>(findStage.Get());
		if (stateGameDungeon)
		{
			if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_DUNGEON_INFO_READ*>(inPacket.Buffer))
			{
				stateGameDungeon->RecvDungeonInfo(*pkt);
			}
		}
	}
}

void FGsNetMessageHandlerWorld::PartyDungeonInfo(const FGsNet::Packet& inPacket)
{
	// BlockUI 닫기 
	FGsUIHelper::HideBlockUI();

#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] ACK_PARTY_DUNGEON_INFO"));
#endif

	PD::SC::PKT_SC_ACK_PARTY_DUNGEON_INFO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_PARTY_DUNGEON_INFO_READ*>(inPacket.Buffer);	
	if (pkt)
	{
		GSDungeon()->RecvPartyEnterdRewardCount(*pkt);
	}	
}

// 2022/06/10 PKT - 파티 던전 진행 사항(던전 내 진행 사항)
void FGsNetMessageHandlerWorld::DungeonNotifyPatyDungeonStatus(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] PARTY_DUNGEON_INFO"));
#endif

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsStageManagerGame* stageMgr = (gameFlow) ? gameFlow->GetStageManagerGame() : nullptr;
	if (nullptr == stageMgr)
	{
		GSLOG(Error, TEXT("nullptr == stage"));
		return;
	}

	// 2022/06/20 PKT - 현재는 파티 던전 입장 (포털이동)전에 해당 패킷이 와버려서 현재 위치에 대한 검사를 하면 안됨. 이 부분은 추후 수정될 수 있음.
	//if (FGsStageMode::Game::DUNGEON != stageMgr->GetCurrentStageMode())
	//{	// 2022/06/10 PKT - Dungeon Stage에서만 와야 한다.
	//	GSLOG(Error, TEXT("Invalid Current StageMode : [%d]"), stageMgr->GetCurrentStageMode());
	//	return;
	//}

	TSharedPtr<FGsStageGameBase> findStage = stageMgr->Find(FGsStageMode::Game::DUNGEON);
	if (false == findStage.IsValid())
	{
		GSLOG(Error, TEXT("false == findStage.IsValid()"));
		return;
	}

	FGsStageGameDungeon* stageDungeon = StaticCast<FGsStageGameDungeon*>(findStage.Get());
	if (stageDungeon)
	{
		PD::SC::PKT_SC_PARTY_DUNGEON_INFO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_PARTY_DUNGEON_INFO_READ*>(inPacket.Buffer);
		stageDungeon->PartyDungeonProgressState(*pkt);
	}
}


void FGsNetMessageHandlerWorld::DungeonNotifyPatyDungeonResult(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] PARTY_DUNGEON_END"));
#endif

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsStageManagerGame* stageMgr = (gameFlow) ? gameFlow->GetStageManagerGame() : nullptr;
	if (nullptr == stageMgr)
	{
		GSLOG(Error, TEXT("nullptr == stage"));
		return;
	}

	if (FGsStageMode::Game::DUNGEON != stageMgr->GetCurrentStageMode())
	{	// 2022/06/10 PKT - Dungeon Stage에서만 와야 한다.
		GSLOG(Error, TEXT("Invalid Current StageMode : [%d]"), stageMgr->GetCurrentStageMode());
		return;
	}

	TSharedPtr<FGsStageGameBase> findStage = stageMgr->Find(FGsStageMode::Game::DUNGEON);
	if (false == findStage.IsValid())
	{
		GSLOG(Error, TEXT("false == dungeonStage.IsValid()"));
		return;
	}

	FGsStageGameDungeon* stageDungeon = StaticCast<FGsStageGameDungeon*>(findStage.Get());
	if (stageDungeon)
	{
		PD::SC::PKT_SC_PARTY_DUNGEON_END_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_PARTY_DUNGEON_END_READ*>(inPacket.Buffer);
		stageDungeon->DungeonResultNotify(*pkt);
	}
}

void FGsNetMessageHandlerWorld::EliteDungeonPortalInfo(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] Elite Dungeon Portal Info"));
#endif

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsStageManagerGame* stageMgr = (gameFlow) ? gameFlow->GetStageManagerGame() : nullptr;
	if (nullptr == stageMgr)
	{
		GSLOG(Error, TEXT("nullptr == stage"));
		return;
	}

	if (FGsStageMode::Game::DUNGEON != stageMgr->GetCurrentStageMode())
	{	// 2022/06/10 PKT - Dungeon Stage에서만 와야 한다.
		GSLOG(Error, TEXT("Invalid Current StageMode : [%d]"), stageMgr->GetCurrentStageMode());
		return;
	}

	TSharedPtr<FGsStageGameBase> findStage = stageMgr->Find(FGsStageMode::Game::DUNGEON);
	if (false == findStage.IsValid())
	{
		GSLOG(Error, TEXT("false == dungeonStage.IsValid()"));
		return;
	}

	FGsStageGameDungeon* stageDungeon = StaticCast<FGsStageGameDungeon*>(findStage.Get());
	if (stageDungeon)
	{
		PD::SC::PKT_SC_ELITEDUNGEON_PORTAL_INFO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ELITEDUNGEON_PORTAL_INFO_READ*>(inPacket.Buffer);
		stageDungeon->EliteDungeonPropPotalInfo(*pkt);
	}
}


// local 천마석 ui on off 응답
void FGsNetMessageHandlerWorld::AckSpiritShotOnOff(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[spiritShot] AckSpiritShotOnOff"));
#endif

	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_SPIRIT_SHOT_ON_OFF_READ*>(inPacket.Buffer))
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_UPDATE_SPIRIT_SHOT, nullptr);
		int32_t result = pkt->Result();
		if (result != PACKET_RESULT_SUCCESS)
		{
			GSLOG(Log, TEXT("AckSpiritShotOnOff, Result = %d"), result);
			FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			return;
		}

		bool isSpiritShotOn = pkt->SpiritShotOnOff();
		GItem()->SetSpiritShotStateByUIState(isSpiritShotOn, true);
		GGameData()->SetSpiritShotOnOff(isSpiritShotOn);
	}
}

void FGsNetMessageHandlerWorld::UserSpiritShotActivation(const FGsNet::Packet& inPacket)
{
}


void FGsNetMessageHandlerWorld::NotifyOfflinePlayResult(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Offline Play] NotifyOfflinePlayResult"));
#endif

	PD::SC::PKT_SC_OFF_LINE_MODE_RESULT_READ* packet =
		reinterpret_cast<PD::SC::PKT_SC_OFF_LINE_MODE_RESULT_READ*>(inPacket.Buffer);

	if (nullptr == packet)
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT( "FGsNetMessageHandlerWorld::NotifyOfflinePlayResult : invaild packet nullptr" ));
#endif
		return;
	}

	FGsOfflinePlayResultData data;
	// 2021/06/21 PKT - 플레이 시간(이용 시간)
	data._runningTime = packet->PlayTime();
	// 2021/06/21 PKT - 획득 경험치
	data._Exp = packet->Exp();
	// 2021/06/21 PKT - 획득 재화
	data._CcurrencyTable = packet->CurrencyTable();
	// 2021/06/21 PKT - 획득 아이템
	packet->MakeItemDataCreatedVector<TArray<ItemDataCreated>, ItemDataCreated>(data._itemList);

	data._isPrevAutoState = packet->AutoPlay();

	GSOfflinePlay()->NotifyOfflineResult(data);
}


void FGsNetMessageHandlerWorld::NotifyOfflinePlayRemainingTimeData(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Offline Play] NotifyOfflinePlayRemainingTimeData"));
#endif

	PD::SC::PKT_SC_OFF_LINE_TIME_READ* packet =
		reinterpret_cast<PD::SC::PKT_SC_OFF_LINE_TIME_READ*>(inPacket.Buffer);

	if (nullptr == packet)
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("FGsNetMessageHandlerWorld::NotifyOfflinePlayRemainingTimeData : invaild packet nullptr"));
#endif
		return;
	}

	if (nullptr != GSOfflinePlay())
	{
		bool InisBuyTime = packet->IsUserPayment();
		time_t InFreeRemainingSecounds = packet->FreeUseMinute();
		time_t InPaidRemainingSecounds = packet->UserUseMinute();
		GSOfflinePlay()->NotifyRemainingSeconds(InFreeRemainingSecounds, InPaidRemainingSecounds, InisBuyTime);
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsNetMessageHandlerWorld::NotifyOfflinePlayRemainingTimeData invaild Offline Play Manager"));
#endif
	}
}

void FGsNetMessageHandlerWorld::NotifyOfflinePlayCancel(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Offline Play] NotifyOfflinePlayCancel"));
#endif

	PD::SC::PKT_SC_SERVER_OFF_LINE_CANCEL_READ* packet =
		reinterpret_cast<PD::SC::PKT_SC_SERVER_OFF_LINE_CANCEL_READ*>(inPacket.Buffer);

	if (nullptr == packet)
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("FGsNetMessageHandlerWorld::NotifyOfflinePlayCancel : invaild packet nullptr"));
#endif
		return;
	}

	if (PACKET_RESULT_SUCCESS != packet->Result())
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(packet->Result()));
		return;
	}

	if (nullptr != GSOfflinePlay())
	{
		GSOfflinePlay()->NotifyCancel();
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsNetMessageHandlerWorld::NotifyOfflinePlayCancel invaild Offline Play Manager"));
#endif
	}
}

void FGsNetMessageHandlerWorld::AckOfflinePlayReadyState(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Offline Play] AckOfflinePlayReadyState"));
#endif

	PD::SC::PKT_SC_ACK_OFF_LINE_READY_READ* packet =
		reinterpret_cast<PD::SC::PKT_SC_ACK_OFF_LINE_READY_READ*>(inPacket.Buffer);

	if (nullptr == packet)
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("FGsNetMessageHandlerWorld::AckOfflinePlayReadyState : invaild packet nullptr"));
#endif
		return;
	}

	if (nullptr != GSOfflinePlay())
	{	// 2021/06/30 PKT - 실패시 메니져에서 팝업 띄움
		GSOfflinePlay()->AckReadyState(packet->Result(), packet->CountDownTime());
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsNetMessageHandlerWorld::AckOfflinePlayReadyState invaild Offline Play Manager"));
#endif
	}
}

void FGsNetMessageHandlerWorld::AckOfflinePlayStartPlay(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Offline Play] AckOfflinePlayStartPlay"));
#endif

	PD::SC::PKT_SC_ACK_OFF_LINE_START_READ* packet =
		reinterpret_cast<PD::SC::PKT_SC_ACK_OFF_LINE_START_READ*>(inPacket.Buffer);

	if (nullptr == packet)
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("FGsNetMessageHandlerWorld::AckOfflinePlayStartPlay : invaild packet nullptr"));
#endif
		return;
	}

	if (nullptr != GSOfflinePlay())
	{	// 2021/06/30 PKT - 실패시 메니져에서 팝업 띄움
		GSOfflinePlay()->AckPlayStart(packet->Result());
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsNetMessageHandlerWorld::AckOfflinePlayStartPlay invaild Offline Play Manager"));
#endif
	}
}


void FGsNetMessageHandlerWorld::AckOfflinePlayDebugMode(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_DEBUG_OFF_LINE_MODE_READ* packet =
		reinterpret_cast<PD::SC::PKT_SC_DEBUG_OFF_LINE_MODE_READ*>(inPacket.Buffer);

#if PLATFORM_WINDOWS && !UE_BUILD_SHIPPING
	// 2023/6/27 PKT - Dev & Editor 기능
	if (nullptr == packet)
	{
		GSLOG(Error, TEXT("FGsNetMessageHandlerWorld::AckOfflinePlayStartPlay : invaild packet nullptr"));
		return;
	}

	if (nullptr != GSOfflinePlay())
	{	// 2021/10/21 PKT - OfflinePlay 관전자 모드 시작( 개발용 )
		GSOfflinePlay()->AckLocalPlayerSpectatorMode(packet->Enable());
	}
#endif
}

// 레드닷 알림 통합
void FGsNetMessageHandlerWorld::ContentsStatePacket(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[ContentsState] ContentsState"));
#endif

	PD::SC::PKT_SC_CONTENTS_STATE_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_CONTENTS_STATE_READ*>(inPacket.Buffer);

	GGameData()->Set(*pkt);
}

void FGsNetMessageHandlerWorld::StatisticsInfoAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_STATISTICS_INFO_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ACK_STATISTICS_INFO_READ*>(inPacket.Buffer);

	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	GSGameStatistics()->SetStatisticsInfo(*pkt);

	FGsUIMsgParamInt param(pkt->TabId());
	GMessage()->GetUI().SendMessage(MessageUI::GAME_STATISTICS_UPDATE_LIST, &param);
}

void FGsNetMessageHandlerWorld::StatisticsDailyInfoAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_STATISTICS_DAILY_INFO_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ACK_STATISTICS_DAILY_INFO_READ*>(inPacket.Buffer);

	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	GSGameStatistics()->SetStatisticsDailyInfo(*pkt);

	FGsUIMsgParamInt param(pkt->StatisticsId());
	GMessage()->GetUI().SendMessage(MessageUI::GAME_STATISTICS_UPDATE_GRAPH, &param);
}

void FGsNetMessageHandlerWorld::AckContentsLocked(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::ForcedHideBlockUI(EGsHideBlockUIFactor::CONTENTS_LOCKED);
	
	PD::SC::PKT_SC_CONTENTS_LOCKED_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_CONTENTS_LOCKED_READ*>(inPacket.Buffer);

	if (nullptr == pkt)
	{
		return;
	}
	
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	uiManager->CloseAllStack();

	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	switch (pkt->UnlockType())
	{
	case UnlockType::CONTENTS_CONDITION:
	{
		unlockManager->ShowLockTicker(pkt->ContentsId(), pkt->ContentsCondition());
		break;
	}
	case UnlockType::CATEGORY:
	{
		unlockManager->ShowLockTicker(pkt->ContentsId(), pkt->Category());
		break;
	}
	}
}

void FGsNetMessageHandlerWorld::WarpGuildCampFailure(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("WarpGuildCamp")));

	PD::SC::PKT_SC_ACK_FAIL_GUILD_CAMP_WARP_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ACK_FAIL_GUILD_CAMP_WARP_READ*>(inPacket.Buffer);

	if (nullptr == pkt)
	{
		return;
	}	

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (FGsWarpFunc::CheckWarpFailure(result))
		{
			// CheckWarpFailure 함수 내부에서 티커 출력을 함
			return;
		}

		FGsUIHelper::PopupNetError(result);
	}
}

void FGsNetMessageHandlerWorld::AckInvadeInfo(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_INVADE_INFO_READ*>(inPacket.Buffer))
	{
		GSInterServer()->SetInvasionStayTime(pkt->MyBaseTime(), pkt->MyAddTime(), pkt->KillPoint());

		TArray<TPair<MapId, int>> mapInfoList;
		auto iterEnd = pkt->GetLastMapIdUserCountListIterator();
		for (auto iter = pkt->GetFirstMapIdUserCountListIterator(); iter != iterEnd; iter++)
		{
			mapInfoList.Add(TPair<MapId, int>{iter->MapId(), iter->Count()});
		}

		GSInterServer()->SetMapInvasionInfoList(mapInfoList);
	}
}

void FGsNetMessageHandlerWorld::AckSpawnedInvasionPortalInfo(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_INVADE_PORTAL_INFO_READ*>(inPacket.Buffer))
	{
		if(pkt->Result() != PACKET_RESULT_SUCCESS)
		{
			//PD::Result result = static_cast<PD::Result>(pkt->Result());
			//if(result == PD::Result::INVADE_ERROR_PORTAL_SPAWNING)
			//GSInterServer()->NotReadyInvasionPortalSpot();
		}
		else
		{
			GSInterServer()->ResetSpawnedInvasionPortal(true);

			int count = pkt->GetInvadePortalInfoListCount();
			auto iterEnd = pkt->GetLastInvadePortalInfoListIterator();
			for (auto iter = pkt->GetFirstInvadePortalInfoListIterator(); iter != iterEnd; iter++)
			{
				TMap<SpotId, bool> spawnedSpots;
				auto spotIterEnd = iter->GetLastSpawnSpotIdListIterator();
				for (auto spotIter = iter->GetFirstSpawnSpotIdListIterator(); spotIter != spotIterEnd; spotIter++)
				{
					spawnedSpots.Add(spotIter->SpotId(), spotIter->IsActive());
				}

				GSInterServer()->SetInvasionPortalSpotListInMap(iter->MapId(), spawnedSpots);
			}
			GSInterServer()->ResetSpawnedInvasionPortal(false);
		}
	}
}


void FGsNetMessageHandlerWorld::AckActiveInvasionPortalList(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_ACTIVE_INVADE_PORTAL_INFO_LIST_READ*>(inPacket.Buffer))
	{
		TArray<ActiveInvadePortalInfo> activePortalInMaps;
		pkt->MakeUserActiveInvadePortalInfoListVector<TArray<ActiveInvadePortalInfo>, ActiveInvadePortalInfo>(activePortalInMaps);

		//GSInterServer()->SetActiveInvasionPortalInMap(activePortalInMaps);
	}
}

//침공전 시작 아님, 침공전 입장
void FGsNetMessageHandlerWorld::InvadeEnter(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_INVADE_ENTER_READ*>(inPacket.Buffer))
	{
		GSInterServer()->EnterInvasionState(pkt->TimeEnter(), pkt->MyBaseTime(), pkt->MyAddTime());
		GSInterServer()->SetInvaderTotalCount(pkt->InvadePlayCount());
		GSInterServer()->SetWorldId(pkt->WorldServerId());
		GMessage()->GetInvasion().SendMessage(MessageInvasion::INVALID_INVASION_USER_COUNT, nullptr);
	}

	//침공전 환경 이벤트 실행
	FGsPrimitiveInt32 envParam(static_cast<int32>(EGsEnvEvent::INVASION));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_START_EVENT, &envParam);

	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);

	GMessage()->GetInvasion().SendMessage(MessageInvasion::INVAION_START, nullptr);
}

//침공전 끝 아님, 침공전 떠남 
void FGsNetMessageHandlerWorld::InvadeLeave(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_INVADE_LEAVE_READ*>(inPacket.Buffer))
	{
		GSInterServer()->LeaveInvasionState(pkt->TimeLeave(), pkt->MyBaseTime(), pkt->MyAddTime());
	}

	//침공전 환경 이벤트 종료
	FGsPrimitiveInt32 envParam(static_cast<int32>(EGsEnvEvent::INVASION));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_END_EVENT, &envParam);

	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);

	GMessage()->GetInvasion().SendMessage(MessageInvasion::INVAION_FINISH, nullptr);
}
//invasion 하기 전에 받아야 한다. - 대륙 맵과 침공가능 맵에서 확인 가능하므로
void FGsNetMessageHandlerWorld::InvadeInfo(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_INVADE_INFO_READ*>(inPacket.Buffer))
	{
		GSInterServer()->SetInvasionStayTime(pkt->MyBaseTime(), pkt->MyAddTime(), pkt->KillPoint());
		GSInterServer()->SetInvaderTotalCount(pkt->InvadePlayCount());
		GMessage()->GetInvasion().SendMessage(MessageInvasion::INVALID_INVASION_USER_COUNT, nullptr);
	}	
}

void FGsNetMessageHandlerWorld::InvadeNumOfCurrentInvader(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_INVADE_PLAY_COUNT_READ*>(inPacket.Buffer))
	{
		GSInterServer()->SetInvaderTotalCount(pkt->InvadePlayCount());
		GMessage()->GetInvasion().SendMessage(MessageInvasion::INVALID_INVASION_USER_COUNT, nullptr);
	}	
}

void FGsNetMessageHandlerWorld::InvadeNumOfInvaderViaPortal(const FGsNet::Packet& inPacket)
{
	if (auto pkt = reinterpret_cast<PD::SC::PKT_SC_INVADE_PORTAL_USER_COUNT_READ*>(inPacket.Buffer))
	{
	//	GSInterServer()->SetInvaderCountViaPortal(pkt->PortalGameId(), pkt->InvadeUserCount());

		FGsUIMsgParamInt param(pkt->PortalGameId());
		GMessage()->GetInvasion().SendMessage(MessageInvasion::INVALID_INVADER_COUNT_VIAPORTAL, &param);
	}
}

//침공전 시작을 알리는 패킷 
void FGsNetMessageHandlerWorld::NotifyStartInvasionTime(const FGsNet::Packet& inPacket)
{
	//GSInterServer()->OnNotiOninvasionTime();

	if (UGsEventManager* eventManager = GSEvent())
	{
		eventManager->EventActionPlayImmediate(INVADE_START_MESSAGE_ID);
	}
}

void FGsNetMessageHandlerWorld::TerritoryUpdate(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_TERRITORY_UPDATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_TERRITORY_UPDATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	
	FGsInvasionChaosParam param = FGsInvasionChaosParam(pkt->IsChaos());
	//invalid player widget
	GMessage()->GetInvasion().SendMessage(MessageInvasion::INVALID_TOWN_CHAOS_FENCE, &param);
}

//침공전 맵 들어갈 때 마다 들어오는 패킷
void FGsNetMessageHandlerWorld::InvadeStart(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_INVADE_START_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_INVADE_START_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GSInterServer()->OnInvasionStart(pkt->TimeStart(), pkt->TimeFinish());

	GGameData()->SetInvasionDefenseState(true);

	//침공전 환경 이벤트 실행
	FGsPrimitiveInt32 envParam(static_cast<int32>(EGsEnvEvent::INVASION));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_START_EVENT, &envParam);

	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);
	
	GMessage()->GetInvasion().SendMessage(MessageInvasion::INVAION_START, nullptr);
}

//침공전 끝날 때만 드렁오는 패킷
void FGsNetMessageHandlerWorld::InvadeFinish(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_INVADE_FINISH_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_INVADE_FINISH_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}	

	GGameData()->SetInvasionDefenseState(false);

	if (UGsEventManager* eventManager = GSEvent())
	{
		eventManager->EventActionPlayImmediate(INVADE_END_MESSAGE_ID);
	}

	//침공전 환경 이벤트 종료
	FGsPrimitiveInt32 envParam(static_cast<int32>(EGsEnvEvent::INVASION));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_END_EVENT, &envParam);

	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);

	GMessage()->GetInvasion().SendMessage(MessageInvasion::INVAION_FINISH, nullptr);

	// 
}

void FGsNetMessageHandlerWorld::TutorialList(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_TUTORIAL_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_TUTORIAL_READ*>(inPacket.Buffer);

	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		tutorialMgr->SetCompleteTutorialList(*pkt);
	}
}

void FGsNetMessageHandlerWorld::TutorialSaveAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_TUTORIAL_SAVE_READ* pkt = 
		reinterpret_cast<PD::SC::PKT_SC_ACK_TUTORIAL_SAVE_READ*>(inPacket.Buffer);

	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	GSTutorial()->SetCompleteTutorial(pkt->TutorialId());
}

void FGsNetMessageHandlerWorld::AckDuelInvite(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_DUEL_INVITE_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ACK_DUEL_INVITE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsArenaManager* arenaManager = GSArena();
	if (nullptr == arenaManager)
		return;

	arenaManager->GetArenaHandler()->Set(*pkt);
}

void FGsNetMessageHandlerWorld::AckDuelInviteCancel(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_DUEL_INVITE_CANCEL_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ACK_DUEL_INVITE_CANCEL_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsArenaManager* arenaManager = GSArena();
	if (nullptr == arenaManager)
		return;

	arenaManager->GetArenaHandler()->Set(*pkt);
}

void FGsNetMessageHandlerWorld::AckDuelAccept(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_DUEL_ACCEPT_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ACK_DUEL_ACCEPT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsArenaManager* arenaManager = GSArena();
	if (nullptr == arenaManager)
		return;

	arenaManager->GetArenaHandler()->Set(*pkt);
}

void FGsNetMessageHandlerWorld::NotifyDuelInviteCancel(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_DUEL_INVITE_CANCEL_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_NOTIFY_DUEL_INVITE_CANCEL_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsArenaManager* arenaManager = GSArena();
	if (nullptr == arenaManager)
		return;

	arenaManager->GetArenaHandler()->Set(*pkt);
}

void FGsNetMessageHandlerWorld::NotifyDuelInvited(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_DUEL_INVITED_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_NOTIFY_DUEL_INVITED_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsArenaManager* arenaManager = GSArena();
	if (nullptr == arenaManager)
		return;

	arenaManager->GetArenaHandler()->Set(*pkt);
}

void FGsNetMessageHandlerWorld::NotifyDuelInviteResult(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_DUEL_INVITE_RESULT_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_NOTIFY_DUEL_INVITE_RESULT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsArenaManager* arenaManager = GSArena();
	if (nullptr == arenaManager)
		return;

	arenaManager->GetArenaHandler()->Set(*pkt);
}

void FGsNetMessageHandlerWorld::NotifyDuelCountStart(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_DUEL_COUNT_START_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_NOTIFY_DUEL_COUNT_START_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsArenaManager* arenaManager = GSArena();
	if (nullptr == arenaManager)
		return;

	arenaManager->GetArenaHandler()->Set(*pkt);
}

void FGsNetMessageHandlerWorld::NotifyDuelStart(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_DUEL_START_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_NOTIFY_DUEL_START_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsArenaManager* arenaManager = GSArena();
	if (nullptr == arenaManager)
		return;

	arenaManager->GetArenaHandler()->Set(*pkt);
}

void FGsNetMessageHandlerWorld::NotifyDuelResult(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_DUEL_RESULT_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_NOTIFY_DUEL_RESULT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsArenaManager* arenaManager = GSArena();
	if (nullptr == arenaManager)
		return;

	arenaManager->GetArenaHandler()->Set(*pkt);
}

void FGsNetMessageHandlerWorld::NotifyDuelReconnectInfo(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_DUEL_RECONNECT_INFO_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_NOTIFY_DUEL_RECONNECT_INFO_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsArenaManager* arenaManager = GSArena();
	if (nullptr == arenaManager)
		return;

	arenaManager->GetArenaHandler()->SetReconnectInfo(*pkt);
}

// notify target share 
void FGsNetMessageHandlerWorld::PartyTarget(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_PARTY_TARGET_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_NOTIFY_PARTY_TARGET_READ*>(inPacket.Buffer);

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}
	int64 sharedTargetGameId = pkt->GameId();
	partyHandler->RecvPartyTargetClick(true, sharedTargetGameId);
}

// notify target share cancel
void FGsNetMessageHandlerWorld::PartyTargetCancel(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_PARTY_TARGET_CANCEL_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_NOTIFY_PARTY_TARGET_CANCEL_READ*>(inPacket.Buffer);

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}
	partyHandler->RecvPartyTargetClick(false);
}

void FGsNetMessageHandlerWorld::AckUserStatBookmarkList(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_LIST_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}	

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		return;
	}

	FGsPlayerBookmarkStatController* bookmarkController = FGsPlayerStatHandler::GetInstance()->GetBookmarkStatController();
	if (bookmarkController)
	{
		bookmarkController->SetBookmarkStat(pkt);
	}

	//invalid player widget
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PLAYER_STAT, nullptr);
}

void FGsNetMessageHandlerWorld::AckUserStatBookmarkAdd(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_ADD_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_ADD_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
		return;
	}

	FGsPlayerBookmarkStatController* bookmarkController = FGsPlayerStatHandler::GetInstance()->GetBookmarkStatController();
	if (bookmarkController)
	{
		bookmarkController->AddBookmarkStat(pkt);
	}

	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PLAYER_STAT, nullptr);
}

void FGsNetMessageHandlerWorld::AckUserStatBookmarkDelete(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_DELETE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_USER_STAT_BOOKMARK_DELETE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
		return;
	}

	FGsPlayerBookmarkStatController* bookmarkController = FGsPlayerStatHandler::GetInstance()->GetBookmarkStatController();
	if (bookmarkController)
	{
		bookmarkController->RemoveBookmarkStat(pkt);
	}

	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PLAYER_STAT, nullptr);	
}

void FGsNetMessageHandlerWorld::CharacterSlotUnLock(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_CHARACTER_SLOT_UNLOCK_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_CHARACTER_SLOT_UNLOCK_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FText findText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_ExtendCharacterSlot"), findText);
	
	if (!findText.IsEmpty())
	{	
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}
}

void FGsNetMessageHandlerWorld::AckUserPresetChange(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_USER_PRESET_CHANGE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_USER_PRESET_CHANGE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPresetManager* presetManager = GSPreset();
	if (nullptr == presetManager)
		return;

	FGsCoolTimeManager* coolTimeManager = GCoolTime();
	if (nullptr == coolTimeManager)
		return;

	FGsGameDataManager* gameDataManager = GGameData();
	if (nullptr == gameDataManager)
		return;

	presetManager->AckUserPresetChange();
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		//GItem()->OnRollbackLoginCreatureWeaponType();
		presetManager->ProcessNetError(pkt->Result());
		return;
	}
	
	gameDataManager->SetUserPreset(pkt->PresetId());
	coolTimeManager->SetCoolTime(EGsCoolTime::Preset, 0, 0, pkt->PresetChangeCoolDown(), true);
	if (0.f < pkt->ClassWeaponChangeCoolDown())
	{
		coolTimeManager->SetCoolTime(EGsCoolTime::Change_Weapon, 0, 0, pkt->ClassWeaponChangeCoolDown(), true);
	}

	FGsPrimitiveUInt8 param(pkt->PresetId());
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_USER_PRESET_ID, &param);
}

void FGsNetMessageHandlerWorld::AckUserPresetCopyTo(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_USER_PRESET_COPY_TO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_USER_PRESET_COPY_TO_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}
}

void FGsNetMessageHandlerWorld::AckUserPresetName(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_USER_PRESET_NAME_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_USER_PRESET_NAME_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPresetManager* presetManager = GSPreset();
	if (nullptr == presetManager)
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	presetManager->SetPresetName(pkt->PresetId(), pkt->PresetName());
	GMessage()->GetUI().SendMessage(MessageUI::INVALIDATE_PRESET_NAME, nullptr);
}

void FGsNetMessageHandlerWorld::NotifyUserPresetName(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_USER_PRESET_NAME_LIST_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_USER_PRESET_NAME_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPresetManager* presetManager = GSPreset();
	if (nullptr == presetManager)
		return;

	using PresetNameListIterator = PktIterator<PD::SC::PKT_SC_USER_PRESET_NAME_LIST_READ::PresetNameList>;
	for (PresetNameListIterator iter = pkt->GetFirstPresetNameListIterator(); iter != pkt->GetLastPresetNameListIterator(); ++iter)
	{
		presetManager->SetPresetName(iter->PresetId(), iter->PresetName());
	}
	
	GMessage()->GetUI().SendMessage(MessageUI::INVALIDATE_PRESET_NAME, nullptr);
}

void FGsNetMessageHandlerWorld::InvadeMinimapInfoUpdate(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_INVADE_MINIMAP_INFO_UPDATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_INVADE_MINIMAP_INFO_UPDATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GSWorldMap()->SetRegionInvadeInfoByServerPacket(pkt);
}

// 시네마틱
void FGsNetMessageHandlerWorld::AckCinematicStart(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_CLIENT_CINEMATIC_START_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_CLIENT_CINEMATIC_START_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (PD::Result::CLIENT_CINEMATIC_ERROR_WARP == result)
		{
			FGsMessageHolder* msgHolder = GMessage();
			if (msgHolder != nullptr)
			{
				msgHolder->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_ERROR_STOP, nullptr);
			}

			FGsUIHelper::TraySectionMessageTickerNetError(result);
			return;			
		}
		else if (PD::Result::CLIENT_CINEMATIC_ALREADY == result)
		{
			FGsUIHelper::TraySectionMessageTickerNetError(result);
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	uint32 sequenceResId = pkt->SequenceResId();
	bool isRewarded = pkt->IsRewarded();

	GSLOG(Log, TEXT("AckCinematicStart -> sequenceResId : %d, isRewarded : %d"), sequenceResId, isRewarded);
	
	GSequence()->SetCinematicStart(sequenceResId, isRewarded);
}

void FGsNetMessageHandlerWorld::AckCinematicEnd(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_CLIENT_CINEMATIC_END_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_CLIENT_CINEMATIC_END_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	uint32 sequenceResId = pkt->SequenceResId();
	bool isRewarded = pkt->GetReward();

	GSLOG(Log, TEXT("AckCinematicEnd -> sequenceResId : %d, isRewarded : %d"), sequenceResId, isRewarded);

	GSequence()->SetCinematicEnd(sequenceResId, isRewarded);
}

void FGsNetMessageHandlerWorld::AckPersonalTradingTry(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_PERSONAL_TRADING_TRY_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_PERSONAL_TRADING_TRY_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		tradeManager->ProcessNetError(pkt->Result());
		return;
	}

	tradeManager->AckPersonalTradingTry(*pkt);
}

void FGsNetMessageHandlerWorld::AckPersonalTradingAccept(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_PERSONAL_TRADING_ACCEPT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_PERSONAL_TRADING_ACCEPT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	tradeManager->AckPersonalTradingAccept(pkt->Result());
}

void FGsNetMessageHandlerWorld::AckPersonalTradingCancel(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_PERSONAL_TRADING_CANCEL_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_PERSONAL_TRADING_CANCEL_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	if (false == tradeManager->IsTradeSet())
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		tradeManager->ProcessNetError(pkt->Result());
		return;
	}

	tradeManager->AckPersonalTradingCancel();
}

void FGsNetMessageHandlerWorld::AckPersonalTradingAbort(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_PERSONAL_TRADING_ABORT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_PERSONAL_TRADING_ABORT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	
	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	if (false == tradeManager->IsTradeSet())
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		tradeManager->ProcessNetError(pkt->Result());
		return;
	}

	tradeManager->AckPersonalTradingAbort();
}

void FGsNetMessageHandlerWorld::AckPersonalTradingLock(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_PERSONAL_TRADING_LOCK_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_PERSONAL_TRADING_LOCK_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		tradeManager->ProcessNetError(pkt->Result());
		return;
	}
	
	tradeManager->AckPersonalTradingLock();
}

void FGsNetMessageHandlerWorld::AckPersonalTradingConfirm(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_PERSONAL_TRADING_CONFIRM_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_PERSONAL_TRADING_CONFIRM_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;
		
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		tradeManager->ProcessNetError(pkt->Result());
		return;
	}

	tradeManager->AckPersonalTradingConfirm();
}

void FGsNetMessageHandlerWorld::AckPersonalTradingPutItem(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_PERSONAL_TRADING_PUT_ITEM_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_PERSONAL_TRADING_PUT_ITEM_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	bool isSuccess = true;
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		isSuccess = false;
		tradeManager->ProcessNetError(pkt->Result());
	}

	tradeManager->AckLocalItemUpdate(isSuccess);
}

void FGsNetMessageHandlerWorld::AckPersonalTradingPutDia(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_PERSONAL_TRADING_PUT_DIA_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_PERSONAL_TRADING_PUT_DIA_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	bool isSuccess = true;
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		isSuccess = false;
		tradeManager->ProcessNetError(pkt->Result());
	}
	
	tradeManager->AckLocalDiaAmount(isSuccess);
}

void FGsNetMessageHandlerWorld::NotifyPersonalTrading(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_INFORM_PERSONAL_TRADING_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_INFORM_PERSONAL_TRADING_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	tradeManager->NotifyPersonalTrading(*pkt);
}

void FGsNetMessageHandlerWorld::NotifyPersonalTradingItemUpdate(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_INFORM_PERSONAL_TRADING_ITEM_UPDATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_INFORM_PERSONAL_TRADING_ITEM_UPDATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	tradeManager->NotifyPersonalTradingItemUpdate(pkt->TradingGameId(), pkt->AddItemData(), pkt->ItemAmount());
}

void FGsNetMessageHandlerWorld::NotifyPersonalTradingItemEquipUpdate(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_INFORM_PERSONAL_TRADING_ITEM_EQUIP_UPDATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_INFORM_PERSONAL_TRADING_ITEM_EQUIP_UPDATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	tradeManager->NotifyPersonalTradingItemEquipUpdate(pkt->TradingGameId(), pkt->AddItemEquipData());
}

void FGsNetMessageHandlerWorld::NotifyPersonalTradingItemRemove(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_INFORM_PERSONAL_TRADING_ITEM_REMOVE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_INFORM_PERSONAL_TRADING_ITEM_REMOVE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	tradeManager->NotifyPersonalTradingItemRemove(pkt->TradingGameId(), pkt->ItemDBId());
}

void FGsNetMessageHandlerWorld::NotifyPersonalTradingDiaUpdate(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_INFORM_PERSONAL_TRADING_DIA_UPDATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_INFORM_PERSONAL_TRADING_DIA_UPDATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsPersonalTradingManager* tradeManager = GSPersonalTrading();
	if (nullptr == tradeManager)
		return;

	tradeManager->NotifyPersonalTradingDiaUpdate(pkt->TradingGameId(), pkt->Dia());
}

void FGsNetMessageHandlerWorld::CheatPersonalTradingDailyTradingCntUpdate(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_FORCE_DAILY_TRADING_CNT_UPDATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_FORCE_DAILY_TRADING_CNT_UPDATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->SetDailyTradingCount(pkt->DailyTradingCnt());
}

void FGsNetMessageHandlerWorld::AckLatency(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_LATENCY_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_LATENCY_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

//#if WITH_EDITOR  
	GSLOG(Log, TEXT("[Latency] SC_ACK_LATENCY : %s"), *pkt->TimeStamp());
//#endif
}

void FGsNetMessageHandlerWorld::AckUserContextOtherServer(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_USER_CONTEXT_UI_OTHER_SERVER_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_USER_CONTEXT_UI_OTHER_SERVER_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		GMessage()->GetUserContext().SendMessage(MessageContentUserContext::INVALID_USER_OTHER_SERVER, nullptr);
		return;
	}

	if (pkt->UserDBId() == INVALID_USER_DB_ID)
	{
		GMessage()->GetUserContext().SendMessage(MessageContentUserContext::INVALID_USER_OTHER_SERVER, nullptr);
		return;
	}

	FGsUserContextInfo info(*pkt);
	FGsUIMsgParamContextUserInfo param(info);
	GMessage()->GetUserContext().SendMessage(MessageContentUserContext::VALID_USER, &param);
}
// summon

// 페어리 뽑기 마일리지 보상 응답
void FGsNetMessageHandlerWorld::AckFairySummonMileage(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_FAIRY_SUMMON_MILEAGE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_FAIRY_SUMMON_MILEAGE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 실패라면
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		return;
	}

	// 성공 티커 출력
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}
	TWeakObjectPtr<UGsUIWidgetBase> openWidget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerMileageSummon"));
	if (openWidget.IsValid())
	{
		TWeakObjectPtr<UGsUITrayTickerMileageSummon> trayTickerMileageSummon = Cast<UGsUITrayTickerMileageSummon>(openWidget);
		if (trayTickerMileageSummon.IsValid())
		{
			trayTickerMileageSummon->StartDataTickerMileageSummon(false, EGsSummonType::SUMMON_FAIRY);
		}
	}

	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	soundPlayer->PlaySound2D(TEXT("UI_RewardOne_1"));
}
// 코스튬 뽑기 마일리지 보상 응답
void FGsNetMessageHandlerWorld::AckCostumeSummonMileage(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_COSTUME_SUMMON_MILEAGE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_COSTUME_SUMMON_MILEAGE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 실패라면
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		return;
	}

	// 성공 티커 출력
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}
	TWeakObjectPtr<UGsUIWidgetBase> openWidget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerMileageSummon"));
	if (openWidget.IsValid())
	{
		TWeakObjectPtr<UGsUITrayTickerMileageSummon> trayTickerMileageSummon = Cast<UGsUITrayTickerMileageSummon>(openWidget);
		if (trayTickerMileageSummon.IsValid())
		{
			trayTickerMileageSummon->StartDataTickerMileageSummon(false, EGsSummonType::SUMMON_COSTUME);
		}
	}

	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	soundPlayer->PlaySound2D(TEXT("UI_RewardOne_1"));
}

// 유니크 페어리 뽑기 마일리지 보상 응답
void FGsNetMessageHandlerWorld::AckUniqueFairySummonMileage(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_UNIQUE_FAIRY_SUMMON_MILEAGE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_UNIQUE_FAIRY_SUMMON_MILEAGE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 실패라면
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		return;
	}

	// 성공 티커 출력
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}
	TWeakObjectPtr<UGsUIWidgetBase> openWidget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerMileageSummon"));
	if (openWidget.IsValid())
	{
		TWeakObjectPtr<UGsUITrayTickerMileageSummon> trayTickerMileageSummon = Cast<UGsUITrayTickerMileageSummon>(openWidget);
		if (trayTickerMileageSummon.IsValid())
		{
			trayTickerMileageSummon->StartDataTickerMileageSummon(true, EGsSummonType::SUMMON_FAIRY);
		}
	}

	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	soundPlayer->PlaySound2D(TEXT("UI_RewardOne_1"));
}
// 유니크 코스튬 뽑기 마일리지 보상 응답
void FGsNetMessageHandlerWorld::AckUniqueCostumeSummonMileage(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_UNIQUE_COSTUME_SUMMON_MILEAGE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_UNIQUE_COSTUME_SUMMON_MILEAGE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 실패라면
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		return;
	}

	// 성공 티커 출력
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}
	TWeakObjectPtr<UGsUIWidgetBase> openWidget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerMileageSummon"));
	if (openWidget.IsValid())
	{
		TWeakObjectPtr<UGsUITrayTickerMileageSummon> trayTickerMileageSummon = Cast<UGsUITrayTickerMileageSummon>(openWidget);
		if (trayTickerMileageSummon.IsValid())
		{
			trayTickerMileageSummon->StartDataTickerMileageSummon(true, EGsSummonType::SUMMON_COSTUME);
		}
	}

	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	soundPlayer->PlaySound2D(TEXT("UI_RewardOne_1"));
}
// 유니크 마일리지로 뽑은 횟수 정보
void FGsNetMessageHandlerWorld::UniqueMileageObtainedInfo(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_UNIQUE_MILEAGE_OBTAINED_INFO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_UNIQUE_MILEAGE_OBTAINED_INFO_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GSSummon()->SetUniqueMileageRewardObtainCount(pkt->UniqueCostumeMileageObtained(), pkt->UniqueFairyMileageObtained());

	// 소환 마일리지 팝업 정보 갱신
	GMessage()->GetUI().SendMessage(MessageUI::CHANGED_SUMMON_MILEAGE_DATA, nullptr);
}

void FGsNetMessageHandlerWorld::BuddyLoginNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_BUDDY_LOGIN_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_NOTIFY_BUDDY_LOGIN_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyLoginNotify(pkt->UserDBId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_LOGIN- ID: %llu"), pkt->UserDBId());
#endif
}

void FGsNetMessageHandlerWorld::BuddyLogoutNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NOTIFY_BUDDY_LOGOUT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_NOTIFY_BUDDY_LOGOUT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsBuddyHandler* buddyHandler = GSBuddy()->GetBuddyHandler();
	if (nullptr == buddyHandler)
		return;

	buddyHandler->RecvBuddyLogoutNotify(pkt->UserDBId());

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] RECV NOTIFY_BUDDY_LOGOUT- ID: %llu"), pkt->UserDBId());
#endif
}