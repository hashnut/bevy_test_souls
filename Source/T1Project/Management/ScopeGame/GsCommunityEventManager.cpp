// Fill out your copyright notice in the Description page of Project Settings.
#include "Management/ScopeGame/GsCommunityEventManager.h"

#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Management/GsScopeGlobal.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "ManageMent/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Item/GsItemManager.h"

#include "Attendance/GsAttendanceDefine.h"
#include "Attendance/GsAttendanceInfo.h"
#include "Attendance/GsSchemaAttendanceEvent.h"
#include "Attendance/Reward/GsSchemaAttendanceCurrency.h"
#include "Attendance/Reward/GsSchemaAttendanceItem.h"
#include "CommunityGirl/GsSchemaCommunityEvent.h"
#include "CommunityEventMission/GsCommunityEventMissionData.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"
#include "UI/UIContent/Popup/GsUIPopupCommonItemConsume.h"

#include "Currency/GsCostPackage.h"

#include "T1Project.h"
#include "Attendance/GsSchemaCommunityEventAttendance.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Option/GsGameUserSettings.h"
#include "Copy.h"

#include "Hive/GsHiveEventMsg.h"
#include "UTIL/GsTimeSyncUtil.h"


void FGsCommunityEventManager::Initialize()
{	
	FGsMessageHolder* msg = GMessage();
	auto& communityeHandler = msg->GetCommunity();
	_communityDelegate = communityeHandler.AddRaw(MessageContentCommunity::REDDOT_UPDATE, this,
		&FGsCommunityEventManager::UpdateRedDot);

	_gameObjectMessage = msg->GetGameObject().AddRaw(MessageGameObject::UPDATE_USER_DATA,
		this, &FGsCommunityEventManager::OnInitializeUserData);

	SetCommunityEventData();
}

void FGsCommunityEventManager::Finalize()
{
	FGsMessageHolder* msg = GMessage();
	auto& communityeHandler = msg->GetCommunity();
	communityeHandler.Remove(_communityDelegate);

	msg->GetGameObject().Remove(_gameObjectMessage);

	_communityDataMap.Empty();
	_attendanceMap.Empty();
	_missionDataMap.Empty();
}

void FGsCommunityEventManager::Update(float inTick)
{
	if (0 < _updateSec)
	{
		_updateRemainSec -= inTick;

		if (0 > _updateRemainSec)
		{
			_updateRemainSec = _updateSec;
			
			for (auto& iter : _communityDataMap)
			{
				TSharedPtr<FGsCommunityData> communityData = iter.Value;
				if (!communityData.IsValid())
					continue;

				if (communityData->GetConditionType())
				{
					if (false == communityData->IsValid())
					{
						GMessage()->GetCommunity().SendMessage(MessageContentCommunity::UPDATE_EVENT_EXPIRE, nullptr);
						communityData->SetConditionType(false);
					}
				}

				if (communityData->GetIsStartWaiting())
				{
					if (communityData->IsOpen())
					{
						EventStartAlert();
					}
				}
			}
		}
	}
}

void FGsCommunityEventManager::EventStartAlert()
{
	_newEventStartAlert = true;
	// HUD 관련 레드닷 갱신 요청	
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COMMUNITY, true);
}

void FGsCommunityEventManager::OpenCommunityEventUI()
{
	_newEventStartAlert = false;
}

// 커뮤니티 이벤트 리스트 요청
void FGsCommunityEventManager::SendReqCommunityList()
{
	FGsNetSendServiceWorld::SendReqCommunityList();
}

// 커뮤니티 목록 요청 응답
void FGsCommunityEventManager::Set(PD::SC::PKT_SC_ACK_COMMUNITY_EVENT_LIST_READ& Packet)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[Attendance] ACK_COMMUNITY_EVENT_LIST"));
#endif
	
	int32_t result = Packet.Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		// 티커 연출
		CommunityErrorMsg(static_cast<PD::Result>(result));
	}
	else
	{
		_attendanceMap.Empty();
		TArray<AttendanceData> attendanceDataList;
		Packet.MakeAttendanceListVector<TArray<AttendanceData>, AttendanceData>(attendanceDataList);
		for (int i = 0; i < attendanceDataList.Num(); ++i)
		{
			AttendanceId id = attendanceDataList[i].mId;				// 출석부 ID
			AttendanceCount	count = attendanceDataList[i].mCount;		// 출석체크 횟수
			bool checkInToday = attendanceDataList[i].mChecked;			// 체크여부
			bool usable = attendanceDataList[i].mUsable;				// 이용가능한가?
			
			// expireData
			time_t	expiredTime = attendanceDataList[i].mExpiredTime;			// 만료시간 (=시작 시간 + durationHoursReceive)
			time_t	complatedTime = attendanceDataList[i].mComplatedTime;		// 데이터상 종료시간 or 모든 보상 받은시간 (최종출석판이 살아있는 기간)

#if WITH_EDITOR
			GSLOG(Log, TEXT("[Attendance] AttendanceData mId: %d, mCount: %d, mChecked: %d, mUsable: %d"),
				id, count, checkInToday, usable);
#endif

			if (const auto schemaAttendance = UGsTableUtil::FindRowById<UGsTableCommunityEventAttendance, FGsSchemaCommunityEventAttendance>(id))
			{
				bool isCompleted = (count >= schemaAttendance->rewardList.Num());

				TSharedPtr<FGsAttendanceData> attendanceData = MakeShared<FGsAttendanceData>(id, count, checkInToday, usable, isCompleted);
				auto data = _attendanceMap.Emplace(id, attendanceData);
				// 데이터 적용
				data->ApplyData();

				if (const FGsSchemaCommunityEvent* schemaCommunityEvent = GetAttendanceSchemaCommunityEvent(id))
				{
					TSharedPtr<FGsCommunityData> communityData = _communityDataMap.FindRef(schemaCommunityEvent->id);
					if (communityData.IsValid())
					{						
						communityData->SetConditionTime(expiredTime, complatedTime);
					}

					data->SetCommunityEventData(schemaCommunityEvent);
					//data->SetStartTime(schemaCommunityEvent->eventTimeStart);
					//data->SetEndTime(schemaCommunityEvent->eventTimeEnd);
				}
			}
		}
	}

	// reddot 갱신`
	UpdateRedDot(nullptr);
	
	// UI 갱신
	GMessage()->GetCommunity().SendMessage(MessageContentCommunity::ACTIVE_WINDOW_INVALIDTEALL, nullptr);
}

