/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaVillageQuestBoardListExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/VillageQuest/BSGsVillageQuestBoardListExcel"))
struct DATACENTER_API FGsSchemaVillageQuestBoardListExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
public:
	FGsSchemaVillageQuestBoardListExcel();
	
protected:
	bool operator== (const FGsSchemaVillageQuestBoardListExcel& __Other) const
	{
		return FGsSchemaVillageQuestBoardListExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaVillageQuestBoardListExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaVillageQuestBoardListExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaVillageQuestBoardListExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaVillageQuestBoardListExcelRow() : Super()
	{
	}
	
	const FGsSchemaVillageQuestBoardListExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaVillageQuestBoardListExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaVillageQuestBoardListExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaVillageQuestBoardListExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaVillageQuestBoardListExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableVillageQuestBoardListExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/VillageQuest");
	}
	
};
