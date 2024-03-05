/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/DataCenter/Public/DataSchema/Guild/GsSchemaGuildSnsShortcutPlatform.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGuildSnsShortcutUrl.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="[기사단] URL을 키로하는 기본 정보 테이블", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildSnsShortcutUrl"))
struct DATACENTER_API FGsSchemaGuildSnsShortcutUrl
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Url 시작 문자열", DisplayName="urlStartWith"))
	FString urlStartWith;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="클릭 시 사용할 액션", DisplayName="click"))
	EGsGuildSnsShortcutAction click;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="SNS 플랫폼 정보", DisplayName="platform"))
	FGsSchemaGuildSnsShortcutPlatformRow platform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="엄격한 URL 체크용 문자열.클라팀에서 필요한 경우에만 입력.", DisplayName="strictTest"))
	FString strictTest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="체크하면 비속어 체크하여 특정 문자들이 변경될 수 있음.", DisplayName="checkBanWord"))
	bool checkBanWord;
public:
	FGsSchemaGuildSnsShortcutUrl();
	
protected:
	bool operator== (const FGsSchemaGuildSnsShortcutUrl& __Other) const
	{
		return FGsSchemaGuildSnsShortcutUrl::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGuildSnsShortcutUrl::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaGuildSnsShortcutUrl::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGuildSnsShortcutUrlRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGuildSnsShortcutUrlRow() : Super()
	{
	}
	
	const FGsSchemaGuildSnsShortcutUrl* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaGuildSnsShortcutUrl* Row = nullptr;
		return Table->FindRow<FGsSchemaGuildSnsShortcutUrl>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGuildSnsShortcutUrlRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaGuildSnsShortcutUrlRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGuildSnsShortcutUrl final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild");
	}
	
};
