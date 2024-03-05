// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataSchema/QuestBoard/GsSchemaVillageQuestBoardConfig.h"
#include "Quest/GsQuestData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageGameObject.h"
#include "DataSchema/QuestBoard/GsSchemaRefreshCurrencyInfoList.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Message/GsMessageUI.h"
#include "DataSchema/QuestBoard/GsSchemaQuestConditionInfo.h"

using namespace PD::SC;

const FTextKey QUEST_BOARD_TEXT_KEY = TEXT("QuestBoardText");

UENUM()
enum class EGsQuestStateType : uint8
{
	Unaccept,
	Accept,
	Complete,
};

struct FGsQuestBoardData
{
private:
	EGsQuestStateType _state;
	QuestStoryIdPair _questIdPair;
	TArray<RewardBoxItemIdPair> _rewardIdList;

public:
	FGsQuestBoardData();
	virtual ~FGsQuestBoardData();

public:
	bool IsEquals(const QuestStoryIdPair& inQuestId) { return inQuestId == _questIdPair; }
	void SetIndex(const QuestIndex inIndex) { _questIdPair.mQuestIndex = inIndex; }
	void SetState(const EGsQuestStateType inState) { _state = inState; }
	const EGsQuestStateType GetState() { return _state; }
	const QuestStoryIdPair GetQuestStoryId() {return _questIdPair;}
	const TArray<RewardBoxItemIdPair> GetRewardArray() {return _rewardIdList;}
	void Init(const PD::SC::PKT_SC_BOARD_QUEST_LIST_READ::BoardQuestInfoListIterator& inData);
	void Init(const PD::SC::PKT_SC_BOARD_QUEST_LIST_READ::AcceptedQuestInfoListIterator& inData);
	void Init(const PD::SC::PKT_SC_BOARD_QUEST_LIST_READ::CompleteQuestInfoListIterator& inData);
	const FString ToString();
};

struct FGsQuestBoardDataList
{
public:
	TArray<TSharedPtr<FGsQuestBoardData>> _questArray;

public:
	FGsQuestBoardDataList() = default;
	virtual ~FGsQuestBoardDataList();

public:
	void InitQuest(PD::SC::PKT_SC_BOARD_QUEST_LIST_READ* inPacket);
	void Clear();

public:
	TArray<TSharedPtr<FGsQuestBoardData>> Sort(EGsQuestStateType inSortType = EGsQuestStateType::Unaccept);
	int32 GetSortPriority(const EGsQuestStateType inSortType, const EGsQuestStateType inType);
	TSharedPtr<FGsQuestBoardData> FindData(const QuestStoryIdPair& InIdPair);
	void AcceptQuest(const QuestStoryIdPair& inQuestId, QuestIndex inNewQuestIndex);
	void GiveUpQuest(const QuestStoryIdPair& inQuestId, QuestIndex inNewQuestIndex);
	void CompleteQuest(const QuestStoryIdPair& inQuestId);
	TArray<TSharedPtr<FGsQuestBoardData>> GetQuestArray(EGsQuestStateType inState);	
	TArray<TSharedPtr<FGsQuestBoardData>> GetAllQuestArray();
};

struct FGsQuestBoardConfigData
{
public:
	TArray<FGsSchemaRefreshCurrencyInfoList> _refreshCostDataArray;
	const struct FGsSchemaVillageQuestBoardConfig* _config;

private:
	int32 _maxRefreshCount;
	int32 _minRefreshCount;

public:
	void Initialize(const struct FGsSchemaVillageQuestBoardConfig* inConfig);
	const struct FGsSchemaRefreshCurrencyInfoList* GetCost(int32 inRefreshCount);

private:
	void Sort();
	void SetMinMaxRefresh();
};

/**
 * 퀘스트 게시판
 */
class T1PROJECT_API FGsGameStateQuestBoard : public FGsContentsGameBase
{
private:
	MsgGameObjHandle _interactionMsgHandler;
	TMap<QuestBoardType, TSharedPtr<FGsQuestBoardDataList>> _questMap;
	CreatureId _creatureId;
	TMap<QuestBoardType, TSharedPtr<FGsQuestBoardConfigData>> _configMap;

	//quest
private:
	QuestBoardType _questBoardType;
	QuestStoryIdPair _selectQuest;	

	//daily
private:
	int32 _availableQuestCount = 0;
	int32 _dailyAcceptableQuest = 0;

	//refresh
private:
	int32 _refreshCount = 0;
	//일일 새로고침 가능한 최대 수
	int32 _dailyMaxRefreshCount = 0;
	int32 _minRefreshCount = 0;
	int32 _maxRefreshCount = 0;

