#include "GsQuestTargetManagement.h"

#include "Engine/World.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "Message/MessageParam/GsMinimapMessageParam.h"
#include "Message/MessageParam/GsQuestMessageParam.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"

#include "Quest/GsQuestData.h"
#include "Quest/GsSchemaQuest.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

#include "TimerManager.h"


// 초기화
void FGsQuestTargetManagement::Initialize()
{
	if (0 >= _addTargetFuncs.Num())
	{
		_addTargetFuncs.Add([this](const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList)
				{ return AddTargetMonster(inQuestKey, inLevelKey, inObjectiveDataList); });
		_addTargetFuncs.Add([this](const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList)
				{ return AddTargetNpc(inQuestKey, inLevelKey, inObjectiveDataList); });
		_addTargetFuncs.Add([this](const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList)
				{ return AddTargetPos(inQuestKey, inLevelKey, inObjectiveDataList); });
		_addTargetFuncs.Add([this](const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList)
			{ return AddTargetProp(inQuestKey, inLevelKey, inObjectiveDataList); });
	}

	if (0 >= _removeTargetFuncs.Num())
	{
		_removeTargetFuncs.Add([this](const QuestKey& inQuestKey) { return RemoveTargetMonster(inQuestKey); });
		_removeTargetFuncs.Add([this](const QuestKey& inQuestKey) { return RemoveTargetNpc(inQuestKey); });
		_removeTargetFuncs.Add([this](const QuestKey& inQuestKey) { return RemoveTargetPos(inQuestKey); });
		_removeTargetFuncs.Add([this](const QuestKey& inQuestKey) { return RemoveTargetProp(inQuestKey); });
	}
}

// 클리어
void FGsQuestTargetManagement::Finalize()
{
	Clear();

	_addTargetFuncs.Reset();
	_removeTargetFuncs.Reset();
}

void FGsQuestTargetManagement::Clear()
{
	if (_mapQuestMonsterId.Num() != 0)
	{
		for (auto& iter : _mapQuestMonsterId)
		{
			iter.Value.Empty();
		}
		_mapQuestMonsterId.Empty();
	}

	if (_mapQuestNpcId.Num() != 0)
	{
		for (auto& iter : _mapQuestNpcId)
		{
			iter.Value.Empty();
		}
		_mapQuestNpcId.Empty();
	}

	if (_mapQuestPropId.Num() != 0)
	{
		for (auto& iter : _mapQuestPropId)
		{
			iter.Value.Empty();
		}
		_mapQuestPropId.Empty();
	}

	_mapQuestSubNpcId.Reset();
}

// 3타입 모두 돌면서 타겟을 추가한다.
void FGsQuestTargetManagement::OnAddTarget(const QuestKey& inQuestKey, const TArray<FGsObjectiveData>& inObjectiveDataList)
{
	UGsLevelManager* levelMgr = GLevel();
	if (nullptr == levelMgr)
		return;

	int32 key = levelMgr->GetCurrentLevelId();

	for (auto& iter : _addTargetFuncs)
	{
		iter(inQuestKey, key, inObjectiveDataList);
	}
}

void FGsQuestTargetManagement::UpdateTartgetNpcInteractionMark(bool isActive, int32 inNpcID, EQuestNpcState inQuestState)
{
	if (true == _mapQuestSubNpcId.Contains(inNpcID))
	{
		if (isActive == true)
		{
			if (inQuestState != _mapQuestSubNpcId.FindRef(inNpcID))
				_mapQuestSubNpcId[inNpcID] = inQuestState;
		}
		else
		{
			_mapQuestSubNpcId.Remove(inNpcID);
		}
	}
	else
	{
		if (isActive == true)
			_mapQuestSubNpcId.Add(inNpcID, inQuestState);
		else
			return;
	}

	//if (_mapQuestNpcId.Contains(inNpcID))
		//return;

	bool isMiniMapOn = false;
	if (isActive)
	{
		if (inQuestState == EQuestNpcState::STATE_SUB_WATING ||
			inQuestState == EQuestNpcState::STATE_PROGRESS ||
			inQuestState == EQuestNpcState::STATE_REWARD_WATING)
			isMiniMapOn = true;
	}

	int outIndex = 0;
	if (GetQuestSubNpcStateIconIndex(inNpcID, outIndex) == true)
	{
		FGsMinimapQuesSubInterationNpcParam param(true, isMiniMapOn, inNpcID, outIndex);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_SUB_INTERATION_NPC, &param);
	}
	else
	{
		FGsMinimapQuesSubInterationNpcParam param(false, isMiniMapOn, inNpcID, outIndex);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_SUB_INTERATION_NPC, &param);
	}
}

