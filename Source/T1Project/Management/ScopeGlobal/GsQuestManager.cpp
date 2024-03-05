// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestManager.h"

#include "SharedPointer.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"

#include "Quest/GsSchemaQuest.h"
#include "Quest/GsSchemaQuestObjective.h"
#include "Quest/GsQuestPoolManager.h"

#include "Quest/Main/GsSchemaQuestMainChapter.h"
#include "Quest/Dialog/SubDialog/GsSubDialog.h"
#include "Quest/Management/GsQuestActionManagement.h"
#include "Quest/Management/GsQuestGuideManagement.h"
#include "Quest/Repeat/GsSchemaQuestRepeatStory.h"
#include "Quest/Config/GsSchemaQuestConfig.h"
#include "Quest/Table/GsQuestRankTableCacheSet.h"
#include "Quest/Table/GsQuestTableCache.h"

#include "Quest/Table/GsQuestMainTableCacheSet.h"
#include "Quest/Table/GsQuestRepeatTableCacheSet.h"
#include "Quest/Table/GsQuestGuildTableCacheSet.h"
#include "Quest/Table/GsQuestGuideTableCacheSet.h"
#include "Quest/Table/GsQuestMainTableCacheSet.h"
#include "Quest/Table/GsQuestSubTableCacheSet.h"
#include "Quest/EGsVisibleType.h"
#include "Quest/EGsQuestCheckType.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Define/GsGameObjectStruct.h"

#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Tray/GsUITraySubDialog.h"
#include "UI/UIContent/Tray/GsUITrayTickerChapterOpen.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

#include "Map/Spawn/GsSchemaSpawnElement.h"
#include "Map/Spawn/GsSchemaSpawnObjectShowHide.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Quest/Sub/GsSchemaQuestSubChapter.h"
#include "DataSchema/Quest/GsSchemaQuestObjectiveAreaPos.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaDungeonData.h"

#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "Option/GsServerOption.h"
#include "UTIL/GsTableUtil.h"
#include "PaperSprite.h"

#include "Message/MessageParam/GsQuestMessageParam.h"
#include "../ContentsServer/GsContentsServerHelper.h"


UGsQuestManager::UGsQuestManager()
{

}
UGsQuestManager::UGsQuestManager(FVTableHelper& Helper) : UObject(Helper)
{

}
UGsQuestManager::~UGsQuestManager()
{

}

void UGsQuestManager::Initialize()
{
}

void UGsQuestManager::Finalize()
{
	//_acceptedSubQuestDataMap.Reset();
	//_watingSubQuestDataMap.Reset();
	_questDataMap.Reset();

	// 메인 퀘스트 테이블 set
	if (nullptr != _mainQuestTableCacheSet)
	{
		_mainQuestTableCacheSet->Finalize();
	}
	_mainQuestTableCacheSet.Reset();

	// 서브 퀘스트 테이블 set
	if (nullptr != _subQuestTableCacheSet)
	{
		_subQuestTableCacheSet->Finalize();
	}
	_subQuestTableCacheSet.Reset();

	// 반복 퀘스트 테이블 set
	if (nullptr != _repeatQuestTableCacheSet)
	{
		_repeatQuestTableCacheSet->Finalize();
	}
	_repeatQuestTableCacheSet.Reset();

	// 랭크 퀘스트 테이블 set
	if (nullptr != _rankQuestTableCacheSet)
	{
		_rankQuestTableCacheSet->Finalize();
	}
	_rankQuestTableCacheSet.Reset();

	// 길드 퀘스트 테이블 set
	if (nullptr != _guildQuestTableCacheSet)
	{
		_guildQuestTableCacheSet->Finalize();
	}
	_guildQuestTableCacheSet.Reset();

	// 가이드 퀘스트 테이블 set
	if (nullptr != _guideQuestTableCacheSet)
	{
		_guideQuestTableCacheSet->Finalize();
	}
	_guideQuestTableCacheSet.Reset();

	// 풀 삭제
	if (nullptr != _questPoolManager)
	{
		if (_questPoolManager.IsValid())
		{
			_questPoolManager->Finalize();
		}
	}

	// 퀘스트 액션 매니지먼트
	if (nullptr != _questActionManagement)
	{
		_questActionManagement->Finalize();
		_questActionManagement = nullptr;
	}

	// 퀘스트 타겟 매니지먼트
	if (nullptr != _questTargetManagement)
	{
		_questTargetManagement->Finalize();
		_questTargetManagement = nullptr;
	}

	// 퀘스트 가이드 매니지먼트
	if (nullptr != _questGuideManagement)
	{
		_questGuideManagement->Finalize();
		_questGuideManagement = nullptr;
	}

	FGsMessageHolder* msg = GMessage();
	if (nullptr != msg)
	{
		if (_listQuestDelegate.Num() != 0)
		{
			for (auto iter : _listQuestDelegate)
			{
				msg->GetQuest().Remove(iter);
			}
			_listQuestDelegate.Empty();
		}

		if (_listGameObjectHandle.Num() != 0)
		{
			for (auto iter : _listGameObjectHandle)
			{
				msg->GetGameObject().Remove(iter);
			}
			_listGameObjectHandle.Empty();
		}

		if (_listSystemDelegate.Num() != 0)
		{
			for (auto iter : _listSystemDelegate)
			{
				msg->GetSystem().Remove(iter);
			}
			_listSystemDelegate.Empty();
		}			
	}	
}

void UGsQuestManager::InitializeAfterResourceDownload()
{
	// 퀘스크메인 테이블캐시
	if (nullptr == _mainQuestTableCacheSet)
	{
		_mainQuestTableCacheSet = TUniquePtr<FGsQuestMainTableCacheSet>(new FGsQuestMainTableCacheSet());
		if (_mainQuestTableCacheSet.IsValid())
		{
			_mainQuestTableCacheSet->Initialize();
		}
	}

	// 퀘스크서브 테이블캐시
	if (nullptr == _subQuestTableCacheSet)
	{
		_subQuestTableCacheSet = TUniquePtr<FGsQuestSubTableCacheSet>(new FGsQuestSubTableCacheSet());
		if (_subQuestTableCacheSet.IsValid())
		{
			_subQuestTableCacheSet->Initialize();
		}
	}

	// 퀘스크반복 테이블캐시
	if (nullptr == _repeatQuestTableCacheSet)
	{
		_repeatQuestTableCacheSet = TUniquePtr<FGsQuestRepeatTableCacheSet>(new FGsQuestRepeatTableCacheSet());
		if (_repeatQuestTableCacheSet.IsValid())
		{
			_repeatQuestTableCacheSet->Initialize();
		}
	}

	// 퀘스크랭크 테이블캐시
	if (nullptr == _rankQuestTableCacheSet)
	{
		_rankQuestTableCacheSet = TUniquePtr<FGsQuestRankTableCacheSet>(new FGsQuestRankTableCacheSet());
		if (_rankQuestTableCacheSet.IsValid())
		{
			_rankQuestTableCacheSet->Initialize();
		}
	}

	// 퀘스크길드 테이블캐시
	if (nullptr == _guildQuestTableCacheSet)
	{
		_guildQuestTableCacheSet = TUniquePtr<FGsQuestGuildTableCacheSet>(new FGsQuestGuildTableCacheSet());
		if (_guildQuestTableCacheSet.IsValid())
		{
			_guildQuestTableCacheSet->Initialize();
		}
	}

	// 퀘스크가이드 테이블캐시
	if (nullptr == _guideQuestTableCacheSet)
	{
		_guideQuestTableCacheSet = TUniquePtr<FGsQuestGuideTableCacheSet>(new FGsQuestGuideTableCacheSet());
		if (_guideQuestTableCacheSet.IsValid())
		{
			_guideQuestTableCacheSet->Initialize();
		}
	}

	// GsQuest 풀링처리를 위한 풀생성
	if (nullptr == _questPoolManager)
	{
		_questPoolManager = TUniquePtr<FGsQuestPoolManager>(new FGsQuestPoolManager());
		_questPoolManager->Initialize();
	}

	if (nullptr == _questActionManagement)
	{
		_questActionManagement = TUniquePtr<FGsQuestActionManagement>(new FGsQuestActionManagement());
		_questActionManagement->Initialize();
	}

	if (nullptr == _questTargetManagement)
	{
		_questTargetManagement = TUniquePtr<FGsQuestTargetManagement>(new FGsQuestTargetManagement());
		_questTargetManagement->Initialize();
	}

	if (nullptr == _questGuideManagement)
	{
		// [B1] | ejrrb19 | C2URWQ-5377 | 크래시 이슈 수정. 자세한 내용은 댓글에 달아 두었습니다
		_questGuideManagement = MakeShared<FGsQuestGuideManagement>();
		//_questGuideManagement = TUniquePtr<FGsQuestGuideManagement>(new FGsQuestGuideManagement());
		_questGuideManagement->Initialize(_guideQuestTableCacheSet.Get());
	}

	InitializeQuestTableCache();
	UpdateActiveQuest();

	// 이벤트 등록
	FGsMessageHolder* message = GMessage();

	_listQuestDelegate.Emplace(
		message->GetQuest().AddUObject(MessageContentQuest::QUEST_MAIN_REWARDED, this, &UGsQuestManager::RecvQuestMainRewardedMessge));
	_listQuestDelegate.Emplace(
		message->GetQuest().AddUObject(MessageContentQuest::QUEST_SUB_REWARDED, this, &UGsQuestManager::RecvQuestSubRewardedMessge));

	_listGameObjectHandle.Emplace(
		message->GetGameObject().AddUObject(MessageGameObject::QUEST_WINDOW_GUIDE_TAB_CLOSE, this, &UGsQuestManager::UpdateGuideQuestActiveList));	

	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::BEFORE_GAME_TO_LOBBY, this, &UGsQuestManager::ClearQuest));
	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::RESTART_GAME, this, &UGsQuestManager::ClearQuest));
}

// 모든 퀘스트를 초기에 [퀘스트]-[오브젝티브] 관계를 캐시해서 들고있는다.
void UGsQuestManager::InitializeQuestTableCache()
{
	TArray<const FGsSchemaQuest*> questList;

	if (false == GetSchemaQuestList(questList))
		return;

	for (const FGsSchemaQuest* schemaQuest : questList)
	{
		QuestId questId = schemaQuest->id;
		TSharedPtr<FGsQuestData> questData;
		switch (schemaQuest->type)
		{
		case QuestType::MAIN:
		{
			questData = MakeShareable(new FGsQuestMainData());
		}
		break;
		case QuestType::SUB:
		{
			questData = MakeShareable(new FGsQuestSubData());
			if (_subQuestTableCacheSet.IsValid())
			{
				TSharedPtr<FGsQuestSubStoryTableCache> storyTableCache;
				int32 npcId;
				if (_subQuestTableCacheSet->GetStoryTableCache(questId, storyTableCache, npcId))
				{
					FGsQuestSubData* subData = static_cast<FGsQuestSubData*>(questData.Get());
					subData->SetQuestSubStoryTableCache(storyTableCache);
					subData->SetNpcID(npcId);
					StoryId storyId = storyTableCache->GetStoryId();
					subData->SetStoryId(storyId);

					QuestId nextQuestId = 0;
					if (true == IsNextQuestSub(storyId, questId, npcId, nextQuestId))
						subData->SetNextQuestId(nextQuestId);
				}
#if WITH_EDITOR
				else
				{
					const FString failTickerString =
						FString::Printf(TEXT("[QuestSub] find fail StoryId, QuestId: %llu"), questId);

					//FText failTickerText = FText::FromString(failTickerString);
					//FGsUIHelper::PopupOK(failTickerText);

					GSLOG(Error, TEXT("[QuestSub] find fail StoryId, QuestId: %llu"), questId);
				}
#endif
			}
		}
		break;
		case QuestType::REPEAT:
		{
			questData = MakeShareable(new FGsQuestRepeatData());
			if (_repeatQuestTableCacheSet.IsValid())
			{
				OUT StoryId outStoryId = 0;
				if (_repeatQuestTableCacheSet->GetStoryId(questId, outStoryId))
					questData->SetStoryId(outStoryId);
#if WITH_EDITOR
				else
				{
					GSLOG(Error, TEXT("[QuestRepeat] find fail ScrollId, QuestId: %llu"), questId);
				}
#endif
			}
		}
		break;
		case QuestType::MULTI_LEVEL_RANK:
		{
			questData = MakeShareable(new FGsQuestData());
		}
		break;
		case QuestType::GUILD:
		{
			questData = MakeShareable(new FGsQuestData());
			if (_guildQuestTableCacheSet.IsValid())
			{
				OUT StoryId outStoryId = 0;
				if (_guildQuestTableCacheSet->GetStoryId(questId, outStoryId))
				{
					questData->SetStoryId(outStoryId);
				}
			}
		}
		break;
		case QuestType::GUIDE:
		{
			questData = MakeShareable(new FGsQuestData());
			//questData->ClaimQuestDynamicData();
			if (_guideQuestTableCacheSet.IsValid())
			{
				OUT StoryId outStoryId = 0;
				if (_guideQuestTableCacheSet->GetStoryId(questId, outStoryId))
				{
					questData->SetStoryId(outStoryId);
				}
			}
		}
		break;
		default:
			break;
		}

		TSharedPtr<FGsQuestTableCache> questTableCache = MakeShareable(new FGsQuestTableCache(schemaQuest));
		questTableCache->Initialize();

		questData->SetQuestType(schemaQuest->type);
		questData->SetQuestId(questId);
		questData->SetQuestTableCache(questTableCache);
		_questDataMap.Emplace(questId, questData);
	}
}

void UGsQuestManager::UpdateActiveQuest()
{
	// 초기화
	ClearQuest();

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	int qestGuideNum = 0;
	bool isMainQuestAccepted = false;
	bool isGuideQuestAccepted = false;
	for (const FGsNetUserData::FQuestInfo& iter : userData->listQuestInfo)
	{
		switch (iter.mQuestType)
		{
		case QuestType::MAIN:
		{
			if (isMainQuestAccepted == false)
				isMainQuestAccepted = true;

			SetQuestData(iter);
		}
		break;
		case QuestType::REPEAT:
		case QuestType::MULTI_LEVEL_RANK:
		{
			SetQuestData(iter);
		}
		break;
		case QuestType::GUILD:
		{
			SetQuestData(iter);
		}
		break;
		case QuestType::GUIDE:
		{
			if (SetQuestGuideData(iter))
				++qestGuideNum;
		}
		break;
		case QuestType::SUB:
		{
			SetQuestSubData(iter);
		}
		break;
		default:
			break;
		}
	}

	// 메인퀘스트가 하나도 없을 시 첫번째 퀘스트를 삽입
	if (false == isMainQuestAccepted)
	{
		SetInitQuestData(GetFirstChapterQuestId());
	}
}

void UGsQuestManager::ClearQuestSub(QuestId inQuestId)
{	
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	FGsQuestSubData* subData = static_cast<FGsQuestSubData*>(questData.Get());
	if (nullptr == subData)
		return;

	TSharedPtr<FGsQuestSubStoryTableCache> subStoryTableCache = subData->GetQuestSubStoryTableCache();
	if (!subStoryTableCache.IsValid())
		return;

	for (QuestId questId : subStoryTableCache->GetQuestIdList())
	{
		TSharedPtr<FGsQuestData> subQuestData = GetQuestData(questId);
		if (!subQuestData.IsValid())
			continue;

		questData->ClearDynamicData();
	}

	SpawnId npcId = subData->GetNpcID();
	if (_activeSubQuestDataMap.Contains(npcId))
	{
		questData = _activeSubQuestDataMap.FindRef(npcId);
		const QuestKey& questKey = QuestKey(questData->GetQuestId());

		if (_activeQuestKeyList.Contains(questKey))
			_activeQuestKeyList.Remove(questKey);

		_activeSubQuestDataMap.Remove(npcId);
	}
}

void UGsQuestManager::SetSubNpcTartget()
{
	TArray<TPair<int32, EQuestNpcState>> subNpcList;
	for (auto& iter : _subQuestTableCacheSet->GetChapterTableCacheList())
	{
		TSharedPtr<FGsQuestSubChapterTableCache> chapterTableCache = iter.Value;
		if (!chapterTableCache.IsValid())
			continue;

		if (false == chapterTableCache->IsExposeQuest())
			continue;

		int32 npcId = iter.Key;
		EQuestNpcState outNpcState;
		TSharedPtr<FGsQuestData> outQuestData;
		if (true == IsQuestSubRuning(npcId, outNpcState, outQuestData))
		{
			if (outNpcState != EQuestNpcState::STATE_NONE)
			{
				subNpcList.Emplace(npcId, outNpcState);
			}
		}
	}

	if (0 < subNpcList.Num())
	{
		_questTargetManagement->UpdateTartgetNpcInteractionMark(true, subNpcList);
	}
}

