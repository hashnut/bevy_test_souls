// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"

class FGsNetManager;
/**
 * 
 */
class T1PROJECT_API FGsNetMessageHandlerWorld : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerWorld() = default;
	virtual ~FGsNetMessageHandlerWorld();
	
public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

	// Channel
protected:
	void ChannelListAck(const FGsNet::Packet& inPacket);
	void SelectChannelAck(const FGsNet::Packet& inPacket);
	void LeaveInstanceSingleMapFailure(const FGsNet::Packet & inPacket);

	// Bookmark
protected:
	void BookmarkAck(const FGsNet::Packet& inPacket);
	void BookmarkCreate(const FGsNet::Packet& inPacket);
	void BookmarkUpdate(const FGsNet::Packet& inPacket);
	void BookmarkDelete(const FGsNet::Packet& inPacket);
	void BookmarkTeleport(const FGsNet::Packet& inPacket);
	void BookmarkShare(const FGsNet::Packet& inPacket);
	void NotifySharedBookmark(const FGsNet::Packet& inPacket);
	void SharedBookmarkWarp(const FGsNet::Packet& inPacket);
	void SharedBookmarkRegister(const FGsNet::Packet& inPacket);
private:
	// 자동전투, 퀘스트 자동진행 끄기
	void StopAuto();

protected:
	// GameOption
	void GameOptionAck(const FGsNet::Packet& inPacket);
	void GameOptionSaveAck(const FGsNet::Packet& inPacket);
private:
	void ApplyGameOption(const GameOption& GameOptionData);



	// portal
protected:
	// 포털 워프 응답
	void PortalWarp(const FGsNet::Packet& inPacket);
	
	// '디스폰을 수반한 워프' 준비 요청.
	void WarpWithDespawnReadyReq(const FGsNet::Packet& inPacket);
	
	// '디스폰을 수반한 워프' 실패.
	void WarpWithDespawnFailure(const FGsNet::Packet& inPacket);

	// 기사단 캠프 워프 실패
	void WarpGuildCampFailure(const FGsNet::Packet& inPacket);
	
protected:
	// 옵션
	void OptionAck(const FGsNet::Packet& inPacket);

protected:
	// Interact
	void TryInteractAck(const FGsNet::Packet& inPacket);			// Interact

protected:
	// Event Shop
	void TryNpcEventShopOpenAck(const FGsNet::Packet& inPacket);

	void EnterTerritoryEventShopData(const FGsNet::Packet& inPacket);
	void LeaveTerritoryEventShopData(const FGsNet::Packet& inPacket);

	//Stat distribution
protected:
	void AckBonusStatIncreaseFailure(const FGsNet::Packet& inPacket);
	void UpdateRemainBonusStatPoint(const FGsNet::Packet& inPacket);
	void UpdateBonusStat(const FGsNet::Packet& inPacket);

	void AckBonusStatExtendCount(const FGsNet::Packet& inPacket);
	void UpdateUsedBonusStatItemCount(const FGsNet::Packet& inPacket);

	//death
protected:
	void UserDeath(const FGsNet::Packet& inPacket);	

	//panelty
protected:
	void DeathPenaltyExpList(const FGsNet::Packet& inPacket);
	void AddDeathPenaltyExp(const FGsNet::Packet& inPacket);
	void DeathPenaltyItemList(const FGsNet::Packet& inPacket);
	void AddDeathPenaltyItem(const FGsNet::Packet& inPacket);
	void AckDeathPenaltyRestore(const FGsNet::Packet& inPacket);
	void DeathPenaltyFreeCount(const FGsNet::Packet& inPacket);
	void DeathPenaltyDelete(const FGsNet::Packet& inPacket);
	void AckExpRecoveryCount(const FGsNet::Packet& inPacket);

protected:
	void NotifyArcasBless(const FGsNet::Packet& inPacket);
	void UpdateBlessPoint(const FGsNet::Packet& inPacket);
	void UpdateMeditationTime(const FGsNet::Packet& inPacket);
	void UpdateUseBlessFlag(const FGsNet::Packet& inPacket);

	// Mail
