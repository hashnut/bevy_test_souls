// Fill out your copyright notice in the Description page of Project Settings.

#define SPACECRACK_DEBUG true

#include "GsNetMessageHandlerGameObject.h"

#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "Shared/Client/SharedEnums/SharedDebugEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedPropEnum.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"

#include "UTIL/GsText.h"
#include "UTIL/GsDir.h"
#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsDebugUtil.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsGameObjectUtil.h"

#include "GSGameInstance.h"

#include "GameMode/GsGameModeWorld.h"
#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"
#include "GameMode/GsGameModeLobby.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Scan/GsScanHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Observer/GsObserverHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Sanctum/GsSanctumHandler.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGame/GsDropObjectManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"
#include "Management/ScopeGame/GsSaveBatteryManager.h"
#include "Management/ScopeGame/GsCampManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsArenaManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsSanctumManager.h"

#include "Data/GsResourceManager.h"

#include "Net/MessageHandler/GsNetMessageHandlerGameObject.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsCoolTimeMessageParam.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"
#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/MessageParam/GsSanctumMessageParam.h"

//ObjectClass
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataRemotePlayer.h"
#include "GameObject/Stat/GsStatNonPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/ObjectClass/GsGameObjectProjectile.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectSanctumBase.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Interaction/GsInteractionHandlerBase.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/Fairy/GsFairyHandlerPlayer.h"

#include "Warp/GsWarpFunc.h"

#include "Skill/GsSkillHelper.h"

//Movement
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerRemote.h"
#include "GameObject/Movement/GsMovementHandlerProjectile.h"
#include "GameObject/Damage/GsDamageHandlerBase.h"

#include "GameObject/Prop/GsSchemaPropData.h"
#include "GameObject/ObjectClass/GsGameObjectPropInteraction.h"
#include "GameObject/ObjectClass/GsGameObjectPropTouch.h"
#include "GameObject/ObjectClass/GsGameObjectSandbag.h"
#include "GameObject/ObjectClass/GsGameObjectCamp.h"
#include "GameObject/ObjectClass/GsGameObjectSanctumDefault.h"
#include "GameObject/ObjectClass/GsGameObjectSanctumNexus.h"
#include "GameObject/ObjectClass/GsGameObjectDropObject.h"
#include "GameObject/Abnormality/CrowdControl/GsCrowdControlLocalPlayer.h"

// Customize
#include "GameObject/Customize/GsCustomizeHandler.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsActorProjectile.h"
#include "ControllerEx/GsPlayerController.h"

//Movement
#include "ActorComponentEx/GsMovementDebugShapeComponent.h"

//RSQL
#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/GameObject/GsSchemaPlayerShape.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "DataSchema/GsSchemaGameMode.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyMoveLocation.h"

#include "Data/GsResourceManager.h"
#include "Data/GsDataContainManager.h"

#include "Cheat/GsCheatManager.h"

#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDAuto.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Popup/GsUIPopupRevive.h"

#include "UI/UIContent/Tray/GsUITrayTickerFieldBoss.h"
#include "UI/UIContent/Tray/GsUITrayAcquireSkill.h"
#include "UI/UIContent/Tray/GsUITrayAcquireCostumeSkill.h"

#include "Currency/GsCurrencyHelper.h"

// Item
#include "Item/GsItemManager.h"
#include "Item/GsItem.h"

#include "T1Project.h"

#include "T1Project/Guild/GsGuildHelper.h"


#ifdef NAVIMESH_TEST_LOCATION_LOG
#include "Cheat/GsCheatManager.h"
#include "Input/GsInputEventMsgBase.h"
#include "Message/GsMessageGameObject.h"
#include "DrawDebugHelpers.h"
#endif // NAVIMESH_TEST_LOCATION_LOG

#include "Kismet/GameplayStatics.h"
#include "Animation/GsAnimInstanceState.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "T1Project/Item/GsItemHelper.h"
#include "GameObject/Boss/GsBossHandler.h"
#include "GameObject/Skill/IdleEmotion/GsIdleEmotionBaseRunner.h"
#include "Management/ScopeGame/GsEventManager.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"


FGsNetMessageHandlerGameObject::~FGsNetMessageHandlerGameObject()
{

}

void FGsNetMessageHandlerGameObject::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& protocolWorld = inManager->GetProtocalWorld();

	InitializeMessageDelegateList(&protocolWorld);

	// 아래로 쭈욱 등록
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SPAWN_NPC, this, &FGsNetMessageHandlerGameObject::SpawnNpcAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_DESPAWN_NPC, this, &FGsNetMessageHandlerGameObject::DespawnNpcAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SPAWN_SPECIAL_NPC, this, &FGsNetMessageHandlerGameObject::SpawnSpecialNpc)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SPAWN_TREASURE_NPC, this, &FGsNetMessageHandlerGameObject::SpawnTreasureNpc)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_DESPAWN_TREASURE_NPC, this, &FGsNetMessageHandlerGameObject::DespawnTreasureNpc)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_TREASURE_NPC_UPDATE, this, &FGsNetMessageHandlerGameObject::UpdateTreasureNpcGuild)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_USER_TREASURE_NPC_POSITION, this, &FGsNetMessageHandlerGameObject::UpdateTreasureNpcPosition)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_NPC_CHANGE_SPECIAL_STATUS, this, &FGsNetMessageHandlerGameObject::NpcChangeSpecialStataus)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SPAWN_ME, this, &FGsNetMessageHandlerGameObject::SpawnMeAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SPAWN_ME_COMPLETE, this, &FGsNetMessageHandlerGameObject::SpawnMeCompleteAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_SPAWN_ME_ACTION,
		this, &FGsNetMessageHandlerGameObject::SpawnMeAction)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_STAT_INFO, this, &FGsNetMessageHandlerGameObject::StatInfoNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SPAWN_USER, this, &FGsNetMessageHandlerGameObject::SpawnUserAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_DESPAWN_USER, this, &FGsNetMessageHandlerGameObject::DespawnUserAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SPAWN_PROJECTILE, this, &FGsNetMessageHandlerGameObject::SpawnProjectileAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_DESPAWN_PROJECTILE, this, &FGsNetMessageHandlerGameObject::DespawnProjectileAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SKILL, this, &FGsNetMessageHandlerGameObject::SkillAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_LOAD_TOPO, this, &FGsNetMessageHandlerGameObject::StatLoadTopo)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_WARP_FAILURE, this, &FGsNetMessageHandlerGameObject::WarpFailure)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_USER_EXP_UPDATE, this, &FGsNetMessageHandlerGameObject::UserExpUpdate)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_USER_CURRENCY_FINAL_UPDATE, this, &FGsNetMessageHandlerGameObject::UserCurrencyUpdate)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_USER_BASIC_DATA, this, &FGsNetMessageHandlerGameObject::UserBasicDataAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_MOVE_TO_LOCATION, this, &FGsNetMessageHandlerGameObject::MoveToLocationAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_MOVE_IMMEDIATE_STOP, this, &FGsNetMessageHandlerGameObject::MoveImmediateStopAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_MOVE_FAILED, this, &FGsNetMessageHandlerGameObject::MoveFailedAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_CORRECTION_POS, this, &FGsNetMessageHandlerGameObject::MoveCorrectionPosNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_REQ_POS_VALIDATE, this, &FGsNetMessageHandlerGameObject::RequestPosValidate)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_FORCED_POS, this, &FGsNetMessageHandlerGameObject::ForcedPosNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_START_ROTATION, this, &FGsNetMessageHandlerGameObject::StartRotationNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_CREATURE_SKILL_ROOT_MOTION_START, this, &FGsNetMessageHandlerGameObject::CreatureSkillRootmotionStartNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_CREATURE_SKILL_ROOT_MOTION_FINISH, this, &FGsNetMessageHandlerGameObject::CreatureSkillRootmotionFinishNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_DEBUG_SHAPE, this, &FGsNetMessageHandlerGameObject::DebugShapeAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_DEBUG_SHAPE_LINE, this, &FGsNetMessageHandlerGameObject::DebugShapeLineAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_DEBUG_SHAPE_CYLINDER_LIST, this, &FGsNetMessageHandlerGameObject::DebugShapeCylinderAck)
	);

#if	!UE_BUILD_SHIPPING
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_DEBUG_TEXT_INFO, this, &FGsNetMessageHandlerGameObject::DebugTextInfoAck)
	);
#endif

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_OBJECT_COLLISION_DEBUG_SHAPE, this, &FGsNetMessageHandlerGameObject::ObjectCollisionDebugShape)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_USER_LOOK_INFO_CHANGED, this, &FGsNetMessageHandlerGameObject::UserLookInfoChangeAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_USER_IFF_STATUS, this, &FGsNetMessageHandlerGameObject::UserIffStatusAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_IFF_STATUS, this, &FGsNetMessageHandlerGameObject::IffStatusAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_UPDATE_IFF_TEAM, this, &FGsNetMessageHandlerGameObject::UpdateIffTeamAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_START_SKILL, this, &FGsNetMessageHandlerGameObject::StartSkillAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SKILL_COOLDOWN, this, &FGsNetMessageHandlerGameObject::SkillCoolDownAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SKILL_FINISHED, this, &FGsNetMessageHandlerGameObject::SkillFinishedAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_START_SKILL_FAILURE, this, &FGsNetMessageHandlerGameObject::SkillFailureNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SKILL_DAMAGED, this, &FGsNetMessageHandlerGameObject::SkillDamagedNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_PASSIVITY_DAMAGED, this, &FGsNetMessageHandlerGameObject::PassivityDamageNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SKILL_HEALED, this, &FGsNetMessageHandlerGameObject::SkillHealedNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SKILL_EFFECT, this, &FGsNetMessageHandlerGameObject::SkillEffectNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_CREATURE_DEATH, this, &FGsNetMessageHandlerGameObject::CreatureDeathAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_USER_RESURRECT, this, &FGsNetMessageHandlerGameObject::UserResurrectAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_USER_RESURRECT_TOWN_AUTO_RESPAWN, this, &FGsNetMessageHandlerGameObject::UserAutoResurrectAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_RESURRECT_FAILURE, this, &FGsNetMessageHandlerGameObject::ResurectFailureAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_USER_LEVEL_UPDATED, this, &FGsNetMessageHandlerGameObject::UserLevelUpdatedNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_CREATURE_COMBAT_STATUS, this, &FGsNetMessageHandlerGameObject::CreatureCombatStatusNotify
	));

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_UPSERT_ABNORMALITY, this, &FGsNetMessageHandlerGameObject::UpsertAbnormalityNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_REMOVE_ABNORMALITY, this, &FGsNetMessageHandlerGameObject::RemoveAbnormalityNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_FAILED_ADD_ABNORMALITY, this, &FGsNetMessageHandlerGameObject::FailedAddAbnormalityAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ABNORMALITY, this, &FGsNetMessageHandlerGameObject::SetAbnormalityNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ADD_ABNORMALITY_RESULT, this, &FGsNetMessageHandlerGameObject::AbnormalityResultNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_PASSIVITY_ADDED, this, &FGsNetMessageHandlerGameObject::PassivityAddedNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_PASSIVITY_REMOVED, this, &FGsNetMessageHandlerGameObject::PassivityRemovedNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_WEIGHT_INFO, this, &FGsNetMessageHandlerGameObject::UserWeightInfoNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FAILURE,
		this, &FGsNetMessageHandlerGameObject::ChangeWeaponTypeFailureAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_START,
		this, &FGsNetMessageHandlerGameObject::ChangeWeaponTypeStartNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED,
		this, &FGsNetMessageHandlerGameObject::ChangeWeaponTypeFinishedNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_ENTER_TERRITORY, this, &FGsNetMessageHandlerGameObject::EnterTerritoryAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_LEAVE_TERRITORY, this, &FGsNetMessageHandlerGameObject::LeaveTerritoryAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_SPAWN_DROP_OBJECT,
		this, &FGsNetMessageHandlerGameObject::SpawnDropObject)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DESPAWN_DROP_OBJECT,
		this, &FGsNetMessageHandlerGameObject::DespawnDropObject)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_DROP_OBJECT_OWNERSHIP_EXPIRED,
		this, &FGsNetMessageHandlerGameObject::DropObjectOwnershipExpired)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_DROP_OBJECT_LOOT_FAILURE,
		this, &FGsNetMessageHandlerGameObject::LootDropObjectFail)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_SKILL_SLOT_CLEAR, this, &FGsNetMessageHandlerGameObject::SkillSlotClearAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_SKILL_SLOT_SET, this, &FGsNetMessageHandlerGameObject::SkillSlotSetAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_SKILL_SLOT_AUTO, this, &FGsNetMessageHandlerGameObject::SkillSlotAutoAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SKILL_LEARN, this, &FGsNetMessageHandlerGameObject::SkillLearn)
	);
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_UPDATE_ACTIVE_SKILL, this, &FGsNetMessageHandlerGameObject::SkillUpdateActiveAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_SKILL_ENCHANT, this, &FGsNetMessageHandlerGameObject::SkillEnchantAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SKILL_BONUS_LEVEL, this, &FGsNetMessageHandlerGameObject::SkillBonusLevelNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_COSTUME_SKILL_LEARN, this, &FGsNetMessageHandlerGameObject::CostumeSkillLearnAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_COSTUME_SKILL_SLOT_SET, this, &FGsNetMessageHandlerGameObject::CostumeSkillSlotSetAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_CHANGE_COSTUME, this, &FGsNetMessageHandlerGameObject::AckExchangeCostume)
	);
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_USE_SKILL_INTERVAL, this, &FGsNetMessageHandlerGameObject::SkillUseSkillCycleAck)
	);

	//prop
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SPAWN_PROP, this, &FGsNetMessageHandlerGameObject::SpawnPropAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_DESPAWN_PROP, this, &FGsNetMessageHandlerGameObject::DespawnPropAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_UPDATE_PROP, this, &FGsNetMessageHandlerGameObject::UpdatePropAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_INVADE_PORTAL_ACTIVE, this, &FGsNetMessageHandlerGameObject::ActiveInvasionPropAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_CLICK_PROP, this, &FGsNetMessageHandlerGameObject::InteractPropInvasionAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_INTERACT_QUEST_PROP_START, this, &FGsNetMessageHandlerGameObject::InteractQuestPropStartAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_INTERACT_PROP_START, this, &FGsNetMessageHandlerGameObject::InteractPropStartAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_INTERACT_PROP_START, this, &FGsNetMessageHandlerGameObject::InteractPropStartNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_INTERACT_QUEST_PROP_END, this, &FGsNetMessageHandlerGameObject::InteractQuestPropEndAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_INTERACT_PROP_END, this, &FGsNetMessageHandlerGameObject::InteractPropEndAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_INTERACT_PROP_END, this, &FGsNetMessageHandlerGameObject::InteractPropEndNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_INTERACT_PROP_CANCEL, this, &FGsNetMessageHandlerGameObject::InteractPropCancelAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_INTERACT_PROP_CANCEL, this, &FGsNetMessageHandlerGameObject::InteractPropCancelNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_PASSIVITY_INVOKED, this, &FGsNetMessageHandlerGameObject::PassivityInvokeNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_SOCIAL_ACTION, this, &FGsNetMessageHandlerGameObject::SocialActionNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_ACK_USER_SOCIAL_EMOTION, this, &FGsNetMessageHandlerGameObject::SocialEmotionNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_MOVE_STIFFEN, this, &FGsNetMessageHandlerGameObject::MoveStiffenNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SKILL_MOVE_LOCATION, this, &FGsNetMessageHandlerGameObject::SkillMoveLocation)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_PROJECTILE_MOVE_TO_LOCATION,
		this, &FGsNetMessageHandlerGameObject::ProjectileMoveToLocationNotify)
	);
	
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_CONTRIBUTION_RANK_LIST,
		this, &FGsNetMessageHandlerGameObject::ContributionRankList)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_AGGRO_TARGET,
		this, &FGsNetMessageHandlerGameObject::SetAggroTarget)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_GUILD_CAMP_CONSTRUCTION_STEP,
		this, &FGsNetMessageHandlerGameObject::CampBuildStepNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_SPAWN_CAMP,
		this, &FGsNetMessageHandlerGameObject::SpawnCampAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_CANCEL_NPC_INTERACT,
		this, &FGsNetMessageHandlerGameObject::CancelNpcInteractNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_GUILD_CAMP_DESTRUCTION,
		this, &FGsNetMessageHandlerGameObject::DespawnCampAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_GUILD_CAMP_DESPAWN,
		this, &FGsNetMessageHandlerGameObject::DespawnCampNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_GUILD_CAMP_INSTALLATION,
		this, &FGsNetMessageHandlerGameObject::AllowedBuildCampNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_USER_NPC_INTERACT_STATE,
		this, &FGsNetMessageHandlerGameObject::UserNpcInteractStateNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_GUILD_CAMP_CONSTRUCTION_COMPLETED,
		this, &FGsNetMessageHandlerGameObject::CampConstructionCompletedNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_IFF_DEBUG,
		this, &FGsNetMessageHandlerGameObject::IffDecugAck)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_SPAWN_CAMP_NAME,
		this, &FGsNetMessageHandlerGameObject::CampNameNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_GUILD_MEMBER_CAMP_NOTIFY,
		this, &FGsNetMessageHandlerGameObject::GuildMemberCampNotify)
	);

	// 성소
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_SANCTUM_NEXUS_INSTALL,
		this, &FGsNetMessageHandlerGameObject::SanctumNexusInstallAck)
	);
	//AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ACK_MAP_SANCTUM_INFO,
	//	this, &FGsNetMessageHandlerGameObject::SanctumMapInfoAck)
//	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_SANCTUM_STATUS,
		this, &FGsNetMessageHandlerGameObject::SanctumStatusNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_SANCTUM_NEXUS_STATUS,
		this, &FGsNetMessageHandlerGameObject::SanctumNexusStatusNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_SANCTUM_NEXUS_CONSTRUCTION_STEP,
		this, &FGsNetMessageHandlerGameObject::SanctumNexusConstructionStepNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_SANCTUM_ALERT_BATTLE_START_FIRST,
		this, &FGsNetMessageHandlerGameObject::SanctumBattlePrepareFirstNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_SANCTUM_ALERT_BATTLE_START_FINAL,
		this, &FGsNetMessageHandlerGameObject::SanctumBattlePrepareFinalNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_SANCTUM_ALERT_BATTLE_START,
		this, &FGsNetMessageHandlerGameObject::SanctumBattleStartNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_SANCTUM_ALERT_BATTLE_END,
		this, &FGsNetMessageHandlerGameObject::SanctumBattleEndNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_SANCTUM_BATTLE_VICTORY,
		this, &FGsNetMessageHandlerGameObject::SanctumBattleVictoryNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_GUILD_MEMBER_SANCTUM_NEXUS_NOTIFY,
		this, &FGsNetMessageHandlerGameObject::SanctumGuildMemberNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_SANCTUM_NEXUS_EMBLEM_CHANGE,
		this, &FGsNetMessageHandlerGameObject::SanctumNexusEmblemChangeNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_BEHAVIOR_STATUS,
		this, &FGsNetMessageHandlerGameObject::BehaviorStatusAck)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_PC_COLLISION_ON,
		this, &FGsNetMessageHandlerGameObject::LocalPlayerCollisionOn)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_PC_COLLISION_OFF,
		this, &FGsNetMessageHandlerGameObject::LocalPlayerCollisionOff)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_COGNITION_TARGET,
		this, &FGsNetMessageHandlerGameObject::CognitionTargetNotify)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_AUTO_BATTLE_RANGE_NOLIMIT,
		this, &FGsNetMessageHandlerGameObject::AutoBattleRangeNoLimit)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_REQ_WARP_WITH_DESPAWN_READY,
		this, &FGsNetMessageHandlerGameObject::WarpWithDespawnReady)
	);

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ELITE_DUNGEON_PORTAL_HIDDEN_SPAWN,
		this, &FGsNetMessageHandlerGameObject::EliteDungeonPortalHiddenSpawnNotify)
	);
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_ELITE_DUNGEON_PORTAL_HIDDEN_DESPAWN,
		this, &FGsNetMessageHandlerGameObject::EliteDungeonPortalHiddenDespawnNotify)
	);
}