void FGsQuestTargetManagement::UpdateTartgetNpcInteractionMark(bool isActive, TArray<TPair<int32, EQuestNpcState>> inSubNpcList)
{
	TArray<FGsSubQuestNpcMarkData> MarkDataList;
	for (const TPair<int32, EQuestNpcState>& npcState : inSubNpcList)
	{
		int32 npcID = npcState.Key;
		EQuestNpcState questState = npcState.Value;

		if (true == _mapQuestSubNpcId.Contains(npcID))
		{
			if (isActive == true)
			{
				if (questState != _mapQuestSubNpcId.FindRef(npcID))
					_mapQuestSubNpcId[npcID] = questState;
			}
			else
			{
				_mapQuestSubNpcId.Remove(npcID);
			}
		}
		else
		{
			if (isActive == true)
				_mapQuestSubNpcId.Add(npcID, questState);
			else
				continue;
		}

		/*if (_mapQuestNpcId.Contains(npcID))
			continue;*/

		bool isMiniMapOn = false;
		if (isActive)
		{
			if (questState == EQuestNpcState::STATE_SUB_WATING ||
				questState == EQuestNpcState::STATE_PROGRESS ||
				questState == EQuestNpcState::STATE_REWARD_WATING)
				isMiniMapOn = true;
		}
		
		int outIndex = 0;
		if (GetQuestSubNpcStateIconIndex(npcID, outIndex) == true)
		{
			MarkDataList.Emplace(FGsSubQuestNpcMarkData(outIndex, isActive, isMiniMapOn, npcID));
		}
		else
		{
			MarkDataList.Emplace(FGsSubQuestNpcMarkData(outIndex, isActive, isMiniMapOn, npcID));
		}
	}
	
	if (0 < MarkDataList.Num())
	{
		FGsQuesSubInterationNpcMarkParam param(MarkDataList);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_SUB_INTERATION_NPC_LIST, &param);
	}
}

// 타겟 목록에 몬스터를 추가한다.
void FGsQuestTargetManagement::AddTargetMonster(const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList)
{
 	FGsMinimapQuestTargetMonsterParam addMonsterParam;
 	addMonsterParam._isAdd = true;
 	
 	FGsMinimapQuestTargetMonsterParam removeMonsterParam;
 	removeMonsterParam._isAdd = false;
 
 	int8 num = inObjectiveDataList.Num();
 	for (int8 i = 0; i < num; i++)
 	{
 		FGsObjectiveData objectiveData = inObjectiveDataList[i];
 
 		if (true == objectiveData._isReady)
 			continue;
 
 		int8 currGoalIndex = objectiveData._currGoalIndex;
 		if (!objectiveData._goalDataList.IsValidIndex(currGoalIndex))
 			continue;
 
 		if (inLevelKey != objectiveData._goalDataList[currGoalIndex]._goalMapId)
 			continue;
 
 		if (objectiveData._type == QuestObjectiveType::KILL
 			|| objectiveData._type == QuestObjectiveType::KILL_CREATURE_TYPE){}
 		else
 			continue;

 		for (int32 iter : objectiveData._tableIdList)
		{
 			// 삭제
 			if (true == objectiveData._isComplete)
 			{	
 				if (_mapQuestMonsterId.Contains(iter))
 				{
 					int numCount = _mapQuestMonsterId.FindChecked(iter).Remove(&inQuestKey);
					 					
 					if (0 >= _mapQuestMonsterId.FindChecked(iter).Num())
 					{
 						_mapQuestMonsterId.Remove(iter);
 					}
 
 					if (!_mapQuestMonsterId.Contains(iter))
 					{
 						// 삭제 파람에 추가
 						removeMonsterParam._tableId.Add(iter);
 					}
 				}
 			}
 			else
 			{	
 				if (!_mapQuestMonsterId.Contains(iter))
 				{
 					// 추가 파람에 추가
 					addMonsterParam._tableId.Add(iter);
 				}
 				_mapQuestMonsterId.FindOrAdd(iter).Add(&inQuestKey);
 			}			
 		}
	}
 
 	if (0 < addMonsterParam._tableId.Num())
 	{
 		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_TARGET_MONSTER, &addMonsterParam);
 	}
 
 	if (0 < removeMonsterParam._tableId.Num())
 	{
 		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_TARGET_MONSTER, &removeMonsterParam);
 	}
}

