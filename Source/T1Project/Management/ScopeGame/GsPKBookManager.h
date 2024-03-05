
#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"
#include "../RedDot/GsRedDotCheck.h"
#include "../PKBook/GsPKRecordData.h"
#include "../PKBook/GsPKBookData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../Message/GsMessagePKBook.h"
#include "../PKBook/GsPKBookEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

/**
* file		GsPKBookManager.h
* @brief	PK Book Contens : InGame 내 PK 정보등을 기록 및 위협 대상을 등록하여 탐색할 수 있는 Contens
* @author	PKT
* @date		2022/10/07
**/
class FGsPKBookManager : public IGsManager, public IGsRedDotCheck
{
	// 2022/09/20 PKT - PK Book Config
	const struct FGsSchemaPKBookConfig* _configData = nullptr;

	// 2022/09/20 PKT - PK 기록 정보
	TArray<FGsPKRecordData> _PKRecordDataHolder;

	// 2022/09/20 PKT - 경계 대상 정보
	TArray<FGsPKDetectionTargetData> _PKDetectionTargetDataHolder;

	// 2022/09/20 PKT - PKRecord Data View Set
	TArray<const FGsPKRecordData*> _PKRecordDataViewSet;

	// 2022/09/20 PKT - Detection Target Data View Set
	TArray<const FGsPKDetectionTargetData*> _PKDetectionTargetDataViewSet;

	// 2022/09/20 PKT - 체크 아웃 타임(레드닷 기준 시간)
	int64 _checkOutTime = 0;

	// 2022/10/06 PKT - 경계 대상 로그인 유저만 보여줄 지 여부
	bool _logInFilter = false;
	// 2022/09/20 PKT - PK 메세지 활성화 여부
	bool _activePKAutoMessage = false;
	// 2022/09/20 PKT - PK Kill 메세지
	FText _PKTeaseMessage = FText::GetEmpty();
	// 2022/09/20 PKT - PK Die 메세지
	FText _PKRevengeMessage = FText::GetEmpty();

	// 2023/1/17 PKT - 조롱하기 메세지
	bool _useMockeryMessage = true;
	FText _mockeryMessage = FText::GetEmpty();

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

	virtual bool CheckRedDot(bool bInUpdate = true) override;

	/**
	* Server -> Client
	*/
public:
	// 2022/10/04 PKT - 로그인 시 기록 PK 기록을 받는다.
	void MakeRecordDataSet(PD::SC::PKT_SC_PK_BOOK_LIST_READ& InPacket);
	// 2022/10/04 PKT - In Game 에서 PK 기록이 추가
	void AddPKRecordData(PD::SC::PKT_SC_ADD_PK_BOOK_INFO_READ& InPacket);

	// 2022/10/04 PKT - 기록에 대한 디테일 정보를 받음.(이후 팝업 오픈까지 이어짐..)
	void SetPKRecordDetailData(PD::SC::PKT_SC_ACK_PK_BOOK_DETAIL_READ& InPacket);
	// 2022/10/04 PKT - 복수에 대한 대응 가능한 카운트 변경
	void SetRevengeRemaindCount(PD::SC::PKT_SC_ACK_PK_BOOK_REVENGE_READ& InPacket);	
	// 2022/10/04 PKT - 조롱에 대한 대응 가능한 카운트 변경
	void SetTeaseRemaindCount(PD::SC::PKT_SC_ACK_PK_BOOK_TEASE_READ& InPacket);

	// 2022/10/04 PKT - 로그인 시 경계 대상 기록을 받는다.
	void MakePKDetectionTargetSet(PD::SC::PKT_SC_MORNITORING_USER_LIST_READ& InPacket);
	// 2022/10/04 PKT - 경계 대상 추가
	void AddPKDetectionTarget(PD::SC::PKT_SC_ACK_ADD_MONITORING_USER_READ& InPacket);
	// 2022/10/04 PKT - 경계 대상 삭제
	void RemovePKDetectionTarget(PD::SC::PKT_SC_ACK_DELETE_MONITORING_USER_READ& InPacket);
	
	// 2022/10/04 PKT - 로그인 시 PK 관련 메세지 정보를 받는다.
	void MakePKMessageConfig(PD::SC::PKT_SC_PK_MESSAGE_INFO_READ& InPacket);
	// 2022/10/04 PKT - 경계 대상 로그인
	void LogInDetectionTarget(PD::SC::PKT_SC_MONITORING_USER_LOGIN_INFO_READ& InPacket);
	// 2022/10/04 PKT - PK 메세지 설정에 대한 응답
	void SavePKMessageConfig(PD::SC::PKT_SC_ACK_SAVE_PK_MESSAGE_READ& InPacket);