// 출석 체크 결과
void FGsCommunityEventManager::Set(PD::SC::PKT_SC_ACK_CHECK_ATTENDANCE_READ& Packet)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[Attendance] ACK_CHECK_ATTENDANCE"));
#endif

	int32_t result = Packet.Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		// 티커 연출
		CommunityErrorMsg(static_cast<PD::Result>(result));
	}
	else
	{
		AttendanceId attendanceId = Packet.AttendanceId();
		AttendanceCheckIdx attendanceCheckIndex = Packet.AttendanceCheckIdx();

		TSharedPtr<FGsAttendanceData> attendanceData = _attendanceMap.FindRef(attendanceId);
		if (nullptr == attendanceData || !attendanceData.IsValid())
			return;

		attendanceData->SetCount(attendanceCheckIndex);
		attendanceData->SetCheckInToday(true);
		// [B1] | ejrrb10 | C2URWQ-4962 | 출석 모두 완료 시, IsCompleted 세팅 (Index 는 0 이 아니라 1부터 시작)
		attendanceData->CheckCompleted(attendanceCheckIndex);

		if (attendanceData->IsAllCompleted())
		{
			// 모든 보상 지급이 끝났다.

			if (const FGsSchemaCommunityEvent* schemaCommunityEvent = GetAttendanceSchemaCommunityEvent(attendanceId))
			{
				TSharedPtr<FGsCommunityData> communityData = _communityDataMap.FindRef(schemaCommunityEvent->id);
				if (communityData.IsValid())
				{
					communityData->SetConditionComplatedTime();
				}
			}
		}

#if WITH_EDITOR
		GSLOG(Log, TEXT("[Attendance] AttendanceData mId: %d, mCount: %d"), attendanceId, attendanceCheckIndex);
#endif

		// 보상을 인벤토리로 지급하였습니다.
		FText findText;
		FText::FindText(TEXT("AttendanceRewardText"), TEXT("UI_Notice_GetReward"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		// 하이브 유저 추적용 이벤트 전송
		FGsHiveEventMsg attendanceMsg(EGsHiveActionType::ATTENDANCE, attendanceId, attendanceCheckIndex);
		GMessage()->GetHiveEvent().SendMessage(MessageContentHiveEvent::SEND_ANALYTICS_EVENT, &attendanceMsg);

		// [U1] | ejrrb10 | 수령 완료시 사운드 연출 추가 (CHR-20871)
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySound2D(TEXT("UI_RewardOne"));
		}
	}
	
	// UI 갱신
	GMessage()->GetCommunity().SendMessage(MessageContentCommunity::ACTIVE_WINDOW_INVALIDTEALL, nullptr);
}

// 출석부 구매 요청 응답
void FGsCommunityEventManager::Set(PD::SC::PKT_SC_ACK_PURCHASE_ATTENDANCE_READ& Packet)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[Attendance] ACK_PURCHASE_ATTENDANCE"));
#endif

	int32_t result = Packet.Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		// 티커 연출
		CommunityErrorMsg(static_cast<PD::Result>(result));
	}
	else
	{
		AttendanceId attendanceId = Packet.AttendanceId();	
		
		TSharedPtr<FGsAttendanceData> attendanceData = _attendanceMap.FindRef(attendanceId);
		if (nullptr == attendanceData || !attendanceData.IsValid())
			return;

		attendanceData->SetUsable(true);

		FText outNameText;
		GetAttendanceNameText(attendanceId, outNameText);

		// 보상이 우편함으로 갔다.
		FText findText;
		FText::FindText(TEXT("AttendanceRewardText"), TEXT("Notice_Unlock"), findText);
		findText = FText::Format(findText, outNameText);

		FGsUIHelper::TrayMessageTicker(findText);
	}

	// UI 갱신
	GMessage()->GetCommunity().SendMessage(MessageContentCommunity::ACTIVE_WINDOW_INVALIDTEALL, nullptr);
}