void FGsNetMessageHandlerGameObject::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerGameObject::SetLocalPlayerCollsionFlag(bool inFlag)
{
	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		if (UGsGameObjectBase* creature = mgr->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (UGsGameObjectLocalPlayer* local = creature->CastGameObject<UGsGameObjectLocalPlayer>())
			{
				local->SetServerCollisionRule(inFlag);
				local->UpdateCollisionPreset();
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SpawnNpcAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SPAWN_NPC_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SPAWN_NPC_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	UGsGameObjectBase* spawnNpc = SpawnNpc(FGsSpawnParam(pkt)
		, pkt->Reason()
		, pkt->IsAlive()
		, pkt->IsCombat()
		, pkt->TerritoryType()
		, pkt->NpcSpawnId()
		, NpcSpecialStatusType::NONE
		, pkt->IsOffensive()
		, pkt->SkipTerrainCheck());

}

UGsGameObjectBase* FGsNetMessageHandlerGameObject::SpawnNpc(const FGsSpawnParam& inSpawnParam, SPAWN_REASON inReason, bool inIsAlive, bool inIsCombat,
	TerritoryType inTerritoryType, int64 inSpawnId, NpcSpecialStatusType inSpecialStatus, bool inIsOffensive, bool inIsSkipTerrainCheck)
{

#if WITH_EDITOR
	FString gameObjectType = FGsTextUtil::GetStringFromEnum<EGsGameObjectType>(TEXT("EGsGameObjectType"), inSpawnParam._type);
	GSLOG(Log, TEXT("Spawn npc - game object type : %s"), *gameObjectType);
#endif

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		UGsGameObjectBase* gameObject = mgr->SpawnObject(&inSpawnParam);
		if (nullptr == gameObject)
		{
			FString log = TEXT("Npc shape missing - id : ") + FString::FromInt(inSpawnParam._tId);
			GSLOG(Error, TEXT("%s"), *log);
			return nullptr;
		}

		//if (FGsMovementHandlerBase* moveHandler = gameObject->GetMovementHandler())
		//{
		//	//이미 스폰시 높이 체크를 한다. 이미 캐릭터가 서있는 상태에서 다시 teleport를 하면 높이가 살짝 튕기는 현상이 있다. 
		//	moveHandler->Reset(FGsDir::RotatorToAngle(inSpawnParam._rot), inSpawnParam._pos);
		//}

		UGsGameObjectNonPlayerBase* npc = gameObject->CastGameObject<UGsGameObjectNonPlayerBase>();
		if (nullptr == npc)
		{
			return nullptr;
		}

		npc->SetOffensive(inIsOffensive);


		if (npc->GetObjectType() == EGsGameObjectType::Camp)
		{
			if (FGsCampManager* campMgr = GSCamp())
			{
				UGsGameObjectCamp* campObj = gameObject->CastGameObject<UGsGameObjectCamp>();
				campMgr->SpawnBuildCamp(campObj, inIsAlive);
			}
		}
		else if (npc->GetObjectType() == EGsGameObjectType::NonPlayer)
		{
			if (FGsGameObjectStateManager* fsm = npc->GetBaseFSM())
			{
				if (false == inIsAlive)
				{
					fsm->ProcessEvent(EGsStateBase::Dying);
				}
			}

		}
		else if (npc->GetObjectType() == EGsGameObjectType::Sandbag)
		{
			if (UGsGameObjectSandbag* sandbag = Cast<UGsGameObjectSandbag>(npc))
			{
				if (false == inIsAlive)
				{
					sandbag->StartFadeOut();
				}
				else
				{
					if (inReason == SPAWN_REASON::NORMAL)
					{
						sandbag->StartFadeIn();
					}

					if (npc->IsQuestTarget())
					{
						npc->SetQuestTargetActive(true);
					}
				}
			}

			const FGsSchemaSpawnElement* spawnData = mgr->FindVisibleSpawnInfo(EGsGameObjectType::Sandbag, inSpawnId);
			// 샌드백 퀘스트 진행도에 따른 visible 처리		
			if (spawnData != nullptr)
			{
				npc->SetVisibleInfo(spawnData);
			}
		}

		if (NpcSpecialStatusType::NONE != inSpecialStatus)
		{
			if (FGsGameObjectStateManager* fsm = npc->GetBaseFSM())
			{
				EGsStateBase state = UGsGameObjectUtil::GetStatusType(inSpecialStatus);
				fsm->ProcessEvent(state);
			}
		}

		// 전투, 평화 상태 동기화
		FGsGameObjectDataCreature* creatureData = npc->GetCastData<FGsGameObjectDataCreature>();
		creatureData->SetUserStatus(inIsCombat ? UserStatus::USER_STATE_COMBAT : UserStatus::USER_STATE_NORMAL);
		// Territory Type
		creatureData->SetTerritoryType(inTerritoryType);
		creatureData->SetSkipTerrainCheck(inIsSkipTerrainCheck);

#if	!UE_BUILD_SHIPPING
		FString reason = FGsTextUtil::GetEnumValueAsString<SPAWN_REASON>(TEXT("SPAWN_REASON"), inReason);
		if (AGsCharacterBase* castActer = Cast<AGsCharacterBase>(npc->GetActor()))
		{
			castActer->_eventLog.Emplace(FString::Printf(TEXT("PKT_SC_SPAWN_NPC_READ - uniqueid : %lld \tid : %d \tpos : %s\t reason : %s")
				, inSpawnParam._gameId, inSpawnParam._tId, *inSpawnParam._pos.ToString(), *reason));
		}
#endif	
		return gameObject;
	}
	return nullptr;
}

void FGsNetMessageHandlerGameObject::DespawnNpcAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_DESPAWN_NPC_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_DESPAWN_NPC_READ*>(inPacket.Buffer))
	{
		if (NPCPoolHandle.Find(pkt->GameId()))
		{
			TSharedPtr<FStreamableHandle>* handle = NPCPoolHandle.Find(pkt->GameId());
			if (handle->IsValid())
			{
				handle->Get()->CancelHandle();
				NPCPoolHandle.Remove(pkt->GameId());
			}

		}
		/*NPCPoolData.Remove(pkt->GameId());*/


#if	!UE_BUILD_SHIPPING
		FString reason = FGsTextUtil::GetEnumValueAsString<SPAWN_REASON>(TEXT("SPAWN_REASON"), pkt->Reason());//SC_SPAWN_TREASURE_NPC
		if (auto npc = Cast<UGsGameObjectNonPlayer>(GSGameObject()->FindObject(EGsGameObjectType::NonPlayer, pkt->GameId())))
		{
			if (AGsCharacterBase* castActer = Cast<AGsCharacterBase>(npc->GetActor()))
			{
				castActer->_eventLog.Emplace(FString::Printf(TEXT("PKT_SC_DESPAWN_NPC_READ - uniqueid : %lld reason : %s"), pkt->GameId(), *reason));
			}
		}
#endif	

		DespawnNpc(pkt->GameId(), pkt->Reason());
	}
}

void FGsNetMessageHandlerGameObject::SpawnSpecialNpc(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SPAWN_SPECIAL_NPC_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SPAWN_SPECIAL_NPC_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	UGsGameObjectBase* spawnNpc = nullptr;
	spawnNpc = SpawnNpc(FGsSpawnParam(pkt)
		, pkt->Reason()
		, pkt->IsAlive()
		, pkt->IsCombat()
		, pkt->TerritoryType()
		, pkt->NpcSpawnId()
		, pkt->SpecialStatusType());
}

void FGsNetMessageHandlerGameObject::NpcChangeSpecialStataus(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_NPC_CHANGE_SPECIAL_STATUS_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_NPC_CHANGE_SPECIAL_STATUS_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		int64 gameId = pkt->GameId();
		UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::NonPlayerBase, gameId);
		if (nullptr == findObject)
		{
			return;
		}

		UGsGameObjectNonPlayerBase* npc = Cast<UGsGameObjectNonPlayerBase>(findObject);
		if (npc)
		{
			FGsGameObjectStateManager* stateManager = npc->GetBaseFSM();
			if (stateManager)
			{
				EGsStateBase statusType = UGsGameObjectUtil::GetStatusType(pkt->SpecialStatusType());
				stateManager->ProcessEvent(statusType);
			}

			npc->_specialType = pkt->SpecialType();
		}
	}
}

// 보물몬스터 스폰
void FGsNetMessageHandlerGameObject::SpawnTreasureNpc(const FGsNet::Packet& inPacket)
{
	auto* pkt = reinterpret_cast<PD::SC::PKT_SC_SPAWN_TREASURE_NPC_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		UGsGameObjectBase* spawnNpc = nullptr;
		spawnNpc = SpawnNpc(FGsSpawnParam(pkt)
			, pkt->Reason()
			, pkt->IsAlive()
			, pkt->IsCombat()
			, pkt->TerritoryType()
			, pkt->NpcSpawnId()
			, NpcSpecialStatusType::NONE);



		const FGsNetUserData* userData = GGameData()->GetUserData();
		uint64 ownerGameId = userData->mUserDBId == pkt->UserDBId() ? userData->mGameId : 0;
		if (ownerGameId > 0)
		{
			if (auto localPlayer = Cast<UGsGameObjectLocalPlayer>(mgr->FindObject(EGsGameObjectType::LocalPlayer)))
			{
				localPlayer->SetOwnTreasureMonsterId(pkt->GameId());
			}
		}

		if (auto npc = Cast<UGsGameObjectNonPlayer>(mgr->FindObject(EGsGameObjectType::NonPlayer, pkt->GameId())))
		{
			npc->SetWidgetTreasureMonsterInfo(pkt->UserName(), pkt->GuardRemainingTime(), pkt->GuardTime(),
				ownerGameId, pkt->GuildDBId(), pkt->UserDBId());
		}
	}
}

//보물 몬스터 디스폰 - 로컬플레이어의 보물몬스터만.
void FGsNetMessageHandlerGameObject::DespawnTreasureNpc(const FGsNet::Packet& inPacket)
{
	auto* pkt = reinterpret_cast<PD::SC::PKT_SC_DESPAWN_TREASURE_NPC_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	auto treasureMonsterGameId = pkt->GameId();
	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		//if (auto npc = Cast<UGsGameObjectNonPlayer>(GSGameObject()->FindObject(EGsGameObjectType::NonPlayer, treasureMonsterGameId)))
		if (auto localPlayer = Cast<UGsGameObjectLocalPlayer>(mgr->FindObject(EGsGameObjectType::LocalPlayer)))
		{
			auto localOwnMonsterGameId = localPlayer->GetOwnTreasureMonsterId();
			if (localOwnMonsterGameId == treasureMonsterGameId)
				localPlayer->SetOwnTreasureMonsterId(0);
		}
	}

	GSWorldMap()->RemoveTreasureMonsterMapInfo(treasureMonsterGameId);
}

//보물몬스터 주인 길드변경- 팀몬스터 인지 적몬스터인지
void FGsNetMessageHandlerGameObject::UpdateTreasureNpcGuild(const FGsNet::Packet& inPacket)
{
	auto* pkt = reinterpret_cast<PD::SC::PKT_SC_TREASURE_NPC_UPDATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		int64 gameId = pkt->GameId();
		UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::NonPlayer, gameId);
		if (nullptr == findObject)
		{
			return;
		}

		if (auto npc = Cast<UGsGameObjectNonPlayer>(findObject))
		{
			npc->ChangeWidgetTreasureMonsterGuildInfo(pkt->GuildDBId());
		}
	}
}

//내 보물 몬스터와 파티 보물 몬스터의 위치정보
void FGsNetMessageHandlerGameObject::UpdateTreasureNpcPosition(const FGsNet::Packet& inPacket)
{
	auto* pkt = reinterpret_cast<PD::SC::PKT_SC_USER_TREASURE_NPC_POSITION_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (auto worldMapManager = GSWorldMap())
	{
		worldMapManager->SetTreasureMonsterMapInfo(pkt->GameId(), pkt->MapId(), pkt->Pos(), pkt->IsPartyMember() == false);
	}

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		if (auto npc = Cast<UGsGameObjectNonPlayer>(mgr->FindObject(EGsGameObjectType::NonPlayer, pkt->GameId())))
		{
			npc->SetValidCheckPartyMonster();
		}
	}
}

void FGsNetMessageHandlerGameObject::DespawnNpc(int64 inGameId, SPAWN_REASON inReason)
{
	int64 gameId = inGameId;

	FString reason = FGsTextUtil::GetEnumValueAsString<SPAWN_REASON>(TEXT("SPAWN_REASON"), inReason);//SC_SPAWN_TREASURE_NPC

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::NonPlayerBase, gameId);
		if (nullptr == findObject)
		{
			GSLOG(Error, TEXT("PKT_SC_DESPAWN_NPC_READ - game id : %lld \treason : %s"), gameId, *reason);
			return;
		}

		GSAI()->RemoveMannerCheckTarget(findObject->GetGameId());

		FGsAIReserveParam aiReserveClearParam(EGsAIActionType::ATTACK_ANYONE, findObject);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_JOB, &aiReserveClearParam);

		mgr->DespawnObject(findObject);
	}
}

void FGsNetMessageHandlerGameObject::DespawnNpcObject(UGsGameObjectBase* In_obj)
{
	if (In_obj == nullptr)
	{
		return;
	}

	GSAI()->RemoveMannerCheckTarget(In_obj->GetGameId());

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		mgr->DespawnObject(In_obj);
	}
}

void FGsNetMessageHandlerGameObject::SpawnMeAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_SPAWN_ME_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SPAWN_ME_READ*>(inPacket.Buffer))
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("====== [SpawnMeAck] ID : SC_SPAWN_ME GameId : %lld ======"), pkt->GameId());
#endif
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			UGsGameObjectBase* local = nullptr;
			FGsGameDataManager* gameDataMgr = GGameData();
			check(gameDataMgr);

			// 현재 채널 저장
			gameDataMgr->SetCurrentChannel(pkt->ChannelId());
			gameDataMgr->Set(*pkt);

			//침공전 방어 생태 초기화
			gameDataMgr->SetInvasionDefenseState(false);

			// 리소스 매니져 Clear
			//GResource()->ClearResource();

			//bak1210 : 같은 타이임에 스폰처리
			mgr->PreSpawnMe(gameDataMgr->GetSpawnLocation(), gameDataMgr->GetSpawnRotation());

			local = mgr->FindObject(EGsGameObjectType::LocalPlayer, pkt->GameId());
			if (nullptr != local)
			{
				if (UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>())
				{
					// 기본 커마 설정
					castLocal->SetCustomizeDataBonScale(gameDataMgr->GetUserData()->mFaceData, gameDataMgr->GetUserData()->mBodyData);
					// 장비 관련 처리
					castLocal->OnUserBasicDataChange(gameDataMgr->GetUserData()->mLookInfo);
					castLocal->UpdateCostumeEquip(gameDataMgr->GetUserData()->mLookInfo.mTransformCostumeId, gameDataMgr->GetUserData()->mLookInfo.mCostumeId, gameDataMgr->GetUserData()->mLookInfo.mViewHelmet);
					castLocal->UpdateFairyEquip(gameDataMgr->GetUserData()->mLookInfo.mFairy);
					castLocal->SetServerCollisionRule(pkt->PcCollision());

					// 방향
					/*castLocal->GetLocalCharacter()->SetActorRotation(FGsDir::AngleToRotator(pkt->Dir()));*/

					// 이동금지, 회전금지
					if (auto localChar = castLocal->GetLocalCharacter())
					{
						if (AGsPlayerController* playerController = localChar->GetPlayerController())
						{
							playerController->SetIgnoreMoveInput(true);
							playerController->SetIgnoreLookInput(true);
						}
						castLocal->UpdateCollisionPreset();
					}
					
					castLocal->OnChangeAggroTarget();
					castLocal->OnChangeContribution();

					FGsGameObjectData* gameObjectData = local->GetData();
					gameObjectData->SetTerritoryType(pkt->TerritoryType());

					// 로딩중 길드 추방되는 상황 대응
					if (INVALID_GUILD_DB_ID == pkt->GuildDBId())
					{
						// 길드정보가 남아있을 경우 제거
						if (FGsGuildManager* guildMgr = GGuild())
						{
							if (guildMgr->IsGuild())
							{
								guildMgr->RemoveAllGuildData();

								// 캐릭터 네임플레이트 업데이트
								castLocal->UpdateBillboard(EGsBillboardUpdateType::Guild);
							}
						}
					}
				}
			}

			// 부활 창이 존재하면 닫아줌
			//GUI()->CloseByKeyName(TEXT("PopupRevive"));
#ifdef NAVIMESH_TEST_LOCATION_LOG
			if (UGsCheatManager::_IsShowNaviMeshInfo)
			{
				if (UGsGameObjectBase* localBase = mgr->FindObject(EGsGameObjectType::LocalPlayer))
				{
					DrawDebugCircle(localBase->GetWorld(), pkt->ConstRefPos(), 80.0f, 100, FColor::Red, false, 10.0f, 0, 5.0f);
					GSLOG(Error, TEXT("NAVIMESH_TEST_LOCATION_LOG : POS [%0.f / %0.f / %0.f ]"), pkt->ConstRefPos().X, pkt->ConstRefPos().Y, pkt->ConstRefPos().Z);
				}
			}
#endif	// NAVIMESH_TEST_LOCATION_LOG
		}
	}
}

void FGsNetMessageHandlerGameObject::SpawnMeCompleteAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SPAWN_ME_COMPLETE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SPAWN_ME_COMPLETE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
#if !UE_BUILD_SHIPPING
		FString errorMsg = FString::Printf(TEXT("[SpawnMeCompleteAck] No SpawnMeCompleteAck \n"));
		GEngine->AddOnScreenDebugMessage(42, 5.f, FColor::Red, errorMsg);
#endif
		return;
	}

	bool isReconnect = pkt->IsReconnect();
	GSLOG(Log, TEXT("====== [SpawnMeCompleteAck] ID : SC_SPAWN_ME_COMPLETE reconnect : %d ======"), isReconnect);	

	UGsGameObjectManager* mgr = GSGameObject();
	if (nullptr == mgr)
	{
#if !UE_BUILD_SHIPPING
		FString errorMsg = FString::Printf(TEXT("[SpawnMeCompleteAck] No GSGameObject \n"));
		GEngine->AddOnScreenDebugMessage(42, 5.f, FColor::Red, errorMsg);
#endif
		return;
	}

	UGsGameObjectBase* local = mgr->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == local)
	{
#if !UE_BUILD_SHIPPING
		FString errorMsg = FString::Printf(TEXT("[SpawnMeCompleteAck] No GameObjectBase \n"));
		GEngine->AddOnScreenDebugMessage(42, 5.f, FColor::Red, errorMsg);
#endif
		return;
	}

	UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();
	if (nullptr == castLocal)
	{
#if !UE_BUILD_SHIPPING
		FString errorMsg = FString::Printf(TEXT("[SpawnMeCompleteAck] No GameObjectLocalPlayer \n"));
		GEngine->AddOnScreenDebugMessage(42, 5.f, FColor::Red, errorMsg);
#endif
		return;
	}

	AGsPlayerController* playerController = nullptr;
	if (AGsCharacterLocalPlayer* localPlayer = castLocal->GetLocalCharacter())
	{
		playerController = localPlayer->GetPlayerController();
		if (playerController)
		{
			playerController->SetIgnoreMoveInput(false);
			playerController->SetIgnoreLookInput(false);
			GSClientSpawn()->InitializeClientSpawnGameObject(localPlayer);
		}
	}

	//Start update fence handler
	if (FGsFenceHandler* fence = castLocal->GetFenceHandler())
	{
		fence->SetActive(true);
	}

	//클라이언트 스폰처리를 위한 초기화	
	GSClientSpawn()->SetActive(true);

	// 클라이언트 스폰 컬링 업데이트 및 스폰 강제호출 1회(퀘스트 대화 대상 이슈)
	GSClientSpawn()->ForcedUpdate();

	// 죽어 있는 상태면 상태 변경 처리
	if (FGsGameObjectStateManager* fsm = castLocal->GetBaseFSM())
	{
		FGsGameDataManager* gameDataMgr = GGameData();
		check(gameDataMgr);


		// [B1] | ejrrb10 | 로비 -> 인게임, 인스턴스 필드 등, 스폰 연출을 할지 안할지 체크(서버에서 분기)
		if (const UGsTableSkillCommon* skillCommonTable = Cast<UGsTableSkillCommon>(FGsSchemaSkillCommon::GetStaticTable()))
		{
			const FGsSchemaSkillCommon* skillCommonData = nullptr;
			if (skillCommonTable->FindRowById(pkt->CommonActionId(), skillCommonData))
			{
				if (CommonActionType::SPAWN == skillCommonData->category)
				{
					castLocal->OnSpawn(gameDataMgr->IsSpawnAlive());

					// [B1] | ejrrb10 | 일반적인 스폰일 때만 페어리 스폰 연출을 타 플레이어에게 노출
					castLocal->OnSpawnFairyEffect();
				}
				else
				{
					castLocal->OnSpawn(gameDataMgr->IsSpawnAlive(), false);
				}
			}
			else
			{
				// [B1] | ejrrb10 | 로비에서 인게임 진입 시 스폰 효과 처리
				// -> 로비에서도 CommonActionId 가 0으로 잘 잡히고 있다
				//castLocal->OnSpawn(gameDataMgr->IsSpawnAlive());
				//castLocal->OnSpawnFairyEffect();
				castLocal->OnSpawn(gameDataMgr->IsSpawnAlive(), false);
			}
		}
	}

	//Load DesignLevel
	if (nullptr != playerController)
	{
		AGsGameModeWorldBase* gameMode = Cast<AGsGameModeWorldBase>(UGameplayStatics::GetGameMode(playerController->GetWorld()));
		if (gameMode)
		{
			gameMode->UnloadArtStreamingLevel();
			gameMode->ClearArtSteamingLevel();
			gameMode->TryLoadArtStreamingLevel();
		}
	}

	//bak1210
	//! 중요 ! 
	/*클라이언트는 [워프 요청] ~[워프 성공 or 실패] 까지 패킷 전송을 하지 않도록 수정합니다
		- 워프 또는 워프 치트 요청 시, “패킷 전송 불가능 상태” 설정.
		- “SC_ACK_WARP_FAILURE” 또는 “SC_SPAWN_ME_COMPLETE” 수신 시, “패킷 전송 불가능 상태” 해제.*/
	if (GNet())
	{
		GNet()->ClearPacketSendBlock();

		TSharedPtr<FGsNetBase> active = GNet()->GetActive();
		//지금 부터 SpawnMe가 올때까진 Send상황 옵저빙 해지
		if (active.IsValid())
		{
			active->SetObserveSend(false);
			//active->SetReconnectAble(true);
		}
	}
	int32 currentMapId = GLevel()->GetCurrentLevelId();
	FGsPrimitiveInt32 paramMapId(currentMapId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, &paramMapId);

	// 로딩창 끄는시점을 늦추자
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->HideLoading();
	}

#if REVIVE_RECONNECT_TEST	
	castLocal->PrintStateType();
	if (GGameData()->IsSpawnAlive())
	{
		GUI()->ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
	}
	else
	{
		//SpawnMeComplete에 이미 플레이어가 죽은 상태라면 재연결이거나 죽은 상태로 로비로 갔다가 돌아온 경우임
		//따라서 부활창을 보여줘야한다
		GMessage()->GetUI().SendMessage(MessageUI::SHOW_REVIVE, nullptr);
	}
