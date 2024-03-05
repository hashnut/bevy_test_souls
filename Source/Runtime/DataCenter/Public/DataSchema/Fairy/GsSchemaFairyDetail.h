/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyEffect.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UParticleSystem;
#include "GsSchemaFairyDetail.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyDetail"))
struct DATACENTER_API FGsSchemaFairyDetail
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="explanation"))
	FString explanation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pathPartFairy"))
	FSoftObjectPath pathPartFairy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pathFairyPortrait"))
	FSoftObjectPath pathFairyPortrait;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="uiSelectedSound"))
	FGsSchemaSoundResDataRow uiSelectedSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pathSummonFairy"))
	FSoftObjectPath pathSummonFairy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="skillEffect"))
	FGsSchemaFairyEffect skillEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="vehicleEffect"))
	FGsSchemaFairyEffect vehicleEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pcTeleportParticleEffectList"))
	TArray<UParticleSystem*> pcTeleportParticleEffectList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyTeleportParticleEffectList"))
	TArray<UParticleSystem*> fairyTeleportParticleEffectList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pathTeleportStartFairy"))
	FSoftObjectPath pathTeleportStartFairy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pathTeleportArriveFairy"))
	FSoftObjectPath pathTeleportArriveFairy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="voiceFilePath"))
	FString voiceFilePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="summonHighGradeVoicePath"))
	FString summonHighGradeVoicePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="flavorText"))
	FText flavorText;
public:
	FGsSchemaFairyDetail();
	
protected:
	bool operator== (const FGsSchemaFairyDetail& __Other) const
	{
		return FGsSchemaFairyDetail::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFairyDetail::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFairyDetail::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFairyDetailRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFairyDetailRow() : Super()
	{
	}
	
	const FGsSchemaFairyDetail* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFairyDetail* Row = nullptr;
		return Table->FindRow<FGsSchemaFairyDetail>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFairyDetailRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFairyDetailRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFairyDetail final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Fairy");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaFairyDetail*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaFairyDetail>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaFairyDetail*& OutRow) const
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
		const FGsSchemaFairyDetail* Temp;
		return !FindRowById(FGsSchemaFairyDetail().id, Temp);
	}
#endif
};
