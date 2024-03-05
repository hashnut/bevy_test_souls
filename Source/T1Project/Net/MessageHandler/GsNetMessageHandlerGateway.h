// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/GSNet.h"
#include "GsNetMessageHandleInterface.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"


/**
 *
 */
class T1PROJECT_API FGsNetMessageHandlerGateway : public IGsNetMessageHandler, FGsNetMessageGateway
{
public:
	FGsNetMessageHandlerGateway() = default;
	virtual ~FGsNetMessageHandlerGateway() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

public:
	void CreateUserAck(const FGsNet::Packet &inPacket);
	void DeleteUserAck(const FGsNet::Packet &inPacket);
	void DeleteUserCancelAck(const FGsNet::Packet &inPacket);
	void GatewayLoginAck(const FGsNet::Packet &inPacket);
	void GatewayZpayLoginAck(const FGsNet::Packet& inPacket);
	void GameGuardCSA(const FGsNet::Packet& inPacket);
	void LoginWatingInfoAck(const FGsNet::Packet& inPacket);
	void LoginWatingCancelAck(const FGsNet::Packet& inPacket);
	void UserListAck(const FGsNet::Packet &inPacket);
	void SelectUserAck(const FGsNet::Packet &inPacket);
	void ReturnToLobbyAck(const FGsNet::Packet &inPacket);
	void WorldTimeAck(const FGsNet::Packet& inPacket);
	// 기간제 컨텐츠 리셋(일일, 주별, 월별, ...)
	void ResetTimeTicketNotify(const FGsNet::Packet& inPacket);

public:
	// 누적 세금
	void AckContinentTax(const FGsNet::Packet& InPacket);

	// 던전
public:
	void DungeonGroupResetTimeTicketNotify(const FGsNet::Packet& inPacket);
	
	// 2022/05/30 PKT - 던전 그룹내 파티목록(협동던전 - 파티 리스트)
	void DungeonGroupPartyListAck(const FGsNet::Packet& inPacket);
	// 2023/6/7 PKT - 던전 참여 시도에 대한 결과
	void PartyDungeonGroupJoin(const FGsNet::Packet& inPacket);
	// 2022/06/16 PKT - 파티 던전을 위해 파티가 결성 되면 오는 패킷(노티)
	void PartyDungeonGroupInfo(const FGsNet::Packet& inPacket);	
	// 2022/06/16 PKT - 파티 던전 매칭 요청에 대한 패킷
	void PartyDungeonGroupMathcingAck(const FGsNet::Packet& inPacket);
	// 2022/06/16 PKT - 파티 던전 매칭 취소에 대한 응답
	void PartyDungeonGroupMathcingCancelAck(const FGsNet::Packet& inPacket);
	// 2022/06/16 PKT - 파티 던전 매칭 실패에 대한 노티
	void PartyDungeonGroupMathcingFailed(const FGsNet::Packet& inPacket);
	// 2022/06/16 PKT - 난입 실패에 대한 노티
	void PatyDungeonForceEnterFailed(const FGsNet::Packet& inPacket);
	// 2022/06/19 PKT - 파티 던전 시작에 대한 응답
	void PartDungeonInDungeonNotify(const FGsNet::Packet& inPacket);
	// 2022/06/29 PKT - 도움 요청에 대한 응답
	void partyDungeonAskHelpAck(const FGsNet::Packet& inPacket);
	// 2022/07/08 PKT - 도움 요청에 대한 정보
	void partyDungeonAskHelpNotify(const FGsNet::Packet& inPacket);

	// 플레이어블 이벤트 
public:
	void PlayableEventUpdateAck(const FGsNet::Packet& inPacket);

	//void OperatorNotify(const FGsNet::Packet& inPacket);

public:
	void ServerPingAck(const FGsNet::Packet& inPacket);

public:
	// 랭킹 목록 응답
	void AckRankingList(const FGsNet::Packet& inPacket);

protected:
	void ShowMessageTicker(const FTextKey& InNamespace, const FTextKey& InKey);

protected:
	void UnlockInfo(const FGsNet::Packet& inPacket);
	void UpdateUnlockInfoAck(const FGsNet::Packet& inPacket);
	void ContentsLockedAck(const FGsNet::Packet& inPacket);
	void LockedContentsListNotify(const FGsNet::Packet& inPacket);
	void ContentsUnlockUpdateNotify(const FGsNet::Packet& inPacket);