protected:
	void NotifyMailUnread(const FGsNet::Packet& inPacket);	
	void MailListAck(const FGsNet::Packet& inPacket);
	void MailRecvAck(const FGsNet::Packet& inPacket);
	void MailReadAck(const FGsNet::Packet& inPacket);

	//PVP Alignment
protected:
	void UpdatePVPAlignmentPoint(const FGsNet::Packet& inPacket);

	// Dungeon
protected:
	void DungeonGroupListAck(const FGsNet::Packet& inPacket);
	void DungeonGroupChargeNotify(const FGsNet::Packet& inPacket);
	void DungeonGroupEnter(const FGsNet::Packet& inPacket);
	void DungeonGroupLeave(const FGsNet::Packet& inPacket);
	void DungeonGroupInfo(const FGsNet::Packet& inPacket);
	void PartyDungeonInfo(const FGsNet::Packet& inPacket);
	// 2022/06/10 PKT - 파티 던전 진행 사항(던전 내 진행 사항)
	void DungeonNotifyPatyDungeonStatus(const FGsNet::Packet& inPacket);
	// 2022/06/10 PKT - 파티 던전 결과
	void DungeonNotifyPatyDungeonResult(const FGsNet::Packet& inPacket);
	// 2022/08/12 PKT - 정예 던전 프랍 포털 정보
	void EliteDungeonPortalInfo(const FGsNet::Packet& inPacket);

	// 천마석(정령탄) spirit shot
protected:
	// local 천마석 ui on off 응답
	void AckSpiritShotOnOff(const FGsNet::Packet& inPacket);
	void UserSpiritShotActivation(const FGsNet::Packet& inPacket);	

protected:
	// 2021/06/21 PKT - 오프라인 플레이 결과 확인 창
	void NotifyOfflinePlayResult(const FGsNet::Packet& inPacket);
	void NotifyOfflinePlayRemainingTimeData(const FGsNet::Packet& inPacket);
	void NotifyOfflinePlayCancel(const FGsNet::Packet& inPacket);
	void AckOfflinePlayReadyState(const FGsNet::Packet& inPacket);
	void AckOfflinePlayStartPlay(const FGsNet::Packet& inPacket);
	void AckOfflinePlayDebugMode(const FGsNet::Packet& inPacket);

protected:
	// 레드닷 알림 통합
	void ContentsStatePacket(const FGsNet::Packet& inPacket);

protected:
	// 통계
	void StatisticsInfoAck(const FGsNet::Packet& inPacket);
	void StatisticsDailyInfoAck(const FGsNet::Packet& inPacket);

protected:
	void AckContentsLocked(const FGsNet::Packet& inPacket);

	//침공전
protected:
	void AckInvadeInfo(const FGsNet::Packet& inPacket);
	void AckSpawnedInvasionPortalInfo(const FGsNet::Packet& inPacket);
	void AckActiveInvasionPortalList(const FGsNet::Packet& inPacket);
	//침공 공격자 침공 시작 알림 패킷 
	void InvadeEnter(const FGsNet::Packet& inPacket);
	//침공 공격자 침공 끝 알림 패킷 
	void InvadeLeave(const FGsNet::Packet& inPacket);
	void InvadeInfo(const FGsNet::Packet& inPacket);
	void InvadeNumOfCurrentInvader(const FGsNet::Packet& inPacket);
	void InvadeNumOfInvaderViaPortal(const FGsNet::Packet& inPacket);
	void OnInvadeStart(const FGsNet::Packet& inPacket);
	void NotifyStartInvasionTime(const FGsNet::Packet& inPacket);

	//침공자가 마을 무법 지대에 있을 때 상태 변화
	void TerritoryUpdate(const FGsNet::Packet& inPacket);
	//침공전 방어자 침공 시작 알림 패킷 
	void InvadeStart(const FGsNet::Packet& inPacket);
	//침공전 방어자 침공 끝 알림 패킷 
	void InvadeFinish(const FGsNet::Packet& inPacket);

	// Tutorial
protected:
	void TutorialList(const FGsNet::Packet& inPacket); // 월드 입장 시
	void TutorialSaveAck(const FGsNet::Packet& inPacket);

	// 대결
