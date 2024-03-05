#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedBookmarkEnum.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"


struct FGsReqEventActionParam;

class FGsNetSendServiceWorld
{
	// Skill
public:
	static void SendLocalPlayerMove(
		const FVector& inCurrPos, int16 inCurrDir,
		const FVector& inDestPos, float inFallingSpeed, uchar inMotionType, uint64 inClinentTick, uint64 inSeqNum);

	static void SendLocalPlayerPosValidate(
		const FVector& inCurrPos, int16 inCurrDir,
		const FVector& inDestPos, uchar inMotionType, uint64 inClinentTick, uint64 inSeqNum);

	static bool SendStartSkill(
		int32 inSkillId, const FVector& inCurrPos, int16 inCurrDir,
		const FVector& inAimStartPos, const FVector& inAimTargetPos, int64 inTargetId, bool inAuto);

	static void SendCancelSkill();
	static void SendSkillSlotClear(CreatureWeaponType InWeapon, SkillCategorySet InCategorySet);
	static void SendRequestSkillSlotSet(CreatureWeaponType InWeapon, SkillId InRootSkillId, SkillId InActiveSkillId, SkillSlotId InSkillSlotId);
	static void SendRequestSkillSlotAuto(CreatureWeaponType InWeapon, SkillId InRootSkillId, SkillId InActiveSkillId, bool bInAutoUse);
	static void SendRequestSkillEnchant(SkillId InRootSkillId, SkillId InActiveSkillId, TArray<ItemDBIdAmountPair>& InCostList);
	static void SendRequestCostumeSkillLearn(CostumeId InCostumeId);
	static void SendRequestCostumeSkillSlotSet(CostumeId InCostumeId, SkillId InRootSkillId, SkillId InActiveSkillId, SkillSlotId InSkillSlotId);
	static void SendRequestUseSkillCycle(SkillId InRootSkillId, SkillId InActiveSkillId,
		bool bUseInterval, bool bUseInPVP, int32 InIntervalSecond);

	// Resurrect
public:
	static void SendResurrect(ResurrectType inType);

	// Quest
public:
	static void SendQuestMainAccept(QuestId inQuestId);
	static void SendQuestSubAccept(StoryId inStoryId, QuestId inQuestId, SpawnId inSpawnId);
	static void SendQuestRepeatAccept(StoryId inStoryId, QuestId inQuestId);
	static void SendQuestMultiLevelRankAccept(StoryId inStoryId, const TArray<QuestId>& InQuestIdList);
	static void SendQuestGuideAccept(QuestId inQuestId);

	static void SendQuestMainReward(QuestId inQuestId, ItemBagIndex inMainIndex = INVALID_ITEM_BAG_INDEX,
		ItemBagIndex inSubIndex = INVALID_ITEM_BAG_INDEX);
	static void SendQuestSubReward(StoryId inStoryId, QuestId inQuestId, ItemBagIndex inMainIndex = INVALID_ITEM_BAG_INDEX,
		ItemBagIndex inSubIndex = INVALID_ITEM_BAG_INDEX);
	static void SendQuestRepeatReward(StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex = INVALID_QUEST_INDEX, ItemBagIndex inMainIndex = INVALID_ITEM_BAG_INDEX,
		ItemBagIndex inSubIndex = INVALID_ITEM_BAG_INDEX);
	static void SendQuestMultiLevelRankComplete(StoryId InStoryId, const TArray<QuestId> InQuestList);	
	static void SendQuestGuideReward(QuestId InQuestId, ItemBagIndex inMainIndex = INVALID_ITEM_BAG_INDEX,
		ItemBagIndex inSubIndex = INVALID_ITEM_BAG_INDEX);

	static void SendQuestMainUpdate(QuestId inQuestId, QuestIndex inQuestIndex, QuestObjectiveId inQuestObjectiveId, SpawnId inSpawnId, int64 inGameId, MapId inMapId = 0);
	static void SendQuestSubUpdate(StoryId inStoryId, QuestIndex inQuestIndex, QuestId inQuestId, QuestObjectiveId inQuestObjectiveId, SpawnId inSpawnId, int64 inGameId, MapId inMapId = 0);
	static void SendQuestRepeatUpdate(StoryId inStoryId, QuestIndex inQuestIndex, QuestId inQuestId, QuestObjectiveId inQuestObjectiveId, SpawnId inSpawnId, int64 inGameId, MapId inMapId = 0);
	
