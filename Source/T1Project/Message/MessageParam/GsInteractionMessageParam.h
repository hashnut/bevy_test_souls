#pragma once

#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "GameObject/Interaction/GsInteractionMoveType.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Define/GsGameObjectDefine.h"

/*
	인터랙션 기본 메시지(object)
*/

struct FGsInteractionMessageParam  : public IGsMessageParam
{
	// 타겟
	int64 _targetGameID;	
	EGsGameObjectType _objectType;
	int32 _itemAmount;
	bool _isAutoLooting = true;

	FGsInteractionMessageParam() = default;
	explicit FGsInteractionMessageParam(int64 In_GameID, EGsGameObjectType In_ObjectType, int32 inItemAmount = 0, bool inIsAutoLooting = true) :
		_targetGameID(In_GameID)
		, _objectType(In_ObjectType)
		, _itemAmount(inItemAmount)
		, _isAutoLooting(inIsAutoLooting)
	{
	}
	virtual ~FGsInteractionMessageParam() = default;
};
/*
	인터랙션 컨텐츠들이 알아야할 정보
	클라이언트 스폰여부, NPC TableID, 오브젝트 ID, NPCFuncType
*/
struct FGsInteractionMessageParamContents final : public FGsInteractionMessageParam
{

	int _npcTblId;
	// 스폰id(서버 사용)
	uint32 _spawnId;
	int64 _gameId;
	// 자동 닫기 막는 ui 있나
	bool _isExistWidgetPreventAutoClose = false;

	FGsInteractionMessageParamContents() = default;
	explicit FGsInteractionMessageParamContents(
		int In_npcTblId, uint32 In_spawnId, int64 In_gameId) :	
		_npcTblId(In_npcTblId), _spawnId(In_spawnId), _gameId(In_gameId)
	{
	}

	explicit FGsInteractionMessageParamContents(UGsGameObjectBase* In_target, 
		int In_npcTblId, uint32 In_spawnId, int64 In_gameId, bool In_isExitWidgetPreventAutoClose) :
		FGsInteractionMessageParam(In_gameId,In_target->GetObjectType()),
		_npcTblId(In_npcTblId), _spawnId(In_spawnId), _gameId(In_gameId),
		_isExistWidgetPreventAutoClose(In_isExitWidgetPreventAutoClose)
	{
	}

	virtual ~FGsInteractionMessageParamContents() = default;
};

/*
	Prop타입 인터랙션 정보
*/
struct FGsPropInteractionMessageParamContents final : public FGsInteractionMessageParam
{
	int64 _gameId;
	int _tableId;
	FGsPropInteractionMessageParamContents() = default;
	FGsPropInteractionMessageParamContents(int64 In_gameId, int In_tableId) :
		_gameId(In_gameId), _tableId(In_tableId)
	{
	}
	virtual ~FGsPropInteractionMessageParamContents() = default;
};
