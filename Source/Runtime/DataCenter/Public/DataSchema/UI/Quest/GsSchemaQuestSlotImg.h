/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestSlotImg.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/UI/Quest/BSGsQuestSlotImg"))
struct DATACENTER_API FGsSchemaQuestSlotImg
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	int32 id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="imgPath", Tooltip))
	FSoftObjectPath imgPath;
	
public:
	FGsSchemaQuestSlotImg();
	
protected:
	bool operator== (const FGsSchemaQuestSlotImg& __Other) const
	{
		return FGsSchemaQuestSlotImg::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestSlotImg::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestSlotImg::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestSlotImgRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestSlotImgRow() : Super()
	{
	}
	
	const FGsSchemaQuestSlotImg* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestSlotImg* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestSlotImg>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestSlotImgRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestSlotImgRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestSlotImg final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("UI/Quest");
	}
	
};
