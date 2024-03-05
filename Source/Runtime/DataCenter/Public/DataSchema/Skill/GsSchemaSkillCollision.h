/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillCollision.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillCollision"))
struct DATACENTER_API FGsSchemaSkillCollision
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="type"))
	SkillCollisionType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="centerType"))
	SkillAreaCenter centerType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="posOffset"))
	int32 posOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="offsetX"))
	int32 offsetX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="offsetY"))
	int32 offsetY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="angleOffset"))
	int32 angleOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="회전, 위치 계산이후 마지막에 회전", DisplayName="angleOffset2"))
	int32 angleOffset2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="width"))
	int32 width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="height"))
	int32 height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="minRadius"))
	int32 minRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="maxRadius"))
	int32 maxRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="angle"))
	int32 angle;
public:
	FGsSchemaSkillCollision();
	
protected:
	bool operator== (const FGsSchemaSkillCollision& __Other) const
	{
		return FGsSchemaSkillCollision::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillCollision::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillCollision::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillCollisionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillCollisionRow() : Super()
	{
	}
	
	const FGsSchemaSkillCollision* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillCollision* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillCollision>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillCollisionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillCollisionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillCollision final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill");
	}
	
};
