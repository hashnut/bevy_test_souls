// Fill out your copyright notice in the Description page of Project Settings.


#include "PostExcelImportScripter.h"
#include "RSQLEditorDelegates.h"

#include "Item/GsSchemaItemEquipDetail.h"
#include "stat/GsSchemaStatConvertData.h"
#include "Gameobject/Npc/GsSchemaNpcBalance.h"


void FPostExcelImportScripter::Bind()
{
	//// 한번만 실행해야하는 정보를 위해 외부 이벤트로 바인딩한후 각정변수등을 초기화한다.
	//FPostExcelImportEvent eventBinder;
	//eventBinder.BindRaw(this, &FPostExcelImportScripter::StartExcelImportStatConvert);
	//FRSQLEditorDelegates::RegisterStartEvent(FGsSchemaStatConvertData::StaticStruct(), eventBinder);
	//

	//// 각 특화된 테이블 수행작업 바인딩 시작
	//FPostExcelImportWork Binder;
	//
	//// 주요스텟값에 따른 2차 스텟 변환테이블 입력한다.  
	//Binder.BindRaw(this, &FPostExcelImportScripter::FillStatConvertDataList);
	//FRSQLEditorDelegates::AddPostImportWork(FGsSchemaStatConvertData::StaticStruct(), Binder);
	
}

void FPostExcelImportScripter::UnBind()
{
	FRSQLEditorDelegates::RemoveAllPostImportWork();
}


void FPostExcelImportScripter::StartExcelImportStatConvert(UDataTable* inTable)
{
	// 이코드는 나중에 스키마를 제작할때 프로퍼티 메타 정보로 지정되어 데이터 테이블 생성시 지정되는 방식으로 변경한다. 
	Cast<UGsDataTable>(inTable)->SetAllowDuplicateRowOnImport(true);
}

//--------------------------------------------------------------------------------------------------


//// 주요스텟값에 따른 2차 스텟 변환테이블 구조로 입력한다.  
//bool FPostExcelImportScripter::FillStatConvertDataList(UDataTable* inTable, const FName& inTableRowName, FTableRowBase* inTableRow)
//{
//	auto FillStat = [](FGsSchemaStatConvertData* destRow, FGsSchemaStatConvertData* src)-> void
//	{
//		FGsSchemaStatConvertTable baseValueTbl;
//		FGsItemStatData data;
//
//		baseValueTbl.baseValue = src->baseValue;
//
//		if (src->statType1 != StatType::NONE)
//		{
//			data.type = src->statType1;
//			data.value = src->statValue1;
//			baseValueTbl.statList.Emplace(data);
//		}
//		if (src->statType2 != StatType::NONE)
//		{
//			data.type = src->statType2;
//			data.value = src->statValue2;
//			baseValueTbl.statList.Emplace(data);
//		}
//		if (src->statType3 != StatType::NONE)
//		{
//			data.type = src->statType3;
//			data.value = src->statValue3;
//			baseValueTbl.statList.Emplace(data);
//		}
//		destRow->convertList.Emplace(baseValueTbl);
//	};
//
//	FString ContextString = TEXT("PostExcel Import Job : FGsSchemaStatConvertData");
//	FGsSchemaStatConvertData* preValue = inTable->FindRow<FGsSchemaStatConvertData>(inTableRowName, *ContextString, false);
//	bool bInsert = true;
//	if (nullptr != preValue)
//	{
//		FillStat(preValue, (FGsSchemaStatConvertData*)inTableRow);
//		bInsert = false;
//	}
//	else
//	{
//		FGsSchemaStatConvertData* castStat = (FGsSchemaStatConvertData*)inTableRow;
//		FillStat(castStat, castStat);		
//	}
//	return bInsert;
//}