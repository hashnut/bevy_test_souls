/**
 * 퀘스트 양산화를 위해 퀘스트 데이터 테이블을 분리합니다.
 * 이에따라 분리된 퀘스트 테이블을 통합해주는 단계가 필요하다고 판단 해당 로직에서 처리합니다.
 * 통합할때는 UE4에서 지원하는 CompoiteDataTable을 활용합니다.
 */

#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "RunTime/Engine/Classes/Engine/CompositeDataTable.h"

#include "DataTableConverter/GsDataTableConvertBase.h"

#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/EffectText/GsSchemaEffectTextSkill.h"
#include "DataSchema/EffectText/GsSchemaEffectTextSkillNotify.h"
#include "EffectText/GsEffectTextParser.h"

#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"


IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterEffectTextSkill)
bool FDataTableConverterEffectTextSkill::Convert()
{
	// 데이터 저장할 EffectTextTable
	const FString EFFECT_TEXT_SKILL_TABLE_NAME(TEXT("/Game/DataCenter/DataTable/EffectText/DTGsEffectTextSkill.DTGsEffectTextSkill"));
	const FString EFFECT_TEXT_SKILL_NOTIFY_TABLE_NAME(TEXT("/Game/DataCenter/DataTable/EffectText/DTGsEffectTextSkillNotify.DTGsEffectTextSkillNotify"));

	// 데이터 생성을 위해 참고할 테이블
	const FString SKILL_TABLE_NAME(TEXT("/Game/DataCenter/DataTable/Skill/DTGsSkillSet.DTGsSkillSet"));
	const FString SKILL_NOTIFY_TABLE_NAME(TEXT("/Game/DataCenter/DataTable/Skill/DTGsSkillNotifySet.DTGsSkillNotifySet"));

	// 파서 생성
	FGsEffectTextParser* tableParser = new FGsEffectTextParser();
	tableParser->Initialize();

	//------------------------------------------------------------------------------------------------------------------
	// Skill
	{
		// 원본 데이터테이블 로드
		UGsDataTable* effectTextSkillTable = nullptr;
		if (!FGsDataTableConvertBase::LoadDataTable(EFFECT_TEXT_SKILL_TABLE_NAME, effectTextSkillTable))
		{
			delete tableParser;
			tableParser = nullptr;

			return false;
		}

		// 원본 데이터테이블 비우기
		effectTextSkillTable->EmptyTable();

		// 데이터테이블 순환하면서 원본 데이터테이블에 입력
		UGsDataTable* skillTable = nullptr;
		if (!FGsDataTableConvertBase::LoadDataTable(SKILL_TABLE_NAME, skillTable))
		{
			delete tableParser;
			tableParser = nullptr;

			return false;
		}		

		// 데이터 처리
		const TArray<FName> skillRowNames(skillTable->GetRowNames());
		for (const FName& skillRowName : skillRowNames)
		{
			const FGsSchemaSkillSet* skillRow = skillTable->FindRow<FGsSchemaSkillSet>(skillRowName, FString());
			if (skillRow)
			{
				// 데이터 채우기
				FGsSchemaEffectTextSkill originalRowSkill;
				originalRowSkill.RowName = skillRow->RowName;
				if (tableParser->SetSkillData(skillRow, originalRowSkill))
				{
					effectTextSkillTable->AddRow(originalRowSkill.RowName, originalRowSkill);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("[Converter] Failed SetData: SkillId: %d"), skillRow->id);
				}
			}
		}

		// 필요없는 정보는 저장 안하므로 레코드 수는 다를 수 있다

		// 원본 데이터테이블 저장
		if (!effectTextSkillTable->SavePackage())
		{
			UE_LOG(LogTemp, Error, TEXT("[Converter] !effectTextSkillTable->SavePackage(), effectTextSkillTable:%s"), 
				*effectTextSkillTable->GetName());

			delete tableParser;
			tableParser = nullptr;

			return false;
		}
	}
	
	//------------------------------------------------------------------------------------------------------------------
	// SkillNotify
	{
		// 원본 데이터테이블 로드
		UGsDataTable* effectTextSkillNotifyTable = nullptr;
		if (!FGsDataTableConvertBase::LoadDataTable(EFFECT_TEXT_SKILL_NOTIFY_TABLE_NAME, effectTextSkillNotifyTable))
		{
			delete tableParser;
			tableParser = nullptr;

			return false;
		}

		// 원본 데이터테이블 비우기
		effectTextSkillNotifyTable->EmptyTable();

		// 데이터테이블 순환하면서 원본 데이터테이블에 입력
		UGsDataTable* skillNotifyTable = nullptr;
		if (!FGsDataTableConvertBase::LoadDataTable(SKILL_NOTIFY_TABLE_NAME, skillNotifyTable))
		{
			delete tableParser;
			tableParser = nullptr;

			return false;
		}

		// 데이터 처리
		const TArray<FName> skillNotifyRowNames(skillNotifyTable->GetRowNames());
		for (const FName& skillNotifyRowName : skillNotifyRowNames)
		{
			const FGsSchemaSkillNotifySet* skillNotifyRow = 
				skillNotifyTable->FindRow<FGsSchemaSkillNotifySet>(skillNotifyRowName, FString());
			if (skillNotifyRow)
			{
				// 데이터 채우기
				FGsSchemaEffectTextSkillNotify originalRowSkillNotify;
				originalRowSkillNotify.RowName = skillNotifyRow->RowName;
				if (tableParser->SetSkillNotifyData(skillNotifyRow, originalRowSkillNotify))
				{
					effectTextSkillNotifyTable->AddRow(originalRowSkillNotify.RowName, originalRowSkillNotify);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("[Converter] Failed SetData: SkillNotifyId: %d"), skillNotifyRow->id);
				}
			}
		}

		// 필요없는 정보는 저장 안하므로 레코드 수는 다를 수 있다

		// 원본 데이터테이블 저장
		if (!effectTextSkillNotifyTable->SavePackage())
		{
			UE_LOG(LogTemp, Error, TEXT("[Converter] !effectTextSkillNotifyTable->SavePackage(), effectTextSkillNotifyTable:%s"), 
				*effectTextSkillNotifyTable->GetName());

			delete tableParser;
			tableParser = nullptr;

			return false;
		}		
	}

	delete tableParser;
	tableParser = nullptr;

	return true;
}
