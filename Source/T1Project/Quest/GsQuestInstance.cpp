// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestInstance.h"

#include "Engine/World.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Net/GsNetSendServiceWorld.h"

#include "GameObject/Prop/GsSchemaPropData.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Npc/GsSchemaNpcData.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionMoveType.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/State/GsGameObjectStateManager.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"

#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/GsMessageContentHud.h"

#include "AI/Quest/GsAIQuestHandler.h"

#include "Map/GsSchemaMapData.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "DataSchema/Quest/GsSchemaObjectiveKillCreatureTypeSet.h"
#include "DataSchema/Quest/GsSchemaObjectiveCraftSet.h"
#include "DataSchema/Quest/GsSchemaObjectiveCompleteQuestTypeSet.h"

#include "Input/TouchMove/FGsAutoMoveBlockList.h"

#include "GsQuestData.h"
#include "Quest/Management/GsQuestTargetManagement.h"
#include "Quest/GsSchemaObjectivePropSet.h"
#include "Quest/GsSchemaQuestObjective.h"
#include "Quest/GsSchemaObjectiveGotoPosSet.h"
#include "Quest/GsSchemaQuest.h"
#include "Quest/GsSchemaObjectiveWarpToInstanceFieldSet.h"
#include "Quest/GsSchemaObjectiveInteractNpcSet.h"
#include "Quest/Table/GsQuestTableCache.h"
#include "Quest/GsSchemaObjectiveKillSet.h"
#include "Quest/Table/GsQuestObjectiveTableCache.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Sequence/GsSequencePlayer.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsCampBase.h"
#include "Components/CapsuleComponent.h"


FGsQuestInstance::~FGsQuestInstance()
{
	Finalize();
}

void FGsQuestInstance::Initialize(UGsGameObjectLocalPlayer* inOwner, StoryId inStoryId, const QuestKey& inQuestKey)
{
	if (0 >= _objectiveTypeFuncs.Num())
	{
		_objectiveTypeFuncs.Emplace(QuestObjectiveType::KILL,
			[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectiveKillData(inObjectiveTableCache, inObjectiveType); });
		_objectiveTypeFuncs.Emplace(QuestObjectiveType::GOTO_POS,
			[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectiveGotoPosData(inObjectiveTableCache, inObjectiveType); });
		_objectiveTypeFuncs.Emplace(QuestObjectiveType::INTERACT_NPC,
			[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectiveInteractNpcData(inObjectiveTableCache, inObjectiveType); });
		_objectiveTypeFuncs.Emplace(QuestObjectiveType::INTERACT_PROP_TOUCH,
			[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectivePropData(inObjectiveTableCache, inObjectiveType); });
		_objectiveTypeFuncs.Emplace(QuestObjectiveType::WARP_TO_INSTANCE_FIELD,
			[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectiveWarpToInstanceFieldData(inObjectiveTableCache, inObjectiveType); });
		_objectiveTypeFuncs.Emplace(QuestObjectiveType::WARP_TO_LAST_PUBLIC_FIELD,
			[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectiveWarpToLastPublicFieldData(inObjectiveTableCache, inObjectiveType); });

		_objectiveTypeFuncs.Emplace(QuestObjectiveType::KILL_CREATURE_TYPE,
			[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectiveKillCreatureTypeData(inObjectiveTableCache, inObjectiveType); });
		_objectiveTypeFuncs.Emplace(QuestObjectiveType::ITEM_CRAFT,
			[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectiveCraftData(inObjectiveTableCache, inObjectiveType); });
		_objectiveTypeFuncs.Emplace(QuestObjectiveType::COMPLETE_QUEST_TYPE,
			[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectiveCompleteQuestTypeData(inObjectiveTableCache, inObjectiveType); });
		_objectiveTypeFuncs.Emplace(QuestObjectiveType::COMPLETE_STORY_TYPE,
			[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectiveStoryQuestTypeData(inObjectiveTableCache, inObjectiveType); });
		_objectiveTypeFuncs.Emplace(QuestObjectiveType::KILL_SPACE_CRACK,
			[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectiveKillSpaceCrackData(inObjectiveTableCache, inObjectiveType); });
		_objectiveTypeFuncs.Emplace(QuestObjectiveType::TELEPORT,
			[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectiveTypeTeleportData(inObjectiveTableCache, inObjectiveType); });


		int i = static_cast<int>(QuestObjectiveType::BUY_SHOP_ITEM);
		int max = static_cast<int>(QuestObjectiveType::MAX);
		for (; i < max; ++i)
		{
			_objectiveTypeFuncs.Emplace(static_cast<QuestObjectiveType>(i),
				[this](TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType) { return SetObjectiveCommonData(inObjectiveTableCache, inObjectiveType); });
		}
	}

	// 데이터 셋팅 전에 초기화
	Clear();

	_questKey = inQuestKey;
	// 퀘스트 Id
	_questId = _questKey._questId;
	// 퀘스트의 owner
	_owner = inOwner;
	// 퀘스트 데이터

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	_questData = questManager->GetQuestData(_questId);
	if (!_questData.IsValid())
		return;

	_questTableCache = _questData->GetQuestTableCache();
	if (nullptr == _questTableCache)
		return;

	_objectiveTableCacheList = _questTableCache->GetObjectiveTableCache();
	_schemaQuest = _questTableCache->GetSchemaQuest();

	// Quest Objective 데이터 셋팅
	SetQuestObjective();
	// Quest Objective 상태 셋팅
	SetQuestObjectiveState();
	StartTimer();

	_drawQuestDistanceLimit = GData()->GetGlobalData()->_drawQuestDistanceLimit;
}

void FGsQuestInstance::Finalize()
{
	Clear();

	_objectiveTypeFuncs.Reset();
}

// 앞선 퀘스트데이터를 초기화 한다.
void FGsQuestInstance::Clear()
{
	// 타겟 표시도 초기화
	UpdateTarget(false);

	// 타이머 삭제
	StopTimer();

	FGsQuestActionManagement* actionManagement = GSQuest()->GetQuestActionManagement();
	if (nullptr != actionManagement)
	{
		actionManagement->ClearQuestAction(_questId);
	}

	_questData = nullptr;
	_questTableCache = nullptr;
	_schemaQuest = nullptr;
	_remainDistance = 0.0f;
	_nextQuestId = 0;
	_isTeleportAble = true;
	_isAutoMoveAble = true;
	_questUIState = EGsQuestUIState::QUEST_STATE_NONE;
	_questUIEffectType = EGsQuestUIEffectType::QUEST_UI_EFFECT_NONE;

	for (size_t i = 0; i < _objectiveDataList.Num(); i++)
	{
		_objectiveDataList[i].Clear();
	}
	_objectiveDataList.Empty();
}

void FGsQuestInstance::Accept()
{
	PlayAcceptEffect();
	SetQuestState(QuestState::ACCEPTED);
}

