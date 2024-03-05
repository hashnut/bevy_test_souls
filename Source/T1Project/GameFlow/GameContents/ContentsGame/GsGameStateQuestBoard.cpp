// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateQuestBoard.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageUI.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "Net/GsNetSendServiceQuest.h"
#include "Net/GsNetSendServiceWorld.h"

#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "DataSchema/QuestBoard/GsSchemaVillageQuestBoardConfig.h"
#include "GsTable.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Quest/Repeat/GsSchemaQuestRepeatStory.h"
#include "Quest/Table/GsQuestRepeatTableCacheSet.h"
#include "Quest/Table/GsQuestMainTableCacheSet.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "Unlock/GsUnlockDefine.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UTIL/GsTableUtil.h"
#include "DataSchema/QuestBoard/GsSchemaVillageQuestBoardList.h"
#include "DataSchema/Quest/Main/GsSchemaQuestMainChapter.h"

const FName WINDOW_QUEST_BOARD = TEXT("WindowQuestBoard");
const FTextKey QUEST_BOARD_MAIN_QUIEST_DENY_KEY = TEXT("QuestBoardMainquestDeny");
const FTextKey QUEST_BOARD_DENY_TEXT_KEY = TEXT("QuestBoardDeny");

//FGsQuestBoardConfigData ========================================================================================================================================
void FGsQuestBoardConfigData::Sort()
{
	_refreshCostDataArray.Sort([&](const FGsSchemaRefreshCurrencyInfoList& left, const FGsSchemaRefreshCurrencyInfoList& right)->bool {
		return left.RefreshNumber < right.RefreshNumber;
		});
}

void FGsQuestBoardConfigData::SetMinMaxRefresh()
{
	_minRefreshCount = TNumericLimits<int32>::Max();
	_maxRefreshCount = TNumericLimits<int32>::Lowest();
	int32 num = _refreshCostDataArray.Num();
	for (int32 i = 0; i < num; ++i)
	{
		if (_refreshCostDataArray[i].RefreshNumber < _minRefreshCount)
		{
			_minRefreshCount = _refreshCostDataArray[i].RefreshNumber;
		}

		if (_refreshCostDataArray[i].RefreshNumber > _maxRefreshCount)
		{
			_maxRefreshCount = _refreshCostDataArray[i].RefreshNumber;
		}
	}
}

void FGsQuestBoardConfigData::Initialize(const FGsSchemaVillageQuestBoardConfig* inConfig)
{
	_config = inConfig;
	_refreshCostDataArray = inConfig->RefreshCurrencyInfoList;
	Sort();
	SetMinMaxRefresh();
}


const FGsSchemaRefreshCurrencyInfoList* FGsQuestBoardConfigData::GetCost(int32 inRefreshCount)
{
	FGsSchemaRefreshCurrencyInfoList* temp = nullptr;
	if (nullptr == _config)
	{
		return temp;
	}

	int32 refreshCount = _config->DailyQuestRefreshLimit - inRefreshCount + 1;
	//갱신 횟수가 최대 비용보다 높으면 최대 비용으로 대체
	if (refreshCount >= _maxRefreshCount)
	{
		const FGsSchemaRefreshCurrencyInfoList* find = _config->RefreshCurrencyInfoList.FindByPredicate([&](const FGsSchemaRefreshCurrencyInfoList& currency)->bool {
			return currency.RefreshNumber == _maxRefreshCount;
			});

		if (find)
		{
			return find;
		}
	}
	//갱신 횟수가 최소 비용보다 낮으면 비용 없음
	else if (refreshCount < _minRefreshCount)
	{
		const FGsSchemaRefreshCurrencyInfoList* find = _config->RefreshCurrencyInfoList.FindByPredicate([&](const FGsSchemaRefreshCurrencyInfoList& currency)->bool {
			return currency.RefreshNumber == _minRefreshCount;
			});

		if (find)
		{
			return find;
		}
	}
	else
	{
		int32 num = _refreshCostDataArray.Num();
		const FGsSchemaRefreshCurrencyInfoList* preRefresh = nullptr;
		for (int32 i = 0; i < num; ++i)
		{
			if (refreshCount < _refreshCostDataArray[i].RefreshNumber)
			{
				return preRefresh;
			}
			preRefresh = &_refreshCostDataArray[i];
		}
	}

	return temp;
}


FGsQuestBoardData::FGsQuestBoardData()
{
#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("Create quest board data"));
#endif
}

FGsQuestBoardData::~FGsQuestBoardData()
{
#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("Destroy quest board data - quset id : %llu\tstory id : %llu\tindex : %llu"), GetQuestStoryId().mQuestId, GetQuestStoryId().mStoryId, GetQuestStoryId().mQuestIndex);
#endif
}

//FGsQuestBoardData =======================================================================================================
void FGsQuestBoardData::Init(const PD::SC::PKT_SC_BOARD_QUEST_LIST_READ::BoardQuestInfoListIterator& inData)
{
	_questIdPair = inData->QuestStoryId();
	_state = EGsQuestStateType::Unaccept;

	_rewardIdList.Empty();
	for (PD::SC::PKT_SC_BOARD_QUEST_LIST_READ::BoardQuestInfoList::RewardBoxListIterator itReward = inData->GetFirstRewardBoxListIterator();
		itReward != inData->GetLastRewardBoxListIterator(); ++itReward)
	{
		_rewardIdList.Emplace(itReward->RewardBoxItemIdPair());
	}
}