	static void SendQuestSubGiveUp(StoryId inStoryId, QuestId inQuestId);
	static void SendQuestRepeatGiveUp(StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex);

	static void SendQuestWarp(QuestId inQuestId, QuestIndex inQuestIndex, QuestObjectiveId inQuestObjectiveId, bool inIsAccpeted = true);
	

	//warp 
public:
	static void SendWarp(uint32 inWarpId);
	static void SendAckWarpWithDespawnReady();
	static void SendWarpGuildCamp(CampId InCampId);
	static void SendReqSpawnMeAction();

	// Event Action
public:
	static void SendEventAction(const FGsReqEventActionParam& Param);

	//Zone
public:
	static void SendEnterTerritory(uint32 inTerritoryId, const FVector& inCurrLoc);
	static void SendLeftTerritory(uint32 inTerritoryId, const FVector& inCurrLoc);

	// Channel
public:
	static void SendRequestChannelList();
	static void SendRequestSelectChannel(ChannelId InChannelId);
	static void SendRequestLeaveInstanceSingleMap();

	//drop object
public:
	static void SendDropObjectLoot(int64 inGameId, bool inAutoLooting);

	// bookmark
public:
	static void SendBookmarkCreate(const FString& In_name);
	static void SendBookmarkUpdate(uint64 In_id, const FString& In_name);
	static void SendBookmarkDelete(uint64 In_id);
	static void SendBookmarkTeleport(uint64 In_bookmarkId, uint64 In_itemDBId);
	static void SendBookmarkShare(BookmarkShareType In_type, BookmarkId In_id);
	static void SendSharedBookmarkWarp(MapId In_mapId, const FVector& In_pos, ItemDBId In_itemDBId, int In_channelId);
	static void SendSharedBookmarkRegister(const FString& In_name, MapId In_mapId, const FVector& In_pos);

	// Interact
public:
	// Npc Interaction
	static void SendTryInteract(CreatureId inNpcId, SpawnId inSpawnId, int64 inGameId);
	// Prop Object Interaction
	static void SendInteractQuestPropStart(int64 In_gameId, TArray<QuestPreocessData>& In_QuestObjectList);
	static void SendInteractQuestPropEnd(int64 In_gameId, TArray<QuestPreocessData>& In_QuestObjectList);
	static void SendInteractPropCancel(class UGsGameObjectPropObject* inProp);
	static bool SendInteractPropCancel(int64 inGameId);

	static void SendInteractPropStart(int64 In_gameId);
	static void SendInteractPropEnd(int64 In_gameId);

	static void SendEventPlayerEffect(CreatureId In_npcId);

	static void SendInteractInvasion(int64 gameId);
	// Event Shop
public:
	static void SendTryNpcEventShopOpen(int64 In_gamdId);



	//stat
public:
	static void SendRequestBonusStatIncrease();
	static void SendReqBonusStatExtendCount();

	// portal
public:
	static void SendPortalWarp(int In_mapId, uint32 In_spotId);

	// portal
public:
	static void SendMonsterSpawnWarp(MapId In_mapId, CreatureId In_npcId, SpawnGroupId In_spawnGroupId);

	//fairy
public:
	static void SendReqFairyEquip(FairyId id); //CS_REQ_FAIRY_EQUIP
	static void SendReqFairyEnchant(FairyId id); //CS_REQ_FAIRY_ENCHANT
	static void SendReqFairyCompose(const TArray<FairyIdAmountPair>& fairyPairList, const TArray<uint8>& fairyCountList);		//CS_REQ_FAIRY_COMPOSE
	static void SendReqFairySummonChange(FairyConfirmId fairyConfirmId);
	static void SendReqFairySummonConfirm(FairyConfirmId fairyConfirmId);
	static void SendReqFairyCeiling(FairyGrade grade, uint8 phase);