// 퀘스트정보를 업데이트한다. 퀘스트 상태(완료, 진행...) 오브젝티브 현재 갱신 숫자 등
void FGsQuestInstance::Update(QuestState inState)
{
	//플레이어가 죽으면 퀘스트 업데이트 할 필요가 없다, 곧 부활 창이 뜰 것이기 때문에
	//죽은 후에도 퀘스트가 업데이트 되면 이벤트 액션이 시작되서 어색하게 나오는 현상이 있다.
	if (UGsGameObjectManager* gameObjectManager = GSGameObject())
	{
		UGsGameObjectBase* local = gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer);
		if (nullptr == local)
		{
			return;
		}

		if (UGsGameObjectLocalPlayer* castLocal = Cast<UGsGameObjectLocalPlayer>(local))
		{
			if (castLocal->IsDeadState())
			{				
				return;
			}
		}
	}	

	SetQuestState(inState);

	UGsQuestManager* questManager = GSQuest();
	if (nullptr != questManager)
	{
		QuestKey activeQuestKey = QuestKey::KeyNone();
		QuestKey warpQuestKey = questManager->GetWarpQuestKey();
		if (!warpQuestKey.IsKeyNone())
		{
			if (warpQuestKey == _questKey)
			{
				activeQuestKey = warpQuestKey;
				questManager->SetWarpQuestId(QuestKey::KeyNone());
			}
		}
		else
		{
			QuestKey autoProgressQuestKey = questManager->GetAutoProgressQuestKey();
			if (!autoProgressQuestKey.IsKeyNone())
			{
				if (autoProgressQuestKey == _questKey)
				{
					activeQuestKey = autoProgressQuestKey;					
				}
			}
		}

		if (!activeQuestKey.IsKeyNone())
		{
#if WITH_EDITOR
			GSLOG(Log, TEXT("[Quest]FGsQuestInstance::Update activeQuestId : %llu, activeQuestIndex : %llu"), activeQuestKey._questId, activeQuestKey._questIndex);
#endif
			bool isUseRandomScroll = questManager->IsUseRandomScroll();
			if (questManager->IsSpawnMeCompleteStartProgress() || isUseRandomScroll)
			{
				StartAutoProgress(isUseRandomScroll);
				questManager->SetIsSpawnMeCompleteStartProgress(false);
				questManager->SetIsUseRandomScroll(false);
			}				
			else
			{
				if (0 >= _activeObjectiveId)
				{
					questManager->ClearAutoProgressQuestKey();
					StopAutoProgress();

					// AI 멈춤 전달
					// 요거 빼면 생기는 문제
					// 1. 퀘스트 이동(자동진행)
					// 2. 지형 pick 이동
					// 3. 지형 이동이 시작되고 다음 tick에서 aiquest에서 퀘스트 진행 상태 보고 멈춰버리면 퀘스트 이동이 아니라 
					// 다음에 들어온 지형 pic 이동을 멈추게함
					GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_MOVE_STOP, nullptr);

					// ui 갱신
					GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
				}				
			}
		}
	}
}

// 퀘스트 상태에 따라 퀘스트 Target 셋팅
// 타겟의 위치 정보로 미니맵, 대상과의 거리 등의 체크를 하게된다.
void FGsQuestInstance::SetQuestState(QuestState inState)
{
	// 상태 저장
	_state = inState;

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	if (inState == QuestState::COMPLETED)
	{
		ClosePopupTeleport();
		// 같은 조건으로 해당 퀘스트 Teleport 막기
		_isTeleportAble = false;
		UpdateTarget(false);
		
		if (questManager->IsAutoProgressQuest(_questKey))
		{
			questManager->ClearAutoProgressQuestKey();
		}		

		StartQuestEvent(QuestEventType::EVENT_QUEST_CLEAR);

		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::QUEST, false);
	}

	switch (_questUIState)
	{
	case EGsQuestUIState::QUEST_STATE_READY:
	case EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED:
		break;
	case EGsQuestUIState::QUEST_STATE_PROGRESS:
	{
		if (inState != QuestState::COMPLETED)
		{
			UpdateTarget(true);
			UpdateAutoProgress();
		}
		else
		{
			UpdateTarget(false);
		}
	}
	break;
	case EGsQuestUIState::QUEST_STATE_REWARD:
	{
		UpdateTarget(false);

		if (questManager->IsAutoProgressQuest(_questKey))
		{
			questManager->ClearAutoProgressQuestKey();
		}		
	}
	break;
	default:
		break;
	}
}

// 미니맵의 정보를 업데이트 하거나 자동이동 거리 표시를 추가 하게 된다.
void FGsQuestInstance::UpdateTarget(bool inIsAdd)
{
	if (nullptr == _owner)
		return;

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	if (FGsQuestTargetManagement* targetManagement = questManager->GetQuestTargetManagement())
	{
		if (true == inIsAdd)
		{
			int8 num = _objectiveDataList.Num();
			for (int8 i = 0; i < num; i++)
			{
				FGsObjectiveData& objectiveData = _objectiveDataList[i];
				objectiveData._isComplete = IsObjectiveCompleted(i);
				objectiveData._isReady = IsObjectiveReady(i);
			}

			if (GGameData()->IsInvadeWorld())
			{
				if (questManager->InvadeQuest(_questId))
				{
					targetManagement->OnAddTarget(_questKey, _objectiveDataList);
				}
			}
			else
			{
				if (!questManager->InvadeQuest(_questId))
				{
					targetManagement->OnAddTarget(_questKey, _objectiveDataList);
				}
			}
		}
		else
		{
			targetManagement->OnRemoveTarget(_questKey);
		}
	}
}

// 자동진행 업데이트
void FGsQuestInstance::UpdateAutoProgress()
{
	int index = 0;
	if (0 < _activeObjectiveId)
	{
		QuestObjectiveId id = _activeObjectiveId;
		index = _objectiveDataList.IndexOfByPredicate([id](const FGsObjectiveData& data)
			{
				return data._objectiveId == id;
			});

		if (-1 == index)
			return;

		int num = _objectiveDataList.Num();
		if (index >= num)
			return;

		if (false == IsObjectiveCompleted(index))
		{
			if (false == _objectiveDataList[index]._isCheckGoal)
			{
				StartAutoProgress();
				return;
			}
		}
		else
		{
			int nextIndex = index + 1;
			if (nextIndex >= num)
				return;

			for (int i = nextIndex; i < num; ++i)
			{
				if (true == IsObjectiveCompleted(i))
					continue;

				StartAutoProgress();
				return;
			}
		}
	}
}