// 커뮤니티 소녀 이벤트 미션 정보 
void FGsCommunityEventManager::Set(PD::SC::PKT_SC_COMMUNITY_EVENT_MISSION_READ& Packet)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[CommunityEventMission] SC_COMMUNITY_EVENT_MISSION"));
#endif
	_missionDataMap.Empty();

	// 미션
	for (PD::SC::PKT_SC_COMMUNITY_EVENT_MISSION_READ::CommunityEventMissionListIterator iter = Packet.GetFirstCommunityEventMissionListIterator();
		iter != Packet.GetLastCommunityEventMissionListIterator(); ++iter)
	{
		CommunityEventMissionId missionId = iter->MissionId();	// 미션 Id
		bool isRewardedBonus = iter->IsRewardedBonus();			// 미션 보너스 보상을 받았는가?

		// expireData
		time_t	expiredTime = iter->ExpiredDate();			// 만료시간 (=시작 시간 + durationHoursReceive)
		time_t	complatedTime = iter->ComplatedDate();		// 데이터상 종료시간 or 모든 보상 받은시간 (최종출석판이 살아있는 기간)

		CommunityEventId eventId = INVALID_COMMUNITY_EVENT_ID;
		if (const FGsSchemaCommunityEvent* schemaCommunityEvent = GetMissionSchemaCommunityEvent(missionId))
		{
			eventId = schemaCommunityEvent->id;
			TSharedPtr<FGsCommunityData> communityData = _communityDataMap.FindRef(eventId);
			if (communityData.IsValid())
			{
				communityData->SetConditionTime(expiredTime, complatedTime);
			}
		}

		TSharedPtr<FGsCommunityEventMissionData> missionData = _missionDataMap.Emplace(
			missionId, MakeShared<FGsCommunityEventMissionData>(missionId, isRewardedBonus));

#if WITH_EDITOR
		GSLOG(Log, TEXT("MissionId : %llu, IsRewardedBonus : %d"), missionId, (int)isRewardedBonus);
#endif
		// 탭
		for (PD::SC::PKT_SC_COMMUNITY_EVENT_MISSION_READ::CommunityEventMissionList::TabListIterator tabiter = iter->GetFirstTabListIterator();
			tabiter != iter->GetLastTabListIterator(); ++tabiter)
		{
			CommunityEventMissionTabId tabId = tabiter->TabId();	// 탭 Id

			TSharedPtr<FGsCommunityEventMissionTab> missionTabData = MakeShared<FGsCommunityEventMissionTab>(tabId, GetCommunityEventMissionTabIndex(missionId, tabId));

			GSLOG(Log, TEXT("TabId : %llu"), tabId);

			// 커먼 (임무)
			for (PD::SC::PKT_SC_COMMUNITY_EVENT_MISSION_READ::CommunityEventMissionList::TabList::CommonListIterator commoniter = tabiter->GetFirstCommonListIterator();
				commoniter != tabiter->GetLastCommonListIterator(); ++commoniter)
			{
				CommunityEventMissionCommonId commonId = commoniter->CommonId();	// 임무 Id
				int32 missionCount = commoniter->MissionCount();					// 임무 카운트
				bool isRewarded = commoniter->IsRewarded();							// 보상받았다?

				GSLOG(Log, TEXT("CommonId : %llu, MissionCount : %llu, IsRewarded : %d"), commonId, missionCount, (int)isRewarded);

				int8 commonIndex = GetCommunityEventMissionCommonIndex(tabId, commonId);

				TSharedPtr<FGsCommunityEventMissionCommon> missionCommonData = MakeShared<FGsCommunityEventMissionCommon>(commonId, missionCount, isRewarded, commonIndex);
				missionTabData->AddMissionCommonData(commonId, missionCommonData);
				missionCommonData->ApplyData();
			}

			missionData->AddMissionTabData(tabId, missionTabData);
			missionTabData->ApplyData();

			// 탭
			IsCommunityEventMissionTabLock(eventId, missionId, tabId);
		}

		missionData->ApplyData();

		/*if (const FGsSchemaCommunityEvent* schemaCommunityEvent = GetMissionSchemaCommunityEvent(missionId))
		{
			TSharedPtr<FGsCommunityData> communityData = _communityDataMap.FindRef(eventId);
			if (communityData.IsValid())
			{
				communityData->SetConditionTime(expiredTime, complatedTime);
			}
		}*/
	}

	// 미션 목록 체크
	CheckNewEventMission();
	// UI 갱신
	GMessage()->GetCommunity().SendMessage(MessageContentCommunity::ACTIVE_WINDOW_INVALIDTEALL, nullptr);

	// HUD 관련 레드닷 갱신 요청	
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COMMUNITY, IsRedDot());
}

// 커뮤니티 소녀 이벤트 미션 보상 응답
void FGsCommunityEventManager::Set(PD::SC::PKT_SC_ACK_COMMUNITY_EVENT_MISSION_REWARD_READ& Packet)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[CommunityEventMission] SC_ACK_COMMUNITY_EVENT_MISSION_REWARD"));
#endif

	int32_t result = Packet.Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		CommunityErrorMsg(static_cast<PD::Result>(result));
	}
	else
	{
		CommunityEventMissionId missionId = Packet.MissionId();
		CommunityEventMissionTabId tabId = Packet.TabId();
		CommunityEventMissionCommonId commonId = Packet.CommonId();
		bool isBonus = Packet.IsBonus();

#if WITH_EDITOR
		GSLOG(Log, TEXT("MissionId : %llu, TabId : %llu, commonId : %llu, IsBonus : %d"), missionId, tabId, commonId, (int)isBonus);
#endif
		TSharedPtr<FGsCommunityEventMissionData> missionData = _missionDataMap.FindRef(missionId);
		if (!missionData.IsValid())
		{
			// error
			return;
		}

		if (isBonus)
		{
			// MissionCommon 보상 요청의 응답이다.
			missionData->SetIsRewardedBonus(true);
		}
		else
		{
			// Mission 보너스 보상 요청의 응답이다.
			TSharedPtr<FGsCommunityEventMissionTab> missionTabData = missionData->GetCommunityEventMissionTabData(tabId);
			if (!missionTabData.IsValid())
			{
				// error
				return;
			}

			TSharedPtr<FGsCommunityEventMissionCommon> missionCommonData = missionTabData->GetCommunityEventMissionCommonData(commonId);
			if (!missionCommonData.IsValid())
			{
				// error
				return;
			}

			missionCommonData->SetIsRewarded(true);
		}

		if (true == missionData->IsAllMissionAndBonusRewarded())
		{
			// 모든 보상 지급이 끝났다.
			if (const FGsSchemaCommunityEvent* schemaCommunityEvent = GetMissionSchemaCommunityEvent(missionId))
			{
				TSharedPtr<FGsCommunityData> communityData = _communityDataMap.FindRef(schemaCommunityEvent->id);
				if (communityData.IsValid())
				{
					communityData->SetConditionComplatedTime();
				}
			}
		}

		// 보상을 인벤토리로 지급하였습니다.
		FText findText;
		FText::FindText(TEXT("AttendanceRewardText"), TEXT("UI_Notice_GetReward"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySound2D(TEXT("UI_RewardOne"));
		}

		// UI 갱신
		GMessage()->GetCommunity().SendMessage(MessageContentCommunity::ACTIVE_WINDOW_INVALIDTEALL, nullptr);
	}
}

