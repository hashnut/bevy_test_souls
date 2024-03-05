// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestObjectiveTableCache.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Quest/GsSchemaObjectiveKillSet.h"
#include "Quest/GsSchemaObjectiveInteractNpcSet.h"
#include "Quest/GsSchemaQuestObjective.h"
#include "Quest/GsSchemaObjectiveGotoPosSet.h"
#include "Quest/GsSchemaObjectiveWarpToInstanceFieldSet.h"
#include "Quest/GsSchemaObjectiveWarpToLastPublicFieldSet.h"
#include "Quest/GsSchemaObjectivePropSet.h"
#include "Quest/GsSchemaObjectiveKillCreatureTypeSet.h"
#include "Quest/GsSchemaObjectiveCraftSet.h"
#include "Quest/GsSchemaObjectiveCompleteQuestTypeSet.h"
#include "Quest/GsSchemaObjectiveCommonSet.h"
#include "Quest/GsSchemaObjectiveKillSpaceCrackSet.h"


FGsQuestObjectiveTableCache::~FGsQuestObjectiveTableCache()
{
	if (nullptr != _objectiveType)
	{
		delete _objectiveType;
	}

	_objectiveType = nullptr;
	_pSchemaQuestObjective = nullptr;
}

void FGsQuestObjectiveTableCache::Initialize()
{
	if (nullptr == _pSchemaQuestObjective)
		return;

	_objectiveType = nullptr;

	switch (_pSchemaQuestObjective->type)
	{
	case QuestObjectiveType::KILL:
		{
			_objectiveType = new FGsObjectiveTypeKill(_objectiveId, GetSchemaObjectiveKillSet());
		}	
		break;
	case  QuestObjectiveType::INTERACT_NPC:
		{
			_objectiveType = new FGsObjectiveTypeInteractNpc(_objectiveId, GetSchemaObjectiveInteractNpcSet());
		}		
		break;
	case QuestObjectiveType::GOTO_POS:
		{
			_objectiveType = new FGsObjectiveTypeGotoPos(_objectiveId, GetSchemaObjectiveGotoPosSet());
		}
		break;
	case QuestObjectiveType::INTERACT_PROP_TOUCH:
		{
			_objectiveType = new FGsObjectiveTypeProp(_objectiveId, GetSchemaObjectivePropSet());
		}
		break;
	case QuestObjectiveType::WARP_TO_INSTANCE_FIELD:
		{
			_objectiveType = new FGsObjectiveTypeWarpToInstanceField(_objectiveId, GetSchemaObjectiveWarpToInstanceFieldSet());
		}
		break;
	case QuestObjectiveType::WARP_TO_LAST_PUBLIC_FIELD:
		{
			_objectiveType = new FGsObjectiveTypeWarpToLastPublicField(_objectiveId, GetSchemaObjectiveWarpToLastPublicFieldSet());
		}
		break;
	case QuestObjectiveType::KILL_CREATURE_TYPE:
		{
			_objectiveType = new FGsObjectiveKillCreatureType(_objectiveId, GetSchemaObjectiveKillCreatureTypeSet());
		}
		break;
	case QuestObjectiveType::ITEM_CRAFT:
		{
			_objectiveType = new FGsObjectiveCraft(_objectiveId, GetSchemaObjectiveCraftSet());
		}
		break;
	case QuestObjectiveType::COMPLETE_QUEST_TYPE:
	case QuestObjectiveType::COMPLETE_STORY_TYPE:
		{
			_objectiveType = new FGsObjectiveCompleteQuestType(_objectiveId, GetObjectiveCompleteQuestTypeSet());
		}
		break;	
	case QuestObjectiveType::KILL_SPACE_CRACK:
	{
		_objectiveType = new FGsObjectiveKillSpaceCrack(_objectiveId, GetObjectiveKillSpaceCrackSet());
	}
	break;
	case QuestObjectiveType::TELEPORT:
	{
		_objectiveType = new FGsObjectiveTypeTeleport(_objectiveId, GetObjectiveTeleportSet());
	}
	break;
	default:
		break;
	}

	int i = static_cast<int>(QuestObjectiveType::BUY_SHOP_ITEM);
	int max = static_cast<int>(QuestObjectiveType::MAX);
	for (; i < max; ++i)
	{
		if (_pSchemaQuestObjective->type == static_cast<QuestObjectiveType>(i))
		{
			_objectiveType = new FGsObjectiveCommonSet(_objectiveId, GetObjectiveCommonSet());
			break;
		}
	}
}

const FGsSchemaObjectiveKillSet* FGsQuestObjectiveTableCache::GetSchemaObjectiveKillSet() const
{
	const FGsSchemaObjectiveKillSet* shemaObjectiveKillSet =
		_pSchemaQuestObjective->typeInfo.GetRowByType<FGsSchemaObjectiveKillSet>();

	return shemaObjectiveKillSet;
}