bool UGsQuestManager::GetSubStoryIndex(int32 npcId, StoryId inStoryId, OUT int32& outIndex)
{
	TSharedPtr<FGsQuestSubChapterTableCache> chapterTableCache = _subQuestTableCacheSet->GetChapterTableCache(npcId);
	if (!chapterTableCache.IsValid())
		return false;

	TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& storyTableCacheList = chapterTableCache->GetStoryTableCacheList();
	for (int i = 0; i < storyTableCacheList.Num(); ++i)
	{
		TSharedPtr<FGsQuestSubStoryTableCache>& storyTableCache = storyTableCacheList[i];
		if (storyTableCache->GetSchemaQuestStory()->id == inStoryId)
		{
			outIndex = i;
			return true;
		}
	}

	return false;
}

TSharedPtr<FGsQuestData>  UGsQuestManager::GetSubQuest(int32 inNpcId, OUT EQuestNpcState& outNpcState)
{
	TSharedPtr<FGsQuestSubChapterTableCache> outCahpterTableCache;
	if (false == IsQuestSubNpc(inNpcId, outCahpterTableCache))
		return nullptr;

	TSharedPtr<FGsQuestData> outQuestData;
	if (false == IsQuestSubRuning(inNpcId, outNpcState, outQuestData))
		return nullptr;

	return outQuestData;
}

// 서브퀘스트를 가진 npc인가?
bool UGsQuestManager::IsQuestSubNpc(int32 inNpcId, OUT TSharedPtr<FGsQuestSubChapterTableCache>& outCahpterTableCache)
{
	outCahpterTableCache = _subQuestTableCacheSet->GetChapterTableCache(inNpcId);
	if (outCahpterTableCache.IsValid())
		return true;

	return false;
}

// 현재 진행 중이거나 진행 가능한게 있나?
bool UGsQuestManager::IsQuestSubRuning(int32 inNpcId, OUT EQuestNpcState& outNpcState, OUT TSharedPtr<FGsQuestData>& outQuestData)
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	uint16 userLevel = userData->mLevel;
	outNpcState = EQuestNpcState::STATE_NONE;

	outQuestData = _activeSubQuestDataMap.FindRef(inNpcId);
	if (!outQuestData.IsValid())
	{
		// 첫번째 꺼 찾아서 레벨체크 하자.
		TSharedPtr<FGsQuestSubChapterTableCache> cahpterTableCache = _subQuestTableCacheSet->GetChapterTableCache(inNpcId);
		if (cahpterTableCache.IsValid())
		{
			if (false == cahpterTableCache->IsExposeQuest())
				return false;

			QuestId stroyQuestId = cahpterTableCache->GetFirstStroyQuestId();
			outNpcState = EQuestNpcState::STATE_SUB_WATING;

			TSharedPtr<FGsQuestSubStoryTableCache> StoryTableCache =
				cahpterTableCache->GetStroyTableCache(cahpterTableCache->GetFirstStroyTableCache().Get()->GetStoryId());

			if (!StoryTableCache.IsValid())
				return false;

			if (false == StoryTableCache->IsExposeQuest())
				return false;

			if (const FGsSchemaQuestSubStory* schemaQuestSubStory = StoryTableCache->GetSchemaQuestStory())
			{
				if (static_cast<int32>(_mainQuestTableCacheSet->GetCurrentStoryId()) > schemaQuestSubStory->requireMainStoryId)
				{
					int32 npcId = 0;
					_subQuestTableCacheSet->GetNpcId(schemaQuestSubStory->requireSubStoryId, npcId);
					if (0 < npcId)
					{
						outNpcState = EQuestNpcState::STATE_SUB_INVALID_SUB_STORY;

						TSharedPtr<FGsQuestData> requireQuestData = _activeSubQuestDataMap.FindRef(npcId);
						if (requireQuestData.IsValid())
						{
							// 진행중인 상태
							// 어느 스토리 중인가?
							if (static_cast<int32>(requireQuestData->GetStoryId()) > schemaQuestSubStory->requireSubStoryId)
							{
								outNpcState = EQuestNpcState::STATE_SUB_WATING;
							}
							else if (static_cast<int32>(requireQuestData->GetStoryId()) == schemaQuestSubStory->requireSubStoryId)
							{
								if (FGsQuestDynamicData* requireDynamicData = requireQuestData->GetQuestDynamicData())
								{
									if (requireDynamicData->GetQuestState() == QuestState::REWARDED ||
										requireDynamicData->GetQuestState() == QuestState::COMPLETED)
									{
										outNpcState = EQuestNpcState::STATE_SUB_WATING;
									}
								}
							}
						}
					}
				}
				else
				{
					outNpcState = EQuestNpcState::STATE_SUB_INVALID_MAIN_STORY;
				}
			}

			if (outNpcState == EQuestNpcState::STATE_SUB_WATING)
			{
				uint16 requireLevel = GetQuestSubRequireLevel(stroyQuestId);
				outNpcState = (userLevel < requireLevel) ? EQuestNpcState::STATE_SUB_INVALID_REQUIRE_LEVEL : outNpcState;
			}

			outQuestData = GetQuestData(stroyQuestId);

			return IsQuestSubInvadeRuning(cahpterTableCache->GetQuestContentsType(), outQuestData->GetQuestId());
		}
		return false;
	}
	else
	{
		TSharedPtr<FGsQuestSubChapterTableCache> cahpterTableCache = _subQuestTableCacheSet->GetChapterTableCache(inNpcId);
		if (!cahpterTableCache.IsValid())
			return false;

		if (false == cahpterTableCache->IsExposeQuest())
			return false;

		FGsQuestSubData* subData = static_cast<FGsQuestSubData*>(outQuestData.Get());
		if (nullptr == subData)
			return false;

		TSharedPtr<FGsQuestSubStoryTableCache> StoryTableCache = cahpterTableCache->GetStroyTableCache(subData->GetStoryId());
		if (!StoryTableCache.IsValid())
			return false;

		if (false == StoryTableCache->IsExposeQuest())
			return false;

		FGsQuestDynamicData* dynamicData = subData->GetQuestDynamicData();
		if (nullptr == dynamicData)
			dynamicData = subData->ClaimQuestDynamicData();

		if (nullptr == dynamicData)
			return false;

		switch (dynamicData->GetQuestState())
		{
		case QuestState::NONE:
		{
			// 수락대기
			uint16 requireLevel = GetQuestSubRequireLevel(outQuestData->GetQuestId());
			outNpcState = EQuestNpcState::STATE_SUB_WATING;

			if (const FGsSchemaQuestSubStory* schemaQuestSubStory = StoryTableCache->GetSchemaQuestStory())
			{
				if (static_cast<int32>(_mainQuestTableCacheSet->GetCurrentStoryId()) > schemaQuestSubStory->requireMainStoryId)
				{
					int32 npcId = 0;
					_subQuestTableCacheSet->GetNpcId(schemaQuestSubStory->requireSubStoryId, npcId);
					if (0 < npcId)
					{
						outNpcState = EQuestNpcState::STATE_SUB_INVALID_SUB_STORY;

						TSharedPtr<FGsQuestData> requireQuestData = _activeSubQuestDataMap.FindRef(npcId);
						if (requireQuestData.IsValid())
						{
							// 진행중인 상태
							// 어느 스토리 중인가?
							if (static_cast<int32>(requireQuestData->GetStoryId()) > schemaQuestSubStory->requireSubStoryId)
							{
								outNpcState = EQuestNpcState::STATE_SUB_WATING;
							}
							else if (static_cast<int32>(requireQuestData->GetStoryId()) == schemaQuestSubStory->requireSubStoryId)
							{
								if (FGsQuestDynamicData* requireDynamicData = requireQuestData->GetQuestDynamicData())
								{
									if (requireDynamicData->GetQuestState() == QuestState::REWARDED ||
										requireDynamicData->GetQuestState() == QuestState::COMPLETED)
									{
										outNpcState = EQuestNpcState::STATE_SUB_WATING;
									}
								}
							}
						}
					}
				}
				else
				{
					outNpcState = EQuestNpcState::STATE_SUB_INVALID_MAIN_STORY;
				}
			}

			if (outNpcState == EQuestNpcState::STATE_SUB_WATING)
			{
				outNpcState = (userLevel < requireLevel) ? EQuestNpcState::STATE_SUB_INVALID_REQUIRE_LEVEL : outNpcState;
			}
		}
		break;
		case QuestState::REWARDED:
		{
			outNpcState = EQuestNpcState::STATE_NONE;
		}
		break;
		case QuestState::PRE_ACCEPT:
		case QuestState::ACCEPTED:
		{
			outNpcState = EQuestNpcState::STATE_SUB_ACCEPT;
		}
		break;
		case  QuestState::COMPLETED:
		{
			outNpcState = EQuestNpcState::STATE_REWARD_WATING;
		}
		break;
		default:
			break;
		}
		
		return IsQuestSubInvadeRuning(cahpterTableCache->GetQuestContentsType(), outQuestData->GetQuestId());
		
	}
	return true;
}

bool UGsQuestManager::IsQuestSubInvadeRuning(QuestContentsType inQuestContentsType, QuestId inQuestId)
{
	if (GGameData()->IsInvadeWorld())
	{		
		return (inQuestContentsType == QuestContentsType::INVADE)? InvadeQuest(inQuestId) : false;		
	}

	return true;	
}

TSharedPtr<FGsQuestRepeatStoryTableCache> UGsQuestManager::GetQuestRepeatStoryTableCache(StoryId inStoryId) const
{
	TSharedPtr<FGsQuestRepeatStoryTableCache> outStoryTableCache;
	if (true == _repeatQuestTableCacheSet->GetRepeatStoryTableCache(inStoryId, outStoryTableCache))
		return outStoryTableCache;

	return nullptr;
}

int32 UGsQuestManager::GetQuestSubRequireLevel(QuestId inQuestId)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return 0;

	FGsQuestSubData* subData = static_cast<FGsQuestSubData*>(questData.Get());
	if (nullptr == subData)
		return 0;

	TSharedPtr<FGsQuestSubStoryTableCache> subStoryTableCache = subData->GetQuestSubStoryTableCache();
	if (!subStoryTableCache.IsValid())
		return 0;

	return subStoryTableCache->GetSchemaQuestStory()->requireLevel;
}

void UGsQuestManager::AcceptSubQuestUpdata(StoryId inStoryId, QuestId inQuestId)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	FGsQuestSubData* subData = static_cast<FGsQuestSubData*>(questData.Get());
	if (nullptr == subData)
		return;

	_activeSubQuestDataMap.Emplace(subData->GetNpcID(), questData);

	int32 npcId = subData->GetNpcID();
	EQuestNpcState outNpcState;
	TSharedPtr<FGsQuestData> outQuestData;
	if (true == IsQuestSubRuning(npcId, outNpcState, outQuestData))
	{
		_questTargetManagement->UpdateTartgetNpcInteractionMark(true, subData->GetNpcID(), EQuestNpcState::STATE_SUB_ACCEPT);
	}
	else
	{
		_questTargetManagement->UpdateTartgetNpcInteractionMark(false, subData->GetNpcID(), EQuestNpcState::STATE_NONE);
	}
}

void UGsQuestManager::RewardSubQuestUpdata(StoryId inStoryId, QuestId inQuestId)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	FGsQuestSubData* subData = static_cast<FGsQuestSubData*>(questData.Get());
	if (nullptr == subData)
		return;

	int32 npcId = subData->GetNpcID();
	// 다음 스토리에 퀘스트가 있으면 수락대기.
	TSharedPtr<FGsQuestSubStoryTableCache> subStoryTableCache =
		_subQuestTableCacheSet->GetNextSubStoryTableCache(npcId, questData->GetStoryId());
	if (!subStoryTableCache.IsValid())
	{
		_questTargetManagement->UpdateTartgetNpcInteractionMark(false, npcId, EQuestNpcState::STATE_NONE);
		return;
	}

	QuestId questId = subStoryTableCache->GetFirstQuestId();
	questData = GetQuestData(questId);
	if (!questData.IsValid())
	{
		_questTargetManagement->UpdateTartgetNpcInteractionMark(false, npcId, EQuestNpcState::STATE_NONE);
		return;
	}

	_activeSubQuestDataMap.Emplace(npcId, questData);
	EQuestNpcState outNpcState;
	TSharedPtr<FGsQuestData> outQuestData;
	if (true == IsQuestSubRuning(npcId, outNpcState, outQuestData))
	{
		_questTargetManagement->UpdateTartgetNpcInteractionMark(true, npcId, outNpcState);
	}
}

void UGsQuestManager::GiveUpSubQuestUpdata(StoryId inStoryId, QuestId inQuestId)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	FGsQuestSubData* subData = static_cast<FGsQuestSubData*>(questData.Get());
	if (nullptr == subData)
		return;

	int32 npcId = subData->GetNpcID();
	if (!_activeSubQuestDataMap.Contains(npcId))
		return;

	TSharedPtr<FGsQuestSubStoryTableCache> storyTableCache = subData->GetQuestSubStoryTableCache();
	if (!storyTableCache.IsValid())
		return;

	for (QuestId questId : storyTableCache->GetQuestIdList())
	{
		questData = GetQuestData(questId);
		if (!questData.IsValid())
			continue;

		questData->ResetDynamicData();
	}

	QuestId firstQuestId = storyTableCache->GetFirstQuestId();
	if (0 < firstQuestId)
	{
		questData = GetQuestData(firstQuestId);
		if (!questData.IsValid())
			return;

		_activeSubQuestDataMap.Emplace(npcId, questData);

		EQuestNpcState outNpcState;
		TSharedPtr<FGsQuestData> outQuestData;
		if (true == IsQuestSubRuning(npcId, outNpcState, outQuestData))
		{
			_questTargetManagement->UpdateTartgetNpcInteractionMark(true, npcId, outNpcState);
		}
	}
}

void UGsQuestManager::AcceptGuideQuestUpdata(QuestId inQuestId)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	/*if (!_activeGuideQuestDataMap.Contains(inQuestId))
		_activeGuideQuestDataMap.Emplace(inQuestId, questData);

	if (!_activeQuestKeyList.Contains(QuestKey(inQuestId)))
		_activeQuestKeyList.Emplace(QuestKey(inQuestId));*/

	/*FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
		return;

	int activeQuestNum = serverOption->GetQuestGuideUIVisibilityCount();
	if (MAX_QUEST_GUIDE_HUD_VIEW_COUNT > activeQuestNum)
	{
		serverOption->SetQuestGuideUIVisibility(true, questData->GetStoryId());
		serverOption->SetQuestGuideUserSelectHide(true, questData->GetStoryId());
	}*/
}

void UGsQuestManager::SetQuestData(const QuestId inQuestId, QuestState  inState, const TArray<int32>& inObjectiveValueList,
									EventActionIndex inPreEventActionIndex, EventActionIndex inPostEventActionIndex,	
									EventActionIndex inPostStoryEventActionIndex, QuestIndex inQuestIndex)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(inQuestIndex);
	if (nullptr == dynamicData)
	{
		dynamicData = questData->ClaimQuestDynamicData(inQuestIndex);
	}

	if (nullptr == dynamicData)
		return;

	dynamicData->SetQuestState(inState);
	dynamicData->SetPostEventActionIndex(inPostEventActionIndex);
	dynamicData->SetPreEventActionIndex(inPreEventActionIndex);
	dynamicData->SetPostStoryEventActionIndex(inPostStoryEventActionIndex);

	TArray<int32>& objectiveValueList = dynamicData->GetObjectiveValueList();
	TArray<bool> isPrintValueList;
	int inValueNum = inObjectiveValueList.Num();
	int dynamicValueNum = objectiveValueList.Num();
	if (0 == dynamicValueNum && 0 < inValueNum)
	{
		dynamicValueNum = questData->GetObjectiveNum();
		for (int i = 0; i < dynamicValueNum; ++i)
		{
			if (true == questData->GetIsTickerHide(i))
			{
				isPrintValueList.Add(false);
			}
			else if (i < inValueNum)
			{
				isPrintValueList.Add((0 != inObjectiveValueList[i]) ? true : false);
			}
		}
	}
	else
	{
		if (dynamicValueNum == inValueNum)
		{
			for (int i = 0; i < dynamicValueNum; ++i)
			{
				if (true == questData->GetIsTickerHide(i))
				{
					isPrintValueList.Add(false);
				}
				else if (i < inValueNum)
				{
					isPrintValueList.Add((objectiveValueList[i] != inObjectiveValueList[i]) ? true : false);
				}
			}
		}
	}

	dynamicData->SetQuestObjectiveValueList(inObjectiveValueList);

	if (!ContainsActiveQuestList(inQuestId, inQuestIndex))
	{
		if (questData->GetQuestType() == QuestType::MAIN)
			_currMainQuestId = inQuestId;

		if (questData->GetQuestType() != QuestType::GUIDE)
		{
			_activeQuestKeyList.Emplace(QuestKey(inQuestId, inQuestIndex));
		}
	}

	TArray<FString> objectiveTextList;
	FString allText;

	int objectiveCount = 0;
	objectiveCount = questData->GetObjectiveNum();
	for (int i = 0; i < objectiveCount; ++i)
	{
		FText questName;				// name
		int32 questObjectveValue = 0;	// static value
		int32 questDynamicValue = 0;	// dynamic value		
		FText countText;
		FText questTypeText;		// 퀘스트 타입 (메인, 서브, ... )
		FString questTypeString;

		// 카운트를 보여줘야 할지 말아야 할지..
		bool IsShowParam = questData->IsShowUIObjectiveCount(i);

		questData->GetObjectiveDestText(i, questName);
		if (IsShowParam)
		{
			questData->GetObjectiveValue(i, questObjectveValue);
			questData->GetObjectiveDynamicValue(i, questDynamicValue, inQuestIndex);
			GetQuestTypeText(questData->GetQuestType(), questTypeText);

			if (questDynamicValue >= questObjectveValue)
			{
				FText findText;
				FText::FindText(TEXT("UICommonText"), TEXT("Complete"), findText);

				countText = findText;
			}
			else
			{
				FText findText;
				FText::FindText(TEXT("QuestUIText"), TEXT("countQuest"), findText);

				// (5/10)
				countText = FText::Format(findText, questDynamicValue, questObjectveValue);
			}

			questTypeString.Empty();
			if (false == questTypeText.IsEmpty())
			{
				questTypeString = FString::Format(TEXT("[{0}] "), { *questTypeText.ToString() });
			}
		}
		else
		{
			FText findText;
			FText::FindText(TEXT("UICommonText"), TEXT("Complete"), findText);

			countText = findText;
		}

		FString resultString = FString::Format(TEXT("<shadow>{0}{1} ({2})</>\n"), { questTypeString, *questName.ToString(), *countText.ToString() });
		objectiveTextList.Emplace(resultString);
	}

	for (size_t i = 0; i < isPrintValueList.Num(); i++)
	{
		if (isPrintValueList[i])
		{
			if (i < objectiveTextList.Num())
			{
				allText += objectiveTextList[i];
			}
		}
	}

	if (!allText.IsEmpty())
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::SYSTEM, FText::FromString(allText), TrayCondition::TC_Close, SectionPriority::None);
	}
}