protected:
	void AckDuelInvite(const FGsNet::Packet& inPacket);
	void AckDuelInviteCancel(const FGsNet::Packet& inPacket);
	void AckDuelAccept(const FGsNet::Packet& inPacket);
	void NotifyDuelInviteCancel(const FGsNet::Packet& inPacket);
	void NotifyDuelInvited(const FGsNet::Packet& inPacket);
	void NotifyDuelInviteResult(const FGsNet::Packet& inPacket);
	void NotifyDuelCountStart(const FGsNet::Packet& inPacket);
	void NotifyDuelStart(const FGsNet::Packet& inPacket);
	void NotifyDuelResult(const FGsNet::Packet& inPacket);
	void NotifyDuelReconnectInfo(const FGsNet::Packet& inPacket);

	// party target share
protected:
	// notify target share 
	void PartyTarget(const FGsNet::Packet& inPacket);
	// notify target share cancel
	void PartyTargetCancel(const FGsNet::Packet& inPacket);

private:
	void AckUserStatBookmarkList(const FGsNet::Packet& inPacket);
	void AckUserStatBookmarkAdd(const FGsNet::Packet& inPacket);
	void AckUserStatBookmarkDelete(const FGsNet::Packet& inPacket);

protected:
	void CharacterSlotUnLock(const FGsNet::Packet& inPacket);

protected:
	void AckUserPresetChange(const FGsNet::Packet& inPacket);
	void AckUserPresetCopyTo(const FGsNet::Packet& inPacket);
	void AckUserPresetName(const FGsNet::Packet& inPacket);
	void NotifyUserPresetName(const FGsNet::Packet& inPacket);

	// region map: invade info
private:
	void InvadeMinimapInfoUpdate(const FGsNet::Packet& inPacket);

protected:
	// 시네마틱
	void AckCinematicStart(const FGsNet::Packet& inPacket);
	void AckCinematicEnd(const FGsNet::Packet& inPacket);

protected:
	void AckPersonalTradingTry(const FGsNet::Packet& inPacket);
	void AckPersonalTradingAccept(const FGsNet::Packet& inPacket);
	void AckPersonalTradingCancel(const FGsNet::Packet& inPacket);
	void AckPersonalTradingAbort(const FGsNet::Packet& inPacket);
	void AckPersonalTradingLock(const FGsNet::Packet& inPacket);
	void AckPersonalTradingConfirm(const FGsNet::Packet& inPacket);
	void AckPersonalTradingPutItem(const FGsNet::Packet& inPacket);
	void AckPersonalTradingPutDia(const FGsNet::Packet& inPacket);
	void NotifyPersonalTrading(const FGsNet::Packet& inPacket);
	void NotifyPersonalTradingItemUpdate(const FGsNet::Packet& inPacket);
	void NotifyPersonalTradingItemEquipUpdate(const FGsNet::Packet& inPacket);
	void NotifyPersonalTradingItemRemove(const FGsNet::Packet& inPacket);
	void NotifyPersonalTradingDiaUpdate(const FGsNet::Packet& inPacket);
	void CheatPersonalTradingDailyTradingCntUpdate(const FGsNet::Packet& inPacket);

protected:
	// Latency Test 패킷
	void AckLatency(const FGsNet::Packet& inPacket);

protected:
	void AckUserContextOtherServer(const FGsNet::Packet& inPacket);

	// summon
protected:
	// 페어리 뽑기 마일리지 보상 응답
	void AckFairySummonMileage(const FGsNet::Packet& inPacket);
	// 코스튬 뽑기 마일리지 보상 응답
	void AckCostumeSummonMileage(const FGsNet::Packet& inPacket);
	// 유니크 페어리 뽑기 마일리지 보상 응답
	void AckUniqueFairySummonMileage(const FGsNet::Packet& inPacket);
	// 유니크 코스튬 뽑기 마일리지 보상 응답
	void AckUniqueCostumeSummonMileage(const FGsNet::Packet& inPacket);
	// 유니크 마일리지로 뽑은 횟수 정보
	void UniqueMileageObtainedInfo(const FGsNet::Packet& inPacket);

protected:
	//buddy
	void BuddyLoginNotify(const FGsNet::Packet& inPacket);
	void BuddyLogoutNotify(const FGsNet::Packet& inPacket);
};