// 커뮤니티 소녀 이벤트 미션 진행도 알림
void FGsCommunityEventManager::Set(PD::SC::PKT_SC_INFORM_COMMUNITY_EVENT_MISSION_PROGRESS_READ& Packet)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[CommunityEventMission] SC_INFORM_COMMUNITY_EVENT_MISSION_PROGRESS"));
#endif

	CommunityEventMissionId missionId = Packet.MissionId();
	CommunityEventMissionTabId tabId = Packet.TabId();
	CommunityEventMissionCommonId commonId = Packet.CommonId();
	int32 missionCount = Packet.MissionCount();

#if WITH_EDITOR
	GSLOG(Log, TEXT("MissionId : %llu, TabId : %llu, commonId : %llu, missionCount : %d"), missionId, tabId, commonId, missionCount);
#endif

	TSharedPtr<FGsCommunityEventMissionData> missionData = _missionDataMap.FindRef(missionId);
	if (!missionData.IsValid())
	{
		// error
		return;
	}

	// Mission 보너스 보상 요청의 응답이다.
	TSharedPtr<FGsCommunityEventMissionTab> missionTabData = missionData->GetCommunityEventMissionTabData(tabId);
	if (!missionTabData.IsValid())
	{
		// error
		return;
	}

	TSharedPtr<FGsCommunityEventMissionCommon> missionCommonData = missionTabData->GetCommunityEventMissionCommonData(commonId);
	if (!missionCommonData.IsValid())
	{
		// error
		return;
	}

	missionCommonData->SetAchieveMissionPoint(missionCount);

	// UI 갱신
	GMessage()->GetCommunity().SendMessage(MessageContentCommunity::ACTIVE_WINDOW_INVALIDTEALL, nullptr);
	// HUD 관련 레드닷 갱신 요청	
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COMMUNITY, IsRedDot());
}

