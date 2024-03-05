/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyEffect.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyBaseEffect.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="fairy base Effect(grade별)", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyBaseEffect"))
struct DATACENTER_API FGsSchemaFairyBaseEffect
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="grade"))
	FairyGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="effect"))
	TArray<FGsSchemaFairyEffect> effect;
public:
	FGsSchemaFairyBaseEffect();
	
protected:
	bool operator== (const FGsSchemaFairyBaseEffect& __Other) const
	{
		return FGsSchemaFairyBaseEffect::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFairyBaseEffect::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFairyBaseEffect::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFairyBaseEffectRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFairyBaseEffectRow() : Super()
	{
	}
	
	const FGsSchemaFairyBaseEffect* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFairyBaseEffect* Row = nullptr;
		return Table->FindRow<FGsSchemaFairyBaseEffect>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFairyBaseEffectRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFairyBaseEffectRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFairyBaseEffect final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Fairy");
	}
	
};