	static void SendMountVehicle();
	static void SendUnmountVehicle();

	// item collection
public:
	static void SendItemCollectionRegister(const CollectionConditionId InCollectionConditionId, const ItemDBId InItemDBId);
	static void SendItemCollectionBookmark(const CollectionId InCollectionID, bool bInChangedFlag);

	//option
public:
	static void SendReqOption(const ClientOptionData& inOptionData);
	static void SendReqGameOption(const GameOption& inGameOptionData);

	//chat
public:
	static void SendReqCheckInterUserExistByName(const FString& nickName, WorldId worldId);

	//penalty
public:
	static void SendReqDeathPenaltyRestore(TArray<DeathPenaltyRestore>& inDeathPenaltyId);
	
#ifdef R3_ARCAS_NOT_USE
	//XpBuff
public:
	static void SendReqUpdateArcasBuff(bool useFairyBless, bool useArcasBless);
#endif

public:
	static void SendReqCostumeEquip(CostumeId id);
	static void SendReqCostumeCompose(const TArray<CostumeIdAmountPair>& pairList, const TArray<uint8>& countList);		//CS_REQ_COSTUME_COMPOSE
	static void SendReqCostumeSummonChange(CostumeConfirmId confirmId);
	static void SendReqCostumeSummonConfirm(CostumeConfirmId confirmId);
	static void SendReqCostumeCeiling(CostumeGrade grade, uint8 phase);
	static void SendReqCostumeExchange(ItemDBId itemDBId, CostumeId recipeCostumeId);

	//mail	
public:
	static void SendReqMailList();
	static void SendReqNewSystmeMailRecv();
	static void SendReqMailRecv(MailBoxType inBoxType, TArray<MailDBId>& inMailDBIdList);
	static void SendReqMailDelete(MailBoxType inBoxType, TArray<MailDBId>& inMailDBIdList);
	static void SendReqMailRead(MailBoxType inBoxType, MailDBId inMailDBId);

	//Dungeon
public:
	static void SendReqDungeonGroupList();

	// 2022/06/09 PKT - Party Dungeon
	static void Send_ExitDungeon();				// 2022/06/09 PKT - 던전에서 나가겠다.
	static void Send_ExitBattleArena();			// 2023/12/4 PKT - 배틀 Arena에서 나가겠다.

	// party target share
public:
	static void SendPartyTargetShare(int64 In_targetGameId);
	static void SendPartyTargetShareCancel();
	
	// 천마석(정령탄) sprit shot
public:
	// 천마석 ui 키고 끄기
	static void SendReqSpiritShotOnOff(bool In_isOn);

	//npc
public:
	static void SendNpcSpecialFunction(int64 inGameId);	

	//SpaceCrack
public:
	static void SendReqSpaceCrackWeekReward();

	
	// multi level rank
public:
	static void SendReqMultiLevelRankPromotion(MultiLevelRankId InRankId);		// 다단계 랭크 승급 요청
	static void SendReqMultiLevelRankChangeRank(MultiLevelRankId InRankId);		// 다단계 랭크 변경 요청

	// 2021/06/29 PKT - Offline Play
public:
	static void SendReqOfflinePlayReadyState();		// 2021/06/29 PKT - 오프라인 플레이 레디 상태 요?
	static void SendReqOfflinePlayStartPlay();		// 2021/06/29 PKT - 오프라인 플레이 시작 상태 요청
	static void SendReqOfflinePlayCancel();			// 2021/06/29 PKT - 오프라인 플레이 취소 상태 요청
	static void SendOfflinePlayResultConfirm();			// 2023/4/7 PKT - 오프라인 플레이 결과 확인 버튼

	// Attendance
public:
	static void SendReqCommunityList();
	static void SendReqAttendanceCheck(AttendanceId inId, AttendanceCheckIdx inIndex);
	static void SendReqPurchaseAttendance(AttendanceId inId);

	//resurrection
public:
	static void SendUserDeathPenaltyCheck();

