/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsSchemaPlayerWeaponDetailInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/BSGsPlayerWeaponDetailInfo"))
struct DATACENTER_API FGsSchemaPlayerWeaponDetailInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type", Tooltip))
	CreatureWeaponType type;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="무기별 애님BP정보"))
	FSoftObjectPath animationBP;
	
public:
	FGsSchemaPlayerWeaponDetailInfo();
	
protected:
	bool operator== (const FGsSchemaPlayerWeaponDetailInfo& __Other) const
	{
		return FGsSchemaPlayerWeaponDetailInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPlayerWeaponDetailInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPlayerWeaponDetailInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPlayerWeaponDetailInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPlayerWeaponDetailInfoRow() : Super()
	{
	}
	
	const FGsSchemaPlayerWeaponDetailInfo* GetRow() const
	{
		const FGsSchemaPlayerWeaponDetailInfo* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaPlayerWeaponDetailInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPlayerWeaponDetailInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPlayerWeaponDetailInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePlayerWeaponDetailInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject");
	}
	
};