#endif	

	// 혹여나 blockUI 가 켜져있는 상황이라면 끔
	//FGsUIHelper::HideBlockUI();

	// 파티 내채널정보 갱신
	int currentChannel = GGameData()->GetCurrentChannel();
	GSParty()->OnChangedMapIdAndChannel(castLocal->GetGameId(),
		currentMapId, currentChannel);

	mgr->SetIsSpawnMeComplete(true);

	// 정령탄 정보 갱신
	// GGameData의 on,off 정보와
	// 아이템 있는지 정보를 합쳐서 세팅
	GItem()->SetSpiritShotStateByUIState(
		GGameData()->GetUserData()->mSpiritShotOnOff);

	// 혹여나 blockUI 가 켜져있는 상황이라면 끔
	FGsUIHelper::ForcedHideBlockUI(EGsHideBlockUIFactor::SPAWN_ME_COMPLETE);

	// 현재 맵이 시공의 틈새이면 카메라 버튼처리를 바꾼다		
	if (const FGsSchemaMapData* map = GLevel()->GetCurrentMapInfo())
	{
		FGsUIMsgParamBool param;
		if (map->mapType == MapType::INSTANCE_SINGLE &&
			map->ContentsType == MapContentsType::SPACE_CRACK)
		{
			param._data = false;
		}
		else
		{
			param._data = true;
		}

		GMessage()->GetUI().SendMessage(MessageUI::ENABLE_CAMERA_BUTTON, &param);
	}

	if (AGsCharacterLocalPlayer* localPlayer = castLocal->GetLocalCharacter())
	{
#if SPAWN_LOCATION_DEBUG
		UGsLevelManager::SetIsPlayerSpawnComplete(true);
#endif

#if PLAYER_SPAWN_DEBUG			
		UGsLevelUtil::DrawDebugLocation(localPlayer->GetWorld(), localPlayer->GetActorLocation(), localPlayer, TEXT("Spawn me complete"), FColor::Yellow);
#endif
	}

	// [B1] | ejrrb10 | 같은 레벨을 로드하는 경우, CS_REQ_SPAWN_ME_ACTION 을 바로 전송한다
	if (UGsLevelManager* levelMgr = GLevel())
	{
		if (false == levelMgr->GetChangedLevelFlag())
		{
			FGsNetSendServiceWorld::SendReqSpawnMeAction();
		}
	}

	WarpReason warpReason = GLevel() ? GLevel()->GetWarpReason() : WarpReason::INVALID;
	//침공전 맵이고 침공한 경우 출력
	if (warpReason == WarpReason::INVADE)
	{
		if (GGameData()->IsInvadeWorld())
		{
			if (UGsEventManager* eventManager = GSEvent())
			{
				eventManager->EventActionPlayImmediate(INVADE_ENTER_MESSAGE_ID);
			}
		}
		//침공맵이지만 침공전 시간이 아닐 때		
		else if (GSInterServer()->GetCurrentInvasionMapInfo().IsValid()
			&& GSInterServer()->IsMapInInvasionTime(GLevel()->GetCurrentLevelId()) == false)
		{
			FText findText;
			FText::FindText(MAP_EVENT_TEXT_KEY, INVADE_MAP_DESC_TEXT_KEY, findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		}
	}

	//침공전 퇴장 
	if (WarpReason::INVADE_LEAVE == warpReason
		|| WarpReason::INVADE_USER_TIME_OVER == warpReason
		|| WarpReason::INVADE_TIME_OVER == warpReason
		|| WarpReason::INVADE_USE_SCROLL_LEAVE == warpReason)
	{
		if (UGsEventManager* eventManager = GSEvent())
		{
			eventManager->EventActionPlayImmediate(INVADE_EXIT_MESSAGE_ID);
		}
	}
}
// 나(me) 를 제외한 타유저들에 한하여 내려오는 packet
void FGsNetMessageHandlerGameObject::SpawnUserAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_SPAWN_USER_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SPAWN_USER_READ*>(inPacket.Buffer))
	{
		UGsGameObjectBase* remote = nullptr;
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			/*const FGsSchemaUserData* pcTable = UGsTableUtil::FindRow<FGsSchemaUserData>(pkt->CreatureId());
			if (nullptr == pcTable)
			{
				GSLOG(Error, TEXT("not found UserData Table"));
			}

			const FGsSchemaPlayerShape* pcShapeTable = pcTable->shapeId.GetRow();
			if (nullptr == pcShapeTable)
			{
				GSLOG(Error, TEXT("not found PlayerShape Table"));
				return;
			}

			UClass* bpClass = nullptr;
			if (const FGsSchemaShapeData* shapeData = pcShapeTable->remoteplayerShapeId.GetRow())
			{
				FString path = shapeData->bPClass.ToString();
				path += TEXT("_C");

				bpClass = LoadObject<UClass>(nullptr, *path);
			}*/

			//if (bpClass)
			{
				// remote 채널 갱신	
				// SpawnObject(-내부에서 InitializeSubClass) 에서 아이콘 정보 참조하기전에
				// 미리 넣어야함
				int currentChannel = GGameData()->GetCurrentChannel();
				int currentMapId = GLevel()->GetCurrentLevelId();
				GSParty()->OnChangedMapIdAndChannel(pkt->GameId(),
					currentMapId, currentChannel);

				// GameObject 클래스 객체는 이미 생성 되어 있을수도 있음
				// 이곳에서 동적할당 하는 객체나 Initialize / DeInitialize 페어가 되는 설정은 금지
				// 관련 로직은 GameObject 클래스 내부에서 처리			
				FGsSpawnParamPlayer param = FGsSpawnParamPlayer(pkt);
				remote = mgr->SpawnObject(&param);
				if (nullptr != remote)
				{
					// Remote Player 외형변경
					UGsGameObjectRemotePlayer* playerObject = remote->CastGameObject<UGsGameObjectRemotePlayer>();
					if (nullptr == playerObject)
						return;

					/*if (FGsMovementHandlerBase* moveHandler = playerObject->GetMovementHandler())
					{
						moveHandler->Reset(pkt->Dir(), pkt->ConstRefPos());
					}*/
					// lookInfo 설정
					playerObject->SetCustomizeDataBonScale(pkt->CustomizingFaceData(), pkt->CustomizingBodyData());
					playerObject->SetUserEquipData(pkt->LookInfo());
					playerObject->UpdateCostumeEquip(pkt->LookInfo().mTransformCostumeId, pkt->LookInfo().mCostumeId, pkt->LookInfo().mViewHelmet);
					playerObject->UpdateFairyEquip(pkt->LookInfo().mFairy);

					bool isNormalReason = (SPAWN_REASON)pkt->Reason() == SPAWN_REASON::NORMAL;
					playerObject->OnSpawn(pkt->IsAlive(), isNormalReason);
					playerObject->OnChangeAggroTarget();
					playerObject->OnChangeContribution();

					if (FGsGameObjectDataRemotePlayer* data =
						remote->GetCastData<FGsGameObjectDataRemotePlayer>())
					{

						// Level 정보 설정
						data->SetLevel(pkt->Level());
						// Territory 정보
						data->SetTerritoryType(pkt->TerritoryType());
					}

					// 관전 시스템 처리															
					if (GSObserver()->GetIsObserveStart() == true)
					{
						if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
						{
							if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
							{
								if (FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud))
								{
									if (FGsObserverHandler* observerHandler = hud->GetObserverHandler())
									{
										observerHandler->AddObserveTarget(playerObject);
									}
								}
							}
						}
					}

					// 2022/09/29 PKT - PK Book 컨텐츠를 개발하면서 타유저의 스폰 정보가 필요하여 추가 하였음.
					FGsPrimitivePairUInt64 paramIDs(pkt->UserDBId(), pkt->GameId());
					GMessage()->GetGameObject().SendMessage(MessageGameObject::SPAW_REMOTE_PLAYER, &paramIDs);
				}
			}
		}

#if WITH_EDITOR
		GSLOG(Warning, TEXT("[SpawnUserAck - GameID] : %d"), pkt->GameId());
		GSLOG(Warning, TEXT("[SpawnUserAck - DBID] : %d"), pkt->UserDBId());
#endif
	}
}

void FGsNetMessageHandlerGameObject::SpawnMeAction(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_SPAWN_ME_ACTION_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_SPAWN_ME_ACTION_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_SPAWN_ME_ACTION, nullptr);

#if WITH_EDITOR
	GSLOG(Warning, TEXT("--------------------------------"));
	GSLOG(Warning, TEXT("[SC_ACK_SPAWN_ME_ACTION ]"));
	GSLOG(Warning, TEXT("--------------------------------"));
#endif

	//재접속시 십자가로 있을때 죽은 상태임에도 불구하고 UI노출되는 현상 수정 
	if (UGsGameObjectManager* gameObjectManager = GSGameObject())
	{
		UGsGameObjectBase* local = gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer);
		if (UGsGameObjectLocalPlayer* castLocal = Cast<UGsGameObjectLocalPlayer>(local))
		{
			if (castLocal->IsDeadState())
			{
				if (UGsUIManager* uiManager = GUI())
				{
					uiManager->CloseAllStack();
					uiManager->SetHUDMode(EGsUIHUDMode::NORMAL);
					uiManager->ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
					uiManager->SetHideFlags(EGsUIHideFlags::STATE_DIE);
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::DespawnUserAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_DESPAWN_USER_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_DESPAWN_USER_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			int64 gameId = pkt->GameId();

			// 디스폰 원인
			SPAWN_REASON despawnReason = (SPAWN_REASON)pkt->Reason();
			FString reason = FGsTextUtil::GetEnumValueAsString<SPAWN_REASON>(TEXT("SPAWN_REASON"), despawnReason);

			if (UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::RemotePlayer, gameId))
			{
				UGsGameObjectPlayer* playerObject = findObject->CastGameObject<UGsGameObjectPlayer>();

				playerObject->SetDespawnReason(despawnReason);

				// [B1] | ejrrb10 | 연출 시작 부분을 SC_ACK_SOCIAL_ACTION 부분으로 이동
				if (despawnReason == SPAWN_REASON::NORMAL)
				{
					// 워프 및 종료면 스캔에서 삭제
					if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
					{
						if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
						{
							if (FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud))
							{
								// 스캔 핸들러에서 처리
								if (FGsScanHandler* scanHandler = hud->GetScanHandler())
								{
									scanHandler->RemoveScanMemberInfo(gameId);
								}

								// 관전 시스템에서 처리
								if (GSObserver()->GetIsObserveStart() == true)
								{
									if (FGsObserverHandler* observerHandler = hud->GetObserverHandler())
									{
										observerHandler->RemoveObserveTarget(findObject);
									}
								}
							}
						}
					}

					mgr->StopCounterAttackShow(gameId);
				}

				GSAI()->RemoveMannerTargetOwnerRemote(gameId);

				FGsAIReserveParam aiReserveClearParam(EGsAIActionType::ATTACK_ANYONE, playerObject);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_JOB, &aiReserveClearParam);


#if AGGRO_DEBUG
				UGsGameObjectRemotePlayer* remotePlayer = Cast<UGsGameObjectRemotePlayer>(findObject);
				if (remotePlayer)
				{
					GSLOG(Log, TEXT("AGGRO_DEBUG - FGsNetMessageHandlerGameObject::DespawnUserAck - gmae id : %lld\tid : %s")
						, remotePlayer->GetGameId()
						, *remotePlayer->GetData()->GetName());
				}				
#endif
				mgr->DespawnObject(findObject);


				// 2022/09/29 PKT - PK Book 컨텐츠를 개발하면서 타유저의 스폰 정보가 필요하여 추가 하였음.
				FGsPrimitiveUInt64 paramGameId(pkt->GameId());
				GMessage()->GetGameObject().SendMessage(MessageGameObject::DESPAW_REMOTE_PLAYER, &paramGameId);
			}
			else
			{
#if WITH_EDITOR
				GSLOG(Error, TEXT("PKT_SC_DESPAWN_USER_READ - game id : %d \treason : %s"), gameId, *reason);
#endif
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SpawnProjectileAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_SPAWN_PROJECTILE_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SPAWN_PROJECTILE_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			/*UClass* bpClass = nullptr;
			if (const FGsSchemaProjectileSet* projectileTable =
				UGsTableUtil::FindRowById<UGsTableProjectileSet, FGsSchemaProjectileSet>(pkt->ProjectileId()))
			{
				if (const FGsSchemaShapeData* shapeData = projectileTable->shapeId.GetRow())
				{
					FString path = shapeData->bPClass.ToString();
					path += TEXT("_C");
					bpClass = LoadObject<UClass>(nullptr, *path);
				}
			}

			if (bpClass)*/
			{
				// GameObject 클래스 객체는 이미 생성 되어 있을수도 있음
				// 이곳에서 동적할당 하는 객체나 Initialize / DeInitialize 페어가 되는 설정은 금지
				// 관련 로직은 GameObject 클래스 내부에서 처리
				FGsSpawnParamProjectile param(pkt);
				if (UGsGameObjectBase* projectile = mgr->SpawnObject(&param))
				{
					// Owner 정보를 설정
					if (UGsGameObjectBase* owner = mgr->FindObject(EGsGameObjectType::Creature, pkt->CasterGameId()))
					{
						UGsGameObjectProjectile* castProjectile = projectile->CastGameObject<UGsGameObjectProjectile>();
						castProjectile->SetCaster(owner);
						castProjectile->SetGameObjectActorVisible(pkt->Visible());

						// 이동형 발사체 타입
						if (FGsMovementHandlerBase* movemHandler = projectile->GetMovementHandler())
						{
							FGsMovementHandlerProjectile* castMoveHandler =
								FGsClassUtil::FCheckStaticCast<FGsMovementHandlerProjectile>(movemHandler);

							castMoveHandler->ReceveDesPos(pkt->ConstRefDestPos());
						}
					}
					// 1. mDestPos 에 대한 체크가 필요 하다면 내부적으로 처리 (UGsGameObjectProjectile)
					//    현재는 클라이언트에서도 충돌 처리가 있기때문에 필요없을것으로 판단

					// 디버깅용 Shape 출력
					if (mgr->_isCollisionDebugShape)
					{
						DrawDebugDirectionalArrow(mgr->GetWorld(), pkt->SpawnPos(), pkt->DestPos(),
							100.f, FColor::Black, false, 5.f);
						DrawDebugSphere(mgr->GetWorld(), pkt->SpawnPos(), 20.f, 100.f, FColor::Yellow, false, 5.f);
					}
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::DespawnProjectileAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_DESPAWN_PROJECTILE_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_DESPAWN_PROJECTILE_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			int64 gameId = pkt->GameId();
			FString reason = FGsTextUtil::GetEnumValueAsString<SPAWN_REASON>(TEXT("SPAWN_REASON"), (SPAWN_REASON)pkt->Reason());
			if (UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::Projectile, gameId))
			{
				if (AActor* actor = findObject->GetActor())
				{
					AGsActorProjectile* castActor = Cast<AGsActorProjectile>(actor);
					castActor->_isNetDespawn = true;
				}

				mgr->DespawnObject(findObject);
			}
			else
			{
#if WITH_EDITOR
				GSLOG(Error, TEXT("PKT_SC_DESPAWN_PROJECTILE_READ - game id : %d \treason : %s"), gameId, *reason);
#endif
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SkillAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SKILL_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SKILL_READ*>(inPacket.Buffer);

	GGameData()->Set(*pkt);

	GSkill()->NetRecvUserSkillSet(pkt);

}

void FGsNetMessageHandlerGameObject::StatLoadTopo(const FGsNet::Packet& inPacket)
{
	// TODO : 이 패킷을 받으면 해당 맵을 로딩
	// 로딩이 완료되며 C_ACK_LOAD_TOPO_FIN 전송
	PD::SC::PKT_SC_LOAD_TOPO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_LOAD_TOPO_READ*>(inPacket.Buffer);

	int32 mapId = pkt->MapId();
	int32 reason = pkt->WarpReason();
	const FVector pos = pkt->ConstRefPos();

	//#if WITH_EDITOR
	GSLOG(Error, TEXT("===== [StatLoadTopo] ID : SC_LOAD_TOPO - mapId:%d, warpReason:%d, pos:%0.f/%0.f/%0.f ====="), mapId, reason, pos.X, pos.Y, pos.Z);
	//#endif
	WarpReason warpReason = static_cast<WarpReason>(reason);
	switch (warpReason)
	{
	case WarpReason::RESURRECT:
	{
		// stop ai, auto quest move
		// https://jira.com2us.com/jira/browse/CHR-9997
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* localBase = mgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				if (UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>())
				{
					local->StopAuto(true);
				}
			}
		}
		break;
	}
	case WarpReason::CHANGE_CHANNEL:
	case WarpReason::USE_WARP_TO_TOWN_SCROLL:
	case WarpReason::EVENT_ACTION_RETURN_TO_PUBLIC:
	case WarpReason::EVENT_ACTION_TO_INSTANCE_SINGLE:
	case WarpReason::LEAVE_INSTANCE_SINGLE:
	{
		// 개인 이벤트 채널 나가기 시 자동 전투, 자동 진행 중지
		// -> only auto quest move stop(22/03/08), https://jira.com2us.com/jira/browse/CHR-9997
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* localBase = mgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				if (UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>())
				{
					local->StopAuto();
				}
			}
		}
	}
	break;
	case WarpReason::PORTAL:
		//bak1210 : 2번이상 보낼때 있어 쿨타임 제어 추가
		GNet()->GetCoolTime().RemoveCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_WARP));
		break;
	default:
		break;
	}

	if (UGsLevelManager* levelMgr = GLevel())
	{
#if 0
		//bak1210 : 부활 디버깅
		DrawDebugCylinder(levelMgr->GetWorld(), pos, FColor::Purple);
#endif
		levelMgr->SetLoadTopoData(mapId, pos, warpReason);

		GGameData()->SetLoadWorldInfo(pkt->WorldId(), pkt->EnterWorldServerType());
	}

	//지금 부터 SpawnMe가 올때까진 Send상황 옵저빙
	TSharedPtr<FGsNetBase> active = GNet()->GetActive();
	if (active.IsValid())
	{
		active->SetPacketSendBlock(true);
		active->SetObserveSend(true);
	}

	GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::CLOSE, nullptr);


	FGsPrimitiveInt32 param(reason);
	GMessage()->GetSystemParam().SendMessage(MessageSystem::LOAD_TOPO_START, &param);

	GSLOG(Error, TEXT("StatLoadTopo End"));


#ifdef NAVIMESH_TEST_LOCATION_LOG
	if (UGsCheatManager::_IsShowNaviMeshInfo)
	{
		if (UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			FVector dir = FVector(0.f, 0.f, -1.0f);
			FGsInputEventMsgLocation location(pos, dir);

			DrawDebugCircle(localBase->GetWorld(), pkt->ConstRefPos(), 80.0f, 100, FColor::Red, false, 10.0f, 0, 5.0f);

			GSLOG(Error, TEXT("NAVIMESH_TEST_LOCATION_LOG : POS [%0.f / %0.f / %0.f ]"), pos.X, pos.Y, pos.Z);
		}
	}
#endif	
}

void FGsNetMessageHandlerGameObject::WarpFailure(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::ForcedHideBlockUI(EGsHideBlockUIFactor::WARP_FAIL);

	if (PD::SC::PKT_SC_ACK_WARP_FAILURE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_WARP_FAILURE_READ*>(inPacket.Buffer))
	{
		int32 result = pkt->Result();
		PD::Result eResult = static_cast<PD::Result>(result);

		GNet()->GetCoolTime().RemoveCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_WARP));

		if (false == FGsWarpFunc::CheckWarpFailure(eResult))
		{
			FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		}

		// 모종의 이유로 워프가 실패했을 경우, 임시로 막았던 단축키 사용을 풀어 주어야 한다
		FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_WAIT_SERVER_RESPONSE, false);
		GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
	}
}

void FGsNetMessageHandlerGameObject::UserExpUpdate(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_USER_EXP_UPDATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_USER_EXP_UPDATE_READ*>(inPacket.Buffer))
	{
		GGameData()->Set(*pkt);
	}
}
void FGsNetMessageHandlerGameObject::UserCurrencyUpdate(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_USER_CURRENCY_FINAL_UPDATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_USER_CURRENCY_FINAL_UPDATE_READ*>(inPacket.Buffer))
	{
		GGameData()->Set(*pkt);
		//GSLOG(Log, TEXT("\tUser Gold up [UpGold:%llu]"), pkt->UpGold());
	}
}

void FGsNetMessageHandlerGameObject::SkillCoolDownAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_SKILL_COOLDOWN_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SKILL_COOLDOWN_READ*>(inPacket.Buffer))
	{
		float remainTime = pkt->RemainTime();
		if (const FGsSchemaSkillSet* skillsetData =
			UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(pkt->SkillId()))
		{
			// 평타 스킬은 쿨타임 자체가 없다.
			//if (false == skillsetData->isNormal)
			{
				GCoolTime()->SetCoolTime(EGsCoolTime::Skill, pkt->SkillCooldownGroupId(), pkt->SkillId(), remainTime);
				FGsCoolTimeMessageParam messageParam(EGsCoolTime::Skill, pkt->SkillCooldownGroupId(), pkt->SkillId(), remainTime);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_COOLTIME_START, &messageParam);
			}
		}
	}
}

// 나를 포함한 주변에 스폰된 유저들에 한하여 내려오는 Packet
void FGsNetMessageHandlerGameObject::UserLookInfoChangeAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_USER_LOOK_INFO_CHANGED_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_USER_LOOK_INFO_CHANGED_READ*>(inPacket.Buffer);

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		if (UGsGameObjectBase* BaseObject = mgr->FindObject(EGsGameObjectType::Player, pkt->GameId()))
		{
			UGsGameObjectPlayer* playerObject = BaseObject->CastGameObject<UGsGameObjectPlayer>();
			if (nullptr == playerObject)
				return;

			if (FGsGameObjectDataPlayer* creatureData = playerObject->GetCastData<FGsGameObjectDataPlayer>())
			{
				creatureData->SetUserLookInfo(*pkt);
			}

			playerObject->OnUserBasicDataChange(pkt->ConstRefLookInfo());
			playerObject->ChangeCostumeEquip(pkt->ConstRefLookInfo().mTransformCostumeId, pkt->ConstRefLookInfo().mCostumeId, pkt->ConstRefLookInfo().mViewHelmet);
			playerObject->ChangeFairyEquip(pkt->ConstRefLookInfo().mFairy);

			//서버에서 오는 game id는 모두 양수이다. 따라서 int64를 uint64가 처리할 수 있다. 
			GMessage()->GetUserBasicInfo().SendMessage(MessageUserInfo::LOOK_INFO, pkt->GameId());
		}
	}
}

