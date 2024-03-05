// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"

//T1 Project
#include "Level/Territory/GsTerritoryZoneDefine.h"
#include "GsTerritoryUniqueTable.generated.h"

//Territory unique id type is unsigned int32 type in server

//uint32 10 space
//object 3 space
//can use space 


//uint32 unique id max 4294967295
//uint8 object type 255
const uint32 TERRITORY_UNIQUE_ID_FACT = 10000000;

 /**
 * Define for Territory unique table
 */

USTRUCT(BlueprintType)
struct TERRITORYEDITOR_API FGsTerritoryUniqueIDRange
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory unique range")
		int64 Min;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory unique range")
		int64 Max;

public:
	bool InvalideRange() { return Max <= Min; }
	const FString ToString() const;
	FString ToCSV();
};

USTRUCT(BlueprintType)
struct TERRITORYEDITOR_API FGsTerritoryUniqueIDRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level script actor")
		EGsTerritoryObjType ObjType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level script actor", meta = (ShowOnlyInnerProperties))
		FGsTerritoryUniqueIDRange Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level script actor")
		int64 Count = 0;

public:
	bool InvalideRange(FString& out_log);
	bool InvalideCount(FString& out_log);	
	bool TryCheckOverlapRange(const FGsTerritoryUniqueIDRow& in_range, FString& out_log);
	bool TryGetUniqueID(int64& out_uniuqe);
	void Set(EGsTerritoryObjType in_obj, FGsTerritoryUniqueIDRange in_range, int64 in_count);
	void Set(EGsTerritoryObjType inObj, int64 inMin, int64 inMax, int64 inCount);

public:
	FString ToString();
	FString ToCSV();
	static FString GetCSVHeader();
};

USTRUCT(BlueprintType)
struct TERRITORYEDITOR_API FGsTerritoryUniqueIDCaution
{
	GENERATED_BODY()

public:
	UPROPERTY()
		EGsTerritoryObjType ObjType;
	UPROPERTY()
		int32 Index;
};