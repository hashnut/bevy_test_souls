// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonSectionData.h"

#include "SkillNotify/CutremineSkillNotify.h"
#include "SkillNotify/CollisionSkillNotify.h"

#include "Data/SkillEditorDataAsset.h"
#include "GsTableManagerEditor.h"
#include "Misc/MessageDialog.h"

#if WITH_EDITOR
void UCommonSectionData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
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
			UGsTable* SkillSetTable = Dman.GetTable(FGsSchemaSkillCommon::StaticStruct());
			const FGsSchemaSkillCommon* row = nullptr;
			if (SkillSetTable->FindRow(FName(*FString::FromInt(SkillSetData.id)), row))
			{
				//FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillSectionData", "Failed id Setting", "SkillSetData id 가 SkillSet 테이블에 이미 있습니다.(중복)"));
				FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed id Setting", "SkillEditor::CommonSectionData - The SkillSetData.id is a duplicate."));
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

void UCommonSectionData::CopyData(USkillMetaData* InSkillMetaData)
{
	UCommonSectionData* BaseData = Cast<UCommonSectionData>(InSkillMetaData);
	if (BaseData == nullptr) return;

	SkillSetData = BaseData->SkillSetData;
}

void UCommonSectionData::NotifiesChanged(USkillEditorDataAsset* InSkillEditorDataAsset, int32 InSectionIndex)
{
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

void UCommonSectionData::SetData(USkillEditorDataAsset* InSkillEditorDataAsset, int32 InSectionIndex)
{
	SkillEditorDataAsset = InSkillEditorDataAsset;

	const FString& AssetName = FPackageName::GetShortName(*InSkillEditorDataAsset->GetOutermost()->GetPathName());
	SkillSetData.nameText = AssetName;

	SkillSetData.category = InSkillEditorDataAsset->CommonCategory;

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

	// 기존에 있던 스킬 노티파이를 리셋 하고 현재 섹션길아 안에 포함된것만 넣는다.	
	AnimMontage->GetSectionStartAndEndTime(InSectionIndex, StartTime, EndTime);

	//// 기존에 있던 스킬 노티파이를 리셋 하고 현재 섹션길아 안에 포함된것만 넣는다.
	//float StartTime = 0;
	//float EndTime = 0;
	//AnimMontage->GetSectionStartAndEndTime(InSectionIndex, StartTime, EndTime);

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
#endif