const FGsSchemaObjectiveInteractNpcSet * FGsQuestObjectiveTableCache::GetSchemaObjectiveInteractNpcSet() const
{
	const FGsSchemaObjectiveInteractNpcSet* shemaObjectiveInteractNpcSet
		= _pSchemaQuestObjective->typeInfo.GetRowByType<FGsSchemaObjectiveInteractNpcSet>();

	return shemaObjectiveInteractNpcSet;
}

const FGsSchemaObjectiveGotoPosSet* FGsQuestObjectiveTableCache::GetSchemaObjectiveGotoPosSet() const
{
	const FGsSchemaObjectiveGotoPosSet* shemaObjectiveGotoPosSet =
		_pSchemaQuestObjective->typeInfo.GetRowByType<FGsSchemaObjectiveGotoPosSet>();

	return shemaObjectiveGotoPosSet;
}

const FGsSchemaObjectivePropSet* FGsQuestObjectiveTableCache::GetSchemaObjectivePropSet() const
{
	const FGsSchemaObjectivePropSet* shemaObjectivePropSet =
		_pSchemaQuestObjective->typeInfo.GetRowByType<FGsSchemaObjectivePropSet>();

	return shemaObjectivePropSet;
}

const FGsSchemaObjectiveWarpToInstanceFieldSet* FGsQuestObjectiveTableCache::GetSchemaObjectiveWarpToInstanceFieldSet() const
{
	const FGsSchemaObjectiveWarpToInstanceFieldSet* shemaObjectiveWarpToInstanceFieldSet =
		_pSchemaQuestObjective->typeInfo.GetRowByType<FGsSchemaObjectiveWarpToInstanceFieldSet>();

	return shemaObjectiveWarpToInstanceFieldSet;
}

const FGsSchemaObjectiveWarpToLastPublicFieldSet* FGsQuestObjectiveTableCache::GetSchemaObjectiveWarpToLastPublicFieldSet() const
{
	const FGsSchemaObjectiveWarpToLastPublicFieldSet* shemaObjectiveWarpToLastPublicFieldSet =
		_pSchemaQuestObjective->typeInfo.GetRowByType<FGsSchemaObjectiveWarpToLastPublicFieldSet>();
	
	return shemaObjectiveWarpToLastPublicFieldSet;
}

const FGsSchemaObjectiveKillCreatureTypeSet* FGsQuestObjectiveTableCache::GetSchemaObjectiveKillCreatureTypeSet() const
{
	const FGsSchemaObjectiveKillCreatureTypeSet* shemaObjectiveKillCreatureTypeSet =
		_pSchemaQuestObjective->typeInfo.GetRowByType<FGsSchemaObjectiveKillCreatureTypeSet>();

	return shemaObjectiveKillCreatureTypeSet;
}

const FGsSchemaObjectiveCraftSet* FGsQuestObjectiveTableCache::GetSchemaObjectiveCraftSet() const
{
	const FGsSchemaObjectiveCraftSet* shemaObjectivCraftSet =
		_pSchemaQuestObjective->typeInfo.GetRowByType<FGsSchemaObjectiveCraftSet>();

	return shemaObjectivCraftSet;
}

const FGsSchemaObjectiveCompleteQuestTypeSet* FGsQuestObjectiveTableCache::GetObjectiveCompleteQuestTypeSet() const
{
	const FGsSchemaObjectiveCompleteQuestTypeSet* shemaObjectivCompleteQuestTypeSet =
		_pSchemaQuestObjective->typeInfo.GetRowByType<FGsSchemaObjectiveCompleteQuestTypeSet>();

	return shemaObjectivCompleteQuestTypeSet;
}

const FGsSchemaObjectiveKillSpaceCrackSet* FGsQuestObjectiveTableCache::GetObjectiveKillSpaceCrackSet() const
{
	const FGsSchemaObjectiveKillSpaceCrackSet* shemaObjectivKillSpaceCrackSet =
		_pSchemaQuestObjective->typeInfo.GetRowByType<FGsSchemaObjectiveKillSpaceCrackSet>();

	return shemaObjectivKillSpaceCrackSet;
}

const FGsSchemaObjectiveTeleportSet* FGsQuestObjectiveTableCache::GetObjectiveTeleportSet() const
{
	const FGsSchemaObjectiveTeleportSet* shemaObjectiveTeleportSet =
		_pSchemaQuestObjective->typeInfo.GetRowByType<FGsSchemaObjectiveTeleportSet>();

	return shemaObjectiveTeleportSet;
}

const FGsSchemaObjectiveCommonSet* FGsQuestObjectiveTableCache::GetObjectiveCommonSet() const
{
	const FGsSchemaObjectiveCommonSet* shemaObjectivCommonSet =
		_pSchemaQuestObjective->typeInfo.GetRowByType<FGsSchemaObjectiveCommonSet>();

	return shemaObjectivCommonSet;
}

const FGsSchemaQuestObjective * FGsQuestObjectiveTableCache::GetSchemaQuestObjective() const
{
	return _pSchemaQuestObjective;
}

/**
 *	FGsObjectiveTypeData
 */