void FGsNetMessageHandlerGameObject::UserIffStatusAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_USER_IFF_STATUS_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_USER_IFF_STATUS_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* local = mgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				// NetIffFactionActorUpdate 함수에서 처리
				// local->GetData()->SetIffFaction(pkt->ConstRefIffFactionActor());

				if (UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(local))
				{
					localPlayer->NetIffFactionActorUpdate(pkt->ConstRefIffFactionActor());
					localPlayer->UpdateBillboard(EGsBillboardUpdateType::NameColor);
					localPlayer->OnIFFChanged();
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::IffStatusAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_IFF_STATUS_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_IFF_STATUS_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				findObject->GetData()->SetIffFaction(pkt->ConstRefIffFactionActor());

				if (findObject->IsObjectType(EGsGameObjectType::Player))
				{
					// 내캐릭터 전용 IFFHelper Update
					if (findObject->IsObjectType(EGsGameObjectType::LocalPlayer))
					{
						UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(findObject);
						localPlayer->NetIffFactionActorUpdate(pkt->ConstRefIffFactionActor());
					}

					UGsGameObjectPlayer* playerObject = Cast<UGsGameObjectPlayer>(findObject);
					playerObject->UpdateBillboard(EGsBillboardUpdateType::NameColor);

				}
				findObject->OnIFFChanged();
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::UpdateIffTeamAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_UPDATE_IFF_TEAM_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_UPDATE_IFF_TEAM_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				// findObject->GetData()->SetIffTeam(pkt->Team());


				if (UGsGameObjectBase* local = mgr->FindObject(EGsGameObjectType::LocalPlayer))
				{
					// 내가 바뀌었을 경우
					if (local->GetGameId() == pkt->GameId())
					{
						// 모든 유저의 네임플레이트 업데이트
						TArray<UGsGameObjectBase*> remotePlayerList = mgr->FindObjectArray(EGsGameObjectType::RemotePlayer);
						for (UGsGameObjectBase* iter : remotePlayerList)
						{
							UGsGameObjectPlayer* remotePlayer = iter->CastGameObject<UGsGameObjectPlayer>();
							if (remotePlayer)
							{
								remotePlayer->UpdateBillboard(EGsBillboardUpdateType::NameColor);
							}
						}

						// 나자신 iff 변경
						local->OnIFFChanged();
					}
					else
					{
						// 다른 유저가 바뀌었을 경우, 해당 유저만 업데이트
						if (UGsGameObjectPlayer* player = findObject->CastGameObject<UGsGameObjectPlayer>())
						{
							player->UpdateBillboard(EGsBillboardUpdateType::NameColor);
							// 타유저 iff 변경
							player->OnIFFChanged();
						}
					}
				}
			}
		}

	}
}

void FGsNetMessageHandlerGameObject::UserLevelUpdatedNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_USER_LEVEL_UPDATED_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_USER_LEVEL_UPDATED_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	UGsGameObjectManager* mgr = GSGameObject();
	if (nullptr == mgr)
	{
		return;
	}

	UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId());
	if (nullptr == findObject)
	{
		GSLOG(Error, TEXT("PKT_SC_USER_LEVEL_UPDATED_READ - game id : %lld not found GameObject"), pkt->GameId());
		return;
	}

	if (findObject->IsObjectType(EGsGameObjectType::Creature))
	{
		//set pre level 
		uint16 preLevel = GGameData()->GetUserData()->mLevel;

		GSLOG(Log, TEXT("pre level : %d\tcurrent level : %d"), preLevel, pkt->UpdatedLevel());

		FGsGameObjectData* gameObjectData = findObject->GetData();
		gameObjectData->SetLevel(pkt->UpdatedLevel());

		//notify player level
		if (findObject->IsObjectType(EGsGameObjectType::LocalPlayer))
		{
			FGsUIMsgParamLevelUp param(preLevel, pkt->UpdatedLevel());
			GMessage()->GetUI().SendMessage(MessageUI::LEVEL_UP, &param);
		}
	}
}

void FGsNetMessageHandlerGameObject::CreatureCombatStatusNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_CREATURE_COMBAT_STATUS_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_CREATURE_COMBAT_STATUS_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				if (FGsGameObjectDataCreature* creatureData = findObject->GetCastData<FGsGameObjectDataCreature>())
				{
					creatureData->SetCreatureCombatStatus(*pkt);

					UGsGameObjectCreature* castObj = findObject->CastGameObject<UGsGameObjectCreature>();
					castObj->UpdateCombatStatus();
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SpawnCampAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("SpawnCampAck()"));

	PD::SC::PKT_SC_ACK_SPAWN_CAMP_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_SPAWN_CAMP_READ*>(inPacket.Buffer);
	int64 result = pkt->Result();
	FGsCampManager* campMgr = GSCamp();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (nullptr != campMgr)
		{
			campMgr->DespawnClientCampActor();
			campMgr->CloseCampPlacementMenuUI();
		}

		FText findText;
		if (result == PD::Result::CAMP_ERROR_NEAR_CAMPS_EXIST)
		{
			// 주변에 다른 캠프가 존재하여 건설할 수 없습니다.
			if (true == FText::FindText(TEXT("GuildText"), TEXT("CAMP_ERROR_NEAR_CAMPS_EXIST"), findText))
			{
				FGsUIHelper::PopupOK(findText);
			}
			//FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		else if (result == PD::Result::CAMP_ERROR_SPAWN_CAMP)
		{
			// 다른 캠프가 이미 설치되어 있어 추가 건설이 불가능합니다.
			if (true == FText::FindText(TEXT("GuildText"), TEXT("CAMP_ERROR_SPAWN_CAMP"), findText))
			{
				FGsUIHelper::PopupOK(findText);
			}
			//FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		else if (result == PD::Result::GUILD_ERROR_GUILD_CAMP_LEVEL)
		{
			// TEXT: 기사단 수호탑 레벨이 부족해 아이템을 사용할 수 없습니다
			if (true == FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_NotUse_05"), findText))
			{
				FGsUIHelper::PopupOK(findText);
			}
			//FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		else if (result == PD::Result::GUILD_ERROR_GRADE)
		{
			// TEXT: 수호탑을 설치할 수 있는 권한이 없습니다.
			if (FText::FindText(TEXT("GuildText"), TEXT("GUILD_ERROR_GRADE"), findText))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			}

			return;
		}
		else if (result == PD::Result::INVADE_ERROR_UNKNOWN)
		{
			if (true == FText::FindText(TEXT("InvadeText"), TEXT("Invade_Cant_Spawn_01"), findText))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			}
			//FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
		GSLOG(Warning, TEXT("[SpawnCampAck - Error Result] ResultID : %d"), result);
		return;
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[SpawnCampAck] pkt->GameId : %llu"), pkt->GameId());
#endif
	campMgr->OnSpawnCampAck(pkt->GameId());
}

void FGsNetMessageHandlerGameObject::AllowedBuildCampNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_GUILD_CAMP_INSTALLATION_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_GUILD_CAMP_INSTALLATION_READ*>(inPacket.Buffer))
	{
		if (FGsCampManager* campMgr = GSCamp())
		{
			campMgr->SetIsAllowedBuildCamp(!pkt->Check());
		}
	}
}

void FGsNetMessageHandlerGameObject::CampBuildStepNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_GUILD_CAMP_CONSTRUCTION_STEP_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_GUILD_CAMP_CONSTRUCTION_STEP_READ*>(inPacket.Buffer))
	{
		UGsGameObjectManager* objectMgr = GSGameObject();
		if (nullptr == objectMgr)
			return;

		if (UGsGameObjectBase* creatureObject = objectMgr->FindObject(EGsGameObjectType::Camp, pkt->GameId()))
		{
			UGsGameObjectCamp* campObject = creatureObject->CastGameObject<UGsGameObjectCamp>();
			if (nullptr != campObject)
			{
				campObject->SetBuildingLevel(pkt->BuildingLevel());
				campObject->SetCampID(pkt->CampId());
				campObject->UpdateCampName(campObject->GetCampActorBase());
				campObject->UpdateEmblemIcon();
				campObject->SetCurStep(pkt->CurrentStep());
				campObject->SetMaxStep(pkt->MaxStep());
				campObject->OnReceiveStepPacket();
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::CampNameNotify(const FGsNet::Packet& InPacket)
{
	if (PD::SC::PKT_SC_SPAWN_CAMP_NAME_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SPAWN_CAMP_NAME_READ*>(InPacket.Buffer))
	{
		UGsGameObjectManager* objectMgr = GSGameObject();
		if (nullptr == objectMgr)
			return;

		if (UGsGameObjectBase* creatureObject = objectMgr->FindObject(EGsGameObjectType::Camp, pkt->GameId()))
		{
			UGsGameObjectCamp* campObject = creatureObject->CastGameObject<UGsGameObjectCamp>();
			if (nullptr != campObject)
			{
				campObject->SetCampOwnerGuildName(pkt->GuildName());
				campObject->SetOwnerGuildDbid(pkt->GuildDBId());
				campObject->SetGuildEmblemID(pkt->GuildEmblemId());
				campObject->SetSpawnReasonType(pkt->Reason());
				campObject->SetGuildWorldId(pkt->HomeWorldId());
				campObject->OnCampNameNotifyFinish();
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::UserNpcInteractStateNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_USER_NPC_INTERACT_STATE_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_USER_NPC_INTERACT_STATE_READ*>(inPacket.Buffer))
	{
		UGsGameObjectManager* objectMgr = GSGameObject();
		if (nullptr == objectMgr)
			return;

		if (UGsGameObjectBase* findObject = objectMgr->FindObject(EGsGameObjectType::RemotePlayer, pkt->UserGameId()))
		{
			UGsGameObjectRemotePlayer* castObject = findObject->CastGameObject<UGsGameObjectRemotePlayer>();
			if (nullptr == castObject)
				return;

			FGsInteractionHandlerBase* interationHandler = castObject->GetInteractionHandler();
			if (nullptr == interationHandler)
				return;

			if (FGsMovementHandlerRemote* moveRemort = castObject->GetCastMovement<FGsMovementHandlerRemote>())
			{
				//moveRemort->SetNetMoveSpeed(pkt->Speed());
				moveRemort->ReceveUserInteractNpcState(pkt);
			}

			// 타겟에 대한 처리
			if (UGsGameObjectBase* sanctumObjectBase = objectMgr->FindObject(EGsGameObjectType::Sanctum, pkt->TargetGameId()))
			{
				interationHandler->StartInteraction(sanctumObjectBase);

				if (UGsGameObjectSanctumBase* sanctumObject = sanctumObjectBase->CastGameObject<UGsGameObjectSanctumBase>())
				{
					sanctumObject->AddInteractionGameId(pkt->UserGameId());
				}
			}
			else if (UGsGameObjectBase* campObject = objectMgr->FindObject(EGsGameObjectType::Camp, pkt->TargetGameId()))
			{
				interationHandler->StartInteraction(campObject);
				GSCamp()->AddInteractionUser(pkt->TargetGameId());
			}
		}
		else
		{
			if (UGsGameObjectBase* sanctumObjectBase = objectMgr->FindObject(EGsGameObjectType::Sanctum, pkt->TargetGameId()))
			{
				if (UGsGameObjectSanctumBase* sanctumObject = sanctumObjectBase->CastGameObject<UGsGameObjectSanctumBase>())
				{
					sanctumObject->AddInteractionGameId(pkt->UserGameId());
				}				
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::CancelNpcInteractNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_CANCEL_NPC_INTERACT_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_CANCEL_NPC_INTERACT_READ*>(inPacket.Buffer))
	{
		UGsGameObjectManager* objectMgr = GSGameObject();
		if (nullptr == objectMgr)
		{
			return;
		}

		if (UGsGameObjectBase* findObject = objectMgr->FindObject(EGsGameObjectType::Player, pkt->GameId()))
		{
			UGsGameObjectPlayer* castObject = findObject->CastGameObject<UGsGameObjectPlayer>();
			if (nullptr == castObject)
				return;

			FGsInteractionHandlerBase* interationHandler = castObject->GetInteractionHandler();
			if (nullptr == interationHandler)
				return;

			interationHandler->CancelInteration();
		}

		if (UGsGameObjectBase* sanctumObjectBase = objectMgr->FindObject(EGsGameObjectType::Sanctum, pkt->TargetGameId()))
		{
			if (UGsGameObjectSanctumBase* sanctumObject = sanctumObjectBase->CastGameObject<UGsGameObjectSanctumBase>())
			{
				sanctumObject->RemoveInteractionGameId(pkt->GameId());
			}
		}
		else if (UGsGameObjectBase* campObjectBase = objectMgr->FindObject(EGsGameObjectType::Camp, pkt->TargetGameId()))
		{
			GSCamp()->RemoveInteractionUser(pkt->TargetGameId());
		}
	}
}

void FGsNetMessageHandlerGameObject::CampConstructionCompletedNotify(const FGsNet::Packet& InPacket)
{
	// 패킷 내용물 없는 노티이므로 캐스트 할 필요없음
	GMessage()->GetGuild().SendMessage(MessageContentGuild::CAMP_CONSTRUCTION, nullptr);
}

void FGsNetMessageHandlerGameObject::DespawnCampNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_GUILD_CAMP_DESPAWN_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_GUILD_CAMP_DESPAWN_READ*>(inPacket.Buffer))
	{

	}
}

void FGsNetMessageHandlerGameObject::DespawnCampAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ACK_GUILD_CAMP_DESTRUCTION_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ACK_GUILD_CAMP_DESTRUCTION_READ*>(inPacket.Buffer))
	{
		if (PACKET_RESULT_SUCCESS != pkt->Result())
		{
			FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
			return;
		}
	}
}

void FGsNetMessageHandlerGameObject::GuildMemberCampNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_GUILD_MEMBER_CAMP_NOTIFY_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_GUILD_MEMBER_CAMP_NOTIFY_READ*>(inPacket.Buffer))
	{
		if (nullptr == GSGameObject())
			return;

		GuildCampNotifyType notifyType = pkt->NotifyType();
		if (GuildCampNotifyType::UNDER_ATTACK == notifyType)
		{
			FText findText;
			FText::FindText(TEXT("GuildText"), TEXT("Alert_CampBeUnderAttack"), findText);
			FGsUIHelper::AddChatMessageSystem(findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
		}
		else
		{
			FText findText;
			if (GuildCampNotifyType::Dismantle == notifyType) // 해제
			{
				FText::FindText(TEXT("GuildText"), TEXT("Alert_CampDismantle"), findText);
				FText textMsg = FText::Format(findText, FText::FromString(pkt->UserName()));
				FGsUIHelper::AddChatMessageSystem(textMsg);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg, TrayCondition::TC_Close);
			}
			else if (GuildCampNotifyType::Destruction == notifyType) // 파괴
			{
				FText::FindText(TEXT("GuildText"), TEXT("Alert_CampDestruction"), findText);
				FGsUIHelper::AddChatMessageSystem(findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			}

			if (GGuild()->RemoveCampData(pkt->CampId()))
			{
				GMessage()->GetGuild().SendMessage(MessageContentGuild::CAMP_DESTRUCTION, nullptr);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SanctumNexusInstallAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_SANCTUM_NEXUS_INSTALL_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ACK_SANCTUM_NEXUS_INSTALL_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		// 인터렉션 예약 취소
		GMessage()->GetSanctum().SendMessage(MessageContentSanctum::CANCEL_RESERVED_INTERACTION, nullptr);

		// 티커 처리		
		if (FGsSanctumHandler::ShowSanctumMessageTicker(pkt->Result()))
		{
			return;
		}

		// 티커 처리 안된 항목 팝업 처리
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}
}

void FGsNetMessageHandlerGameObject::SanctumStatusNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SANCTUM_STATUS_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SANCTUM_STATUS_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	UGsGameObjectManager* gameObjectMgr = GSGameObject();
	if (nullptr == gameObjectMgr)
	{
		return;
	}

	UGsGameObjectBase* creatureObject = gameObjectMgr->FindObject(EGsGameObjectType::Sanctum, pkt->GameId());
	if (nullptr == creatureObject)
	{
		return;
	}

	if (UGsGameObjectSanctumBase* sanctumObject = creatureObject->CastGameObject<UGsGameObjectSanctumBase>())
	{
		sanctumObject->OnSanctumSataus(pkt->SanctumAreaId(), pkt->CanInteract());
	}
}

void FGsNetMessageHandlerGameObject::SanctumNexusStatusNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SANCTUM_NEXUS_STATUS_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SANCTUM_NEXUS_STATUS_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	UGsGameObjectManager* gameObjectMgr = GSGameObject();
	if (nullptr == gameObjectMgr)
	{
		return;
	}

	UGsGameObjectBase* creatureObject = gameObjectMgr->FindObject(EGsGameObjectType::Sanctum, pkt->GameId());
	if (nullptr == creatureObject)
	{
		return;
	}

	if (false == creatureObject->IsA<UGsGameObjectSanctumNexus>())
	{
		return;
	}

	if (UGsGameObjectSanctumNexus* sanctumObject = creatureObject->CastGameObject<UGsGameObjectSanctumNexus>())
	{
		sanctumObject->OnSanctumSataus(pkt->SanctumAreaId(), pkt->CanInteract());

		sanctumObject->SetSpawnReasonType(pkt->Reason());
		sanctumObject->SetGuildInfo(pkt->GuildDBId(), pkt->GuildEmblemId(), pkt->GuildName(), pkt->HomeWorldId(), 
			pkt->BuildingLevel());
		sanctumObject->SetCurrConstructionStep(pkt->CurrentStep());
		sanctumObject->SetMaxConstructionStep(pkt->MaxStep());

		sanctumObject->OnConstructionStep(pkt->CurrentStep());

		// 우리 길드일 경우 갱신 메시지 보냄
		if (GGuild()->GetGuildDBId() == pkt->GuildDBId())
		{
			GMessage()->GetGuild().SendMessage(MessageContentGuild::SANCTUM_CONSTRUCTION_STATE_CHANGED, nullptr);
		}

		// 인터렉션 예약에 대한 체크
		FGsSanctumMsgParamSanctumId param(INVALID_SANCTUM_AREA_ID, pkt->GameId());
		GMessage()->GetSanctum().SendMessage(MessageContentSanctum::TRY_START_RESERVED_INTERACTION, &param);
	}
}

void FGsNetMessageHandlerGameObject::SanctumNexusConstructionStepNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SANCTUM_NEXUS_CONSTRUCTION_STEP_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SANCTUM_NEXUS_CONSTRUCTION_STEP_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	UGsGameObjectManager* gameObjectMgr = GSGameObject();
	if (nullptr == gameObjectMgr)
	{
		return;
	}

	UGsGameObjectBase* creatureObject = gameObjectMgr->FindObject(EGsGameObjectType::Sanctum, pkt->GameId());
	if (nullptr == creatureObject)
	{
		return;
	}

	if (false == creatureObject->IsA<UGsGameObjectSanctumNexus>())
	{
		return;
	}

	if (UGsGameObjectSanctumNexus* sanctumObject = creatureObject->CastGameObject<UGsGameObjectSanctumNexus>())
	{
		//pkt->SanctumNexsusId(); // 필요한가?		
		sanctumObject->SetCurrConstructionStep(pkt->CurrentStep());
		sanctumObject->SetMaxConstructionStep(pkt->MaxStep());
		sanctumObject->SetGuildBuildingLevel(pkt->BuildingLevel()); // 필요한가?

		sanctumObject->OnConstructionStep(pkt->CurrentStep());

		// 건설 완료 시 기사단 페이지 갱신을 위한 메시지
		if (0 != pkt->MaxStep() &&
			pkt->CurrentStep() == pkt->MaxStep())
		{
			if (GGuild()->GetGuildDBId() == sanctumObject->GetGuildDBId())
			{
				GMessage()->GetGuild().SendMessage(MessageContentGuild::SANCTUM_CONSTRUCTION_STATE_CHANGED, nullptr);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SanctumBattlePrepareFirstNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SANCTUM_ALERT_BATTLE_START_FIRST_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SANCTUM_ALERT_BATTLE_START_FIRST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GMessage()->GetSanctum().SendMessage(MessageContentSanctum::SANCTUM_BATTLE_FIRST_NOTIFY, nullptr);
}

void FGsNetMessageHandlerGameObject::SanctumBattlePrepareFinalNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SANCTUM_ALERT_BATTLE_START_FINAL_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SANCTUM_ALERT_BATTLE_START_FINAL_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GMessage()->GetSanctum().SendMessage(MessageContentSanctum::SANCTUM_BATTLE_FINAL_NOTIFY, nullptr);
}

void FGsNetMessageHandlerGameObject::SanctumBattleStartNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SANCTUM_ALERT_BATTLE_START_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SANCTUM_ALERT_BATTLE_START_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GMessage()->GetSanctum().SendMessage(MessageContentSanctum::SANCTUM_BATTLE_START, nullptr);
}

void FGsNetMessageHandlerGameObject::SanctumBattleEndNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SANCTUM_ALERT_BATTLE_END_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SANCTUM_ALERT_BATTLE_END_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 종료상황
	if (0 == pkt->RemainSec())
	{
		GMessage()->GetSanctum().SendMessage(MessageContentSanctum::SANCTUM_BATTLE_END, nullptr);
	}
	else
	{
		FGsPrimitiveInt32 param(pkt->RemainSec());
		GMessage()->GetSanctum().SendMessage(MessageContentSanctum::SANCTUM_BATTLE_BEFORE_END_NOTIFY, &param);
	}
}

void FGsNetMessageHandlerGameObject::SanctumBattleVictoryNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SANCTUM_BATTLE_VICTORY_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SANCTUM_BATTLE_VICTORY_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 점령 성공한 기사단에게만 오는 패킷
	FGsSanctumMsgParamSanctumId param(pkt->SanctumAreaId(), 0);
	GMessage()->GetSanctum().SendMessage(MessageContentSanctum::SANCTUM_BATTLE_OCCUPY_SUCCESS, &param);
}

void FGsNetMessageHandlerGameObject::SanctumGuildMemberNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_GUILD_MEMBER_SANCTUM_NEXUS_NOTIFY_READ* pkt = 
		reinterpret_cast<PD::SC::PKT_SC_GUILD_MEMBER_SANCTUM_NEXUS_NOTIFY_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GuildCampNotifyType notifyType = pkt->NotifyType();
	switch (pkt->NotifyType())
	{
	case GuildCampNotifyType::UNDER_ATTACK:
	{
		// TEXT: 수호탑이 공격을 받고 있습니다
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_CampBeUnderAttack"), textMsg);
		FGsUIHelper::AddChatMessageSystem(textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg, TrayCondition::TC_Close);
	}
	break;
	case GuildCampNotifyType::Dismantle:
	{
		// TEXT:{0}님이 수호탑을 해체 했습니다.
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_CampDismantle"), textFormat))
		{
			FText textMsg = FText::Format(textFormat, FText::FromString(pkt->UserName()));
			FGsUIHelper::AddChatMessageSystem(textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg, TrayCondition::TC_Close);
		}

		// 성소 정보를 비움
		GGuild()->SetClearSanctumNexusData();

		GMessage()->GetGuild().SendMessage(MessageContentGuild::SANCTUM_DESTRUCTION, nullptr);
	}
	break;
	case GuildCampNotifyType::Destruction:
	{
		// TEXT: 수호탑이 파괴 되었습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_CampDestruction"), textMsg);
		FGsUIHelper::AddChatMessageSystem(textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg, TrayCondition::TC_Close);

		// 성소 정보를 비움
		GGuild()->SetClearSanctumNexusData();

		GMessage()->GetGuild().SendMessage(MessageContentGuild::SANCTUM_DESTRUCTION, nullptr);
	}
	break;
	}
}