void FGsQuestBoardData::Init(const PD::SC::PKT_SC_BOARD_QUEST_LIST_READ::AcceptedQuestInfoListIterator& inData)
{
	_questIdPair = inData->QuestStoryId();
	_state = EGsQuestStateType::Accept;

	_rewardIdList.Empty();
	for (PD::SC::PKT_SC_BOARD_QUEST_LIST_READ::AcceptedQuestInfoList::RewardBoxListIterator itReward = inData->GetFirstRewardBoxListIterator();
		itReward != inData->GetLastRewardBoxListIterator(); ++itReward)
	{
		_rewardIdList.Emplace(itReward->RewardBoxItemIdPair());
	}
}

void FGsQuestBoardData::Init(const PD::SC::PKT_SC_BOARD_QUEST_LIST_READ::CompleteQuestInfoListIterator& inData)
{
	_questIdPair = inData->QuestStoryId();
	_state = EGsQuestStateType::Complete;

	_rewardIdList.Empty();
	for (PD::SC::PKT_SC_BOARD_QUEST_LIST_READ::CompleteQuestInfoList::RewardBoxListIterator itReward = inData->GetFirstRewardBoxListIterator();
		itReward != inData->GetLastRewardBoxListIterator(); ++itReward)
	{
		_rewardIdList.Emplace(itReward->RewardBoxItemIdPair());
	}
}

const FString FGsQuestBoardData::ToString()
{
	FString state = FGsTextUtil::GetStringFromEnum(TEXT("EGsQuestStateType"), _state);
	FString temp = FString::Printf(TEXT("quset id : %llu\tstory id : %llu\tindex : %llu\tstate : %s")
		, GetQuestStoryId().mQuestId
		, GetQuestStoryId().mStoryId
		, GetQuestStoryId().mQuestIndex
		, *state);

	return temp;
}

// FGsQuestBoardDataList=======================================================================================================
FGsQuestBoardDataList::~FGsQuestBoardDataList()
{
	Clear();
}


void FGsQuestBoardDataList::InitQuest(PD::SC::PKT_SC_BOARD_QUEST_LIST_READ* inPacket)
{	
	_questArray.Empty();
	for (PD::SC::PKT_SC_BOARD_QUEST_LIST_READ::BoardQuestInfoListIterator iter =
		inPacket->GetFirstBoardQuestInfoListIterator(); iter != inPacket->GetLastBoardQuestInfoListIterator(); ++iter)
	{
		TSharedPtr<FGsQuestBoardData> data = MakeShareable(new FGsQuestBoardData());
		if (false == data.IsValid())
		{
			continue;
		}

		data.Get()->Init(iter);
		_questArray.Emplace(data);
	}

	for (PD::SC::PKT_SC_BOARD_QUEST_LIST_READ::CompleteQuestInfoListIterator iter =
		inPacket->GetFirstCompleteQuestInfoListIterator(); iter != inPacket->GetLastCompleteQuestInfoListIterator(); ++iter)
	{
		TSharedPtr<FGsQuestBoardData> data = MakeShareable(new FGsQuestBoardData());
		if (false == data.IsValid())
		{
			continue;
		}

		data.Get()->Init(iter);
		_questArray.Emplace(data);
	}

	//수락된 퀘스트가 편의성을 위해 가장 늦게 받도록 수정	
	for (PD::SC::PKT_SC_BOARD_QUEST_LIST_READ::AcceptedQuestInfoListIterator iter =
		inPacket->GetFirstAcceptedQuestInfoListIterator(); iter != inPacket->GetLastAcceptedQuestInfoListIterator(); ++iter)
	{
		TSharedPtr<FGsQuestBoardData> data = MakeShareable(new FGsQuestBoardData());
		if (false == data.IsValid())
		{
			continue;
		}

		data.Get()->Init(iter);
		_questArray.Emplace(data);
	}

	static TFunction<bool(const TSharedPtr<FGsQuestBoardData>&, const TSharedPtr<FGsQuestBoardData>&)> sortFunc = [this](const TSharedPtr<FGsQuestBoardData>& source, const TSharedPtr<FGsQuestBoardData>& dest)->bool {

		//EGsQuestStateType 은 내림 차순으로 정렬
		if (GetSortPriority(EGsQuestStateType::Unaccept, source->GetState()) > GetSortPriority(EGsQuestStateType::Unaccept, dest->GetState()))
		{
			return true;
		}
		else if (GetSortPriority(EGsQuestStateType::Unaccept, source->GetState()) < GetSortPriority(EGsQuestStateType::Unaccept, dest->GetState()))
		{
			return false;
		}
		else
		{
			//등급 내림 차순으로 정렬
			RepeatStoryGrade sourceGrade = GSQuest()->GetRepeatStoryGrade(source.Get()->GetQuestStoryId());
			RepeatStoryGrade destGrade = GSQuest()->GetRepeatStoryGrade(dest.Get()->GetQuestStoryId());

			if ((int32)sourceGrade > (int32)destGrade)
			{
				return true;
			}
			else if ((int32)sourceGrade < (int32)destGrade)
			{
				return false;
			}
			else
			{
				//글씨 오름 차순으로 정렬
				const FString& sourceQuestTitle = GSQuest()->GetRepeatQuestTitle(source.Get()->GetQuestStoryId().mStoryId);
				const FString& destQuestTitle = GSQuest()->GetRepeatQuestTitle(dest.Get()->GetQuestStoryId().mStoryId);
				return sourceQuestTitle < destQuestTitle;
			}
		}

		return true;
	};

	_questArray.Sort(sortFunc);
}

