/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCheatArgu.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Cheat/BSGsCheatArgu"))
struct DATACENTER_API FGsSchemaCheatArgu
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Type", Tooltip))
	EGsCheatArguType Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="TypeInt32Value", Tooltip))
	int32 TypeInt32Value;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="TypefloatValue", Tooltip))
	float TypefloatValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="TypeStringValue", Tooltip))
	FString TypeStringValue;
	
public:
	FGsSchemaCheatArgu();
	
protected:
	bool operator== (const FGsSchemaCheatArgu& __Other) const
	{
		return FGsSchemaCheatArgu::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCheatArgu::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCheatArgu::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCheatArguRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCheatArguRow() : Super()
	{
	}
	
	const FGsSchemaCheatArgu* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCheatArgu* Row = nullptr;
		return Table->FindRow<FGsSchemaCheatArgu>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCheatArguRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCheatArguRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCheatArgu final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Cheat");
	}
	
};