// 퀘스트에 부여된 오브젝티브를 셋팅한다.
void FGsQuestInstance::SetQuestObjective()
{
	for (int i = 0; i < _objectiveTableCacheList.Num(); ++i)
	{
		TSharedPtr<FGsQuestObjectiveTableCache> ObjectiveTableCache = _objectiveTableCacheList[i];
		if (nullptr == ObjectiveTableCache)
			continue;

		const QuestObjectiveType type = ObjectiveTableCache->GetObjectiveType();
		if (0 < _objectiveTypeFuncs.Num() && _objectiveTypeFuncs.Contains(type))
		{
			TFunction<void(TSharedPtr<FGsQuestObjectiveTableCache>, QuestObjectiveType)> func = _objectiveTypeFuncs.FindRef(type);
			if (nullptr != func)
			{
				func(ObjectiveTableCache, type);
			}
		}
	}
}

void FGsQuestInstance::SetQuestObjectiveState()
{
	int8 num = _objectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		FGsObjectiveData& objectiveData = _objectiveDataList[i];
		objectiveData._isComplete = IsObjectiveCompleted(i);
		objectiveData._isReady = IsObjectiveReady(i);
	}
}

// 외부에서 자동진행이 불렸다.
bool FGsQuestInstance::StartAutoProgress(bool inIsAuto)
{
	if (false == _schemaQuest->isAutoPlayAble ||
		false == _isAutoMoveAble)
	{
		FText text;
		FText::FindText(TEXT("UICommonText"), TEXT("Quest_AutoMove_Block"), text);
		FGsUIHelper::TrayMessageTicker(text);

		return false;
	}

	if (false == FGsAutoMoveBlockList::IsAutoMovePossibility())
	{
		FText text;
		FText::FindText(TEXT("QuestUIText"), TEXT("Ticker_unableQuest"), text);
		FGsUIHelper::TrayMessageTicker(text);

		return false;
	}

	// 자동요청
	return RequestAutoMove(inIsAuto);
}

// 자동이동 요청
bool FGsQuestInstance::RequestAutoMove(bool inIsAuto)
{
	if (EGsQuestUIState::QUEST_STATE_PROGRESS != _questUIState)
		return false;

	if (!_questData.IsValid())
		return false;

	FGsAIQuestAutoMoveMessageParam param;

	bool isImmediateToUpdate = false;
	int8 num = _objectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		if (true == IsObjectiveCompleted(i))
			continue;		

		FGsObjectiveData& objectiveData = _objectiveDataList[i];

		if (objectiveData._type == QuestObjectiveType::TELEPORT)
		{
			// 워프는 서버로 바로 요청한다.
			_activeObjectiveId = objectiveData._objectiveId;
			isImmediateToUpdate = true;
			break;
		}

		param._questId = _questId;
		param._moveType = objectiveData._type;

		int8 goalindex = (true == inIsAuto)? objectiveData.GetGoalIndex() : objectiveData._currGoalIndex;
		if (!objectiveData._goalDataList.IsValidIndex(goalindex))
			continue;

		param._mapId = objectiveData._goalDataList[goalindex]._goalMapId;
		param._range = objectiveData._goalDataList[goalindex]._goalRadius;
		objectiveData._goalDataList[goalindex]._isDrawPosition = true;

		switch (objectiveData._type)
		{
		case QuestObjectiveType::KILL:
		case QuestObjectiveType::KILL_CREATURE_TYPE:
		{
			// 도착 위치			
			param._arrPos.Emplace(objectiveData._goalDataList[goalindex]._frontPos);
		}
		break;
		case QuestObjectiveType::INTERACT_NPC:
		{
			// 도착 위치랑
			param._arrPos.Emplace(objectiveData._goalDataList[goalindex]._frontPos);
			// npcId
			param._npcId = objectiveData._tableIdList[0];
		}
		break;
		case QuestObjectiveType::GOTO_POS:
		{
			if (objectiveData._goalDataList[goalindex]._frontPos.Equals(objectiveData._goalDataList[goalindex]._goalPos))
			{
				// 최종 위치
				param._arrPos.Emplace(objectiveData._goalDataList[goalindex]._goalPos);
			}
			else
			{
				// 도착 위치랑
				param._arrPos.Emplace(objectiveData._goalDataList[goalindex]._frontPos);
				// 최종 위치
				param._arrPos.Emplace(objectiveData._goalDataList[goalindex]._goalPos);
			}
		}
		break;
		case QuestObjectiveType::INTERACT_PROP_TOUCH:
		{
			// 도착 위치랑
			param._arrPos.Emplace(objectiveData._goalDataList[goalindex]._frontPos);
			// npcId
			param._npcId = objectiveData._tableIdList[0];
		}
		break;
		default:
			break;
		}

		// 수행조건이 충족하지 않은 순으로 이동
		// 자동이동 objectiveId 저장
		_activeObjectiveId = objectiveData._objectiveId;
		break;
	}

	if (nullptr == _owner)
		return false;

	FGsTargetHandlerBase* targetHandler = _owner->GetTargetHandler();
	if (nullptr == targetHandler)
		return false;

	// 타겟 클리어후 
	// 이동해야함
	targetHandler->ClearTarget(true);

	if (isImmediateToUpdate)
	{
		SendQuestUpdate(_activeObjectiveId, 0, 0);
	}
	else
	{
		GSQuest()->SetAutoProgressQuest(_questKey);

#if WITH_EDITOR
		GSLOG(Log, TEXT("[Quest] FGsQuestInstance::StartAutoProgress QuestId : %llu"), _questId);
#endif

		// AI에게 자동진행 지시
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_MOVE_START, &param);
	}

	return true;
}

// AI에게 자동진행 중지를 받았다.
void FGsQuestInstance::StopAutoProgress()
{
	_activeObjectiveId = 0;
}

// AI에게 자동진행 도착을 받았다.
void FGsQuestInstance::ArrivedAutoProgress(const IGsMessageParam* In_data)
{
	if (EGsQuestUIState::QUEST_STATE_PROGRESS != _questUIState)
		return;

	const FGsAIQuestMessageParam* autoMoveParam = In_data->Cast<const FGsAIQuestMessageParam>();
	SpawnId spawnId = autoMoveParam->_spawnId;

	bool isNextAutoMoveStart = false;
	int8 num = _objectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		FGsObjectiveData& objectiveData = _objectiveDataList[i];
		if (objectiveData._objectiveId != _activeObjectiveId)
			continue;

		if (true == IsObjectiveCompleted(i))
			continue;

		if (false == objectiveData._isCheckGoal)
		{
			int currGoalIndex = objectiveData._currGoalIndex;
			if (!objectiveData._goalDataList.IsValidIndex(currGoalIndex))
				continue;

			if (!objectiveData._goalDataList[currGoalIndex]._warpPos.IsZero())
			{
				objectiveData._goalDataList[currGoalIndex]._warpPos = FVector::ZeroVector;
				return;
			}

			if (objectiveData._type == QuestObjectiveType::GOTO_POS)
			{
				int32 key = GLevel()->GetCurrentLevelId();

				if (objectiveData._goalDataList[currGoalIndex]._goalMapId == key)
				{
					SendQuestUpdate(_activeObjectiveId, spawnId, 0);
					objectiveData._isCheckGoal = true;
				}
			}
			else
			{
				objectiveData._isCheckGoal = true;
			}
		}

		return;
	}
}

