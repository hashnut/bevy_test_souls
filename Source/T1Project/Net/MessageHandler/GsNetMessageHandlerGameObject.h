// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "GsNetMessageHandleInterface.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"


class FGsNetManager;
class UGsGameObjectBase;
struct FGsSpawnParam;
/**
 * 
 */
class T1PROJECT_API FGsNetMessageHandlerGameObject : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerGameObject() = default;
	virtual ~FGsNetMessageHandlerGameObject();
	
public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

	// Ack
private:
	/*TMap<int64, PD::SC::PKT_SC_SPAWN_NPC_READ> NPCPoolData;*/
	TMap<int64, TSharedPtr<FStreamableHandle>> NPCPoolHandle;

private:
	void SetLocalPlayerCollsionFlag(bool inFlag);

protected:
	//Spawn
	void SpawnNpcAck(const FGsNet::Packet& inPacket);
	void DespawnNpcAck(const FGsNet::Packet& inPacket);	

	//특수한 성격을 포함한 npc 스폰
protected:
	void SpawnSpecialNpc(const  FGsNet::Packet& inPacket);
	void NpcChangeSpecialStataus(const  FGsNet::Packet& inPacket);	
	void SpawnTreasureNpc(const FGsNet::Packet& inPacket);
	void DespawnTreasureNpc(const FGsNet::Packet& inPacket);
	void UpdateTreasureNpcGuild(const FGsNet::Packet& inPacket);
	void UpdateTreasureNpcPosition(const FGsNet::Packet& inPacket);

public:	
	static UGsGameObjectBase* SpawnNpc(const FGsSpawnParam& inSpawnParam, SPAWN_REASON inReason, bool inIsAlive, bool inIsCombat,
		TerritoryType inTerritoryType = TerritoryType::NONE, int64 inSpawnId = -1, NpcSpecialStatusType = NpcSpecialStatusType::NONE,
		bool inIsOffensive = false, bool inIsSkipTerrainCheck = false);
	static void DespawnNpc(int64 inGameId, SPAWN_REASON inReason);
	static void DespawnNpcObject(UGsGameObjectBase* In_obj);
public:
	void SpawnMeAck(const FGsNet::Packet& inPacket);
	void SpawnMeCompleteAck(const FGsNet::Packet& inPacket);
	void SpawnMeAction(const FGsNet::Packet& inPacket);
	void SpawnUserAck(const FGsNet::Packet& inPacket);
	void DespawnUserAck(const FGsNet::Packet& inPacket);
	void SpawnProjectileAck(const FGsNet::Packet& inPacket);
	void DespawnProjectileAck(const FGsNet::Packet& inPacket);

public:
	//LocalPlayer
	void SkillAck(const FGsNet::Packet &inPacket);
	void StatLoadTopo(const FGsNet::Packet &inPacket);
	void WarpFailure(const FGsNet::Packet& inPacket);
	void UserExpUpdate(const FGsNet::Packet& inPacket);
	void UserCurrencyUpdate(const FGsNet::Packet& inPacket);

protected:
	void SkillCoolDownAck(const FGsNet::Packet& inPacket);

protected:
	//UserData
	void UserBasicDataAck(const FGsNet::Packet& inPacket);
	void UserLookInfoChangeAck(const FGsNet::Packet& inPacket);

	//Iff
	void UserIffStatusAck(const FGsNet::Packet& inPacket);
	void IffStatusAck(const FGsNet::Packet& inPacket);
	void UpdateIffTeamAck(const FGsNet::Packet& inPacket);

	// Notify
protected:
	void UserLevelUpdatedNotify(const FGsNet::Packet& inPacket);
	void CreatureCombatStatusNotify(const FGsNet::Packet& inPacket);

	// Camp
public:
	void SpawnCampAck(const FGsNet::Packet& inPacket);
	void CampBuildStepNotify(const FGsNet::Packet& inPacket);
	void UserNpcInteractStateNotify(const FGsNet::Packet& inPacket); // 주의: 성소도 같이 쓴다
	void CancelNpcInteractNotify(const FGsNet::Packet& inPacket);  // 주의: 성소도 같이 쓴다
	void CampConstructionCompletedNotify(const FGsNet::Packet& InPacket);
	void DespawnCampNotify(const FGsNet::Packet& inPacket);
	void DespawnCampAck(const FGsNet::Packet& inPacket);
	void CampNameNotify(const FGsNet::Packet& InPacket);
	void GuildMemberCampNotify(const FGsNet::Packet& inPacket);
	void AllowedBuildCampNotify(const FGsNet::Packet& inPacket);

	// Sanctum
