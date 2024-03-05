/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/DataCenter/Public/Unlock/GsUnlockDefine.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDemandInfoConstraint.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="사용처 정보 제한 테이블", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/DemandInfo/BSGsDemandInfoConstraint"))
struct DATACENTER_API FGsSchemaDemandInfoConstraint
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(Tooltip="showContentsTypes 의 처리 방법", DisplayName="showContentsTypesMethod"))
	EGsContentsShortcutMethod showContentsTypesMethod;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(Tooltip="출력할 컨텐츠 링크", DisplayName="showContentsTypes"))
	TSet<EGsUnlockContentsType> showContentsTypes;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(Tooltip="BM상점 탭ID", DisplayName="bmshopTabId"))
	int32 bmshopTabId;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(Tooltip="상시 출력할 추가 텍스트. 한 요소마다 한 줄로 출력.", DisplayName="extraTexts"))
	TArray<FText> extraTexts;
public:
	FGsSchemaDemandInfoConstraint();
	
protected:
	bool operator== (const FGsSchemaDemandInfoConstraint& __Other) const
	{
		return FGsSchemaDemandInfoConstraint::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaDemandInfoConstraint::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaDemandInfoConstraint::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaDemandInfoConstraintRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaDemandInfoConstraintRow() : Super()
	{
	}
	
	const FGsSchemaDemandInfoConstraint* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaDemandInfoConstraint* Row = nullptr;
		return Table->FindRow<FGsSchemaDemandInfoConstraint>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaDemandInfoConstraintRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaDemandInfoConstraintRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableDemandInfoConstraint final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("DemandInfo");
	}
	
};