void UGsQuestManager::SetQuestSubData(const FGsNetUserData::FQuestInfo& inQuestInfo)
{
	QuestId questId = inQuestInfo.mQuestId;
	QuestIndex questIndex = inQuestInfo.mQuestIndex;

	TSharedPtr<FGsQuestData> questData = GetQuestData(questId);
	if (!questData.IsValid())
		return;

	FGsQuestSubData* subData = static_cast<FGsQuestSubData*>(questData.Get());
	if (nullptr == subData)
		return;

	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questIndex);
	if (nullptr == dynamicData)
	{
		dynamicData = questData->ClaimQuestDynamicData(questIndex);
	}

	int32 npcId = subData->GetNpcID();
	QuestState state = inQuestInfo.mQuestState;

	if (nullptr != dynamicData)
	{
		if (state == QuestState::REWARDED)
		{
			// 다음 스토리에 퀘스트가 있으면 수락대기.
			TSharedPtr<FGsQuestSubStoryTableCache> subStoryTableCache =
				_subQuestTableCacheSet->GetNextSubStoryTableCache(npcId, questData->GetStoryId());
			if (subStoryTableCache.IsValid())
			{
				questId = subStoryTableCache->GetFirstQuestId();
				questData = GetQuestData(questId);
				if (!questData.IsValid())
					return;

				dynamicData->SetQuestState(state = QuestState::NONE);
			}
			else
				dynamicData->SetQuestState(inQuestInfo.mQuestState);
		}
		else
		{
			dynamicData->SetQuestState(inQuestInfo.mQuestState);
			dynamicData->SetQuestObjectiveValueList(inQuestInfo.mObjectiveValueList);
		}
	}

	_activeSubQuestDataMap.Emplace(npcId, questData);
	if (!ContainsActiveQuestList(questId) && state != QuestState::NONE && state != QuestState::REWARDED)
		_activeQuestKeyList.Emplace(QuestKey(questId));
}

void UGsQuestManager::SetQuestData(const FGsNetUserData::FQuestInfo& inQuestInfo)
{
	QuestId questId = inQuestInfo.mQuestId;
	QuestIndex questIndex = inQuestInfo.mQuestIndex;

	TSharedPtr<FGsQuestData> questData = GetQuestData(questId);
	if (!questData.IsValid())
		return;

	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questIndex);
	if (nullptr == dynamicData)
	{
		dynamicData = questData->ClaimQuestDynamicData(questIndex);
	}

	if (nullptr != dynamicData)
	{
		dynamicData->SetQuestState(inQuestInfo.mQuestState);
		dynamicData->SetQuestObjectiveValueList(inQuestInfo.mObjectiveValueList);
		dynamicData->SetPostEventActionIndex(inQuestInfo.mPostEventActionIndex);
		dynamicData->SetPreEventActionIndex(inQuestInfo.mPreEventActionIndex);
	}

	if (!ContainsActiveQuestList(questId, questIndex))
	{
		if (questData->GetQuestType() == QuestType::MAIN)
		{
			if (nullptr != _mainQuestTableCacheSet)
			{
				_mainQuestTableCacheSet->InitializeTableCache(questId);
			}
			_currMainQuestId = questId;
		}

		if (questData->GetQuestType() != QuestType::GUIDE)
		{
			_activeQuestKeyList.Emplace(QuestKey(questId, questIndex));
		}
	}
}

void UGsQuestManager::SetQuestData(const QuestId inQuestId, QuestState inState, QuestIndex inQuestIndex)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(inQuestIndex);
	if (nullptr == dynamicData)
	{
		dynamicData = questData->ClaimQuestDynamicData(inQuestIndex);
	}

	if (nullptr != dynamicData)
	{
		dynamicData->SetQuestState(inState);
	}

	if (!ContainsActiveQuestList(inQuestId, inQuestIndex))
	{
		if (questData->GetQuestType() == QuestType::MAIN)
			_currMainQuestId = inQuestId;

		if (questData->GetQuestType() != QuestType::GUIDE)
		{
			_activeQuestKeyList.Emplace(QuestKey(inQuestId, inQuestIndex));
		}
	}

	// 서브 npc 상태 업데이트
	SetSubNpcTartget();
}

void UGsQuestManager::SetQuestRewardBoxItemIdList(QuestId inQuestId, QuestIndex inQuestIndex, uint8 inRefreshCount, 
	uint32 inRepeatScrollId, const TArray<RewardBoxItemIdPair>& inRewardBoxItemIdList)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	FGsQuestRepeatData* repeatData = static_cast<FGsQuestRepeatData*>(questData.Get());
	if (nullptr == repeatData)
		return;

	repeatData->SetRefreshCount(inQuestIndex, inRefreshCount);
	repeatData->SetRepeatScrollId(inQuestIndex, inRepeatScrollId);
	repeatData->SetDynamicRewardBoxItemIdPairList(inQuestIndex, inRewardBoxItemIdList);
}


void UGsQuestManager::SetInitQuestData(const QuestId inQuestId)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	const FGsSchemaQuestConfig* row = nullptr;
	if (const UGsTable* table = FGsSchemaQuestConfig::GetStaticTable())
	{
		if (table->FindRow(TEXT("0"), row))
		{
			if (FGsQuestMainData* mainData = static_cast<FGsQuestMainData*>(questData.Get()))
			{
				if (const FGsSchemaEventActionGroup* eventActionGroup = row->initEventActionGroupId.GetRow())
				{
					mainData->SetInitEventActionGroupId(eventActionGroup->id);
				}
			}
		}
	}

	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData();
	if (nullptr == dynamicData)
	{
		dynamicData = questData->ClaimQuestDynamicData();		
	}

	if (!ContainsActiveQuestList(inQuestId))
	{
		if (questData->GetQuestType() == QuestType::MAIN)
			_currMainQuestId = inQuestId;

		if (questData->GetQuestType() != QuestType::GUIDE)
		{
			_activeQuestKeyList.Emplace(QuestKey(inQuestId));
		}
	}
}

bool UGsQuestManager::SetQuestGuideData(const FGsNetUserData::FQuestInfo& inQuestInfo)
{
	QuestId questId = inQuestInfo.mQuestId;
	QuestIndex questIndex = inQuestInfo.mQuestIndex;
	QuestState questState = inQuestInfo.mQuestState;

	TSharedPtr<FGsQuestData> questData = GetQuestData(questId);
	if (!questData.IsValid())
		return false;

	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questIndex);
	if (nullptr == dynamicData)
	{
		dynamicData = questData->ClaimQuestDynamicData(questIndex);
	}

	if (nullptr != dynamicData)
	{
		dynamicData->SetQuestState(inQuestInfo.mQuestState);
		dynamicData->SetQuestObjectiveValueList(inQuestInfo.mObjectiveValueList);
	}	

	/*_activeGuideQuestDataMap.Emplace(questId, questData);
	if (!ContainsActiveQuestList(questId, questIndex) && questState != QuestState::NONE && questState != QuestState::REWARDED)
	{
		_activeQuestKeyList.Emplace(questId, questIndex);
		return true;
	}*/

	return false;
}

QuestId UGsQuestManager::GetGuideQuestIdByStoryId(StoryId inStoryId)
{
	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& guideStoryTableCacheList = _guideQuestTableCacheSet->GetGuideStoryTableCache();
	for (TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache : guideStoryTableCacheList)
	{
		if (!storyTableCache.IsValid())
			continue;

		if (inStoryId == storyTableCache->GetStoryId())
		{
			return storyTableCache->GetQuestId();
		}
	}

	return 0;
}

StoryId UGsQuestManager::GetGuideStoryIdByQuestId(QuestId inQuestId)
{
	TSharedPtr<FGsQuestGuideStoryTableCache> outStoryTableCache = GetQuestGuideStoryTableCache(inQuestId);
	if (!outStoryTableCache.IsValid())
		return 0;

	return outStoryTableCache->GetStoryId();
}

void UGsQuestManager::UpdateGuideQuestActiveList(const IGsMessageParam* In_data)
{
	/*FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
		return;

	int activeQuestNum = serverOption->GetQuestGuideUIVisibilityCount();
	int num = 0;
	const TArray<StoryId>& storyList = serverOption->GetQuestGuideUIVisibility();
	for (StoryId storyId : storyList)
	{
		QuestId questId = GetGuideQuestIdByStoryId(storyId);
		if (0 < questId)
		{
			if (!ContainsActiveQuestList(questId))
			{
				if (num < MAX_QUEST_GUIDE_HUD_VIEW_COUNT)
				{
					_activeQuestKeyList.Emplace(QuestKey(questId));
					++num;
				}
			}
		}
	}

	uint16 userLevel = 0;
	if (FGsGameDataManager* dataManager = GGameData())
	{
		if (const FGsNetUserData* userData = dataManager->GetUserData())
		{
			userLevel = userData->mLevel;
		}
	}

	if (!_mainQuestTableCacheSet.IsValid())
		return;

	StoryId currMainStoryId = _mainQuestTableCacheSet->GetCurrentStoryId();
	if (0 >= currMainStoryId)
		return;

	if (!_guideQuestTableCacheSet.IsValid())
		return;

	int addNum = (MAX_QUEST_GUIDE_HUD_VIEW_COUNT <= num) ? 0 : MAX_QUEST_GUIDE_HUD_VIEW_COUNT - num;

	num = 0;
	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& guideStoryTableCacheList = _guideQuestTableCacheSet->GetGuideStoryTableCache();
	for (TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache : guideStoryTableCacheList)
	{
		if (!storyTableCache.IsValid())
			continue;

		QuestId questId = storyTableCache->GetQuestId();
		TSharedPtr<FGsQuestData> questData = GetQuestData(questId);
		if (!questData.IsValid())
			continue;

		if (false == serverOption->IsHideQuestGuideUI(questData->GetStoryId()))
			continue;

		if (true == serverOption->IsUserSelectHideQuestGuide(questData->GetStoryId()))
			continue;

		int32 requireLevel = storyTableCache->GetRequireLevel();
		StoryId mainStoryId = storyTableCache->GetRequireMainStoryId();

		if (false == questData->IsQuestDynamicData())
		{
			continue;
		}
		else
		{
			FGsQuestDynamicData* data = questData->GetQuestDynamicData();
			if (nullptr == data)
				continue;

			if (QuestState::NONE != data->GetQuestState())
				continue;

			if (userLevel >= requireLevel && currMainStoryId > mainStoryId)
			{
				if (num < addNum)
				{
					if (!ContainsActiveQuestList(questId))
					{						
						_activeQuestKeyList.Emplace(QuestKey(questId));						
						serverOption->SetQuestGuideUIVisibility(true, questData->GetStoryId());
						++num;
					}
				}
			}
		}
	}

	if (FGsQuestHandler* QuestHandler = GetQuestHandler())
	{
		QuestHandler->UpdateQuestGuide();
	}*/
}

void UGsQuestManager::UpdateGuideQuestHud(bool inIsShow, const StoryId inStoryId)
{
	//FGsServerOption* serverOption = GServerOption();
	//if (nullptr == serverOption)
	//	return;

	//FGsQuestData* questData = nullptr;
	//bool isFind = false;
	//for (QuestKey& questKey : _activeQuestKeyList)
	//{
	//	QuestId questId = questKey._questId;

	//	TSharedPtr<FGsQuestData> questDataPtr = GetQuestData(questId);
	//	if (!questDataPtr.IsValid())
	//		continue;

	//	questData = questDataPtr.Get();

	//	if (QuestType::GUIDE == questData->GetQuestType())
	//	{
	//		if (inStoryId == questData->GetStoryId())
	//		{
	//			isFind = true;
	//			break;
	//		}
	//	}
	//}

	//if (inIsShow)
	//{
	//	if (isFind)
	//	{
	//		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
	//		return;
	//	}

	//	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& guideStoryTableCacheList = _guideQuestTableCacheSet->GetGuideStoryTableCache();
	//	for (TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache : guideStoryTableCacheList)
	//	{
	//		if (!storyTableCache.IsValid())
	//			continue;

	//		if (inStoryId != storyTableCache->GetStoryId())
	//			continue;

	//		if (true == serverOption->IsUserSelectHideQuestGuide(inStoryId))
	//			continue;

	//		_activeQuestKeyList.Emplace(QuestKey(storyTableCache->GetQuestId()));
	//		serverOption->SetQuestGuideUIVisibility(true, storyTableCache->GetStoryId());
	//		break;
	//	}
	//}
	//else
	//{
	//	if (isFind && nullptr != questData)
	//	{
	//		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData();
	//		if (nullptr != dynamicData)
	//		{
	//			if (QuestState::NONE == dynamicData->GetQuestState())
	//			{
	//				RemoveActiveQuestKey(questData->GetQuestId());
	//				GetQuestHandler()->RemoveQuestGuide(inStoryId);
	//			}
	//		}
	//	}
	//}

	//if (FGsQuestHandler* QuestHandler = GetQuestHandler())
	//{
	//	QuestHandler->UpdateQuestGuide();
	//}
}

//void UGsQuestManager::AddGuideQuestToHudAutomatically()
//{
//	FGsServerOption* serverOption = GServerOption();
//	if (nullptr == serverOption)
//		return;
//
//	int num = serverOption->GetQuestGuideUIVisibilityCount();
//	if (num >= MAX_QUEST_GUIDE_HUD_VIEW_COUNT)
//		return;
//
//	int addNum = (MAX_QUEST_GUIDE_HUD_VIEW_COUNT <= num) ? 0 : MAX_QUEST_GUIDE_HUD_VIEW_COUNT - num;
//	uint16 userLevel = 0;
//	if (FGsGameDataManager* dataManager = GGameData())
//	{
//		if (const FGsNetUserData* userData = dataManager->GetUserData())
//		{
//			userLevel = userData->mLevel;
//		}
//	}
//
//	if (!_mainQuestTableCacheSet.IsValid())
//		return;
//
//	StoryId currMainStoryId = _mainQuestTableCacheSet->GetCurrentStoryId();
//	if (0 >= currMainStoryId)
//		return;
//
//	if (!_guideQuestTableCacheSet.IsValid())
//		return;
//
//	num = 0;
//	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& guideStoryTableCacheList = _guideQuestTableCacheSet->GetGuideStoryTableCache();
//	for (TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache : guideStoryTableCacheList)
//	{
//		if (!storyTableCache.IsValid())
//			continue;
//
//		QuestId questId = storyTableCache->GetQuestId();
//		TSharedPtr<FGsQuestData> questData = GetQuestData(questId);
//		if (!questData.IsValid())
//			continue;
//
//		if (true == serverOption->IsHideQuestGuideUI(questData->GetStoryId()))
//			continue;
//
//		if (true == serverOption->IsUserSelectHideQuestGuide(questData->GetStoryId()))
//			continue;
//
//		int32 requireLevel = storyTableCache->GetRequireLevel();
//		StoryId mainStoryId = storyTableCache->GetRequireMainStoryId();
//
//		if (false == questData->IsQuestDynamicData())
//		{
//			continue;
//		}
//		else
//		{
//			FGsQuestDynamicData* data = questData->GetQuestDynamicData();
//			if (nullptr == data)
//				continue;
//
//			if (userLevel >= requireLevel && currMainStoryId > mainStoryId)
//			{
//				if (num < addNum)
//				{
//					serverOption->SetQuestGuideUIVisibility(true, questData->GetStoryId());
//					++num;
//				}
//			}
//		}
//	}
//
//}

