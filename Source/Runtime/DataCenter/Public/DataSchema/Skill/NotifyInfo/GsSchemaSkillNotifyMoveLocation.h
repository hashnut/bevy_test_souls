/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillNotifyMoveLocation.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/NotifyInfo/BSGsSkillNotifyMoveLocation"))
struct DATACENTER_API FGsSchemaSkillNotifyMoveLocation
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="타겟을 중심으로 목적지를 산출할 것인지, 캐스터를 중심으로 목적지를 산출할 것인지 구분 데이터 (SharedSkillEnum.h - SkillAreaCenter 참조)", DisplayName="centerType"))
	SkillAreaCenter centerType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="이동 구분 데이터 (SkillNotifyMoveType Enum 내용 참고)", DisplayName="moveType"))
	SkillNotifyMoveType moveType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="이동 후 어느 방향을 기준으로 바라볼 것인지 구분 데이터 (SkillNotifyMoveDirectionType Enum 내용 참고)", DisplayName="postDirectionType"))
	SkillNotifyPostDirectionType postDirectionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="centerType의 대상의 moveDirectionType 에 따른 방향으로 어느 정도 거리에 있을 것인지 구분", DisplayName="distance"))
	int32 distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="클라이언트 이동 보간 속도 기준(m/s)", DisplayName="moveSpeed"))
	int32 moveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="클라이언트 이동 보간 시간", DisplayName="duration"))
	float duration;
public:
	FGsSchemaSkillNotifyMoveLocation();
	
protected:
	bool operator== (const FGsSchemaSkillNotifyMoveLocation& __Other) const
	{
		return FGsSchemaSkillNotifyMoveLocation::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillNotifyMoveLocation::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillNotifyMoveLocation::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillNotifyMoveLocationRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillNotifyMoveLocationRow() : Super()
	{
	}
	
	const FGsSchemaSkillNotifyMoveLocation* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillNotifyMoveLocation* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillNotifyMoveLocation>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillNotifyMoveLocationRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillNotifyMoveLocationRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillNotifyMoveLocation final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/NotifyInfo");
	}
	
};
