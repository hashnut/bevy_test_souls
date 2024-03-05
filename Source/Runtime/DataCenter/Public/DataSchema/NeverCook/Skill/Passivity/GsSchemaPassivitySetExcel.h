/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivitySetExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Skill/Passivity/BSGsPassivitySetExcel"))
struct DATACENTER_API FGsSchemaPassivitySetExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="descText"))
	FText descText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	PassivityType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectInfoGroupId"))
	int32 effectInfoGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isBuff"))
	bool isBuff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hitEffectId"))
	FName hitEffectId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hitSoundId"))
	FName hitSoundId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hideUI"))
	bool hideUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconId"))
	FName iconId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="smallIconId"))
	FName smallIconId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectText"))
	FText effectText;
public:
	FGsSchemaPassivitySetExcel();
	
protected:
	bool operator== (const FGsSchemaPassivitySetExcel& __Other) const
	{
		return FGsSchemaPassivitySetExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassivitySetExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassivitySetExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassivitySetExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassivitySetExcelRow() : Super()
	{
	}
	
	const FGsSchemaPassivitySetExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassivitySetExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaPassivitySetExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassivitySetExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassivitySetExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassivitySetExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Skill/Passivity");
	}
	
};