	//play cinematic
public:
	static void SendCinematicStartByEventAction(uint32 inSequenceResId, const FVector& inCurrPos, int16 inCurrDir);
	static void SendCinematicEndByEventAction(uint32 inSequenceResId);

	//업적
public:
	//static void SendReqAchievementThemeRewards(AchievementThemeId id);
	static void SendReqAchievementRewards(AchievementId id);
	static void SendReqAchievementEnableRewardList();
	static void SendReqAchievementReceiveRewardAll();

	// 파티
public:
	// 파티장에게로 이동(요거만 월드에서 하고 나머진 gateway 쪽에서 함)
	static void SendWarpToPartyLeader();

	// Camp
public:
	static void SendCancelCamp(CampId InCampId);

	// 성소
public:
	static void SendReqSanctumNexusInstall(SanctumAreaId InSanctumId, int64 InInteractNpcGameId, FVector InCurrPos, int16 InCurrDir);
	//static void SendReqSanctumMapInfo(MapId InMapId);

	// 2021/11/23 PKT - 몬스터 도감
public:
	static void Send_MonsterKnowledgeLevelUp(uint32 InId);
	static void Send_MonsterKnowledgeLevelAll();
	static void Send_MonsterKnowledgeExpIncrease();
	static void Send_MonsterKnowledgeExpGainItemUse(ItemDBId InItemDBId, ItemAmount InAmount, CodexMapGroupId InId);
	static void Send_MonsterKnowledgeExpGainItemUseAll(
		ItemDBId InItemDBId, ItemAmount InAmount, const TArray<MonsterKnowledgeBookGrade>& InGrades, int32 InPriority
	);
	
	static void Send_MonsterKnowledgeSlate(MonsterKnowledgeSlateId InId);
	static void Send_MonsterKnowledgeUnLock(ItemDBId InItemDBId);
	static void Send_ActivateMonsterKnowledgeSlate(MonsterKnowledgeSlateId InSlateId, MonsterKnowledgeSlateSlotId InSlotId);
	static void Send_ActivateMonsterKnowledgeSlateAll(MonsterKnowledgeSlateId InSlateId);
	static void Send_MonsaterKnowledgeSlateReset(ItemDBId InItemDBId);

#pragma region Codex
	static void Send_Codex(CodexCategoryId InCategoryId);
	static void Send_MonsterBookPointExchange();
	static void Send_CodexCategoryLevelUp(CodexCategoryId InCategoryId);
	static void Send_ActivateSlateSpecialNode(MonsterKnowledgeSlateId InSlateId, SlateSpecialNodeId InSpecialNodeId);
#pragma endregion

	//
public:
	static void SendReqStatisticsInfo(StatisticsTabId InTabId);
	static void SendReqStatisticsDailyInfo(StatisticsId InId);

	// 2022/01/24 PKT - 타이틀(칭호)
public:
	static void Send_TitleEquip(TitleId InEquipTitleId, TitleEquipSlot InEquipTitleSlot);
	static void Send_TitleConfirm(const TArray<TitleId>& InList);

public:
	// 대륙맵에서 침공 정보 갱신 요청
	static void Send_ReqInvadeInfo();
	// 월드맵의 spawn된 포털 위치 정보 요청
	static void Send_ReqInvasionPortalInfo();
	// 침공전에서 내가 등록한 포탈이 있는 맵의 정보 요청
	static void Send_ReqActiveInvasionPortalList();
	// 튜토리얼
public:
	static void SendReqTutorialSave(TutorialId InTutorialId);

	// 대결
public:
	static void SendReqDuelInvite(UserDBId InUserDBId);
	static void SendReqDuelInviteCancel();
	static void SendReqDuelAccept(bool InIsAccept);

	//배틀패스
public:
	static void SendReqActiveBattlePassAddReward(BattlePassId passId, ItemDBId itemId = 0);
	static void SendReqBuyBattlePassLevel(BattlePassId passId, Level targetLevel);
	static void SendReqGetBattlePassLevelReward(BattlePassId passId, Level rewardLevel, bool isaddReward = false);