FGsObjectiveTypeBase::FGsObjectiveTypeBase(QuestObjectiveId inObjectiveId) : _typeInfoId(inObjectiveId)
{
}


/**
 *	FGsObjectiveTypeKillData
 */
FGsObjectiveTypeKill::FGsObjectiveTypeKill(
	QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveKillSet * inSchemaObjectiveKill)
	: FGsObjectiveTypeBase(inTypeInfoId), _schemaObjectiveKill(inSchemaObjectiveKill)
{
}


/**
 *	FGsObjectiveTypeInteractNpcData
 */
FGsObjectiveTypeInteractNpc::FGsObjectiveTypeInteractNpc(
	QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveInteractNpcSet * inSchemaObjectiveInteractNpc)
	: FGsObjectiveTypeBase(inTypeInfoId), _schemaObjectiveInteractNpc(inSchemaObjectiveInteractNpc)
{
}

/**
 *	FGsObjectiveTypeGotoPosData
 */
FGsObjectiveTypeGotoPos::FGsObjectiveTypeGotoPos(QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveGotoPosSet * inSchemaObjectiveGotoPos)
	: FGsObjectiveTypeBase(inTypeInfoId), _schemaObjectiveGotoPos(inSchemaObjectiveGotoPos)
{
}

/**
 *	FGsObjectiveTypePropData
 */
FGsObjectiveTypeProp::FGsObjectiveTypeProp(
	QuestObjectiveId inTypeInfoId, const FGsSchemaObjectivePropSet * inSchemaObjectiveProp)
	: FGsObjectiveTypeBase(inTypeInfoId), _schemaObjectiveProp(inSchemaObjectiveProp)
{
}

/**
 *	FGsObjectiveTypeWarpToInstanceFieldData
 */
 FGsObjectiveTypeWarpToInstanceField::FGsObjectiveTypeWarpToInstanceField(
	 QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveWarpToInstanceFieldSet * inSchemaObjectiveWarpToInstanceField)
	 : FGsObjectiveTypeBase(inTypeInfoId), _schemaObjectiveWarpToInstanceField(inSchemaObjectiveWarpToInstanceField)
 {
 }

 /**
 *	FGsObjectiveTypeWarpToLastPublicFieldData
 */
 FGsObjectiveTypeWarpToLastPublicField::FGsObjectiveTypeWarpToLastPublicField(
	 QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveWarpToLastPublicFieldSet * inSchemaObjectiveWarpToLastPublicField)
	 : FGsObjectiveTypeBase(inTypeInfoId), _schemaObjectiveWarpToLastPublicField(inSchemaObjectiveWarpToLastPublicField)
 {
 }

 /**
 *	FGsObjectiveKillCreatureTypeData
 */
 FGsObjectiveKillCreatureType::FGsObjectiveKillCreatureType(
	 QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveKillCreatureTypeSet* inSchemaObjectiveKillCreatureType)
	 : FGsObjectiveTypeBase(inTypeInfoId), _schemaObjectiveKillCreatureType(inSchemaObjectiveKillCreatureType)
 {
 }

/**
*	FGsObjectiveCraftData
*/
 FGsObjectiveCraft::FGsObjectiveCraft(
	 QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveCraftSet* inSchemaObjectiveCraft)
	 : FGsObjectiveTypeBase(inTypeInfoId), _schemaObjectiveCraft(inSchemaObjectiveCraft)
 {
 }

/**
*	FGsObjectiveCompleteQuestTypeData
*/
 FGsObjectiveCompleteQuestType::FGsObjectiveCompleteQuestType(
	 QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveCompleteQuestTypeSet* inSchemaObjectiveCompleteQuestType)
	 : FGsObjectiveTypeBase(inTypeInfoId), _schemaObjectiveCompleteQuestType(inSchemaObjectiveCompleteQuestType)
 {
 }

 /**
*	FGsObjectiveKillSpaceCrack
*/
 FGsObjectiveKillSpaceCrack::FGsObjectiveKillSpaceCrack(
	 QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveKillSpaceCrackSet* inSchemaObjectiveKillSpaceCrack)
	 : FGsObjectiveTypeBase(inTypeInfoId), _schemaObjectiveKillSpaceCrack(inSchemaObjectiveKillSpaceCrack)
 {
 }

 /**
*	FGsObjectiveTeleport
*/
 FGsObjectiveTypeTeleport::FGsObjectiveTypeTeleport(
	 QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveTeleportSet* inSchemaObjectiveTypeTeleportSet)
	 : FGsObjectiveTypeBase(inTypeInfoId), _schemaObjectiveTeleport(inSchemaObjectiveTypeTeleportSet)
 {
 }

 /**
*	FGsObjectiveCommonSetData
*/
 FGsObjectiveCommonSet::FGsObjectiveCommonSet(
	 QuestObjectiveId inTypeInfoId, const FGsSchemaObjectiveCommonSet* inSchemaObjectiveCommonSet)
	 : FGsObjectiveTypeBase(inTypeInfoId), _schemaObjectiveCommonSet(inSchemaObjectiveCommonSet)
 {
 }
