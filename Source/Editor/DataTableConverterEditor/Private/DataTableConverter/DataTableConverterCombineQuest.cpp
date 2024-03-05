/**
 * 퀘스트 양산화를 위해 퀘스트 데이터 테이블을 분리합니다.
 * 이에따라 분리된 퀘스트 테이블을 통합해주는 단계가 필요하다고 판단 해당 로직에서 처리합니다.
 * 통합할때는 UE4에서 지원하는 CompoiteDataTable을 활용합니다.
 */

#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "RunTime/Engine/Classes/Engine/CompositeDataTable.h"

#include "DataTableConverter/GsDataTableConvertBase.h"

#include "Quest/GsSchemaQuest.h"
#include "Quest/GsSchemaQuestObjective.h"

#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"

//	// 원본 데이터테이블의 Package Name
//static const FString ORIGINAL_DATATABLE_PACKAGE_NAME(TEXT("/Game/DataCenter/DataTable/Quest/DTGsQuest.DTGsQuest"));
//static const FString QUEST_DATATABLE_COMPOSITE_PACKAGE_NAME(TEXT("/Game/DataCenter/DataTable/Quest/WorkingQuest/DTGsQuestComposite.DTGsQuestComposite"));
//
//
//IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterCombineQuest)
//bool FDataTableConverterCombineQuest::Convert()
//{
//	// 원본 데이터테이블 로드
//	UGsDataTable* OriginalDataTable = nullptr;
//	if (!FGsDataTableConvertBase::LoadDataTable(ORIGINAL_DATATABLE_PACKAGE_NAME, OriginalDataTable))
//	{
//		return false;
//	}
//	
//	// 합쳐진 데이터 가져오기
//	UCompositeDataTable* CompositeData = nullptr;
//	if (!FGsDataTableConvertBase::LoadDataTableComposite(QUEST_DATATABLE_COMPOSITE_PACKAGE_NAME, CompositeData))
//	{
//		return false;
//	}
//
//	// 원본 데이터테이블 비우기
//	OriginalDataTable->EmptyTable();
//
//
//	// 데이터테이블 순환하면서 원본 데이터테이블에 입력
//	for (TMap<FName, uint8*>::TConstIterator RowMapIter(CompositeData->GetRowMap().CreateConstIterator()); RowMapIter; ++RowMapIter)
//	{
//		if (ensure(RowMapIter.Value() != nullptr))
//		{
//			// UDataTable::AddRow will first remove the row if it already exists so we don't need to do anything special here
//			FTableRowBase* TableRowBase = (FTableRowBase*)RowMapIter.Value();
//			OriginalDataTable->AddRow(RowMapIter.Key(), *TableRowBase);
//		}
//	}
//
//	// 복제한 레코드의 갯수가 동일한지 체크
//	if (OriginalDataTable->GetRowMap().Num() != CompositeData->GetRowMap().Num())
//	{
//		UE_LOG(LogTemp, Error, TEXT("OriginalDataTable->GetRowMap().Num() != TempDataTable->GetRowMap().Num(), OriginalDataTable:%s"), *OriginalDataTable->GetName());
//		return false;
//	}
//
//	// 원본 데이터테이블 저장
//	if (!OriginalDataTable->SavePackage())
//	{
//		UE_LOG(LogTemp, Error, TEXT("!OriginalDataTable->SavePackage(), OriginalDataTable:%s"), *OriginalDataTable->GetName());
//		return false;
//	}
//
//	return true;
//}

static const FString ORIGINAL_DATATABLE_PACKAGE_NAME2(TEXT("/Game/DataCenter/DataTable/Quest/DTGsQuestObjective.DTGsQuestObjective"));
static const FString QUEST_DATATABLE_COMPOSITE_PACKAGE_NAME2(TEXT("/Game/DataCenter/DataTable/Quest/WorkingQuest/DTGsQuestObjectiveComposite.DTGsQuestObjectiveComposite"));


IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterCombineQuestObjective)
bool FDataTableConverterCombineQuestObjective::Convert()
{
	// 원본 데이터테이블 로드
	UGsDataTable* OriginalDataTable = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(ORIGINAL_DATATABLE_PACKAGE_NAME2, OriginalDataTable))
	{
		return false;
	}

	// 합쳐진 데이터 가져오기
	UCompositeDataTable* CompositeData = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTableComposite(QUEST_DATATABLE_COMPOSITE_PACKAGE_NAME2, CompositeData))
	{
		return false;
	}

	// 원본 데이터테이블 비우기
	OriginalDataTable->EmptyTable();


	// 데이터테이블 순환하면서 원본 데이터테이블에 입력
	for (TMap<FName, uint8*>::TConstIterator RowMapIter(CompositeData->GetRowMap().CreateConstIterator()); RowMapIter; ++RowMapIter)
	{
		if (ensure(RowMapIter.Value() != nullptr))
		{
			// UDataTable::AddRow will first remove the row if it already exists so we don't need to do anything special here
			FTableRowBase* TableRowBase = (FTableRowBase*)RowMapIter.Value();
			OriginalDataTable->AddRow(RowMapIter.Key(), *TableRowBase);
		}
	}

	// 복제한 레코드의 갯수가 동일한지 체크
	if (OriginalDataTable->GetRowMap().Num() != CompositeData->GetRowMap().Num())
	{
		UE_LOG(LogTemp, Error, TEXT("OriginalDataTable->GetRowMap().Num() != TempDataTable->GetRowMap().Num(), OriginalDataTable:%s"), *OriginalDataTable->GetName());
		return false;
	}

	// 원본 데이터테이블 저장
	if (!OriginalDataTable->SavePackage())
	{
		UE_LOG(LogTemp, Error, TEXT("!OriginalDataTable->SavePackage(), OriginalDataTable:%s"), *OriginalDataTable->GetName());
		return false;
	}

	return true;
}