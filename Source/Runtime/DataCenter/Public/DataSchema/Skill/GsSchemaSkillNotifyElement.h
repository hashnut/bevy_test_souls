/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillNotifyElement.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillNotifyElement"))
struct DATACENTER_API FGsSchemaSkillNotifyElement
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="executeConditionId"))
	int32 executeConditionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="자신의 BackAggroComponent 에 등록된 개체(Npc) 들에게 어그로 적립", DisplayName="addAggro"))
	int32 addAggro;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="effectText"))
	FText effectText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="typeInfo"))
	FGsSchemaRowPoly typeInfo;
public:
	FGsSchemaSkillNotifyElement();
	
protected:
	bool operator== (const FGsSchemaSkillNotifyElement& __Other) const
	{
		return FGsSchemaSkillNotifyElement::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillNotifyElement::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillNotifyElement::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillNotifyElementRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillNotifyElementRow() : Super()
	{
	}
	
	const FGsSchemaSkillNotifyElement* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillNotifyElement* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillNotifyElement>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillNotifyElementRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillNotifyElementRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillNotifyElement final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill");
	}
	
};
