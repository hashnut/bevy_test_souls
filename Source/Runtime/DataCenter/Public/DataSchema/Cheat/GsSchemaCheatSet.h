/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Cheat/GsSchemaCheatSetData.h"
#include "GsSchemaCheatSet.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="수행할 치트목록관리", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Cheat/BSGsCheatSet"))
struct DATACENTER_API FGsSchemaCheatSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="CommandSet", Tooltip))
	TArray<FGsSchemaCheatSetDataRow> CommandSet;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="설명을 적어요"))
	FString Desc;
	
public:
	FGsSchemaCheatSet();
	
protected:
	bool operator== (const FGsSchemaCheatSet& __Other) const
	{
		return FGsSchemaCheatSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCheatSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCheatSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCheatSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCheatSetRow() : Super()
	{
	}
	
	const FGsSchemaCheatSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCheatSet* Row = nullptr;
		return Table->FindRow<FGsSchemaCheatSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCheatSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCheatSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCheatSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Cheat");
	}
	
};
