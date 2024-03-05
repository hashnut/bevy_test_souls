// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Quest/GsSchemaQuestObjective.h"
#include "Quest/GsSchemaObjectiveKillSet.h"
#include "Quest/GsSchemaObjectiveInteractNpcSet.h"
#include "Quest/GsSchemaObjectiveGotoPosSet.h"
#include "Quest/GsSchemaObjectiveWarpToInstanceFieldSet.h"
#include "Quest/GsSchemaObjectiveWarpToLastPublicFieldSet.h"
#include "Quest/GsSchemaObjectivePropSet.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Quest/GsSchemaObjectiveKillCreatureTypeSet.h"
#include "Quest/GsSchemaObjectiveCraftSet.h"
#include "Quest/GsSchemaObjectiveCompleteQuestTypeSet.h"
#include "Quest/GsSchemaObjectiveCommonSet.h"
#include "Quest/GsSchemaObjectiveKillSpaceCrackSet.h"
#include "Quest/GsSchemaObjectiveTeleportSet.h"

struct FGsSchemaQuest;
struct FGsSchemaObjectiveKillSet;
struct FGsSchemaObjectiveInteractNpcSet;
struct FGsSchemaObjectiveGotoPosSet;
struct FGsSchemaQuestObjective;
struct FGsSchemaObjectiveWarpToInstanceFieldSet;
struct FGsSchemaObjectiveWarpToLastPublicFieldSet;
struct FGsSchemaObjectivePropSet;
struct FGsSchemaObjectiveCompleteQuestTypeSet;
struct FGsSchemaObjectiveCraftSet;
struct FGsSchemaObjectiveKillCreatureTypeSet;
struct FGsSchemaObjectiveCommonSet;
struct FGsSchemaObjectiveKillSpaceCrackSet;

/**
 * 오브젝티브 타입 Data (kill, InteractNpc....)
 */
class T1PROJECT_API FGsObjectiveTypeBase
{
protected:
	QuestObjectiveId _typeInfoId;

public:
	FGsObjectiveTypeBase(QuestObjectiveId inTypeInfoId);
	virtual ~FGsObjectiveTypeBase() = default;

public:
	virtual int32 GetValue() { return 1; }
};

/**
 *  오브젝티브 kill Type Data
 */
class T1PROJECT_API FGsObjectiveTypeKill : public FGsObjectiveTypeBase
{
private:
	const FGsSchemaObjectiveKillSet* _schemaObjectiveKill;

public:
	FGsObjectiveTypeKill(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveKillSet* inSchemaObjectiveKill);

public:
	const FGsSchemaObjectiveKillSet* GetSchemaObjectiveKill() const { return _schemaObjectiveKill; }
	virtual int32 GetValue() override { return _schemaObjectiveKill? _schemaObjectiveKill->count : 0; }
};

/**
 *	오브젝티브 InteractNpc Type Data
 */
class T1PROJECT_API FGsObjectiveTypeInteractNpc : public FGsObjectiveTypeBase
{
private:
	const FGsSchemaObjectiveInteractNpcSet* _schemaObjectiveInteractNpc;

public:
	FGsObjectiveTypeInteractNpc(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveInteractNpcSet* inSchemaObjectiveInteractNpc);

public:
	const FGsSchemaObjectiveInteractNpcSet* GetSchemaObjectiveInteractNpc() const { return _schemaObjectiveInteractNpc; }
};

/**
 *	오브젝티브 GoToPos Type Data
 */
class T1PROJECT_API FGsObjectiveTypeGotoPos : public FGsObjectiveTypeBase
{
private:
	const FGsSchemaObjectiveGotoPosSet* _schemaObjectiveGotoPos;

public:
	FGsObjectiveTypeGotoPos(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveGotoPosSet* inSchemaObjectiveGotoPos);

public:
	const FGsSchemaObjectiveGotoPosSet* GetSchemaObjectiveGotoPos() const { return _schemaObjectiveGotoPos; }
};

/**
 *  오브젝티브 Prop Type Data
 */
class T1PROJECT_API FGsObjectiveTypeProp : public FGsObjectiveTypeBase
{
private:
	const FGsSchemaObjectivePropSet* _schemaObjectiveProp;

public:
	FGsObjectiveTypeProp(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectivePropSet* inSchemaObjectiveProp);

public:
	const FGsSchemaObjectivePropSet* GetSchemaObjectiveProp() const { return _schemaObjectiveProp; }
	virtual int32 GetValue() override { return _schemaObjectiveProp? _schemaObjectiveProp->count : 0; }
};

/**
 *	오브젝티브 WarpToInstanceField Type Data
 */
