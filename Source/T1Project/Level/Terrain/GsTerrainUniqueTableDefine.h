// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "GsTerrainShapeDefine.h"
#include "GsTerrainZoneDefine.h"
#include "GsTerrainUniqueTableDefine.generated.h"

/**
 *
 */
 //unsigned int32 4,294,967,29
const uint32 MAX_TERRAIN_UNIQUE_ID = 4294967295;
const uint32 MAX_TERRAIN_OBJ_ID = 255;
const uint32 TERRAIN_UNIQUE_ID_FACT = 10000000;

USTRUCT(BlueprintType)
struct T1PROJECT_API FGsTerrainUniqueRange
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain unique range")
		int64 Min;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain unique range")
		int64 Max;

public:
	bool InvalideRange() { return Max <= Min; }
	const FString ToString() const;	
};

USTRUCT(BlueprintType)
struct T1PROJECT_API FGsTerrainUnique
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level script actor")
		ETerrainObjType ObjType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level script actor", meta = (ShowOnlyInnerProperties))
		FGsTerrainUniqueRange Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level script actor")
		int64 Count = 0;

public:
	bool InvalideRange();
	bool InvalideRange(FString& out_log);
	bool InvalideCount(FString& out_log);	
	bool TryCheckOverlapRange(const FGsTerrainUnique* in_range);
	bool TryCheckOverlapRange(const FGsTerrainUnique& in_range, FString& out_log);
	bool TryGetUniqueID(int64& out_uniuqe);	
	void Set(ETerrainObjType in_obj, FGsTerrainUniqueRange in_range, int64 in_count);
	FString ToString();
};

USTRUCT(BlueprintType)
struct T1PROJECT_API FGsTerrainUniqueSchema
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level script actor", meta=(AllowedClasses="UWorld"))
		FSoftObjectPath _map;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level script actor")
		TArray<FGsTerrainUnique> _uniqueArray;
};

USTRUCT(BlueprintType)
struct T1PROJECT_API FGsTerrainCaution
{
	GENERATED_BODY()

public:
	UPROPERTY()
		ETerrainObjType ObjType;
	UPROPERTY()
		int32 Index;
};