void FGsQuestBoardDataList::Clear()
{
	_questArray.Empty();
}

TArray<TSharedPtr<FGsQuestBoardData>> FGsQuestBoardDataList::Sort(EGsQuestStateType inSortType)
{
	TFunction<bool(const TSharedPtr<FGsQuestBoardData>&, const TSharedPtr<FGsQuestBoardData>&)> sortFunc = [this, inSortType](const TSharedPtr<FGsQuestBoardData>& source, const TSharedPtr<FGsQuestBoardData>& dest)->bool {

		//EGsQuestStateType 내림 차순으로 정렬
		if (GetSortPriority(inSortType, source->GetState()) > GetSortPriority(inSortType, dest->GetState()))
		{
			return true;
		}
		else if(GetSortPriority(inSortType, source->GetState()) < GetSortPriority(inSortType, dest->GetState()))
		{
			return false;
		}
		else
		{
			//등급 내림 차순으로 정렬
			RepeatStoryGrade sourceGrade = GSQuest()->GetRepeatStoryGrade(source.Get()->GetQuestStoryId());
			RepeatStoryGrade destGrade = GSQuest()->GetRepeatStoryGrade(dest.Get()->GetQuestStoryId());

			if ((int32)sourceGrade > (int32)destGrade)
			{
				return true;
			}
			else if ((int32)sourceGrade < (int32)destGrade)
			{
				return false;
			}
			else
			{
				//글씨 오름 차순으로 정렬
				const FString& sourceQuestTitle = GSQuest()->GetRepeatQuestTitle(source.Get()->GetQuestStoryId().mStoryId);
				const FString& destQuestTitle = GSQuest()->GetRepeatQuestTitle(dest.Get()->GetQuestStoryId().mStoryId);
				return sourceQuestTitle < destQuestTitle;
			}
		}	

		return true;		
	};

	_questArray.Sort(sortFunc);

	return _questArray;
}

TSharedPtr<FGsQuestBoardData> FGsQuestBoardDataList::FindData(const QuestStoryIdPair& InIdPair)
{
	TSharedPtr<FGsQuestBoardData> temp;
	TSharedPtr<FGsQuestBoardData>* find = _questArray.FindByPredicate([&](const TSharedPtr<FGsQuestBoardData>& data)->bool {
		if (data.IsValid())
		{
			return data.Get()->IsEquals(InIdPair);
		}

		return false;
		});

	if (find)
	{
		temp = (*find);
	}
	
	return temp;
}

void FGsQuestBoardDataList::AcceptQuest(const QuestStoryIdPair& inQuestId, QuestIndex inNewQuestIndex)
{
	TSharedPtr<FGsQuestBoardData> data = FindData(inQuestId);
	if (false == data.IsValid())
	{
		return;
	}

	data->SetIndex(inNewQuestIndex);
	data->SetState(EGsQuestStateType::Accept);

	Sort(EGsQuestStateType::Unaccept);

#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("Accept quest board - quset id : %llu\tstory id : %llu\tindex : %llu"), data->GetQuestStoryId().mQuestId, data->GetQuestStoryId().mStoryId, data->GetQuestStoryId().mQuestIndex);
#endif
}

void FGsQuestBoardDataList::GiveUpQuest(const QuestStoryIdPair& inQuestId, QuestIndex inNewQuestIndex)
{
	TSharedPtr<FGsQuestBoardData> data = FindData(inQuestId);
	if (false == data.IsValid())
	{
		return;
	}

	data->SetIndex(inNewQuestIndex);
	data->SetState(EGsQuestStateType::Unaccept);

#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("Giveup quest board - quset id : %llu\tstory id : %llu\tindex : %llu"), inQuestId.mQuestId, inQuestId.mStoryId, inQuestId.mQuestIndex);
#endif
}

void FGsQuestBoardDataList::CompleteQuest(const QuestStoryIdPair& inQuestId)
{	
	TSharedPtr<FGsQuestBoardData> questData = FindData(inQuestId);
	_questArray.Remove(questData);

#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("Complete quest board - quset id : %llu\tstory id : %llu\tindex : %llu"), inQuestId.mQuestId, inQuestId.mStoryId, inQuestId.mQuestIndex);
#endif
}

TArray<TSharedPtr<FGsQuestBoardData>> FGsQuestBoardDataList::GetQuestArray(EGsQuestStateType inState)
{
	TArray<TSharedPtr<FGsQuestBoardData>> temp;
	for (TSharedPtr<FGsQuestBoardData> quest : _questArray)
	{
		if (quest->GetState() == inState)
		{
			temp.Emplace(quest);
		}
	}

	return temp;
}

TArray<TSharedPtr<FGsQuestBoardData>> FGsQuestBoardDataList::GetAllQuestArray()
{
	return _questArray;
}

