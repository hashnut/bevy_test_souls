// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritoryDataManager.h"
#include "DataSchema/Map/Line/GsSchemaLineData.h"
#include "DataSchema/Map/Line/GsSchemaLineElement.h"

//t1 project


/**
 * 
 */
class TERRITORYEDITOR_API FGsTerritoryLineData : public FGsTerritoryDataBase
{
private:
	FString _dataTableName;
	TWeakObjectPtr<UGsTable> _table;

public:
	FGsTerritoryLineData();
	virtual ~FGsTerritoryLineData();

public:
	virtual bool Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inLevel, const FString& inLevelKey, bool inSaveForce=false) override;

protected:		
	virtual void Export(const FString& inLevelKey) override;	

private:
	bool IsNeedToSaveLineData(const FString& inLevelKey, const TArray<FGsSchemaLineElement>& inLineElementDataArray);
	void SaveLineData(const FString& inLevelKey, const TArray<FGsSchemaLineElement>& inSpawnDataArray);
};

//이미 pointer type이 기본으로 != operator 타입에 정의 되었기 때문에 비교를 하려면 참조 형태로 비교하도록 해야한다.
static bool operator != (const FGsSchemaLineElement& inSource, const FGsSchemaLineElement& inDest)
{
	if (inSource.ShapeType != inDest.ShapeType
	|| inSource.isReturn != inDest.isReturn
	|| inSource.isRepeat != inDest.isRepeat
	|| inSource.repeatDelay != inDest.repeatDelay
	|| inSource.isSuicide != inDest.isSuicide
	|| inSource.Distance != inDest.Distance
	|| inSource.Radius != inDest.Radius
	|| inSource.LineId != inDest.LineId
	|| inSource.Center != inDest.Center
	|| inSource.isReverse != inDest.isReverse)
	{
		return true;
	}

	int32 num = inSource.VertextList.Num();
	if (num != inDest.VertextList.Num())
	{
		return true;
	}

	for (int32 i = 0; i < num; ++i)
	{	
		if (inSource.VertextList[i] != inDest.VertextList[i])
		{
			return true;
		}
	}

	return false;
}

static bool operator != (const FGsSchemaLineData& inSource, const FGsSchemaLineData& inDest)
{	
	if (inSource.MapId != inDest.MapId)
	{
		return true;
	}

	if (inSource.LineList.Num() != inDest.LineList.Num())
	{
		return true;
	}
	
	int32 num = inSource.LineList.Num();
	for (int i = 0; i < num; ++i)
	{		
		if (inSource.LineList[i] != inDest.LineList[i])
		{
			return true;
		}
	}

	return false;
}