// 출석부 구입 요청
void FGsCommunityEventManager::SendReqPurchaseAttendance(AttendanceId inId)
{
	TSharedPtr<FGsAttendanceData> attendanceData = _attendanceMap.FindRef(inId);
	if (nullptr == attendanceData || !attendanceData.IsValid())
		return;

	const FGsSchemaCommunityEventAttendance* schemaAttendance = attendanceData->GetSchemaAttendance();
	if (nullptr == schemaAttendance)
	{
		FText findText;		
		FText::FindText(TEXT("AttendanceRewardText"), TEXT("Alert_Invalid_Event_Id"), findText);
		if (!findText.IsEmpty())
		{
			FGsUIHelper::TrayMessageTicker(findText);
		}
		return;
	}		

	const FGsSchemaAttendanceEvent* schemaAttendanceEvent =
		schemaAttendance->typeInfo.GetRowByType<FGsSchemaAttendanceEvent>();
	if (nullptr == schemaAttendanceEvent)
		return;

	FText outNameText;
	GetAttendanceNameText(schemaAttendance->id, outNameText);

	// TEXT: {0} 출석 보상을 해제하시겠습니까?
	FText text;
	FText::FindText(TEXT("AttendanceRewardText"), TEXT("UI_PopUp_CommonPay_Main"), text);
	text = FText::Format(text, outNameText);

	FText text1;
	FText::FindText(TEXT("AttendanceRewardText"), TEXT("UI_PopUp_CommonPay_Sub1"), text1);

	if (auto popup = Cast<UGsUIPopupCommonPay>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPay")).Get()))
	{
		CostType costType = schemaAttendanceEvent->costType;
		if (costType == CostType::CURRENCY)
		{
			if (const FGsSchemaAttendanceCurrency* schemaAttendanceCurrency =
				schemaAttendanceEvent->typeInfo.GetRowByType<FGsSchemaAttendanceCurrency>())
			{
				TSharedRef<FGsCostPackage> CostPackage(FGsCostPackage::CreateSimple(schemaAttendanceCurrency->currencyType, schemaAttendanceCurrency->currencyAmount));
				popup->SetData(text, MoveTemp(CostPackage),
					[inId](bool bInIsOk)
					{
						if (bInIsOk)
						{
							// 서버에 구매패킷 전송
							FGsNetSendServiceWorld::SendReqPurchaseAttendance(inId);
						}
					});
				popup->SetUnderText(text1);
			}
		}
		else if (costType == CostType::ITEM)
		{
			if (const FGsSchemaAttendanceItem* schemaAttendanceItem =
				schemaAttendanceEvent->typeInfo.GetRowByType<FGsSchemaAttendanceItem>())
			{
				if (const FGsSchemaItemCommon* itemCommon = schemaAttendanceItem->itemId.GetRow())
				{				
					popup->SetData(text,
						FGsCostPackage::CreateSimple(itemCommon->id, schemaAttendanceItem->itemCount),
						[inId](bool InIsOk) 
						{
							if (!InIsOk)
								return;

							// 서버에 구매패킷 전송
							FGsNetSendServiceWorld::SendReqPurchaseAttendance(inId);
						});
					popup->SetUnderText(text1);
				}
			}
		}
	}
}

// 출첵 요청
void FGsCommunityEventManager::SendReqCheckAttendance(AttendanceId inId, AttendanceCheckIdx inIndex)
{
	FGsNetSendServiceWorld::SendReqAttendanceCheck(inId, inIndex);
}

// 커뮤니티소녀 미션 보상 요청
void FGsCommunityEventManager::SendReqMissionReward(CommunityEventMissionId InMissionId, CommunityEventMissionTabId InTabId,
	CommunityEventMissionCommonId InCommonId, bool InIsBonus)
{
	FGsNetSendServiceWorld::SendReqCommunityEventMissionReward(InMissionId, InTabId, InCommonId, InIsBonus);
}

const FGsSchemaCommunityEvent* FGsCommunityEventManager::GetAttendanceSchemaCommunityEvent(IN AttendanceId inAttendanceId)
{
	for (auto& iter : _communityDataMap)
	{
		TSharedPtr<FGsCommunityData> communityData = iter.Value;
		if (!communityData.IsValid())
			continue;

		if (communityData->GetType() == CommunityEventType::ATTENDANCE)
		{
			if (const FGsSchemaCommunityEvent* schemaCommunityEvent = communityData->GetSchemaCommunityEvent())
			{
				const FGsSchemaCommunityEventAttendance* schemaAttendance
					= schemaCommunityEvent->typeInfoEventId.GetRowByType<FGsSchemaCommunityEventAttendance>();

				if (nullptr != schemaAttendance)
				{
					if (schemaAttendance->id == inAttendanceId)
					{
						return schemaCommunityEvent;
					}
				}
			}
		}
	}

	return nullptr;
}

const FGsSchemaCommunityEvent* FGsCommunityEventManager::GetMissionSchemaCommunityEvent(IN CommunityEventMissionId inMissionId)
{
	for (auto& iter : _communityDataMap)
	{
		TSharedPtr<FGsCommunityData> communityData = iter.Value;
		if (!communityData.IsValid())
			continue;

		if (communityData->GetType() == CommunityEventType::MISSION)
		{
			if (const FGsSchemaCommunityEvent* schemaCommunityEvent = communityData->GetSchemaCommunityEvent())
			{
				const FGsSchemaCommunityEventMission* schemaEventMission
					= schemaCommunityEvent->typeInfoEventId.GetRowByType<FGsSchemaCommunityEventMission>();

				if (nullptr != schemaEventMission)
				{
					if (schemaEventMission->id == (int32)inMissionId)
					{
						return schemaCommunityEvent;
					}
				}
			}
		}
	}

	return nullptr;
}

void FGsCommunityEventManager::GetAttendanceNameText(IN AttendanceId inAttendanceId, OUT FText& outNameText)
{
	for (auto& iter : _communityDataMap)
	{
		TSharedPtr<FGsCommunityData> communityData = iter.Value;
		if (!communityData.IsValid())
			continue;

		if (communityData->GetType() == CommunityEventType::ATTENDANCE)
		{
			if (const FGsSchemaCommunityEvent* schemaCommunityEvent = communityData->GetSchemaCommunityEvent())
			{
				const FGsSchemaCommunityEventAttendance* schemaAttendance
					= schemaCommunityEvent->typeInfoEventId.GetRowByType<FGsSchemaCommunityEventAttendance>();

				if (nullptr != schemaAttendance)
				{
					if (schemaAttendance->id == inAttendanceId)
					{
						outNameText = schemaCommunityEvent->tabNameText;
					}
				}
			}
		}
	}
}

const TArray<const FGsSchemaCommunityEvent*> FGsCommunityEventManager::GetSchemaCommunityEventList() const
{
	TArray<const FGsSchemaCommunityEvent*> communityEventMissionList;
	if (const UGsTable* table = FGsSchemaCommunityEvent::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaCommunityEvent>(communityEventMissionList);
	}

	return communityEventMissionList;
}

void FGsCommunityEventManager::CheckNewEventAttendance()
{
	for (auto& iter : _communityDataMap)
	{
		TSharedPtr<FGsCommunityData> communityData = iter.Value;
		if (!communityData.IsValid())
			continue;

		if (communityData->GetType() != CommunityEventType::ATTENDANCE)
			continue;

		const FGsSchemaCommunityEvent* communityEvent = communityData->GetSchemaCommunityEvent();
		if (nullptr == communityEvent)
			continue;

		const FGsSchemaCommunityEventAttendance* schemaAttendance
			= communityEvent->typeInfoEventId.GetRowByType<FGsSchemaCommunityEventAttendance>();

		if (nullptr == schemaAttendance)
			continue;

		if (communityEvent->eventTimeTextType == ECommunityEventTimeType::CONDITION)
		{
			if (false == communityData->IsValid())
			{
				if (_attendanceMap.Contains(schemaAttendance->id))
				{
					_attendanceMap.Remove(schemaAttendance->id);
				}

				SetRedDot(communityEvent->id);				
			}
		}
	}
}

void FGsCommunityEventManager::CheckNewEventMission()
{
	for (auto& iter : _communityDataMap)
	{
		TSharedPtr<FGsCommunityData> communityData = iter.Value;
		if (!communityData.IsValid())
			continue;

		if (communityData->GetType() != CommunityEventType::MISSION)
			continue;
		
		const FGsSchemaCommunityEvent* communityEvent = communityData->GetSchemaCommunityEvent();
		if (nullptr == communityEvent)
			continue;

		const FGsSchemaCommunityEventMission* schemaMission
			= communityEvent->typeInfoEventId.GetRowByType<FGsSchemaCommunityEventMission>();

		if (nullptr == schemaMission)
			continue;

		if (communityData->IsValid())
		{		
			if (!_missionDataMap.Contains(schemaMission->id))
			{
				TSharedPtr<FGsCommunityEventMissionData> missionData = _missionDataMap.Emplace(
					schemaMission->id, MakeShared<FGsCommunityEventMissionData>(schemaMission->id, false));
				for (auto& iterTab : schemaMission->eventMissionTablist)
				{
					const FGsSchemaCommunityEventMissionTab* itemTab = iterTab.GetRow();
					if (nullptr == itemTab)
						continue;

					TSharedPtr<FGsCommunityEventMissionTab> missionTabData =
						MakeShared<FGsCommunityEventMissionTab>(itemTab->id, GetCommunityEventMissionTabIndex(schemaMission->id, itemTab->id));

					for (auto& iterCommon : itemTab->eventMissionList)
					{
						const FGsSchemaCommunityEventMissionCommon* itemCommon = iterCommon.GetRow();
						if (nullptr == itemCommon)
							continue;

						int8 commonIndex = GetCommunityEventMissionCommonIndex(itemTab->id, itemCommon->id);

						TSharedPtr<FGsCommunityEventMissionCommon> missionCommonData = MakeShared<FGsCommunityEventMissionCommon>(itemCommon->id, 0, false, commonIndex);
						missionTabData->AddMissionCommonData(itemCommon->id, missionCommonData);
						missionCommonData->ApplyData();
					}

					missionData->AddMissionTabData(itemTab->id, missionTabData);
					missionTabData->ApplyData();
				}

				missionData->ApplyData();
			}
		}
		else
		{
			if (_missionDataMap.Contains(schemaMission->id))
			{
				_missionDataMap.Remove(schemaMission->id);
			}

			if (false == communityData->IsValid())
			{
				SetRedDot(communityEvent->id);
			}
		}
	}
}

void FGsCommunityEventManager::GetCommunityEventDataList(OUT TArray<CommunityEventId>& outCommunityEventIdList)
{	
	CheckNewEventAttendance();
	CheckNewEventMission();

	for (auto& iter : _communityDataMap)
	{
		TSharedPtr<FGsCommunityData> communityData = iter.Value;
		if (!communityData.IsValid())
			continue;

		if (communityData->GetType() == CommunityEventType::BANNER)
		{
			if (communityData->IsValid())
			{
				outCommunityEventIdList.Emplace(communityData->GetId());
			}
		}
		else
		{
			const FGsSchemaCommunityEvent* schemaCommunityEvent = communityData->GetSchemaCommunityEvent();
			if (nullptr == schemaCommunityEvent)
				continue;

			if (schemaCommunityEvent->communityEventConditionType == CommunityEventConditionType::RETURNING_USER ||
				schemaCommunityEvent->communityEventConditionType == CommunityEventConditionType::NEW_USER)
			{
				if (false == communityData->GetConditionType())
				{
					continue;
				}
			}

			if (communityData->IsValid())
			{
				if (communityData->GetType() == CommunityEventType::ATTENDANCE)
				{
					const FGsSchemaCommunityEventAttendance* schemaAttendance
						= schemaCommunityEvent->typeInfoEventId.GetRowByType<FGsSchemaCommunityEventAttendance>();

					if (nullptr != schemaAttendance)
					{
						TSharedPtr<FGsAttendanceData> attendanceData = GetAttendanceData(schemaAttendance->id);
						if (attendanceData.IsValid())
						{
							outCommunityEventIdList.Emplace(communityData->GetId());
						}
					}
				}
				else if (communityData->GetType() == CommunityEventType::MISSION)
				{
					const FGsSchemaCommunityEventMission* schemaMission
						= schemaCommunityEvent->typeInfoEventId.GetRowByType<FGsSchemaCommunityEventMission>();

					if (nullptr != schemaMission)
					{
						TSharedPtr<FGsCommunityEventMissionData> missionData = GetMissionData(schemaMission->id);
						if (missionData.IsValid())
						{
							outCommunityEventIdList.Emplace(communityData->GetId());
						}
					}
				}
			}
		}
	}
}

bool FGsCommunityEventManager::IsRedDot()
{
	if (true == GGameData()->IsNewDataAttendance())
		return true;

	if (_newEventStartAlert)
		return true;

	return IsCurrentRedDot();
}

bool FGsCommunityEventManager::IsCurrentRedDot()
{
	bool isRedDot = false;

	if (0 < _attendanceMap.Num())
	{
		for (auto& iter : _attendanceMap)
		{
			TSharedPtr<FGsAttendanceData> attendanceData = iter.Value;
			if (!attendanceData.IsValid())
				continue;

			// 구입을 했고, 완료가 되지 않았으며, 오늘 출석하지 않았다면 레드닷 출력
			if (attendanceData->IsUsable() &&
				!attendanceData->IsCompleted() &&
				!attendanceData->IsCheckInToday())
			{
				// [B1] | ejrrb10 | C2URWQ-4962 | 지혜 선임님 요청으로 예외처리 추가 
				// 현재 이벤트 기간과 출석부 기간이 별도로 존재하는 기이한 상황이라, 추가로 예외처리
				// 이벤트 기간은 충족하나, 출석부 기간은 만족하지 못하면, 레드닷 키지 않는다.
				// 만약 글로벌 서비스를 하면, 타임 체킹을 국가별로 해야 하는가? 서버시간과 데이터값 사이의 오차는?
				// 여러모로 문제가 발생할 여지가 많은 예외처리가 될 수 있다. 지금은 서버 기준 시간으로 처리
				FDateTime startDateTime = attendanceData->GetStartTime();
				FDateTime endDateTime = attendanceData->GetEndTime();
				FDateTime currentDateTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();

				if (currentDateTime >= startDateTime && currentDateTime <= endDateTime)
				{
					isRedDot = true;
					break;
				}
			}
		}
	}
	else
	{
		isRedDot = GGameData()->IsNewDataAttendance();
	}	

	for (auto& info : _bannerInfo)
	{
		UGsGameUserSettings::FBannerInfo &bannerInfo = info;

		FDateTime currentDateTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();

		if (currentDateTime >= bannerInfo.tmStart && currentDateTime <= bannerInfo.tmEnd)
		{
		}
		else
		{
			bannerInfo.reddot = false;
		}
	}

	if (!isRedDot) isRedDot = _bannerInfo.FindByPredicate([](auto& e) {return e.valid && e.reddot; }) != nullptr;

	if (false == isRedDot)
	{
		for (auto iter : _missionDataMap)
		{
			TSharedPtr<FGsCommunityEventMissionData> missionData = iter.Value;
			if (!missionData.IsValid())
			{
				continue;
			}

			if (missionData->IsBonusRedDot() || missionData->IsRedDot())
			{
				return true;
			}			
		}
	}

	return isRedDot;
}

void FGsCommunityEventManager::UpdateRedDot(const IGsMessageParam*)
{
	GGameData()->SetNewDatAttendance(false);
	// HUD 관련 레드닷 갱신 요청	
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COMMUNITY, IsRedDot());
}