public:
	void SanctumNexusInstallAck(const FGsNet::Packet& inPacket);
	void SanctumStatusNotify(const FGsNet::Packet& inPacket);
	void SanctumNexusStatusNotify(const FGsNet::Packet& inPacket);
	void SanctumNexusConstructionStepNotify(const FGsNet::Packet& inPacket); //CampBuildStepNotify
	void SanctumBattlePrepareFirstNotify(const FGsNet::Packet& inPacket);
	void SanctumBattlePrepareFinalNotify(const FGsNet::Packet& inPacket);
	void SanctumBattleStartNotify(const FGsNet::Packet& inPacket);
	void SanctumBattleEndNotify(const FGsNet::Packet& inPacket);
	void SanctumBattleVictoryNotify(const FGsNet::Packet& inPacket);
	void SanctumGuildMemberNotify(const FGsNet::Packet& inPacket);
	void SanctumNexusEmblemChangeNotify(const FGsNet::Packet& inPacket);

public:
	void StatInfoNotify(const FGsNet::Packet& inPacket);
	void UserWeightInfoNotify(const FGsNet::Packet& inPacket);

public:
	void MoveToLocationAck(const FGsNet::Packet& inPacket);
	void MoveImmediateStopAck(const FGsNet::Packet& inPacket);
	void MoveFailedAck(const FGsNet::Packet& inPacket);
	void MoveStiffenNotify(const FGsNet::Packet& inPacket);
	void SkillMoveLocation(const FGsNet::Packet& inPacket);
	
	// 이동형 발사체 이동 패킷
	void ProjectileMoveToLocationNotify(const FGsNet::Packet& inPacket);

	// CS_REQ_DROP_OBJECT_LOOT, CS_REQ_TRY_INTERACT등의 요청에 의한 브로드 캐스팅 패킷
	void MoveCorrectionPosNotify(const FGsNet::Packet& inPacket);

	//https://jira.com2us.com/wiki/pages/viewpage.action?pageId=294813136
	void RequestPosValidate(const FGsNet::Packet& inPacket);
	void ForcedPosNotify(const FGsNet::Packet & inPacket);

	// 회전 연출 시작
	void StartRotationNotify(const FGsNet::Packet& inPacket);
	
	// Rootmotion 시작
	void CreatureSkillRootmotionStartNotify(const FGsNet::Packet& inPacket);
	void CreatureSkillRootmotionFinishNotify(const FGsNet::Packet& inPacket);

public:
	void LocalPlayerCollisionOn(const FGsNet::Packet& inPacket);
	void LocalPlayerCollisionOff(const FGsNet::Packet& inPacket);

public:
	void StartSkillAck(const FGsNet::Packet& inPacket);
	void SkillFinishedAck(const FGsNet::Packet& inPacket);
	void SkillFailureNotify(const FGsNet::Packet& inPacket);
	void SkillDamagedNotify(const FGsNet::Packet& inPacket);
	void PassivityDamageNotify(const FGsNet::Packet& inPacket);
	void SkillHealedNotify(const FGsNet::Packet& inPacket);
	void SkillEffectNotify(const FGsNet::Packet& inPacket);

	void PassivityInvokeNotify(const FGsNet::Packet& inPacket);

	void SocialActionNotify(const FGsNet::Packet& inPacket);
	void SocialEmotionNotify(const FGsNet::Packet& inPacket);

public:
	void UpsertAbnormalityNotify(const FGsNet::Packet& inPacket);
	void RemoveAbnormalityNotify(const FGsNet::Packet& inPacket);
	void FailedAddAbnormalityAck(const FGsNet::Packet& inPacket);
	void SetAbnormalityNotify(const FGsNet::Packet& inPacket);
	void AbnormalityResultNotify(const FGsNet::Packet& inPacket);

	void PassivityAddedNotify(const FGsNet::Packet& inPacket);
	void PassivityRemovedNotify(const FGsNet::Packet& inPacket);

	void BehaviorStatusAck(const FGsNet::Packet& inPacket);