// 타겟 목록에 npc를 추가한다.
void FGsQuestTargetManagement::AddTargetNpc(const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList)
{
	// 추가
	FGsMinimapQuestTargetNpcParam addNpcParam;
	addNpcParam._isAdd = true;

	// 삭제
	FGsMinimapQuestTargetNpcParam removeNpcParam;
	removeNpcParam._isAdd = false;

	int8 num = inObjectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		FGsObjectiveData objectiveData = inObjectiveDataList[i];

		if (true == objectiveData._isReady)
			continue;

		int8 currGoalIndex = objectiveData._currGoalIndex;
		if (!objectiveData._goalDataList.IsValidIndex(currGoalIndex))
			continue;

		if (inLevelKey != objectiveData._goalDataList[currGoalIndex]._goalMapId)
			continue;

		if (objectiveData._type != QuestObjectiveType::INTERACT_NPC)
			continue;
		
		if(0 >= objectiveData._tableIdList.Num())
			continue;

		int32 npcId = objectiveData._tableIdList[0];
		if (0 < npcId)
		{
			// 삭제
			if (true == objectiveData._isComplete)
			{
				if (_mapQuestNpcId.Contains(npcId))
				{
					_mapQuestNpcId.FindChecked(npcId).Remove(&inQuestKey);
					if (0 >= _mapQuestNpcId.FindChecked(npcId).Num())
					{
						_mapQuestNpcId.Remove(npcId);
					}

					if (!_mapQuestNpcId.Contains(npcId))
					{
						// 삭제 파람에 추가
						removeNpcParam._tableId.Add(npcId);
					}
				}
			}
			else
			{
				if (!_mapQuestNpcId.Contains(npcId))
				{
					// 추가 파람에 추가
					addNpcParam._tableId.Add(npcId);
				}
				_mapQuestNpcId.FindOrAdd(npcId).Add(&inQuestKey);
			}
		}	
	}

	if (0 < addNpcParam._tableId.Num())
	{
		addNpcParam._interactionIndex = 0;
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_TARGET_NPC, &addNpcParam);
	}

	if (0 < removeNpcParam._tableId.Num())
	{		
		for (auto& iter : _mapQuestSubNpcId)
		{
			if (removeNpcParam._tableId.Contains(iter.Key))
			{
				removeNpcParam._tableId.Remove(iter.Key);
				UpdateTartgetNpcInteractionMark(true, iter.Key, iter.Value);
			}			
		}

		if (0 < removeNpcParam._tableId.Num())
		{
			GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_TARGET_NPC, &removeNpcParam);
		}
	}
}

// 타겟 목록에 위치를 추가한다. 
void FGsQuestTargetManagement::AddTargetPos(const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList)
{
	// 추가
	FGsMinimapQuestTargetPosParam addPosParam;
	addPosParam._isAdd = true;

	// 삭제
	FGsMinimapQuestTargetPosParam removePosParam;
	removePosParam._isAdd = false;

	int8 num = inObjectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		FGsObjectiveData objectiveData = inObjectiveDataList[i];

		if (true == objectiveData._isReady)
			continue;

		int8 currGoalIndex = objectiveData._currGoalIndex;		
		if (!objectiveData._goalDataList.IsValidIndex(currGoalIndex))
			continue;

		if (inLevelKey != objectiveData._goalDataList[currGoalIndex]._goalMapId)
			continue;

		if(objectiveData._type != QuestObjectiveType::GOTO_POS)
			continue;

		int32 mapId = objectiveData._goalDataList[currGoalIndex]._goalMapId;
		int32 spotId = objectiveData._goalDataList[currGoalIndex]._spotId;

		if (true == objectiveData._isComplete)
		{
			if (_mapQuestPos.Contains(spotId))
			{
				_mapQuestPos.FindChecked(spotId).Remove(&inQuestKey);
				if (0 >= _mapQuestPos.FindChecked(spotId).Num())
				{
					_mapQuestPos.Remove(spotId);
				}

				if (!_mapQuestPos.Contains(spotId))
				{
					// 삭제 파람에 추가
					removePosParam._posData.Add(spotId);
				}
			}
		}
		else
		{
			// 추가
			if (!_mapQuestPos.Contains(spotId))
			{
				// 추가 파람에 추가
				addPosParam._posData.Add(spotId);
			}
			_mapQuestPos.FindOrAdd(spotId).Add(&inQuestKey);
		}
	}
	
	if (0 < addPosParam._posData.Num())
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_TARGET_POS, &addPosParam);
	}	
	
	if (0 < removePosParam._posData.Num())
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_TARGET_POS, &removePosParam);
	}	
}