// 오토진행으로 다음 맵이동 시 도착위치 알림
void FGsQuestInstance::NextMapAutoProgress(const IGsMessageParam* In_data)
{
	if (EGsQuestUIState::QUEST_STATE_PROGRESS != _questUIState)
		return;

	const FGsAIMoveNextMapWarpInfoParam* warpInfoParam = In_data->Cast<const FGsAIMoveNextMapWarpInfoParam>();

	int8 num = _objectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		FGsObjectiveData& objectiveData = _objectiveDataList[i];
		if (objectiveData._objectiveId != _activeObjectiveId)
			continue;

		if (true == IsObjectiveCompleted(i))
			continue;

		if (true == objectiveData._isCheckGoal)
			continue;

		int currGoalIndex = objectiveData._currGoalIndex;
		if (!objectiveData._goalDataList.IsValidIndex(currGoalIndex))
			continue;

		objectiveData._goalDataList[currGoalIndex]._warpPos = warpInfoParam->_pos;
	}
}

// 내부적으로 체크(수동이동, 자동이동거리표시)가 도착을 받았다. 
void FGsQuestInstance::ArrivedManually()
{
	if (nullptr == _owner)
		return;

	bool isQuestAutoActive = false;
	if (FGsMovementHandlerLocalPlayer* movement = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>())
	{
		isQuestAutoActive =
			(movement->GetStateAutoMove().GetMovementAutoContentsType() == EGsMovementAutoContentsType::QuestMove) ?
			true : false;
	}

	if (true == isQuestAutoActive)
		return;

	if (EGsQuestUIState::QUEST_STATE_PROGRESS != _questUIState)
		return;

	int8 num = _objectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		FGsObjectiveData& objectiveData = _objectiveDataList[i];

		if (true == IsObjectiveCompleted(i))
			continue;

		if (true == IsObjectiveReady(i))
			continue;

		if (true == objectiveData._isCheckGoal)
			continue;

		QuestObjectiveId objectiveId = objectiveData._objectiveId;
		if (objectiveData._type == QuestObjectiveType::GOTO_POS)
		{
			SendQuestUpdate(objectiveId, 0, 0);
			objectiveData._isCheckGoal = true;
		}
		break;
	}
}

bool FGsQuestInstance::IsInteractionPropContents(int32 inTableId, OUT QuestObjectiveId& outObjectiveId) const
{
	if (EGsQuestUIState::QUEST_STATE_READY == _questUIState ||
		EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED == _questUIState)
	{
		return false;
	}		

	bool isInteracting = false;
	int8 num = _objectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		const FGsObjectiveData& objectiveData = _objectiveDataList[i];
		if (IsObjectiveCompleted(i))
		{
			continue;
		}			

		if (IsObjectiveReady(i))
		{
			continue;
		}
			
		if (QuestObjectiveType::INTERACT_PROP_TOUCH != objectiveData._type)
		{
			continue;
		}			

		if (objectiveData._tableIdList[0] != inTableId)
		{
			continue;
		}

		outObjectiveId = objectiveData._objectiveId;
		isInteracting = true;
	}

	return isInteracting;
}

bool FGsQuestInstance::OnInteractionContents(const IGsMessageParam* In_data)
{
	const FGsInteractionMessageParamContents* param = In_data->Cast<const FGsInteractionMessageParamContents>();

	if (_questUIState == EGsQuestUIState::QUEST_STATE_READY ||
		_questUIState == EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED)
		return false;

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return false;

	bool isInvadeWorld = GGameData()->IsInvadeWorld();
	bool isInteracting = false;
	int8 num = _objectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		FGsObjectiveData& objectiveData = _objectiveDataList[i];

		if (true == IsObjectiveCompleted(i))
			continue;

		if (true == IsObjectiveReady(i))
			continue;

		if (objectiveData._type != QuestObjectiveType::INTERACT_NPC)
			continue;

		if (objectiveData._tableIdList[0] != param->_npcTblId)
			continue;

		if (_schemaQuest)
		{
			if (isInvadeWorld)
			{
				if (_schemaQuest->questContentsType != QuestContentsType::INVADE)
					continue;
			}
			else
			{
				if (_schemaQuest->questContentsType == QuestContentsType::INVADE)
					continue;
			}
		}

		_activeObjectiveId = objectiveData._objectiveId;

		objectiveData._uniqueId = param->_gameId;
		objectiveData._isCheckGoal = true;
		isInteracting = true;
		
		FGsQuestTargetManagement* targetManagement = questManager->GetQuestTargetManagement();
		if (nullptr != targetManagement)
		{
			targetManagement->OnRemoveTarget(_questKey);

			if (questManager->IsAutoProgressQuest(_questKey))
			{
				questManager->ClearAutoProgressQuestKey();
			}

			// ui 갱신
			GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
		}		

		TryInteract(param->_gameId);
	}

	return isInteracting;
}

void FGsQuestInstance::TryInteract(const int64 inGameId)
{
	if (nullptr == _owner)
		return;

	if (nullptr == _schemaQuest)
		return;

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;	

	FGsQuestActionManagement* actionManagement = questManager->GetQuestActionManagement();
	if (nullptr == actionManagement)
		return;

	const FGsSchemaNpcDialogData* npcDialogData = _schemaQuest->questClearNpcDialogId.GetRow();
	if (nullptr == npcDialogData)
		return;
	
	// npc 대화 시 슬롯 잠금
	SetTouchLock(true);

	// Quest Hud Slot 상태는 none
	_questUIState = EGsQuestUIState::QUEST_STATE_NONE;

	actionManagement->OpenNpcDialog(_questId, inGameId, npcDialogData, FNpcDialogEndDelegate::CreateLambda([this](bool inResult, int64 inUniqueId)
		{
			if (inResult)
			{
				// 서버로 대화 완료 패킷 전송
				SendQuestUpdateIntractNpc(inUniqueId);
			}
			else
			{
				// 실패 시 진행 중으로 변경
				_questUIState = EGsQuestUIState::QUEST_STATE_PROGRESS;
				SetTouchLock(false);
				UpdateTarget(true);
			}
		}));		
}

void FGsQuestInstance::SendQuestUpdateIntractNpc(int64 inUniqueId)
{
	// npc 대화 종료 시 슬롯 잠금 해제
	SetTouchLock(false);

	// Quest Hud Slot 상태는 진행
	_questUIState = EGsQuestUIState::QUEST_STATE_PROGRESS;

	if (0 > inUniqueId)
	{
		uint32 spawId = GSClientSpawn()->GetUniqueID(inUniqueId);
		SendQuestUpdate(_activeObjectiveId, spawId, 0);
	}
	else
	{
		SendQuestUpdate(_activeObjectiveId, 0, inUniqueId);
	}
}