void UGsQuestManager::RemoveActiveQuestKey(QuestId inQuestId, QuestIndex inQuestIndex)
{
	QuestKey removeQuestKey = QuestKey::KeyNone();
	for (QuestKey& questKey : _activeQuestKeyList)
	{
		if (questKey == QuestKey(inQuestId, inQuestIndex))
		{
			removeQuestKey = questKey;
		}
	}

	if (!removeQuestKey.IsKeyNone())
	{
		_activeQuestKeyList.Remove(removeQuestKey);
	}
}

void UGsQuestManager::ClearActiveQuestIdList(QuestType inQuestType)
{
	if (QuestType::MAX == inQuestType)
	{
		_activeQuestKeyList.Reset();
	}
	else
	{
		int num = _activeQuestKeyList.Num();
		for (int i = num - 1; i >= 0; --i)
		{
			if(false == _activeQuestKeyList.IsValidIndex(i))
				continue;

			QuestKey& questKey = _activeQuestKeyList[i];
			if (inQuestType == GetQuestType(questKey._questId))
			{
				_activeQuestKeyList.RemoveAt(i);
			}
		}
	}
}

void UGsQuestManager::RemoveCharacter()
{
	ClearMainQuestTableCacheIndex();
}

void UGsQuestManager::ClearMainQuestTableCacheIndex()
{
	if (nullptr == _mainQuestTableCacheSet)
		return;

	// 메인퀘스트 index 초기화
	_mainQuestTableCacheSet->ClearIndex();
}

TSharedPtr<FGsQuestData> UGsQuestManager::GetQuestData(QuestId inQuestId) const
{
	return  _questDataMap.FindRef(inQuestId);
}

TSharedPtr<FGsQuestData> UGsQuestManager::GetQuestData(QuestKey inQuestKey) const
{
	if (inQuestKey.IsKeyNone())
	{
		return nullptr;
	}

	TSharedPtr<FGsQuestData> questData = _questDataMap.FindRef(inQuestKey._questId);
	if (!questData.IsValid())
	{
		return nullptr;
	}

	return questData;
}

// 퀘스트Id로 캐시해둔 QuestTable을 얻는다.
TSharedPtr<FGsQuestTableCache> UGsQuestManager::GetQuestTableCache(const QuestId& inQuestId) const
{
	TSharedPtr<FGsQuestData> questData = _questDataMap.FindRef(inQuestId);
	if (nullptr == questData)
		return nullptr;

	return questData.Get()->GetQuestTableCache();
}

FGsQuestMainTableCacheSet* UGsQuestManager::GetQuestMainTableCache(QuestId inQuestId) const
{
	if (nullptr == _mainQuestTableCacheSet)
		return nullptr;

	// 메인퀘스트인 경우 퀘스트가 속한 챕터와 스토리를 로드한다.
	_mainQuestTableCacheSet->InitializeTableCache(inQuestId);

	return _mainQuestTableCacheSet.Get();
}

FGsQuestMainTableCacheSet* UGsQuestManager::GetQuestMainTableCache() const
{
	return _mainQuestTableCacheSet.Get();
}

StoryId UGsQuestManager::GetCurrentStoryId()
{
	return _mainQuestTableCacheSet->GetCurrentStoryId();
}

QuestContentsType UGsQuestManager::GetSubQuestContenType(int32 inNpcId) const
{
	TSharedPtr<FGsQuestSubChapterTableCache> npcTableCache = _subQuestTableCacheSet->GetChapterTableCache(inNpcId);
	if (!npcTableCache.IsValid())
		return QuestContentsType::NORMAL;

	const FGsSchemaQuestSubChapter* schemaChapter = npcTableCache->GetSchemaQuestChapter();
	if (nullptr == schemaChapter)
		return QuestContentsType::NORMAL;

	return schemaChapter->questContentsType;
}

TSharedPtr<FGsQuestData> UGsQuestManager::GetSubQuestData(int32 inNpcId, EQuestNpcState& outNpcState)
{	
	TSharedPtr<FGsQuestData> questData = GetSubQuest(inNpcId, outNpcState);
	if (!questData.IsValid())
		return nullptr;

	//if (outNpcState == EQuestNpcState::STATE_SUB_WATING)
		//return questData;

	return questData;
}

bool UGsQuestManager::IsInteractionPossible(int32 inNpcTblId, int64 inGameId)
{
	FGsQuestHandler* questHandler = GetQuestHandler();
	if (nullptr == questHandler)
		return false;

	if (questHandler->IsHasInteractionQuest(inNpcTblId, inGameId))
	{
		return true;
	}

	EQuestNpcState outNpcState;
	TSharedPtr<FGsQuestData> questData = GetSubQuest(inNpcTblId, outNpcState);
	if (questData.IsValid())
	{
		if (outNpcState == EQuestNpcState::STATE_SUB_WATING)
			return true;
	}

	return false;
}

// 퀘스트Id 만으로 퀘스트타입을 얻는다.
const QuestType UGsQuestManager::GetQuestType(const QuestId& inQuestId)
{
	TSharedPtr<FGsQuestTableCache>  questTableCache = GetQuestTableCache(inQuestId);
	if (nullptr != questTableCache)
	{
		const FGsSchemaQuest* schemaQuest = questTableCache->GetSchemaQuest();
		return schemaQuest->type;
	}

	return QuestType::MAIN;
}

// 퀘스트 스키마를 읽어온다.
bool UGsQuestManager::GetSchemaQuestList(OUT TArray<const FGsSchemaQuest*>& outQuestList) const
{
	if (const UGsTable* table = FGsSchemaQuest::GetStaticTable())
	{
		return table->GetAllRows<FGsSchemaQuest>(outQuestList);
	}

	return false;
}

// 다음 퀘스트가 있는가?
bool UGsQuestManager::IsNextQuest(QuestId inQuestId, OUT QuestId& outQuestId)
{
	return  _mainQuestTableCacheSet->GetNextQuestId(inQuestId, outQuestId);
}

bool UGsQuestManager::IsNextQuestSub(StoryId inStoryId, QuestId inQuestId, int32 inNpcId, OUT QuestId& outQuestId)
{
	return _subQuestTableCacheSet->GetNextQuestId(inStoryId, inQuestId, inNpcId, outQuestId);
}

bool UGsQuestManager::IsNextQuestRepeat(StoryId inStoryId, QuestId inQuestId, OUT QuestId& outQuestId)
{
	return _repeatQuestTableCacheSet->GetNextQuestId(inStoryId, inQuestId, outQuestId);
}

bool UGsQuestManager::IsNextStory(QuestId inQuestId, OUT QuestId& outQuestId)
{
	if (false == _mainQuestTableCacheSet->ActiveNextIndex())
		return false;

	return _mainQuestTableCacheSet->GetStartQuestId(outQuestId);
}

// 다음 퀘스트를 습득한다.
void UGsQuestManager::AddNextQuest(QuestId inQuestId)
{
	// 다음 퀘스트 
	FGsQuestHandler* questHandler = GetQuestHandler();
	if (nullptr == questHandler)
		return;

	//GSLOG(Log, TEXT("[UGsQuestManager] AddNextQuest ID: %d"), inQuestId);
	questHandler->AddQuest(inQuestId);	
}

// 다음 퀘스트를 습득한다.
void UGsQuestManager::AddNextQuestSub(StoryId inStoryId, QuestId inQuestId)
{
	// 다음 퀘스트 
	FGsQuestHandler* questHandler = GetQuestHandler();
	if (nullptr == questHandler)
		return;

	//GSLOG(Log, TEXT("[UGsQuestManager] AddNextQuest ID: %d"), inQuestId);
	questHandler->AddQuestSub(inStoryId, inQuestId);
}

void UGsQuestManager::AddNextQuestRepeat(StoryId inStoryId, QuestId inQuestId)
{
	// 다음 퀘스트 
	FGsQuestHandler* questHandler = GetQuestHandler();
	if (nullptr == questHandler)
		return;

	//GSLOG(Log, TEXT("[UGsQuestManager] AddNextQuest ID: %d"), inQuestId);
	questHandler->AddQuestRepeat(inStoryId, inQuestId);
}

void UGsQuestManager::AddNextStoryQuest(QuestId inQuestId)
{
	// 다음 스토리의 첫번째 퀘스트 
	FGsQuestHandler* questHandler = GetQuestHandler();
	if (nullptr == questHandler)
		return;

	questHandler->AddStoryQuest(inQuestId);	
}

QuestId UGsQuestManager::GetFirstQuestId()
{
	QuestId outQuestId;
	if (false == _mainQuestTableCacheSet->GetStartQuestId(outQuestId))
		return 0;

	return outQuestId;
}

QuestId UGsQuestManager::GetQuestSubStoryInFirstQuestId(int32 inNpcId, StoryId inStoryId)
{
	return _subQuestTableCacheSet->GetFirstQuestId(inNpcId, inStoryId);
}

QuestId UGsQuestManager::GetFirstChapterQuestId()
{
	return (_mainQuestTableCacheSet.IsValid() ? _mainQuestTableCacheSet->GetFirstChapterQuestId() : 0);
}

FGsQuestHandler* UGsQuestManager::GetQuestHandler()
{
	if (UGsGameObjectManager* objManager = GSGameObject())
	{
		if (UGsGameObjectBase* local = objManager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();
			if (nullptr != castLocal)
			{
				FGsQuestHandler* questHandler = castLocal->GetQuestHandler();
				if (nullptr != questHandler)
				{					
					return questHandler;
				}
			}
		}
	}

	return nullptr;
}

TSharedPtr<FGsQuestData> UGsQuestManager::GetPreQuestData(const QuestId& inQuestId)
{
	if (nullptr == _mainQuestTableCacheSet)
		return nullptr;

	QuestId preQuestId;
	if (_mainQuestTableCacheSet->GetPreQuestId(inQuestId, preQuestId))
	{
		return GetQuestData(preQuestId);
	}

	return nullptr;
}

bool UGsQuestManager::IsNewChapterOpen(QuestId inQuestId)
{
	TSharedPtr<FGsQuestStoryTableCache> storyTableCache = _mainQuestTableCacheSet->GetQuestStoryTableCache(0);
	if (nullptr != storyTableCache)
	{
		TArray<QuestId> questIdList = storyTableCache->GetQuestIdList();
		if (0 < questIdList.Num())
		{
			if (questIdList[0] == inQuestId)
				return true;
		}
	}

	return false;
}

void UGsQuestManager::ClearQuest()
{
	for (auto& iter : _questDataMap)
	{
		TSharedPtr<FGsQuestData>& ptrQuestData = iter.Value;
		if (nullptr == ptrQuestData)
			continue;

		if (FGsQuestData* questData = ptrQuestData.Get())
		{
			questData->AllClearDynamicData();
		}
	}

	if (_questActionManagement.IsValid())
	{
		_questActionManagement->Clear();
	}
	if (_questTargetManagement.IsValid())
	{
		_questTargetManagement->Clear();
	}
	if (_questGuideManagement.IsValid())
	{
		_questGuideManagement->Clear();
	}

	// 메인퀘스트 index 초기화
	_mainQuestTableCacheSet->ClearIndex();
	// active 리스트 초기화
	_activeQuestKeyList.Empty();
	_activeSubQuestDataMap.Empty();
	_activeGuideQuestDataMap.Empty();
	_autoProgressQuestKey = QuestKey::KeyNone();
	_currMainQuestId = 0;
	_isUseRandomScroll = false;
	_isUseWarpScroll = false;
}

void UGsQuestManager::ClearQuestMain()
{
	for (auto& iter : _questDataMap)
	{
		TSharedPtr<FGsQuestData>& ptrQuestData = iter.Value;
		if (nullptr == ptrQuestData)
			continue;

		FGsQuestData* questData = ptrQuestData.Get();
		if (nullptr == questData)
			continue;

		TSharedPtr<FGsQuestTableCache> questTableCache = questData->GetQuestTableCache();
		if (!questTableCache.IsValid())
			continue;

		if (QuestType::MAIN == questTableCache->GetSchemaQuest()->type)
		{
			questData->ClearDynamicData();
		}
	}

	if (nullptr == _mainQuestTableCacheSet)
		return;

	// 메인퀘스트 index 초기화
	_mainQuestTableCacheSet->ClearIndex();
	// 메인퀘스트 아이디 초기화
	_currMainQuestId = 0;

	// 현재 수행 중인 퀘스트 아이디 초기화
	ClearActiveQuestIdList(QuestType::MAIN);

	FGsQuestHandler* questHandler = GetQuestHandler();
	if (nullptr == questHandler)
		return;

	// 퀘스트 인스턴스 모두 삭제
	questHandler->ClearQuest(QuestType::MAIN);
}

void UGsQuestManager::ClearQuestRepeat(QuestKey inQuestKey)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestKey._questId);
	if (!questData.IsValid())
		return;

	questData->ClearDynamicData(inQuestKey._questIndex);
}

void UGsQuestManager::ResetQuestRepeat(QuestKey inQuestKey)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestKey._questId);
	if (!questData.IsValid())
		return;

	questData->ResetDynamicData(inQuestKey._questIndex);

	if (FGsServerOption* serverOption = GServerOption())
	{
		serverOption->SetRemoveQuestRepeatViewIndexList(0, inQuestKey);
	}
}

void UGsQuestManager::ClearQuestRank(QuestId inQuestId)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	questData->ClearDynamicData();
}

void UGsQuestManager::ClearQuestGuild(QuestId inQuestId)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	questData->ClearDynamicData();
}

void UGsQuestManager::ClearQuestGuide(QuestId inQuestId)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
		return;

	for (auto& iter : _activeGuideQuestDataMap)
	{
		TSharedPtr<FGsQuestData> data = iter.Value;
		if (data->GetQuestId() == inQuestId)
		{
			if (_activeQuestKeyList.Contains(QuestKey(inQuestId)))
				_activeQuestKeyList.Remove(QuestKey(inQuestId));

			break;
		}
	}

	UpdateGuideQuestActiveList();
}

int32 UGsQuestManager::GetQuestCount(QuestType inQuestType)
{
	FGsQuestHandler* questHandler = GetQuestHandler();
	if (nullptr == questHandler)
		return 0;

	return questHandler->GetQuestCount(inQuestType);
}

// outList1 : 게시판 퀘스트 중 완료된 목록
// outList2 : 게시판 퀘스트 중 진행중 목록
// outList3 : 스크롤 퀘스트 목록
void UGsQuestManager::GetRepeatQuestKeyList(OUT TArray<QuestKey>& outList1, OUT TArray<QuestKey>& outList2, OUT TArray<QuestKey>& outList3)
{
	TArray<QuestKey> repeatBoardQuestKeyList;
	TArray<QuestKey> repeatBoardCompletedQuestKeyList;
	TArray<QuestKey> repeatScrollQuestKeyList;

	for (QuestKey& questKey : _activeQuestKeyList)
	{
		TSharedPtr<FGsQuestData> questData = GetQuestData(questKey._questId);
		if (!questData.IsValid())
			continue;

		if (QuestType::REPEAT == questData->GetQuestType())
		{
			RepeatStoryType repeatStoryType = GetRepeatStoryType(questData->GetStoryId());
			if (RepeatStoryType::BOARD == repeatStoryType)
			{
				FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questKey._questIndex);
				if (nullptr == dynamicData)
					continue;

				if (dynamicData->GetQuestState() == QuestState::COMPLETED)
				{
					repeatBoardCompletedQuestKeyList.Add(questKey);
				}
				else
				{
					repeatBoardQuestKeyList.Add(questKey);
				}
			}
			else
			{
				repeatScrollQuestKeyList.Add(questKey);
			}
		}
	}

	repeatBoardQuestKeyList.Sort([this](const QuestKey& A, const QuestKey& B)
		{
			if (A._questId < B._questId)
				return true;
			else if (A._questId == B._questId)
			{
				if (A._questIndex < B._questIndex)
					return true;
			}
			return false;
		});

	repeatScrollQuestKeyList.Sort([this](const QuestKey& A, const QuestKey& B)
		{
			if (A._questId < B._questId)
				return true;
			else if (A._questId == B._questId)
			{
				if (A._questIndex < B._questIndex)
					return true;
			}
			return false;
		});

	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
		return;

	TArray<QuestKey> repeatBoardQuestKeySortList;

	const TArray<QuestStoryIdPair>& questIndexList = serverOption->GetRepeatQuestviewIndexList();
	for (QuestStoryIdPair idPair : questIndexList)
	{
		QuestId questId = idPair.mQuestId;
		QuestIndex questIndex = idPair.mQuestIndex;

		QuestKey* questkey = repeatBoardQuestKeyList.FindByPredicate(
			[questId, questIndex](const QuestKey& key) -> bool
			{
				return key._questIndex == questIndex && key._questId == questId;
			});

		if (nullptr != questkey)
		{
			repeatBoardQuestKeySortList.Add(*questkey);
		}
	}

	int sortNum = repeatBoardQuestKeySortList.Num();
	int num = repeatBoardQuestKeyList.Num();
	if (sortNum != num)
	{
		for (QuestKey questKey : repeatBoardQuestKeyList)
		{
			QuestIndex index = questKey._questIndex;
			bool find = repeatBoardQuestKeySortList.ContainsByPredicate(
				[index](const QuestKey& key) -> bool
				{
					return key._questIndex == index;
				});

			if (false == find)
			{
				repeatBoardQuestKeySortList.Add(questKey);
			}
		}
	}

	outList1 = repeatBoardCompletedQuestKeyList;
	outList2 = repeatBoardQuestKeySortList;
	outList3 = repeatScrollQuestKeyList;
}

