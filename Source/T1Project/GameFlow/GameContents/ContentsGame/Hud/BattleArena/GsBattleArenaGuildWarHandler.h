#pragma once

#include "CoreMinimal.h"
#include "GsBattleArenaData.h"
#include "../UTIL/GsTimeUtil.h"
#include "../Message/GsMessageStage.h"
#include "../Message/GsMessageSystem.h"
#include "../Message/GsMessageContents.h"
#include "../Message/GsMessageContentDungeon.h"
#include "../Message/MessageParam/GsItemMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"



class T1PROJECT_API FGsBattleArenaGuildWarHandler
{
private:
	MapId _mapId = INVALID_MAP_ID;
	EBattleArenaGuildWarState _state = EBattleArenaGuildWarState::ENone;
	time_t _stateEndTime;

	TArray<FGsBattleArenaEntryInfo> _entryMemberSet;

	FGsBattleArenaGuildWarTeamInfo _redTeamInfo;
	FGsBattleArenaGuildWarTeamInfo _blueTeamInfo;
	
	FGSBattleArenaGuilWarResultData _endResultData;

	TArray<FGsSimpleTimer> _countdownTickerTimerSet;

private:
	MsgSystemHandleArray _systemMsg;
	MsgGameObjHandleArray _gameObjectMsg;
	MsgDungeonHandleArray _listDungeonDelegate;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemActionDelegates;	
	TArray<TPair<MessageContentUnlock, FDelegateHandle>> _unlockDelegateList;
	TArray<TPair<MessageContentGuild, FDelegateHandle>> _guildMsgHandler;

public:
	FGsBattleArenaGuildWarHandler() = default;
	virtual ~FGsBattleArenaGuildWarHandler();

	void Init();
	void Close();
	void OnReconection();

	void Update(float InDeltaTime);

protected:
	void Destory();
	void ResetData();
	void BindMessage();
	void UnBindMessage();
	
	// 2023/11/22 PKT - State Change
	void ChangedState(EBattleArenaGuildWarState InNextState);
	// 2023/11/22 PKT - Send Message
	void SendWindowMessage(EBattleArenaGuildWarState InPrevState, EBattleArenaGuildWarState InCurrentState) const;
	// 2023/11/22 PKT - 서버 타입 -> 클라 타입
	EBattleArenaGuildWarState ConvertServerStateToClientState(GuildWarState InGuildWarState, bool InIsRegister = false) const;

	/**
	 * Server -> Client
	 */
public:
	// 2024/1/10 PKT - 그림자 전장 시작 티커용 패킷
	void ArenaStartAlarm(MapId InMapId);
	// 2023/11/30 PKT - Arena 입장 전 진행 상태(Arena '외' 에서 발생 되는 상태)
	void UpdateState(GuildWarState InState, MapId InMapId, const TArray<FGsBattleArenaEntryInfo>& InEntrySet, int32 maxCapacity, time_t InEndTime);
	// 2023/11/30 PKT - Arena 입장 후 진행 상태(Arena '내' 에서 발생 되는 상태)
	void UpdateState(GuildWarState InState, const FGsBattleArenaGuildWarTeamInfo& InRedTeam, const FGsBattleArenaGuildWarTeamInfo& InBlueTeam, time_t InEndTime);
	// 2023/11/23 PKT - 참여 신청 업데이트(본인이 신청한 결과에 대한 응답)
	void UpdateState(bool InRegister);
	// 2023/11/29 PKT - 진행 중 모든 상태를 초기화 해야 하는 경우가 있을 수 있음.
	void ContentsShotDown();
	// 2023/11/23 PKT - 클라가 요청한 BattleArena 디테일 점수 정보
	void DetailScore(const FGsBattleArenaGuildWarTeamInfo& InRedTeam, const FGsBattleArenaGuildWarTeamInfo& InBlueTeam);
	// 2023/11/28 PKT - 게임 종료에 대한 결과
	void PlayEndResult(GuildWarEndReason InReason, GuildDBId InWinGuildId, int32 InMyPoint, const FGsBattleArenaGuildWarTeamInfo& InRedTeam, const FGsBattleArenaGuildWarTeamInfo& InBlueTeam);
	// 2023/12/27 PKT - Member Position Info
	void MemberPositionInfo(const FGsBattleArenaMemberPositionInfos& InRedTeam, const FGsBattleArenaMemberPositionInfos& InBlueTeam);
	
