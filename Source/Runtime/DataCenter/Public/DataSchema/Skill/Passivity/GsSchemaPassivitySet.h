/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectInfo.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UGsHitEffectData;
#include "GsSchemaPassivitySet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Passivity/BSGsPassivitySet"))
struct DATACENTER_API FGsSchemaPassivitySet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name", Tooltip="기획서버참고용메모"))
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(MultiLine="true", DisplayName="descText"))
	FText descText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	PassivityType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectInfoList"))
	TArray<FGsSchemaPassivityEffectInfo> effectInfoList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="isBuff"))
	bool isBuff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="hiteffectData", Tooltip="맞은 대상에게 붙는 이펙트 정보(Override)"))
	UGsHitEffectData* hiteffectData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="hitSoundData", Tooltip="맞은 대상에게 붙는 사운드 정보 (Override)"))
	FGsSchemaSoundResDataRow hitSoundData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="hideUI", Tooltip="아이콘 등 UI에 표시여부"))
	bool hideUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="iconPath"))
	FSoftObjectPath iconPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="smallIconPath"))
	FSoftObjectPath smallIconPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="effectText"))
	FText effectText;
public:
	FGsSchemaPassivitySet();
	
protected:
	bool operator== (const FGsSchemaPassivitySet& __Other) const
	{
		return FGsSchemaPassivitySet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassivitySet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassivitySet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassivitySetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassivitySetRow() : Super()
	{
	}
	
	const FGsSchemaPassivitySet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassivitySet* Row = nullptr;
		return Table->FindRow<FGsSchemaPassivitySet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassivitySetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassivitySetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassivitySet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/Passivity");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaPassivitySet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPassivitySet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaPassivitySet*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaPassivitySet* Temp;
		return !FindRowById(FGsSchemaPassivitySet().id, Temp);
	}
#endif
};