void FGsQuestInstance::SendQuestUpdate(QuestObjectiveId inQuestObjectiveId, SpawnId inSpawnId, int64 inGameId, MapId inMapId) const
{
	FGsNetSendServiceWorld::SendQuestMainUpdate(_questId, _questKey._questIndex, inQuestObjectiveId, inSpawnId, inGameId, inMapId);	
}

UGsGameObjectBase* FGsQuestInstance::GetGameObject(int inTableId) const
{
	TArray<UGsGameObjectBase*> listnpc = GSGameObject()->FindObjectArray(EGsGameObjectType::NonPlayer);
	for (UGsGameObjectBase* el : listnpc)
	{
		FGsGameObjectData* objectData = el->GetData();
		if (nullptr == objectData)
			continue;

		if (objectData->GetTableId() == inTableId)
		{
			return el;
		}
	}

	return nullptr;
}

// 내부적으로 거리체크(수동이동, 자동이동거리표시)를 한다.
void FGsQuestInstance::CheckGoalRadius()
{
	if (QuestState::NONE == _state)
		return;

	if (nullptr == _owner)
		return;

	AActor* actor = _owner->GetActor();
	if (nullptr == actor)
		return;

	FGsMovementHandlerLocalPlayer* movement = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (movement == nullptr)
		return;

	UGsLevelManager* levelMgr = GLevel();
	if (nullptr == levelMgr)
		return;

	int32 key = levelMgr->GetCurrentLevelId();

	bool isArrived = false;
	int8 num = _objectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		if (true == IsObjectiveCompleted(i))
			continue;

		if (true == IsObjectiveReady(i))
			continue;

		FGsObjectiveData& objectiveData = _objectiveDataList[i];

		int8 currGoalIndex = objectiveData._currGoalIndex;
		if (!objectiveData._goalDataList.IsValidIndex(currGoalIndex))
			continue;

		FGsObjectiveGoalData& goalData = objectiveData._goalDataList[currGoalIndex];

		bool isGoalPosZero = true;
		FVector2D pos2D;
		FVector pos;
		float range;
		bool isGoalMap = (goalData._goalMapId == key) ? true : false;
		if (!isGoalMap)
		{
			if (goalData._warpPos.IsZero())
				continue;

			pos2D = (FVector2D)goalData._warpPos;
			pos = goalData._warpPos;
			range = goalData._goalRadius;
		}
		else
		{
			isGoalPosZero = goalData._goalPos.IsZero();
			pos2D = (true == isGoalPosZero) ? (FVector2D)goalData._frontPos : (FVector2D)goalData._goalPos;
			pos = (true == isGoalPosZero) ? goalData._frontPos : goalData._goalPos;
			range = (true == isGoalPosZero) ? goalData._goalRadius : goalData._goalRadius;
		}

		float remainDistance = (pos2D - FVector2D(actor->GetActorLocation())).Size();
		if (objectiveData._objectiveId == _activeObjectiveId)
		{
			_remainDistance = remainDistance;
		}

		if (movement->GetStateAutoMove().GetMovementAutoContentsType() == EGsMovementAutoContentsType::QuestMove)
		{
			if (remainDistance < _drawQuestDistanceLimit)
			{
				goalData._isDrawPosition = false;
			}
		}

#if WITH_EDITOR
		if (UWorld* world = actor->GetWorld())
		{
			if (GSQuest()->GetDrawQuestGoalSpot())
			{
				DrawDebugLine(world, actor->GetActorLocation(), pos, FColor::Blue, false, 1.0f, 0, 10.0f);
				DrawDebugSphere(world, pos, range, 12, FColor::Blue, false, 1.0f, 0, 1.0f);
			}
		}
#endif
		if (true == objectiveData._isCheckGoal)
			continue;

		float Dist = remainDistance - (_owner->GetData()->GetScaledCapsuleRadius() * 1.1f);

		if (goalData._isDrawPosition == false)
		{
			_remainDistance = 0;
		}

		if (goalData._goalRadius <= Dist)
			continue;

		if (isGoalMap)
		{
			ArrivedManually();
		}
	}
}

int64 FGsQuestInstance::GetNpcUniqueId(int inTableId) const
{
	UGsGameObjectBase* object = GetGameObject(inTableId);

	if (nullptr == object)
		return 0;

	FGsGameObjectData* objectData = object->GetData();

	if (nullptr == objectData)
		return 0;

	return objectData->GetGameId();
}

// 내부적으로 체크(수동이동, 자동이동거리표시)를 0.5f 주기 마다 체크한다.
void FGsQuestInstance::StartTimer()
{
	UWorld* world = GSQuest()->GetWorld();
	if (nullptr == world)
		return;

	if (_goalRadiusHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_goalRadiusHandle);
	}

	world->GetTimerManager().SetTimer(_goalRadiusHandle,
		FTimerDelegate::CreateWeakLambda(_owner, [this]()
			{
				CheckGoalRadius();

			}), 0.25f, true);
}

// 내부적으로 모든 타이머를 멈춘다.
void FGsQuestInstance::StopTimer()
{
	UWorld* world = GSQuest()->GetWorld();
	if (nullptr == world)
		return;

	world->GetTimerManager().ClearTimer(_goalRadiusHandle);
	world->GetTimerManager().ClearAllTimersForObject(this);
}

// 텔레포트 정보를 찾아서 알려준다.
bool FGsQuestInstance::GetTeleportData(OUT int32& outTeleportGold, OUT QuestObjectiveId& outObjectiveId) const
{
	int8 num = _objectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		const FGsObjectiveData& objectiveData = _objectiveDataList[i];
		if (false == objectiveData._isComplete)
		{
			outTeleportGold = (nullptr == _schemaQuest) ? 0 : _schemaQuest->teleportGold;
			outObjectiveId = objectiveData._objectiveId;

			if (objectiveData._goalDataList.IsValidIndex(objectiveData._currGoalIndex))
			{
				int32 mapId = objectiveData._goalDataList[objectiveData._currGoalIndex]._goalMapId;
				if (GLevel()->IsDungeonMap(mapId))
				{
					bool isPossible = GSDungeon()->IsPossibleWarpByMapId(mapId);
					if (false == isPossible)
					{
						return false;
					}
				}
			}

			return true;
		}
	}

	return false;
}

// 다음 스토리의 첫 퀘스트 시작
void FGsQuestInstance::AddNextStoryQuest()
{
	if (0 < _nextQuestId)
	{
		GSQuest()->AddNextStoryQuest(_nextQuestId);
	}
}

