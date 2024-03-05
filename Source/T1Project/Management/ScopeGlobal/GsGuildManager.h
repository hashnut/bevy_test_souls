// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsManager.h"
#include "Classes/GsManager.h"
#include "Classes/Collection/GsCollectionDataContainer.h"
#include "RedDot/GsRedDotCheck.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageGameObject.h"
#include "Classes/GsPool.h"
#include "Classes/Collection/GsCollectionData.h"
#include "Guild/GsGuildData.h"
#include "Guild/GsGuildMemberData.h"
#include "Guild/GsGuildBuildingCollectionData.h"
#include "Guild/GsGuildItemCollectionData.h"
#include "Guild/GsGuildQuestDataHandler.h"
#include "Guild/GsGuildSendCoolTimeController.h"
#include "Guild/GsGuildStorageData.h"
#include "Guild/GsGuildCampData.h"
#include "Guild/GsGuildSanctumData.h"
#include "Guild/GsGuildRedDotCheckData.h"
#include "Guild/GsGuildAuctionData.h"
#include "Guild/GsAgitDataController.h"
#include "Guild/GsGuildGroupChatData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "RedDot/GsRedDotCheckFunc.h"


struct FGsGuildBuildingData;
struct FGsGuildSetting;
struct FGsSchemaGuildSnsShortcutUrl;

/**
 * 길드 관련 데이타를 관리하는 클래스
 */
