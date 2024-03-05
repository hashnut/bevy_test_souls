/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMapEventActionDialog.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/EventAction/BSGsMapEventActionDialog"))
struct DATACENTER_API FGsSchemaMapEventActionDialog
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="style"))
	EventDialogSyle style;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mainMessage"))
	FText mainMessage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="subMessage"))
	FText subMessage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="checkRefresh"))
	bool checkRefresh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconPath"))
	FSoftObjectPath iconPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sound"))
	FGsSchemaSoundResDataRow sound;
public:
	FGsSchemaMapEventActionDialog();
	
protected:
	bool operator== (const FGsSchemaMapEventActionDialog& __Other) const
	{
		return FGsSchemaMapEventActionDialog::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMapEventActionDialog::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMapEventActionDialog::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMapEventActionDialogRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMapEventActionDialogRow() : Super()
	{
	}
	
	const FGsSchemaMapEventActionDialog* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMapEventActionDialog* Row = nullptr;
		return Table->FindRow<FGsSchemaMapEventActionDialog>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMapEventActionDialogRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMapEventActionDialogRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMapEventActionDialog final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/EventAction");
	}
	
};