// 다음 연퀘 시작
void FGsQuestInstance::AddNextQuest()
{
	if (0 >= _nextQuestId)
		return;

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Quest] FGsQuestInstance AddNextQuest ID: %llu"), _nextQuestId);
#endif

	GSQuest()->AddNextQuest(_nextQuestId);
}

// Hud에서 쓸 오브젝티브 수행 목표 스트링 ("오크를 처치하라")
void FGsQuestInstance::GetObjectiveContentsText(IN int inIndex, OUT FText& outObjectiveText) const
{
	if (inIndex >= _objectiveTableCacheList.Num())
		return;

	FGsQuestObjectiveTableCache* objectiveTableCache = _objectiveTableCacheList[inIndex].Get();
	if (nullptr == objectiveTableCache)
		return;

	outObjectiveText = objectiveTableCache->GetDescText();
}

// Hud에서 쓸 오브젝티브 수행 목표 카운트 "5/10"
bool FGsQuestInstance::GetObjectiveCountText(IN int inIndex, OUT bool& outIsComplete, OUT bool& outIsReady, OUT int& outCurrValue, OUT int& outTableValue)
{
	outCurrValue = 0;
	outTableValue = 0;
	outIsComplete = false;
	outIsReady = false;

	if (nullptr == _schemaQuest)
		return false;

	// PRE_ACCEPT 상태일 때에는 앞전 퀘스트의 정보를 출력한다.
	bool isPreAcceptState = (_state == QuestState::PRE_ACCEPT) ? true : false;

	TArray<TSharedPtr<FGsQuestObjectiveTableCache>> objectiveTableCacheList = (isPreAcceptState) ?
		_preQuestTableCache->GetObjectiveTableCache() : _objectiveTableCacheList;

	if (inIndex >= objectiveTableCacheList.Num())
		return false;

	FGsQuestObjectiveTableCache* objectiveTableCache = objectiveTableCacheList[inIndex].Get();
	if (nullptr == objectiveTableCache)
		return false;

	outIsReady = IsObjectiveReady(inIndex);

	if (objectiveTableCache->GetisCountHide())
	{
		outIsComplete = IsObjectiveCompleted(inIndex);
		return false;
	}
	else
	{
		outTableValue = objectiveTableCache->GetObjectiveValue();
		outCurrValue = (false == isPreAcceptState) ? GetObjectiveValue(inIndex) : outTableValue;
		// 목표를 다 채웠는가? 10/10??
		outIsComplete = (outCurrValue >= outTableValue) ? true : false;
		return true;
	}

	return false;
}

// 오브젝티브가 수행 완료 했는가? (몬스터 처치 10/10)
bool FGsQuestInstance::IsObjectiveCompleted(int32 inIndex) const
{
	if (inIndex >= _objectiveTableCacheList.Num())
		return false;

	FGsQuestObjectiveTableCache* objectiveTableCache = _objectiveTableCacheList[inIndex].Get();
	if (nullptr == objectiveTableCache)
		return false;

	return (GetObjectiveValue(inIndex) >= objectiveTableCache->GetObjectiveValue()) ? true : false;
}

// 오브젝티브 수행 카운트
int32 FGsQuestInstance::GetObjectiveValue(int32 inIndex) const
{
	if (!_questData.IsValid())
		return 0;

	FGsQuestDynamicData* dynamicData = _questData->GetQuestDynamicData(_questKey._questIndex);
	if (nullptr == dynamicData)
		return 0;

	TArray<int32>& arrValue = dynamicData->GetObjectiveValueList();
	if (inIndex >= arrValue.Num())
		return 0;

	return arrValue[inIndex];
}

// 퀘스트에 속한 오브젝티브 갯수
int32 FGsQuestInstance::GetObjectiveNum() const
{
	return _objectiveTableCacheList.Num();
}

// 자동진행 중인지 체크하기 위해 AIQuestHandler를 얻어온다.
FGsAIQuestHandler* FGsQuestInstance::GetAIQuestHandler() const
{
	FGsAIManager* aiMgr = GSAI();
	if (nullptr == aiMgr)
		return nullptr;

	return aiMgr->GetAIQuestHandler();
}

bool FGsQuestInstance::IsGotoFieldType()
{
	int num = _objectiveDataList.Num();
	if (0 >= num || 1 < num)
		return false;

	FGsObjectiveData& objectiveData = _objectiveDataList[0];
	if (objectiveData._type == QuestObjectiveType::WARP_TO_INSTANCE_FIELD ||
		objectiveData._type == QuestObjectiveType::WARP_TO_LAST_PUBLIC_FIELD||
		objectiveData._type == QuestObjectiveType::TELEPORT)
	{
		return true;
	}

	return false;
}

