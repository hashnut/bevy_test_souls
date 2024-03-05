/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Cheat/GsSchemaCheatArgu.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCheatSetData.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="무슨일을 하는지 적어주세요", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Cheat/BSGsCheatSetData"))
struct DATACENTER_API FGsSchemaCheatSetData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Command", Tooltip))
	FString Command;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Argument", Tooltip))
	TArray<FGsSchemaCheatArgu> Argument;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Desciption", Tooltip))
	FString Desciption;
	
public:
	FGsSchemaCheatSetData();
	
protected:
	bool operator== (const FGsSchemaCheatSetData& __Other) const
	{
		return FGsSchemaCheatSetData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCheatSetData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCheatSetData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCheatSetDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCheatSetDataRow() : Super()
	{
	}
	
	const FGsSchemaCheatSetData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCheatSetData* Row = nullptr;
		return Table->FindRow<FGsSchemaCheatSetData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCheatSetDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCheatSetDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCheatSetData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Cheat");
	}
	
};