//banner user data 이므로 data가 정식으로 세팅되고 나서 저장한다
void FGsCommunityEventManager::OnInitializeUserData(const IGsMessageParam* param)
{
	LoadBannerRedDot();

	TArray<TSharedPtr<FGsCommunityData>> dataList;
	_communityDataMap.GenerateValueArray(dataList);

	TArray<TSharedPtr<FGsCommunityData>> bannerList =
	dataList.FilterByPredicate([](auto e) { return e->GetType() == CommunityEventType::BANNER || e->GetType() == CommunityEventType::MISSION; });
	for (auto bannerData : bannerList)
	{
		auto id = bannerData->GetId();
		if (auto data = _bannerInfo.FindByPredicate([id](auto& e) {return e.id == id; }))
		{
			bannerData->SetRedDot(data->reddot);
		}
		else
		{
			bannerData->SetRedDot(true); //새로추가 valid한것과 상관없음
			_bannerInfo.Add(UGsGameUserSettings::FBannerInfo{ id, bannerData->GetStartTime(), bannerData->GetEndTime(), bannerData->IsValid(), true });
		}
	}

	// HUD 관련 레드닷 갱신 요청	
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COMMUNITY, IsRedDot());
}

void FGsCommunityEventManager::ResetTimeContents(ResetTimeTicket InType)
{
	if (InType != ResetTimeTicket::RTT_ATTENDANCE_DAILY)
		return;

	// 출석부 데이터 삭제
	_attendanceMap.Empty();

	// 서버로 목록 요청
	SendReqCommunityList();
}