void UGsQuestManager::GetRepeatQuestList(OUT TArray<TSharedPtr<FGsQuestData>>& outList)
{
	for (QuestKey& questKey : _activeQuestKeyList)
	{
		TSharedPtr<FGsQuestData> questData = GetQuestData(questKey._questId);
		if (!questData.IsValid())
			continue;

		if (QuestType::REPEAT == questData->GetQuestType())
		{
			RepeatStoryType repeatStoryType = GetRepeatStoryType(questData->GetStoryId());
			if (RepeatStoryType::BOARD == repeatStoryType)
			{
				outList.Add(questData);
			}
		}
	}
}

void UGsQuestManager::GetRepeatQuestSlotStateInfo(IN StoryId inStoryId, OUT QuestWindowUISlotType& outSlotType)
{

}

RepeatStoryType UGsQuestManager::GetRepeatStoryType(StoryId inStoryId)
{
	TSharedPtr<FGsQuestRepeatStoryTableCache> repeatStoryTableCache = GetQuestRepeatStoryTableCache(inStoryId);
	if (!repeatStoryTableCache.IsValid())
		return RepeatStoryType::BOARD;

	const FGsSchemaQuestRepeatStory* schemaQuestStory = repeatStoryTableCache->GetSchemaQuestStory();
	if (nullptr == schemaQuestStory)
		return RepeatStoryType::BOARD;

	return schemaQuestStory->repeatStoryType;
}

RepeatStoryType UGsQuestManager::GetRepeatStoryType(QuestKey inQuestKey)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestKey);
	if (!questData.IsValid())
		return RepeatStoryType::BOARD;

	return GetRepeatStoryType(questData->GetStoryId());
}


const FLinearColor& UGsQuestManager::GetTextColor(QuestType inType, StoryId inStoryId)
{
	switch (inType)
	{
	case QuestType::MAIN:
	{
		return FGsUIColorHelper::GetColor(EGsUIColorType::QUEST_MAIN_TITLE_COLOR);
	}
	break;
	case QuestType::SUB:
	{
		return FGsUIColorHelper::GetColor(EGsUIColorType::QUEST_SUB_TITLE_COLOR);
	}
	break;
	case QuestType::REPEAT:
	{
		TSharedPtr<FGsQuestRepeatStoryTableCache> outStoryTableCache;
		_repeatQuestTableCacheSet->GetRepeatStoryTableCache(inStoryId, outStoryTableCache);

		if (outStoryTableCache.IsValid())
		{
			if (const FGsSchemaQuestRepeatStory* schemaQuestRepeatStory = outStoryTableCache->GetSchemaQuestStory())
			{
				switch (schemaQuestRepeatStory->repeatStoryGrade)
				{
				case RepeatStoryGrade::NORMAL:
					return FGsUIColorHelper::GetColor(EGsUIColorType::ITEM_GRADE_NORMAL);
					break;
				case RepeatStoryGrade::MAGIC:
					return FGsUIColorHelper::GetColor(EGsUIColorType::ITEM_GRADE_MAGIC);
					break;
				case RepeatStoryGrade::RARE:
					return FGsUIColorHelper::GetColor(EGsUIColorType::ITEM_GRADE_RARE);
					break;
				default:
					break;
				}
			}
		}
	}
	break;
	case QuestType::MULTI_LEVEL_RANK:
	case QuestType::GUILD:
	{
		// None
	}
	break;
	case QuestType::GUIDE:
	{
		return FGsUIColorHelper::GetColor(EGsUIColorType::QUEST_GUIDE_TITLE_COLOR);
	}
	break;
	default:
		break;
	}

	return FGsUIColorHelper::GetColor(EGsUIColorType::QUEST_MAIN_TITLE_COLOR);
}

class UPaperSprite* UGsQuestManager::GetRepeatQuestGradeIcon(const QuestStoryIdPair& inPair)
{
	const FGsSchemaQuestConfig* findConfig = nullptr;
	if (const UGsTable* table = FGsSchemaQuestConfig::GetStaticTable())
	{
		table->FindRow(TEXT("0"), findConfig);
	}

	if (nullptr == findConfig)
	{
		return nullptr;
	}

	TSharedPtr<FGsQuestRepeatStoryTableCache> outStoryTableCache;
	_repeatQuestTableCacheSet->GetRepeatStoryTableCache(inPair.mStoryId, outStoryTableCache);

	if (outStoryTableCache.IsValid())
	{
		if (const FGsSchemaQuestRepeatStory* schemaQuestRepeatStory = outStoryTableCache->GetSchemaQuestStory())
		{
			switch (schemaQuestRepeatStory->repeatStoryGrade)
			{
			case RepeatStoryGrade::NORMAL:
				return Cast<UPaperSprite>(UAssetManager::GetStreamableManager().LoadSynchronous(findConfig->repeatStoryGradeIconNormal));
				break;
			case RepeatStoryGrade::MAGIC:
				return Cast<UPaperSprite>(UAssetManager::GetStreamableManager().LoadSynchronous(findConfig->repeatStoryGradeIconMagic));
				break;
			case RepeatStoryGrade::RARE:
				return Cast<UPaperSprite>(UAssetManager::GetStreamableManager().LoadSynchronous(findConfig->repeatStoryGradeIconRare));
				break;
			default:
				break;
			}
		}
	}

	return nullptr;
}

const FLinearColor& UGsQuestManager::GetQuestGradeColor(const QuestStoryIdPair& inPair)
{
	RepeatStoryGrade grade = GSQuest()->GetRepeatStoryGrade(inPair);
	EGsUIColorType colorType = EGsUIColorType::ITEM_GRADE_NORMAL;
	switch (grade)
	{
	case RepeatStoryGrade::NORMAL:
	{
		colorType = EGsUIColorType::ITEM_GRADE_NORMAL;
	}
	break;
	case RepeatStoryGrade::MAGIC:
	{
		colorType = EGsUIColorType::ITEM_GRADE_MAGIC;
	}
	break;
	case RepeatStoryGrade::RARE:
	{
		colorType = EGsUIColorType::ITEM_GRADE_RARE;
	}
	break;
	}

	return FGsUIColorHelper::GetColor(colorType);
}

RepeatStoryGrade UGsQuestManager::GetRepeatStoryGrade(const QuestStoryIdPair& inPair)
{
	TSharedPtr<FGsQuestRepeatStoryTableCache> outStoryTableCache;
	_repeatQuestTableCacheSet->GetRepeatStoryTableCache(inPair.mStoryId, outStoryTableCache);

	if (false == outStoryTableCache.IsValid())
	{
		return RepeatStoryGrade::MAX;
	}

	const FGsSchemaQuestRepeatStory* schemaQuestRepeatStory = outStoryTableCache->GetSchemaQuestStory();
	if (nullptr == schemaQuestRepeatStory)
	{
		return RepeatStoryGrade::MAX;
	}

	return schemaQuestRepeatStory->repeatStoryGrade;
}

const FString UGsQuestManager::GetRepeatQuestTitle(const StoryId& inStoryId)
{
	TSharedPtr<FGsQuestRepeatStoryTableCache> storyTableCache = GSQuest()->GetQuestRepeatStoryTableCache(inStoryId);
	if (false == storyTableCache.IsValid())
	{
		return TEXT("");
	}

	const FGsSchemaQuestRepeatStory* schemaQuestRepeatStory = storyTableCache.Get()->GetSchemaQuestStory();
	if (nullptr == schemaQuestRepeatStory)
	{
		return TEXT("");
	}

	return schemaQuestRepeatStory->storyTitleText.ToString();
}

bool UGsQuestManager::IsCompleted(QuestId inQuestId, QuestIndex inQuestIndex)
{
	if (0 < _activeQuestKeyList.Num())
	{
		for (const QuestKey& questKey : _activeQuestKeyList)
		{
			TSharedPtr<FGsQuestData> questData = GetQuestData(questKey._questId);
			if (!questData.IsValid())
				continue;

			if (QuestType::MAIN != questData->GetQuestType())
				continue;

			if (questKey._questId > inQuestId)
			{
				// 이미 한 퀘스트
				return true;
			}
			else
			{
				// 상태 체크 후 
				const QuestState state = GetQuestHandler()->GetQuestState(QuestKey(inQuestId, inQuestIndex));
				if (QuestState::COMPLETED == state ||
					QuestState::REWARDED == state)
				{
					return true;
				}
			}
		}
	}
	else
	{
		if (inQuestId >= _currMainQuestId)
			return true;
	}

	return false;
}

bool UGsQuestManager::IsStoryCompleted(StoryId inStoryId)
{
	return inStoryId < _mainQuestTableCacheSet->GetCurrentStoryId();
}

bool UGsQuestManager::IsProgress(QuestId inQuestId, QuestIndex inQuestIndex)
{
	return IsProgress(QuestKey(inQuestId, inQuestIndex));
}

bool UGsQuestManager::IsProgress(QuestKey inQuestKey)
{
	return _activeQuestKeyList.Contains(inQuestKey);
}

bool UGsQuestManager::IsProgress(QuestId inStartQuestId, QuestId inEndQuestID)
{
	// 진행중 체크는 end quest id가 0이 아니면
	// 상태랑 상관없이 progress quest id가 
	// inStartQuestId <= progressQuestId < inEndQuestId 조건이면 진행중이라고 본다

	for (const QuestKey& questKey : _activeQuestKeyList)
	{
		if (questKey.IsKeyNone())
			continue;

		QuestId questId = questKey._questId;
		TSharedPtr<FGsQuestData> questData = GetQuestData(questId);
		if (!questData.IsValid())
			continue;

		if (QuestType::MAIN != questData->GetQuestType())
			continue;

		// end가 0이면 기존 방식
		if (inEndQuestID == 0)
		{
			if (questId != inStartQuestId)
				continue;

			FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questKey._questIndex);
			if (nullptr == dynamicData)
				continue;
			
			const QuestState state = dynamicData->GetQuestState();			
			if (QuestState::COMPLETED == state ||
				QuestState::REWARDED == state)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			// 사이 값이면 진행중으로 본다
			if (inStartQuestId <= questId &&
				questId < inEndQuestID)
			{
				return true;
			}
			// 끝의 값이 같으면 끝의 값으로 체크
			else if (questId == inEndQuestID)
			{				
				TSharedPtr<FGsQuestData> endQuestData = GetQuestData(inEndQuestID);
				if (!endQuestData.IsValid())
					continue;

				FGsQuestDynamicData* endDynamicData = endQuestData->GetQuestDynamicData(questKey._questIndex);
				if (nullptr == endDynamicData)
					continue;

				const QuestState state = endDynamicData->GetQuestState();
				if (QuestState::COMPLETED == state ||
					QuestState::REWARDED == state)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool UGsQuestManager::IsQuestAccepted(QuestId inQuestId)
{
	for (const QuestKey& questKey : _activeQuestKeyList)
	{
		if (questKey.IsKeyNone())
			continue;

		TSharedPtr<FGsQuestData> questData = GetQuestData(questKey._questId);
		if (!questData.IsValid())
			continue;
		
		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questKey._questIndex);
		if (nullptr == dynamicData)
			continue;

		const QuestState state = dynamicData->GetQuestState();
		return (QuestState::ACCEPTED == state) ? true : false;		
	}

	return false;
}

// 진행 중인 퀘스트인지 체크(체크 당할 퀘스트 id 직접 입력)
bool UGsQuestManager::IsNextProgress(QuestId In_tableQuestId)
{
	QuestId findNextQuestId = 0;
	if (IsNextQuest(_currMainQuestId, findNextQuestId) == false)
	{
		return false;
	}

	if (findNextQuestId == 0)
	{
		return false;
	}

	return (findNextQuestId == In_tableQuestId) ? true : false;
}

bool UGsQuestManager::IsVisible(const FGsSchemaSpawnElement* inSpawnData
	, bool In_isCheckCompleteNextQuest)
{
	if (inSpawnData == nullptr)
	{
		return false;
	}

	// 조건이 없으면 초기화 visible로 세팅
	if (inSpawnData->QuestCondition.Num() == 0)
	{
		return inSpawnData->InitVisible;
	}
	else
	{
		QuestId startQuestId;
		QuestId endQuestId;
		for (auto& iter : inSpawnData->QuestCondition)
		{
			startQuestId = (iter.StartConditionQuestId.GetRow() ? iter.StartConditionQuestId.GetRow()->id : 0);
			endQuestId = (iter.EndConditionQuestId.GetRow() ? iter.EndConditionQuestId.GetRow()->id : 0);
			// 하나라도 있으면 true
			if (IsVisible(inSpawnData->InitVisible,
				startQuestId, endQuestId,
				iter.QuestCheckType,
				inSpawnData->VisibleType,
				In_isCheckCompleteNextQuest))
			{
				return true;
			}
		}
	}
	return false;
}
// show info array 로 체크
bool UGsQuestManager::IsVisibleShowInfoArray(TArray<FGsSpawnObjectShowHide>& In_arrayShowInfo,
	bool In_isInitVisible, EGsVisibleType In_visibleType)
{
	// 조건이 없으면 초기화 visible로 세팅
	if (In_arrayShowInfo.Num() == 0)
	{
		return In_isInitVisible;
	}
	else
	{
		for (auto& iter : In_arrayShowInfo)
		{
			if (IsVisible(In_isInitVisible,
				iter.StartConditionQuestId,
				iter.EndConditionQuestId,
				iter.QuestCheckType,
				In_visibleType))
			{
				return true;
			}
		}
	}
	return false;
}
bool UGsQuestManager::IsVisible(bool isInitVisible,
	QuestId inStartQuestID, QuestId inEndQuestID,
	EGsQuestCheckType inCheckType, EGsVisibleType inVisibleType,
	bool In_isCheckCompleteNextQuest)
{
	if (inStartQuestID != 0)
	{
		if (inCheckType == EGsQuestCheckType::Complete)
		{
			if (IsCompleted(inStartQuestID))
			{
				return (inVisibleType == EGsVisibleType::Show ? true : false);
			}
		}
		else if (inCheckType == EGsQuestCheckType::Running)
		{
			if (IsProgress(inStartQuestID, inEndQuestID))
			{
				return (inVisibleType == EGsVisibleType::Show ? true : false);
			}
			// 현재 조건은 아니지만
			// 다음조건에 만족하는지 체크
			else if (In_isCheckCompleteNextQuest == true &&
				IsNextProgress(inStartQuestID))
			{
				return (inVisibleType == EGsVisibleType::Show ? true : false);
			}
		}

		return isInitVisible;
	}

	return true;
}

bool UGsQuestManager::IsQuestStateCompleted(QuestType inType, StoryId inStoryId, OUT QuestId& outPogressQuestId, QuestIndex inQuestIndex)
{
	for (QuestKey& questKey : _activeQuestKeyList)
	{
		TSharedPtr<FGsQuestData> questData = GetQuestData(questKey._questId);
		if (!questData.IsValid())
			continue;

		if (inType != questData->GetQuestType())
			continue;

		if (inType == QuestType::MAIN)
		{
			if (inStoryId != GetCurrentStoryId())
				continue;
		}
		else
		{
			if (inStoryId != questData->GetStoryId())
				continue;
		}

		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(inQuestIndex);
		if (nullptr == dynamicData)
			continue;
		
		QuestId nextQuestId;
		outPogressQuestId = questData->GetQuestId();
		if (true == IsNextQuest(questData->GetQuestId(), nextQuestId))
			return false;
		
		return (dynamicData->GetQuestState() == QuestState::COMPLETED)? true : false;	
	}

	return false;
}

bool UGsQuestManager::IsStoryRewarded(QuestType inType, StoryId inStoryId, QuestIndex inQuestIndex)
{
	for (QuestKey& questKey : _activeQuestKeyList)
	{
		TSharedPtr<FGsQuestData> questData = GetQuestData(questKey._questId);
		if (!questData.IsValid())
			continue;

		if (inType != questData->GetQuestType())
			continue;

		if (inType == QuestType::MAIN)
		{
			if (inStoryId != GetCurrentStoryId())
				continue;
		}
		else
		{
			if (inStoryId != questData->GetStoryId())
				continue;
		}

		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(inQuestIndex);
		if (nullptr == dynamicData)
			continue;

		return (dynamicData->GetQuestState() == QuestState::REWARDED) ? true : false;
	}

	return false;
}

bool UGsQuestManager::IsQuestGiveUpPossible(QuestType inType, StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex)
{
	for (QuestKey& questKey : _activeQuestKeyList)
	{
		TSharedPtr<FGsQuestData> questData = GetQuestData(questKey._questId);
		if (!questData.IsValid())
			continue;

		if (inType != questData->GetQuestType())
			continue;

		if (inType == QuestType::MAIN)
		{
			if (inStoryId != GetCurrentStoryId())
				continue;
		}
		else
		{
			if (inStoryId != questData->GetStoryId())
				continue;
		}

		if (inQuestId != questData->GetQuestId())
			continue;

		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(inQuestIndex);
		if (nullptr == dynamicData)
			return false;

		QuestId nextQuestId;
		if (true == IsNextQuest(questData->GetQuestId(), nextQuestId))
			return false;
			
		return (dynamicData->GetQuestState() == QuestState::COMPLETED)? false : true;		
	}

	return false;
}

bool UGsQuestManager::IsTeleportPossible(QuestType inType, StoryId inStoryId)
{
	bool isInvadeWorld = GGameData()->IsInvadeWorld();

	for (QuestKey& questKey : _activeQuestKeyList)
	{
		TSharedPtr<FGsQuestData> questData = GetQuestData(questKey._questId);
		if (!questData.IsValid())
			continue;

		if (inType != questData->GetQuestType())
			continue;

		if (questData->GetStoryId() != inStoryId)
			continue;

		TSharedPtr<FGsQuestTableCache> questTableCache = questData->GetQuestTableCache();
		if (!questTableCache.IsValid())
			continue;

		const FGsSchemaQuest* schemaQuest = questTableCache->GetSchemaQuest();
		if (nullptr == schemaQuest)
			continue;

		if (false == schemaQuest->isTeleportAble)
			return false;	

		if (isInvadeWorld)
		{
			if (schemaQuest->questContentsType != QuestContentsType::INVADE)
				return false;
		}
		else
		{
			if (schemaQuest->questContentsType == QuestContentsType::INVADE)
				return false;
		}

		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questKey._questIndex);
		if (nullptr == dynamicData)
			return false;				
		
		QuestState state = dynamicData->GetQuestState();
		if (state == QuestState::NONE ||
			state == QuestState::PRE_ACCEPT)
		{
			return false;
		}
		else
		{
			return true;
		}		
	}

	return false;
}

bool UGsQuestManager::IsAutoMovePossible(QuestType inType, StoryId inStoryId)
{
	bool isInvadeWorld = GGameData()->IsInvadeWorld();

	for (QuestKey& questKey : _activeQuestKeyList)
	{
		TSharedPtr<FGsQuestData> questData = GetQuestData(questKey._questId);
		if (!questData.IsValid())
			continue;

		if (inType != questData->GetQuestType())
			continue;

		if (questData->GetStoryId() != inStoryId)
			continue;

		TSharedPtr<FGsQuestTableCache> questTableCache = questData->GetQuestTableCache();
		if (!questTableCache.IsValid())
			return false;

		const FGsSchemaQuest* schemaQuest = questTableCache->GetSchemaQuest();
		if (nullptr == schemaQuest)
			return false;

		if (false == schemaQuest->isAutoPlayAble)
			return false;

		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questKey._questIndex);
		if (nullptr == dynamicData)
			return false;

		if (isInvadeWorld)
		{
			if (schemaQuest->questContentsType != QuestContentsType::INVADE)
				return false;
		}
		else
		{
			if (schemaQuest->questContentsType == QuestContentsType::INVADE)
				return false;
		}
					
		QuestState state = dynamicData->GetQuestState();
		if (state == QuestState::NONE ||
			state == QuestState::PRE_ACCEPT)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	return false;
}

// 랭크
TSharedPtr<FGsQuestMultiLevelRankStoryTableCache> UGsQuestManager::GetQuestRankStoryTableCache(StoryId inStoryId) const
{
	TSharedPtr<FGsQuestMultiLevelRankStoryTableCache> outStoryTableCache;
	if (true == _rankQuestTableCacheSet->GetRankStoryTableCache(inStoryId, outStoryTableCache))
		return outStoryTableCache;

	return nullptr;
}

bool UGsQuestManager::GetRankQuestDataList(StoryId inStoryId, OUT TArray<TSharedPtr<FGsQuestData>>& outQuestList)
{
	TSharedPtr<FGsQuestMultiLevelRankStoryTableCache> storyTableCache = GetQuestRankStoryTableCache(inStoryId);
	if (!storyTableCache.IsValid())
		return false;

	if (0 >= storyTableCache->GetQuestIdList().Num())
		return false;

	for (QuestId questId : storyTableCache->GetQuestIdList())
	{
		outQuestList.Add(GetQuestData(questId));
	}

	return true;
}

void UGsQuestManager::SetAutoProgressQuest(const QuestKey inQuestKey)
{
	_autoProgressQuestKey = inQuestKey;

	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestKey);
	if (!questData.IsValid())
		return;

	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(inQuestKey._questIndex);
	if (nullptr == dynamicData)
	{
	}
	else
	{
		if (QuestState::NONE != dynamicData->GetQuestState())
		{
			FGsPrimitiveInt32 param(static_cast<int32>(true));
			GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_ACTIVE, &param);
		}
	}
}

