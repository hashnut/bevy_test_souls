// Fill out your copyright notice in the Description page of Project Settings.

#include "GsPartsHandlerBase.h"
#include "Util/GsTableUtil.h"

#include "Data/GsResourceManager.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeData.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/GsSchemaPartsShapeInfo.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/Define/EGsPartsType.h"
#include "Components/SceneComponent.h"


const FGsSchemaPartsShapeInfo* FGsPartsHandlerBase::GetShapeInfo(CreatureGenderType GenderType, int PartsId)
{
	if (const FGsSchemaPartsShapeData* partsTable =
		UGsTableUtil::FindRowById<UGsTablePartsShapeData, FGsSchemaPartsShapeData>(PartsId))
	{
		return partsTable->shapeInfo.Find(GenderType);
	}
	return nullptr;
}

void FGsPartsHandlerBase::Initialize(UGsGameObjectBase* Owner)
{
	_owner = Owner;

	InitPartsLayer(_owner);
}

void FGsPartsHandlerBase::Finalize()
{
	ClearPartsLayer();
}

void FGsPartsHandlerBase::InvalidatePartsData(const UserLookInfo* LookInfoData)
{	
}

void FGsPartsHandlerBase::SetPartsData(const UserLookInfo* LookInfoData)
{
}


void FGsPartsHandlerBase::InitPartsLayer(UGsGameObjectBase* Owner)
{
}

void FGsPartsHandlerBase::AddPartsLayer(EVisibleLayer LayerType, USceneComponent* Component)
{
	if (TArray<USceneComponent*>* findList = _mapPartsLayer.Find(LayerType))
	{
		(*findList).AddUnique(Component);
	}
	else
	{
		TArray<USceneComponent*> newList;
		newList.Emplace(Component);
		_mapPartsLayer.Emplace(LayerType, newList);
	}
}

void FGsPartsHandlerBase::ClearPartsLayer()
{
	_mapPartsLayer.Empty();
}

void FGsPartsHandlerBase::SetVisibleParts(EVisibleLayer Type)
{
	for (auto& el : _mapPartsLayer)
	{
		for (USceneComponent* el2 : el.Value)
		{
			el2->SetVisibility(el.Key == Type);
		}
	}
}

const FGsSchemaPartsShapeData* FGsPartsHandlerBase::GetPartsData(EGsPartsType PartsType)
{
	return _arrPartsData[(int)PartsType];
}