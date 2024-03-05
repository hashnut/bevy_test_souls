// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/SharedCommunityEventEnum.h"

#include "GsManager.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Community/GsCommunityInfo.h"
#include "CommunityEventMission/GsCommunityEventMissionData.h"
#include "Option/GsGameUserSettings.h"


struct FGsSchemaCommunityEventMission;
struct FGsSchemaCommunityEvent;
class FGsAttendanceData;
/**
 *  출석체크 매니져
 */
class T1PROJECT_API FGsCommunityEventManager final : public IGsManager
{
private:
	TMap<CommunityEventId, TSharedPtr<FGsCommunityData>>					_communityDataMap;
	TMap<AttendanceId, TSharedPtr<FGsAttendanceData>>						_attendanceMap;
	TMap<CommunityEventMissionId, TSharedPtr<FGsCommunityEventMissionData>>	_missionDataMap;
	
	TArray<UGsGameUserSettings::FBannerInfo>								_bannerInfo;

	MsgGameObjHandle										_gameObjectMessage;
	TPair<MessageContentCommunity, FDelegateHandle>			_communityDelegate;

	bool	_newEventStartAlert = false;
	float	_updateSec			= 1.0f;
	float	_updateRemainSec	= 1.0f;
	
public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;

	virtual void Update(float inTick) override;

public:
	// 커뮤니티 목록 요청 응답
	void Set(struct PD::SC::PKT_SC_ACK_COMMUNITY_EVENT_LIST_READ& Packet);
	// 출석 체크 결과
	void Set(struct PD::SC::PKT_SC_ACK_CHECK_ATTENDANCE_READ& Packet);
	// 출석부 구매 요청 응답
	void Set(struct PD::SC::PKT_SC_ACK_PURCHASE_ATTENDANCE_READ& Packet);

	// 커뮤니티 소녀 이벤트 미션 정보 
	void Set(struct PD::SC::PKT_SC_COMMUNITY_EVENT_MISSION_READ& Packet);
	// 커뮤니티 소녀 이벤트 미션 보상 응답
	void Set(struct PD::SC::PKT_SC_ACK_COMMUNITY_EVENT_MISSION_REWARD_READ& Packet);
	// 커뮤니티 소녀 이벤트 미션 진행도 알림
	void Set(struct PD::SC::PKT_SC_INFORM_COMMUNITY_EVENT_MISSION_PROGRESS_READ& Packet);

	// 커뮤니티 이벤트 리스트 요청
	void SendReqCommunityList();
	// 출석부 구입 요청
	void SendReqPurchaseAttendance(AttendanceId inId);
	// 출첵 요청
	void SendReqCheckAttendance(AttendanceId inId, AttendanceCheckIdx inIndex);
	// 커뮤니티소녀 미션 보상 요청
	void SendReqMissionReward(CommunityEventMissionId InMissionId, CommunityEventMissionTabId InTabId,
		CommunityEventMissionCommonId InCommonId, bool InIsBonus);

	// 티켓 리셋
	void ResetTimeContents(ResetTimeTicket InType);

	// 레드닷 갱신 (출석과 배너 둘 다)
	void UpdateRedDot(const struct IGsMessageParam*);

	// 커뮤니티 ui창 열렸을때
	void OpenCommunityEventUI();

protected:
	// 서버에서 내려준 에러타입 별 티커
	//void AttendanceErrorMsg(int inResult);
	void CommunityErrorMsg(int inResult);
	void GetAttendanceNameText(IN AttendanceId inAttendanceId, OUT FText& outNameText);
	int8 GetCommunityEventMissionTabIndex(CommunityEventMissionId  inMissionId, CommunityEventMissionTabId inTabId);
	int8 GetCommunityEventMissionCommonIndex(CommunityEventMissionTabId inTabId, CommunityEventMissionCommonId  inCommonId);
	const FGsSchemaCommunityEvent* GetAttendanceSchemaCommunityEvent(IN AttendanceId inAttendanceId);
	const FGsSchemaCommunityEvent* GetMissionSchemaCommunityEvent(IN CommunityEventMissionId inMissionId);

protected:
	void SetCommunityEventData();

	void OnInitializeUserData(const IGsMessageParam* param);
public:
	TMap<CommunityEventId, TSharedPtr<FGsCommunityData>>* GeCommunityEventDataDataList() { return &_communityDataMap; }
	void GetCommunityEventDataList(OUT TArray<CommunityEventId>& outCommunityEventIdList);

	TSharedPtr<FGsCommunityData> GetCommunityEventData(CommunityEventId inCommunityEventId)
	{
		if (_communityDataMap.Contains(inCommunityEventId))
		{
			return _communityDataMap.FindRef(inCommunityEventId);
		}

		return nullptr;
	}

	// 출석 리스트
	TMap<AttendanceId, TSharedPtr<FGsAttendanceData>>* GeAttendanceDataList() { return &_attendanceMap; }
	int GetAttendanceDataListCount() { return _attendanceMap.Num(); }

	TSharedPtr<FGsAttendanceData> GetAttendanceData(int inAttendanceId)
	{
		if (_attendanceMap.Contains(inAttendanceId))
		{
			return _attendanceMap.FindRef(inAttendanceId);
		}
		
		return nullptr;	
	}

	TSharedPtr<FGsCommunityEventMissionData> GetMissionData(CommunityEventMissionId inMissionId)
	{
		if (_missionDataMap.Contains(inMissionId))
		{
			return _missionDataMap.FindRef(inMissionId);
		}

		return nullptr;
	}

public:
	bool IsRedDot();
	bool IsRedDot(CommunityEventId inId);
	void SetRedDot(CommunityEventId inId);
	void SaveRedDot();

	void LoadBannerRedDot();
	bool IsCommunityEventMissionTabLock(CommunityEventId inEventId,
		CommunityEventMissionId inEventMissionId, CommunityEventMissionTabId inEventMissionTabId);

private:
	bool IsCurrentRedDot();
	void CheckNewEventAttendance();
	void CheckNewEventMission();
	void EventStartAlert();

	const TArray<const FGsSchemaCommunityEvent*> GetSchemaCommunityEventList() const;
};

#define GSCommunityEvent() UGsScopeHolder::GetGameManagerFType<FGsCommunityEventManager>(UGsScopeGame::EManagerType::CommunityEvent)