void FGsNetMessageHandlerGameObject::SanctumNexusEmblemChangeNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SANCTUM_NEXUS_EMBLEM_CHANGE_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SANCTUM_NEXUS_EMBLEM_CHANGE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
	{
		if (UGsGameObjectBase* sanctumObjectBase = gameObjectMgr->FindObject(EGsGameObjectType::Sanctum, pkt->GameId()))
		{
			if (false == sanctumObjectBase->IsA<UGsGameObjectSanctumNexus>())
			{
				return;
			}

			if (UGsGameObjectSanctumNexus* sanctumObject = sanctumObjectBase->CastGameObject<UGsGameObjectSanctumNexus>())
			{
				sanctumObject->SetGuildEmblem(pkt->GuildEmblemId());
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::StatInfoNotify(const FGsNet::Packet& inPacket)
{
	// 자동 회복류는 나만 응답 받고
	// 물약등의 Hp, Mp 변화가 있는것은 브로드 캐스팅
	PD::SC::PKT_SC_STAT_INFO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_STAT_INFO_READ*>(inPacket.Buffer);
	int count = pkt->StatInfoSize() / sizeof(StatInfo);
	int64 gameid = pkt->GameId();

#if WITH_EDITOR
	GSLOG(Warning, TEXT("--------------------------------"));
	GSLOG(Warning, TEXT("[SC_STAT_INFO - GameID ] : %d"), gameid);
	GSLOG(Warning, TEXT("[SC_STAT_INFO - GetStatInfoCount] : %d"), count);
	GSLOG(Warning, TEXT("--------------------------------"));
#endif

	if (0 == count)
	{
		return;
	}

	UGsGameObjectManager* mgr = GSGameObject();
	if (nullptr == mgr)
	{
		return;
	}

	UGsGameObjectBase* creatureObject = mgr->FindObject(EGsGameObjectType::Creature, gameid);
	if (nullptr == creatureObject)
	{
		return;
	}

	FGsGameObjectDataCreature* creatureData = creatureObject->GetCastData<FGsGameObjectDataCreature>();
	if (nullptr == creatureData)
	{
		return;
	}
					
	using StatInfoArray = TArray<StatInfo>;
	StatInfoArray statInfoArray;
	pkt->MakeStatInfoVector<StatInfoArray, StatInfo>(statInfoArray);	
	TArray<TPair<StatType, int32>> statInfoMap;

#if !UE_BUILD_SHIPPING
	GSLOG(Warning, TEXT("PKT_SC_STAT_INFO  Owner[%lld] : %s --------------------------------"), gameid, *creatureData->GetName());
#endif

	for (int i = 0; i < statInfoArray.Num(); ++i)
	{
		StatType type = statInfoArray[i].mType;
		StatValue value = statInfoArray[i].mValue;

#if !UE_BUILD_SHIPPING
		//GSLOG(Error, TEXT("SC_STAT_INFO  -> StatType[%s] : Value[%d]"), *StatTypeEnumToString(type), value);
#endif

		//전투력은 서버에서 오버헤드 처리하지 않기 위해 클라에서 100으로 나눠준다. 
		if (StatType::COMBAT_POWER == type)
		{
			value /= 100;
		}

		statInfoMap.Emplace(TPair< StatType, StatValue>(type, value));
	}

	//Player stat 변경시 
	if (mgr->FindObject(EGsGameObjectType::LocalPlayer, gameid))
	{
		//플레이어 변경시 노티파이
		GSStatChangeNotify()->CheckNotify(pkt->StatUpdateReason(), statInfoMap);
	}

	creatureData->UpdateCreatureStatInfo(statInfoMap);

	int printhp = 0;
	if (const FGsStatInfo* hpInfo = creatureData->GetStatBase()->GetStatInfo(StatType::HP))
	{
		printhp = hpInfo->GetStatValue();
	}
#if !UE_BUILD_SHIPPING
	GSLOG(Warning, TEXT("PKT_SC_STAT_INFO AfterHp[%d]--------------------------------"), printhp);
#endif

	//Player stat 변경시 
	if (mgr->FindObject(EGsGameObjectType::LocalPlayer, gameid))
	{
		//플레이어 변경시 노티파이
		FGsGameObjectMessageParamPlayerStat message(creatureObject, statInfoMap);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PLAYER_STAT_UPDATE, &message);
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_STAT);
	}

	statInfoMap.Empty();
}

void FGsNetMessageHandlerGameObject::UserBasicDataAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_USER_BASIC_DATA_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_USER_BASIC_DATA_READ*>(inPacket.Buffer);

	if (nullptr == GMode())
	{
		GSLOG(Log, TEXT("\tUser Basic [DBId: %llu, Status: %d, Level: %d, Exp:%llu, GameId: %lld, Name: %s]"),
			pkt->UserDBId(), pkt->UserStatus(), pkt->Level(), pkt->Exp(), pkt->GameId(), *pkt->UserName());
		return;
	}
	GGameData()->Set(*pkt);
		
	if (INVALID_GUILD_DB_ID == pkt->GuildDBId())
	{
		// 길드 삭제 중 네트워크 중단될 경우, 답변을 못받아서 길드 정보가 지워지지 않는 이슈가 있어서 확인함
		if (GGuild()->IsGuild())
		{
			GGuild()->RemoveAllGuildData();
		}
	}

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		if (UGsGameObjectBase* local = mgr->FindObject(EGsGameObjectType::LocalPlayer, pkt->GameId()))
		{
			if (UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>())
			{
				const FGsNetUserData* userData = GGameData()->GetUserData();
				castLocal->OnUserBasicDataChange(userData->mLookInfo);
				castLocal->UpdateCostumeEquip(userData->mLookInfo.mTransformCostumeId, userData->mLookInfo.mCostumeId, userData->mLookInfo.mViewHelmet);
				castLocal->UpdateFairyEquip(userData->mLookInfo.mFairy);
			}
		}
	}

	/*	if (UGsGameObjectBase * local = FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* cast = local->CastGameObject< UGsGameObjectLocalPlayer>();
			cast->GetCustomize()->ReceiveCustomData(pkt->ConstRefCustomizingFaceData(), pkt->ConstRefCustomizingBodyData());
		}*/
}


//TestCode
void FGsNetMessageHandlerGameObject::TestSpawnRemote(int64 inGameId, FVector& inPos, FRotator& inRot)
{
	UClass* bpClass = nullptr;
	if (const FGsSchemaShapeData* shapeData =
		UGsTableUtil::FindRowById<UGsTableShapeData, FGsSchemaShapeData>(2))
	{
		FString path = shapeData->bPClass.ToString();
		path += TEXT("_C");

		bpClass = LoadObject<UClass>(nullptr, *path);
	}
	if (bpClass)
	{
		// GameObject 클래스 객체는 이미 생성 되어 있을수도 있음
		// 이곳에서 동적할당 하는 객체나 Initialize / DeInitialize 페어가 되는 설정은 금지
		// 관련 로직은 GameObject 클래스 내부에서 처리

		FGsSpawnParam TestParam;
		TestParam._tId = 1;
		TestParam.Set(EGsGameObjectType::RemotePlayer, inGameId, inPos, inRot);

		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* remote = mgr->SpawnObject(&TestParam))
			{
			}
		}
	}
}


// RemortPC 이동패킷이 여기로 온다.
// 내패킷도 올수 있는데 이는 에러상황에서만 오게된다.. 에러 처리용
void FGsNetMessageHandlerGameObject::MoveToLocationAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_MOVE_TO_LOCATION_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_MOVE_TO_LOCATION_READ*>(inPacket.Buffer))
	{
		// bak1210 : 클라이언트 네트워크 동기화용 테스트 코드 주석(추후에 쓸수 있음)

		//if (UGsGameObjectBase* local = FindObject(EGsGameObjectType::LocalPlayer))
		//{
		//	if (local->GetGameId() == pkt->GameId())
		//	{
		//		check(local->GetActor());
		//		/*GSLOG(Log, TEXT("\t MoveToLocation [GameId: %llu], [Dir: %d], [MotionType: %d], [ClientTick:%llu]"),
		//			pkt->GameId(), pkt->CurDir(), pkt->MotionType(), pkt->ClientTick());*/				


		//		// TestCode Start ####################################################################################################################
		//		if (UGsGameObjectBase* remote = FindObject(EGsGameObjectType::RemotePlayer, 1000))
		//		{
		//			if (nullptr == local->GetActor())
		//			{
		//				GSLOG(Log, TEXT("\tMoveToLocation Msg Can't Find Actor [GameId: %lld]"), 1000);
		//				return;
		//			}

		//			if (auto component = Cast<UGsMovementDebugShapeComponent>(remote->GetActor()->GetComponentByClass(UGsMovementDebugShapeComponent::StaticClass())))
		//			{
		//				component->SetLocation(pkt->CurPos());
		//				component->SetDestination(pkt->DestPos());
		//			}

		//			if (FGsMovementHandlerRemote* moveRemote = remote->CastGameObject<UGsGameObjectCreature>()->GetCastMovement<FGsMovementHandlerRemote>())
		//			{
		//				moveRemote->SetNetMoveSpeed(pkt->Speed());
		//				moveRemote->ReceveMoveToLocation(EGsGameObjectType::RemotePlayer, inPacket);
		//			}
		//		}
		//		else
		//		{
		//			FRotator DirRot = FGsDir::AngleToRotator(pkt->CurDir());
		//			FVector Pos = pkt->ConstRefCurPos();
		//			//TestSpawnRemote(1000, Pos, DirRot);
		//		}
		//		// TestCode End ####################################################################################################################
		//	}
		//}

		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* creature = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				if (auto Actor = creature->GetActor())
				{
					if (auto component = Cast<UGsMovementDebugShapeComponent>(Actor->GetComponentByClass(UGsMovementDebugShapeComponent::StaticClass())))
					{
						component->SetLocation(pkt->CurPos());
						component->SetDestination(pkt->DestPos());
					}
				}

				// TerritoryType Sync
				if (FGsGameObjectData* gameObjectData = creature->GetData())
				{
					gameObjectData->SetTerritoryType(pkt->TerritoryType());
				}

				if (creature->GetObjectType() == EGsGameObjectType::LocalPlayer)
				{
					UGsGameObjectCreature* castCreature = creature->CastGameObject<UGsGameObjectCreature>();
					FGsMovementHandlerLocalPlayer* moveLocalPlayer = castCreature->GetCastMovement<FGsMovementHandlerLocalPlayer>();
					if (nullptr == moveLocalPlayer)
					{
						return;
					}
					if (UGsGameObjectLocalPlayer::EMovementType::Spectator == Cast<UGsGameObjectLocalPlayer>(creature)->GetMovementType())
					{
						FGsAbnormalityHandlerBase* abnormality = castCreature->GetAbnormalityHandler();
						bool ignoreDirection = false;
						bool touchEffect = !abnormality->IsAbnormalityEffectType(AbnormalityEffectType::CC_FEAR);
						FGsSkillHandlerBase* skillHandler = castCreature->GetSkillHandler();
						FGsSkillRunnerBase* skillRunner = skillHandler->GetSkillRunner();
						ignoreDirection = skillRunner->IsValid() && false == skillRunner->CanCancel();

						moveLocalPlayer->SpectatorReceveMoveToLocation(creature->GetObjectType(), pkt, touchEffect,
							ignoreDirection);
						moveLocalPlayer->SpectatorSetSpeedMovementComponent(pkt->Speed());

						if (UGsCheatManager::IsShowCreatureDestLocation)
						{
							UGsCheatManager::DrawDebugCreatureDestLocation(creature, pkt->DestPos());
						}
					}
				}
				// LocalPlayer 가 아닐때
				else
				{
					UGsGameObjectCreature* castCreature = creature->CastGameObject<UGsGameObjectCreature>();
					if (FGsMovementHandlerRemote* moveRemort = castCreature->GetCastMovement<FGsMovementHandlerRemote>())
					{
						// [U1] | ejrrb10 | RemotePlayer 가 Warp 상태일 때는, MoveToLoaction 패킷 명령을 무시한다
						if (castCreature->GetStateType() == EGsStateBase::Warp)
						{
							// 이동하지 않는다
						}
						else if (false == castCreature->IsRootmotion())
						{
							bool ignoreDirection = false;
							// 타유저는 스킬 사용중일때 방향 설정을 적용하지 않음
							if (castCreature->IsObjectType(EGsGameObjectType::RemotePlayer))
							{
								FGsSkillHandlerBase* skillHandler = castCreature->GetSkillHandler();
								FGsSkillRunnerBase* skillRunner = skillHandler->GetSkillRunner();
								ignoreDirection = skillRunner->IsValid() && false == skillRunner->CanCancel();
							}
							moveRemort->ReceveMoveToLocation(creature->GetObjectType(), pkt, ignoreDirection);
							moveRemort->SetSpeedMovementComponent(pkt->Speed());
							moveRemort->UpdateVelocityByMovetoLocation();

							// 회전 연출
							if ((MotionType)pkt->MotionType() != MotionType::STOP)
							{
								castCreature->LookAt(FGsDir::angleToDir(pkt->CurDir()).Rotation(), true);
							}
						}
						else
						{
							moveRemort->DirectMoveUpdate(pkt->CurPos(), FGsDir::angleToDir(pkt->CurDir()).Rotation());
#if UE_EDITOR
							DrawDebugSphere(castCreature->GetWorld(), pkt->CurPos(), 10.f, 100, FColor::Magenta, false, 0.5f);
#endif
						}
					}
					

					if (UGsCheatManager::IsShowCreatureDestLocation)
					{
						UGsCheatManager::DrawDebugCreatureDestLocation(creature, pkt->DestPos());
					}
				}
			}
		}
	}
}

// 바로 제자리에 멈춰야한다.
void FGsNetMessageHandlerGameObject::MoveImmediateStopAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_MOVE_IMMEDIATE_STOP_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_MOVE_IMMEDIATE_STOP_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* creature = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				if (nullptr == creature->GetActor())
				{
					GSLOG(Log, TEXT("\tMoveToLocation Msg Can't Find Actor [GameId: %lld]"), pkt->GameId());
					return;
				}

				if (FGsMovementHandlerBase* moveHandler = creature->CastGameObject<UGsGameObjectCreature>()->GetMovementHandler())
				{
					moveHandler->ReceveNetMotionStopReason(static_cast<MotionStopReason>(pkt->Reason()));
				}
			}
		}

	}
}

