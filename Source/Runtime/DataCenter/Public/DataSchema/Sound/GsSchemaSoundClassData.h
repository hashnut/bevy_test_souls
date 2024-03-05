/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class USoundClass;
#include "GsSchemaSoundClassData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Sound/BSGsSoundClassData"))
struct DATACENTER_API FGsSchemaSoundClassData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="type"))
	EGsSoundVolumeType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="soundClass"))
	USoundClass* soundClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="soundClassPath"))
	FSoftObjectPath soundClassPath;
public:
	FGsSchemaSoundClassData();
	
protected:
	bool operator== (const FGsSchemaSoundClassData& __Other) const
	{
		return FGsSchemaSoundClassData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSoundClassData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSoundClassData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSoundClassDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSoundClassDataRow() : Super()
	{
	}
	
	const FGsSchemaSoundClassData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSoundClassData* Row = nullptr;
		return Table->FindRow<FGsSchemaSoundClassData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSoundClassDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSoundClassDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSoundClassData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Sound");
	}
	
};