	// 2023/1/17 PKT - 조롱 메세지 설정에 대한 응답
	void SaveMockeryMessage(PD::SC::PKT_SC_ACK_SAVE_PK_TEASE_MESSAGE_READ& InPacket);
public:
	/**
	 * Client -> Server
	 */
	// 2022/10/04 PKT - 레드닷 갱신 타임 변경(서버에 저장)
	void Send_CheckOut(int64 InCheckOut);

protected:
	//void Limit
	void CutCountRecordSet(const int32 InLimitCount);
	// 2022/09/20 PKT - Send Message To Window
	void SendWindowMessage(bool InUpdateRedDot, MessageContentPKBook InMessage = MessageContentPKBook::NONE, int64 InParam = 0);
	// 2022/09/20 PKT - Find Record Data Index
	int32 FindIndexByRecordDataId(PKRecordId InId) const;
	// 2022/09/20 PKT - Find Detection Target Data Index
	int32 FindIndexByDetectionTargetUserDBId(UserDBId InId) const;


public:
	// 2022/09/20 PKT - Get Config
	const FGsSchemaPKBookConfig* GetConfig() const;

	// 2022/10/07 PKT - RedDot 기준 타임
	int64 GetCheckOutTime() const;

	// 2022/09/20 PKT - make Record Data View Set
	int32 MakeRecordDataViewSet(EPKBookFilter InFilter);
	// 2022/09/20 PKT - View Set Data
	const TArray<const FGsPKRecordData*>& GetRecordDataViewSet() const;
	// 2022/09/20 PKT - Finder
	const FGsPKRecordData* FindPKRecordDataByPKRecordId(PKRecordId InPKRecordId) const;


	// 2022/10/07 PKT - 경계대상 카운트
	const int32 GetDetectionTargetCountAll() const;
	// 2023/3/16 PKT - 경계대상 Index로 찾는다.(View Data가 아닌 점에 주의!!! 지금은 절전 모드에서 사용)
	const FGsPKDetectionTargetData* GetDetectionTargetAt(int32 InIndex) const;
	// 2022/09/20 PKT - make Detection Target View Set
	int32 MakeDetectionTargetViewSet(EPKDetectionTargetFilter InFilter);
	// 2022/09/20 PKT - View Set Data
	const TArray<const FGsPKDetectionTargetData*>& GetDetectionTargetDataViewSet() const;
	// 2022/09/20 PKT - Finder
	const FGsPKDetectionTargetData* FindDetectionTargetDataById(UserDBId InUserDBId) const;
	// 2023/1/17 PKT - DBId / 이름으로 경계대상에 추가 되어있는 유저 인지 체크
	bool CheckDetectionTarget(UserDBId InUserDBId, const FText& InUserName) const;
	// 2023/1/17 PKT - DBId / 이름으로 경계대상에 추가 되어있는 유저 인지 체크(WorldId 같이 검사)
	bool CheckDetectionTarget(UserDBId InUserDBId, WorldId InWorldId, const FText& InUserName) const;

	// 2022/09/20 PKT - 복수/조롱에 필요한 재화 정보
	bool ActionCostInfo(PKBookResult InResult, CurrencyType& OutType, Currency& OutAmount) const;

	// 2022/09/28 PKT - Pk Window Option
	bool IsShowLogInDetectionTargetOnly() const;
	bool IsPKMessageActive() const;
	const FText& GetTeasePKMessage() const;
	const FText& GetRevengePKMessage() const;

	bool IsUseMockeryMessage() const;
	const FText& GetMockeryMessage() const;

private:
	// 2022/09/21 PKT - PK Book Config Data
	void LoadConfig();

	// 2022/09/26 PKT - 기여자 유저 정보
	void MakeContributorDataByUser(
		FGsPKRecordingContributorData& OutData, PKBookResult InResult, PKContributorId InId, const FText& InUserName, GuildEmblemId InEmblemId
		, const FText& InGuildName, CreatureWeaponType InWeaponType = CreatureWeaponType::NONE, int32 InDamegeAmount = 0, WorldId InHomeWorldId = INVALID_WORLD_ID);

	// 2022/09/26 PKT - 기여자 NPC 정보
	void MakeContributorDataByNPC(
		FGsPKRecordingContributorData& OutData, PKBookResult InResult, PKContributorId InId, int32 InDamegeAmount = 0);

	// 2022/10/07 PKT - 경계 대상 이름 사전순 정렬
	void SortDetectionTargetSet();
};

#define GsPKBook() UGsScopeHolder::GetGameManagerFType<FGsPKBookManager>(UGsScopeGame::EManagerType::PKBook)