class T1PROJECT_API FGsGuildManager final : 
	public IGsManager, 
	public IGsCollectionDataContainer, 
	public IGsRedDotCheck
{
public:
	enum class EGsGuildRedDotType : int32
	{
		Attendance = 0,
		Notice,
		Buildings,
		Buffs,
		Approval,
		DiplomacyEnemy,
		DiplomacyAlly,
		DiplomacyState,
		Quest,
		ItemCollection,
		GuildStorage, // 주의: Storage로 명하면 Hive쪽 Enum과 충돌
		AcquisitionSetting, // 획득 방식 설정
		Division, // 분배
		Auction, // 경매
		GuildCalculate, // 정산
		Sanctum,
	};

private:
	// 내 길드 정보
	FGsGuildDataMine _myGuildData; // 내 길드 정보
	FGsGuildBuildingCollectionData _myBuildingData; // 내 길드 건물 정보 
	FGsGuildItemCollectionData _myItemCollection;	// 2021/09/28 PKT - 내 길드 아이템 컬렉션 정보
	TUniquePtr<FGsGuildQuestDataHandler> _myGuildQuestHandler; // 내 길드 퀘스트 정보
	FGsGuildSendCoolTimeController _sendController; // 패킷 전송 쿨타임 관리
	FGsGuildStorageData _myStorageData; // 내 길드 창고 정보
	FGsGuildAuctionData _myAuctionData; // 기사단 경매/분배 정보
	FGsGuildSanctumData _mySanctumData; // 내 길드 성소 정보
	FGsGuildRedDotCheckData _redDotCheckData; // 레드닷 체크를 위한 시간 데이터	
	FGsAgitDataController _agitDataController;

	// 데이터 풀
	TGsPoolDefault<FGsGuildData> _poolGuildData;
	TGsPoolDefault<FGsGuildMemberData> _poolGuildMemberData;
	TGsPoolDefault<FGsGuildCampData> _poolCampData;

	// 재가입 쿨타임
	time_t _rejoinCoolTimeEndTime = 0;
	
	TArray<FGsGuildChatList> _loggedChatList;
	FGsGuildGroupChatData _groupChatRoomData;

	TMap<EGsGuildRedDotType, IGsRedDotCheck*> _redDotCheckMap;

private:
	// 필요 시 요청해서 채우는 것들(기본적으로 비어있음)
	TArray<FGsGuildData*> _joinGuildList; // 기사단 가입/검색 길드 리스트
	TArray<FGsGuildData*> _joinWaitingGuildList; // 내가 요청 중인 길드 리스트

	GuildDBId _tempOtherGuildDBId = INVALID_GUILD_DB_ID; // 타길드 상세보기 패킷 성공여부에 따른 처리위해 임시로 저장하는 ID
	FGsGuildDataDetail* _otherGuildDetailData = nullptr; // 타길드 상세보기
	FGsGuildBuildingCollectionData* _otherGuildBuildingData = nullptr; // 타길드 상세보기 건물정보	


	TArray<FGsGuildJoinWaitingUser> _joinWaitingUserList; // 가입 승인 대기자 리스트
	int32 _joinWaitingUserCount = 0; // 가입 승인 대기자 수. 월드 입장 시 서버에서 몇명인지 준다. 레드닷 체크를 위함.
	TArray<GuildMemberRightsbyGrade> _authorityList; // 기사단 권한. index는 GuildMemberGradeType. (예: 0은 기사단장, 1은 부단장...)	
	TArray<FGsGuildMemberData*> _memberList; // 기사단 멤버 리스트
	TArray<FGsGuildCampData*> _campDataList; // 캠프 정보 리스트

	TArray<TPair<MessageContentGuild, FDelegateHandle>> _listCheckRedDotHandler;
	MsgGameObjHandleArray _listGameObjectHandler;

public:
	FGsGuildManager()			= default;
	virtual ~FGsGuildManager()	= default;

public:
	// IGsManager override
	virtual void Initialize() override;
	virtual void Finalize() override;

	// 2021/10/29 PKT - 이벤트 등록
	void RegisterMessages();
	// 2021/10/29 PKT - 이벤트 해제
	void UnRegisterMessages();

	// IGsCollectionDataContainer override
	virtual const IGsCollectionData* GetCollectionData() const { return &_myBuildingData; }

	// IGsIsRedDotCheck override
	virtual bool CheckRedDot(bool bInUpdate = true) override;

public:
	void RemoveAll();
	// 로비로 갈 때 삭제할 것들
	void RemoveWhenFlowChanged();	

public:
	// 길드 관련된 모든 정보 삭제(길드 해산/탈퇴 시)
	void RemoveAllGuildData(bool bInUpdatePassivity = true);
	// 길드 리스트 정보 삭제
	void RemoveAllGuildList();
	// 길드 리스트의 길드 삭제
	void RemoveJoinGuild(GuildDBId InGuildDBId);
	// 가입 신청했던 길드 삭제
	void RemoveJoinWaitingGuild(GuildDBId InGuildDBId);
	// 가입 신청중인 길드리스트 삭제
	void RemoveAllJoinWaitingGuild();
	// 길드원 삭제
	bool RemoveGuildMember(UserDBId InUserDBId);
	bool RemoveGuildMember(const FString& InUserName);
	void RemoveAllGuildMember();
	// 캠프 삭제
	bool RemoveCampData(CampId InId);

public:
	// 시간에 따른 컨텐츠 리셋(일일, 주간, 월간...)
	void ResetTimeContents(ResetTimeTicket InType);

private:
	// 일일 컨텐츠 리셋
	void ResetDaily();

	void ResetQuest();
	// 2021/10/29 PKT - Item Collection 리셋
	void ResetItemCollectionList(ResetTimeTicket InType);

public:
	// 입장 시 처음 받는 길드 상세정보
	void SetMyGuildInfo(PD::GC::PKT_GC_MY_GUILD_INFO_READ& InPacket);
	// 접속해있는 상태에서 가입시
	void SetUserGuildInfo(PD::SC::PKT_SC_USER_GUILD_INFO_READ& InPacket);
	// 길드 메인창 입장을 위해 요청해서 받는 정보
	void SetUserGuildInfoDetail(PD::GC::PKT_GC_ACK_OPEN_GUILD_READ& InPacket);
	// 길드 리스트
	void SetGuildList(PD::GC::PKT_GC_ACK_GUILD_LIST_READ& InPacket);
	// 가입 신청한 길드 목록
	void SetRequestedJoinGuildList(PD::GC::PKT_GC_ACK_REQUESTED_JOIN_GUILD_LIST_READ& InPacket);
	// 가입 신청 후 신청대기 결과 받았을 경우 처리
	void SetJoinResultWaiting(GuildDBId InGuildDBId);
	// 길드 가입 대기자 목록
	void SetJoinWaitingUserCount(int32 InCount) { _joinWaitingUserCount = InCount; }
	void SetJoinWaitingUserList(PD::GC::PKT_GC_ACK_WAITING_JOIN_GUILD_USER_LIST_READ& InPacket);
	// 길드원 목록
	void SetGuildMemberList(PD::GC::PKT_GC_ACK_GUILD_MEMBER_LIST_READ& InPacket);
	// 가입 승인
	void SetApproveJoin(UserDBId InUserDBId);
	// 가입 거부
	void SetRejectJoin(UserDBId InUserDBId);
	// 등급별 권한 리스트	
	void SetAuthorityList(PD::GC::PKT_GC_ACK_GUILD_MEMBER_RIGHTS_BY_GRADE_LIST_READ& InPacket);
	// 권한 변경 결과(변경 정보만 온다)
	void SetChangeAuthority(PD::GC::PKT_GC_ACK_CHANGED_GUILD_MEMBER_RIGHTS_BY_GRADE_READ& InPacket);
	// 내 권한이 변경되었을 때 노티 
	void SetChangedMyAuthority(GuildRightsLevel InRights);
	// 공지/소개글 변경 (bIsNotice: T:공지글, F:소개글)
	void SetChangeGuildString(const FString& InString, bool bIsNotice);
	// 가입방식 변경
	void SetChangeJoinType(GuildJoinType InType);
	// 내 길드 문장 변경(내가 변경했거나, 노티받았을 때)
	void SetChangeEmblem(GuildEmblemId InEmblemId);
	// 가입 제한 레벨 변경
	void SetChangeLimitUserLevel(Level InLevel);
	// 길드원 멤버 등급 변경
	void SetChangeMemberGrade(UserDBId InUserDBId, GuildMemberGradeType InGrade);
	void SetChangeMemberGrade(const FString& InUserName, GuildMemberGradeType InGrade);
	// 길드 일일활동
	void SetDailyActivity(PD::SC::PKT_SC_ACK_GUILD_DAILY_ACTIVITY_READ& InPacket);
	// 길드 레벨, 공헌도 변경
	void SetGuildContribution(PD::GC::PKT_GC_GUILD_CONTRIBUITON_UPDATE_READ& InPacket, bool bInUpdate);
	// 길드 운영비가 변경됨
	void SetGuildAsset(Currency InAmount, bool bInUpdate = true);
	// 기사단 SNS 정보 변경
	void SetSNSLink(int32 InIndex, const FString& InURL);

	// 길드 건물 리스트 응답
	void SetGuildBuildingList(PD::GC::PKT_GC_ACK_GUILD_BUILDING_LIST_READ& InPacket);
	// 길드 건물 오픈 응답
	void SetBuildingOpen(const GuildBuildingData& InData);
	// 길드 건물 레벨업 응답
	void SetBuildingUpgrade(GuildBuildingId InId, GuildBuildingLevel InNextLevel);
	// 길드 버프 활성화
	void SetActiveGuildBuff(PD::GC::PKT_GC_ACK_GUILD_BUILDING_PASSIVITY_READ& InPacket);
	// 길드 건물 노티 //SetBuildingOpen지울 수 있음 지우자
	void SetBuildingNotify(PD::SC::PKT_SC_NOTIFY_GUILD_BUILDING_READ& InPacket);
	// 다른 길드 상세정보
	void SetOtherGuildDetailData(PD::GC::PKT_GC_ACK_GUILD_DETAIL_READ* InPacket, bool bIsRemoved);
	// 기사단 퀘스트
	void SetQuestInfo(PD::GC::PKT_GC_ACK_GUILD_QUEST_INFO_READ& InPacket);
	void SetGuildZealPoint(GuildZealPoint InZealPoint);
	
	// 기사단 외교
	void SetEnemyList(PD::GC::PKT_GC_ACK_GUILD_HOSTILE_LIST_READ& InPacket);
	void SetAllyList(PD::GC::PKT_GC_ACK_GUILD_ALLY_LIST_READ& InPacket);
	void SetAllyStateList(PD::GC::PKT_GC_ACK_GUILD_ALLY_REQUEST_LIST_READ& InPacket);	
	
	// 채팅 리스트
	void SetGuildChatList(PD::GC::PKT_GC_GUILD_CHAT_LIST_RECENT_READ& InPacket);
	//void SetAllyGuildChatList(PD::GC::PKT_GC_GUILD_ALLY_CHAT_LIST_RECENT_READ& InPacket);

	// 2021/10/26 PKT - 아이템 컬렉션 관련
	void BuildItemCollectionList(PD::GC::PKT_GC_ACK_GUILD_ITEM_COLLECTION_LIST_READ& InPacket);
	void CompletedItemCollectionConditionAck(PD::SC::PKT_SC_ACK_GUILD_ITEM_COLLECTION_REGISTER_READ& InPacket);
	void CompletedItemCollectionConditionNotify(PD::SC::PKT_SC_INFORM_GUILD_ITEM_COLLECTION_REGISTER_READ& InPacket);
	void BookMarkItemCollection(CollectionId InCollectionId, bool InFlag);

	// 기사단 던전 Best레벨
	void SetDungeonBestLevel(GuildDunGeonLevel InBestLevel);	

	// 기사단 성소
	void SetClearSanctumNexusData();
	void SetSanctumOccupySuccessTime();

private:
	void SetBuildingDataInter(const GuildBuildingData& InData, bool bInCheckType);

public:
	// 기사단 상세정보 요청
	void SendRequestGuildDetail(GuildDBId InGuildDBId);

	// 등급별 권한 요청
	bool SendRequestChangeAuthority(TArray<GuildRightsLevel>& InDataList);

public:
	// 레드닷 체크 등을 위한 길드 정보 저장
	void SaveLastSettings();

private:
	IGsRedDotCheck* GetRedDotCheck(EGsGuildRedDotType InType);
	const IGsRedDotCheck* GetRedDotCheckConst(EGsGuildRedDotType InType) const;

	// 레드닷 체크 함수들. 체크만 하고 캐싱되지 않음
	bool CheckRedDotAttendance() const;
	bool CheckRedDotNoticeChanged() const;
	bool CheckRedDotBuildings() const;	
	bool CheckRedDotBuff() const;	
	bool CheckRedDotDiplomacyEnemy();
	bool CheckRedDotDiplomacyAlly();
	bool CheckRedDotDiplomacyState();
	bool CheckRedDotQuest() const;
	bool CheckRedDotItemCollection() const;
	bool CheckRedDotStorage() const;
	bool CheckRedDotAcquisitionSetting() const;
	bool CheckRedDotDivision() const;
	bool CheckRedDotAuction() const;
	bool CheckRedDotCalculate() const;
	bool CheckRedDotSanctum() const;
	
	bool CheckRedDotBuildingInter(const FGsGuildBuildingData& InData) const;

public:
	// 레드닷 체크 후 캐싱 됨
	void CheckRedDotByType(EGsGuildRedDotType InType);
	void CheckAllRedDot();

	bool IsRedDotBuilding(GuildBuildingId InId) const;
	bool IsRedDotByType(EGsGuildRedDotType InType) const;
	int32 GetRedDotApproval() const;

	void OnCheckRedDot(const struct IGsMessageParam* InParam);

public:
	bool IsGuild() const { return (INVALID_GUILD_DB_ID != _myGuildData._guildDBId) ? true : false; }
	bool IsGuildMaster() const { return (GuildMemberGradeType::CAPTAIN == _myGuildData._memberGrade) ? true : false; }
	// 내가 가입 신청중인 길드인가
	bool IsJoinWaitingGuild(GuildDBId InGuildDBId) const;
	// 활성화 중인 버프가 있는가
	bool IsExistActiveBuff() const;
	bool IsEnemyGuild(GuildDBId InGuildDBId) const;
	bool IsAllyGuild(GuildDBId InGuildDBId) const;
	
public:
	FGsGuildSendCoolTimeController& GetSendController() { return _sendController; }
	const FGsGuildDataMine* GetMyGuildData() const { return &_myGuildData; }
	FGsGuildDataMine* GetMyGuildDataNoConst() { return &_myGuildData; }
	
	TArray<FGsGuildData*> GetJoinGuildList() const { return _joinGuildList; }
	TArray<FGsGuildData*> GetJoinWaitingGuildList() const { return _joinWaitingGuildList; }
	TArray<FGsGuildJoinWaitingUser> GetJoinWaitingUserList() const { return _joinWaitingUserList; }	
	TArray<FGsGuildMemberData*> GetMemberList() const { return _memberList; }
	const FGsGuildItemCollectionData* GetItemCollection() const { return &_myItemCollection; }
	GuildRightsLevel GetAuthorityByMemberGrade(GuildMemberGradeType InGrade) const;
	float GetReJoinRemainTime() const;
	GuildDBId GetGuildDBId() const { return _myGuildData._guildDBId; }
	int32 GetGuildLevel() const { return _myGuildData._guildLevel; }
	uint16 GetCampLevel() const { return _myGuildData._campLevel; }
	GuildMemberGradeType GetMemeberGrade() const { return _myGuildData._memberGrade; }
	uint32 GetAuthority() const { return _myGuildData._authority; }	
	Currency GetGuildAssetAmount() const;
	GuildZealPoint GetGuildZealPoint() const { return _myGuildData.GetZealPoint(); }
	int32 GetJoinWaitingUserCount() const;
	time_t GetBuffReactiveTime() const { return _myBuildingData._buffReactivateTime; }
	FGsGuildStorageData* GetMyGuildStorageData() { return &_myStorageData; }
	FGsGuildAuctionData* GetMyGuildAuctionData() { return &_myAuctionData; }
	const FGsGuildCampData* GetCampData(int32 InIndex) const;
	const FGsGuildSanctumData* GetSanctumData() const { return &_mySanctumData; }
	FGsAgitDataController* GetAgitDataController() { return &_agitDataController; }

	// 다른 길드 정보랑 통일해서 쓰기 위한 인터페이스 
	const FGsGuildDataDetail* GetGuildDetailData(bool bIsMyGuild = true) const;
	const FGsGuildBuildingData* GetBuildingData(GuildBuildingId InId, bool bIsMyGuild = true) const;	
	const TArray<struct FGsGuildBuildingData>& GetBuildingList() const { return _myBuildingData._dataList; }
	FGsGuildQuestDataHandler* GetQuestHandler() { return _myGuildQuestHandler.Get(); }	
	FGsGuildRedDotCheckData& GetRedDotCheckData() { return _redDotCheckData; }

public:
	// 길드 채팅
	void ResetRawChatList() { _loggedChatList.Empty(); }
	const TArray<FGsGuildChatList>& GetChatListRawData() const { return _loggedChatList;  }

	// 길드 채팅 그룹 
	void  InitializeGroupChatRoom(bool init = true);
	const FGsGuildGroupChatData* GetGuildGroupChatInfo() const { return &_groupChatRoomData;  }

	int32 GetMaxGroupRoomCount() { return _groupChatRoomData._maxChatGroupRoomCount; }
	int32 GetMaxGroupGuildCount() { return _groupChatRoomData._maxChatGroupGuildCount; }
	int32 GetMaxGroupInvitationCount() { return _groupChatRoomData._maxChatGroupInvitationCount; }

public:
	// 길드 채팅 그룹 룸
	void AddListGroupChatRooms(TArray<FGsGroupChatRoomInfo>& roomInfos);
	void AddGroupChatRoom(FGsGroupChatRoomInfo& roomInfo, bool refresh = false);
	void AddGroupChatRoom(FGsGroupChatRoomInfo&& roomInfo, bool refresh = false);
	void RemoveGroupChatRoom(GuildGroupChatRoomId roomId, MessageContentChat reason = MessageContentChat::CHATGROUP_LEAVE);

	// 길드 채팅 그룹 룸으로의 초대
	void AddListInvitationToGroupChat(TArray<FGsGuildGroupChatInviteHost>& invitations); //초대장 갱신
	void AddInvitationToGroupChat(FGsGuildGroupChatInviteHost& invitation);
	void RemoveInvitationToGroupChat(GuildGroupChatRoomId roomId, FString guildName = TEXT(""), MessageContentChat reason = MessageContentChat::CHATGROUP_INVITATION_ACCEPTED);

	// 길드 채팅 그룹 룸 멤버 길드
	void AddListGroupMemberGuilds(GuildGroupChatRoomId roomId, TArray<FGsGuildGroupChatMemberGuild>& members);
	void AddGroupMemberGuild(GuildGroupChatRoomId roomId, FGsGuildGroupChatMemberGuild& member);
	void RemoveGroupMemberGuild(GuildGroupChatRoomId roomId, GuildDBId guildId, MessageContentChat reason = MessageContentChat::CHATGROUP_LEAVE);

	// 길드 채팅 그룹 룸에서 초대한 길드
	void AddListInvitationGuests(GuildGroupChatRoomId roomId, TArray<FGsGuildGroupChatMemberGuild>& guests);
	void AddInvitationGuest(GuildGroupChatRoomId roomId, FGsGuildGroupChatMemberGuild& guest);
	void RemoveInvitationGuest(GuildGroupChatRoomId roomId, GuildDBId guildId, MessageContentChat reason = MessageContentChat::CHATGROUP_GUEST_REMOVE);

	// 길드 채팅 그룹 룸 멤버, 초대길드, 초대장 변경 알림  - 다시 리스트 요청
	void OnNotificationGuestAdded(GuildGroupChatRoomId roomId);
	void OnNotificationEnterGroupChat(GuildGroupChatRoomId roomId);
	void OnNotificationInvitedToGroupChat(GuildGroupChatRoomId roomId);

	// 길드 채팅 그룹 룸 설정 변경 알림
	void OnNotificationEntrustedWithRoomMaster(GuildGroupChatRoomId roomId, GuildDBId guildId, FString guildName);
	void OnNotificaationAuthorizedChanged(GuildGroupChatRoomId roomId, GuildMemberGradeType grade);

public:
	// 길드 채팅 그룹 룸 정보 가져오기
	int  GetGroupChatRooms(OUT TArray<TWeakPtr<FGsGuildGroupChatRoom>>& rooms);

	TWeakPtr<FGsGuildGroupChatRoom> GetGroupChatRoom(GuildGroupChatRoomId roomId);
	TWeakPtr<FGsGuildGroupChatInviteHost> GetInvitation(GuildGroupChatRoomId roomId);

public:
	// SNS URL
	int32 GetSnsUrlMaxCount() const;
	int32 GetSnsUrlMaxLength() const;

private:
	UserDBId GetMyUserDBId() const;
	FString GetUserName() const;

public:
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam);

	/** SNS 바로가기 관련 */
public:
	/** SNS URL 전체로 GuildSnsShortcutUrl 테이블 조회 */
	const FGsSchemaGuildSnsShortcutUrl* FindSnsLinkInfo(const FString& InSnsUrl) const;

	/** SNS 링크 목록을 반환. */
	const TArray<FString> MakeSnsLinks(const bool InIsMyGuild) const;

	/** SNS URL 을 입력하여 설정된 액션을 수행. 비속어 처리를 위해 실제 수행은 TryToActionSnsLinkDelayed() 에서 처리함. */
	void TryToActionSnsLink(const FString& InSnsUrl) const;

	/** SNS URL 을 입력하여 설정된 액션을 수행 */
	void TryToActionSnsLinkDelayed(const FString& InSnsUrl) const;
};

#define GGuild() UGsScopeHolder::GetGlobalManagerFType<FGsGuildManager>(UGsScopeGlobal::EManagerType::Guild)
#ifdef GGuild
	#define GGuildItemCollection() ((nullptr != GGuild()) ? GGuild()->GetItemCollection() : nullptr)
#endif // GGuild