class T1PROJECT_API FGsObjectiveTypeWarpToInstanceField : public FGsObjectiveTypeBase
{
private:
	const FGsSchemaObjectiveWarpToInstanceFieldSet* _schemaObjectiveWarpToInstanceField;

public:
	FGsObjectiveTypeWarpToInstanceField(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveWarpToInstanceFieldSet* inSchemaObjectiveWarpToInstanceField);

public:
	const FGsSchemaObjectiveWarpToInstanceFieldSet* GetSchemaObjectiveWarpToInstanceField() const { return _schemaObjectiveWarpToInstanceField; }
};

/**
 *	오브젝티브 WarpToLastPublicField Type Data
 */
class T1PROJECT_API FGsObjectiveTypeWarpToLastPublicField : public FGsObjectiveTypeBase
{
private:
	const FGsSchemaObjectiveWarpToLastPublicFieldSet* _schemaObjectiveWarpToLastPublicField;

public:
	FGsObjectiveTypeWarpToLastPublicField(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveWarpToLastPublicFieldSet* inSchemaObjectiveWarpToLastPublicField);

public:
	const FGsSchemaObjectiveWarpToLastPublicFieldSet* GetSchemaObjectiveWarpToLastPublicField() const { return _schemaObjectiveWarpToLastPublicField; }
};

/**
 *  오브젝티브 ObjectiveKillCreature Type Data
 */
class T1PROJECT_API FGsObjectiveKillCreatureType : public FGsObjectiveTypeBase
{
private:
	const FGsSchemaObjectiveKillCreatureTypeSet* _schemaObjectiveKillCreatureType;

public:
	FGsObjectiveKillCreatureType(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveKillCreatureTypeSet* inSchemaObjectiveKillCreature);

public:
	const FGsSchemaObjectiveKillCreatureTypeSet* GetSchemaObjectiveKillCreatureTypeSet() const { return _schemaObjectiveKillCreatureType; }
	virtual int32 GetValue() override { return _schemaObjectiveKillCreatureType? _schemaObjectiveKillCreatureType->count : 0; }
};

/**
 *  오브젝티브 ObjectiveCraft Type Data
 */
class T1PROJECT_API FGsObjectiveCraft : public FGsObjectiveTypeBase
{
private:
	const FGsSchemaObjectiveCraftSet* _schemaObjectiveCraft;

public:
	FGsObjectiveCraft(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveCraftSet* inSchemaObjectiveCraft);

public:
	const FGsSchemaObjectiveCraftSet* GetSchemaObjectiveCraftSet() const { return _schemaObjectiveCraft; }
	virtual int32 GetValue() override { return _schemaObjectiveCraft? _schemaObjectiveCraft->itemCount : 0; }
};

/**
 *  오브젝티브 CompleteQuest Type Data
 */
class T1PROJECT_API FGsObjectiveCompleteQuestType : public FGsObjectiveTypeBase
{
private:
	const FGsSchemaObjectiveCompleteQuestTypeSet* _schemaObjectiveCompleteQuestType;

public:
	FGsObjectiveCompleteQuestType(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveCompleteQuestTypeSet* inSchemaObjectiveCompleteQuestType);

public:
	const FGsSchemaObjectiveCompleteQuestTypeSet* GetObjectiveCompleteQuestTypeSet() const { return _schemaObjectiveCompleteQuestType; }
	virtual int32 GetValue() override { return _schemaObjectiveCompleteQuestType? _schemaObjectiveCompleteQuestType->count : 0; }
};

/**
 *  오브젝티브 KillSpaceCrack Data
 */
class T1PROJECT_API FGsObjectiveKillSpaceCrack : public FGsObjectiveTypeBase
{
private:
	const FGsSchemaObjectiveKillSpaceCrackSet* _schemaObjectiveKillSpaceCrack;

public:
	FGsObjectiveKillSpaceCrack(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveKillSpaceCrackSet* inSchemaObjectiveKillSpaceCrack);

public:
	const FGsSchemaObjectiveKillSpaceCrackSet* GetObjectiveKillSpaceCrackSet() const { return _schemaObjectiveKillSpaceCrack; }
	virtual int32 GetValue() override { return _schemaObjectiveKillSpaceCrack? _schemaObjectiveKillSpaceCrack->count : 0; }
};

/**
 *	오브젝티브 TELEPORT Type Data
 */
class T1PROJECT_API FGsObjectiveTypeTeleport : public FGsObjectiveTypeBase
{
private:
	const FGsSchemaObjectiveTeleportSet* _schemaObjectiveTeleport;

public:
	FGsObjectiveTypeTeleport(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveTeleportSet* inSchemaObjectiveTeleport);

public:
	const FGsSchemaObjectiveTeleportSet* GetSchemaObjectiveTeleport() const { return _schemaObjectiveTeleport; }
};

/**
 *  오브젝티브 CommonSet Type Data
 */
