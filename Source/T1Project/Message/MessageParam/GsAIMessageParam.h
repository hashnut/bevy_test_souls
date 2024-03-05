#pragma once
#include "CoreMinimal.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "AI/Data/GsAIDefine.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

class UGsGameObjectBase;

/*
	ai 퀘스트 메시지
*/
struct FGsAIQuestMessageParam : public IGsMessageParam
{
	// 퀘스트 id
	QuestId _questId;
	// 이동 타입
	QuestObjectiveType _moveType;
	// 서버 전송하기 위한 스폰 id(npc meet에만 쓴다)
	uint32 _spawnId;
	int64 _gameId;

	FGsAIQuestMessageParam() = default;
	explicit FGsAIQuestMessageParam(QuestId In_questId,
		QuestObjectiveType In_moveType, uint32 In_spawnId = 0, int64 In_gameId = 0) :
		_questId(In_questId), _moveType(In_moveType), 
		_spawnId(In_spawnId), _gameId(In_gameId)
	{
	}
	virtual ~FGsAIQuestMessageParam() = default;
};

/*
	ai 퀘스트 자동 이동 메시지
*/
struct FGsAIQuestAutoMoveMessageParam final : public FGsAIQuestMessageParam
{
	// npc id
	int _npcId;
	// 갈 map id
	int _mapId;
	TArray<FVector> _arrPos;
	float _range;

	FGsAIQuestAutoMoveMessageParam() = default;
	explicit FGsAIQuestAutoMoveMessageParam(
		int In_mapId, TArray<FVector> In_arrPos, QuestId In_questId, QuestObjectiveType In_moveType, int In_npcId, float In_range) :
		FGsAIQuestMessageParam(In_questId, In_moveType),
		_npcId(In_npcId),
		_mapId(In_mapId),
		_arrPos(In_arrPos),
		_range(In_range)
	{
	}
	virtual ~FGsAIQuestAutoMoveMessageParam() = default;
};
/*
	ai 퀘스트 npc 
*/
struct FGsAIQuestNpcParam final : public IGsMessageParam
{
	// npc id
	int _npcId;
	// map id
	int _mapId;
	// 위치
	FVector _pos;
	// 노드 중간에 들어갈지(현기획은 no, 무조건 첫번째 부터)
	bool _isCenterNode;

	FGsAIQuestNpcParam() = default;
	explicit FGsAIQuestNpcParam(int In_npcId, int In_mapId,
		FVector In_pos, bool In_isCenterNode) :
		_npcId(In_npcId), _mapId(In_mapId),_pos( In_pos), _isCenterNode(In_isCenterNode)
	{
	}
	virtual ~FGsAIQuestNpcParam() = default;
};
// 자동 이동으로 다른맵 갈때 워프 정보
struct FGsAIMoveNextMapWarpInfoParam final : public IGsMessageParam
{
	FVector _pos;
	FGsAIMoveNextMapWarpInfoParam() = default;
	explicit FGsAIMoveNextMapWarpInfoParam(FVector In_pos):_pos(In_pos)
	{
	}
	virtual ~FGsAIMoveNextMapWarpInfoParam() = default;
};

// reserve job param
struct FGsAIReserveParam final : public IGsMessageParam
{
	EGsAIActionType _actionType;
	UGsGameObjectBase* _targetObj;
	int _addIndex;
	
	FGsAIReserveParam() = default;
	explicit FGsAIReserveParam(EGsAIActionType In_actionType) :
		_actionType(In_actionType)
	{
	}
	explicit FGsAIReserveParam(EGsAIActionType In_actionType, 
		int In_addIndex) :
		_actionType(In_actionType),
		_addIndex(In_addIndex)
	{
	}
	explicit FGsAIReserveParam(EGsAIActionType In_actionType,
		UGsGameObjectBase* In_targetObj) :
		_actionType(In_actionType),
		_targetObj(In_targetObj)
	{
	}
	explicit FGsAIReserveParam(EGsAIActionType In_actionType,
		UGsGameObjectBase* In_targetObj,
		int In_addIndex) :
		_actionType(In_actionType),
		_targetObj(In_targetObj),
		_addIndex(In_addIndex)
	{
	}

	virtual ~FGsAIReserveParam() = default;
};
// 자동 이동 타입 변경 파람
struct FGsAutoMoveContentsChangedParam final : public IGsMessageParam
{
	// 변경 전 타입
	EGsMovementAutoContentsType _oldContentsType;
	// 변경 후 타입
	EGsMovementAutoContentsType _newContentsType;

	FGsAutoMoveContentsChangedParam() = default;
	explicit FGsAutoMoveContentsChangedParam(EGsMovementAutoContentsType In_oldType, 
		EGsMovementAutoContentsType In_newType) :
		_oldContentsType(In_oldType),
		_newContentsType(In_newType)
	{
	}

	virtual ~FGsAutoMoveContentsChangedParam() = default;
};