// 서버에서 내려준 에러타입 별 티커
void FGsCommunityEventManager::CommunityErrorMsg(int inResult)
{
	FText errorText;

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Community] Failed, Result = %d"), (int)inResult);
#endif

	// NetText 테이블에서 사용할 티커값 조회
	FString key = FString::Format(TEXT("{0}"), { PD::ResultEnumToDisplayKey(static_cast<PD::Result>(inResult)) });
	FText::FindText(TEXT("NetText"), key, errorText);

	// NetText 를 사용했는데도 에러 메시지를 찾을 수 없으면...
	if (errorText.IsEmpty())
	{
		// 알 수 없는 에러가 발생했습니다.
		FText::FindText(TEXT("MailText"), TEXT("MailError_Unknown"), errorText);

		// 에디터에서는 에러 값 좀 더 자세히 표시
#if WITH_EDITOR	
		FString msgMain =
			FString::Format(TEXT("{0})\nErrorCode({1})\n{2}"), { *errorText.ToString(), inResult, PD::ResultEnumToString(static_cast<PD::Result>(inResult)) });
		errorText = Forward<FText>(FText::FromString(msgMain));
		GSLOG(Log, TEXT("[Community] Failed, Result = %d"), (int)inResult);
#endif
	}

	FGsUIHelper::TrayMessageTicker(errorText);
}

int8 FGsCommunityEventManager::GetCommunityEventMissionTabIndex(CommunityEventMissionId  inMissionId, CommunityEventMissionTabId inTabId)
{
	const FGsSchemaCommunityEventMission* schemaMission = UGsTableUtil::FindRowById<UGsTableCommunityEventMission, FGsSchemaCommunityEventMission>(inMissionId);
	if (nullptr == schemaMission)
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("[CommunityEventMission] FGsSchemaCommunityEventMission nullptr = Id : %d"), inMissionId);
#endif
		return 0;
	}

	const TArray<FGsSchemaCommunityEventMissionTabRow>& missionTabData = schemaMission->eventMissionTablist;
	const int32 RowToIndex = missionTabData.IndexOfByPredicate([&](const FGsSchemaCommunityEventMissionTabRow& tabRow) -> bool
		{
			return tabRow.GetRow()->id == inTabId;
		});

	return (RowToIndex == -1)? 0 : RowToIndex;
}

int8 FGsCommunityEventManager::GetCommunityEventMissionCommonIndex(CommunityEventMissionTabId inTabId, CommunityEventMissionCommonId  inCommonId)
{
	const FGsSchemaCommunityEventMissionTab* schemaMissionTab = UGsTableUtil::FindRowById<UGsTableCommunityEventMissionTab, FGsSchemaCommunityEventMissionTab>(inTabId);
	if (nullptr == schemaMissionTab)
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("[CommunityEventMission] FGsSchemaCommunityEventMissionTab nullptr = Id : %d"), inTabId);
#endif
		return 0;
	}

	
	const TArray<FGsSchemaCommunityEventMissionCommonRow>& missionCommonData = schemaMissionTab->eventMissionList;
	const int32 RowToIndex = missionCommonData.IndexOfByPredicate([&](const FGsSchemaCommunityEventMissionCommonRow& tabRow) -> bool
		{
			return tabRow.GetRow()->id == inCommonId;
		});

	return (RowToIndex == -1) ? 0 : RowToIndex;
}

