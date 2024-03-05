#pragma once


#include "Classes/GsManager.h"
#include "Classes/Collection/GsCollectionDataContainer.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageUserInfo.h"
#include "MultiLevelRank/GsMultiLevelRankData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "MultiLevelRank/GsMultiLevelRankPassivityCollection.h"
#include "RedDot/GsRedDotCheck.h"


/*
 * 승급 시스템 Data Manager
 * 진행도에 따른 Passivity Collection System 사용을 위해 IGsCollectionDataContainer 상속
 */
class T1PROJECT_API FGsMultiLevelRankManager final
	: public IGsManager, public IGsCollectionDataContainer, public IGsRedDotCheck
{	
	enum class ChangedSlotState
	{
		Force,		// 강제적으로 상태를 덮어 씀
		SortBy,		// 우선순위에 의거 상태를 교환
	};


	/************************************************************************/
	/*  멤버 변수                                                           */
	/************************************************************************/
private:		
	TArray<TPair<MessageUserInfo, FDelegateHandle>> _msgHandleUserInfoList;
	TArray<TPair<MessageContentQuest, FDelegateHandle>> _msgHandleContentQuestList;

	FGsMultiLevelRankPassivityCollection _passivityCollectionData;			// Passivity 정보 취합( 패시브 UI 출력용 )

	/**
	 * Data
	 */
	TArray<FGsMultiLevelRankData> _rankDataList;	// Rank Data

	const FGsMultiLevelRankSlotData* _activeLastSlot = nullptr;		// Activate 상태 중 가장 마지막 슬롯

	const FGsMultiLevelRankSlotData* _inProgressSlot = nullptr;		// 현재 임무 수행중인 슬롯

	const FGsMultiLevelRankSlotData* _pledgePrevSlot = nullptr;		// 서약 이전 마지막 슬롯
	int32 _selectedPledgeGroupId = INDEX_NONE;
	

	Exp _maxAccumulateExp = 0;

	bool _cheatSended = false;

	/************************************************************************/
	/* IGsCollectionDataContainer 구현                                      */
	/************************************************************************/
public:
	const IGsCollectionData* GetCollectionData() const final
	{
		return &_passivityCollectionData;
	}

	/************************************************************************/
	/*  가상 함수 구현                                                      */
	/************************************************************************/
public:
	virtual void Initialize() final;
	virtual void Finalize() final;

	/************************************************************************/
	/*  Build Rank Data                                                     */
	/************************************************************************/
protected:
	void ResetData();
	void LoadRankStaticData();			// schema load..
	void MakeLinkedRankData();			// linked slot..
	bool SetSlotState(FGsMultiLevelRankSlotData* InSlot, MultiLevelRankSlotState InState);
	void ChangeLinkedSlotState(ChangedSlotState InType, FGsMultiLevelRankSlotData* InBeginSlot
							, MultiLevelRankSlotState InState);	// state changed		

	/************************************************************************/
	/*  Collection Data	                                                    */
	/************************************************************************/
	void AddPassivity();

	/************************************************************************/
	/*  Quest			                                                    */
	/************************************************************************/
public:
	bool CheckQuestCompleted();		// story quest 완료 여부 판단
	void ReleaseQuestData();		// story quest를 지워준다.
	
	/************************************************************************/
	/*  Server <----> Client                                                */
	/************************************************************************/	
protected:
	void OnChangeUserExp(uint64 InExp);										// 경험치 변경 시 호출
	void OnQuestRankCompleted(const IGsMessageParam*);

public:	
	void Notify_MultiLevelRankList(const TArray<MultiLevelRankSlotInfo>& InSlotList, Exp InExp);	// 랭크 정보(서버->클라) ※ 최초 로그인시 1회
	void Ack_MultiLevelRankPromotion(MultiLevelRankId InRankId);			// 승급(서버->클라)
	void Ack_MultiLevelRankChangeRank(MultiLevelRankId InPrevRankId, MultiLevelRankId InNextRankId); // 랭크 변경(서버->클라)
	void Ack_QuestMultiLevelRankAccept(MultiLevelRankId InRankId);			// 임무 수락(서버->클라)
	void Ack_QuestMultiLevelRankComplete(MultiLevelRankId InRankId);		// 퀘스트 완료에 대한 응답(서버->클라)

	void SendQuestAccept(StoryId InId);										// 임무 수락(클라->서버)
	void SendQuestFinish(StoryId InId, const TArray<QuestId>& InQuestList);	// 임무 완료(클라->서버)
	void SendRankPromotion(MultiLevelRankId InId);							// 승급(클라->서버)
	void SendRankChange(MultiLevelRankId InId);								// 랭크 변경(클라->서버)


	/************************************************************************/
	/*  Find			                                                    */
	/************************************************************************/
protected:
	FGsMultiLevelRankData* FindRankById(MultiLevelRank InId);
	FGsMultiLevelRankData* FindRankByIndex(int32 InIndex);
	FGsMultiLevelRankSlotData* FindRankSlotByRankId(MultiLevelRankId InRankId);	

	
	/************************************************************************/
	/* RedDot                                                               */
	/************************************************************************/
public:
	virtual bool CheckRedDot(bool bInUpdate = true) override;

	/************************************************************************/
	/*  Get Data ...	                                                    */
	/************************************************************************/
public:
	TArray<FGsMultiLevelRankData> GetRankDataList();
	int32 GetRankMaxCount() const;

	void GetSlotDataList(OUT TArray<const FGsMultiLevelRankSlotData*> OutSlotDataList);

	const FGsMultiLevelRankData* GetRankByIndex(int32 InIndex);			// index를 key로 Rank를 찾는다.
	const FGsMultiLevelRankData* GetRankById(MultiLevelRank InId);		// Rank Id를 Key로 Rank를 찾는다.

	int32 GetIndexByRankData(const FGsMultiLevelRankData* InData);		// Rank data를 Key로 Index를 찾는다.
	int32 GetIndexByRankId(MultiLevelRank InRank);						// Rank Id를 Key로 Index를 찾는다.

	const FGsMultiLevelRankSlotData* GetActiveLastSlot() const;			// Activate 슬롯 중 가장 최종 슬롯
	
	const FGsMultiLevelRankSlotData* GetQuestInProgressSlot() const;	// Activate 슬롯 중 가장 최종 슬롯
	
	const FGsMultiLevelRankSlotData* GetTopLevleRankSlot(const FGsMultiLevelRankSlotData* InTarget) const;
	
	const FGsMultiLevelRankSlotData* GetPledgePrevSlot() const;

	const FGsMultiLevelRankSlotData* GetRankSlot(MultiLevelRank InRank, int32 InGroupId);

	int32 GetSelectedPledgeId(); // 선택된 서약의 GruopID 반환

	uint64 GetMaxAccumulateExp() { return _maxAccumulateExp; }

	bool IsPledgeSelected();

	bool CheckExpConditionById(MultiLevelRank InId);
	bool CheckPrevRankCondition(const FGsMultiLevelRankSlotData* InSlotData);
	
	bool CanAcceptQuest(const FGsMultiLevelRankSlotData* InSlotData);	// 임무 수락 가능 상태 확인
	bool CanPromotion();	// 승급 가능 상태 확인
	bool CanSelectPledge();	// 서약 선택 가능 상태 확인

	/************************************************************************/
	/*  Update Data ...	                                                    */
	/************************************************************************/
public:
	void SetCheatSended(bool InCheatSended) { _cheatSended = InCheatSended; }

	/************************************************************************/
	/*  Update Data ...	                                                    */
	/************************************************************************/
protected:
	void UpdatePledgeInfo();
};


#define GSMultiLevelRank() UGsScopeHolder::GetGameManagerFType<FGsMultiLevelRankManager>(UGsScopeGame::EManagerType::MultiLevelRank)
	