class T1PROJECT_API FGsObjectiveCommonSet : public FGsObjectiveTypeBase
{
private:
	const FGsSchemaObjectiveCommonSet* _schemaObjectiveCommonSet;

public:
	FGsObjectiveCommonSet(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveCommonSet* inSchemaObjectiveCommonSet);

public:
	const FGsSchemaObjectiveCommonSet* GetObjectiveCommonSet() const { return _schemaObjectiveCommonSet; }
	virtual int32 GetValue() override
	{ 
		int outValue = 0;
		if (nullptr == _schemaObjectiveCommonSet)
			return outValue;
		
		if (!_schemaObjectiveCommonSet->value2.IsEmpty())
		{
			outValue = FCString::Atoi(*_schemaObjectiveCommonSet->value2);
		}
		return outValue;
	}
};

/**
 *  오브젝티브 TableCache
 */
class T1PROJECT_API FGsQuestObjectiveTableCache
{
private:
	QuestObjectiveId _objectiveId;
	FGsObjectiveTypeBase* _objectiveType = nullptr;
	const FGsSchemaQuestObjective* _pSchemaQuestObjective = nullptr;

public:
	explicit FGsQuestObjectiveTableCache(QuestObjectiveId inObjectiveId, const FGsSchemaQuestObjective* inSchemaObjective)
		:_objectiveId(inObjectiveId), _pSchemaQuestObjective(inSchemaObjective){}
	~FGsQuestObjectiveTableCache();

public:
	void Initialize();

public:
	// 킬 타입 스키마
	const FGsSchemaObjectiveKillSet* GetSchemaObjectiveKillSet() const;
	// InteractNpc 타입 스키마
	const FGsSchemaObjectiveInteractNpcSet* GetSchemaObjectiveInteractNpcSet() const;
	// GotoPos 타입 스키마
	const FGsSchemaObjectiveGotoPosSet* GetSchemaObjectiveGotoPosSet() const;
	// Prop 타입 스키마
	const FGsSchemaObjectivePropSet* GetSchemaObjectivePropSet() const;
	// WarpToInstanceField 타입 스키마
	const FGsSchemaObjectiveWarpToInstanceFieldSet* GetSchemaObjectiveWarpToInstanceFieldSet() const;
	// WarpToLastPublicField 타입 스키마
	const FGsSchemaObjectiveWarpToLastPublicFieldSet* GetSchemaObjectiveWarpToLastPublicFieldSet() const;
	// KillCreatureType 타입 스키마
	const FGsSchemaObjectiveKillCreatureTypeSet* GetSchemaObjectiveKillCreatureTypeSet() const;
	// Craft 타입 스키마
	const FGsSchemaObjectiveCraftSet* GetSchemaObjectiveCraftSet() const;
	// CompleteQuestType 타입 스키마
	const FGsSchemaObjectiveCompleteQuestTypeSet* GetObjectiveCompleteQuestTypeSet() const;
	// KillSpaceCrack 타입 스키마
	const FGsSchemaObjectiveKillSpaceCrackSet* GetObjectiveKillSpaceCrackSet() const;
	// Teleport 타입 스키마
	const FGsSchemaObjectiveTeleportSet* GetObjectiveTeleportSet() const;
	// Common 타입 스키마
	const FGsSchemaObjectiveCommonSet* GetObjectiveCommonSet() const;

	// Objective 스키마
	const FGsSchemaQuestObjective* GetSchemaQuestObjective() const;	
	
	// 오브젝티브 타입
	const QuestObjectiveType GetObjectiveType() const { 
		return (_pSchemaQuestObjective)?_pSchemaQuestObjective->type : QuestObjectiveType::KILL;
	}
	
	// 수행 목표 (아무개를 잡아라. 아무개를 만나라....)
	const FText& GetDescText() const {
		return (_pSchemaQuestObjective) ? _pSchemaQuestObjective->descStringId : FText::GetEmpty(); 
	}
	
	// 오브젝티브ID
	QuestObjectiveId GetObjectiveId() const { return _objectiveId; }
	
	// 몬스터 킬수, 수행카운트...
	int32 GetObjectiveValue() const { return (nullptr != _objectiveType)? _objectiveType->GetValue() : 0; }
	
	//// ui 카운트 연출 유무
	//bool IsShowUIObjectiveCount() const {
	//	return (_objectiveType)? _objectiveType->IsShowUIObjectiveCount() : false; 
	//}

	// 티컷 연출 유무
	bool GetisTickerHide() const {
		return (_pSchemaQuestObjective) ? _pSchemaQuestObjective->isTickerHide : false;
	}

	// ui 카운트 연출 유무
	bool GetisCountHide() const {
		return (_pSchemaQuestObjective) ? _pSchemaQuestObjective->isCountHide : false;
	}
};
