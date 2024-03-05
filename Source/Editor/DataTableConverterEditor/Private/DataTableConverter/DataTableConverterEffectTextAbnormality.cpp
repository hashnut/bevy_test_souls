/**
 * 퀘스트 양산화를 위해 퀘스트 데이터 테이블을 분리합니다.
 * 이에따라 분리된 퀘스트 테이블을 통합해주는 단계가 필요하다고 판단 해당 로직에서 처리합니다.
 * 통합할때는 UE4에서 지원하는 CompoiteDataTable을 활용합니다.
 */

#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "RunTime/Engine/Classes/Engine/CompositeDataTable.h"

#include "DataTableConverter/GsDataTableConvertBase.h"

#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextAbnormality.h"
#include "EffectText/GsEffectTextParser.h"

#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"


IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterEffectTextAbnormality)
bool FDataTableConverterEffectTextAbnormality::Convert()
{
	// 데이터 저장할 EffectTextTable
	const FString EFFECT_TEXT_TABLE_NAME(TEXT("/Game/DataCenter/DataTable/EffectText/DTGsEffectTextAbnormality.DTGsEffectTextAbnormality"));
	// 데이터 생성을 위해 참고할 테이블
	const FString ABNORMALITY_TABLE_NAME(TEXT("/Game/DataCenter/DataTable/Abnormality/DTGsAbnormalitySet.DTGsAbnormalitySet"));

	// 원본 데이터테이블 로드
	UGsDataTable* effectTextTable = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(EFFECT_TEXT_TABLE_NAME, effectTextTable))
	{
		return false;
	}

	// 원본 데이터테이블 비우기
	effectTextTable->EmptyTable();

	// 데이터테이블 순환하면서 원본 데이터테이블에 입력
	UGsDataTable* abnormalityTable = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(ABNORMALITY_TABLE_NAME, abnormalityTable))
	{
		return false;
	}

	// 파서 생성
	FGsEffectTextParser* tableParser = new FGsEffectTextParser();
	tableParser->Initialize();

	// EffectTextTable 데이터 채우기
	const TArray<FName> RowNames(abnormalityTable->GetRowNames());
	for (const FName& RowName : RowNames)
	{
		const FGsSchemaAbnormalitySet* abnormalityRow = abnormalityTable->FindRow<FGsSchemaAbnormalitySet>(RowName, FString());
		if (abnormalityRow)
		{
			FGsSchemaEffectTextAbnormality effectTextRow;
			effectTextRow.RowName = abnormalityRow->RowName;

			if (tableParser->SetAbnormalityData(abnormalityRow, effectTextRow))
			{
				effectTextTable->AddRow(effectTextRow.RowName, effectTextRow);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[Converter] Failed SetData. AbnormalityId: %d"), abnormalityRow->id);
			}
		}
	}

	// 레코드의 갯수가 동일한지 체크
	if (effectTextTable->GetRowMap().Num() != abnormalityTable->GetRowMap().Num())
	{
		UE_LOG(LogTemp, Error, 
			TEXT("[Converter] effectTextTable->GetRowMap().Num() != abnormalityTable->GetRowMap().Num(), effectTextTable:%s"), 
			*effectTextTable->GetName());

		// 주의: 저장을 안하면 위험해서 일단 저장은 함

		//delete tableParser;
		//tableParser = nullptr;

		//return false;
	}

	// 원본 데이터테이블 저장
	if (!effectTextTable->SavePackage())
	{
		UE_LOG(LogTemp, Error, TEXT("[Converter] !effectTextTable->SavePackage(), effectTextTable:%s"), 
			*effectTextTable->GetName());
		
		delete tableParser;
		tableParser = nullptr;

		return false;
	}

	delete tableParser;
	tableParser = nullptr;

	return true;
}