// 프랍
void FGsQuestTargetManagement::AddTargetProp(const QuestKey& inQuestKey, int32 inLevelKey, const TArray<FGsObjectiveData>& inObjectiveDataList)
{
	// 추가
	FGsMinimapQuestTargetPropParam addPropParam;
	addPropParam._isAdd = true;

	// 삭제
	FGsMinimapQuestTargetPropParam removePropParam;
	removePropParam._isAdd = false;

	int8 num = inObjectiveDataList.Num();
	for (int8 i = 0; i < num; i++)
	{
		FGsObjectiveData objectiveData = inObjectiveDataList[i];
		if (true == objectiveData._isReady)
			continue;

		int8 currGoalIndex = objectiveData._currGoalIndex;
		if (!objectiveData._goalDataList.IsValidIndex(currGoalIndex))
			continue;

		if (inLevelKey != objectiveData._goalDataList[currGoalIndex]._goalMapId)
			continue;

		if (objectiveData._type != QuestObjectiveType::INTERACT_PROP_TOUCH)
			continue;

		if (0 >= objectiveData._tableIdList.Num())
			continue;

		int32 propId = objectiveData._tableIdList[0];
		if (0 < propId)
		{
			// 삭제
			if (true == objectiveData._isComplete)
			{
				if (_mapQuestPropId.Contains(propId))
				{
					_mapQuestPropId.FindChecked(propId).Remove(&inQuestKey);
					if (0 >= _mapQuestPropId.FindChecked(propId).Num())
					{
						_mapQuestPropId.Remove(propId);
					}

					if (!_mapQuestPropId.Contains(propId))
					{
						// 삭제 파람에 추가
						removePropParam._tableId.Add(propId);
					}
				}
			}
			else
			{
				if (!_mapQuestPropId.Contains(propId))
				{
					// 추가 파람에 추가
					addPropParam._tableId.Add(propId);
				}
				_mapQuestPropId.FindOrAdd(propId).Add(&inQuestKey);
			}
		}
	}

	if (0 < addPropParam._tableId.Num())
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_TARGET_PROP, &addPropParam);
	}

	if (0 < removePropParam._tableId.Num())
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_TARGET_PROP, &removePropParam);
	}
}

// 3타입 모두 돌면 타겟 삭제
void FGsQuestTargetManagement::OnRemoveTarget(const QuestKey& inQuestKey)
{
	for (auto& iter : _removeTargetFuncs)
	{
		iter(inQuestKey);
	}
}

// 타겟 목록에서 몬스터를 삭제한다.
void FGsQuestTargetManagement::RemoveTargetMonster(const QuestKey& inQuestKey)
{	
	if (0 >= _mapQuestMonsterId.Num())
		return;

	FGsMinimapQuestTargetMonsterParam param;
	param._isAdd = false;

	TArray<int32> deleteList;
	for (auto iter = _mapQuestMonsterId.CreateConstIterator(); iter; ++iter)
	{
		_mapQuestMonsterId.FindChecked(iter->Key).Remove(&inQuestKey);
		if (0 >= _mapQuestMonsterId.FindChecked(iter->Key).Num())
		{
			param._tableId.Add(iter->Key);
			deleteList.Add(iter->Key);
			//_mapQuestMonsterId.Remove(iter->Key);
			// ++iter;
		}
	}

	for (int32 deleteId : deleteList)
	{
		_mapQuestMonsterId.Remove(deleteId);
	}

	if (0 < param._tableId.Num())
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_TARGET_MONSTER, &param);
	}
}