	static void SendReqBattlePassLevelRewardList(BattlePassId passId);
	static void SendReqBattlePassReceiveRewardAll(BattlePassId passId);

public:
	static void SendReqUserStatBookmarkList();
	static void SendReqUserStatBookmarkAdd(StatType inStatType);
	static void SendReqUserStatBookmarkDelete(StatType inStatType);

	// 2022/09/21 PKT - PKBook
public:
	// 2022/09/22 PKT - Save Check Out
	static void Send_PKBookCheckOutTime(int64 InCheckOutTime);
	// 2022/09/22 PKT - Detail info 요청
	static void Send_ReqPKBookDetailInfo(PKRecordId InRecordId);
	// 2022/09/22 PKT - 복수 하러 가겠다.
	static void Send_ReqPKBookGoForRevenge(PKRecordId InRecordId);
	// 2022/09/22 PKT - 조롱 메세지를 보낸다.
	static void Send_ReqPKBookSendTeaseMessage(PKRecordId InRecordId, const FText& InMessage);
	// 2022/09/22 PKT - 경계 대상 등록 요청
	static void Send_ReqPKBookAddDetectionTarget(WorldId InHomeWorldId, UserDBId InUserId, const FText& InTargetName = FText::GetEmpty());
	// 2022/09/22 PKT - 경계 대상 삭제 요청
	static void Send_ReqPKBookDeleteDetectionTarget(UserDBId InUserId);
	// 2022/09/22 PKT - 메세지 저장
	static void Send_ReqPKBookSavePKMessage(bool InLogInOption, bool InUseMessage, const FText& InKillMessage, const FText& InDieMessage);
	// 2023/1/17 PKT - 조롱 메세지 저장하기
	static void Send_ReqSavedMockeryMessage(bool InUsed, const FText& InMessage);

public:
	static void SendReqUserSocialEmotion(UserSocialEmotion emotion);

public:
	static void SendReqUserPresetChange(PresetId InPresetId);
	static void SendReqUserPresetCopyTo(PresetId InPresetId);
	static void SendReqUserPresetName(PresetId InPresetId, const FString& InPresetName);

public:
	static void SendReqPersonalTradingTry(int64 InTargetGameId);	// (거래전) 개인 거래 요청
	static void SendReqPersonalTradingAccept(int64 InOwnerGameId);	// (거래전) 개인 거래 승락
	static void SendReqPersonalTradingCancel(int64 InOwnerGameId);	// (거래전) 개인 거래 요청/승락 취소
	static void SendReqPersonalTradingAbort();				// (거래중) 개인 거래 중지
	static void SendReqPersonalTradingLock(bool InIsLock);	// (거래중) 개인 거래 잠금 요청
	static void SendReqPersonalTradingConfirm(bool InIsConfirm);			// (거래중) 개인 거래 확인 요청
	static void SendReqPersonalTradingPutItem(ItemDBId InItemDBId, ItemAmount InItemAmount);	// (거래중) 개인 거래 아이템 추가/변경/삭제
	static void SendReqPersonalTradingPutDia(Gem InDiaAmount);	// (거래중) 개인 거래 재화 변경

public:
	static void SendReqLatency(const FString& InTimeStamp);

public:
	static void SendReqUserContextOtherServer(const FString& InUserName, WorldId InWorldId);


	// summon
public:
	// 뽑기 마일리지 페어리 보상 아이템 요청
	static void SendReqFairySummonMileage();
	// 뽑기 마일리지 코스튬 보상 아이템 요청
	static void SendReqCostumeSummonMileage();

	// 뽑기 마일리지 유니크 페어리 보상 아이템 요청
	static void SendReqUniqueFairySummonMileage();
	// 뽑기 마일리지 유니크 코스튬 보상 아이템 요청
	static void SendReqUniqueCostumeSummonMileage();

public:
	// CommunityEventMission
	static void SendReqCommunityEventMissionReward(CommunityEventMissionId InMissionId, 
		CommunityEventMissionTabId InTabId, CommunityEventMissionCommonId InCommonId, bool InIsBonus);

public:
	static void SendReqBattleArenaGuildWarDetailScore();
};
