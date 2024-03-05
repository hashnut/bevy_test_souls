// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Quest/Management/GsQuestTargetManagement.h"
#include "Quest/Management/GsQuestActionManagement.h"
#include "Quest/Management/GsQuestGuideManagement.h"
#include "Quest/GsQuestPoolManager.h"
#include "Quest/Dialog/SubDialog/GsSubDialog.h"
#include "Quest/GsQuestData.h"
#include "Quest/Management/GsQuestTargetManagement.h"
#include "Quest/EGsVisibleType.h"
#include "Quest/EGsQuestCheckType.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "RedDot/GsRedDotCheck.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "GsQuestManager.generated.h"

class FGsQuestMainTableCacheSet;
class FGsQuestSubTableCacheSet;
class FGsQuestRepeatTableCacheSet;
class FGsQuestGuildTableCacheSet;
class FGsQuestGuideTableCacheSet;
class FGsQuestHandler;
class FGsQuestTargetManagement;
class FGsQuestGuideManagement;
class FGsQuestRankTableCacheSet;
struct FGsSpawnObjectShowHide;
struct FGsSchemaSpawnElement;
struct FGsSchemaQuest;
struct FGsQuestRepeatStoryTableCache;
struct FGsQuestGuildStoryTableCache;
struct FGsQuestMultiLevelRankStoryTableCache;
struct FGsQuestSubChapterTableCache;
struct FGsQuestGuideStoryTableCache;

/**
 * 퀘스트 매니저
 */
