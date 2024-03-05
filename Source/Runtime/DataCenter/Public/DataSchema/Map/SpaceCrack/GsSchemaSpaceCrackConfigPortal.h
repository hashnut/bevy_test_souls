/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpaceCrackConfigPortal.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/SpaceCrack/BSGsSpaceCrackConfigPortal"))
struct DATACENTER_API FGsSchemaSpaceCrackConfigPortal
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	CreatureRankType grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="portalProp"))
	FGsSchemaPropDataRow portalProp;
public:
	FGsSchemaSpaceCrackConfigPortal();
	
protected:
	bool operator== (const FGsSchemaSpaceCrackConfigPortal& __Other) const
	{
		return FGsSchemaSpaceCrackConfigPortal::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSpaceCrackConfigPortal::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSpaceCrackConfigPortal::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSpaceCrackConfigPortalRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSpaceCrackConfigPortalRow() : Super()
	{
	}
	
	const FGsSchemaSpaceCrackConfigPortal* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSpaceCrackConfigPortal* Row = nullptr;
		return Table->FindRow<FGsSchemaSpaceCrackConfigPortal>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSpaceCrackConfigPortalRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSpaceCrackConfigPortalRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSpaceCrackConfigPortal final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/SpaceCrack");
	}
	
};