int32 FGsQuestBoardDataList::GetSortPriority(const EGsQuestStateType inSortType, const EGsQuestStateType inType)
{
	switch (inSortType)
	{
	case EGsQuestStateType::Unaccept:
	{
		switch (inType)
		{
		case  EGsQuestStateType::Accept:
		{
			return 0;
		}
			break;
		case  EGsQuestStateType::Complete:
		{
			return 1;
		}
			break;
		case  EGsQuestStateType::Unaccept:
		{
			return 2;
		}
			break;		
		}
	}
	break;
	case EGsQuestStateType::Accept:
	{
		switch (inType)
		{
		case  EGsQuestStateType::Accept:
		{
			return 2;
		}
		break;
		case  EGsQuestStateType::Complete:
		{
			return 1;
		}
		break;
		case  EGsQuestStateType::Unaccept:
		{
			return 0;
		}
		break;
		}		
	}
	break;
	case EGsQuestStateType::Complete:
	{
		switch (inType)
		{
		case  EGsQuestStateType::Accept:
		{
			return 1;
		}
		break;
		case  EGsQuestStateType::Complete:
		{
			return 2;
		}
		break;
		case  EGsQuestStateType::Unaccept:
		{
			return 0;
		}
		break;
		}
	}
	break;
	default:
		break;
	}

	return 0;
}

//==================================================================================================================
FGsGameStateQuestBoard::FGsGameStateQuestBoard() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsQuestBoard)
{
	FGsMessageHolder* message = GMessage();
	_interactionMsgHandler = message->GetGameObject().AddRaw(MessageGameObject::INTERACTION_CONTENTS_BY_NPC_START,
		this, &FGsGameStateQuestBoard::OnInteractionContents);

	InitConfig();
}

FGsGameStateQuestBoard::~FGsGameStateQuestBoard()
{
	if (FGsMessageHolder* message = GMessage())
	{
		message->GetGameObject().Remove(_interactionMsgHandler);
	}	

	_configMap.Empty();
}

void FGsGameStateQuestBoard::Enter()
{
	FGsContentsGameBase::Enter();	

	if (FGsMessageHolder* msg = GMessage())
	{
		_uiDelegateArray.Emplace(msg->GetUI().AddRaw(MessageUI::INVALID_QUEST_BOARD_INDEX, this, &FGsGameStateQuestBoard::OnInvalidQuestIndex));
	}
	
	GUI()->OpenAndGetWidget(WINDOW_QUEST_BOARD);

	SetNavigationBar(CurrencyType::NONE);
}

void FGsGameStateQuestBoard::Exit()
{
	_questMap.Empty();

#if ACTIVE_QUEST_BOARD_EVENT_PROGRESS
	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{		
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::QUEST_BOARD);
	}
#endif

	if (FGsMessageHolder* msg = GMessage())
	{
		if (_uiDelegateArray.Num() > 0)
		{
			for (auto iter : _uiDelegateArray)
			{
				msg->GetUI().Remove(iter);
			}
			_uiDelegateArray.Empty();
		}
	}

	FGsContentsGameBase::Exit();
}

void FGsGameStateQuestBoard::SetQuestBoardData(PD::SC::PKT_SC_BOARD_QUEST_LIST_READ* inPacket)
{
	_creatureId = inPacket->NpcId();
	_refreshCount = inPacket->RefreshCount();

	//금일 수주 가능한 퀘스트 개수
	_availableQuestCount = inPacket->AcceptableCount();
	_questBoardType = inPacket->QuestBoardType();

	if (false == _questMap.Contains(_questBoardType))
	{
		_questMap.Emplace(_questBoardType, MakeShareable(new FGsQuestBoardDataList()));
	}	
	
	TSharedPtr<FGsQuestBoardDataList> boardPtr = _questMap[_questBoardType];
	if (false == boardPtr.IsValid())
	{
		return;
	}

	boardPtr->Clear();
	boardPtr->InitQuest(inPacket);

#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("=== PKT_SC_BOARD_QUEST_LIST_READ ==="));
	Print(_questBoardType);
	GSLOG(Log, TEXT("======"));
#endif	
}

void FGsGameStateQuestBoard::ClearSelectQuestData()
{
	_selectQuest = QuestStoryIdPair();
}

void FGsGameStateQuestBoard::ClearQuestBoardData()
{
	_refreshCount = 0;
	_availableQuestCount = 0;
	_questMap.Empty();
}

bool FGsGameStateQuestBoard::IsAccpetQuest(const QuestStoryIdPair& inQuest)
{
	return GSQuest()->IsProgress(inQuest.mQuestId, inQuest.mQuestIndex);
}

bool FGsGameStateQuestBoard::IsCompleteQuest(const QuestStoryIdPair& inQuest)
{
	return GSQuest()->IsCompleted(inQuest.mQuestId, inQuest.mQuestIndex);
}

int32 FGsGameStateQuestBoard::GetDailyMaxAcceptableQuestCount()
{
	if (_configMap.Contains(_questBoardType))
	{
		return _configMap[_questBoardType]->_config->DailyQuestAcceptLimit;
	}

	return 0;
}

int32 FGsGameStateQuestBoard::GetDailyAccetableQuestCount()
{
	if (_configMap.Contains(_questBoardType))
	{
		return _configMap[_questBoardType]->_config->DailyQuestNumber;
	}

	return 0;
}

int32 FGsGameStateQuestBoard::GetDailyCompleteQuestcount()
{
	if (_configMap.Contains(_questBoardType))
	{
		return _configMap[_questBoardType]->_config->DailyQuestNumber - _dailyAcceptableQuest;
	}

	return 0;
}

TSharedPtr<FGsQuestBoardData> FGsGameStateQuestBoard::GetQuestData(const QuestStoryIdPair inIdPair)
{
	TSharedPtr<FGsQuestBoardDataList> boardPtr = GetQuestBoardList(_questBoardType);
	if (boardPtr.IsValid())
	{
		return boardPtr->FindData(inIdPair);
	}

	return nullptr;
}

int32 FGsGameStateQuestBoard::GetRefreshCount()
{
	return _refreshCount;
}

