/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillCollision.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillNotifySkillGuide.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/NotifyInfo/BSGsSkillNotifySkillGuide"))
struct DATACENTER_API FGsSchemaSkillNotifySkillGuide
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="이펙트 지속 시간", DisplayName="duration"))
	float duration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="이펙트 경로", DisplayName="effectPath"))
	FSoftObjectPath effectPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="오브젝트 이동에 영향을 받습니다.", DisplayName="isAttach"))
	bool isAttach;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="SkillCollision 정보가 아닌 하위 Colision 구조체 데이터를 사용합니다.", DisplayName="isOverride"))
	bool isOverride;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="overrideCollisionInfo"))
	FGsSchemaSkillCollisionRow overrideCollisionInfo;
public:
	FGsSchemaSkillNotifySkillGuide();
	
protected:
	bool operator== (const FGsSchemaSkillNotifySkillGuide& __Other) const
	{
		return FGsSchemaSkillNotifySkillGuide::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillNotifySkillGuide::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillNotifySkillGuide::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillNotifySkillGuideRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillNotifySkillGuideRow() : Super()
	{
	}
	
	const FGsSchemaSkillNotifySkillGuide* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillNotifySkillGuide* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillNotifySkillGuide>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillNotifySkillGuideRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillNotifySkillGuideRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillNotifySkillGuide final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/NotifyInfo");
	}
	
};