// 이동실패시 받는 패킷(나만 받는다)
void FGsNetMessageHandlerGameObject::MoveFailedAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_MOVE_FAILED_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_MOVE_FAILED_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* local = mgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				if (FGsMovementHandlerLocalPlayer* moveHandler = local->CastGameObject<UGsGameObjectLocalPlayer>()->GetCastMovement<FGsMovementHandlerLocalPlayer>())
				{
					moveHandler->SetNetMoveFailed(FGsDir::angleToDir(pkt->CurDir()), pkt->CurPos());
				}

				// 사망상태인지 확인
				if (local->IsZeroHP())
				{
					GSLOG(Error, TEXT("LocalPlayer is Dead!"));

					FGsGameObjectStateManager* fsm = local->GetBaseFSM();
					fsm->ProcessEvent(EGsStateBase::Dead);
				}

				/*if (auto component = Cast<UGsMovementDebugShapeComponent>(local->GetActor()->GetComponentByClass(UGsMovementDebugShapeComponent::StaticClass())))
				{
					component->SetServerDebugPos(pkt->HitPos());
				}*/
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::MoveStiffenNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_MOVE_STIFFEN_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_MOVE_STIFFEN_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* player = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				if (pkt->IsNoTimeLine())
				{
					if (FGsDamageHandlerBase* damageHandler = player->GetDamageHandler())
					{
						if (false == damageHandler->AddStiffen(pkt->SkillNotifyId(), pkt->ConstRefCurPos(), pkt->Duration()))
						{
							player->CastGameObject<UGsGameObjectCreature>()->OnStiffen(pkt->ConstRefCurPos(), pkt->Duration());
						}
					}
				}
				else
				{
					player->CastGameObject<UGsGameObjectCreature>()->OnStiffen(pkt->ConstRefCurPos(), pkt->Duration());
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SkillMoveLocation(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_SKILL_MOVE_LOCATION_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_SKILL_MOVE_LOCATION_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			UGsGameObjectBase* gameObject = mgr->FindObject(EGsGameObjectType::Creature, pkt->TargetId());
			if (nullptr == gameObject)
			{
				return;
			}

			const FGsSchemaSkillNotifySet* skillNotifySet =
				UGsTableUtil::FindRowById<UGsTableSkillNotifySet, FGsSchemaSkillNotifySet>(pkt->SkillNotifyId());
			if (nullptr == skillNotifySet || skillNotifySet->type != SkillNotifyType::MOVE_LOCATION)
			{
				return;
			}

			// 이건 따로 받아야 한다.
			const FGsSchemaSkillNotifyElement* element = skillNotifySet->notifyElementList[0].GetRow();
			const FGsSchemaSkillNotifyMoveLocation* movelocationTbl = element->typeInfo.GetRowByType<FGsSchemaSkillNotifyMoveLocation>();
			UGsGameObjectCreature* castObject = gameObject->CastGameObject<UGsGameObjectCreature>();
			if (movelocationTbl->moveSpeed != 0)
			{
				castObject->StartMoveLocation(pkt->ConstRefBeforePos(), pkt->ConstRefCurPos(), movelocationTbl->moveSpeed);
			}
			else
			{
				castObject->StartMoveLocation(pkt->ConstRefBeforePos(), pkt->ConstRefCurPos(), movelocationTbl->duration);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::ProjectileMoveToLocationNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_PROJECTILE_MOVE_TO_LOCATION_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_PROJECTILE_MOVE_TO_LOCATION_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::Projectile, pkt->GameId()))
			{
				if (FGsMovementHandlerBase* movemHandler = findObject->GetMovementHandler())
				{
					FGsMovementHandlerProjectile* castMoveHandler =
						FGsClassUtil::FCheckStaticCast<FGsMovementHandlerProjectile>(movemHandler);

					castMoveHandler->ReceveDesPos(pkt->ConstRefDestPos());
					castMoveHandler->ReceveSpeed(pkt->Speed());
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::MoveCorrectionPosNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_CORRECTION_POS_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_CORRECTION_POS_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			// 플레이어 까지만 검색
			if (UGsGameObjectBase* player = mgr->FindObject(EGsGameObjectType::Player, pkt->GameId()))
			{
				// 위치, 방향 동기화
				if (player->IsObjectType(EGsGameObjectType::RemotePlayer))
				{
					if (FGsMovementHandlerRemote* moveRemort = player->CastGameObject<
						UGsGameObjectCreature>()->GetCastMovement<FGsMovementHandlerRemote>())
					{
						moveRemort->ReceveMoveToLocation(pkt);
					}
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::RequestPosValidate(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_REQ_POS_VALIDATE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_REQ_POS_VALIDATE_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* local = mgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				if (FGsMovementHandlerLocalPlayer* moveHandler = local->CastGameObject<UGsGameObjectLocalPlayer>()->GetCastMovement<FGsMovementHandlerLocalPlayer>())
				{
					moveHandler->PositionValidateByServer(pkt->PosValidateSeq(), true, local->GetRotation().Vector(), pkt->ConstRefSrcPos());
				}

				if (auto component = Cast<UGsMovementDebugShapeComponent>(local->GetActor()->GetComponentByClass(UGsMovementDebugShapeComponent::StaticClass())))
				{
					component->SetLocation(pkt->ConstRefSrcPos());
					component->SetDestination(pkt->ConstRefDestPos());
					component->SetServerDebugPos(pkt->ConstRefCollisionPos());
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::ForcedPosNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_FORCED_POS_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_FORCED_POS_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* local = mgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				if (FGsMovementHandlerLocalPlayer* moveHandler = local->CastGameObject<UGsGameObjectLocalPlayer>()->GetCastMovement<FGsMovementHandlerLocalPlayer>())
				{
					moveHandler->PositionValidateByServer(pkt->PosValidateSeq(), false, FGsDir::angleToDir(pkt->CurDir()), pkt->ConstRefCurPos());
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::StartRotationNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_START_ROTATION_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_START_ROTATION_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* target = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				if (target->IsObjectType(EGsGameObjectType::NonPlayerBase))
				{
					UGsGameObjectCreature* castObject = target->CastGameObject<UGsGameObjectCreature>();
					FGsMovementHandlerRemote* movementHandler = castObject->GetCastMovement<FGsMovementHandlerRemote>();

					movementHandler->StartRotation(pkt);
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::CreatureSkillRootmotionStartNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_CREATURE_SKILL_ROOT_MOTION_START_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_CREATURE_SKILL_ROOT_MOTION_START_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* target = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				FGsMovementHandlerBase* movementHandler = target->GetMovementHandler();
				// 타 캐릭터 방향 위치 동기화
				if (false == target->IsObjectType(EGsGameObjectType::LocalPlayer))
				{
					FGsMovementHandlerRemote* rmoteMovementHandler =
						FGsClassUtil::FCheckStaticCast<FGsMovementHandlerRemote>(movementHandler);
					rmoteMovementHandler->ReceveStartRootmotion(target->GetObjectType(), pkt);
				}

				UGsGameObjectCreature* castObject = target->CastGameObject<UGsGameObjectCreature>();
				castObject->StartRootmotion(pkt->DestDir(), pkt->CurPos(), pkt->DestPos(), pkt->RootMotionId());
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::CreatureSkillRootmotionFinishNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_CREATURE_SKILL_ROOT_MOTION_FINISH_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_CREATURE_SKILL_ROOT_MOTION_FINISH_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* target = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				FGsMovementHandlerBase* movementHandler = target->GetMovementHandler();
				// 타 캐릭터 방향 위치 동기화
				if (false == target->IsObjectType(EGsGameObjectType::LocalPlayer) &&
					false == mgr->_isNotSyncRootmotion)
				{
					FGsMovementHandlerRemote* rmoteMovementHandler =
						FGsClassUtil::FCheckStaticCast<FGsMovementHandlerRemote>(movementHandler);
					rmoteMovementHandler->ReceveFinishRootmotion(target->GetObjectType(), pkt);
				}

				UGsGameObjectCreature* castObject = target->CastGameObject<UGsGameObjectCreature>();
				castObject->EndRootmotion(pkt->CurDir(), pkt->CurPos(), pkt->ReactionId());
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::LocalPlayerCollisionOn(const FGsNet::Packet& inPacket)
{
	SetLocalPlayerCollsionFlag(true);
}

void FGsNetMessageHandlerGameObject::LocalPlayerCollisionOff(const FGsNet::Packet& inPacket)
{
	SetLocalPlayerCollsionFlag(false);
}

void FGsNetMessageHandlerGameObject::DebugShapeAck(const FGsNet::Packet& inPacket)
{

	if (PD::SC::PKT_SC_DEBUG_SHAPE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DEBUG_SHAPE_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (mgr->_isCollisionDebugShape)
			{
				// 서버에서 적용된 컬러값 참고
				//  constexpr uint32 FRGBA(uint8 r, uint8 g, uint8 b, uint8 a = 0)
				//	{
				//		return ((b) | (g << 8) | (r << 16) | (a << 24));
				//	}
				float lifeTime = pkt->Duration() * 0.001f;
				switch ((DebugShapeType)pkt->ShapeType())
				{
				case DebugShapeType::FAN:
					UGsDebugUtil::DrawDebugFan(mgr->GetWorld(), pkt->Pos(), FGsDir::angleToDir(pkt->Dir()), pkt->Height(),
						pkt->Angle(), pkt->MinRadius(), pkt->MaxRadius(), FColor(pkt->ColorRGB()), lifeTime);
					break;
				case DebugShapeType::SPHERE:
					//DrawDebugSphere(GetWorld(), pkt->Pos(), pkt->MaxRadius(), 100, FColor(pkt->ColorRGB()), false, lifeTime);
					DrawDebugCircle(mgr->GetWorld(), pkt->Pos(), pkt->MaxRadius(), 100, FColor(pkt->ColorRGB()), false,
						lifeTime, 0, 0.f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));

					if (pkt->MinRadius() > 0.f)
					{
						DrawDebugCircle(mgr->GetWorld(), pkt->Pos(), pkt->MinRadius(), 100, FColor(pkt->ColorRGB()), false,
							lifeTime, 0, 0.f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
					}
					break;
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::DebugShapeLineAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_DEBUG_SHAPE_LINE_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_DEBUG_SHAPE_LINE_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (mgr->_isCollisionDebugShape)
			{
				TArray<FVector> listPos;
				for (int offset = 0; offset < pkt->PosSize(); offset += sizeof(FVector))
				{
					const FVector* pos = reinterpret_cast<const FVector*>(pkt->Pos() + offset);
					listPos.Emplace(*pos);
				}

				FBox minMax(listPos);
				FVector oriPos = minMax.GetCenter();
				// 순서 대로 그리기 위해서 각점들의 방향 각도에 따라 재정렬 시킨다.
				listPos.Sort([&oriPos](const FVector& lhs, const FVector& rhs)
					{
						return (lhs - oriPos).HeadingAngle() < (rhs - oriPos).HeadingAngle();
					});

				float lifeTime = pkt->Duration() * 0.001f;
				FColor color(pkt->ColorRGB());
				FVector startPos = listPos[0];
				for (int i = 1; i < listPos.Num(); ++i)
				{
					DrawDebugLine(mgr->GetWorld(), startPos, listPos[i], color, false, lifeTime);
					startPos = listPos[i];
				}
				DrawDebugLine(mgr->GetWorld(), startPos, listPos[0], color, false, lifeTime);
			}
		}
	}
}

//이동 DebugShape정보 (일단 포지션만 세팅합니다)
void FGsNetMessageHandlerGameObject::DebugShapeCylinderAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_DEBUG_SHAPE_CYLINDER_LIST_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DEBUG_SHAPE_CYLINDER_LIST_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* creature = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				if (AActor* actor = creature->GetActor())
				{
					if (auto component = Cast<UGsMovementDebugShapeComponent>(actor->GetComponentByClass(UGsMovementDebugShapeComponent::StaticClass())))
					{
						component->SetServerLocation(pkt->Pos());
					}
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::ObjectCollisionDebugShape(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_OBJECT_COLLISION_DEBUG_SHAPE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_OBJECT_COLLISION_DEBUG_SHAPE_READ*>(inPacket.Buffer))
	{
		GSGameObject()->_isCollisionDebugShape = pkt->Enable();
	}
}

void FGsNetMessageHandlerGameObject::IffDecugAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_IFF_DEBUG_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_IFF_DEBUG_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectBase* creature = GSGameObject()->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
		{
			bool isEnemy = UGsGameObjectUtil::IsEnemy(creature);
			FString message = UGsGameObjectUtil::IsEnemy(creature) ? TEXT("true") : TEXT("false");
			GSLOG(Error, TEXT("[SC_IFF_DEBUG] GameId : %I64d  IsEnemy : %s"), pkt->GameId(), *message);
		}
	}
}

#if	!UE_BUILD_SHIPPING
void FGsNetMessageHandlerGameObject::DebugTextInfoAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_DEBUG_TEXT_INFO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DEBUG_TEXT_INFO_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectBase* creature = GSGameObject()->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
		{
			if (UGsGameObjectNonPlayer* npc = Cast<UGsGameObjectNonPlayer>(creature))
			{
				npc->OnDebugTextInfo(pkt);
			}
		}
	}
}
#endif

void FGsNetMessageHandlerGameObject::DrawDebugCylinder(UWorld* inWorld, const FVector& inLocation, const FLinearColor& inColor)
{
	FVector halfH = FVector(0, 0, 95.0f);
	FVector floor = inLocation;
	UKismetSystemLibrary::DrawDebugCylinder(inWorld, floor, floor + 2 * halfH, 25.0f, 12, inColor, 0.0f, 0.0f);
}

void FGsNetMessageHandlerGameObject::EnterTerritoryAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ACK_ENTER_TERRITORY_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_ENTER_TERRITORY_READ*>(inPacket.Buffer))
	{
		GFence()->AddEnteredTerritory(pkt->TerritoryId());
	}
}

void FGsNetMessageHandlerGameObject::LeaveTerritoryAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ACK_LEAVE_TERRITORY_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_LEAVE_TERRITORY_READ*>(inPacket.Buffer))
	{
		GFence()->RemoveEnterTerritory(pkt->TerritoryId());
	}
}

void FGsNetMessageHandlerGameObject::StartSkillAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_START_SKILL_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_START_SKILL_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* creature = mgr->FindObject(EGsGameObjectType::Creature, pkt->CasterId()))
			{
				if (creature->IsObjectType(EGsGameObjectType::Player) == true)
				{
					if (FGsGameObjectDataPlayer* data = creature->GetCastData<FGsGameObjectDataPlayer>())
					{
						data->SetIsActiveSpiritShot(pkt->IsActiveSpiritShot());
						data->SetSpiritShotSumLevel(pkt->SpiritShotSumLevel());
					}
				}


				// 위치, 방향 동기화
				if (creature->GetObjectType() != EGsGameObjectType::LocalPlayer)
				{
					if (FGsMovementHandlerRemote* moveRemort = creature->CastGameObject<
						UGsGameObjectCreature>()->GetCastMovement<FGsMovementHandlerRemote>())
					{
						//moveRemort->SetNetMoveSpeed(pkt->Speed());
						moveRemort->ReceveStartSkill(creature->GetObjectType(), pkt);
					}

					// 매너모드 remote 스킬 수신 처리
					if (creature->GetObjectType() == EGsGameObjectType::RemotePlayer)
					{
						GSAI()->RecvStartSkillRemote(pkt->TargetId(), creature->GetGameId());
					}
				}
				else if (creature->GetObjectType() == EGsGameObjectType::LocalPlayer)
				{
					if (UGsGameObjectLocalPlayer::EMovementType::Spectator == Cast<UGsGameObjectLocalPlayer>(creature)->GetMovementType())
					{
						if (FGsMovementHandlerLocalPlayer* moveLocalPlayer = creature->CastGameObject<
							UGsGameObjectCreature>()->GetCastMovement<FGsMovementHandlerLocalPlayer>())
						{
							moveLocalPlayer->SpectatorReceveStartSkill(creature->GetObjectType(), pkt);
						}
					}

					// 매너모드 local 스킬 수신 처리					
					GSAI()->RecvStartSkillLocal(pkt->TargetId());
				}


				// 스킬 사용
				UGsGameObjectCreature* cast = creature->CastGameObject<UGsGameObjectCreature>();
				FGsSkillHandlerBase* skill = cast->GetSkillHandler();

				// PVP 컨텐츠종료시, 맵이동시 GameObjectLocal은 껍데기는 존재하고 내부 핸들러들이 삭제된다.
				// 따라서 nullptr 체크 필수
				if (nullptr != skill)
				{
					GSLOG(Error, TEXT("[SkillDebug] StartSkill Ack %d, %d"), pkt->SkillId(), pkt->SkillStartType());
					skill->NetSkillStart(pkt);

					if (creature->GetObjectType() == EGsGameObjectType::LocalPlayer
						&& UGsGameObjectLocalPlayer::EMovementType::Spectator == Cast<UGsGameObjectLocalPlayer>(cast)->GetMovementType())
					{
						skill->OnSkill(pkt->SkillId());
					}
				}

				//FVector dir = FGsDir::angleToDir(pkt->CurrDir());
				//DrawDebugDirectionalArrow(GetWorld(), pkt->ConstRefCurPos(), dir * 200.f + pkt->ConstRefCurPos(), 200.f,
				//	FColor::Red, false, 10.f);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SkillFinishedAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_SKILL_FINISHED_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SKILL_FINISHED_READ*>(inPacket.Buffer))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT(">> SC_SKILL_FINISHED SkillId : %d  Reson : %d"), pkt->SkillId(), pkt->SkillFinishReason());
#endif

		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* creature = mgr->FindObject(EGsGameObjectType::Creature, pkt->CasterId()))
			{
				// 위치, 방향 동기화
				if (creature->GetObjectType() != EGsGameObjectType::LocalPlayer)
				{
					if (FGsMovementHandlerRemote* moveRemort = creature->CastGameObject<
						UGsGameObjectCreature>()->GetCastMovement<FGsMovementHandlerRemote>())
					{
						//moveRemort->SetNetMoveSpeed(pkt->Speed());
						moveRemort->ReceveFinishSkill(creature->GetObjectType(), pkt);
				
					}

					GSLOG(Error, TEXT("[SkillDebug] SkillFinish Ack %d"), pkt->SkillId());
				}

				if (creature->GetObjectType() == EGsGameObjectType::LocalPlayer)
				{
					if (UGsGameObjectLocalPlayer::EMovementType::Spectator == Cast<UGsGameObjectLocalPlayer>(creature)->GetMovementType())
					{
						if (FGsMovementHandlerLocalPlayer* moveLocalPlayer = creature->CastGameObject<
							UGsGameObjectCreature>()->GetCastMovement<FGsMovementHandlerLocalPlayer>())
						{
							moveLocalPlayer->SpectatorReceveFinishSkill(creature->GetObjectType(), pkt);
						}
					}
				}

				// 스킬 사용
				UGsGameObjectCreature* cast = creature->CastGameObject<UGsGameObjectCreature>();
				FGsSkillHandlerBase* skillhandler = cast->GetSkillHandler();
				if (nullptr != skillhandler)
				{
					skillhandler->NetSkillEnd(pkt);
				}
				else
				{
#if WITH_EDITOR
					GSLOG(Error, TEXT(">> SkillFinishedAck skillHandler DoesNotExist"));
#endif
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SkillFailureNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ACK_START_SKILL_FAILURE_READ* pkt = reinterpret_cast<
		PD::SC::PKT_SC_ACK_START_SKILL_FAILURE_READ*>(inPacket.Buffer))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT(">> SC_ACK_START_SKILL_FAILURE SkillId : %d  Reson : %d"), pkt->SkillId(), pkt->Result());
#endif
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* local = mgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				if (local->IsVaildClass())
				{
					FGsSkillHandlerBase* skillHandler = local->GetSkillHandler();
					skillHandler->NetSkillFailure(pkt->SkillId(), (PD::Result)pkt->Result());

					int result = pkt->Result();

					if (PD::Result::SKILL_ERROR_NOT_USABLE_IN_SAFE_ZONE == (PD::Result)result)
					{
						//안전 지역에서는 공격스킬을 사용할 수 없습니다.
						FText findText;
						FText::FindText(TEXT("ContextUIText"), TEXT("pvpNotallowedText"), findText);
						FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
						return;
					}
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SkillDamagedNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_SKILL_DAMAGED_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SKILL_DAMAGED_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* casterObject = mgr->FindObject(EGsGameObjectType::Creature, pkt->CasterId()))
			{
				if (casterObject->IsObjectType(EGsGameObjectType::Player) == true)
				{
					if (FGsGameObjectDataPlayer* data = casterObject->GetCastData<FGsGameObjectDataPlayer>())
					{
						data->SetIsActiveSpiritShot(pkt->IsActiveSpiritShot());
						//data->SetSpiritShotSumLevel(pkt->SpiritShotSumLevel());
					}
				}
				else if (casterObject->IsObjectType(EGsGameObjectType::NonPlayerBase) == true)
				{
					// Nonplayer는 회전 동기화를 맞춰준다.
					if (FGsMovementHandlerRemote* moveRemort = casterObject->CastGameObject<
						UGsGameObjectCreature>()->GetCastMovement<FGsMovementHandlerRemote>())
					{
						moveRemort->ReceveSkillRotation(casterObject->GetObjectType(), pkt);
					}
				}
			}


			if (UGsGameObjectBase* hitObject = mgr->FindObject(EGsGameObjectType::Creature, pkt->TargetId()))
			{
				if (hitObject->IsVaildClass())
				{
					// 선판정 스킬 확인
					// 서버에서 미리 알려주면 좋을 듯
					const FGsSchemaSkillSet* skillsetData =
						UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(pkt->SkillId());
					if (skillsetData && skillsetData->noTimeline && DamageByType::SKILL == pkt->DamageByType())
					{
						if (FGsDamageHandlerBase* damageHandler = hitObject->GetDamageHandler())
						{
							damageHandler->Add(pkt);
						}
					}
					else
					{
						UGsGameObjectCreature* castObject = hitObject->CastGameObject<UGsGameObjectCreature>();
						castObject->NetSkillDamagedAck(pkt);
					}
				}
				else
				{
					// 이상황이 오면 안될것 같다 서버팀과 패킷 응답을 논의해야함
					GSLOG(Error, TEXT("GameObject Class Deleting..."));
				}
			}

			// 프로젝타일 히트 효과 처리 (mMediumId 값이 있으면 프로젝타일 정보)
			if (pkt->MediumId() != 0)
			{
				if (UGsGameObjectBase* projectile = mgr->FindObject(EGsGameObjectType::Projectile, pkt->MediumId()))
				{
					if (projectile->IsVaildClass())
					{
						projectile->CastGameObject<UGsGameObjectProjectile>()->NetSkillDamagedAck(pkt);
					}
					else
					{
						// 이상황이 오면 안될것 같다 서버팀과 패킷 응답을 논의해야함
						GSLOG(Error, TEXT("GameObject Class Deleting..."));
					}
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::PassivityDamageNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_PASSIVITY_DAMAGED_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_PASSIVITY_DAMAGED_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* hitObject = mgr->FindObject(EGsGameObjectType::Creature, pkt->TargetId()))
			{
				UGsGameObjectCreature* castObject = hitObject->CastGameObject<UGsGameObjectCreature>();
				castObject->NetSkillDamagedAck(pkt); // PassivityOwnerType::FAIRY 인 경우도 상관없이 hit 처리된다.
			}
		}
	}
}