void UGsQuestManager::ClearAutoProgressQuestKey()
{
	FGsPrimitiveInt32 param(static_cast<int32>(false));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_ACTIVE, &param);

	_autoProgressQuestKey = QuestKey::KeyNone();
}

// 길드
TSharedPtr<FGsQuestGuildStoryTableCache> UGsQuestManager::GetQuestGuildStoryTableCache(StoryId inStoryId) const
{
	TSharedPtr<FGsQuestGuildStoryTableCache> outStoryTableCache;
	if (true == _guildQuestTableCacheSet->GetGuildStoryTableCache(inStoryId, outStoryTableCache))
		return outStoryTableCache;

	return nullptr;
}

bool UGsQuestManager::GetGuildQuestDataList(StoryId inStoryId, OUT TSharedPtr<FGsQuestData>& outQuestData)
{
	TSharedPtr<FGsQuestGuildStoryTableCache> storyTableCache = GetQuestGuildStoryTableCache(inStoryId);
	if (!storyTableCache.IsValid())
		return false;

	QuestId questId = storyTableCache->GetQuestId();

	if (0 >= questId)
		return false;

	outQuestData = GetQuestData(questId);

	return true;
}

// 가이드
TSharedPtr<FGsQuestGuideStoryTableCache> UGsQuestManager::GetQuestGuideStoryTableCache(QuestId inQuestId) const
{
	TSharedPtr<FGsQuestGuideStoryTableCache> outStoryTableCache;
	if (true == _guideQuestTableCacheSet->GetQuestStoryTableCache(inQuestId, outStoryTableCache))
		return outStoryTableCache;

	return nullptr;
}

TSharedPtr<FGsQuestGuideStoryTableCache> UGsQuestManager::GetGuideStoryTableCache(StoryId inStoryId) const
{
	TSharedPtr<FGsQuestGuideStoryTableCache> outStoryTableCache;
	if (true == _guideQuestTableCacheSet->GetStoryTableCache(inStoryId, outStoryTableCache))
		return outStoryTableCache;

	return nullptr;
}

bool UGsQuestManager::GetGuideQuestDataList(QuestId inQuestId, OUT TSharedPtr<FGsQuestData>& outQuestData)
{
	TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache = GetQuestGuideStoryTableCache(inQuestId);
	if (!storyTableCache.IsValid())
		return false;

	QuestId questId = storyTableCache->GetQuestId();

	if (0 >= questId)
		return false;

	outQuestData = GetQuestData(questId);

	return true;
}

void UGsQuestManager::GetGuideQuestList(OUT TArray<TSharedPtr<FGsQuestData>>& outList)
{
	for (QuestKey& questKey : _activeQuestKeyList)
	{
		TSharedPtr<FGsQuestData> questData = GetQuestData(questKey._questId);
		if (!questData.IsValid())
			continue;

		if (QuestType::GUIDE == questData->GetQuestType())
		{
			outList.Add(questData);
		}
	}
}

void UGsQuestManager::GetValidGuideDataList(OUT TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& outGuideStoryTableCacheList)
{
	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>> acceptedList;
	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>> readyList;

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	uint16 userLevel = userData->mLevel;

	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& guideStoryTableCacheList = _guideQuestTableCacheSet->GetGuideStoryTableCache();
	for (TSharedPtr<FGsQuestGuideStoryTableCache> guidePtr : guideStoryTableCacheList)
	{
		QuestId questId = guidePtr->GetQuestId();
		TSharedPtr<FGsQuestData> questData = GetQuestData(questId);
		if (!questData.IsValid())
			continue;

		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData();
		if (nullptr == dynamicData)
		{
			dynamicData = questData->ClaimQuestDynamicData();
		}

		if (nullptr != dynamicData)
		{
			QuestState state = dynamicData->GetQuestState();
			if (state != QuestState::REWARDED)
			{				
				if (state == QuestState::ACCEPTED ||
					state == QuestState::COMPLETED)
				{
					// 진행중인 퀘스트
					acceptedList.Emplace(guidePtr);
				}
				else
				{
					const FGsSchemaQuestGuideStory* schemaQuestGuideStory = guidePtr->GetSchemaQuestStory();
					if (nullptr == schemaQuestGuideStory)
						continue;

					int32 requireLevel = schemaQuestGuideStory->requireLevel;

					if (userLevel >= requireLevel)
					{					
						const FGsSchemaQuestMainStory* aSchemaQuestMainStory = schemaQuestGuideStory->requireMainStoryId.GetRow();
						if (aSchemaQuestMainStory == nullptr)
						{
							readyList.Emplace(guidePtr);
						}
						else
						{
							if (GetCurrentStoryId() < static_cast<StoryId>(aSchemaQuestMainStory->id))
							{								
							}
							else
							{
								readyList.Emplace(guidePtr);
							}
						}
					}
				}
			}
		}
	}
	
	outGuideStoryTableCacheList += acceptedList;
	outGuideStoryTableCacheList += readyList;
}

void UGsQuestManager::GetGuideDataList(OUT TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& outGuideStoryTableCacheList)
{
	/*
	1. 수락한 퀘스트 그룹
	2. 수락할 수 있는 퀘스트 그룹
	3. 잠긴 그룹
	4. 완료한 그룹
	*/

	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>> acceptedList;
	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>> readyList;
	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>> lockedList;
	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>> rewardedList;

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	uint16 userLevel = userData->mLevel;
	 
	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& guideStoryTableCacheList = _guideQuestTableCacheSet->GetGuideStoryTableCache();
	for (TSharedPtr<FGsQuestGuideStoryTableCache> guidePtr : guideStoryTableCacheList)
	{
		QuestId questId = guidePtr->GetQuestId();
		StoryId storyId = guidePtr->GetStoryId();
		TSharedPtr<FGsQuestData> questData = GetQuestData(questId);
		if (!questData.IsValid())
			continue;

		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData();
		if (nullptr == dynamicData)
		{
			dynamicData = questData->ClaimQuestDynamicData();
		}

		if (nullptr != dynamicData)
		{
			QuestState state = dynamicData->GetQuestState();
			if (state == QuestState::REWARDED)
			{
				// 완료한 퀘스트
				rewardedList.Emplace(guidePtr);
			}
			else
			{
				if (state == QuestState::ACCEPTED ||
					state == QuestState::COMPLETED)
				{
					// 진행중인 퀘스트
					acceptedList.Emplace(guidePtr);
				}
				else
				{
					const FGsSchemaQuestGuideStory* schemaQuestGuideStory = guidePtr->GetSchemaQuestStory();
					if (nullptr == schemaQuestGuideStory)
						continue;
					
					int32 requireLevel = schemaQuestGuideStory->requireLevel;

					if (userLevel < requireLevel)
					{
						// 요구 레벨 제한에 걸린경우
						lockedList.Emplace(guidePtr);
					}
					else
					{
						const FGsSchemaQuestMainStory* aSchemaQuestMainStory = schemaQuestGuideStory->requireMainStoryId.GetRow();
						if (aSchemaQuestMainStory == nullptr)
						{
							readyList.Emplace(guidePtr);
						}
						else
						{
							if (GetCurrentStoryId() < static_cast<StoryId>(aSchemaQuestMainStory->id))
							{
								// 요구 메인스토리 제한에 걸린경우
								lockedList.Emplace(guidePtr);
							}
							else
							{
								readyList.Emplace(guidePtr);
							}
						}
					}
				}				
			}
		}
	}

	lockedList.Sort([this](
		const TSharedPtr<FGsQuestGuideStoryTableCache>& A, const TSharedPtr<FGsQuestGuideStoryTableCache>& B)
		{
			if (A.IsValid() && B.IsValid())
			{
				QuestId aQuestId = A->GetQuestId();
				QuestId bQuestId = B->GetQuestId();

				TSharedPtr<FGsQuestData> aQuestData = GetQuestData(aQuestId);
				TSharedPtr<FGsQuestData> bQuestData = GetQuestData(bQuestId);
				if (aQuestData.IsValid() && bQuestData.IsValid())
				{
					const FGsSchemaQuestGuideStory* aSchemaQuestGuideStory = A->GetSchemaQuestStory();
					const FGsSchemaQuestGuideStory* bSchemaQuestGuideStory = B->GetSchemaQuestStory();
					if (nullptr != aSchemaQuestGuideStory && nullptr != bSchemaQuestGuideStory)
					{
						return aSchemaQuestGuideStory->requireLevel < bSchemaQuestGuideStory->requireLevel;
					}
				}
			}

			return false;
		});

	outGuideStoryTableCacheList += acceptedList;
	outGuideStoryTableCacheList += readyList;
	outGuideStoryTableCacheList += lockedList;
	outGuideStoryTableCacheList += rewardedList;
}

void UGsQuestManager::GetGuideQuestSlotStateInfo(IN StoryId inStoryId, OUT QuestState& outQuestState, OUT QuestWindowUISlotType& outSlotType)
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	uint16 userLevel = userData->mLevel;

	TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache = GetGuideStoryTableCache(inStoryId);
	if (!storyTableCache.IsValid())
		return;

	const FGsSchemaQuestGuideStory* schemaQuestGuideStory = storyTableCache->GetSchemaQuestStory();
	if (nullptr == schemaQuestGuideStory)
		return;

	QuestId questId = storyTableCache->GetQuestId();
	TSharedPtr<FGsQuestData> questData = GetQuestData(questId);
	if (!questData.IsValid())
		return;

	outSlotType = QuestWindowUISlotType::STORY_WATING;
	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData();
	if (nullptr == dynamicData)
	{
		outSlotType = QuestWindowUISlotType::STORY_WATING;
	}
	else
	{
		outQuestState = dynamicData->GetQuestState();
		if (outQuestState == QuestState::REWARDED)
		{
			// 완료한 퀘스트
			outSlotType = QuestWindowUISlotType::STORY_REWARDED;
		}
		else
		{
			if (outQuestState == QuestState::ACCEPTED)
			{
				// 진행중인 퀘스트
				outSlotType = QuestWindowUISlotType::STORY_PROGRESS;
			}
			else if (outQuestState == QuestState::COMPLETED)
			{
				// 진행중인 퀘스트
				outSlotType = QuestWindowUISlotType::STORY_PROGRESS;
			}
			else
			{
				int32 requireLevel = schemaQuestGuideStory->requireLevel;

				if (userLevel < requireLevel)
				{
					// 요구 레벨 제한에 걸린경우
					outSlotType = QuestWindowUISlotType::STORY_LOCKED_EQUIRE_LEVEL;
				}
				else
				{
					const FGsSchemaQuestMainStory* aSchemaQuestMainStory = schemaQuestGuideStory->requireMainStoryId.GetRow();
					if (aSchemaQuestMainStory != nullptr)
					{
						if (GetCurrentStoryId() <= static_cast<StoryId>(aSchemaQuestMainStory->id))
						{
							// 요구 메인스토리 제한에 걸린경우	
							outSlotType = QuestWindowUISlotType::STORY_LOCKED_MAIN_STORY;
						}
						else
						{
							// 수락 대기중인 상태
							outSlotType = QuestWindowUISlotType::STORY_WATING;
						}
					}
				}
			}
		}
	}
}