	// 소셜 컨텐츠 : 프로필
protected:
	void AckUserProfile(const FGsNet::Packet& inPacket);
	void AckUserProfileIntroductionUpdate(const FGsNet::Packet& inPacket);
	void AckUserProfileKeywordInfo(const FGsNet::Packet& inPacket);
	void AckUserProfileKeywordUpdate(const FGsNet::Packet& inPacket);
	void AckUserProfileCommentList(const FGsNet::Packet& inPacket);
	void AckUserProfileCommentInput(const FGsNet::Packet& inPacket);
	void AckUserProfileCommentDelete(const FGsNet::Packet& inPacket);
	void NotifyUserProfile(const FGsNet::Packet& inPacket);
	void AckUserProfileNickNameChangeList(const FGsNet::Packet& inPacket);

protected:
	void AckUserContext(const FGsNet::Packet& inPacket);

	// 커뮤니티 소녀 말풍선
protected:
	void AckCommunityTalkRank(const FGsNet::Packet& inPacket);
	void AckCommunityTalkMarket(const FGsNet::Packet& inPacket);

protected:
	// 서버 Revision
	void AckServerRevisionInfo(const FGsNet::Packet& inPacket);

	// window KeyboardMapping
protected:
	void AckKeyboardMappingSave(const FGsNet::Packet& inPacket);
	void AckKeyboardMappingLoad(const FGsNet::Packet& inPacket);

	// 제작
protected:
	/**
	 * 서버 별 수량 변화 알림
	 * 서버로부터 "제작갯수 업데이트" 등의 패킷을 받으면 발생.
	 * 이 이벤트를 수신하는 곳에서, "새로운 제작갯수 요청" 등을 수행함
	 */
	void NOTIFY_CRAFT_STAT_LIST_WORLD_CHANGED(const FGsNet::Packet& InPacket);

	/**
	 * CS_REQ_CRAFT_STAT_LIST 의 응답
	 *
	 * * CraftCategoryId craftCategoryId: 제작 카테고리 Id.
	 * * TArray<CraftIdAmountPair> craftStatList0: 현재까지 서버에 제작된 개수 목록.
	 * * TArray<CraftIdAmountPair> craftStatList1: 현재까지 서버 계정에 제작된 개수 목록.
	 */
	void ACK_CRAFT_STAT_LIST(const FGsNet::Packet& InPacket);

protected:
	// 신규 시스템 우편 알림
	void NotifyNewSystemMail(const FGsNet::Packet& inPacket);

public:
	// 계정삭제
	void AckDeleteAccount(const FGsNet::Packet& inPacket);

public:
	// 외형변경 요청 결과
	void AckCharacterCustomizing(const FGsNet::Packet& inPacket);

	// 계정충돌
	void IdpConflictPlayerAck(const FGsNet::Packet& inPacket);

public:
	// 강제 접속 종료
	void AckLeaveToInActivity(const FGsNet::Packet& inPacket);

public:
	/** 필드 보스 마지막 처치 시간 */
	void AckCompetitionFieldBossLastDeadTime(const FGsNet::Packet& inPacket);

	// 닉네임 변경권
public:
	void AckCheckNickNameChange(const FGsNet::Packet& inPacket);
	void AckDecideNickNameChange(const FGsNet::Packet& inPacket);
	void NotifyBuddyNickNameChange(const FGsNet::Packet& inPacket); // 친구가 닉네임 변경 시 캐싱데이터 교체 위함	
	bool CheckNickNameError(PD::Result InResult);	

	// 성소(지도 정보 응답)
	// 처음엔 FGsNetMessageHandlerGameObject에 있었는데
	// 차준규 차석님 요청으로 gateway로 변경
public:
	void AckMapSanctumInfo(const FGsNet::Packet& inPacket);

	void AckMigrationNickNameChanged(const FGsNet::Packet& inPacket);
	void AckMigrationGuildNameChanged(const FGsNet::Packet& inPacket);
};