//passivity 발동시 알림
void FGsNetMessageHandlerGameObject::PassivityInvokeNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_PASSIVITY_INVOKED_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_PASSIVITY_INVOKED_READ*>(inPacket.Buffer))
	{
		if (pkt->PassivityOwnerType() == PassivityOwnerType::FAIRY)
		{
			if (UGsGameObjectManager* mgr = GSGameObject())
			{
				if (UGsGameObjectBase* obj = mgr->FindObject(EGsGameObjectType::Player, pkt->CasterId()))
				{
					UGsGameObjectPlayer* castObj = obj->CastGameObject<UGsGameObjectPlayer>();
					if (FGsFairyHandlerPlayer* fairyHandler = castObj->GetFairyHandler())
						fairyHandler->SetFairySkillEffect();
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SocialActionNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ACK_SOCIAL_ACTION_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_SOCIAL_ACTION_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* obj = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				FGsSkillHandlerBase* skillHandler = obj->GetSkillHandler();
				
				if (nullptr == skillHandler)
					return;

				skillHandler->OnSocial(pkt->CommonActionId());

//#if WITH_EDITOR
//				FDateTime currentTime = FDateTime::UtcNow();
//				FString timeString = FDateTime::UtcNow().ToString(TEXT("%H:%M:%S.%f"));
//
//				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
//					*FString::Printf(TEXT("EGsGameObjectType::Creature (OnSocial) Player's GameId : %d / Npc Name : %s / Time : %s"), obj->GetData()->GetGameId(), *obj->GetData()->GetName(), *timeString));
//#endif
			}

			// [B1] | ejrrb10 | SocialActionNotify 를 뿌려주는 순간에서 이미 FGsGameFlow::Mode::GAME
			//// 로비->인게임 시에는 Warp, Fade 연출을 사용하지 않음(맵 전환 과정에서 지저분하게 보임)
			//// 로비->인게임 시에는 GameFlow가 인게임 판정나므로 FGsGameFlow::Mode::LOBBY가 아닌 GameMode로 검사
			//if (UWorld* world = GLevel()->GetWorld())
			//{
			//	if (AGameModeBase* gameMode = world->GetAuthGameMode())
			//	{
			//		if (gameMode->IsA<AGsGameModeLobby>())
			//		{
			//			return;
			//		}
			//	}
			//}

			const UGsTableSkillCommon* skillCommonTable = Cast<UGsTableSkillCommon>(FGsSchemaSkillCommon::GetStaticTable());
			if (nullptr == skillCommonTable)
			{
				return;
			}

			const FGsSchemaSkillCommon* skillCommonData = nullptr;
			if (false == skillCommonTable->FindRowById(pkt->CommonActionId(), skillCommonData))
			{
				return;
			}

			if (CommonActionType::TELEPORT == skillCommonData->category)
			{
				if (UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::Player, pkt->GameId()))
				{
					UGsGameObjectPlayer* playerObject = findObject->CastGameObject<UGsGameObjectPlayer>();

					if (nullptr == playerObject)
						return;

					// [B1] | ejrrb10 | 인게임에서 로비로 진입 시의 연출 여부 설정 :
					// 자신 및 타 플레이어의 연출을 하지 않음 (Ref : 린2M, 린W, 오딘)
					// 1. 자신에 대한 연출
					if (FGsGameFlowManager* gameFlowMng = GMode())
					{
						if (FGsGameFlow::Mode::LOBBY == gameFlowMng->GetCurrentFlowType())
						{
							return;
						}
					}

					// 2. 타인에 대한 연출
					if (false == playerObject->IsWarpEffectOn())
					{
						return;
					}
					else
					{
						// local not show in dead state
						if (playerObject->GetObjectType() == EGsGameObjectType::LocalPlayer)
						{
							if (FGsGameObjectStateManager* fsm = playerObject->GetBaseFSM())
							{
								// 파티던전 죽는 순간에 나가면 카메라 모드 warp에서 저장됨
								// Dying 상태에서도 카메라 on warp 처리 안함
								// https://jira.com2us.com/jira/browse/C2URWQ-5960
								if (true == fsm->IsState(EGsStateBase::Dead) ||
									true == fsm->IsState(EGsStateBase::Dying))
								{
									return;
								}
							}
						}

//#if WITH_EDITOR
//						FDateTime currentTime = FDateTime::UtcNow();
//						FString timeString = FDateTime::UtcNow().ToString(TEXT("%H:%M:%S.%f"));
//
//						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
//							*FString::Printf(TEXT("EGsGameObjectType::Player (OnWarp) Player's GameId : %d / Npc Name : %s / Time : %s"), playerObject->GetData()->GetGameId(), *playerObject->GetData()->GetName(), *timeString));
//#endif


						/*1. 서버 소셜 패킷(warp)
						2. onWarp
						3. game object state warp(애니메이션)
						4. state exit->LOAD_TOPO_END_WARP_EFFECT 메시지
						5. FGsSystemMessageHandlerGlobal::OnLoadTopoEndWarpEffect
						6. 서버 load to po start와 클라 애니 연출 끝이 동시 만족시 다음 레벨 로드*/
						playerObject->OnWarp();
					}
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SocialEmotionNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ACK_USER_SOCIAL_EMOTION_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_USER_SOCIAL_EMOTION_READ*>(inPacket.Buffer))
	{
		if (PACKET_RESULT_SUCCESS != pkt->Result())
		{
		}
		else
		{
			if (auto player = GSGameObject()->FindObject(EGsGameObjectType::Player, pkt->GameId()))
			{
				if (auto skillHandler = player->GetSkillHandler())
				{	
					skillHandler->PlaySocialEmotion(pkt->UserSocialEmotion());
				}
			}
		}
	}
}


void FGsNetMessageHandlerGameObject::SkillHealedNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_SKILL_HEALED_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SKILL_HEALED_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* hitObject = mgr->FindObject(EGsGameObjectType::Creature, pkt->TargetId()))
			{
				// NotifyTickOffset값이 있으면 딜레이 처리
				if (false == FMath::IsNearlyZero(pkt->NotifyTickOffset()))
				{
					if (FGsDamageHandlerBase* damageHandler = hitObject->GetDamageHandler())
					{
						damageHandler->Add(pkt);
					}
				}
				else
				{
					UGsGameObjectCreature* castObject = hitObject->CastGameObject<UGsGameObjectCreature>();
					castObject->NetSkillHealAck(pkt);
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::SkillEffectNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_SKILL_EFFECT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SKILL_EFFECT_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* creature = mgr->FindObject(EGsGameObjectType::Creature, pkt->CasterId()))
			{
				FGsSkillHandlerBase* skillHandler = creature->GetSkillHandler();
				skillHandler->NetSkillNotifyAction(pkt);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::UpsertAbnormalityNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_UPSERT_ABNORMALITY_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_UPSERT_ABNORMALITY_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* targetObject = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				if (FGsAbnormalityHandlerBase* abnormmalityMgr = targetObject->GetAbnormalityHandler())
				{
					abnormmalityMgr->Add(pkt->ConstRefAbnormalityData());
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::RemoveAbnormalityNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_REMOVE_ABNORMALITY_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_REMOVE_ABNORMALITY_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* targetObject = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				if (FGsAbnormalityHandlerBase* abnormmalityMgr = targetObject->GetAbnormalityHandler())
				{
					abnormmalityMgr->Remove(pkt->AbnormalityInstanceId());
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::FailedAddAbnormalityAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_FAILED_ADD_ABNORMALITY_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_FAILED_ADD_ABNORMALITY_READ*>(inPacket.Buffer))
	{
		FString reason = FGsTextUtil::GetEnumValueAsString<PD::Result>(TEXT("Result"), (PD::Result)pkt->Result());
#if WITH_EDITOR
		GSLOG(Error, TEXT("PKT_SC_FAILED_ADD_ABNORMALITY_READ Reson : %s (%d)"), *reason, pkt->Result());
#endif
	}
}

void FGsNetMessageHandlerGameObject::SetAbnormalityNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ABNORMALITY_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ABNORMALITY_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* findobject = GSGameObject()->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
			{
				if (FGsAbnormalityHandlerBase* abnormmalityMgr = findobject->GetAbnormalityHandler())
				{
					abnormmalityMgr->Set(pkt);
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::AbnormalityResultNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ADD_ABNORMALITY_RESULT_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ADD_ABNORMALITY_RESULT_READ*>(inPacket.Buffer))
	{
		// 현재는 상태이상 이뮨처리에 대한 결과값 정보만 오는것으로 알고 있음
		// https://jira.gamevilcom2us.com/wiki/pages/viewpage.action?pageId=251233548
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* hitObject = mgr->FindObject(EGsGameObjectType::Creature, pkt->TargetId()))
			{
				UGsGameObjectCreature* castObject = hitObject->CastGameObject<UGsGameObjectCreature>();
				castObject->NetAbnormalityResultAck(pkt);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::PassivityAddedNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_PASSIVITY_ADDED_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_PASSIVITY_ADDED_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
		{
			// LocalPlayer에게만 오는 패킷
			if (UGsGameObjectBase* local = gameObjectMgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				// GameObject가 생성된 상태라면 Handler클래스는 무조건 유효해야 한다. (예외처리 X)
				FGsAbnormalityHandlerBase* abnormmalityMgr = local->GetAbnormalityHandler();
				abnormmalityMgr->Set(pkt);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::PassivityRemovedNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_PASSIVITY_REMOVED_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_PASSIVITY_REMOVED_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
		{
			// LocalPlayer에게만 오는 패킷
			if (UGsGameObjectBase* local = gameObjectMgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				// Handler클래스는 무조건 유효해야 한다. (예외처리 X)
				FGsAbnormalityHandlerBase* abnormmalityMgr = local->GetAbnormalityHandler();
				abnormmalityMgr->Set(pkt);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::BehaviorStatusAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_BEHAVIOR_STATUS_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_BEHAVIOR_STATUS_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* gameObject = mgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				if (FGsAbnormalityHandlerBase* abnormalityhandler = gameObject->GetAbnormalityHandler())
				{
					if (FGsCrowdControlBase* crowdContol = abnormalityhandler->GetCrowdControl())
					{
						FGsCrowdControlLocalPlayer* castCrowdContol = static_cast<FGsCrowdControlLocalPlayer*>(crowdContol);
						castCrowdContol->NetBehaviorStatusAck(pkt);
					}
				}
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::CreatureDeathAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_CREATURE_DEATH_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_CREATURE_DEATH_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* creature = mgr->FindObject(EGsGameObjectType::Creature, pkt->DeadGameId()))
			{
				creature->NetDeathAck(pkt);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::UserResurrectAck(const FGsNet::Packet& inPacket)
{
	// M2 : 무덤 부활 방식(ResurrectType::GRAVE)에선 이 패킷에 들어오지 않는다.
	PD::SC::PKT_SC_USER_RESURRECT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_USER_RESURRECT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		if (UGsGameObjectBase* findobject = mgr->FindObject(EGsGameObjectType::Creature, pkt->GameId()))
		{
			UGsGameObjectCreature* creature = findobject->CastGameObject<UGsGameObjectCreature>();
			creature->OnRevive();

			//if player, close revive ui
			if (findobject->IsA(UGsGameObjectLocalPlayer::StaticClass()))
			{
				// 부활 UI 닫기 처리
				GUI()->CloseByKeyName(POPUP_REVIVE_NAME);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::ResurectFailureAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_RESURRECT_FAILURE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_RESURRECT_FAILURE_READ*>(inPacket.Buffer))
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Reason()));

		FString reason = FGsTextUtil::GetEnumValueAsString<PD::Result>(TEXT("Result"), (PD::Result)pkt->Reason());
#if WITH_EDITOR
		GSLOG(Log, TEXT("PKT_SC_RESURRECT_FAILURE_READ Reason : %s"), *reason);
#endif
	}

	// 실패했으므로 닫지 않고 버튼의 락만 푼다
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(POPUP_REVIVE_NAME);
	if (widget.IsValid())
	{
		if (UGsUIPopupRevive* popup = Cast<UGsUIPopupRevive>(widget.Get()))
		{
			popup->ClearLockAll();
		}
	}
}

void FGsNetMessageHandlerGameObject::UserAutoResurrectAck(const FGsNet::Packet& inPacket)
{
	// M2 : 무덤 부활 방식(ResurrectType::GRAVE)에선 이 패킷에 들어오지 않는다.
	PD::SC::PKT_SC_USER_RESURRECT_TOWN_AUTO_RESPAWN_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_USER_RESURRECT_TOWN_AUTO_RESPAWN_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	UGsUIWidgetBase* widget = GUI()->OpenAndGetWidget(POPUP_REVIVE_NAME).Get();
	UGsUIPopupRevive* reviveWidget = Cast<UGsUIPopupRevive>(widget);
	if (reviveWidget)
	{
		//자동 부활시 사망 부활 팝업창 띄운다
		reviveWidget->ShowRevivePopup(true);
		// 길드 던전 퇴장 텍스트 세팅
		reviveWidget->ShowGuildDungeonKickRemainTimeText(false);
	}

	if (FGsSaveBatteryManager* saveBattery = GSSaveBattery())
	{
		if (saveBattery->IsReStart())
		{
			GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_SAVE_BATTERY);
		}
	}

	GSLOG(Log, TEXT("%s"), TEXT("Revive Debug - Auto resurrect"));
}

void FGsNetMessageHandlerGameObject::ChangeWeaponTypeFailureAck(const FGsNet::Packet& inPacket)
{
	GItem()->SetIsRecvDiffEquipTypeProcPacket(true);

	if (PD::SC::PKT_SC_ACK_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FAILURE_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ACK_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FAILURE_READ*>(inPacket.Buffer))
	{
		int result = pkt->Result();
		if (PD::Result::CHANGE_WEAPON_TYPE_ERROR_IN_CHANGE_CLASS == (PD::Result)result)
		{
			// 아이템 장착에 실패하였습니다. 다시 시도해주세요.
			FText findText;
			FText::FindText(TEXT("UICommonText"), TEXT("EquipFailure"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			return;
		}
		else if (result == PD::Result::ITEM_ERROR_INVALID_USER_STATUS) // 상태이상 상태일때 내려오는 errorID
		{
			// 스턴 상태일때 불가능 합니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		else if (result == PD::Result::CHANGE_WEAPON_TYPE_ERROR_COOLDOWN_TIME) // 무기 변경 쿨타임 도중에 내려오는 errorID
		{
			// 아직 무기를 변경할 수 없습니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("Notice_WeaponChangeCooltime"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		else if (result == PD::Result::PRESET_ERROR_COOLDOWN_TIME)
		{
			// 아직 무기를 변경할 수 없습니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("Notice_WeaponChangeCooltime"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}


		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));

		// 시작전에만 오는 패킷으로 알고 있다.
		// 시작전에만 온다면 처리해줄게 없음
		//if (UGsGameObjectManager* mgr = GSGameObject())
		//{
		//	if (UGsGameObjectBase* local = mgr->FindObject(EGsGameObjectType::LocalPlayer))
		//	{
		//		UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();
		//		castLocal->NetChangeWeaponFinishedAck(nullptr);
		//		/*if (FGsGameObjectStateManager* fsm = local->GetBaseFSM())
		//		{
		//			fsm->ProcessEvent(EGsStateBase::CommonActionEnd);
		//		}*/
		//	}
		//}
	}
}

void FGsNetMessageHandlerGameObject::ChangeWeaponTypeStartNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_START_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_START_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* obj = mgr->FindObject(EGsGameObjectType::Player, pkt->GameId()))
			{
				GSStatChangeNotify()->SetPreWeaponType();

				UGsGameObjectPlayer* castObj = obj->CastGameObject<UGsGameObjectPlayer>();
				castObj->NetChangeWeaponStartAck(pkt->CreatureWeaponType());
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::ChangeWeaponTypeFinishedNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* obj = mgr->FindObject(EGsGameObjectType::Player, pkt->GameId()))
			{
				if (obj->IsObjectType(EGsGameObjectType::LocalPlayer))
				{
					GSkill()->NetChangeWeaponFinishedAck(pkt);

					FGsCoolTimeManager* coolTimeMgr = GCoolTime();
					coolTimeMgr->SetCoolTime(EGsCoolTime::Preset, 0, 0, pkt->PresetChangeCoolDown(), true);

					// 0초 상태일때 패킷 받아서 처리해버리면 기존에 돌고 있던 쿨타임이 강제로 0초로 리셋 처리 되므로
					// 시간이 있는 경우에만 보냄
					if (0.f < pkt->ClassWeaponChangeCoolDown())
					{
						coolTimeMgr->SetCoolTime(EGsCoolTime::Change_Weapon, 0, 0, pkt->ClassWeaponChangeCoolDown(), true);
					}
				}

				UGsGameObjectPlayer* castObj = obj->CastGameObject<UGsGameObjectPlayer>();
				castObj->NetChangeWeaponFinishedAck(pkt);				
			}
		}
	}

	GItem()->SetIsRecvDiffEquipTypeProcPacket(true);
}

void FGsNetMessageHandlerGameObject::UserWeightInfoNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_WEIGHT_INFO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_WEIGHT_INFO_READ*>(inPacket.Buffer))
	{
		//GSLOG(Warning, TEXT("PKT_SC_WEIGHT_INFO_READ - Grade : %d"), (uint8)pkt->Grade());
		GItem()->UpdateWeightPacketData((uint8)pkt->Grade());
	}
}

void FGsNetMessageHandlerGameObject::SpawnDropObject(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SPAWN_DROP_OBJECT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SPAWN_DROP_OBJECT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (GSDropObject())
	{
		GSDropObject()->SpawnDropObject(pkt);
	}
}

void FGsNetMessageHandlerGameObject::DespawnDropObject(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_DESPAWN_DROP_OBJECT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DESPAWN_DROP_OBJECT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (GSDropObject())
	{
		GSDropObject()->DespawnDropObject(pkt);
	}
}

//If drop object's ownership expired, everybody can loot drop object
void FGsNetMessageHandlerGameObject::DropObjectOwnershipExpired(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_DROP_OBJECT_OWNERSHIP_EXPIRED_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DROP_OBJECT_OWNERSHIP_EXPIRED_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (GSDropObject())
	{
		GSDropObject()->OwnershipExpired(pkt);
	}
}

void FGsNetMessageHandlerGameObject::LootDropObjectFail(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_DROP_OBJECT_LOOT_FAILURE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_DROP_OBJECT_LOOT_FAILURE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		int result = pkt->Result();
		bool isSuccess = false;
		if (PD::Result::DROP_OBJECT_LOOT_GUILD_STORE_OWNERSHIP == (PD::Result)result)
		{
			// 나에게 아이템이 들어오지 않았지만 기사단 소유로 습득 성공. 따라서 드랍 오브젝트 상태를 Restore하지 않습니다.
			isSuccess = true;
		}

		if (false == isSuccess)
		{
			// 드랍 오브젝트 상태 Restore
			if (UGsGameObjectBase* targetObject = mgr->FindObject(EGsGameObjectType::DropObject, pkt->DropObjectGameId()))
			{
				UGsGameObjectDropObject* dropObject = targetObject->CastGameObject<UGsGameObjectDropObject>();
				dropObject->SetLooted(false);

				int32 ownedItemTotalWeight = GItem()->GetOwnedItemWeight();
				float weightPercent = FGsItemHelper::GetWeightPercent(ownedItemTotalWeight);
				GSLOG(Error, TEXT("DropObject loot fail - result : %d\titem id : %d\tweight : %d"), (int32)result, dropObject->GetItemId(), weightPercent);
			}

			if (PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL == (PD::Result)result)
			{
				FText errorText;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), errorText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
			}
			else
			{
				FString errorMsgKey = PD::ResultEnumToString((PD::Result)result);
				FText errorText;
				if (FText::FindText(TEXT("NetText"), *errorMsgKey, errorText))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
				}

				GSLOG(Error, TEXT("LootDropObjectFail - key : %s\tmsg : %s"), *errorMsgKey, *errorText.ToString());
			}
		}
	}
	// auto play시 drop item 있을때 멍때리고 있는 이슈 대응
	// 루팅 실패시 ai에서 서버 응답대기 건거 해지
	if (FGsAIManager* aiManager = GSAI())
	{
		aiManager->GetContentsConditionDefault()->SetIsLootServerResponseWait(false);
	}
}

void FGsNetMessageHandlerGameObject::SkillSlotClearAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_SKILL_SLOT_CLEAR_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_SKILL_SLOT_CLEAR_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	GSkill()->NetAckSkillSlotClear(pkt);
}

void FGsNetMessageHandlerGameObject::SkillSlotSetAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_SKILL_SLOT_SET_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_SKILL_SLOT_SET_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (static_cast<int>(PACKET_RESULT_SUCCESS) != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	UGsSkillManager* skillMgr = GSkill();
	if (nullptr == skillMgr)
	{
		return;
	}

	CreatureWeaponType weaponType = pkt->WeaponType();

	TArray<SkillSlotInfo> updatedSlotList;
	pkt->MakeSkillSlotUpdatedVector<TArray<SkillSlotInfo>, SkillSlotInfo>(updatedSlotList);

	// 주의: 순서 보장해서 처리하기 위해 인덱스로 for문 처리(지우기부터 진행 후 덮어쓰기 처리 위함)
	for (int32 i = 0; i < updatedSlotList.Num(); ++i)
	{
		skillMgr->NetAckSkillSlotSet(updatedSlotList[i], weaponType);
	}
}

void FGsNetMessageHandlerGameObject::SkillSlotAutoAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_SKILL_SLOT_AUTO_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_SKILL_SLOT_AUTO_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (static_cast<int>(PACKET_RESULT_SUCCESS) != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());

		// 이 경우는 발생할 수 있는 경우로 무시 함
		if (PD::Result::SKILL_SLOT_ERROR_SAME_AUTO_FLAG == result)
		{
			// 로그만 찍어둠
			FGsUIHelper::LogNetError(result);

			// UI가 꼬이지 않게 현재 그대로 갱신함
			if (const FGsSkill* skill = GSkill()->FindSkill(pkt->ActiveSkillId(), pkt->WeaponType()))
			{
				FGsGameObjectMessageParamSkillSlot param(pkt->WeaponType(), skill->GetSlotId(), skill);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT_AUTO, &param);
			}

			return;
		}
		else if (PD::Result::SKILL_SLOT_ERROR_NOT_RESIST_SLOT == result)
		{
			// TEXT: 등록되지 않은 스킬입니다
			FText textMsg;
			FText::FindText(TEXT("SkillUIText"), TEXT("Notice_NotRegistration"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// 주의: Result에 Error를 준 상황에선 mAutoUse값을 무조건 false로 내려주므, Error일 경우 값을 덮어쓰면 안됨
	GSkill()->NetAckSkillSlotAuto(pkt);
}

void FGsNetMessageHandlerGameObject::SkillLearn(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_SKILL_LEARN_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SKILL_LEARN_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		GSkill()->NetRecvSkillLearn(pkt);
		SkillId newSkillId = pkt->ActiveSkillId();
		
		// 코스튬 스킬인지 체크
		CostumeId costumeId = FGsSkillHelper::GetCostumeIdBySkillId(newSkillId);
		if (INVALID_COSTUME_ID == costumeId)
		{
			// 스킬 획득 티커 출력(큐에 넣음)
			GUI()->PushQueue(TEXT("TrayAcquireSkill"), 0, [newSkillId](UGsUIWidgetBase* InWidget)
				{
					if (UGsUITrayAcquireSkill* tray = Cast<UGsUITrayAcquireSkill>(InWidget))
					{
						tray->SetSkill(newSkillId);
					}
				});

			// 스킬북 아이템의 Learn마크 갱신
			if (FGsSkillUIDataHandler* skillDataHandler = GSkill()->GetSkillUIDataHandler())
			{
				if (const TArray<ItemId>* skillBookListPtr = skillDataHandler->GetExistSkillBookList(newSkillId))
				{
					if (UGsItemManager* itemMgr = GItem())
					{
						for (const ItemId& skillBookItemId : *skillBookListPtr)
						{
							itemMgr->UpdateSkillBookState(skillBookItemId);
						}
					}
				}
			}
		}
		else
		{
			// 코스튬 스킬 획득 티커 출력(큐에 넣음)
			GUI()->PushQueue(TEXT("TrayAcquireCostumeSkill"), 0, [newSkillId, costumeId](UGsUIWidgetBase* InWidget)
				{
					if (UGsUITrayAcquireCostumeSkill* tray = Cast<UGsUITrayAcquireCostumeSkill>(InWidget))
					{
						tray->SetData(newSkillId, costumeId);
					}
				});
		}		

		// 스킬 획득 연출
		if (UGsGameObjectBase* findLocal = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* castLocal = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
			if (castLocal)
			{
				castLocal->OnLearnSkill(newSkillId);
			}
		}

		// HUD 레드닷 갱신
		if (FGsMessageHolder* msgMgr = GMessage())
		{
			msgMgr->GetHudRedDot().SendMessage(MessageContentHUDRedDot::SKILL, false);
			msgMgr->GetUI().SendMessage(MessageUI::SKILL_UPDATE_REDDOT, nullptr);
		}

		// 튜토리얼 체크 위한 메시지
		FGsTutorialMsgParamSkill param(newSkillId);
		GMessage()->GetTutorial().SendMessage(MessageContentTutorial::LEARN_SKILL, &param);
	}
}

void FGsNetMessageHandlerGameObject::SkillEnchantAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_ACK_SKILL_ENCHANT_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_SKILL_ENCHANT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));

		// 결과 갱신을 위해 보낸 후 리턴
		FGsUIMsgParamSkillEnchantResult param(pkt->EnchantResult(), pkt->ActiveSkillId(), pkt->ActiveSkillId());
		GMessage()->GetUI().SendMessage(MessageUI::SKILL_ENCHANT_EFFECT_RESULT, &param);

		return;
	}

	// 강화된 스킬정보 저장(강화 실패 시 함수 내부에서 리턴처리 함)
	GSkill()->NetAckSkillEnchant(pkt);

	// 결과 갱신
	if (FGsMessageHolder* msgMgr = GMessage())
	{
		FGsUIMsgParamSkillEnchantResult param(pkt->EnchantResult(), pkt->ActiveSkillId(), pkt->ActiveSkillId());
		msgMgr->GetUI().SendMessage(MessageUI::SKILL_ENCHANT_EFFECT_RESULT, &param);
	}
}

void FGsNetMessageHandlerGameObject::SkillUpdateActiveAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::SC::PKT_SC_UPDATE_ACTIVE_SKILL_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_UPDATE_ACTIVE_SKILL_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GSkill()->NetUpdateActiveSkill(pkt);
}

void FGsNetMessageHandlerGameObject::SkillBonusLevelNotify(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SKILL_BONUS_LEVEL_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SKILL_BONUS_LEVEL_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 패시비티 등에 의해 스킬 보너스 레벨이 변경될 경우 저장
	GSkill()->NetSkillBonusLevelNotify(pkt);

	// 결과 갱신을 위해 보낸 후 리턴
	FGsPrimitiveUInt32 paramMapId(pkt->ActiveSkillId());
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_UPDATE_SKILL_BONUS_LEVEL, &paramMapId);
}

void FGsNetMessageHandlerGameObject::CostumeSkillLearnAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_COSTUME_SKILL_LEARN_READ* pkt = 
		reinterpret_cast<PD::SC::PKT_SC_ACK_COSTUME_SKILL_LEARN_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	// 스킬 데이터쪽 갱신
	GSkill()->NetCostumeSkillLearn(pkt->ActiveSkillId());
	// 코스튬 데이터 쪽 갱신
	GSCostume()->SetCostumeItemIsLearnSkill(pkt->CostumeId(), true);

	// HUD 레드닷 갱신
	if (FGsMessageHolder* msgMgr = GMessage())
	{
		msgMgr->GetHudRedDot().SendMessage(MessageContentHUDRedDot::SKILL, false);
		msgMgr->GetUI().SendMessage(MessageUI::SKILL_UPDATE_REDDOT, nullptr);
	}
}

