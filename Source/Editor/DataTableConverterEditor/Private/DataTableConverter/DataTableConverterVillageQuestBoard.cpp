#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Runtime/Core/Public/Algo/ForEach.h"

#include "DataTableConverter/GsDataTableConvertBase.h"
#include "QuestBoard/GsSchemaQuestConditionInfo.h"
#include "QuestBoard/GsSchemaVillageQuestBoardList.h"

#include "DataSchema/NeverCook/VillageQuest/GsSchemaVillageQuestBoardListExcel.h"
#include "DataSchema/NeverCook/VillageQuest/GsSchemaVillageQuestBoardItem.h"
#include "DataSchema/NeverCook/VillageQuest/GsSchemaVillageQuestRepeatItem.h"

#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"

/**
 * 이 파일 구성시에는 아직 존재하지 않거나 이미 삭제한 클래스등을 사용하는 경우가 있습니다.
 * 그런 부분을 매번 주석처리하고 해제하는 과정이 반복될 가능성이 있어 손쉬운 빌드 적용을 위해 페이즈로 나누었습니다.
 * 각 페이즈간 차이는 다음과 같습니다.
 * 
 * - 페이즈 0: include 를 포함한 본 파일의 모든 스크립트를 빌드하지 않습니다.
 * - 페이즈 1: "임시 데이터테이블 생성" 파트만 빌드합니다.
 * - 페이즈 2: "데이터 이동" 파트만 빌드합니다.
 * - 페이즈 3: "임시 데이터테이블 생성" 와 "데이터 이동" 모두 빌드합니다.
 */
#define DATATABLE_CONVERTER_PHASE 1

/**
 * 상황에 따라서 include 파일을 적절히 변경합니다. 이하의 코드는 페이즈가 0 이 아닌 경우에만 빌드합니다.
 */
#if DATATABLE_CONVERTER_PHASE != 0


	// 원본 데이터테이블의 Package Name
	static const FString ORIGINAL_DATATABLE_PACKAGE_NAME(TEXT("/Game/DataCenter/DataTable/QuestBoard/DTGsVillageQuestBoardList"));

	/*static const FString TEMP_DATATABLE_PACKAGE_QUESTBOARD_ITEM(TEXT("/Game/DataCenter/DataTable/DataCenter/DataTable/NeverCook/VillageQuest/DTGsVillageQuestBoardItem"));
	static const FString TEMP_DATATABLE_PACKAGE_QUESTBOARD_LIST(TEXT("/Game/DataCenter/DataTable/DataCenter/DataTable/NeverCook/VillageQuest/DTGsVillageQuestBoardListExcel"));
	static const FString TEMP_DATATABLE_PACKAGE_QUESTREPEAT_ITEM(TEXT("/Game/DataCenter/DataTable/DataCenter/DataTable/NeverCook/VillageQuest/DTGsVillageQuestRepeatItem"));*/

	/**
	 * 페이즈 1: "임시 데이터테이블 생성" 파트입니다.
	 */
	#if DATATABLE_CONVERTER_PHASE & 1

		IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterVillageQuestBoardDataTable)
		bool FDataTableConverterVillageQuestBoardDataTable::Convert()
		{
			// 1. 원본 데이터테이블 로드
			UGsDataTable* OriginalDataTable = nullptr;
			if (!FGsDataTableConvertBase::LoadDataTable(ORIGINAL_DATATABLE_PACKAGE_NAME, OriginalDataTable))
			{
				return false;
			}
			OriginalDataTable->EmptyTable();

			const UGsTableVillageQuestBoardListExcel* ExcelData = Cast<UGsTableVillageQuestBoardListExcel>(FGsSchemaVillageQuestBoardListExcel::GetStaticTable());
			TArray< const FGsSchemaVillageQuestBoardListExcel*> ExcelAllRow;
			if (false == ExcelData->GetAllRows(ExcelAllRow))
			{
				UE_LOG(LogTemp, Error, TEXT("UGsTableVillageQuestBoardListExcel empty "));
				return false;
			}

			const UGsTableVillageQuestBoardItem* BoardItemData = Cast<UGsTableVillageQuestBoardItem>(FGsSchemaVillageQuestBoardItem::GetStaticTable());
			TArray< const FGsSchemaVillageQuestBoardItem*> ItemExcelAllRow;
			if (false == BoardItemData->GetAllRows(ItemExcelAllRow))
			{
				UE_LOG(LogTemp, Error, TEXT("UGsTableVillageQuestBoardItem empty "));
				return false;
			}

			const UGsTableVillageQuestRepeatItem* BoardRepeatQuestItemData = Cast<UGsTableVillageQuestRepeatItem>(FGsSchemaVillageQuestRepeatItem::GetStaticTable());
			TArray< const FGsSchemaVillageQuestRepeatItem*> RepeatQuestExcelAllRow;
			if (false == BoardRepeatQuestItemData->GetAllRows(RepeatQuestExcelAllRow))
			{
				UE_LOG(LogTemp, Error, TEXT("UGsTableVillageQuestRepeatItem empty "));
				return false;
			}

			// 2. 데이터 입력
			for (auto itr : ExcelAllRow)
			{
				FGsSchemaVillageQuestBoardList Info;
				const FGsSchemaVillageQuestBoardListExcel* ExcelDt = itr;
				Algo::ForEach(ItemExcelAllRow, [ExcelDt, &RepeatQuestExcelAllRow, &Info](const FGsSchemaVillageQuestBoardItem* item)
					{
						if (item->ParentID == ExcelDt->id)
						{
							FGsSchemaQuestConditionInfo insert;
							insert.QuestBoardType = item->QuestBoardType;
							insert.QuestMainStroy = item->QuestMainStory;

							Algo::ForEach(RepeatQuestExcelAllRow, [&item, &insert](const FGsSchemaVillageQuestRepeatItem* repeatQuest)
								{
									if (item->id == repeatQuest->ParentID)
									{
										FGsSchemaQuestRepeatScrollList repeatitem;
										repeatitem.QuestRepeatScroll = repeatQuest->QuestRepetScroll;
										repeatitem.QuestProbability = repeatQuest->QuestProbablity;
										insert.QuestRepeatScrollList.Add(repeatitem);
									}
								});
							Info.QuestConditionInfo.Add(insert);
						}
					});
				Info.id = ExcelDt->id;
				OriginalDataTable->AddRow(ExcelDt->RowName, Info);
			}
			
			// 3. 원본 데이터테이블 저장
			if (!OriginalDataTable->SavePackage())
			{
				UE_LOG(LogTemp, Error, TEXT("!OriginalDataTable->SavePackage(), OriginalDataTable:%s"), *OriginalDataTable->GetName());
				return false;
			}

	
			return true;
		}

	#endif // DATATABLE_CONVERTER_PHASE & 1

	/**
	 * 2-7. 정리
	 * 빌드 후 컨버터 목록에서 이 컨버터를 실행한 후, 데이터가 잘 이전되었는지 확인합니다.
	 * 이전을 확인하면 임시 생성했던 헤더, 소스 파일과 데이터테이블을 삭제합니다.
	 * 컨버트를 완료한 스키마, 헤더, 소스, 데이터테이블 파일을 커밋합니다.
	 */

#endif // DATATABLE_CONVERTER_PHASE > 0