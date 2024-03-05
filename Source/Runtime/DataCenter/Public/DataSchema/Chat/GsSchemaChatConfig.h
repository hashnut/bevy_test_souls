/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaChatConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Chat/BSGsChatConfig"))
struct DATACENTER_API FGsSchemaChatConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spamCount"))
	int32 spamCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spamIntervalTimeS"))
	int64 spamIntervalTimeS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spamPenaltyTimeS"))
	int64 spamPenaltyTimeS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeChatOpenLevel"))
	int32 invadeChatOpenLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="freeInvadeChatCount"))
	int32 freeInvadeChatCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeChatCurrencyType"))
	CurrencyType invadeChatCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeChatCurrencyAmount"))
	int32 invadeChatCurrencyAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupChatRoomMaxCount"))
	int32 groupChatRoomMaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupChaRoomMemberMaxCount"))
	int32 groupChaRoomMemberMaxCount;
public:
	FGsSchemaChatConfig();
	
protected:
	bool operator== (const FGsSchemaChatConfig& __Other) const
	{
		return FGsSchemaChatConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaChatConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaChatConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaChatConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaChatConfigRow() : Super()
	{
	}
	
	const FGsSchemaChatConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaChatConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaChatConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaChatConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaChatConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableChatConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Chat");
	}
	
};