void FGsNetMessageHandlerGameObject::CostumeSkillSlotSetAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_COSTUME_SKILL_SLOT_SET_READ* pkt = 
		reinterpret_cast<PD::SC::PKT_SC_ACK_COSTUME_SKILL_SLOT_SET_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	UGsSkillManager* skillMgr = GSkill();
	if (nullptr == skillMgr)
	{
		return;
	}
		
	TArray<SkillSlotInfo> updatedSlotList;
	pkt->MakeSkillSlotUpdatedVector<TArray<SkillSlotInfo>, SkillSlotInfo>(updatedSlotList);

	// 제거하거나 새로 넣는 경우는 1개, 바꾸는 상황이면 2개(기존스킬 제거, 새스킬 할당)가 온다
	bool isChangeSkill = (1 < updatedSlotList.Num()) ? true : false;

	// 주의: 순서 보장해서 처리하기 위해 인덱스로 for문 처리(지우기부터 진행 후 덮어쓰기 처리 위함)
	for (int32 i = 0; i < updatedSlotList.Num(); ++i)
	{
		SkillSlotInfo& slotInfo = updatedSlotList[i];
		
		// 제거
		if (INVALID_SKILL_SLOT_ID == slotInfo.mSkillSlotId)
		{
			// 스킬을 바꾸는 상황인 경우, 해제 메시지는 찍지 않은다
			if (false == isChangeSkill)
			{
				// TEXT: 코스튬 스킬을 해제했습니다
				FText textMsg;
				if (FText::FindText(TEXT("SkillUIText"), TEXT("Notice_CostumeSkill_Removed"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}
			}
		}
		else
		{
			// TEXT: 코스튬 스킬을 장착했습니다
			FText textMsg;
			if (FText::FindText(TEXT("SkillUIText"), TEXT("Notice_CostumeSkill_Equipped"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}

		skillMgr->NetAckSkillSlotSet(updatedSlotList[i], CreatureWeaponType::MAX);
	}
}

void FGsNetMessageHandlerGameObject::AckExchangeCostume(const FGsNet::Packet& inPacket)
{
	if (PKT_SC_ACK_CHANGE_COSTUME_READ* pkt = reinterpret_cast<PKT_SC_ACK_CHANGE_COSTUME_READ*>(inPacket.Buffer))
	{
		GSLOG(Warning, TEXT("AckExchangeCostume"));

		auto result = pkt->Result();
		if (result == PACKET_RESULT_SUCCESS && GSkill())
		{
			GSkill()->NetCostumeSkillRemoved(pkt);
		}
	}
}

void FGsNetMessageHandlerGameObject::SkillUseSkillCycleAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_USE_SKILL_INTERVAL_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_USE_SKILL_INTERVAL_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());

		// 같은 정보 두번 요청한 경우.굳이 표시할 필요 없어서 로그만 찍고 리턴
		if (PD::Result::SKILL_SLOT_ERROR_SAME_INTERVAL_VALUE == result)
		{
			FGsUIHelper::LogNetError(result);
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	GSkill()->NetUseSkillCylceAck(pkt);

	FGsGameObjectMessageParamSkill param(nullptr, pkt->ActiveSkillId());
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_SKILL_CYCLE, &param);
}

void FGsNetMessageHandlerGameObject::SpawnPropAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SPAWN_PROP_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SPAWN_PROP_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	UGsGameObjectManager* mgr = GSGameObject();
	if (nullptr == mgr)
	{
		return;
	}

	//Quest 연출로 인해 SpawnID를 통해 Visible 정보를 얻어온다.
	int64 gameId = pkt->GameId();
	int64 spawnId = pkt->PropSpawnId();
	int32 tId;
	bool initVisible = true;
	const FGsSchemaSpawnElement* spawnData = mgr->FindVisibleSpawnInfo(EGsGameObjectType::PropInteraction, spawnId);
	if (nullptr == spawnData)
	{
		tId = pkt->PropTemplateId();
	}
	else
	{
		tId = spawnData->CreatureId;
		initVisible = spawnData->InitVisible;
	}

	const FGsSchemaPropData* propData = UGsTableUtil::FindRowById<UGsTablePropData, FGsSchemaPropData>(tId);
	if (nullptr == propData)
	{
		FString log = TEXT("Prop data missing - id : ") + FString::FromInt(tId);
		GSLOG(Error, TEXT("%s"), *log);
		return;
	}

	FGsSpawnParamProp param(EGsGameObjectType::PropInteraction, gameId, tId, pkt->Pos(), pkt->Dir(), initVisible, pkt->IsExActive());
	UGsGameObjectBase* spawn = mgr->SpawnObject(&param);
	if (nullptr == spawn)
	{
		FString log = TEXT("Prop shape missing - id : ") + FString::FromInt(param._tId);
		GSLOG(Error, TEXT("%s"), *log);
		return;
	}

	if (spawnData)
	{
		spawn->SetVisibleInfo(spawnData);
	}

	UGsGameObjectPropInteraction* castObject = spawn->CastGameObject<UGsGameObjectPropInteraction>();
	if (nullptr == castObject)
	{
		FString log = TEXT("Prop interaction casting fail - id : ") + FString::FromInt(param._tId);
		GSLOG(Error, TEXT("%s"), *log);
		return;
	}

	castObject->SetPropSpawnState(pkt);

	FGsGameObjectData* gameObjectData = castObject->GetData();
	gameObjectData->SetTerritoryType(pkt->TerritoryType());
}

void FGsNetMessageHandlerGameObject::DespawnPropAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_DESPAWN_PROP_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_DESPAWN_PROP_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	UGsGameObjectManager* manager = GSGameObject();
	if (nullptr == manager)
	{
		return;
	}

	FString reason = FGsTextUtil::GetEnumValueAsString<SPAWN_REASON>(TEXT("SPAWN_REASON"), pkt->Reason());
	UGsGameObjectBase* findObject = manager->FindObject(EGsGameObjectType::PropInteraction, pkt->GameId());
	if (nullptr == findObject)
	{
		GSLOG(Error, TEXT("PKT_SC_DESPAWN_PROP_READ - game id : %lld \treason : %s"), pkt->GameId(), *reason);
		return;
	}

	UGsGameObjectPropInteraction* propObject = findObject->CastGameObject<UGsGameObjectPropInteraction>();
	if (nullptr == propObject)
	{
		return;
	}

	const FGsSchemaPropData* propData = propObject->GetPropTableData();
	if (nullptr == propData)
	{
		return;
	}

	FGsAIReserveParam aiReserveClearParam(EGsAIActionType::INTERACTION, findObject);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_JOB, &aiReserveClearParam);

	manager->DespawnObject(findObject);
}

void FGsNetMessageHandlerGameObject::UpdatePropAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_UPDATE_PROP_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_UPDATE_PROP_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::PropInteraction, pkt->PropGameId());
		if (nullptr == findObject)
		{
			GSLOG(Error, TEXT("PKT_SC_UPDATE_PROP_READ - game id : %d"), pkt->PropGameId());
			return;
		}

		UGsGameObjectPropTouch* propTouch = Cast<UGsGameObjectPropTouch>(findObject);
		if (nullptr == propTouch)
		{
			GSLOG(Error, TEXT("PKT_SC_UPDATE_PROP_READ - game id : %d"), pkt->PropGameId());
			return;
		}

		propTouch->UpdatePropState(pkt);
	}
}

void FGsNetMessageHandlerGameObject::ActiveInvasionPropAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_INVADE_PORTAL_ACTIVE_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_INVADE_PORTAL_ACTIVE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	//if (UGsGameObjectManager* mgr = GSGameObject())
	//{
	//	if (auto propTouch = Cast<UGsGameObjectPropTouchInvasion>(mgr->FindObject(EGsGameObjectType::PropInteractInvasion, pkt->PortalGameId())))
	//	{
	//		propTouch->SetAciveCallbackInvasion(true);
	//	}
	//	else
	//	{
	//		GSLOG(Error, TEXT("PKT_SC_INVADE_PORTAL_ACTIVE_READ - game id : %d"), pkt->PortalGameId());
	//	}
	//}
}

void FGsNetMessageHandlerGameObject::InteractPropInvasionAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_CLICK_PROP_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_CLICK_PROP_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	UGsGameObjectPropTouchInvasion* invasionProp = nullptr;
	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		invasionProp = Cast<UGsGameObjectPropTouchInvasion>(mgr->FindObject(EGsGameObjectType::PropInteractInvasion, pkt->PropGameId()));
		if (auto local = Cast<UGsGameObjectLocalPlayer>(mgr->FindObject(EGsGameObjectType::LocalPlayer)))
		{
			if (FGsInteractionHandlerBase* interationHandler = local->GetInteractionHandler())
				interationHandler->CancelInteration(invasionProp);
		}
	}

	int32 result = pkt->Result();

	// 시작 실패시 켄슬 처리
	if (PACKET_RESULT_SUCCESS == result)
	{
		if (invasionProp) invasionProp->SetInvasionInteractEnd(true);
	}
	else
	{
		FGsInteractionManager::InteractionErrorMessage((PD::Result)result);
		if (invasionProp) invasionProp->SetInvasionInteractEnd(false);
	}
}

void FGsNetMessageHandlerGameObject::InteractQuestPropStartAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_INTERACT_QUEST_PROP_START_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_INTERACT_QUEST_PROP_START_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	int32 result = pkt->Result();
	// 시작 실패시 켄슬 처리
	if (PACKET_RESULT_SUCCESS == result)
	{
#if PROP_INTERACTION_DEBUG
		GSLOG(Log, TEXT("prop debug - quest prop interaciton start - gmae id : %lld"), pkt->PropGameId());
#endif
		return;
	}

	if (UGsGameObjectManager* manager = GSGameObject())
	{
		UGsGameObjectBase* propObject = manager->FindObject(EGsGameObjectType::PropInteraction, pkt->PropGameId());
		if (UGsGameObjectBase* findLocal = manager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* castLocal = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
			FGsInteractionHandlerBase* interationHandler = castLocal->GetInteractionHandler();
			// Handler클래스는 무조건 유효해야 한다. (예외처리 X)
#if PROP_INTERACTION_DEBUG
			FVector sourceLocation;
			FVector destLocation;

			UGsGameObjectBase* target = interationHandler->GetInteractionTarget();
			if (target)
			{
				if (AActor* actor = target->GetActor())
				{
					destLocation = actor->GetActorLocation();
					sourceLocation = UGameplayStatics::GetPlayerCharacter(actor->GetWorld(), 0)->GetMovementComponent()->GetActorFeetLocation();
					float dist = (destLocation - sourceLocation).Size();

					GSLOG(Error, TEXT("prop debug - quest prop Interaction start fail - id : %lld\tsource : %s\t dest : %s\tdistance : %f"), pkt->PropGameId(), *sourceLocation.ToString(), *destLocation.ToString(), dist);
				}
			}
#endif

			interationHandler->CancelInteration(propObject);

			//인터렉션 실패시 위젯 갱신
			if (UGsGameObjectPropInteraction* castProp = Cast<UGsGameObjectPropInteraction>(propObject))
			{
				castProp->SetInteractPropEnd(false);
			}
		}
	}
	PD::Result resultEnum = (PD::Result)result;

	// 기존에는 인터랙션 실패이면 ai 켜져있으면 ai 끄는걸로 처리했는데
	// 퀘스트 ui는 켜져있고 ai 내부적으로만꺼져서 그냥 가만히 있는상태가 됨
	// 원래 인터랙션 실패후 다시 시도시도해서 끄는 처리가 들어간거 같아서
	// 1초 대기 같은 대기태스크를 만들어 처리해야할듯
	// https://jira.com2us.com/jira/browse/C2URWQ-4432
	if (GSAI()->IsAIOn())
	{
		FGsAIReserveParam aiReserveAddParam(EGsAIActionType::WAIT_SECOND, 0);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_ADD_RESERVE_JOB, &aiReserveAddParam);
	}

	//// https://jira.com2us.com/jira/browse/C2URWQ-4432
	//// 프랍 동시 인원수 초과 실패(1명)일때는 ai를 끄면 다음 진행이 안됨
	//if(PD::Result::INTERACT_ERROR_INVALID_PROP_MAX_USERCOUNT_LIMIT_OVER != resultEnum)
	//{
	//	//https://jira.com2us.com/jira/browse/CHR-2499
	//	//Prop 이 Interaction이 실패한다면 AI도 꺼줘야한다.
	//	// 거리 실패(클라ok, 서버 fail 상황)일때 ai 꺼줬음
	//	if (GSAI()->IsAIOn())
	//	{
	//		GSAI()->SetActiveAI(false);
	//	}
	//}

	FGsInteractionManager::InteractionErrorMessage(resultEnum);

	GSLOG(Warning, TEXT("PKT_SC_ACK_INTERACT_PROP_START_READ - game id : %d\tresult : %d"), pkt->PropGameId(), result);
}

void FGsNetMessageHandlerGameObject::InteractPropStartAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_INTERACT_PROP_START_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_INTERACT_PROP_START_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	int32 result = pkt->Result();
	// 시작 실패시 켄슬 처리
	if (PACKET_RESULT_SUCCESS == result)
	{
		return;
	}

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		if (UGsGameObjectBase* findLocal = mgr->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectBase* propObject = mgr->FindObject(EGsGameObjectType::PropInteraction, pkt->PropGameId());
			UGsGameObjectLocalPlayer* castLocal = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
			// Handler클래스는 무조건 유효해야 한다. (예외처리 X)
			FGsInteractionHandlerBase* interationHandler = castLocal->GetInteractionHandler();
#if WITH_EDITOR
			FVector sourceLocation;
			FVector destLocation;

			UGsGameObjectBase* target = interationHandler->GetInteractionTarget();
			if (target)
			{
				if (AActor* actor = target->GetActor())
				{
					destLocation = actor->GetActorLocation();
					sourceLocation = UGameplayStatics::GetPlayerCharacter(actor->GetWorld(), 0)->GetMovementComponent()->GetActorFeetLocation();
					float dist = (destLocation - sourceLocation).Size();

					GSLOG(Error, TEXT("Prop Interaction fail - source : %s\t dest : %s\tdistance : %f"), *sourceLocation.ToString(), *destLocation.ToString(), dist);
				}
			}
#endif
			interationHandler->CancelInteration(propObject);

			//인터렉션 실패시 위젯 갱신
			if (UGsGameObjectPropInteraction* castProp = Cast<UGsGameObjectPropInteraction>(propObject))
			{
				castProp->SetInteractPropEnd(false);
			}
		}

		//Prop 이 Interaction이 실패한다면 AI도 꺼줘야한다.
		if (GSAI()->IsAIOn())
		{
			GSAI()->SetActiveAI(false);
		}

		FGsInteractionManager::InteractionErrorMessage((PD::Result)result);

		GSLOG(Warning, TEXT("PKT_SC_ACK_INTERACT_PROP_START_READ - game id : %d\tresult : %d"), pkt->PropGameId(), result);
	}
}

void FGsNetMessageHandlerGameObject::InteractPropStartNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_INTERACT_PROP_START_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_INTERACT_PROP_START_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* manager = GSGameObject())
		{
			UGsGameObjectBase* propObject = manager->FindObject(EGsGameObjectType::PropInteraction, pkt->PropGameId());
			if (UGsGameObjectBase* findObject = manager->FindObject(EGsGameObjectType::Player, pkt->UserGameId()))
			{
				UGsGameObjectPlayer* castObject = findObject->CastGameObject<UGsGameObjectPlayer>();
				FGsInteractionHandlerBase* interationHandler = castObject->GetInteractionHandler();

				interationHandler->StartInteraction(propObject);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::InteractQuestPropEndAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_INTERACT_QUEST_PROP_END_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_INTERACT_QUEST_PROP_END_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	bool isSuccess = (pkt->Result() == PACKET_RESULT_SUCCESS);

	if (UGsGameObjectManager* manager = GSGameObject())
	{
		UGsGameObjectBase* propObject = manager->FindObject(EGsGameObjectType::PropInteraction, pkt->PropGameId());
		if (UGsGameObjectBase* findLocal = manager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* castLocal = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
			FGsInteractionHandlerBase* interationHandler = castLocal->GetInteractionHandler();

			interationHandler->EndInteraction(propObject);

			//클라이언트에서 먼저 위젯을 갱신하면 안되다. 
			//프랍 인터렉션 후 퀘스트 컴플릿 패킷 받고 이 패킷에서 프랍 위젯 갱신
			if (UGsGameObjectPropInteraction* castProp = Cast<UGsGameObjectPropInteraction>(propObject))
			{
				castProp->SetInteractPropEnd(isSuccess);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::InteractPropEndAck(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_ACK_INTERACT_PROP_END_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_ACK_INTERACT_PROP_END_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	auto result = static_cast<PD::Result>(pkt->Result());
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsInteractionManager::InteractionErrorMessage((PD::Result)result);
	}

	if (UGsGameObjectManager* manager = GSGameObject())
	{
		UGsGameObjectBase* propObject = manager->FindObject(EGsGameObjectType::PropInteraction, pkt->PropGameId());
		if (UGsGameObjectBase* findLocal = manager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* castLocal = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
			FGsInteractionHandlerBase* interationHandler = castLocal->GetInteractionHandler();

			interationHandler->EndInteraction(propObject);
		}

		//클라이언트에서 먼저 위젯을 갱신하면 안되다. 
		//프랍 인터렉션 후 퀘스트 컴플릿 패킷 받고 이 패킷에서 프랍 위젯 갱신
		if (UGsGameObjectPropInteraction* castProp = Cast<UGsGameObjectPropInteraction>(propObject))
		{
			castProp->SetInteractPropEnd(result == PACKET_RESULT_SUCCESS);
		}
	}
}

void FGsNetMessageHandlerGameObject::InteractPropEndNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_INTERACT_PROP_END_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_INTERACT_PROP_END_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* manager = GSGameObject())
		{
			UGsGameObjectBase* propObject = manager->FindObject(EGsGameObjectType::PropInteraction, pkt->PropGameId());
			if (UGsGameObjectBase* findObject = manager->FindObject(EGsGameObjectType::Player, pkt->UserGameId()))
			{
				UGsGameObjectPlayer* castObject = findObject->CastGameObject<UGsGameObjectPlayer>();
				FGsInteractionHandlerBase* interationHandler = castObject->GetInteractionHandler();

				interationHandler->EndInteraction(propObject);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::InteractPropCancelAck(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ACK_INTERACT_PROP_CANCEL_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ACK_INTERACT_PROP_CANCEL_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* manager = GSGameObject())
		{
			UGsGameObjectBase* propObject = manager->FindObject(EGsGameObjectType::PropInteraction, pkt->PropGameId());
			if (UGsGameObjectBase* findLocal = manager->FindObject(EGsGameObjectType::LocalPlayer))
			{
				UGsGameObjectLocalPlayer* castLocal = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
				FGsInteractionHandlerBase* interationHandler = castLocal->GetInteractionHandler();

				interationHandler->CancelInteration(propObject);
			}

			//클라이언트에서 먼저 위젯을 갱신하면 안되다. 
			//프랍 인터렉션 후 퀘스트 컴플릿 패킷 받고 이 패킷에서 프랍 위젯 갱신
			if (UGsGameObjectPropInteraction* castProp = Cast<UGsGameObjectPropInteraction>(propObject))
			{
				castProp->CancelPropInteraction();
				castProp->SetCanceling(false);
#if PROP_INTERACTION_DEBUG
				GSLOG(Log, TEXT("prop debug - prop interaciton canel - id : %lld"), propObject->GetGameId());
#endif
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::InteractPropCancelNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_INTERACT_PROP_CANCEL_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_INTERACT_PROP_CANCEL_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* manager = GSGameObject())
		{
			UGsGameObjectBase* propObject = manager->FindObject(EGsGameObjectType::PropInteraction, pkt->PropGameId());
			if (UGsGameObjectBase* findObject = manager->FindObject(EGsGameObjectType::Player, pkt->UserGameId()))
			{
				UGsGameObjectPlayer* castObject = findObject->CastGameObject<UGsGameObjectPlayer>();
				FGsInteractionHandlerBase* interationHandler = castObject->GetInteractionHandler();

				interationHandler->CancelInteration(propObject);
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::ContributionRankList(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_CONTRIBUTION_RANK_LIST_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_CONTRIBUTION_RANK_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	TArray<int64> gameIdArray;
	for (PD::SC::PKT_SC_CONTRIBUTION_RANK_LIST_READ::HighestPartyMemberListIterator iter = pkt->GetFirstHighestPartyMemberListIterator();
		iter != pkt->GetLastHighestPartyMemberListIterator();
		++iter)
	{
		gameIdArray.Add(iter->GameId());
	}

#if CONTRIBUTION_DEBUG
#if WITH_EDITOR
	if (gameIdArray.Num() > 0)
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::Player, gameIdArray[0]);
			if (findObject)
			{
				if (AActor* find = findObject->GetActor())
				{
					GSLOG(Log, TEXT("FGsNetMessageHandlerGameObject::ContributionRankList - game id : %lld\tname : %s"), gameIdArray[0], *find->GetActorLabel());
				}
			}
			else
			{
				GSLOG(Log, TEXT("FGsNetMessageHandlerGameObject::ContributionRankList - game id : %lld"), gameIdArray[0]);
			}
		}
	}
#endif
#endif

	FGsBossHandler* handler = FGsBossHandler::GetInstance();
	if (handler)
	{
		handler->GetFieldBossHandler()->SetConstributionUserId(gameIdArray);
	}

	GMessage()->GetBoss().SendMessage(MessageBoss::CHANGE_CONTRIBUTION, nullptr);


}

void FGsNetMessageHandlerGameObject::SetAggroTarget(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_AGGRO_TARGET_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_AGGRO_TARGET_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	
	//show hud	
	FGsBossHandler* handler = FGsBossHandler::GetInstance();
	if (handler)
	{
		if (FGsFieldBossHandler* bossHandler = handler->GetFieldBossHandler())
		{
			if (0 == pkt->TargetGameId())
			{
				bossHandler->ClearAggroTarget(pkt->NpcGameId());
			}
			else
			{
				bossHandler->AddAggroTarget(pkt->NpcGameId(), pkt->TargetGameId());
			}
		}		
	}

	GMessage()->GetBoss().SendMessage(MessageBoss::CHANGE_AGGRO, nullptr);
}

void FGsNetMessageHandlerGameObject::CognitionTargetNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_COGNITION_TARGET_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_COGNITION_TARGET_READ*>(inPacket.Buffer))
	{
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::NonPlayerBase, pkt->NpcGameId()))
			{
				UGsGameObjectNonPlayerBase* castObject = findObject->CastGameObject<UGsGameObjectNonPlayerBase>();
				castObject->SetCognitionTargetEffect();
			}
		}
	}
}

void FGsNetMessageHandlerGameObject::AutoBattleRangeNoLimit(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_AUTO_BATTLE_RANGE_NOLIMIT_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_AUTO_BATTLE_RANGE_NOLIMIT_READ*>(inPacket.Buffer))
	{
		GData()->GetGlobalData()->_autoReturnNoLimit = pkt->AttackRange();
	}
}
void FGsNetMessageHandlerGameObject::WarpWithDespawnReady(const FGsNet::Packet& inPacket)
{
	if (FGsInteractionManager* interactionManager = GSInteraction())
	{
		interactionManager->SetGuildDungeonWarpReq(false);
	}
}

void FGsNetMessageHandlerGameObject::EliteDungeonPortalHiddenSpawnNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ELITE_DUNGEON_PORTAL_HIDDEN_SPAWN_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ELITE_DUNGEON_PORTAL_HIDDEN_SPAWN_READ*>(inPacket.Buffer))
	{
		// TEXT: 갈라진 전당의 틈이 생성되었습니다.
		FText textMsg;
		if (FText::FindText(TEXT("DungeonText"), TEXT("Notice_EliteDungeon_HiddenPortalCreate"), textMsg))
		{
			FGsUIHelper::TrayMessageTicker(textMsg);
		}

		GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_SPAWN_INTER_PORTAL, nullptr);
	}
}

void FGsNetMessageHandlerGameObject::EliteDungeonPortalHiddenDespawnNotify(const FGsNet::Packet& inPacket)
{
	if (PD::SC::PKT_SC_ELITE_DUNGEON_PORTAL_HIDDEN_DESPAWN_READ* pkt =
		reinterpret_cast<PD::SC::PKT_SC_ELITE_DUNGEON_PORTAL_HIDDEN_DESPAWN_READ*>(inPacket.Buffer))
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_DESPAWN_INTER_PORTAL, nullptr);
	}
}