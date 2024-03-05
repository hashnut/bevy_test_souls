/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/InterServer/GsSchemaBossTimeInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaUserLookInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaInvadeWorldConfigDataEx.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/BSGsInvadeWorldConfigDataEx"))
struct DATACENTER_API FGsSchemaInvadeWorldConfigDataEx
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeStartCron"))
	FString invadeStartCron;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeBossTime"))
	FGsSchemaBossTimeInfo invadeBossTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeMarginTime"))
	int32 invadeMarginTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="InvadePlayTimeMinute"))
	int32 InvadePlayTimeMinute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadePortalTicketItemId"))
	FGsSchemaItemCommonRow invadePortalTicketItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadePortalPropId"))
	FGsSchemaPropDataRow invadePortalPropId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="enterPortalAbnormality"))
	FGsSchemaAbnormalitySetRow enterPortalAbnormality;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invasionBGM"))
	FGsSchemaSoundResDataRow invasionBGM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadeAttackerTransformCostumeId"))
	FGsSchemaUserLookInfoRow invadeAttackerTransformCostumeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadeAttackCurrencyItemId"))
	FGsSchemaItemCommonRow invadeAttackCurrencyItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadeDefenceCurrencyItemId"))
	FGsSchemaItemCommonRow invadeDefenceCurrencyItemId;
public:
	FGsSchemaInvadeWorldConfigDataEx();
	
protected:
	bool operator== (const FGsSchemaInvadeWorldConfigDataEx& __Other) const
	{
		return FGsSchemaInvadeWorldConfigDataEx::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaInvadeWorldConfigDataEx::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaInvadeWorldConfigDataEx::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaInvadeWorldConfigDataExRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaInvadeWorldConfigDataExRow() : Super()
	{
	}
	
	const FGsSchemaInvadeWorldConfigDataEx* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaInvadeWorldConfigDataEx* Row = nullptr;
		return Table->FindRow<FGsSchemaInvadeWorldConfigDataEx>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaInvadeWorldConfigDataExRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaInvadeWorldConfigDataExRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableInvadeWorldConfigDataEx final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("InterServer");
	}
	
};