void FGsGameStateQuestBoard::SetQuestBoardType(QuestBoardType inQuest)
{
	_questBoardType = inQuest;
}

void FGsGameStateQuestBoard::SelectQuestBoardType(QuestBoardType inQuest)
{	
	SetQuestBoardType(inQuest);
	SaveQuestBoard();

	FGsNetSendServiceQuest::SendReqQuestList(_creatureId, _questBoardType);
}

void FGsGameStateQuestBoard::SetSelectQuest(const QuestStoryIdPair& inQuest)
{
	_selectQuest = inQuest;

	SaveQuestBoard();
}

void FGsGameStateQuestBoard::AcceptQuestBoard(const QuestStoryIdPair& inQuestId, QuestIndex inNewQuestIndex)
{	
	TSharedPtr<FGsQuestBoardDataList> boardPtr = GetQuestBoardList(_questBoardType);
	if (false == boardPtr.IsValid())
	{
		return;
	}

	boardPtr->AcceptQuest(inQuestId, inNewQuestIndex);

	SelectFirstQuestBoard();

#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("=== AcceptQuestBoard ==="));
	Print(_questBoardType);
	GSLOG(Log, TEXT("======"));
#endif	
}

int32 FGsGameStateQuestBoard::GetAcceptedQuestCount()
{
	TSharedPtr<FGsQuestBoardDataList> boardPtr = GetQuestBoardList(_questBoardType);
	if (boardPtr.IsValid())
	{
		return boardPtr->GetQuestArray(EGsQuestStateType::Accept).Num();
	}

	return 0;
}

void FGsGameStateQuestBoard::GiveUpQuestBoard(const QuestStoryIdPair& inQuestId, QuestIndex inNewQuestIndex)
{
	TSharedPtr<FGsQuestBoardDataList> boardPtr = GetQuestBoardList(_questBoardType);
	if (false == boardPtr.IsValid())
	{
		return;
	}

	boardPtr->GiveUpQuest(inQuestId, inNewQuestIndex);

#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("=== GiveUpQuestBoard ==="));
	Print(_questBoardType);
	GSLOG(Log, TEXT("======"));
#endif	
}

void FGsGameStateQuestBoard::CompleteQuestBoard(const QuestStoryIdPair& inQuestId)
{
	TSharedPtr<FGsQuestBoardDataList> boardPtr = GetQuestBoardList(_questBoardType);
	if (false == boardPtr.IsValid())
	{
		return;
	}

	boardPtr->CompleteQuest(inQuestId);

#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("=== CompleteQuestBoard ==="));
	Print(_questBoardType);
	GSLOG(Log, TEXT("======"));
#endif	
}

void FGsGameStateQuestBoard::SendRefreshQuestBaord()
{
	FGsNetSendServiceQuest::SendReqBoardQuestRefresh(_creatureId, _questBoardType);
}

void FGsGameStateQuestBoard::SendAcceptQuestBoard()
{
#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("quest board debug - send accept quest - quest id : %lld\t story id : %lld\tindex : %lld")
		, (int64)_selectQuest.mQuestId
		, (int64)_selectQuest.mStoryId
		, (int64)_selectQuest.mQuestIndex);
#endif

	FGsNetSendServiceQuest::SendReqBoardQuestAccept(_creatureId, _selectQuest, _questBoardType);
}

void FGsGameStateQuestBoard::SendGiveUpQuestBoard()
{
#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("quest board debug - send giveup quest - quest id : %lld\t story id : %lld\tindex : %lld")
		, (int64)_selectQuest.mQuestId
		, (int64)_selectQuest.mStoryId
		, (int64)_selectQuest.mQuestIndex);
#endif

	FGsNetSendServiceQuest::SendReqQuestRepeatGiveUp(_selectQuest);
}

void FGsGameStateQuestBoard::SendCompleteQuestBoard()
{
	TSharedPtr<FGsQuestRepeatStoryTableCache> storyTableCache = GSQuest()->GetQuestRepeatStoryTableCache(_selectQuest.mStoryId);
	if (false == storyTableCache.IsValid())
	{
		return;
	}

	TWeakPtr<struct FGsQuestRepeatStoryTableCache> storyTable = storyTableCache;

	const FGsSchemaQuestRepeatStory* schemaQuestRepeatStory = storyTable.Pin()->GetSchemaQuestStory();
	if (nullptr == schemaQuestRepeatStory)
	{
		return;
	}

	int32 mainRewardId = 0;
	if (const FGsSchemaRewardData* schemaRewardData = schemaQuestRepeatStory->mainRewardId.GetRow())
		mainRewardId = schemaRewardData->id;

	int32 subRewardId = 0;
	if (const FGsSchemaRewardData* schemaRewardData = schemaQuestRepeatStory->subRewardId.GetRow())
		subRewardId = schemaRewardData->id;

	SetSelectFirst(true);

#if QUEST_BOARD_DEBUG
	GSLOG(Log, TEXT("quest board debug - send complete quest - quest id : %lld\t story id : %lld\tindex : %lld")
		, (int64)_selectQuest.mQuestId
		, (int64)_selectQuest.mStoryId
		, (int64)_selectQuest.mQuestIndex);
#endif

	FGsNetSendServiceWorld::SendQuestRepeatReward(_selectQuest.mStoryId, _selectQuest.mQuestId, _selectQuest.mQuestIndex, mainRewardId, subRewardId);
}