UCLASS()
class T1PROJECT_API UGsQuestManager :
	public UObject,
	public IGsManager,
	public IGsRedDotCheck
{
	GENERATED_BODY()
public:
	UGsQuestManager();
	UGsQuestManager(FVTableHelper& Helper); 	/** DO NOT USE. This constructor is for internal usage only for hot-reload purposes. */
	virtual ~UGsQuestManager();

private:
	// 퀘스트메인 테이블캐시 리스트
	TUniquePtr<FGsQuestMainTableCacheSet>	_mainQuestTableCacheSet;
	// 퀘스트서브 테이블캐시 리스트
	TUniquePtr<FGsQuestSubTableCacheSet>	_subQuestTableCacheSet;
	// 퀘스트서브 테이블캐시 리스트
	TUniquePtr<FGsQuestRepeatTableCacheSet>	_repeatQuestTableCacheSet;
	// 퀘스트랭크 테이블캐시 리스트
	TUniquePtr<FGsQuestRankTableCacheSet>	_rankQuestTableCacheSet;
	// 퀘스트길드 테이블캐시 리스트
	TUniquePtr<FGsQuestGuildTableCacheSet>	_guildQuestTableCacheSet;
	// 퀘스트가이드 테이블캐시 리스트
	TUniquePtr<FGsQuestGuideTableCacheSet>	_guideQuestTableCacheSet;

	// 퀘스트 풀 매니저
	TUniquePtr<FGsQuestPoolManager>			_questPoolManager;
	// 퀘스트 액션 매니지먼트
	TUniquePtr<FGsQuestActionManagement>	_questActionManagement;
	// 퀘스트 타겟 매니지먼트	
	TUniquePtr<FGsQuestTargetManagement>	_questTargetManagement;
	// 퀘스트 가이드 매니지먼트	
	TSharedPtr<FGsQuestGuideManagement>		_questGuideManagement;

	// 퀘스트 테이블캐시 리스트
	TMap<QuestId, TSharedPtr<FGsQuestData>>_questDataMap;
	// 수행중인 퀘스트 아이디 리스트
	TArray<QuestKey>						_activeQuestKeyList;
	// 현재 자동 진행 중인 퀘스트	
	QuestKey								_autoProgressQuestKey = QuestKey::KeyNone();
	// 현재 자동 진행 중인 퀘스트	
	QuestKey								_warpQuestKey = QuestKey::KeyNone();
	// 반복 퀘스트 재 습득을 위해 두루마리 아이디 저장
	ItemDBId								_repeatItemDBId;
	// 진행 중이 서브 퀘스트목록
	TMap<SpawnId, TSharedPtr<FGsQuestData>>	_activeSubQuestDataMap;

	// 진행 중이 가이드 퀘스트목록
	TMap<QuestId, TSharedPtr<FGsQuestData>>	_activeGuideQuestDataMap;

	// 현재 메인 퀘스트 id	
	QuestId									_currMainQuestId = 0;

	// 도착 위치 그릴꺼야?
	bool _isDrawQuestGoalSpot = false;

	// 퀘스트 아이디 찍을꺼야??
#if WITH_EDITOR
	bool _isShowQuestId = true;
#else
	bool _isShowQuestId = false;
#endif

	bool _isSetDebugTest = false;	

	// 퀘스트 npc에게 자동이동 중인가?
	bool _isMovingToNpc = false;
	// 퀘스트 npc에게 워프 상태인가?
	bool _isWarpToNpc = false;
	// 워프 대상npc Id
	int32 _warpToNpcId = 0;
	// 캐릭터 스폰 이후 자동으로 퀘스트를 진행 할지 여부
	// 퀘스트 허브UI 워프&자동이동, 퀘스트 windowUI 워프&자동이동
	bool _isSpawnMeCompleteStartProgress = false;

	// 퀘스트 습득 전 npc 위치로 이동 시
	int _gotoMoveNpcId = 0;
	int	_gotoMoveNpcMapId = 0;
	bool _isGotoMoveNpc = false;
	TArray<FVector> _gotoMoveNpcPos;

	// 반복퀘스트 자동진행 관련 상태값
	bool _isRepeatQuestAutoPressing = false;
	// 반복퀘스트 진행 중에 랜덤 순간이동 주문서를 사용했을때 상태값 저장
	bool _isUseRandomScroll = false;
	bool _isUseWarpScroll = false;

	TArray<TPair<MessageContentQuest, FDelegateHandle>>	_listQuestDelegate;
	TArray < TPair<MessageGameObject, FDelegateHandle>> _listGameObjectHandle;
	MsgSystemHandleArray	_listSystemDelegate;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

	// IGsIsRedDotCheck override
	virtual bool CheckRedDot(bool bInUpdate = true) override;
	virtual void InitializeAfterResourceDownload() override;

private:
	void InitializeQuestTableCache();

public:
	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	메인
	/*-----------------------------------------------------------------------------------------------------------------------*/
	// 다음 퀘스트가 있어? 있으면 다음 퀘스트 아이디를 알려줘.
	bool IsNextQuest(QuestId inQuestId, OUT QuestId& outQuestId);
	// 퀘스트가 완료 됐을 때 다음 퀘스트Id를 찾아 핸들러에게 퀘스트 add 한다.
	bool IsNextStory(QuestId inQuestId, OUT QuestId& outQuestId);
	// 새로운 챕터가 시작되었는가?
	bool IsNewChapterOpen(QuestId inQuestId);

	void AddNextQuest(QuestId inQuestId);
	// 스토리가 완료 됐을 때 다음 스토리에 첫번째 퀘스트Id를 찾아 핸들러에게 퀘스트 add 한다.
	void AddNextStoryQuest(QuestId inQuestId);
	// 다음 스토리가 있어? 있으면 첫번째 퀘스트 아이디 알려줘.	
	// 챕터, 스토리 인덱스 초기화
	void ClearMainQuestTableCacheIndex();
	// 현재 진행중인 스토리상태
	bool IsQuestMainStateCompleted();
	// 메인 퀘스트가 완료되어 보상을 받았을때
	void RecvQuestMainRewardedMessge(const IGsMessageParam* In_data = nullptr);
	// 현재 진행중인 챕터나 스토리가 서비스 제한에 걸린 상태인가?
	// 서비스 제한에 걸려있으면 노출이 안되어야한다.
	bool IsCurrentMainExposeQuest() const;

	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	서브
	/*-----------------------------------------------------------------------------------------------------------------------*/
	// 다음 퀘스트가 있어? 있으면 다음 퀘스트 아이디를 알려줘.
	bool IsNextQuestSub(StoryId inStoryId, QuestId inQuestId, int32 inNpcId, OUT QuestId& outQuestId);
	// NPC의 스토리를 수행 중인가?
	bool IsQuestSubRuning(int32 inNpcId, OUT EQuestNpcState& outNpcState, OUT TSharedPtr<FGsQuestData>& outQuestData);
	// 내가 속한 지역이 침공이고 해당엔피씨가 침공퀘스트를 주는 npc인가?
	bool IsQuestSubInvadeRuning(QuestContentsType inQuestContentsType, QuestId inQuestId);
	// NPC가 퀘스트를 주는가?
	bool IsQuestSubNpc(int32 inNpcId, OUT TSharedPtr<FGsQuestSubChapterTableCache>& outCahpterTableCache);
	// 현재 진행중인 챕터나 스토리가 서비스 제한에 걸린 상태인가?
	// 서비스 제한에 걸려있으면 노출이 안되어야한다.
	bool IsSubExposeQuest(IN int32 inNpcId, IN StoryId inStoryId);

	// 다음 연퀘 자동 수락
	void AddNextQuestSub(StoryId inStoryId, QuestId inQuestId);
	// 서브퀘스트가 발생했을 때 NPC 상태처리
	void AcceptSubQuestUpdata(StoryId inStoryId, QuestId inQuestId);
	void RewardSubQuestUpdata(StoryId inStoryId, QuestId inQuestId);
	void GiveUpSubQuestUpdata(StoryId inStoryId, QuestId inQuestId);
	// 서브 퀘스트가 완료되어 보상을 받았을때
	void RecvQuestSubRewardedMessge(const IGsMessageParam* In_data = nullptr);
	// npc 퀘스트 컨텐츠타입 (노말, 침공...)
	QuestContentsType GetSubQuestContenType(int32 inNpcId) const;

	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	반복
	/*-----------------------------------------------------------------------------------------------------------------------*/
	bool IsNextQuestRepeat(StoryId inStoryId, QuestId inQuestId, OUT QuestId& outQuestId);
	void AddNextQuestRepeat(StoryId inStoryId, QuestId inQuestId);
	// 새로고침이 가능한 상태인가?
	bool IsQuestRefreshPossible(QuestId inQuestId, QuestIndex inQuestIndex);

	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	가이드
	/*-----------------------------------------------------------------------------------------------------------------------*/
	void AcceptGuideQuestUpdata(QuestId inQuestId);
	void UpdateGuideQuestActiveList(const IGsMessageParam* In_data = nullptr);
	void UpdateGuideQuestHud(bool inIsShow, const StoryId inStoryId);
	//void AddGuideQuestToHudAutomatically();

	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	퀘스트
	/*-----------------------------------------------------------------------------------------------------------------------*/
	// 계속 들고 있을 이유가 없기 때문에 인스턴스가 소멸할때 같이 데이터도 삭제
	void RemoveActiveQuestKey(QuestId inQuestId, QuestIndex inQuestIndex = INVALID_QUEST_INDEX);
	// 캐릭터가 삭제 될떄
	void RemoveCharacter();
	// 치트로 퀘스트를 습득하거나 인던에서 필드로 돌아갈때 퀘스트 리셋처리
	void ClearQuest();
	void ClearQuestMain();
	void ClearQuestSub(QuestId inQuestId);
	void ClearQuestRepeat(QuestKey inQuestKey);
	void ClearQuestRank(QuestId inQuestId);
	void ClearQuestGuild(QuestId inQuestId);
	void ClearQuestGuide(QuestId inQuestId);
	void ResetQuestRepeat(QuestKey inQuestKey);
	// 월드 진입 시, 재연결 시 퀘스트 리스트를 새로 셋팅하기 위해 리셋 처리
	void ClearActiveQuestIdList(QuestType inQuestType = QuestType::MAX);
	// 월드 진입 시, 재연결 시 퀘스트 리스트를 업데이트
	void UpdateActiveQuest();
	// 자동 진행 중인 퀘스트아이디 체크
	bool IsAutoProgressQuest(QuestKey inQuestKey) { return (_autoProgressQuestKey == inQuestKey); }
	// 자동 진행 중인가?
	bool IsAutoProgressing() { return (_autoProgressQuestKey.IsKeyNone()? false : true); }
	// 완료 한 퀘스트인지 체크
	bool IsCompleted(QuestId inQuestId, QuestIndex inQuestIndex = INVALID_QUEST_INDEX);
	// 완료 한 스토리인지 체크
	bool IsStoryCompleted(StoryId inStoryId);
	bool IsStoryRewarded(QuestType inType, StoryId inStoryId, QuestIndex inQuestIndex = INVALID_QUEST_INDEX);
	// 진행 중인 퀘스트인지 체크
	bool IsProgress(QuestId inQuestId, QuestIndex inQuestIndex = INVALID_QUEST_INDEX);
	bool IsProgress(QuestKey inQuestKey);
	bool IsProgress(QuestId inStartQuestId, QuestId inEndQuestID);
	// 소유한 퀘스트 중에 ACCEPTED 상태일때만 참
	bool IsQuestAccepted(QuestId inQuestId);
	// 다음퀘가 진행 퀘인지
	bool IsNextProgress(QuestId In_tableQuestId);
	// 진행 중인 퀘스트인지 체크
	bool IsVisible(const FGsSchemaSpawnElement* inSpawnData, bool In_isCheckCompleteNextQuest = false);
	bool IsVisible(bool isInitVisible, QuestId inStartQuestID, QuestId inEndQuestID,
		EGsQuestCheckType inCheckType, EGsVisibleType inVisibleType,
		bool In_isCheckCompleteNextQuest = false);
	// show info array 로 체크
	bool IsVisibleShowInfoArray(TArray<FGsSpawnObjectShowHide>& In_arrayShowInfo, bool In_isInitVisible, EGsVisibleType In_visibleType);
	// 스토리 아이디로 현재진행 중인 퀘스트의 완료상태를 얻는다.
	bool IsQuestStateCompleted(QuestType inType, StoryId inStoryId, OUT QuestId& outPogressQuestId, QuestIndex inQuestIndex = INVALID_QUEST_INDEX);
	// 스토리 아이디로 현재진행 중인 퀘스트의 워프가능 상태를 얻는다.
	bool IsTeleportPossible(QuestType inType, StoryId inStoryId);
	bool IsAutoMovePossible(QuestType inType, StoryId inStoryId);
	bool IsMovingToNpc() { return _isMovingToNpc; }
	bool IsWarpToNpc() { return _isWarpToNpc; }
	bool IsSpawnMeCompleteStartProgress() { return _isSpawnMeCompleteStartProgress; }
	bool InvadeQuest(QuestId inQuestId);
	bool IsQuestGiveUpPossible(QuestType inType, StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex = INVALID_QUEST_INDEX);
	// 인터랙션이 가능한 상태인가?
	bool IsInteractionPossible(int32 inNpcTblId, int64 inGameId);

public:
	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	메인 Geter/Seter
	/*-----------------------------------------------------------------------------------------------------------------------*/
	// 테이블
	FGsQuestMainTableCacheSet* GetQuestMainTableCache(QuestId inQuestId) const;
	FGsQuestMainTableCacheSet* GetQuestMainTableCache() const;
	StoryId GetCurrentStoryId();

	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	서브 Geter/Seter
	/*-----------------------------------------------------------------------------------------------------------------------*/
	// 테이블
	FGsQuestSubTableCacheSet* GetQuestSubTableCache() const { return _subQuestTableCacheSet.Get(); }
	TSharedPtr<FGsQuestData> GetSubQuestData(int32 inNpcId, EQuestNpcState& outNpcState);
	int32 GetQuestSubRequireLevel(QuestId inQuestId);
	bool GetSubStoryIndex(int32 npcId, StoryId inStoryId, OUT int32& outIndex);
	TSharedPtr<FGsQuestData> GetSubQuest(int32 inNpcId, OUT EQuestNpcState& outNpcState);
	void SetQuestSubData(const FGsNetUserData::FQuestInfo& inQuestInfo);
	void SetSubNpcTartget();
	void SetWarpToNpcId(int32 inNpcId) { _warpToNpcId = inNpcId; }
	int32 GetWarpToNpcId() { return _warpToNpcId; }
	void GetSubNpcList(OUT TArray<TSharedPtr<FGsSubQuestNpcMoveData>>& outSubQuestNpcMoveDataList);

	TSharedPtr<FGsQuestSubChapterTableCache> GetSubChapterTableCache(int32 inNpcId);
	
	// 챕터 지역에 아이콘 상태 ( 발급가능, 진행중, 진행불가 )
	QuestWindowUISubStateType GetSubChapterStateType(IN int32 inMapId);
	// npc 아이콘 상태 ( 발급가능, 진행중, 진행불가 )
	QuestWindowUISubStateType GetSubNpcStateType(IN int32 inNpcId);
	// 스토리 아이콘 상태 ( 발급가능, 진행중, 진행불가 )
	QuestWindowUISubStateType GetSubStoryStateType(IN int32 inNpcId, IN StoryId inStoryId);
	int GetSubStoryIndex(IN int32 inNpcId, IN StoryId inStoryId);
	int GetSubMyMapIndex();
	void GetSubQuestSlotStateInfo(IN int32 inNpcId, IN StoryId inStoryId, OUT QuestWindowUISlotType& outSlotType);

	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	반복 Geter/Seter
	/*-----------------------------------------------------------------------------------------------------------------------*/
	// 테이블
	FGsQuestRepeatTableCacheSet* GetQuestRepeatTableCache();
	TSharedPtr<FGsQuestRepeatStoryTableCache> GetQuestRepeatStoryTableCache(StoryId inStoryId) const;
	void SetQuestRepeatUseItemTlbId(ItemDBId inItemDBId) { _repeatItemDBId = inItemDBId; }
	ItemDBId GetQuestRepeatUseItemTlbId() const { return _repeatItemDBId; }
	void GetRepeatQuestKeyList(OUT TArray<QuestKey>& outList1, OUT TArray<QuestKey>& outList2, OUT TArray<QuestKey>& outList3);
	void GetRepeatQuestList(OUT TArray<TSharedPtr<FGsQuestData>>& outList);
	void GetRepeatQuestSlotStateInfo(IN StoryId inStoryId, OUT QuestWindowUISlotType& outSlotType);
	RepeatStoryType GetRepeatStoryType(StoryId inStoryId);
	RepeatStoryType GetRepeatStoryType(QuestKey inQuestKey);

	// 반복퀘스트 자동진행 상태값
	bool IsRepeatQuestAutoPressing() { return _isRepeatQuestAutoPressing; }
	void SetIsRepeatQuestAutoPressing(bool inValue) { _isRepeatQuestAutoPressing = inValue; }
	// 반복퀘스트 자동진행 중 랜덤 순간이동 주문서 사용 상태값
	bool IsUseRandomScroll() { return _isUseRandomScroll; }
	void SetIsUseRandomScroll(bool inValue) { _isUseRandomScroll = inValue; }
	bool IsUseWarpScroll() { return _isUseRandomScroll; }
	void SetIsUseWarpScroll(bool inValue) { _isUseRandomScroll = inValue; }

	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	랭크 Geter/Seter
	/*-----------------------------------------------------------------------------------------------------------------------*/
	// 테이블
	TSharedPtr<FGsQuestMultiLevelRankStoryTableCache> GetQuestRankStoryTableCache(StoryId inStoryId) const;
	bool GetRankQuestDataList(StoryId inStoryId, OUT TArray<TSharedPtr<FGsQuestData>>& outQuestList);

	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	길드 Geter/Seter
	/*-----------------------------------------------------------------------------------------------------------------------*/
	// 테이블
	TSharedPtr<FGsQuestGuildStoryTableCache> GetQuestGuildStoryTableCache(StoryId inStoryId) const;
	bool GetGuildQuestDataList(StoryId inStoryId, OUT TSharedPtr<FGsQuestData>& outQuestData);

	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	가이드 Geter/Seter
	/*-----------------------------------------------------------------------------------------------------------------------*/
	// 테이블
	TSharedPtr<FGsQuestGuideStoryTableCache> GetQuestGuideStoryTableCache(QuestId inQuestId) const;
	TSharedPtr<FGsQuestGuideStoryTableCache> GetGuideStoryTableCache(StoryId inStoryId) const;
	bool GetGuideQuestDataList(QuestId inQuestId, OUT TSharedPtr<FGsQuestData>& outQuestData);
	void GetGuideQuestList(OUT TArray<TSharedPtr<FGsQuestData>>& outList);
	//TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& GetGuideDataList();
	void GetGuideDataList(OUT TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& outGuideStoryTableCacheList);
	void GetValidGuideDataList(OUT TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& outGuideStoryTableCacheList);
	QuestId GetGuideQuestIdByStoryId(StoryId inStoryId);
	StoryId GetGuideStoryIdByQuestId(QuestId inQuestId);
	int32 GetQuestGuideProgressCount();	// 진행 중인 가이드 퀘스트 count 얻기
	int32 GetQuestGuideHudCount(); // HUD 에 띄워져 있는 가이드 퀘스트 count 얻기
	void GetGuideQuestSlotStateInfo(IN StoryId inStoryId, OUT QuestState& outQuestState, OUT QuestWindowUISlotType& outSlotType);
	int GetGuideActivatedCount();
	bool IsGuideQuestHudOn();

	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	퀘스트 Geter/Seter
	/*-----------------------------------------------------------------------------------------------------------------------*/
	// 퀘스트 스키마 로드
	bool GetSchemaQuestList(OUT TArray<const FGsSchemaQuest*>& outQuestList) const;
	// 첫 퀘스트의 아이디를 얻어온다. (첫 챕터에, 첫 스토리에, 첫퀘스트)
	QuestId GetFirstQuestId();
	QuestId GetQuestSubStoryInFirstQuestId(int32 inNpcId, StoryId inStoryId);
	// m4 임시 : 첫챕터에 첫 퀘스트 아이디
	QuestId GetFirstChapterQuestId();
	// 퀘스트 아이디로 퀘스트데이터를 얻어온다.
	TSharedPtr<FGsQuestData> GetQuestData(QuestId inQuestId) const;
	TSharedPtr<FGsQuestData> GetQuestData(QuestKey inQuestKey) const;
	// 현재 수행 중에 퀘스트 데이터 아이디 리스트
	const TArray<QuestKey>& GetActiveQuestKeyList() const { return _activeQuestKeyList; }

	bool ContainsActiveQuestList(QuestId inQuestId, QuestIndex inQuestIndex = INVALID_QUEST_INDEX)
	{
		for (QuestKey& questKey :_activeQuestKeyList)
		{
			if (questKey == QuestKey(inQuestId, inQuestIndex))
			{
				return true;
			}
		}

		return false;
	}

	// inQuestId의 퀘스트테이블
	TSharedPtr<FGsQuestTableCache> GetQuestTableCache(const QuestId& inQuestId) const;
	// 퀘스트Id로 퀘스트타입을 얻는다.
	const QuestType GetQuestType(const QuestId& inQuestId);
	// PRE_ACCEPT 상태 때 필요한 이전퀘스트 데이터
	TSharedPtr<FGsQuestData> GetPreQuestData(const QuestId& inQuestId);
	// 자동 진행 중이던 퀘스트 아이디
	const QuestKey GetAutoProgressQuestKey() { return _autoProgressQuestKey; }
	// 자동 진행 중이던 퀘스트의 현재 수행중인 오브젝티브타입
	bool GetAutoProgressObjectiveType(OUT QuestObjectiveType& outObjectiveType);
	// 워프요청 중인 퀘스트 아이디
	const QuestKey GetWarpQuestKey() { return _warpQuestKey; }
	// 타입으로 퀘스트count 얻기
	int32 GetQuestCount(QuestType inQuestType);
	// 퀘스트 아이디로 텔레포트 정보 얻기
	bool GetQuestTeleportData(IN const QuestId inQuestId, OUT int32& outTeleportGold, OUT QuestObjectiveId& outObjectiveId);
	// 스토리Id로 요구 스토리 text 얻기
	void GetRequireStoryText(IN QuestType inQuestType, IN StoryId inStoryId, OUT FText& outText);
	// 퀘스트 아이디로 titleStringId 텍스트를 얻어온다.
	void GetTitleStringId(const QuestId inQuestId, OUT FText& outTitleStringId);
	
	// 자동진행 퀘스트
	void SetAutoProgressQuest(const QuestKey inQuestKey);
	void ClearAutoProgressQuestKey();

	// 워프요청 퀘스트
	void SetWarpQuestId(QuestKey inQuestKey) { _warpQuestKey = inQuestKey; }
	// 워프를 타거나 할 경우 퀘스트 인스턴스는 삭제 되는데
	// 서버에서 퀘스트 리스트를 다시 받지 않는다.
	// 해서 퀘스트 수행 정보를 QuestData 로 관리한다.
	void SetQuestData(const FGsNetUserData::FQuestInfo& inQuestInfo);
	void SetQuestData(const QuestId inQuestId, QuestState inState, const TArray<int32>& inObjectiveValueList, 
		EventActionIndex inPreEventActionIndex, EventActionIndex inPostEventActionIndex, 
		EventActionIndex inPostStoryEventActionIndex, QuestIndex inQuestIndex = INVALID_QUEST_INDEX);
	void SetQuestData(const QuestId inQuestId, QuestState inState, QuestIndex inQuestIndex = INVALID_QUEST_INDEX);
	void SetInitQuestData(const QuestId inQuestId);
	bool SetQuestGuideData(const FGsNetUserData::FQuestInfo& inQuestInfo);
	void SetIsMovingToNpc(bool inValue) { _isMovingToNpc = inValue; }
	void SetIsWarpToNpc(bool inValue) { _isWarpToNpc = inValue; }
	void SetIsSpawnMeCompleteStartProgress(bool inValue) { _isSpawnMeCompleteStartProgress = inValue; }
	void SetQuestRewardBoxItemIdList(QuestId inQuestId, QuestIndex inQuestIndex, uint8 inRefreshCount, 
		uint32 inRepeatScrollId, const TArray<RewardBoxItemIdPair>& inRewardBoxItemIdList);

	// RedDot
	bool IsSubQuestRedDot();
	bool IsRepeatQuestRedDot();
	bool IsGuideQuestRedDot();

	// 첫챕터에 첫퀘스트 대기상태인가?
	bool IsReadyFirstQuest();

	/*-----------------------------------------------------------------------------------------------------------------------*/
	//	관리 클래스
	/*-----------------------------------------------------------------------------------------------------------------------*/
	// 퀘스트인스턴스 관리를 위한 풀매니저
	FGsQuestPoolManager* GetQuestPoolManager() const { return _questPoolManager.Get(); }
	// 퀘스트 액션 매니지먼트 접근
	FGsQuestActionManagement* GetQuestActionManagement() const { return _questActionManagement.Get(); }
	FGsQuestTargetManagement* GetQuestTargetManagement() const { return _questTargetManagement.Get(); }
	FGsQuestGuideManagement* GetQuestGuideManagement() const { return _questGuideManagement.Get(); }
	// 퀘스트 핸들러 접근
	FGsQuestHandler* GetQuestHandler();

	// 퀘스트 등급에 따른 색깔
	const FLinearColor& GetTextColor(QuestType inType, StoryId inStoryId);
	class UPaperSprite* GetRepeatQuestGradeIcon(const QuestStoryIdPair& inPair);
	const FLinearColor& GetQuestGradeColor(const QuestStoryIdPair& inPair);

	// 도착위치 Draw set/get
	void SetDrawQuestGoalSpot(bool inIsDraw) { _isDrawQuestGoalSpot = inIsDraw; }
	bool GetDrawQuestGoalSpot() { return _isDrawQuestGoalSpot; }

	void CheatGotoQuestSubNpc(QuestId inQuestId);

public:
	// 외부에서 서브 다이얼로그 닫기
	void OnSubDialogClose();
	// 외부에서 npc 다이얼로그 닫기
	void OnNpcDialogClose();

public:	
	void SetGogotoMoveNpcId(int inNpcId) { _gotoMoveNpcId = inNpcId; }
	void SetGogotoMoveNpcMapId(int inNpcId) { _gotoMoveNpcMapId = inNpcId; }
	void SetIsGotoMoveNpc(bool inIsMove) { _isGotoMoveNpc = inIsMove; }
	void SetGogotoMoveNpcPos(const TArray<FVector>& inGotoMoveNpcPos) { _gotoMoveNpcPos = inGotoMoveNpcPos; }
	int GetGogotoMoveNpcId() { return _gotoMoveNpcId; }
	int GetGogotoMoveNpcMapId() { return _gotoMoveNpcMapId; }
	bool GetIsGotoMoveNpc() { return _isGotoMoveNpc; }
	const TArray<FVector>& GetGogotoMoveNpcPos() { return _gotoMoveNpcPos; }

public:
	// 퀘스트 id Show set/get
	void SetIsShowQuestId(bool inIsValue) { _isShowQuestId = inIsValue; }
	bool GetIsShowQuestId() { return _isShowQuestId; }
	QuestId GetCurrentMainQuestId() { return _currMainQuestId; }
	RepeatStoryGrade GetRepeatStoryGrade(const QuestStoryIdPair& inPair);
	const FString GetRepeatQuestTitle(const StoryId& inStoryId);
	static const FString GetQuestObjectText(StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex = INVALID_QUEST_INDEX);
	int32 GetCurrentAllObjectCount(const QuestId inQuestId);
	void SetDebugTest(bool inValue) { _isSetDebugTest = inValue; }
	bool GetDebugTest() { return _isSetDebugTest; }
	void GetQuestTypeText(QuestType inQuestType, OUT FText& outText);

public:
	bool GetIsChapterLockedInIndex(int32 inChapterIndex);
	bool GetIsMyChapterInIndex(int32 inChapterIndex);
};

#define GSQuest() UGsScopeHolder::GetGlobalManager<UGsQuestManager>(UGsScopeGlobal::EManagerType::Quest)