	//restore
private:
	QuestBoardType _saveQuestType;
	QuestStoryIdPair _saveQuest;
	CreatureId _saveCreature;
	EGsQuestStateType _saveSortType;
	bool _saveIsUnacceptOrder;

	//sort
private:
	EGsQuestStateType _sortType = EGsQuestStateType::Unaccept;
	bool _isSelectFirst = true;

private:
	TArray<TPair<MessageUI, FDelegateHandle>> _uiDelegateArray;

public:
	FGsGameStateQuestBoard();
	virtual ~FGsGameStateQuestBoard();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	void SetQuestBoardData(PD::SC::PKT_SC_BOARD_QUEST_LIST_READ* inPacket);
	void ClearSelectQuestData();
	void ClearQuestBoardData();
	//하루에 최대 수주 가능한 퀘스트 갯수
	int32 GetDailyMaxAcceptableQuestCount();
	//하루에 수주 가능한 퀘스트 갯수
	int32 GetDailyAccetableQuestCount();
	//하루에 완료한 퀘스트 갯수
	int32 GetDailyCompleteQuestcount();
	void SetAvailableQuestCount(int32 inCount) { _availableQuestCount = inCount; }
	int32 GetAvailableQuestCount() { return _availableQuestCount; }
	void SetCreatureId(CreatureId inCreatureId) { _creatureId = inCreatureId; }
	TSharedPtr<FGsQuestBoardData> GetQuestData(const QuestStoryIdPair inIdPair);

	//select
public:
	void SetQuestBoardType(QuestBoardType inQuest);
	void SelectQuestBoardType(QuestBoardType inQuest);
	void SetSelectQuest(const QuestStoryIdPair& inQuest);
	const QuestStoryIdPair& GetSelectQuest();
	QuestBoardType GetQuestBoardType() { return _questBoardType; }
	void SetSelectFirst(bool inSelect) { _isSelectFirst = inSelect; }
	bool GetSelectFirst() { return _isSelectFirst; }
	void SelectFirstQuestBoard();

	//refresh
public:
	int32 GetRefreshCount();
	const FGsSchemaRefreshCurrencyInfoList* GetCostInfo();

	//accept
public:
	bool IsAccpetQuest(const QuestStoryIdPair& inQuest);
	void AcceptQuestBoard(const QuestStoryIdPair& inQuestId, QuestIndex inNewQuestIndex);
	int32 GetAcceptedQuestCount();
	bool IsCanAcceptQuest();

	//complete
public:
	bool IsCompleteQuest(const QuestStoryIdPair& inQuest);
	void CompleteQuestBoard(const QuestStoryIdPair& inQuestId);

	//give up
public:
	void GiveUpQuestBoard(const QuestStoryIdPair& inQuestId, QuestIndex inNewQuestIndex);

	//send packet
public:
	void SendRefreshQuestBaord();
	void SendAcceptQuestBoard();
	void SendGiveUpQuestBoard();
	void SendCompleteQuestBoard();

public:
	TArray<TSharedPtr<FGsQuestBoardData>> GetQuestDataArray();
	int32 GetQuestBoardCount() {return GetQuestDataArray().Num();}
	void InitializeSetting();

	//restore
public:
	void RecoveryQuestBoard();
	void SaveQuestBoard();

	//sort
public:
	EGsQuestStateType GetSortType();
	void SetSortType(EGsQuestStateType inSortType);

	//interaction
private:
	void OnInteractionContents(const struct IGsMessageParam* In_data);
	void OnInvalidQuestIndex(const struct IGsMessageParam* inParam);

	//config
private:
	void InitConfig();

public:
	const struct FGsSchemaVillageQuestBoardConfig* GetConfig(QuestBoardType inQuestBoard);
	static void PopupNetError(PD::Result inResult, CreatureId inCreatureId);
	static bool TryGetQuestText(StoryId inStoryId, OUT FText& outQuestText);

public:
	bool TryGetLockText(OUT FText& outLockText, QuestBoardType inQuestBoardType);
	bool TryGetMainQuestCondition(QuestBoardType inQuestBoardType, OUT const FGsSchemaQuestConditionInfo*& outQuestCondition);
	bool IsValidMainQuestCondition(QuestBoardType inType);
	bool IsValidQuestCondition(QuestBoardType inType);
	void SetNavigationBar(CurrencyType InType);

public:
	void Print(QuestBoardType InType);

public:
	TSharedPtr<FGsQuestBoardDataList> GetQuestBoardList(QuestBoardType InType);

	public:
		static FGsGameStateQuestBoard* GetInstance();
};
