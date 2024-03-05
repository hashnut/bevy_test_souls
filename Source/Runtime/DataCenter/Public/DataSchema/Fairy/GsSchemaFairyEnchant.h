/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Fairy/GsSchemaFairyEnchantLevel.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "GsSchemaFairyEnchant.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyEnchant"))
struct DATACENTER_API FGsSchemaFairyEnchant
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade", Tooltip))
	FairyGrade grade;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="materialItemId", Tooltip))
	int32 materialItemId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantLevelList", Tooltip))
	TArray<FGsSchemaFairyEnchantLevel> enchantLevelList;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="level Up UI Effect ID", Tooltip="0은 사용안함"))
	int32 effectIdUp;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="level Down UI Effect ID", Tooltip="0은 사용안함"))
	int32 effectIdDown;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="Keep Level UI Effect ID", Tooltip="0은 사용안함"))
	int32 effectIdKeep;
	
public:
	FGsSchemaFairyEnchant();
	
protected:
	bool operator== (const FGsSchemaFairyEnchant& __Other) const
	{
		return FGsSchemaFairyEnchant::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFairyEnchant::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFairyEnchant::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFairyEnchantRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFairyEnchantRow() : Super()
	{
	}
	
	const FGsSchemaFairyEnchant* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFairyEnchant* Row = nullptr;
		return Table->FindRow<FGsSchemaFairyEnchant>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFairyEnchantRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFairyEnchantRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFairyEnchant final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Fairy");
	}
	
};
