/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UMediaSource;
class USoundBase;
#include "GsSchemaMovieData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Movie/BSGsMovieData"))
struct DATACENTER_API FGsSchemaMovieData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mediaSource"))
	TSoftObjectPtr<UMediaSource> mediaSource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="sound"))
	TSoftObjectPtr<USoundBase> sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="IsPlayBGSound"))
	bool IsPlayBGSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="영상 무한 반복 여부", DisplayName="isLoop"))
	bool isLoop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킵버튼을 출력할것인가?", DisplayName="isSkip"))
	bool isSkip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="화면을 터치해야 스킵버튼이 스르륵 나옴 isSkip이 on 일때만 작동", DisplayName="isSkipEffect"))
	bool isSkipEffect;
public:
	FGsSchemaMovieData();
	
protected:
	bool operator== (const FGsSchemaMovieData& __Other) const
	{
		return FGsSchemaMovieData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMovieData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMovieData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMovieDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMovieDataRow() : Super()
	{
	}
	
	const FGsSchemaMovieData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMovieData* Row = nullptr;
		return Table->FindRow<FGsSchemaMovieData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMovieDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMovieDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMovieData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Movie");
	}
	
};