FGsGameStateQuestBoard* FGsGameStateQuestBoard::GetInstance()
{
	if (FGsGameFlowManager* mode = GMode())
	{
		if (FGsGameFlowGame* gameFlow = mode->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				return contents->GetContentsStateObject<FGsGameStateQuestBoard>(FGsContentsMode::InGame::ContentsQuestBoard);
			}
		}
	}

	return nullptr;
}

TArray<TSharedPtr<FGsQuestBoardData>> FGsGameStateQuestBoard::GetQuestDataArray()
{
	if (_questMap.Contains(_questBoardType))
	{
		return _questMap[_questBoardType]->GetAllQuestArray();
	}

	return TArray<TSharedPtr<FGsQuestBoardData>>();
}

const QuestStoryIdPair& FGsGameStateQuestBoard::GetSelectQuest()
{
	return _selectQuest;
}

void FGsGameStateQuestBoard::OnInteractionContents(const struct IGsMessageParam* In_data)
{
	const FGsInteractionMessageParamContents* interactionParam = In_data->Cast<const FGsInteractionMessageParamContents>();
	if (nullptr == interactionParam)
	{
		return;
	}

	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(interactionParam->_npcTblId);
	if (nullptr == npcData)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsSchemaNpcData::FindRowById is EmptyData - _npcTblId : %d"), interactionParam->_npcTblId);
#endif
		return;
	}

	if (npcData->npcFunctionType != NpcFunctionType::QUEST_BOARD)
	{
		return;
	}

	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_QUEST_BOARD);
}

void FGsGameStateQuestBoard::OnInvalidQuestIndex(const struct IGsMessageParam* inParam)
{
	const FGsUIMsgParamSelectQuestBoard* param = inParam->Cast<const FGsUIMsgParamSelectQuestBoard>();
	if (nullptr == param)
	{
		return;
	}

	QuestId questId = param->_quest.mQuestId;
	StoryId storyId = param->_quest.mStoryId;
	QuestIndex questIndex = param->_quest.mQuestIndex;
	if (_selectQuest == param->_quest)
	{
		if (-1 != param->_newQuestIndex)
		{
			_selectQuest.mQuestIndex = param->_newQuestIndex;
		}		
	}

	if (_saveQuest == param->_quest)
	{
		if (-1 != param->_newQuestIndex)
		{
			_saveQuest.mQuestIndex = param->_newQuestIndex;
		}
	}
}

void FGsGameStateQuestBoard::InitConfig()
{
	const UGsTable* table = FGsSchemaVillageQuestBoardConfig::GetStaticTable();
	const UGsTableVillageQuestBoardConfig* configTable = Cast<const UGsTableVillageQuestBoardConfig>(table);
	TArray<const FGsSchemaVillageQuestBoardConfig*> configArray;
	configTable->GetAllRows(configArray);

	_configMap.Empty();
	for (const FGsSchemaVillageQuestBoardConfig* config : configArray)
	{
		if (nullptr == config)
		{
			continue;
		}

		if (false == _configMap.Contains(config->questBoardType))
		{
			_configMap.Emplace(config->questBoardType, MakeShareable(new FGsQuestBoardConfigData()));
		}

		_configMap[config->questBoardType]->Initialize(config);
	}
}


const FGsSchemaVillageQuestBoardConfig* FGsGameStateQuestBoard::GetConfig(QuestBoardType inQuestBoard)
{
	if (_configMap.Contains(inQuestBoard))
	{
		return _configMap[inQuestBoard]->_config;
	}

	return nullptr;
}

void FGsGameStateQuestBoard::PopupNetError(PD::Result inResult, CreatureId inCreatureId)
{
	FText msg;

	switch (inResult)
	{
	case PD::Result::QUEST_ERROR_QUEST_BOARD_INVALID_QUEST_BOARD_DATA:
	{
		//find npc		
		const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(inCreatureId);
		if (npcData)
		{
			if (const FGsSchemaVillageQuestBoardList* findQuestBoard = UGsTableUtil::FindRowById<UGsTableVillageQuestBoardList, FGsSchemaVillageQuestBoardList>(npcData->questBoardGroupIdList))
			{
				if (findQuestBoard->QuestConditionInfo.Num() > 0)
				{		
					FGsGameStateQuestBoard* board = FGsGameStateQuestBoard::GetInstance();
					if (nullptr == board)
					{
						return;
					}

					board->ClearQuestBoardData();
					board->SetCreatureId(inCreatureId);					

					//퀘스트 보드 상태로 변경 
					FGsInteractionMessageParamContents sendParam(inCreatureId, 0, 0);
					GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_NPC_START, &sendParam);
					GMessage()->GetUI().SendMessage(MessageUI::INITIALIZE_QUEST_BOARD, nullptr);
				}
			}
		}				
		break;
	}
	case PD::Result::QUEST_ERROR_QUEST_BOARD_NOT_QUEST_BOARD_NPC:
	{
		FText::FindText(QUEST_BOARD_TEXT_KEY, QUEST_BOARD_DENY_TEXT_KEY, msg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, msg);
		break;
	}
	break;
	default:
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(inResult));
	}
	break;
	}
}