int UGsQuestManager::GetGuideActivatedCount()
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return 0;

	uint16 userLevel = userData->mLevel;

	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>> guideStoryTableCacheList;
	GetGuideDataList(guideStoryTableCacheList);

	int count = 0;
	for (TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache : guideStoryTableCacheList)
	{
		if (!storyTableCache.IsValid())
			continue;;

		QuestId questId = storyTableCache->GetQuestId();
		TSharedPtr<FGsQuestData> questData = GetQuestData(questId);
		if (!questData.IsValid())
			continue;

		QuestWindowUISlotType slotType = QuestWindowUISlotType::STORY_WATING;
		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData();
		if (nullptr == dynamicData)
			continue;

		const FGsSchemaQuestGuideStory* schemaQuestGuideStory = storyTableCache->GetSchemaQuestStory();
		if (nullptr == schemaQuestGuideStory)
			continue;

		QuestState questState = dynamicData->GetQuestState();
		if (questState == QuestState::REWARDED)
			continue;

		int32 requireLevel = schemaQuestGuideStory->requireLevel;

		if (userLevel < requireLevel)
		{
			continue;
		}
		else
		{
			const FGsSchemaQuestMainStory* aSchemaQuestMainStory = schemaQuestGuideStory->requireMainStoryId.GetRow();
			if (aSchemaQuestMainStory != nullptr)
			{
				if (GetCurrentStoryId() < static_cast<StoryId>(aSchemaQuestMainStory->id))
				{
					continue;
				}
			}
		}
		++count;
	}

	return count;
}

int32 UGsQuestManager::GetQuestGuideProgressCount()
{
	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& storyList = _guideQuestTableCacheSet->GetGuideStoryTableCache();

	int32 count = 0;
	for (TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache : storyList)
	{
		if (!storyTableCache.IsValid())
			continue;

		QuestId questId = storyTableCache->GetQuestId();

		TSharedPtr<FGsQuestData> questData = GetQuestData(questId);
		if (!questData.IsValid())
			continue;

		if (FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData())
		{
			QuestState state = dynamicData->GetQuestState();
			if (state == QuestState::ACCEPTED ||
				state == QuestState::COMPLETED)
			{
				++count;
			}
		}
	}

	return count;
}

bool UGsQuestManager::IsGuideQuestHudOn()
{
	return GetQuestGuideHudCount() < MAX_QUEST_GUIDE_HUD_VIEW_COUNT;
}

int32 UGsQuestManager::GetQuestGuideHudCount()
{
	FGsQuestGuideManagement* guideManagement = GSQuest()->GetQuestGuideManagement();
	if (nullptr == guideManagement)
		return MAX_QUEST_GUIDE_HUD_VIEW_COUNT;

	TArray<StoryId> guideList = guideManagement->GetGuideVaildList();

	return guideList.Num();
}

bool UGsQuestManager::IsQuestMainStateCompleted()
{
	for (QuestKey& questKey : _activeQuestKeyList)
	{
		TSharedPtr<FGsQuestData> questData = GetQuestData(questKey._questId);
		if (!questData.IsValid())
			continue;	

		if (questData->GetQuestType() != QuestType::MAIN)
			continue;

		if (FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questKey._questIndex))
		{
			QuestId nextQuestId;
			if (true == IsNextQuest(questData->GetQuestId(), nextQuestId))
				return false;

			return dynamicData->GetQuestState() == QuestState::COMPLETED;
		}
	}

	return false;
}

// RedDot

bool UGsQuestManager::CheckRedDot(bool bInUpdate)
{
	if (bInUpdate)
	{
		if (false == IsSubQuestRedDot())
		{
			if (false == IsRepeatQuestRedDot())
			{
				if (false == IsGuideQuestRedDot())
				{
					SetRedDot(false);
					return false;
				}
			}
		}

		SetRedDot(true);
		return true;
	}

	return IsRedDot();
}

bool UGsQuestManager::IsSubQuestRedDot()
{
	for (auto iter : _activeSubQuestDataMap)
	{
		TSharedPtr<FGsQuestData> questData = iter.Value;
		if (questData.IsValid())
		{
			FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData();
			if (nullptr == dynamicData)
				continue;

			FGsQuestSubData* subData = static_cast<FGsQuestSubData*>(questData.Get());
			if (nullptr == subData)
				continue;

			QuestId nextQuestId = 0;
			if (true == IsNextQuestSub(questData->GetStoryId(), questData->GetQuestId(), subData->GetNpcID(), nextQuestId))
				continue;

			if (QuestState::COMPLETED == dynamicData->GetQuestState())
				return true;
		}
	}

	return false;
}

bool UGsQuestManager::IsRepeatQuestRedDot()
{
	TArray<TSharedPtr<FGsQuestData>> repeatQuestList;
	GetRepeatQuestList(repeatQuestList);

	for (TSharedPtr<FGsQuestData> questData : repeatQuestList)
	{
		if (!questData.IsValid())
			continue;

		for (auto& iter : questData->GetQuestDynamicDataList())
		{
			TSharedPtr<FGsQuestDynamicData> dynamicDataPtr = iter.Value;

			FGsQuestDynamicData* dynamicData = dynamicDataPtr.Get();
			if (nullptr == dynamicData)
				continue;

			if (QuestState::COMPLETED == dynamicData->GetQuestState())
			{
				return true;
			}
		}
	}

	return false;
}

bool UGsQuestManager::IsGuideQuestRedDot()
{
	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& storyList = _guideQuestTableCacheSet->GetGuideStoryTableCache();

	int32 count = 0;
	for (TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache : storyList)
	{
		if (!storyTableCache.IsValid())
			continue;

		QuestId questId = storyTableCache->GetQuestId();

		TSharedPtr<FGsQuestData> questData = GetQuestData(questId);
		if (!questData.IsValid())
			continue;

		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData();
		if (nullptr == dynamicData)
			continue;

		if (QuestState::COMPLETED == dynamicData->GetQuestState())
		{
			return true;
		}
	}

	return false;
}

bool UGsQuestManager::GetQuestTeleportData(
	IN const QuestId inQuestId, OUT int32& outTeleportGold, OUT QuestObjectiveId& outObjectiveId)
{
	TSharedPtr<FGsQuestTableCache>  questTableCache = GetQuestTableCache(inQuestId);
	if (!questTableCache.IsValid())
		return false;

	const FGsSchemaQuest* schemaQuest = questTableCache->GetSchemaQuest();

	if (nullptr == schemaQuest)
		return false;

	outTeleportGold = schemaQuest->teleportGold;

	TArray<TSharedPtr<FGsQuestObjectiveTableCache>> objectiveTableCacheList = questTableCache->GetObjectiveTableCache();
	int num = objectiveTableCacheList.Num();
	if (0 >= num)
		return false;

	if (!objectiveTableCacheList[0].IsValid())
		return false;

	outObjectiveId = objectiveTableCacheList[0]->GetObjectiveId();

	return true;
}

void UGsQuestManager::GetRequireStoryText(IN QuestType inQuestType, IN StoryId inStoryId, OUT FText& outText)
{
	if (inQuestType == QuestType::MAIN)
	{
		// [메인 1-4. 수상한 바람] 완료 후 진행 가능
		FText findText;
		FText::FindText(TEXT("QuestUIText"), TEXT("RequirePreMainStory"), findText);

		const TArray<TSharedPtr<FGsQuestChapterTableCache>>& chapterTableCacheList =
			_mainQuestTableCacheSet->GetQuestChapterTableCacheList();
		int chapterIndex = 0;
		for (TSharedPtr<FGsQuestChapterTableCache> chapterTableCache : chapterTableCacheList)
		{
			if (chapterTableCache.IsValid())
			{
				TArray<TSharedPtr<FGsQuestStoryTableCache>>& storyTableCacheList =
					chapterTableCache->GetStoryTableCacheList();
				int storyIndex = 0;
				for (TSharedPtr<FGsQuestStoryTableCache> storyTableCache : storyTableCacheList)
				{
					if (storyTableCache->GetStoryId() == inStoryId)
					{
						outText = FText::Format(findText, chapterIndex + 1, storyIndex + 1,
							storyTableCache->GetSchemaQuestStory()->storyTitleText);
						return;
					}
					++storyIndex;
				}
			}
			++chapterIndex;
		}
	}
	else if (inQuestType == QuestType::SUB)
	{
		// [서브 {로렐라이의강}_{에리스}. {수상한 바람}] 완료 후 진행 가능
		FText findText;
		FText::FindText(TEXT("QuestUIText"), TEXT("RequirePreSubStory"), findText);

		TMap<int32, TArray<TSharedPtr<FGsQuestSubChapterTableCache>>>& mapMap =
			_subQuestTableCacheSet->GetChapterAreaTableCacheList();

		for (auto& iterMap : mapMap)
		{
			TArray<TSharedPtr<FGsQuestSubChapterTableCache>>& npcList = iterMap.Value;
			for (TSharedPtr<FGsQuestSubChapterTableCache> iterNpc : npcList)
			{
				TSharedPtr<FGsQuestSubChapterTableCache>& npcTableCache = iterNpc;
				if (!npcTableCache.IsValid())
					continue;

				const FGsSchemaQuestSubChapter* schemaChapter = npcTableCache->GetSchemaQuestChapter();
				if (nullptr == schemaChapter)
					continue;

				TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& storyTableCacheList = npcTableCache->GetStoryTableCacheList();
				int storyNum = storyTableCacheList.Num();
				if (0 < storyNum)
				{
					for (int i = 0; i < storyNum; ++i)
					{
						TSharedPtr<FGsQuestSubStoryTableCache> storyTableCache = storyTableCacheList[i];
						if (!storyTableCache.IsValid())
							continue;

						if (storyTableCache->GetStoryId() == inStoryId)
						{
							outText = FText::Format(findText, schemaChapter->npcId.GetRow()->nameText, i + 1,
								storyTableCache->GetSchemaQuestStory()->storyTitleText);

							return;
						}
					}
				}
			}
		}
	}
}

void UGsQuestManager::GetTitleStringId(const QuestId inQuestId, OUT FText& outTitleStringId)
{
	TSharedPtr<FGsQuestTableCache>  questTableCache = GetQuestTableCache(inQuestId);
	if (nullptr != questTableCache)
	{
		const FGsSchemaQuest* schemaQuest = questTableCache->GetSchemaQuest();
		outTitleStringId = schemaQuest->titleStringId;
		return;
	}

	outTitleStringId = FText::GetEmpty();
}

void UGsQuestManager::GetSubNpcList(OUT TArray<TSharedPtr<FGsSubQuestNpcMoveData>>& outSubQuestNpcMoveDataList)
{
	int32 mapId = GLevel()->GetCurrentLevelId();

	TMap<int32, TArray<TSharedPtr<FGsQuestSubChapterTableCache>>>& chapterLevelMapList =
		_subQuestTableCacheSet->GetChapterLevelTableCacheList();

	TArray<TSharedPtr<FGsQuestSubChapterTableCache>> SubChapterTableCacheList = chapterLevelMapList.FindRef(mapId);
	for (TSharedPtr<FGsQuestSubChapterTableCache> npcTableCache : SubChapterTableCacheList)
	{
		const FGsSchemaQuestSubChapter* schemaQuestChapter = npcTableCache->GetSchemaQuestChapter();
		if (nullptr == schemaQuestChapter)
			continue;

		const FGsSchemaNpcData* npcData = schemaQuestChapter->npcId.GetRow();
		if (nullptr == npcData)
			continue;

		int32 npcId = npcData->id;
		TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& storyTableCacheList = npcTableCache->GetStoryTableCacheList();
		int storyNum = storyTableCacheList.Num();

		EQuestNpcState outNpcStata = EQuestNpcState::STATE_NONE;
		TSharedPtr<FGsQuestData> outQuestData;
		if (true == IsQuestSubRuning(npcId, outNpcStata, outQuestData))
		{
		}

		if (!outQuestData.IsValid())
			continue;

		QuestId questId = outQuestData->GetQuestId();
		StoryId storyId = outQuestData->GetStoryId();
		QuestObjectiveType objectiveType = QuestObjectiveType::INTERACT_NPC;
		int interactionRange = npcData->interactionRange;

		if (outNpcStata == EQuestNpcState::STATE_SUB_ACCEPT)
		{
			outSubQuestNpcMoveDataList.Emplace(
				MakeShareable(new FGsSubQuestNpcMoveData(outNpcStata, storyId, npcId)));
		}
		else if (outNpcStata == EQuestNpcState::STATE_SUB_WATING)
		{
			outSubQuestNpcMoveDataList.Emplace(
				MakeShareable(new FGsSubQuestNpcMoveData(outNpcStata, storyId, questId, objectiveType, mapId,
					interactionRange, npcId, schemaQuestChapter->npcPos)));
		}
	}
}

TSharedPtr<FGsQuestSubChapterTableCache> UGsQuestManager::GetSubChapterTableCache(int32 inNpcId)
{
	FGsQuestSubTableCacheSet* questSubData = GetQuestSubTableCache();
	if (nullptr == questSubData)
		return nullptr;

	return questSubData->GetChapterTableCache(inNpcId);
}

bool IsDungeonMap(IN int32 inDungeonGroupId, OUT int32& outMapId)
{
	int32 key = GLevel()->GetCurrentLevelId();

	TArray<const FGsSchemaDungeonGroupData*> dungeonGroupDataList;
	if (const UGsTable* table = FGsSchemaDungeonGroupData::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaDungeonGroupData>(dungeonGroupDataList);
	}

	for (const FGsSchemaDungeonGroupData* dungeonGroupData : dungeonGroupDataList)
	{
		if (nullptr == dungeonGroupData)
			continue;

		if (dungeonGroupData->id == inDungeonGroupId)
		{
			for (auto& iterDungeon : dungeonGroupData->dungeonList)
			{
				if (const FGsSchemaDungeonData* schemaDungeonData = iterDungeon.GetRow())
				{
					if (const FGsSchemaMapData* schemaMapData = schemaDungeonData->mapId.GetRow())
					{ 
						outMapId = schemaMapData->id;
						return true;						
					}
				}
			}
		}
	}

	return false;
}

// 챕터 지역에 아이콘 상태 ( 발급가능, 진행중, 진행불가 )
QuestWindowUISubStateType UGsQuestManager::GetSubChapterStateType(IN int32 inMapId)
{
	if (0 >= inMapId)
	{
		GSLOG(Error, TEXT("[QuestSub] inMapId 0 >= inMapId : %d"), inMapId);
		return QuestWindowUISubStateType::STATE_COMPLETED;
	}

	FGsQuestSubTableCacheSet* questSubData = GetQuestSubTableCache();
	if (nullptr == questSubData)
		return QuestWindowUISubStateType::STATE_COMPLETED;

	TMap<int32, TArray<TSharedPtr<FGsQuestSubChapterTableCache>>>& mapMap =
		questSubData->GetChapterAreaTableCacheList();

	QuestWindowUISubStateType outType = QuestWindowUISubStateType::STATE_COMPLETED;
	TArray<TSharedPtr<FGsQuestSubChapterTableCache>>* subChapterTableCacheList = mapMap.Find(inMapId);

	if(nullptr == subChapterTableCacheList)
	{
		GSLOG(Error, TEXT("[QuestSub] nullptr == subChapterTableCacheList -> inMapId : %d"), inMapId);
		return QuestWindowUISubStateType::STATE_COMPLETED;
	}

	for (auto& iterNpc : *subChapterTableCacheList)
	{
		TSharedPtr<FGsQuestSubChapterTableCache>& npcTableCache = iterNpc;
		if (!npcTableCache.IsValid())
			continue;

		const FGsSchemaQuestSubChapter* schemaQuestChapter = npcTableCache->GetSchemaQuestChapter();
		if (nullptr == schemaQuestChapter)
			continue;

		const FGsSchemaNpcData* npcData = schemaQuestChapter->npcId.GetRow();
		if (nullptr == npcData)
			continue;

		QuestWindowUISubStateType type = GetSubNpcStateType(npcData->id);
		if (outType > type)
		{
			outType = type;
		}
	}

	return outType;
}

int UGsQuestManager::GetSubMyMapIndex()
{
	int32 key = GLevel()->GetCurrentLevelId();

	FGsQuestSubTableCacheSet* questSubData = GetQuestSubTableCache();
	if (nullptr == questSubData)
		return 0;

	TMap<int32, TArray<TSharedPtr<FGsQuestSubChapterTableCache>>>& mapMap =
		questSubData->GetChapterAreaTableCacheList();

	int index = 0;
	QuestWindowUISubStateType outType = QuestWindowUISubStateType::STATE_COMPLETED;
	for (auto& iterMap : mapMap)
	{
		const FGsSchemaMapData* mapData =
			UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(iterMap.Key);

		int32 mapId = 0;		

		TArray<TSharedPtr<FGsQuestSubChapterTableCache>>& npcList = iterMap.Value;
		for (auto& iterNpc : npcList)
		{
			TSharedPtr<FGsQuestSubChapterTableCache>& npcTableCache = iterNpc;
			if (!npcTableCache.IsValid())
				continue;					

			const FGsSchemaQuestSubChapter* schemaQuestChapter = npcTableCache->GetSchemaQuestChapter();
			if (nullptr == schemaQuestChapter || nullptr == schemaQuestChapter->npcId.GetRow())
				continue;

			mapId = schemaQuestChapter->npcPos.mapId;
			/*mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(mapId);
			if (mapData->mapType == MapType::PUBLIC_DUNGEON)
			{
				mapId = questSubData->GetDungeonGroupId(mapId);
			}*/

			if (key == mapId)
			{
				return index;
			}
		}
		++index;
	}

	return 0;
}



