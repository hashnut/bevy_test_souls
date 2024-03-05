// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactionSectionData.h"

#include "SkillNotify/CutremineSkillNotify.h"
#include "SkillNotify/CollisionSkillNotify.h"

#include "Data/SkillEditorDataAsset.h"
#include "GsTableManagerEditor.h"
#include "Misc/MessageDialog.h"

#if WITH_EDITOR
void UReactionSectionData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName = (PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None);;
		/*if (PropertyName == TEXT("cooldownTime"))
		{
			if (SkillSetData.cooldownTime <= FLT_EPSILON)
				SkillSetData.cooldownTime = 0.1f;
		}
		else
			if (PropertyName == TEXT("targetCount"))
			{
				if (SkillSetData.targetFilterInfo.targetCount <= 0)
					SkillSetData.targetFilterInfo.targetCount = 1;
			}*/
		if (PropertyName == TEXT("id"))
		{
			// 중복 체크
			UGsTableManagerEditor& Dman = UGsTableManagerEditor::GetInstance();
			UGsTable* SkillSetTable = Dman.GetTable(FGsSchemaSkillReaction::StaticStruct());
			const FGsSchemaSkillReaction* row = nullptr;
			if (SkillSetTable->FindRow(FName(*FString::FromInt(SkillSetData.id)), row))
			{
				//FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillSectionData", "Failed id Setting", "SkillSetData id 가 SkillSet 테이블에 이미 있습니다.(중복)"));
				FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed id Setting", "SkillEditor::ReactionSectionData - The SkillSetData.id is a duplicate."));
				//SkillSetData.id = 0;
			}
		}
		else
		if (PropertyName == TEXT("category"))
		{
			if (SkillEditorDataAsset)
				SkillEditorDataAsset->ChangeSectionCategory(static_cast<int32>(SkillSetData.category));
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UReactionSectionData::CopyData(USkillMetaData* InSkillMetaData)
{
	UReactionSectionData* BaseData = Cast<UReactionSectionData>(InSkillMetaData);
	if (BaseData == nullptr) return;

	SkillSetData = BaseData->SkillSetData;
}

void UReactionSectionData::NotifiesChanged(USkillEditorDataAsset* InSkillEditorDataAsset, int32 InSectionIndex)
{
	// 기존에 있던 스킬 노티파이를 리셋 하고 현재 섹션길아 안에 포함된것만 넣는다.
	//float StartTime = 0;
	//float EndTime = 0;
	//AnimMontage->GetSectionStartAndEndTime(InSectionIndex, StartTime, EndTime);

	// 스킬 노티파이 이벤트를 가져온다.
	/*TArray< FAnimNotifyEvent* > SkillNotifyEvent;
	for (int32 index = 0; index < AnimMontage->GetNotifies().Num(); ++index)
	{
		if (AnimMontage->GetNotifies()[index].SkillNotify != nullptr)
			SkillNotifyEvent.Add(&(AnimMontage->GetNotifies()[index]));
	}*/

	// 해당 섹션안에 있는 스킬 노티파이만 추린다. ( 현재 CommonSection은 노티파이들을 따로 사용하지 않아서 그리고 리스트 사용안할거 같지만 일단 추가는 해달라고 요청옴(범님))
	/*SkillSetData.notifyInfoList.Reset();
	TArray< USkillNotify*> SkillNotifys;
	for (int32 index = 0; index < InSkillEditorDataAsset->SkillNotifies.Num(); ++index)
	{
		if ((StartTime <= InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue())
			&& (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() < EndTime))
		{
			if (InSkillEditorDataAsset->SkillNotifies[index].SkillNotify != nullptr)
			{
				int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();
				// [LBY : 20.09.11] int 형 참조 정보에서 FGsSchemaSkillNotifySetRow 참조로 변경
				//SkillSetData.notifyInfoList[addIndex].skillNotifyId = SkillNotifyEvent[index]->SkillNotify->SkillNotifyId;
				SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
				SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime;
			}
		}
	}*/
}

void UReactionSectionData::SetData(USkillEditorDataAsset* InSkillEditorDataAsset, int32 InSectionIndex)
{
	SkillEditorDataAsset = InSkillEditorDataAsset;

	const FString& AssetName = FPackageName::GetShortName(*InSkillEditorDataAsset->GetOutermost()->GetPathName());
	SkillSetData.nameText = AssetName;

	SkillSetData.category = InSkillEditorDataAsset->ReactionCategory;

	UAnimMontage* AnimMontage = Cast<UAnimMontage>(InSkillEditorDataAsset->AnimMontage);
	if (AnimMontage == nullptr) return;

	SkillSetData.aniPath = AnimMontage->GetPathName();

	if (InSkillEditorDataAsset->AutoTotalRuntimeSetting)
	{
		if (InSkillEditorDataAsset->IsDivide() == false)
			SkillSetData.totalRuntime = AnimMontage->SequenceLength;
		else
			SkillSetData.totalRuntime = AnimMontage->GetSectionLength(InSectionIndex);
	}

	//// 기존에 있던 스킬 노티파이를 리셋 하고 현재 섹션길아 안에 포함된것만 넣는다.
	//float StartTime = 0;
	//float EndTime = 0;
	AnimMontage->GetSectionStartAndEndTime(InSectionIndex, StartTime, EndTime);

	// Rootmotion 정보 추출
	SetRootmotionData(AnimMontage);

	//// 스킬 노티파이 이벤트를 가져온다.
	//TArray< FAnimNotifyEvent* > SkillNotifyEvent;
	//for (int32 index = 0; index < AnimMontage->GetNotifies().Num(); ++index)
	//{
	//	if (AnimMontage->GetNotifies()[index].SkillNotify != nullptr)
	//		SkillNotifyEvent.Add(&(AnimMontage->GetNotifies()[index]));
	//}

	//// 해당 섹션안에 있는 스킬 노티파이만 추린다. ( 현재 CommonSection은 노티파이들을 따로 사용하지 않아서 그리고 리스트 사용안할거 같지만 일단 추가는 해달라고 요청옴(범님))
	//SkillSetData.notifyInfoList.Reset();
	//TArray< USkillNotify*> SkillNotifys;
	//for (int32 index = 0; index < SkillNotifyEvent.Num(); ++index)
	//{
	//	if ((StartTime <= SkillNotifyEvent[index]->GetLinkValue())
	//		&& (SkillNotifyEvent[index]->GetLinkValue() < EndTime))
	//	{
	//		if (SkillNotifyEvent[index]->SkillNotify != nullptr)
	//		{
	//			int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();
	//			// [LBY : 20.09.11] int 형 참조 정보에서 FGsSchemaSkillNotifySetRow 참조로 변경
	//			//SkillSetData.notifyInfoList[addIndex].skillNotifyId = SkillNotifyEvent[index]->SkillNotify->SkillNotifyId;
	//			SkillSetData.notifyInfoList[addIndex].repeatCount = SkillNotifyEvent[index]->SkillNotify->RepeatCount;
	//			SkillSetData.notifyInfoList[addIndex].offset = SkillNotifyEvent[index]->SkillNotify->Offset = SkillNotifyEvent[index]->GetLinkValue() - StartTime;
	//		}
	//	}
	//}

	//InSkillEditorDataAsset->ListSkillSetData.Add(this);
}

void UReactionSectionData::ExtractRootMotion(UAnimMontage* InMontage, float InOffset, const FTransform& InTransform,
	OUT FGsSchemaRootmotionSet* InRootmotionSetData)
{
	FTransform rootmotionTm = InMontage->ExtractRootMotionFromTrackRange(0.f, InOffset);
	// FTransform convertTm = rootmotionTm.GetRelativeTransform(InTransform);

	const FVector DeltaTranslation = rootmotionTm.GetTranslation() - InTransform.GetTranslation();
	const FVector dir = InTransform.GetRotation() * DeltaTranslation;

	AddRootmotionSet(dir, rootmotionTm.GetRotation().Vector(), InOffset, InRootmotionSetData);
}

float UReactionSectionData::AppendExtractRootMotion(UAnimMontage* InMontage, float InTotalLen,
	const FTransform& InTransform, OUT FGsSchemaRootmotionSet* InRootmotionSetData)
{
	float startLoopPos = 0.f;
	float endLoopPos = 0.f;
	float startReactionEndPos = 0.f;
	float endReactionEndPos = 0.f;

	int loopIdx = InMontage->GetSectionIndex(TEXT("Loop"));
	int reactionEndIdx = InMontage->GetSectionIndex(TEXT("ReactionEnd"));
	float loopLength = InMontage->GetSectionLength(loopIdx);
	float reactionEndLength = InMontage->GetSectionLength(reactionEndIdx);
	InMontage->GetSectionStartAndEndTime(loopIdx, startLoopPos, endLoopPos);
	InMontage->GetSectionStartAndEndTime(reactionEndIdx, startReactionEndPos, endReactionEndPos);
	
	// 첫째 자리 소숫점 오차 문제로 인해 반올림 처리
	// 현재는 3프레임(0.1초) 미만을 지원하지 않음
	loopLength = FMath::RoundToFloat(loopLength * 10.f) / 10.f;
	reactionEndLength = FMath::RoundToFloat(reactionEndLength * 10.f) / 10.f;

	// Loop구간 이전까지 0.1초 간격으로 처리
	int roundupNumber = startLoopPos * 10;
	roundupNumber = FMath::Max(roundupNumber, 1);
	float offset = 0.f;
	for (int i = 0; i < roundupNumber; ++i)
	{
		offset = i * 0.1f;
		ExtractRootMotion(InMontage, offset, InTransform, InRootmotionSetData);
	}
	
	// 0.1초 간격으로 떨어지지 않아서 나머지 시간만 대입
	if (startLoopPos - offset > 0.01f)
	{
		offset = startLoopPos;
		ExtractRootMotion(InMontage, offset, InTransform, InRootmotionSetData);
	}

	// loop 구간 이전 까지 이동거리
	FTransform appendRootMotionTransform = InMontage->ExtractRootMotionFromTrackRange(0.f, startLoopPos);
	// loop 구간 이동 거리
	FTransform loopTransform = InMontage->ExtractRootMotionFromTrackRange(startLoopPos, startLoopPos + loopLength);
	// Loop 구간 반복
	float loopTime = InTotalLen - (startLoopPos + reactionEndLength);
	int loopcnt = (loopTime / loopLength);
	for (int i = 0; i < loopcnt; ++i)
	{
		appendRootMotionTransform.AddToTranslation(loopTransform.GetTranslation());
		FVector dir = InTransform.GetRotation() * appendRootMotionTransform.GetTranslation();
		offset = startLoopPos + (loopLength * (i + 1));
		AddRootmotionSet(dir, loopTransform.GetRotation().Vector(), offset, InRootmotionSetData);
	}

	// Loop 이후(reactionEnd) 구간 0.1초 간격으로 처리
	roundupNumber = reactionEndLength * 10;	
	float animationOffset = startReactionEndPos;
	for (int i = 0; i < roundupNumber; ++i)
	{
		FTransform endTransform = InMontage->ExtractRootMotionFromTrackRange(animationOffset, animationOffset + 0.1f);
		appendRootMotionTransform.AddToTranslation(endTransform.GetTranslation());
		FVector dir = InTransform.GetRotation() * appendRootMotionTransform.GetTranslation();
		animationOffset = startReactionEndPos + ((i + 1) * 0.1f);
		offset += 0.1f;
		AddRootmotionSet(dir, endTransform.GetRotation().Vector(), offset, InRootmotionSetData);
	}

	// 최종 나머지 시간 처리
	float remain = InTotalLen - offset;
	if (remain > 0.01f)
	{
		offset = endReactionEndPos;
		FTransform tm = InMontage->ExtractRootMotionFromTrackRange(endReactionEndPos - remain, offset);
		appendRootMotionTransform.AddToTranslation(tm.GetTranslation());
		FVector dir = InTransform.GetRotation() * appendRootMotionTransform.GetTranslation();
		AddRootmotionSet(dir, tm.GetRotation().Vector(), offset, InRootmotionSetData);
	}

	return offset;
}

void UReactionSectionData::AddRootmotionSet(const FVector& InMoveDir, const FVector& InObjDir, float InOffset,
	OUT FGsSchemaRootmotionSet* InRootmotionSetData)
{
	// FGsDir::dirToAngle(dir);
	float angle = (short)(FMath::Atan2(InMoveDir.Y, InMoveDir.X) * 32768 / 3.141592f);
	float objectAngle = (short)(FMath::Atan2(InObjDir.Y, InObjDir.X) * 32768 / 3.141592f);
	FGsSchemaRootmotionInfo newInfo;
	newInfo.Offset = InOffset;
	newInfo.Angle = angle;
	newInfo.ObjectAngle = objectAngle;
	newInfo.Pos = InMoveDir;
	newInfo.Distance = InMoveDir.Size();

	// OutPut
	InRootmotionSetData->timeLine.Add(newInfo);
}

void UReactionSectionData::SetRootmotionData(UAnimMontage* InMontage)
{
	if (InMontage && InMontage->HasRootMotion())
	{
		// rootmotion 테이블 로드
		UGsDataTable* DTGsRootmotionSet = LoadObject<UGsDataTable>(
			nullptr, TEXT("/Game/DataCenter/DataTable/Skill/RootMotion/DTGsRootmotionSet.DTGsRootmotionSet"));
		if (nullptr == DTGsRootmotionSet)
		{
			return;
		}

		// 리액션 타입 넘버링 부여 1
		FName RowName = FName(*FString::Printf(TEXT("1%d"), SkillSetData.id));
		
		FGsSchemaRootmotionSet newRootmotion;
		newRootmotion.id = FCString::Atoi(*RowName.ToString());

		// 서버에도 반영되야 되서 totalRuntime을 총길이로 얻기
		float totalLen = SkillSetData.totalRuntime;
		// 클라이언트에서 모델 데이터는 x축을 전방으로 하기 위해 90도 회전이 있다.
		// 애니메이션 원본 데이터는 Y축 전방이므로 90도 회전 적용을 함
		FTransform originTm(FRotator(0.f, -90.f, 0.f));
		int roundupNumber = totalLen * 10;
		
		float offset = 0.f;
		// Loop구간 존재 유무 확인		
		int loopIdx = InMontage->GetSectionIndex(TEXT("Loop"));
		if (loopIdx != INDEX_NONE)
		{
			offset = AppendExtractRootMotion(InMontage, totalLen, originTm, &newRootmotion);
		}
		else
		{			
			// 0.1초 간격으로 입력
			for (int i = 0; i < roundupNumber + 1; ++i)
			{
				offset = i * 0.1f;

				ExtractRootMotion(InMontage, offset, originTm, &newRootmotion);
			}
			// 최종 나머지 시간 계산
			if (totalLen - offset > 0.01f)
			{
				offset = totalLen;
				ExtractRootMotion(InMontage, offset, originTm, &newRootmotion);
			}

		}

		// 기존 데이터 검색
		if (FGsSchemaRootmotionSet* rootmotionSet =
			DTGsRootmotionSet->FindRow<FGsSchemaRootmotionSet>(RowName, FString()))
		{
			// 필요 정보 설정 
			newRootmotion.collisionDisable = rootmotionSet->collisionDisable;
			// 기존 정보 지우기
			DTGsRootmotionSet->RemoveRow(RowName);
		}
		DTGsRootmotionSet->AddRow(RowName, newRootmotion);

		// RowPick 연결
		SkillSetData.rootMotionId.DataTable = DTGsRootmotionSet;
		SkillSetData.rootMotionId.RowName = RowName;

		// 테이블 저장
		DTGsRootmotionSet->SavePackage();
	}
}
#endif