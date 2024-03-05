#pragma once

#include "CoreMinimal.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Map/WorldMap/GsWorldMapDefine.h"

/*
	미니맵 아이콘 팀 세팅 메시지용 파람
*/

struct FGsMinimapIconTeamMessageParam final : public IGsMessageParam
{
public:
	// 나인가
	bool _isMe = false;
	int64 _gameId;

public:
	FGsMinimapIconTeamMessageParam() = default;
	FGsMinimapIconTeamMessageParam(bool In_isMe, int64 In_gameID) :
		_isMe(In_isMe),_gameId(In_gameID) {}
	virtual ~FGsMinimapIconTeamMessageParam() = default;
};

/*
	미니맵 퀘스트 타겟 파람(npc)
*/

struct FGsMinimapQuestTargetNpcParam final : public IGsMessageParam
{
public:	
	// 추가인지, 삭제인지
	bool _isAdd = false;
	// npc table id array
	TArray<int32> _tableId;

	int _interactionIndex = 0;

public:
	FGsMinimapQuestTargetNpcParam() = default;
	virtual ~FGsMinimapQuestTargetNpcParam() = default;
	FGsMinimapQuestTargetNpcParam(
		bool In_isAdd,
		TArray<int32> In_tblId, int In_InteractionIndex) :
		_isAdd(In_isAdd),
		_tableId(In_tblId),
		_interactionIndex(In_InteractionIndex)
	{
	}
};
/*
	미니맵 퀘스트 타겟 파람(몬스터)
*/

struct FGsMinimapQuestTargetMonsterParam final : public IGsMessageParam
{
public:	
	// 추가인지, 삭제인지
	bool _isAdd = false;
	// 몬스터 테이블 id array
	TArray<int32> _tableId;

public:
	FGsMinimapQuestTargetMonsterParam() = default;
	virtual ~FGsMinimapQuestTargetMonsterParam() = default;
	FGsMinimapQuestTargetMonsterParam(
		bool In_isAdd,
		TArray<int32> In_tblId) : 
		_isAdd(In_isAdd),
		_tableId(In_tblId)
	{
	}
};
/*
	미니맵 퀘스트 타겟 파람(위치)
*/

struct FGsMinimapQuestTargetPosParam final : public IGsMessageParam
{
public:
	// 추가인지, 삭제인지
	bool _isAdd = false;
	// npc 테이블 id(spot id) array
	// 미니맵은 현재 맵만 출력하므로
	// spot id 만 넣는다
	TArray<int> _posData;

public:
	FGsMinimapQuestTargetPosParam() = default;
	virtual ~FGsMinimapQuestTargetPosParam() = default;
	FGsMinimapQuestTargetPosParam(
		bool In_isAdd,
		TArray<int> In_posData) :
		_isAdd(In_isAdd),
		_posData(In_posData)
	{
	}
};

/*
	ui 맵 파티 멤버 추가 삭제
*/

struct FGsUIMapPartyMemberParam final : public IGsMessageParam
{
public:
	// 추가인지, 삭제인지
	bool _isAdd = false;
	// 파티 멤버 game id array
	TArray<int64> _gameId;

public:
	FGsUIMapPartyMemberParam() = default;
	virtual ~FGsUIMapPartyMemberParam() = default;
	FGsUIMapPartyMemberParam(
		bool In_isAdd,
		TArray<int64> In_gameId) :
		_isAdd(In_isAdd),
		_gameId(In_gameId)
	{
	}
};

/*
	미니맵 퀘스트 타겟 파람(프랍)
*/

struct FGsMinimapQuestTargetPropParam final : public IGsMessageParam
{
public:
	// 추가인지, 삭제인지
	bool _isAdd = false;
	// 몬스터 테이블 id array
	TArray<int32> _tableId;

public:
	FGsMinimapQuestTargetPropParam() = default;
	virtual ~FGsMinimapQuestTargetPropParam() = default;
	FGsMinimapQuestTargetPropParam(
		bool In_isAdd,
		TArray<int32> In_tblId) :
		_isAdd(In_isAdd),
		_tableId(In_tblId)
	{
	}
};

/*
	미니맵 퀘스트 서브 인터랙션 npc 파람
*/