// 타겟 목록에서 위치를 삭제한다.
void FGsQuestTargetManagement::RemoveTargetPos(const QuestKey& inQuestKey)
{
	if (0 >= _mapQuestPos.Num())
		return;

	FGsMinimapQuestTargetPosParam param;
	param._isAdd = false;

	TArray<int32> deleteList;
	for (auto iter = _mapQuestPos.CreateConstIterator(); iter; ++iter)
	{
		_mapQuestPos.FindChecked(iter->Key).Remove(&inQuestKey);
		if (0 >= _mapQuestPos.FindChecked(iter->Key).Num())
		{
			param._posData.Add(iter->Key);
			deleteList.Add(iter->Key);
			//_mapQuestPos.Remove(iter->Key);
			// ++iter;
		}
	}

	for (int32 deleteId : deleteList)
	{
		_mapQuestPos.Remove(deleteId);
	}

	if (0 < param._posData.Num())
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_TARGET_POS, &param);
	}
}

void FGsQuestTargetManagement::RemoveTargetProp(const QuestKey& inQuestKey)
{
	if (0 >= _mapQuestPropId.Num())
		return;

	FGsMinimapQuestTargetPropParam param;
	param._isAdd = false;

	TArray<int32> deleteList;
	for (auto iter = _mapQuestPropId.CreateConstIterator(); iter; ++iter)
	{
		_mapQuestPropId.FindChecked(iter->Key).Remove(&inQuestKey);
		if (0 >= _mapQuestPropId.FindChecked(iter->Key).Num())
		{
			param._tableId.Add(iter->Key);
			deleteList.Add(iter->Key);
			//_mapQuestPropId.Remove(iter->Key);
			// ++iter;
		}
	}

	for (int32 deleteId : deleteList)
	{
		_mapQuestPropId.Remove(deleteId);
	}

	if (0 < param._tableId.Num())
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_TARGET_PROP, &param);
	}
}

// 타겟 목록에서 npc를 삭제한다.
void FGsQuestTargetManagement::RemoveTargetNpc(const QuestKey& inQuestKey)
{
	if (0 >= _mapQuestNpcId.Num())
		return;

	FGsMinimapQuestTargetNpcParam param;
	param._isAdd = false;

	TArray<int32> deleteList;
	for (auto iter = _mapQuestNpcId.CreateConstIterator(); iter; ++iter)
	{
		_mapQuestNpcId.FindChecked(iter->Key).Remove(&inQuestKey);
		if (0 >= _mapQuestNpcId.FindChecked(iter->Key).Num())
		{
			param._tableId.Add(iter->Key);
			deleteList.Add(iter->Key);
			//_mapQuestNpcId.Remove(iter->Key);
			// ++iter;
		}
	}

	for (int32 deleteId : deleteList)
	{
		_mapQuestNpcId.Remove(deleteId);
	}

	if (0 < param._tableId.Num())
	{		
		for (auto& iter : _mapQuestSubNpcId)
		{
			if (param._tableId.Contains(iter.Key))
			{
				UpdateTartgetNpcInteractionMark(true, iter.Key, iter.Value);
			}
		}

		if (0 < param._tableId.Num())
		{
			GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_TARGET_NPC, &param);
		}
	}
}

// 퀘스트 몬스터인가
bool FGsQuestTargetManagement::IsQuestMonster(int In_tblId)
{
	return _mapQuestMonsterId.Contains(In_tblId);
}

bool FGsQuestTargetManagement::IsQuestMonster(int In_tblId, OUT QuestType& outType)
{
	outType = QuestType::MAX;
	bool isFind = _mapQuestMonsterId.Contains(In_tblId);
	if (false == isFind)
		return false;

	UGsQuestManager* questMgr = GSQuest();
	if (nullptr == questMgr)
		return false;

	const TSet<const QuestKey*>& QuestKeyList = _mapQuestMonsterId.FindChecked(In_tblId);
	for (const QuestKey* id : QuestKeyList)
	{
		QuestType valueType = questMgr->GetQuestType(id->_questId);
		if (outType > valueType)
		{
			outType = valueType;
		}
	}

	return true;
}

