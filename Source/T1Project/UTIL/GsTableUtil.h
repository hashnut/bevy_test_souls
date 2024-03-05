// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeInfo.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsTableUtil.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsTableUtil : public UObject
{
	GENERATED_BODY()
	
public:
	//-------------------------------------------------------------------------------------------
	// 데이터 센터 UTable FindRow() 간략화 유틸 함수
	//-------------------------------------------------------------------------------------------
	template <typename tSchema>
	static const tSchema* FindRow(FName RowKey)
	{
		const UGsTable* gsTable = tSchema::GetStaticTable();
		if (nullptr == gsTable)
		{
			return nullptr;
		}

		const tSchema* schema;
		if (gsTable->FindRow<tSchema>(RowKey, schema))
		{
			return schema;
		}
		return nullptr;
	}
	template <typename tSchema>
	static const tSchema* FindRow(const FString& RowKey)
	{
		return FindRow<tSchema>(FName(*RowKey));
	}
	template <typename tSchema>
	static const tSchema* FindRow(int32 RowKey)
	{
		return FindRow<tSchema>(FName(*FString::FromInt(RowKey)));
	}
	template <typename tSchema>
	static const tSchema* FindRow(uint32 RowKey)
	{
		return FindRow<tSchema>(FName(*FString::FromInt(RowKey)));
	}
	template <typename tSchema>
	static const tSchema* FindRow(float RowKey)
	{
		return FindRow<tSchema>(FName(*FString::SanitizeFloat(RowKey)));
	}

	template <typename tTable, typename tSchema, typename tKey>
	static const tSchema* FindRowById(tKey Key)
	{
		const tTable* gsTable = Cast<tTable>(tSchema::GetStaticTable());
		if (nullptr == gsTable)
		{
			return nullptr;
		}

		const tSchema* schema;

		if (gsTable->FindRowById(Key, schema))
		{
			return schema;
		}
		return nullptr;
	}

	template <typename tSchema>
	static const tSchema* FindFirstRow()
	{
		const UGsTable* table = tSchema::GetStaticTable();
		if (table)
		{			
			TArray<const tSchema*> dataArray;
			table->GetAllRows(dataArray);
			if (dataArray.Num() > 0)
			{
				return dataArray[0];
			}
		}
		
		return nullptr;
	}
	//-------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------
	// FGsSchemaBase 클래스의 RowName 컨버터 함수
	// 맞는 데이터 타입에 따라 특수화 처리 필요 (기본형은 거의 의미가 없음)
	//-------------------------------------------------------------------------------------------
	template <typename T>
	static T ConvertRow(const FGsSchemaBase* Schema)
	{
		check(nullptr != Schema);
		return static_cast<T>(Schema->GetRowName());
	}

	template<typename tableData>
	static FString GetCustomTerritoryDataTableName(const FString& inLevelKey)
	{
		FString name = tableData::StaticClass()->GetName();
		name.RemoveFromStart(TEXT("GsTable"));

		return "DTGs" + name + "_" + inLevelKey;
	}
};

template <> int32 UGsTableUtil::ConvertRow(const FGsSchemaBase* Schema);
template <> uint32 UGsTableUtil::ConvertRow(const FGsSchemaBase* Schema);
template <> int64 UGsTableUtil::ConvertRow(const FGsSchemaBase* Schema);