	/**
	 * Client -> Server
	 */
	// 2023/11/23 PKT - 신청 등록
	void TryToRegisterOrCancel();
	// 2023/11/23 PKT - 아레나 입장
	void TryToEnterBattleArenaGuildWar();	
	// 2023/11/23 PKT - 점수 현황 요청
	void SendReqGuildWarScoreInfo();

	/**
	 * Client -> Client
	 */
	void TryToOpenPlayDetailResult();

protected:
	/**
	 * Client -> Client
	 */
	void OnLoadTopoStart(const struct IGsMessageParam* inParam);
	// 2023/12/12 PKT - Spawn( UI 세팅 용 )
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam*);
	// 2023/12/12 PKT - Checked Contents
	void OnContetnsUnlock(const struct IGsMessageParam* InParm);

	// 2023/12/12 PKT - Arena Exit
	void OnExitBattleArena(const struct IGsMessageParam* InParam);
	
	// 2023/12/12 PKT - Item Update
	void OnAddToInventory(struct FGsItemMessageParamBase& InParam);
	void OnChangeToInventory(struct FGsItemMessageParamBase& InParam);
	void OnRemoveToInventory(struct FGsItemMessageParamBase& InParam);

	// 2023/12/22 PKT - Guild
	void OnLeaveGuild(const struct IGsMessageParam* inMessage);

	bool CheckedCoinItem(ItemId InItemId);

	enum EResult {
		Vaild, InvalidUnKnown, InvalidMapData, InvalidUnlockContents
		, InvalidNotFindGuildInfo, InvalidGuildMemberGrade, InvalidGuildLevel
		, InvalidImpossibleWarp, InvalidAlreadyResiger, InvalidEnterState
		, InvalidateEntranceWait
	};
	// 2023/12/12 PKT - 신청 가능 한가?
	EResult CheckedRegisterCondition(bool InIsMessage = false) const;
	// 2023/12/12 PKT - 입장 가능 한가?
	EResult CheckedEnterCondition(bool InIsMessage = false) const;
	// 2023/12/12 PKT - 가능 여부에 대해 불가한 경우에 대한 Ticker 처리
	void ShowTickerMessage(EResult InResult) const;

	bool IsHiddenArena() const;

	void SetCountDownTicker(EBattleArenaGuildWarState InState);

public:
	// 2023/12/4 PKT - 진행 하려고 하는 MapId
	MapId GetMapId() const { return _mapId; }
	// 2023/12/4 PKT - 상태(Server -> Client Type으로 컨버팅 한 것.)
	EBattleArenaGuildWarState GetState() const { return _state; }
	// 2023/12/4 PKT - Arena 신청 한 인원 수(입장 인원이 아님)
	int32 GetEntryCount() const { return _entryMemberSet.Num(); }
	// 2023/12/4 PKT - 각 상태 끝 나는 시간
	time_t GetStateEndTime() const { return _stateEndTime; }
	// 2023/12/6 PKT - 각 상태에 따른 남은 시간을 Text로 리턴
	FText TextStateRemainingTime() const;
	// 2023/12/12 PKT - Arena 주화 갯수
	ItemAmount GetCoinAmount() const;
		
	// 2023/12/4 PKT - 팀 정보
	const FGsBattleArenaGuildWarTeamInfo& RedTeam() const { return _redTeamInfo; }
	const FGsBattleArenaGuildWarTeamInfo& BlueTeam() const { return _blueTeamInfo; }

	const struct FGsSchemaGuildWarConfigData* GetConfigData(const MapId InMapId) const;

	// 2024/1/4 PKT - 결과 정보
	const FGSBattleArenaGuilWarResultData* GetResultData() const { return &_endResultData; }

	/**
	 * 연출
	 */
public:
	// 2023/12/5 PKT - 카운트 다운을 예약
	void ReservedShowTickerStartCount() const;

	void OpenWindowPlayResult() const;
};