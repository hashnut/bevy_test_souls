/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillNotifyProjectile.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/NotifyInfo/BSGsSkillNotifyProjectile"))
struct DATACENTER_API FGsSchemaSkillNotifyProjectile
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 projecttileId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="한꺼번에 발사할 수량"))
	int32 fireCount;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="발사원점으로부터의 발사지점을 향한 방향을 기준으로 한 오프셋 거리 (해당 Creature의 CylinderRadius은 이미 반영되어 있음)"))
	float fireOffset;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="fireOffset으로 X, Y를 구한 뒤, 더할 Z Offset (기준 Z값은 해당 Creature의 HalfHeight)"))
	float fireHeightOffset;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="발사체의 방향으로부터 각도 오프셋 (초기값)"))
	int32 initAngleOffset;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="여러발을 한꺼번에 발사할 때 개체별 각도 Offset (초기값(initAngleOffset)으로부터 발사수만큼 누적)"))
	int32 perAngleOffset;
	
public:
	FGsSchemaSkillNotifyProjectile();
	
protected:
	bool operator== (const FGsSchemaSkillNotifyProjectile& __Other) const
	{
		return FGsSchemaSkillNotifyProjectile::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillNotifyProjectile::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillNotifyProjectile::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillNotifyProjectileRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillNotifyProjectileRow() : Super()
	{
	}
	
	const FGsSchemaSkillNotifyProjectile* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillNotifyProjectile* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillNotifyProjectile>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillNotifyProjectileRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillNotifyProjectileRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillNotifyProjectile final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/NotifyInfo");
	}
	
};
