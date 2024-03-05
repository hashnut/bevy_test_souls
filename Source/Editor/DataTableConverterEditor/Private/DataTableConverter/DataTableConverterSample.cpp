#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

#include "DataTableConverter/GsDataTableConvertBase.h"
#include "Event/EventAction/GsSchemaEventActionGroup.h"
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
#define DATATABLE_CONVERTER_PHASE 0

/**
 * 상황에 따라서 include 파일을 적절히 변경합니다. 이하의 코드는 페이즈가 0 이 아닌 경우에만 빌드합니다.
 */
#if DATATABLE_CONVERTER_PHASE != 0

	#include "Event/EventAction/GsSchemaEventActionGroup_Temp.h"

	// 원본 데이터테이블의 Package Name
	static const FString ORIGINAL_DATATABLE_PACKAGE_NAME(TEXT("/Game/DataCenter/DataTable/Event/EventAction/DTGsEventActionGroup"));
	static const FString TEMP_SCHEMA_HEADER_FILENAME(TEXT("C:/u4/ClientG/Source/T1Project/DataCenter/DataSchema/Event/EventAction/GsSchemaEventActionGroup_Temp.h"));

	/**
	 * 페이즈 1: "임시 데이터테이블 생성" 파트입니다.
	 */
	#if DATATABLE_CONVERTER_PHASE & 1

		IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterSample1CreateTempDataTable)
		bool FDataTableConverterSample1CreateTempDataTable::Convert()
		{
			// 1-1. 여기서 코드를 작성하기 전에, 원본 스키마의 헤더와 소스 파일을 임시 파일로 복사하고, 이름을 적절히 변경합니다.
			// 생성할 임시 클래스는 작업 완료 후 삭제하므로, 적절한 파일에 선언/정의하고, 해당 헤더 파일을 include 합니다.
			// 예) FGsSchemaEventActionGroup_Temp 를 생성하고 "EventActionGroup" 부분을 EventActionGroup_Temp 등으로 변경

			// 1-2. 임시 데이터테이블에 할당할 새로운 임시 스키마
			static UScriptStruct* TempSchema = FGsSchemaEventActionGroup_Temp::StaticStruct();
	
			// 1-3. 원본 데이터테이블을 복제하여 임시 데이터테이블을 생성하고, 임시 스키마로 교체함.
			// 이 과정에서 사용하는 두 스키마는 완전히 동일한 멤버로 구성되어 있어야 합니다.
			if (!FGsDataTableConvertBase::CreateTempDataTable(ORIGINAL_DATATABLE_PACKAGE_NAME, TempSchema))
			{
				return false;
			}
	
			return true;
		}

	#endif // DATATABLE_CONVERTER_PHASE & 1

	/**
	 * 1-4. 에디터 빌드
	 * 1-5. 에디터 실행 후 "데이터테이블 컨버터 UI" 등에서 위에 작성한 "임시 데이터테이블 생성" 파트를 실행
	 * 1-6. IDE 에서 리팩토링 기능을 사용할 경우, 해당 클래스에서 리팩토링을 활용하여 코드를 수정하고, 에셋스키마를 수정한 후 스키마 생성 실행.
	 *      IDE 에서 리팩토링 기능이 불필요한 경우, 에셋스키마를 수정한 후 스키마 생성 실행.
	 * 1-7. 에디터 종료
	 */

	/**
	 * 페이즈 2: 데이터테이블 "데이터 이동" 파트입니다.
	 */
	#if DATATABLE_CONVERTER_PHASE & 2

		IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterSample2TransferDatas)
		bool FDataTableConverterSample2TransferDatas::Convert()
		{
			// 2-1. 원본 데이터테이블 로드
			UGsDataTable* OriginalDataTable = nullptr;
			if (!FGsDataTableConvertBase::LoadDataTable(ORIGINAL_DATATABLE_PACKAGE_NAME, OriginalDataTable))
			{
				return false;
			}

			// 2-2. 원본 데이터테이블 비우기
			OriginalDataTable->EmptyTable();

			// 2-3. 임시 데이터테이블 경로 가져오기
			FString TempDataTablePackageName;
			if (!FGsDataTableConvertBase::CreateTempDataTablePackageName(ORIGINAL_DATATABLE_PACKAGE_NAME, TempDataTablePackageName))
			{
				return false;
			}

			// 2-4. 임시 데이터테이블 로드
			UGsDataTable* TempDataTable = nullptr;
			if (!FGsDataTableConvertBase::LoadDataTable(TempDataTablePackageName, TempDataTable))
			{
				return false;
			}

			// 2-5. 임시 데이터테이블 순환하면서 원본 데이터테이블에 입력
			const TArray<FName> RowNames(TempDataTable->GetRowNames());
			for (const FName RowName : RowNames)
			{
				const FGsSchemaEventActionGroup_Temp* TempRow = TempDataTable->FindRow<FGsSchemaEventActionGroup_Temp>(RowName, FString());

				FGsSchemaEventActionGroup OriginalRow;

				// RowName 변환이 필요한 경우, 다음과 같이 수정할 수도 있지만, 이 데이터테이블을 다른 테이블에서 RowPicker 로 연결한 경우, 연결이 깨질 수 있습니다.
				// DataTableUtils::MakeValidName(FString::Printf(TEXT("%d.%s"), TempRow->id, TEXT("blahblah")));
				OriginalRow.RowName = TempRow->RowName;
				OriginalRow.idMy = TempRow->id;
				OriginalRow.boolMy = true; // 일일이 입력하지 않아도 기본값은 입력됩니다.(생성자를 참조)
				OriginalRow.eventActionListMy = TempRow->eventActionList;

				// 생성한 레코드를 추가
				OriginalDataTable->AddRow(OriginalRow.RowName, OriginalRow);
			}

			// 복제한 레코드의 갯수가 동일한지 체크
			if (OriginalDataTable->GetRowMap().Num() != TempDataTable->GetRowMap().Num())
			{
				UE_LOG(LogTemp, Error, TEXT("OriginalDataTable->GetRowMap().Num() != TempDataTable->GetRowMap().Num(), OriginalDataTable:%s"), *OriginalDataTable->GetName());
				return false;
			}

			// 2-6. 원본 데이터테이블 저장
			if (!OriginalDataTable->SavePackage())
			{
				UE_LOG(LogTemp, Error, TEXT("!OriginalDataTable->SavePackage(), OriginalDataTable:%s"), *OriginalDataTable->GetName());
				return false;
			}

			if (!FGsDataTableConvertBase::DeleteTempFiles(TempDataTablePackageName, TEMP_SCHEMA_HEADER_FILENAME))
			{
				UE_LOG(LogTemp, Error, TEXT("!OriginalDataTable->SavePackage(), OriginalDataTable:%s"), *OriginalDataTable->GetName());
				return false;
			}

			return true;
		}

	#endif // DATATABLE_CONVERTER_PHASE & 2

	/**
	 * 2-7. 정리
	 * 빌드 후 컨버터 목록에서 이 컨버터를 실행한 후, 데이터가 잘 이전되었는지 확인합니다.
	 * 이전을 확인하면 임시 생성했던 헤더, 소스 파일과 데이터테이블을 삭제합니다.
	 * 컨버트를 완료한 스키마, 헤더, 소스, 데이터테이블 파일을 커밋합니다.
	 */

#endif // DATATABLE_CONVERTER_PHASE > 0