public:
	void CreatureDeathAck(const FGsNet::Packet& inPacket);
	void UserResurrectAck(const FGsNet::Packet& inPacket);
	void ResurectFailureAck(const FGsNet::Packet& inPacket);
	void UserAutoResurrectAck(const FGsNet::Packet& inPacket);

public:
	void ChangeWeaponTypeFailureAck(const FGsNet::Packet& inPacket);
	void ChangeWeaponTypeStartNotify(const FGsNet::Packet& inPacket);
	void ChangeWeaponTypeFinishedNotify(const FGsNet::Packet& inPacket);

private:
	void TestSpawnRemote(int64 inGameId, FVector& inPos, FRotator& inRot);

public:
	void DebugShapeAck(const FGsNet::Packet& inPacket);
	void DebugShapeLineAck(const FGsNet::Packet& inPacket);
	void DebugShapeCylinderAck(const FGsNet::Packet& inPacket);
	void ObjectCollisionDebugShape(const FGsNet::Packet& inPacket);

	void DrawDebugCylinder(UWorld* inWorld, const FVector& inLocation, const FLinearColor& inColor);

	void IffDecugAck(const FGsNet::Packet& inPacket);

#if	!UE_BUILD_SHIPPING
	void DebugTextInfoAck(const FGsNet::Packet& inPacket);
#endif

public:
	void EnterTerritoryAck(const FGsNet::Packet& inPacket);
	void LeaveTerritoryAck(const FGsNet::Packet& inPacket);

	//DropObject
public:
	void SpawnDropObject(const FGsNet::Packet& inPacket);
	void DespawnDropObject(const FGsNet::Packet& inPacket);
	void DropObjectOwnershipExpired(const FGsNet::Packet& inPacket);	
	void LootDropObjectFail(const FGsNet::Packet& inPacket);

	// Skill
protected:
	void SkillSlotClearAck(const FGsNet::Packet& inPacket);
	void SkillSlotSetAck(const FGsNet::Packet& inPacket);
	void SkillSlotAutoAck(const FGsNet::Packet& inPacket);
	void SkillLearn(const FGsNet::Packet& inPacket);
	void SkillUpdateActiveAck(const FGsNet::Packet& inPacket);
	void SkillEnchantAck(const FGsNet::Packet& inPacket);
	void SkillBonusLevelNotify(const FGsNet::Packet& inPacket);
	void CostumeSkillLearnAck(const FGsNet::Packet& inPacket);
	void CostumeSkillSlotSetAck(const FGsNet::Packet& inPacket);
	void AckExchangeCostume(const FGsNet::Packet& inPacket);
	void SkillUseSkillCycleAck(const FGsNet::Packet& inPacket);

	//Prop
public:
	void SpawnPropAck(const FGsNet::Packet& inPacket);
	void DespawnPropAck(const FGsNet::Packet& inPacket);
	void UpdatePropAck(const FGsNet::Packet& inPacket);
	void ActiveInvasionPropAck(const FGsNet::Packet& inPacket);
	void InteractPropInvasionAck(const FGsNet::Packet& inPacket);
	
	// Ack는 Send한 대상에게만 오는 패킷이며, Notify는 브로드캐스트로 온 패킷 정보
	void InteractQuestPropStartAck(const FGsNet::Packet& inPacket);
	void InteractPropStartAck(const FGsNet::Packet& inPacket);
	void InteractPropStartNotify(const FGsNet::Packet& inPacket);
	void InteractQuestPropEndAck(const FGsNet::Packet& inPacket);
	void InteractPropEndAck(const FGsNet::Packet& inPacket);
	void InteractPropEndNotify(const FGsNet::Packet& inPacket);
	void InteractPropCancelAck(const FGsNet::Packet& inPacket);
	void InteractPropCancelNotify(const FGsNet::Packet& inPacket);		

	//Field Boss
public:	
	void ContributionRankList(const FGsNet::Packet& inPacket);

	//Aggro
public:
	void SetAggroTarget(const FGsNet::Packet& inPacket);
	void CognitionTargetNotify(const FGsNet::Packet& inPacket);

public:	
	void AutoBattleRangeNoLimit(const FGsNet::Packet& inPacket);

public:
	void WarpWithDespawnReady(const FGsNet::Packet& inPacket);

	// 시련의 전당 포탈
public:	
	void EliteDungeonPortalHiddenSpawnNotify(const FGsNet::Packet& inPacket);
	void EliteDungeonPortalHiddenDespawnNotify(const FGsNet::Packet& inPacket);
};
