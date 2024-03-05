/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDealScrollData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/DealScroll/BSGsDealScrollData"))
struct DATACENTER_API FGsSchemaDealScrollData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="WidgetPath"))
	FSoftObjectPath WidgetPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="LifeTime"))
	float LifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="RelativeLocation"))
	FVector RelativeLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="LocationRandomOffset"))
	float LocationRandomOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="DirOffset"))
	float DirOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="SocketName"))
	FName SocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="DescText"))
	FText DescText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="screenOffset"))
	FVector2D screenOffset;
public:
	FGsSchemaDealScrollData();
	
protected:
	bool operator== (const FGsSchemaDealScrollData& __Other) const
	{
		return FGsSchemaDealScrollData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaDealScrollData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaDealScrollData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaDealScrollDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaDealScrollDataRow() : Super()
	{
	}
	
	const FGsSchemaDealScrollData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaDealScrollData* Row = nullptr;
		return Table->FindRow<FGsSchemaDealScrollData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaDealScrollDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaDealScrollDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableDealScrollData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("DealScroll");
	}
	
};