void FGsCommunityEventManager::SetCommunityEventData()
{
	TArray<const FGsSchemaCommunityEvent*> tempTabList;
	if (const UGsTable* table = FGsSchemaCommunityEvent::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaCommunityEvent>(tempTabList);
	}

	for (const FGsSchemaCommunityEvent* iter : tempTabList)
	{
		auto data = _communityDataMap.Emplace(iter->id, MakeShared<FGsCommunityData>(iter->id, iter));
		// 데이터 적용

		/*bool isRedDot = true;
		if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
		{
			gameUserSettings->IsCommunityBannerRedDot(iter->id);
		}*/
		data->ApplyData();
	}

	_communityDataMap.ValueSort([](const TSharedPtr<FGsCommunityData>& A, const TSharedPtr<FGsCommunityData>& B)
		{
			return (A->GetIndex() < B->GetIndex()) ? true : false;
		});
}

bool FGsCommunityEventManager::IsRedDot(CommunityEventId inId)
{
	TSharedPtr<FGsCommunityData> communityData = _communityDataMap.FindRef(inId);
	if (!communityData.IsValid())
		return false;

	if (communityData->GetType() == CommunityEventType::ATTENDANCE)
	{
		if (const FGsSchemaCommunityEvent* schemaCommunityEvent = communityData->GetSchemaCommunityEvent())
		{
			const FGsSchemaCommunityEventAttendance* schemaAttendance
				= schemaCommunityEvent->typeInfoEventId.GetRowByType<FGsSchemaCommunityEventAttendance>();
			
			if (nullptr != schemaAttendance)
			{
				TSharedPtr<FGsAttendanceData> attendanceData = GSCommunityEvent()->GetAttendanceData(schemaAttendance->id);
				if (attendanceData.IsValid())
				{
					return attendanceData->IsSetRedDot();
				}
			}
		}
	}
	else if (communityData->GetType() == CommunityEventType::MISSION)
	{	
		if (true == communityData->IsRedDot())
		{
			return true;
		}

		if (const FGsSchemaCommunityEvent* schemaCommunityEvent = communityData->GetSchemaCommunityEvent())
		{
			const FGsSchemaCommunityEventMission* schemaMission
				= schemaCommunityEvent->typeInfoEventId.GetRowByType<FGsSchemaCommunityEventMission>();

			if (nullptr != schemaMission)
			{
				if (_missionDataMap.Contains(schemaMission->id))
				{
					TSharedPtr<FGsCommunityEventMissionData> missionData = _missionDataMap.FindRef(schemaMission->id);
					if (missionData.IsValid())
					{
						bool isMissionRedDot = missionData->IsRedDot();
						bool isHasRewardBonus = missionData->IsHasRewardBonus();
						bool isBonusRedDot = missionData->IsBonusRedDot();
						isBonusRedDot = (isHasRewardBonus && isBonusRedDot) ? true : false;
						isMissionRedDot = (isMissionRedDot || isBonusRedDot) ? true : false;

						return isMissionRedDot;
					}
				}
			}
		}				
	}
	else 
	{
		return communityData->IsRedDot();
	}

	return false;
}

void FGsCommunityEventManager::SetRedDot(CommunityEventId inId)
{
	TSharedPtr<FGsCommunityData> communityData = _communityDataMap.FindRef(inId);
	if (!communityData.IsValid())
		return;

	if (communityData->GetType() == CommunityEventType::BANNER || 
		communityData->GetType() == CommunityEventType::MISSION)
	{
		communityData->SetRedDot(false);
		if (auto data = _bannerInfo.FindByPredicate([inId](auto& e) {return e.id == inId; }))
		{
			data->reddot = false;
		}
	}
}

void FGsCommunityEventManager::SaveRedDot()
{
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{	
		gameUserSettings->SetAllCommunityInfo(_bannerInfo);
	}
}

void FGsCommunityEventManager::LoadBannerRedDot()
{
	_bannerInfo.Reset();

	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		TArray<UGsGameUserSettings::FBannerInfo> bannerInfo;
		gameUserSettings->GetAllCommunityInfo(bannerInfo);

		for (auto& iter : bannerInfo)
		{
			if(!_communityDataMap.Contains(iter.id))
				continue;

			TSharedPtr<FGsCommunityData> communityData = _communityDataMap.FindRef(iter.id);
			if (!communityData.IsValid())
				continue;
			
			if (communityData->GetType() == CommunityEventType::BANNER ||
				communityData->GetType() == CommunityEventType::MISSION)
				_bannerInfo.Emplace(iter);
		}
	}
}

bool FGsCommunityEventManager::IsCommunityEventMissionTabLock(CommunityEventId inEventId, 
	CommunityEventMissionId inEventMissionId, CommunityEventMissionTabId inEventMissionTabId)
{
	TSharedPtr<FGsCommunityData> communityData = _communityDataMap.FindRef(inEventId);
	if (!communityData.IsValid())
		return true;

	const FDateTime startTime = communityData->GetStartTime();

	TSharedPtr<FGsCommunityEventMissionData> missionData = GSCommunityEvent()->GetMissionData(inEventMissionId);
	if (!missionData.IsValid())
		return true;

	TSharedPtr<FGsCommunityEventMissionTab> missionTabData = missionData->GetCommunityEventMissionTabData(inEventMissionTabId);
	if (!missionTabData.IsValid())
		return true;

	if (false == missionTabData->GetIsTimeLock())
		return false;

	int32 tabOpenTime = missionTabData->GetTabOpenTime();

	if (0 == tabOpenTime)
	{
		// 오픈
		missionTabData->SetIsTimeLock(false);
		return false;
	}

	FDateTime addTime((tabOpenTime * 60 * 60) * ETimespan::TicksPerSecond);
	FDateTime currentDateTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	int64 currentTick = currentDateTime.GetTicks();
	int64 startTick = startTime.GetTicks();
	int64 addTick = addTime.GetTicks();
	
	if (startTick + addTick < currentTick)
	{
		// 오픈
		missionTabData->SetIsTimeLock(false);
		return false;
	}	

	return true;
}