bool FGsQuestTargetManagement::IsActiveQuestMonster(int In_tblId)
{
	bool isFind = _mapQuestMonsterId.Contains(In_tblId);
	if (false == isFind)
		return false;

	UGsQuestManager* questMgr = GSQuest();
	if (nullptr == questMgr)
		return false;

	const TSet<const QuestKey*>& QuestKeyList = _mapQuestMonsterId.FindChecked(In_tblId);
	for (const QuestKey* id : QuestKeyList)
	{
		const QuestKey& activeQueskKey = questMgr->GetAutoProgressQuestKey();
		if (*id == activeQueskKey)
		{
			return true;
		}
	}

	return false;
}

// 퀘스트 npc 인가
bool FGsQuestTargetManagement::IsQuestNpc(int In_tblId)
{	
	return _mapQuestNpcId.Contains(In_tblId);
}

// 퀘스트 prop 인가?
bool FGsQuestTargetManagement::IsQuestProp(int In_tblId)
{
	return _mapQuestPropId.Contains(In_tblId);
}

// 퀘스트 서브 npc 인가?
bool FGsQuestTargetManagement::IsQuestSubNpc(int In_tblId)
{
	if (true == _mapQuestSubNpcId.Contains(In_tblId))
	{
		EQuestNpcState state = _mapQuestSubNpcId.FindRef(In_tblId);
		if (state == EQuestNpcState::STATE_SUB_WATING ||
			state == EQuestNpcState::STATE_PROGRESS || 
			state == EQuestNpcState::STATE_REWARD_WATING)
		{
			if (GGameData()->IsInvadeWorld())
			{
				UGsQuestManager* questMgr = GSQuest();
				if (nullptr == questMgr)
					return false;

				bool invadeQuest = false;
				EQuestNpcState outNpcState;
				TSharedPtr<FGsQuestData> questData = questMgr->GetSubQuestData(In_tblId, outNpcState);
				if (questData.IsValid())
				{					
					return questMgr->InvadeQuest(questData->GetQuestId());
				}
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

bool FGsQuestTargetManagement::GetQuestSubNpcStateIconIndex(int In_tblId, OUT int32& outIndex)
{
	outIndex = 0;

	bool isInvadeWorld = GGameData()->IsInvadeWorld();
	
	if (true == _mapQuestSubNpcId.Contains(In_tblId))
	{
		EQuestNpcState state = _mapQuestSubNpcId.FindRef(In_tblId);
		if (state == EQuestNpcState::STATE_NONE)
			return false;		

		if (isInvadeWorld)
		{
			UGsQuestManager* questMgr = GSQuest();
			if (nullptr == questMgr)
				return false;

			bool invadeQuest = false;
			EQuestNpcState outNpcState;
			TSharedPtr<FGsQuestData> questData = questMgr->GetSubQuestData(In_tblId, outNpcState);
			if (questData.IsValid())
			{
				if (questMgr->InvadeQuest(questData->GetQuestId()))
				{
					if (IsQuestNpc(In_tblId) == true)
					{
						outIndex = 0;
						return true;
					}
					else 
					{
						outIndex = PrivateGetQuestStateConvertToIndex(state);
						return true;
					}
				}
			}
		}
		else
		{
			if (IsQuestNpc(In_tblId) == true)
			{
				outIndex = 0;
				return true;
			}
			else
			{
				outIndex = PrivateGetQuestStateConvertToIndex(state);
				return true;
			}
		}
	}

	return false;
}

int FGsQuestTargetManagement::PrivateGetQuestStateConvertToIndex(EQuestNpcState inState)
{
	int index = 0;
	if (inState != EQuestNpcState::STATE_NONE)
	{
		// 인터랙션 버튼 설정				
		if (inState == EQuestNpcState::STATE_SUB_INVALID_ACCEPT ||
			inState == EQuestNpcState::STATE_SUB_INVALID_MAIN_STORY ||
			inState == EQuestNpcState::STATE_SUB_INVALID_SUB_STORY ||
			inState == EQuestNpcState::STATE_SUB_INVALID_REQUIRE_LEVEL)
			index = 4;
		if (inState == EQuestNpcState::STATE_SUB_WATING)
			index = 2;
		if (inState == EQuestNpcState::STATE_PROGRESS)
			index = 0;
		if (inState == EQuestNpcState::STATE_SUB_ACCEPT)
			index = 1;
		if (inState == EQuestNpcState::STATE_REWARD_WATING)
			index = 3;
	}

	return index;
}