void FGsQuestInstance::OnGotoField()
{
	UGsLevelManager* levelManager = GLevel();
	if (nullptr == levelManager)
		return;

	// 시네마틱 플레이 중이면?
	if (true == UGsSequencePlayer::IsSequencePlaying())
	{
		FText findText;
		FText::FindText(TEXT("NetText"), TEXT("WARP_ERROR_CINEMATIC_CANCLE"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return;
	}

	MapType currMapType = MapType::PUBLIC_FIELD;
	if (const FGsSchemaMapData* mapData = GLevel()->GetCurrentMapInfo())
	{
		currMapType = mapData->mapType;
	}

	FGsObjectiveData& objectiveData = _objectiveDataList[0];
	int8 currGoalIndex = objectiveData._currGoalIndex;

	if (objectiveData._goalDataList.IsValidIndex(currGoalIndex))
	{
		if (objectiveData._type == QuestObjectiveType::WARP_TO_INSTANCE_FIELD)
		{
			if (MapType::INSTANCE_SINGLE != currMapType)
			{
				ClosePopupTeleport();
				SendQuestUpdate(objectiveData._objectiveId, 0, 0, objectiveData._goalDataList[currGoalIndex]._goalMapId);
			}
		}
		else if (objectiveData._type == QuestObjectiveType::WARP_TO_LAST_PUBLIC_FIELD)
		{
			// 2023-05-04 레오 성으로 퀘스트 중 싱글필드에서 퍼블릭 필드로 워프해야하는 상황인데
			// 이미 퍼블릭 필드인 상태로 퀘스트가 수행되면서 동일 상태 맵이라 서버로 업데이트를 못보내는 상황이 발생했다.
			// 임시로 조건체크를 빼고 무조건 업데이트를 보낸다.
			// 로그만 남김
			if (MapType::INSTANCE_SINGLE == currMapType ||
				MapType::SINGLE_FIELD == currMapType)
			{
#if WITH_EDITOR
				GSLOG(Log, TEXT("[Quest] OnGotoField SINGLE -> PUBLIC Error : MapType::INSTANCE_SINGLE == currMapType or MapType::SINGLE_FIELD == currMapType"), _questId);
				GSLOG(Log, TEXT("[Quest] OnGotoField Error QuestId : %llu"), _questId);
#endif
			}

			ClosePopupTeleport();
			SendQuestUpdate(objectiveData._objectiveId, 0, 0, objectiveData._goalDataList[currGoalIndex]._goalMapId);
		}
	}
	else
	{
		if (objectiveData._type == QuestObjectiveType::TELEPORT)
		{
			SendQuestUpdate(objectiveData._objectiveId, 0, 0);
		}
	}

	OnPostGotoField();
}

void FGsQuestInstance::ClosePopupTeleport()
{
	// 퀘스트 완료 될때 퀘스트워프창 오픈되어 있으면 닫기 처리
	// QuestObjectiveType::WARP_TO_LAST_PUBLIC_FIELD,  QuestObjectiveType::WARP_TO_INSTANCE_FIELD 요청할때 추가처리
	if (UGsUIManager* uiManager = GUI())
	{
		if (true == uiManager->IsActiveWidget(TEXT("PopupTeleport")))
		{
			uiManager->CloseByKeyName(TEXT("PopupTeleport"));
		}
	}
}

bool FGsQuestInstance::GetIsTeleportAble() const
{
	if (false == _isTeleportAble)
		return false;

	if (nullptr == _schemaQuest)
		return false;

	if (false == _schemaQuest->isAutoPlayAble)
		return false;

	return _schemaQuest->isTeleportAble;
}

bool FGsQuestInstance::IsShowHudUI() const
{
	if (_questUIState == EGsQuestUIState::QUEST_STATE_NONE)
	{
		return false;
	}

	if (nullptr == _schemaQuest)
	{
		return false;
	}

	// 침공서버에서는 침공 체크된 퀘스트만 표시
	if (GGameData()->IsInvadeWorld())
	{
		if (QuestContentsType::INVADE != _schemaQuest->questContentsType)
			return false;
	}

	return true;
}

bool FGsQuestInstance::IsObjectiveReady(int32 inIndex) const
{
	if (_schemaQuest->objectiveProcessType == QuestObjectiveProcessType::ORDER)
	{
		for (int i = 0; i <= inIndex; ++i)
		{
			if (i < inIndex)
			{
				if (false == IsObjectiveCompleted(i))
				{
					return true;
				}
			}
		}
	}

	return false;
}


void FGsQuestInstance::GetQuestUIState(OUT EGsQuestUIState& outUiState)
{
	outUiState = _questUIState;
}

void FGsQuestInstance::UpdateUIEffect(EGsQuestUIState inValue)
{
}

void FGsQuestInstance::SetQuestUIEffectType(IN EGsQuestUIEffectType inUiEffectType)
{
	if (inUiEffectType == EGsQuestUIEffectType::QUEST_UI_EFFECT_UPDATE)
	{
		if (_questUIState == EGsQuestUIState::QUEST_STATE_PROGRESS)
		{
			_questUIEffectType = inUiEffectType;
		}
	}
	else
	{
		_questUIEffectType = inUiEffectType;
	}
}

void FGsQuestInstance::GetQuestUIEffectType(OUT EGsQuestUIEffectType& outUiEffectType)
{
	outUiEffectType = _questUIEffectType;

	if (_questUIEffectType != EGsQuestUIEffectType::QUEST_UI_EFFECT_REWARD)
	{
		_questUIEffectType = EGsQuestUIEffectType::QUEST_UI_EFFECT_NONE;
	}
}

bool FGsQuestInstance::GetActiveObjectiveType(OUT QuestObjectiveType& outObjectiveType)
{
	int index = 0;
	if (0 >= _activeObjectiveId)
		return false;
	
	QuestObjectiveId id = _activeObjectiveId;
	index = _objectiveDataList.IndexOfByPredicate([id](const FGsObjectiveData& data)
		{
			return data._objectiveId == id;
		});

	if (-1 == index)
		return false;

	int num = _objectiveDataList.Num();
	if (index >= num)
		return false;

	outObjectiveType = _objectiveDataList[index]._type;
	return true;
}

QuestContentsType FGsQuestInstance::GetQuestContentsType()
{
	if (nullptr == _schemaQuest)
		return QuestContentsType::NORMAL;

	return _schemaQuest->questContentsType;
}

// 프리 시작
void FGsQuestInstance::PrAcceptBeginEvent(QuestEventType inQuestEventType)
{
	if (!_questData.IsValid())
		return;

	FGsQuestDynamicData* dynamicData = _questData->GetQuestDynamicData();
	if (nullptr == dynamicData)
		return;

	EventActionIndex preEventActionIndex = dynamicData->GetPreEventActionIndex();
	EventActionGroupId eventActionGroupId = 0;
	if (nullptr != _schemaQuest)
	{
		if (const FGsSchemaEventActionGroup* eventActionGroup = _schemaQuest->preQuestEventActionGroupId.GetRow())
		{
			eventActionGroupId = eventActionGroup->id;
		}
	}

	GSLOG(Log, TEXT("[Quest] QuestMainInstance PrAcceptBeginEvent -> questId : %llu, eventActionGroupId : %d, preEventActionIndex : %d"), _questId, eventActionGroupId, preEventActionIndex);

	FGsQuestBeginActionParam parm(inQuestEventType, _questId, eventActionGroupId, preEventActionIndex);
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_EVENT_BEGIN_START, &parm);
}

// 퀘스트 시작연출
void FGsQuestInstance::StartQuestBeginEvent(QuestEventType inQuestEventType)
{
	int32 subDialogId = 0;
	if (nullptr != _schemaQuest)
	{
		if (const FGsSchemaSubDialogData* data = _schemaQuest->questBeginSubDialogId.GetRow())
		{
			subDialogId = data->Id;
		}
	}

	FGsQuestBeginActionParam parm(inQuestEventType, _questId, subDialogId);
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_EVENT_BEGIN_START, &parm);
}


