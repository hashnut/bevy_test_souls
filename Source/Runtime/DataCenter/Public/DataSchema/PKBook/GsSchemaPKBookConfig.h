/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/PKBook/GsPKBookStruct.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPKBookConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/PKBook/BSGsPKBookConfig"))
struct DATACENTER_API FGsSchemaPKBookConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maximumCountOfPKRecord"))
	int32 maximumCountOfPKRecord;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="availableTimeForAction"))
	int32 availableTimeForAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maximumCountOfRevenge"))
	int32 maximumCountOfRevenge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="revengeCostType"))
	CurrencyType revengeCostType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="revengeCost"))
	int32 revengeCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maximumCountOfTease"))
	int32 maximumCountOfTease;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="teaseCostType"))
	CurrencyType teaseCostType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="teaseCost"))
	int32 teaseCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="killPoint"))
	int32 killPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="assistPoint"))
	int32 assistPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="damageTimer"))
	int32 damageTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="randomRadiusForTargetAround"))
	int32 randomRadiusForTargetAround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maximumCountOfDetectionTarget"))
	int32 maximumCountOfDetectionTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="detectionEffectInfoSet"))
	TArray<FGsPKDetectionEffectInfo> detectionEffectInfoSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="soundResource"))
	FSoftObjectPath soundResource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="soundResourceMock"))
	FSoftObjectPath soundResourceMock;
public:
	FGsSchemaPKBookConfig();
	
protected:
	bool operator== (const FGsSchemaPKBookConfig& __Other) const
	{
		return FGsSchemaPKBookConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPKBookConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPKBookConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPKBookConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPKBookConfigRow() : Super()
	{
	}
	
	const FGsSchemaPKBookConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPKBookConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaPKBookConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPKBookConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPKBookConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePKBookConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("PKBook");
	}
	
};