bool FGsGameStateQuestBoard::TryGetQuestText(StoryId inStoryId, OUT FText& outQuestText)
{
	const FGsSchemaQuestMainStory* mainStory = UGsTableUtil::FindRowById<UGsTableQuestMainStory, FGsSchemaQuestMainStory>((int32)inStoryId);
	if (nullptr == mainStory)
	{
		return false;
	}

	const FGsQuestMainTableCacheSet* questMainData = GSQuest()->GetQuestMainTableCache();
	if (nullptr == questMainData)
	{
		return false;
	}

	const TArray<TSharedPtr<FGsQuestChapterTableCache>>& chapterDataList = questMainData->GetQuestChapterTableCacheList();
	int32 chapterNum = chapterDataList.Num();
	for (int32 chapterIndex = 0; chapterIndex < chapterNum; ++chapterIndex)
	{
		TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = chapterDataList[chapterIndex];
		if (nullptr == chapterTableCache)
			continue;

		const FGsSchemaQuestMainChapter* schemaQuestChapter = chapterTableCache->GetSchemaQuestChapter();
		if (nullptr == schemaQuestChapter)
			continue;

		int32 storyNum = schemaQuestChapter->questStoryList.Num();
		for (int32 storyIndex = 0; storyIndex < storyNum; ++storyIndex)
		{
			const FGsSchemaQuestMainStory* schemaQuestMainStory = schemaQuestChapter->questStoryList[storyIndex].GetRow();
			if (nullptr == schemaQuestMainStory)
				continue;

			StoryId findStoryId = schemaQuestMainStory->id;
			if (inStoryId == findStoryId)
			{
				FText msg;
				FText::FindText(QUEST_BOARD_TEXT_KEY, QUEST_BOARD_MAIN_QUIEST_DENY_KEY, msg);
				// [{0}챕터 {1}. {0}-{2} {3}] 퀘스트를 완료해야 활성화됩니다.
				outQuestText = FText::Format(msg,
					(chapterIndex + 1), schemaQuestChapter->chapterTitleText, storyIndex + 1, mainStory->storyTitleText);

				//, storyIndex + 1, mainStory->storyTitleText
				return true;
			}
		}
	}

	return false;
}

void FGsGameStateQuestBoard::SelectFirstQuestBoard()
{
	TArray<TSharedPtr<FGsQuestBoardData>> questArray = GetQuestDataArray();
	if (questArray.IsValidIndex(0))
	{
		_selectQuest = questArray[0].Get()->GetQuestStoryId();
	}
	else
	{
		_selectQuest = QuestStoryIdPair();
	}

	SetSelectQuest(_selectQuest);
}

void FGsGameStateQuestBoard::SetSortType(EGsQuestStateType inSortType)
{
	_sortType = inSortType;

	SaveQuestBoard();

	if (_questMap.Contains(_questBoardType)
		&& _questMap[_questBoardType].IsValid())
	{
		_questMap[_questBoardType]->Sort(inSortType);
	}	
}

bool FGsGameStateQuestBoard::IsCanAcceptQuest()
{
#if QUEST_BOARD_DEBUG
	if (GSQuest()->GetQuestCount(QuestType::REPEAT) >= MAX_ACTIVE_QUEST_REPEAT_COUNT)
	{
		return false;
	}

	if (GetAvailableQuestCount() <= 0)
	{
		return false;
	}

	return true;
#else
	return (GSQuest()->GetQuestCount(QuestType::REPEAT) < MAX_ACTIVE_QUEST_REPEAT_COUNT)
		&& (GetAvailableQuestCount() > 0);
#endif
}

void FGsGameStateQuestBoard::InitializeSetting()
{
	_creatureId = 0;
	_questBoardType = QuestBoardType::VILLAGE;
	_selectQuest = QuestStoryIdPair();
	_sortType = EGsQuestStateType::Unaccept;
}

void FGsGameStateQuestBoard::RecoveryQuestBoard()
{
	_creatureId = _saveCreature;
	_questBoardType = _saveQuestType;
	_selectQuest = _saveQuest;
	_sortType = _saveSortType;
	SetSelectFirst(false);

	FGsNetSendServiceQuest::SendReqQuestList(_creatureId, _questBoardType);
}

void FGsGameStateQuestBoard::SaveQuestBoard()
{
	_saveCreature = _creatureId;
	_saveQuestType = _questBoardType;
	_saveQuest = _selectQuest;
	_saveSortType = _sortType;
}

EGsQuestStateType FGsGameStateQuestBoard::GetSortType()
{
	return _sortType;
}


const FGsSchemaRefreshCurrencyInfoList* FGsGameStateQuestBoard::GetCostInfo()
{
	if (_configMap.Contains(_questBoardType))
	{
		return _configMap[_questBoardType]->GetCost(_refreshCount);
	}

	return nullptr;
}

bool FGsGameStateQuestBoard::TryGetLockText(OUT FText& outLockText, QuestBoardType inQuestBoardType)
{
	const FGsSchemaQuestConditionInfo* data = nullptr;
	if (false == TryGetMainQuestCondition(inQuestBoardType, data))
	{
		return false;
	}

	if (nullptr == data
		|| nullptr == data->QuestMainStroy.GetRow())
	{
		return false;
	}

	StoryId questId = data->QuestMainStroy.GetRow()->id;
	return TryGetQuestText(questId, outLockText);
}

