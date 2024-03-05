
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

#include "DataTableConverter/GsDataTableConvertBase.h"
#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "Animation/AnimMontage.h"
#include "../../../../T1Project/UTIL/GsDir.h"
#include "DataSchema/Skill/GsSchemaSkillReaction.h"
#include "TransformVectorized.h"


IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterRootmotionSet)
bool FDataTableConverterRootmotionSet::Convert()
{
	UGsDataTable* DTGsSkillSet = nullptr;
	UGsDataTable* DTGsSkillReactionSet = nullptr;
	UGsDataTable* DTGsRootmotionSet = nullptr;

	// 테이블 로드
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Skill/DTGsSkillSet.DTGsSkillSet"),
		DTGsSkillSet))
	{
		return false;
	}

	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Skill/DTGsSkillReaction.DTGsSkillReaction"),
		DTGsSkillReactionSet))
	{
		return false;
	}

	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Skill/RootMotion/DTGsRootmotionSet.DTGsRootmotionSet"),
		DTGsRootmotionSet))
	{
		return false;
	}
	
	// 테이블 비우기
	DTGsRootmotionSet->EmptyTable();

	// 클라이언트에서 모델 데이터는 x축을 전방으로 하기 위해 90도 회전이 있다.
	// 애니메이션 원본 데이터는 Y축 전방이므로 90도 회전 적용을 함
	FTransform originTm(FRotator(0.f, -90.f, 0.f));


	// SkillReactionSet데이터에서 찾기
	const TArray<FName> skillReactionSetRowName(DTGsSkillReactionSet->GetRowNames());
	for (const FName& rowName : skillReactionSetRowName)
	{
		if (FGsSchemaSkillReaction* skillReactionSet =
			DTGsSkillReactionSet->FindRow<FGsSchemaSkillReaction>(rowName, FString()))
		{
			// 몽타주 데이터 찾기
			if (UObject* loadObject = skillReactionSet->aniPath.TryLoad())
			{
				UAnimMontage* montage = Cast<UAnimMontage>(loadObject);
				if (nullptr == montage)
				{
					continue;
				}

				// rootmotion 데이터 찾기
				if (montage->HasRootMotion())
				{
					// id규칙이 있다면 문의할것
					FGsSchemaRootmotionSet newRootmotion;
					newRootmotion.id = skillReactionSet->id;

					// 서버에도 반영되야 되서 totalRuntime을 총길이로 얻기
					float totalLen = skillReactionSet->totalRuntime;

					// 0.1초 간격으로 입력
					for (int i = 0; i < (int)(totalLen * 10) + 1; ++i)
					{
						float offset = i * 0.1f;
						FTransform rootmotionTm = montage->ExtractRootMotionFromTrackRange(0.f, offset);
						FTransform convertTm = rootmotionTm.GetRelativeTransform(originTm);

						const FVector DeltaTranslation = rootmotionTm.GetTranslation() - originTm.GetTranslation();
						const FVector dir = originTm.GetRotation() * DeltaTranslation;

						float angle = FGsDir::dirToAngle(dir);
						float degree = FGsDir::AngleToDegree(angle);

						FGsSchemaRootmotionInfo newInfo;
						newInfo.Offset = offset;
						newInfo.Angle = angle;
						newInfo.Pos = dir;
						newInfo.Distance = dir.Size();

						newRootmotion.timeLine.Add(newInfo);
					}

					FName RowName = FName(*FString::Printf(TEXT("%d.%d"), skillReactionSet->id, DTGsRootmotionSet->GetRowMap().Num()));
					DTGsRootmotionSet->AddRow(RowName, newRootmotion);

					// RowPick 연결
					skillReactionSet->rootMotionId.DataTable = DTGsRootmotionSet;
					skillReactionSet->rootMotionId.RowName = RowName;
				}
			}
		}
	}

	
	// Skillset데이터에서 찾기
	const TArray<FName> skillSetRowName(DTGsSkillSet->GetRowNames());
	for (const FName& rowName : skillSetRowName)
	{
		if (FGsSchemaSkillSet* skillSet = 
			DTGsSkillSet->FindRow<FGsSchemaSkillSet>(rowName, FString()))
		{
			// 몽타주 데이터 찾기
			if (UObject* loadObject = skillSet->aniPath.TryLoad())
			{
				UAnimMontage* montage = Cast<UAnimMontage>(loadObject);
				if (nullptr == montage)
				{
					continue;
				}

				// rootmotion 데이터 찾기
				if (montage->HasRootMotion())
				{
					// id규칙이 있다면 문의할것
					FGsSchemaRootmotionSet newRootmotion;
					newRootmotion.id = 1 << 16 | skillSet->id;

					// 서버에도 반영되야 되서 totalRuntime을 총길이로 얻기
					float totalLen = skillSet->totalRuntime;

					// 0.1초 간격으로 입력
					for (int i = 0; i < (int)(totalLen * 10) + 1; ++i)
					{
						float offset = i * 0.1f;
						FTransform rootmotionTm = montage->ExtractRootMotionFromTrackRange(0.f, offset);
						FTransform convertTm = rootmotionTm.GetRelativeTransform(originTm);

						const FVector DeltaTranslation = rootmotionTm.GetTranslation() - originTm.GetTranslation();
						const FVector dir = originTm.GetRotation() * DeltaTranslation;

						float angle = FGsDir::dirToAngle(dir);
						float degree = FGsDir::AngleToDegree(angle);

						FGsSchemaRootmotionInfo newInfo;
						newInfo.Offset = offset;
						newInfo.Angle = angle;
						newInfo.Pos = dir;
						newInfo.Distance = dir.Size();

						newRootmotion.timeLine.Add(newInfo);
					}

					FName RowName = FName(*FString::Printf(TEXT("%d.%d"), skillSet->id, DTGsRootmotionSet->GetRowMap().Num()));
					DTGsRootmotionSet->AddRow(RowName, newRootmotion);

					// RowPick 연결
					skillSet->rootMotionId.DataTable = DTGsRootmotionSet;
					skillSet->rootMotionId.RowName = RowName;
				}
			}
		}
	}

	// 테이블 저장
	DTGsRootmotionSet->SavePackage();
	DTGsSkillReactionSet->SavePackage();
	DTGsSkillSet->SavePackage();

	return true;
}