// 퀘스트 종료 연출
void FGsQuestInstance::StartQuestClearEvent(QuestEventType inQuestEventType)
{
	if (!_questData.IsValid())
		return;

	FGsQuestDynamicData* dynamicData = _questData->GetQuestDynamicData();
	if (nullptr == dynamicData)
		return;

	EventActionIndex postEventActionIndex = dynamicData->GetPostEventActionIndex();
	EventActionIndex preEventActionIndex = INVALID_EVENT_ACTION_INDEX;

	int32 postEventActionGroupId = 0;
	int32 subDialogId = 0;
	if (nullptr != _schemaQuest)
	{
		if (const FGsSchemaEventActionGroup* postQuestEventActionGroup = _schemaQuest->postQuestEventActionGroupId.GetRow())
		{
			postEventActionGroupId = postQuestEventActionGroup->id;			
		}

		if (const FGsSchemaSubDialogData* data = _schemaQuest->questClearSubDialogId.GetRow())
		{
			subDialogId = data->Id;
		}
	}

	int32 preEventActionGroupId = 0;
	TSharedPtr<FGsQuestData> nextQuestData = GSQuest()->GetQuestData(_nextQuestId);
	if (nextQuestData.IsValid())
	{
		FGsQuestDynamicData* nextQuestDynamicData = nextQuestData->GetQuestDynamicData();
		if (nullptr == nextQuestDynamicData)
		{
			nextQuestDynamicData = nextQuestData->ClaimQuestDynamicData(_questKey._questIndex);
		}

		if (nextQuestDynamicData)
		{
			if (TSharedPtr<FGsQuestTableCache> questTableCache = GSQuest()->GetQuestTableCache(_nextQuestId))
			{
				if (const FGsSchemaQuest* schemaQuest = questTableCache->GetSchemaQuest())
				{
					if (const FGsSchemaEventActionGroup* preQuestEventActionGroup = schemaQuest->preQuestEventActionGroupId.GetRow())
					{
						preEventActionGroupId = preQuestEventActionGroup->id;
					}
				}
			}
		}
	}

	GSLOG(Log, TEXT("[Quest] QuestMainInstance QuestClearEvent -> questId : %llu, _nextQuestId : %llu, subDialogId : %d"), _questId, _nextQuestId, subDialogId);
	GSLOG(Log, TEXT("[Quest] QuestMainInstance QuestClearEvent -> preEventActionGroupId : %d, postEventActionGroupId : %d, preEventActionIndex : %d, postEventActionIndex : %d"),
		preEventActionGroupId, postEventActionGroupId, preEventActionIndex, postEventActionIndex);

	FGsQuestClearActionParam parm(inQuestEventType, _questId, _nextQuestId, subDialogId,
		preEventActionGroupId, postEventActionGroupId, preEventActionIndex, postEventActionIndex);
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_EVENT_CLEAR_START, &parm);
}


// 종료
void FGsQuestInstance::StartObjectiveClearEvent(QuestEventType inQuestEventType, int8 inindex)
{
	if (inindex >= _objectiveTableCacheList.Num())
		return;

	int32 subDialogId = 0;
	if (FGsQuestObjectiveTableCache* objectiveTableCache = _objectiveTableCacheList[inindex].Get())
	{
		if (const FGsSchemaQuestObjective* objective = objectiveTableCache->GetSchemaQuestObjective())
		{
			if (const FGsSchemaSubDialogData* data = objective->objectiveClearSubDialogId.GetRow())
			{
				subDialogId = data->Id;
			}
		}
	}

	FGsQuestClearActionParam parm(inQuestEventType, _questId, subDialogId);
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_EVENT_CLEAR_START, &parm);
}

// 캠프랑 부딪혔을 때
void FGsQuestInstance::OnCampHit(const AGsCampBase* In_camp)
{
	if (In_camp == nullptr)
		return;

	if (QuestState::NONE == _state)
		return;

	if (nullptr == _owner)
		return;

	AActor* actor = _owner->GetActor();
	if (nullptr == actor)
		return;

	UGsLevelManager* levelMgr = GLevel();
	if (nullptr == levelMgr)
		return;

	const FVector& campLocation = In_camp->GetActorLocation();	
	UCapsuleComponent* capsule = In_camp->GetCapsuleComponent();
	if (capsule == nullptr)
		return;

	float campRadius = capsule->GetScaledCapsuleRadius();

	int32 key = levelMgr->GetCurrentLevelId();
	bool isArrived = false;
	int8 num = _objectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		if (true == IsObjectiveCompleted(i))
			continue;

		if (true == IsObjectiveReady(i))
			continue;

		FGsObjectiveData& objectiveData = _objectiveDataList[i];
		if (true == objectiveData._isCheckGoal)
			continue;

		int8 currGoalIndex = objectiveData._currGoalIndex;
		if (!objectiveData._goalDataList.IsValidIndex(currGoalIndex))
			continue;

		FGsObjectiveGoalData& goalData = objectiveData._goalDataList[currGoalIndex];
		if (goalData._goalMapId != key)
		{
			continue;
		}

		bool isGoalPosZero = goalData._goalPos.IsZero();
		FVector pos = (isGoalPosZero) ? goalData._frontPos : goalData._goalPos;
		float dist = FVector::Dist(pos, campLocation);

		if (campRadius < dist)
		{
			continue;
		}

		FVector2D pos2D;
		pos2D = (true == isGoalPosZero) ? (FVector2D)goalData._frontPos : (FVector2D)goalData._goalPos;	

		float remainDistance = (pos2D - FVector2D(actor->GetActorLocation())).Size();
		float Dist = remainDistance - (_owner->GetData()->GetScaledCapsuleRadius() * 1.1f);

		float acceptanceRadius = 0.0f;
		if (const UGsGlobalConstant* globalConstData = GData()->GetGlobalData())
		{
			acceptanceRadius =
				globalConstData->_autoMoveCampHitAddAcceptanceRadius;
		}

		if (acceptanceRadius >= Dist)
		{
			ArrivedManually();
		}
	}
}

bool FGsQuestInstance::IsInGoalArea(OUT bool& outIsInGalArea)
{
	if (QuestState::NONE == _state)
		return false;

	if (nullptr == _owner)
		return false;

	AActor* actor = _owner->GetActor();
	if (nullptr == actor)
		return false;

	FGsMovementHandlerLocalPlayer* movement = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (movement == nullptr)
		return false;

	UGsLevelManager* levelMgr = GLevel();
	if (nullptr == levelMgr)
		return false;

	int32 key = levelMgr->GetCurrentLevelId();

	bool isArrived = false;
	int8 num = _objectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		if (true == IsObjectiveCompleted(i))
			continue;

		if (true == IsObjectiveReady(i))
			continue;

		FGsObjectiveData& objectiveData = _objectiveDataList[i];
		for (int j = 0; j < objectiveData._goalDataList.Num(); ++j)
		{
			FGsObjectiveGoalData& goalData = objectiveData._goalDataList[j];

			bool isGoalPosZero = true;
			FVector2D pos2D;
			FVector pos;
			float range;
			bool isGoalMap = (goalData._goalMapId == key) ? true : false;
			if (!isGoalMap)
			{				
				continue;
			}
			else
			{
				isGoalPosZero = goalData._goalPos.IsZero();
				pos2D = (true == isGoalPosZero) ? (FVector2D)goalData._frontPos : (FVector2D)goalData._goalPos;
				pos = (true == isGoalPosZero) ? goalData._frontPos : goalData._goalPos;
				range = (true == isGoalPosZero) ? goalData._goalRadius : goalData._goalRadius;
			}

			float remainDistance = (pos2D - FVector2D(actor->GetActorLocation())).Size();
			float Dist = remainDistance - (_owner->GetData()->GetScaledCapsuleRadius() * 1.1f);

			if (goalData._goalRadius > Dist)
			{
				outIsInGalArea = true;
				return true;
			}
		}		
	}

	return true;
}