bool FGsGameStateQuestBoard::TryGetMainQuestCondition(QuestBoardType inQuestBoardType, OUT const FGsSchemaQuestConditionInfo*& outQuestCondition)
{
	outQuestCondition = nullptr;
	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(_creatureId);
	if (nullptr == npcData)
	{
		return false;
	}

	const FGsSchemaVillageQuestBoardList* findQuestBoard = UGsTableUtil::FindRowById<UGsTableVillageQuestBoardList, FGsSchemaVillageQuestBoardList>(npcData->questBoardGroupIdList);
	if (nullptr == findQuestBoard)
	{
		return false;
	}

	const TArray<FGsSchemaQuestConditionInfo>& conditionArray = findQuestBoard->QuestConditionInfo;
	TArray<const FGsSchemaQuestConditionInfo*> tempArray;
	for (const FGsSchemaQuestConditionInfo& condition : conditionArray)
	{
		if (inQuestBoardType == condition.QuestBoardType)
		{
			tempArray.Add(&condition);
		}
	}

	if (0 == tempArray.Num())
	{
		return false;
	}

	tempArray.Sort([](const FGsSchemaQuestConditionInfo& inSource, const FGsSchemaQuestConditionInfo& inDest)->bool {
		return inSource.QuestMainStroy.GetRow()->id < inDest.QuestMainStroy.GetRow()->id;
		});

	outQuestCondition = tempArray[0];

	return true;
}

bool FGsGameStateQuestBoard::IsValidMainQuestCondition(QuestBoardType inType)
{	
	const FGsSchemaQuestConditionInfo* data = nullptr;
	if (false == TryGetMainQuestCondition(inType, data))
	{
		//퀘스트 컨디션은 존재 할 수 없다. 없다면 에러 
#if QUEST_BOARD_DEBUG
		FString questType = FGsTextUtil::GetStringFromEnum(TEXT("QuestBoardType"), inType);
		GSLOG(Error, TEXT("quest board  debug - main quest condition is missing - quest type : %s\tcreature id : %u"), *questType, _creatureId);
#endif
		return false;
	}
	
	if (nullptr == data->QuestMainStroy.GetRow())
	{
		//퀘스트 컨디션에 메인 퀘스트가 없을 수 있다. 기획자의 의도에 따라 메인 퀘스트 조건 없이 보여주고 싶을 때 설정하지 않는다. 
#if QUEST_BOARD_DEBUG
		FString questType = FGsTextUtil::GetStringFromEnum(TEXT("QuestBoardType"), inType);
		GSLOG(Warning, TEXT("quest board debug - main quest condition rowpicker is missing - quest type : %s\tcreature id : %u"), *questType, _creatureId);
#endif
		return true;
	}

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		return false;
	}

#if QUEST_BOARD_DEBUG
	if (questManager->GetCurrentStoryId() > (StoryId)data->QuestMainStroy.GetRow()->id)
	{
		GSLOG(Log, TEXT("quest board debug - IsValidMainQuestCondition true - id : %lld\t id : %d"), (int64)questManager->GetCurrentStoryId(), data->QuestMainStroy.GetRow()->id);
	}
	else
	{
		GSLOG(Log, TEXT("quest board debug - IsValidMainQuestCondition false - id : %lld\t id : %d"), (int64)questManager->GetCurrentStoryId(), data->QuestMainStroy.GetRow()->id);
	}
#endif	

	//메인 퀘스트 조건이 존재한다면 현재 진행중인 메인 퀘스트와 비교하여 퀘스트 id가 높다면 퀘스트를 보여준다.
	return questManager->GetCurrentStoryId() > (StoryId)data->QuestMainStroy.GetRow()->id;
}

bool FGsGameStateQuestBoard::IsValidQuestCondition(QuestBoardType inType)
{
	const FGsSchemaQuestConditionInfo* data = nullptr;
	if (false == TryGetMainQuestCondition(inType, data))
	{
		//퀘스트 컨디션은 존재 할 수 없다.
		//예를들어 침공의뢰는 지원하지 않을 수 도 있다.
#if QUEST_BOARD_DEBUG
		FString questType = FGsTextUtil::GetStringFromEnum(TEXT("QuestBoardType"), inType);
		GSLOG(Log, TEXT("quest board  debug - main quest condition is missing - quest type : %s\tcreature id : %u"), *questType, _creatureId);
#endif
		return false;
	}

	return true;
}

TSharedPtr<FGsQuestBoardDataList> FGsGameStateQuestBoard::GetQuestBoardList(QuestBoardType InType)
{
	if (TSharedPtr<FGsQuestBoardDataList>* dataPtr = _questMap.Find(InType))
	{
		return *dataPtr;
	}
	
	return nullptr;
}

void FGsGameStateQuestBoard::Print(QuestBoardType InType)
{
	TSharedPtr<FGsQuestBoardDataList> questList = GetQuestBoardList(InType);

	for (const TSharedPtr<FGsQuestBoardData>& data : questList->_questArray)
	{
		FString state = FGsTextUtil::GetStringFromEnum(TEXT("EGsQuestStateType"), data->GetState());
		GSLOG(Log, TEXT("quest board - accept quest list  - quest id : %lld\t story id : %lld\tindex : %lld\tstate : %s")			
			, (int64)data->GetQuestStoryId().mQuestId
			, (int64)data->GetQuestStoryId().mStoryId
			, (int64)data->GetQuestStoryId().mQuestIndex
			, *state);
	}
}

void FGsGameStateQuestBoard::SetNavigationBar(CurrencyType InType)
{
	UGsUIManager* uiMgr = GUI();
	FName nameNav(TEXT("TrayNavigationBar"));
	if (false == uiMgr->IsActiveWidget(nameNav))
	{
		uiMgr->Open(nameNav);
	}

	FGsUIMsgParamInt param(static_cast<int32>(InType));
	GMessage()->GetUI().SendMessage(MessageUI::SET_NAVIGATION_BAR_CURRENCY, &param);
}