// npc 아이콘 상태 ( 발급가능, 진행중, 진행불가 )
QuestWindowUISubStateType UGsQuestManager::GetSubNpcStateType(IN int32 inNpcId)
{
	StoryId storyId = 0;
	TSharedPtr<FGsQuestData> outQuestData;
	EQuestNpcState outNpcQuestState;
	bool isValue = IsQuestSubRuning(inNpcId, outNpcQuestState, outQuestData);
			
	if (outNpcQuestState == EQuestNpcState::STATE_SUB_INVALID_ACCEPT ||
		outNpcQuestState == EQuestNpcState::STATE_SUB_INVALID_MAIN_STORY ||
		outNpcQuestState == EQuestNpcState::STATE_SUB_INVALID_SUB_STORY ||
		outNpcQuestState == EQuestNpcState::STATE_SUB_INVALID_REQUIRE_LEVEL)
	{
		return QuestWindowUISubStateType::STATE_LOCK;
	}		
	else if (outNpcQuestState == EQuestNpcState::STATE_SUB_WATING)
	{
		return QuestWindowUISubStateType::STATE_ACCEPTABLE;
	}		
	else if (outNpcQuestState == EQuestNpcState::STATE_PROGRESS ||
		outNpcQuestState == EQuestNpcState::STATE_SUB_ACCEPT ||
		outNpcQuestState == EQuestNpcState::STATE_REWARD_WATING)
	{
		return QuestWindowUISubStateType::STATE_PROGRESSING;
	}

	return QuestWindowUISubStateType::STATE_COMPLETED;
}

QuestWindowUISlotType GetUISlotType(int inCurruntStoryIndex, int inStoryIndex, EQuestNpcState inNpcQuestState)
{
	QuestWindowUISlotType slotType = QuestWindowUISlotType::STORY_LOCKED;

	if (inStoryIndex < inCurruntStoryIndex)
	{
		// 수행 한거
		slotType = QuestWindowUISlotType::STORY_REWARDED;
	}
	else if (inStoryIndex > inCurruntStoryIndex)
	{
		slotType = QuestWindowUISlotType::STORY_LOCKED;
	}
	else
	{
		if (inNpcQuestState == EQuestNpcState::STATE_SUB_WATING)
		{
			slotType = QuestWindowUISlotType::STORY_WATING;
		}
		else if (inNpcQuestState == EQuestNpcState::STATE_SUB_INVALID_ACCEPT)
		{
			slotType = QuestWindowUISlotType::STORY_LOCKED;
		}
		else if (inNpcQuestState == EQuestNpcState::STATE_SUB_ACCEPT)
		{
			slotType = QuestWindowUISlotType::STORY_PROGRESS;
		}
		else if (inNpcQuestState == EQuestNpcState::STATE_NONE)
		{
			slotType = QuestWindowUISlotType::STORY_REWARDED;
		}
		else if (inNpcQuestState == EQuestNpcState::STATE_PROGRESS)
		{
			slotType = QuestWindowUISlotType::STORY_PROGRESS;
		}
		else if (inNpcQuestState == EQuestNpcState::STATE_REWARD_WATING)
		{
			slotType = QuestWindowUISlotType::STORY_COMPLETED;
		}
		else if (inNpcQuestState == EQuestNpcState::STATE_SUB_INVALID_MAIN_STORY)
		{
			slotType = QuestWindowUISlotType::STORY_LOCKED_MAIN_STORY;
		}
		else if (inNpcQuestState == EQuestNpcState::STATE_SUB_INVALID_SUB_STORY)
		{
			slotType = QuestWindowUISlotType::STORY_LOCKED_SUB_STORY;
		}
		else if (inNpcQuestState == EQuestNpcState::STATE_SUB_INVALID_REQUIRE_LEVEL)
		{
			slotType = QuestWindowUISlotType::STORY_LOCKED_EQUIRE_LEVEL;
		}
	}

	return slotType;
}

int UGsQuestManager::GetSubStoryIndex(IN int32 inNpcId, IN StoryId inStoryId)
{
	TSharedPtr<FGsQuestSubChapterTableCache> subChapterTableCache = GetSubChapterTableCache(inNpcId);
	if (!subChapterTableCache.IsValid())
		return 0;

	TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& storyList = subChapterTableCache->GetStoryTableCacheList();
	int index = storyList.IndexOfByPredicate([inStoryId](TSharedPtr<FGsQuestSubStoryTableCache> data)
		{
			if (!data.IsValid())
				return false;

			return data->GetStoryId() == inStoryId;
		});

	return (-1 == index) ? 0 : index;
}

QuestWindowUISubStateType UGsQuestManager::GetSubStoryStateType(IN int32 inNpcId, IN StoryId inStoryId)
{
	StoryId curruntStoryId = 0;
	TSharedPtr<FGsQuestData> outQuestData;
	EQuestNpcState outNpcQuestState;
	bool isValue = IsQuestSubRuning(inNpcId, outNpcQuestState, outQuestData);
	if (true == isValue)
	{
		if (!outQuestData.IsValid())
			return QuestWindowUISubStateType::STATE_LOCK;

		curruntStoryId = outQuestData->GetStoryId();
	}

	int curruntIndex = GetSubStoryIndex(inNpcId, curruntStoryId);
	int index = GetSubStoryIndex(inNpcId, inStoryId);

	// 진행중인게 있나?
	bool outIsSelecting = false;

	QuestWindowUISlotType slotType = GetUISlotType(curruntIndex, index, outNpcQuestState);

	if (slotType == QuestWindowUISlotType::STORY_LOCKED_MAIN_STORY ||
		slotType == QuestWindowUISlotType::STORY_LOCKED_SUB_STORY ||
		slotType == QuestWindowUISlotType::STORY_LOCKED_EQUIRE_LEVEL ||
		slotType == QuestWindowUISlotType::STORY_LOCKED)
	{
		return QuestWindowUISubStateType::STATE_LOCK;
	}
	else if (slotType == QuestWindowUISlotType::STORY_WATING)
	{
		return QuestWindowUISubStateType::STATE_ACCEPTABLE;
	}
	else if (slotType == QuestWindowUISlotType::STORY_PROGRESS)
	{
		return QuestWindowUISubStateType::STATE_PROGRESSING;
	}

	return QuestWindowUISubStateType::STATE_COMPLETED;
}

void UGsQuestManager::GetSubQuestSlotStateInfo(IN int32 inNpcId, IN StoryId inStoryId, OUT QuestWindowUISlotType& outSlotType)
{
	StoryId curruntStoryId = 0;
	TSharedPtr<FGsQuestData> outQuestData;
	EQuestNpcState outNpcQuestState;
	bool isValue = IsQuestSubRuning(inNpcId, outNpcQuestState, outQuestData);
	if (true == isValue)
	{
		if (!outQuestData.IsValid())
			return;

		curruntStoryId = outQuestData->GetStoryId();
	}

	int curruntIndex = GetSubStoryIndex(inNpcId, curruntStoryId);
	int index = GetSubStoryIndex(inNpcId, inStoryId);

	outSlotType = GetUISlotType(curruntIndex, index, outNpcQuestState);
}

FGsQuestRepeatTableCacheSet* UGsQuestManager::GetQuestRepeatTableCache()
{
	return _repeatQuestTableCacheSet.Get();
}

void UGsQuestManager::RecvQuestMainRewardedMessge(const IGsMessageParam* In_data)
{
	// 가이드 퀘스트 업데이트
	UpdateGuideQuestActiveList(In_data);
	// 서브 npc 상태 업데이트
	SetSubNpcTartget();
}

void UGsQuestManager::RecvQuestSubRewardedMessge(const IGsMessageParam* In_data)
{
	// 가이드 퀘스트 업데이트
	UpdateGuideQuestActiveList(In_data);
	// 서브 npc 상태 업데이트
	SetSubNpcTartget();
}

bool UGsQuestManager::InvadeQuest(QuestId inQuestId)
{
	if (0 >= inQuestId)
		return false;

	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (questData.IsValid())
	{
		TSharedPtr<FGsQuestTableCache> questTableCache = questData->GetQuestTableCache();
		if (questTableCache.IsValid())
		{
			if (const FGsSchemaQuest* schemaQuest = questTableCache->GetSchemaQuest())
			{
				return (schemaQuest->questContentsType == QuestContentsType::INVADE) ? true : false;;
			}
		}
	}

	return false;
}

const FString UGsQuestManager::GetQuestObjectText(StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex)
{
	FString allText;
	TSharedPtr<FGsQuestData> questData = GSQuest()->GetQuestData(inQuestId);
	if (false == questData.IsValid())
		return allText;

	//퀘스트오브젝티브 출력	
	TArray<FString> objectiveTextList;
	FText questName;				// name
	int32 questObjectveValue = 0;	// static value
	int32 questDynamicValue = 0;	// dynamic value

	//FText descText;
	FText countText;
	int objectiveCount = 0;
	bool IsShowParam;
	FText findText;
	FString resultString;

	objectiveCount = questData->GetObjectiveNum();
	for (int i = 0; i < objectiveCount; ++i)
	{
		questObjectveValue = 0;	// static value
		questDynamicValue = 0;	// dynamic value

		// 카운트를 보여줘야 할지 말아야 할지..
		IsShowParam = questData->IsShowUIObjectiveCount(i);

		questData->GetObjectiveDestText(i, questName);
		if (IsShowParam)
		{
			questData->GetObjectiveValue(i, questObjectveValue);
			questData->GetObjectiveDynamicValue(i, questDynamicValue);
			questData->GetObjectiveDynamicValue(i, questDynamicValue, inQuestIndex);


			FText::FindText(TEXT("QuestUIText"), TEXT("countQuest"), findText);

			// (5/10)
			countText = FText::Format(findText, questDynamicValue, questObjectveValue);
		}
		else
		{
			countText = FText::FromString(TEXT(""));
		}

		resultString = FString::Format(TEXT("{0} {1}\n"), { *questName.ToString(), *countText.ToString() });
		objectiveTextList.Emplace(resultString);
	}

	for (auto& iter : objectiveTextList)
	{
		allText += iter;
	}

	return allText;
}

int32 UGsQuestManager::GetCurrentAllObjectCount(const QuestId inQuestId)
{
	//퀘스트오브젝티브 출력				
	int32 questDynamicValue = 0;	// dynamic value
	int32 allAuestDynamicValue = 0;

	TSharedPtr<FGsQuestData> questData = GSQuest()->GetQuestData(inQuestId);
	if (false == questData.IsValid())
	{
		return allAuestDynamicValue;
	}

	//FText descText;	
	int objectiveCount = 0;
	bool IsShowParam;
	objectiveCount = questData->GetObjectiveNum();
	for (int i = 0; i < objectiveCount; ++i)
	{
		questDynamicValue = 0;	// dynamic value

		// 카운트를 보여줘야 할지 말아야 할지..
		IsShowParam = questData->IsShowUIObjectiveCount(i);
		if (IsShowParam)
		{
			questData->GetObjectiveDynamicValue(i, questDynamicValue);
			allAuestDynamicValue += questDynamicValue;
		}
	}

	return allAuestDynamicValue;
}

void UGsQuestManager::CheatGotoQuestSubNpc(QuestId inQuestId)
{
	TSharedPtr<FGsQuestData> questData = GetQuestData(inQuestId);
	if (!questData.IsValid())
	{
		FText textMsg = FText::FromString(TEXT("[TEST_Cheat] 존재하지 않는 퀘스트아이디 입니다."));
		FGsUIHelper::TrayMessageTicker(textMsg);

		return;
	}

	if (QuestType::SUB != questData->GetQuestType())
	{
		FText textMsg = FText::FromString(TEXT("[TEST_Cheat] 서브퀘스트에서만 사용가능한 치트입니다."));
		FGsUIHelper::TrayMessageTicker(textMsg);

		return;
	}

	TSharedPtr<FGsQuestSubStoryTableCache> storyTableCache;
	int32 npcId;
	if (_subQuestTableCacheSet->GetStoryTableCache(inQuestId, storyTableCache, npcId))
	{
	}
	else
	{
		FText textMsg = FText::FromString(TEXT("[TEST_Cheat] 스토리에 포함이 안된 서브퀘스트입니다."));
		FGsUIHelper::TrayMessageTicker(textMsg);

		return;
	}

	StoryId storyId = questData->GetStoryId();
	FGsQuestTeleportParam param(inQuestId, storyId, npcId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_NPC_TO_TELEPORT, &param);
}

// 외부에서 서브 다이얼로그 닫기
void UGsQuestManager::OnSubDialogClose()
{
	FGsQuestActionManagement* questActionManagement = GetQuestActionManagement();
	if (nullptr == questActionManagement)
		return;

	questActionManagement->OnCloseSubDialog();
}

// 외부에서 npc 다이얼로그 닫기
void UGsQuestManager::OnNpcDialogClose()
{
	FGsQuestActionManagement* questActionManagement = GetQuestActionManagement();
	if (nullptr == questActionManagement)
		return;

	questActionManagement->OnCloseNpcDialog();
}

bool UGsQuestManager::IsReadyFirstQuest()
{
	if (false == _mainQuestTableCacheSet.IsValid())
		return false;

	QuestId firstQuestId = GetFirstChapterQuestId();
	if (0 >= firstQuestId)
		return false;

	if (firstQuestId != _currMainQuestId)
		return false;

	TSharedPtr<FGsQuestData> questData = GetQuestData(firstQuestId);
	if (!questData.IsValid())
		return false;

	FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData();
	if (nullptr == dynamicData)
		return false;

	if (FGsQuestMainData* mainData = static_cast<FGsQuestMainData*>(questData.Get()))
	{
		if (0 < mainData->GetInitEventActionGroupId())
		{
			if (QuestState::NONE == dynamicData->GetQuestState())
			{
				return true;
			}
		}
	}

	return false;
}

bool UGsQuestManager::GetAutoProgressObjectiveType(OUT QuestObjectiveType& outObjectiveType)
{
	if (false == IsAutoProgressing())
		return false;

	QuestKey autoProgressQuestKey = GetAutoProgressQuestKey();
	if (autoProgressQuestKey.IsKeyNone())
		return false;

	return GetQuestHandler()->GetQuestObjectiveType(autoProgressQuestKey, outObjectiveType);
}

void UGsQuestManager::GetQuestTypeText(QuestType inQuestType, OUT FText& outText)
{
	switch (inQuestType)
	{
	case QuestType::MULTI_LEVEL_RANK:
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Title"), outText);
		break;
	default:
		break;
	}
}

bool UGsQuestManager::GetIsChapterLockedInIndex(int32 inChapterIndex)
{
	int32 myChapterIndex = 0;

	const FGsQuestMainTableCacheSet* questMainData = GetQuestMainTableCache();
	if (nullptr != questMainData)
	{
		myChapterIndex = questMainData->GetCurrentQuestChapterIndex();
	}

	return myChapterIndex < inChapterIndex;
}

bool UGsQuestManager::GetIsMyChapterInIndex(int32 inChapterIndex)
{
	int32 myChapterIndex = 0;

	const FGsQuestMainTableCacheSet* questMainData = GetQuestMainTableCache();
	if (nullptr != questMainData)
	{
		myChapterIndex = questMainData->GetCurrentQuestChapterIndex();
	}

	return myChapterIndex == inChapterIndex;
}

bool UGsQuestManager::IsQuestRefreshPossible(QuestId inQuestId, QuestIndex inQuestIndex)
{
	for (QuestKey& questKey : _activeQuestKeyList)
	{
		TSharedPtr<FGsQuestData> questData = GetQuestData(questKey._questId);
		if (!questData.IsValid())
			continue;

		if (questData->GetQuestType() != QuestType::REPEAT)
			continue;

		if (questKey._questId != inQuestId)
			continue;

		if (questKey._questIndex != inQuestIndex)
			continue;

		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(inQuestIndex);
		if (nullptr == dynamicData)
			return false;

		QuestId nextQuestId;
		if (true == IsNextQuest(questData->GetQuestId(), nextQuestId))
			return false;

		return (dynamicData->GetQuestState() == QuestState::COMPLETED) ? false : true;
	}

	return false;
}

bool UGsQuestManager::IsCurrentMainExposeQuest() const
{
	return _mainQuestTableCacheSet->IsCurrentExposeQuest();
}

bool UGsQuestManager::IsSubExposeQuest(IN int32 inNpcId, IN StoryId inStoryId)
{
	TSharedPtr<FGsQuestSubChapterTableCache> subChapterTableCache = GetSubChapterTableCache(inNpcId);
	if (!subChapterTableCache.IsValid())
		return false;

	if (false == subChapterTableCache->IsExposeQuest())
		return false;

	TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& storyList = subChapterTableCache->GetStoryTableCacheList();
	TSharedPtr<FGsQuestSubStoryTableCache> *storyTableCache = storyList.FindByPredicate([inStoryId](TSharedPtr<FGsQuestSubStoryTableCache> data)
		{
			if (!data.IsValid())
				return false;

			return data->GetStoryId() == inStoryId;
		});

	TSharedPtr<FGsQuestSubStoryTableCache> table = *storyTableCache;
	if (!table.IsValid())
		return false;

	return table->IsExposeQuest();
}