struct FGsMinimapQuesSubInterationNpcParam final : public IGsMessageParam
{
public:
	bool _isInterationActive = false;
	// 추가인지, 삭제인지
	bool _isMinimapAdd = false;
	// npc table id array
	int32 _tableId;
	// 인터랙션 아이콘 index
	int _interationIconIndex = 0;

public:
	FGsMinimapQuesSubInterationNpcParam() = default;
	virtual ~FGsMinimapQuesSubInterationNpcParam() = default;
	FGsMinimapQuesSubInterationNpcParam(
		bool In_isInterationActive,
		bool In_isMinimapAdd,
		int32 In_tblId,
		int In_interationIconIndex) :
		_isInterationActive(In_isInterationActive),
		_isMinimapAdd(In_isMinimapAdd),
		_tableId(In_tblId),
		_interationIconIndex(In_interationIconIndex)
	{
	}
};
/*
	맵 시공의 틈 포탈 파람
*/
struct FGsMapSpaceCrackAlarmParam final : public IGsMessageParam
{
public:
	// 추가, 삭제
	bool _isAdd = false;
	
	MapId _regionMapId;	
	FVector _pos;

public:
	FGsMapSpaceCrackAlarmParam() = default;
	virtual ~FGsMapSpaceCrackAlarmParam() = default;
	FGsMapSpaceCrackAlarmParam(MapId In_regionMapId, FVector In_pos):
		_isAdd(true), _regionMapId(In_regionMapId) , _pos(In_pos)
	{		
	}
	FGsMapSpaceCrackAlarmParam(MapId In_regionMapId) :
		_isAdd(false),
		_regionMapId(In_regionMapId)		
	{
	}

};

/* 
* 로컬 플레이어 보물 몬스터
*/
struct FGsMapTreasureMonsterParam final : public IGsMessageParam
{
public:
	// 추가, 삭제
	bool _isAdd{ false };
	int64 _seperator{ 0 };

public:
	FGsMapTreasureMonsterParam() = default;
	virtual ~FGsMapTreasureMonsterParam() = default;
	FGsMapTreasureMonsterParam(bool add, int64 seperator = 0) :
		_isAdd(add), _seperator(seperator)
	{
	}
};

/*
* 침공 포털
*/
struct FGsMapInvasionPortalParam final : public IGsMessageParam
{
public:
	// 추가, 삭제
	bool _isAdd{ false };
	int64   _seperator;
	MapId   _mapId;
	FVector _pos;
	bool	_active{ false };

public:
	FGsMapInvasionPortalParam() = default;
	virtual ~FGsMapInvasionPortalParam() = default;
	FGsMapInvasionPortalParam(int64 seperator, MapId InMapId, FVector InPos, bool active = false) :
		_isAdd(true), _seperator(seperator), _mapId(InMapId), _pos(InPos), _active(active)
	{
	}
};


struct FGsMapInvasionNPCParam final : public IGsMessageParam
{
public:
	// 추가, 삭제
	bool _isAdd = false;
	TArray<EGsRegionInvadeNPCType> _arrayNPCType;

public:
	FGsMapInvasionNPCParam() = default;
	virtual ~FGsMapInvasionNPCParam() = default;
	FGsMapInvasionNPCParam(bool In_isAdd, TArray<EGsRegionInvadeNPCType> In_arrayNPCType) :
		_isAdd(In_isAdd), _arrayNPCType(In_arrayNPCType)
	{
	}
};

/*
* 몬스터 아이템 드랍 위치 인지 개선 관련 아이콘
*/

struct FGsMapMonsterDropIconParam final : public IGsMessageParam
{
public:
	MapId _mapId;
	CreatureId _npcId;
	TArray<SpawnGroupId> _spawnGroupIdArray;
	TArray<FVector> _posArray;

public:
	FGsMapMonsterDropIconParam() = default;
	virtual ~FGsMapMonsterDropIconParam() = default;
	FGsMapMonsterDropIconParam(MapId In_mapId, CreatureId In_npcId, TArray<SpawnGroupId> In_spawnGroupIdArray, TArray<FVector> In_posArray) :
		_mapId(In_mapId), _npcId(In_npcId), _spawnGroupIdArray(In_spawnGroupIdArray), _posArray(In_posArray)
	{
	}
};

/*
	미니맵 자동 이동 경로 아이콘
*/

struct FGsMinimapAutoMovePathParam final : public IGsMessageParam
{
public:
	// 추가인지, 삭제인지
	bool _isAdd = false;
	// 위치 array
	TArray<FVector> _arrayPos;

public:
	FGsMinimapAutoMovePathParam() = default;
	virtual ~FGsMinimapAutoMovePathParam() = default;
	FGsMinimapAutoMovePathParam(
		bool In_isAdd,
		TArray<FVector> In_arrayPos) :
		_isAdd(In_isAdd),
		_arrayPos(In_arrayPos)
	{
	}
};