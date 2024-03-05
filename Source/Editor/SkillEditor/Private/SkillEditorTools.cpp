//#include "SkillEditor/Public/SkillEditorTools.h"
#include "SkillEditorTools.h"

#include "SkillEditor.h"
#include "Data/SkillEditorDataAsset.h"
#include "SkillNotify/CollisionSkillNotify.h"
#include "SkillNotify/CancleSkillNotify.h"
#include "SkillSection/SkillSectionData.h"
#include "SkillSection/CommonSectionData.h"
#include "SkillSection/ReactionSectionData.h"
#include "SkillNotify/ProjectileSkillNotify.h"
#include "SkillNotify/GuideEffectSkillNotify.h"
#include "SkillNotify/CutremineSkillNotify.h"
#include "SkillNotify/HitSkillNotify.h"
#include "SkillNotify/SkillNotifyBase.h"
#include "SkillEditorViewportClient.h"

//#include "GameObject/Skill/GsSkillBase.h"
//#include "SharedFuncs.h"

#include "LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogSkillEditorTools, Log, All);

namespace SkillEditorTools
{
	void MontageEditorDataToEditorData(USkillEditorDataAsset* SkillEditorDataAsset)
	{	
		// 2020/10/26 - 제거 예정, BSAM
		//if (ensure(SkillEditorDataAsset) && ensure(SkillEditorDataAsset->AnimMontage))
		//{
		//	FSkillEditorViewportClient::DrawManager.ClearDrawData();
		//	SkillEditorDataAsset->ListSkillSetData.Reset();
		//	UAnimMontage* AnimMontage = Cast<UAnimMontage>(SkillEditorDataAsset->AnimMontage);
		//	if (AnimMontage == nullptr) return;

		//	ActionCategory SkillSectionActionCategory = SkillEditorDataAsset->ActionCategory;

		//	// 섹션 에 있는 스킬 정보 세팅(== SkillSection)
		//	for (int32 SectionIndex = 0; SectionIndex < AnimMontage->GetCompositeSections().Num(); ++SectionIndex)
		//	{
		//		USkillMetaData*& SkillMetaData = AnimMontage->GetAnimCompositeSection(SectionIndex).GetSkillSetMetaData();

		//		if (SkillSectionActionCategory == ActionCategory::SKILL)
		//		{
		//			if (SkillMetaData == nullptr || Cast<USkillSectionData>(SkillMetaData) == nullptr)
		//				SkillMetaData = NewObject<USkillSectionData>(USkillSectionData::StaticClass());

		//			Cast<USkillSectionData>(SkillMetaData)->SetData(SkillEditorDataAsset, SectionIndex);
		//		}
		//		else
		//		if (SkillSectionActionCategory == ActionCategory::COMMON)
		//		{
		//			if (SkillMetaData == nullptr || Cast<UCommonSectionData>(SkillMetaData) == nullptr)
		//				SkillMetaData = NewObject<UCommonSectionData>(UCommonSectionData::StaticClass());

		//			Cast<UCommonSectionData>(SkillMetaData)->SetData(SkillEditorDataAsset, SectionIndex);
		//		}
		//		else
		//		if (SkillSectionActionCategory == ActionCategory::REACTION)
		//		{
		//			if (SkillMetaData == nullptr || Cast<UReactionSectionData>(SkillMetaData) == nullptr)
		//				SkillMetaData = NewObject<UReactionSectionData>(UReactionSectionData::StaticClass());

		//			Cast<UReactionSectionData>(SkillMetaData)->SetData(SkillEditorDataAsset, SectionIndex);
		//		}
		//	}

		//	if (SkillSectionActionCategory == ActionCategory::SKILL)
		//	{
		//		for (int32 SectionIndex = 0; SectionIndex < AnimMontage->GetCompositeSections().Num(); ++SectionIndex)
		//		{
		//			int nextSectionIndex = SectionIndex + 1;
		//			if (nextSectionIndex == AnimMontage->GetCompositeSections().Num())
		//				nextSectionIndex = 0;

		//			USkillMetaData*& SkillMetaData = AnimMontage->GetAnimCompositeSection(SectionIndex).GetSkillSetMetaData();
		//			USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);

		//			USkillMetaData*& NextSkillMetaData = AnimMontage->GetAnimCompositeSection(nextSectionIndex).GetSkillSetMetaData();
		//			USkillSectionData* NextSkillSectionData = Cast< USkillSectionData >(NextSkillMetaData);

		//			SkillSectionData->SkillSetData.chainInfo.comboInfo.nextComboSkillId = NextSkillSectionData->SkillSetData.id;
		//			// 범님이 현재로 다시바꿔달라고 요청(원래 현재거였는데...)
		//			SkillSectionData->SkillSetData.chainInfo.comboInfo.sectionName = AnimMontage->GetAnimCompositeSection(SectionIndex).SectionName;//AnimMontage->GetAnimCompositeSection(nextSectionIndex).SectionName;
		//		}
		//	}
		//	
		//	SkillEditorDataAsset->MarkPackageDirty();
		//}
	}

	TArray<USkillNotify*> GetSkillNotifys(UAnimMontage* InAnimMontage)
	{
		// 스킬 노티파이 이벤트를 가져온다.
		TArray< FAnimNotifyEvent* > SkillNotifyEvent;
		for (int32 index = 0; index < InAnimMontage->GetNotifies().Num(); ++index)
		{
			if (InAnimMontage->GetNotifies()[index].SkillNotify != nullptr)
				SkillNotifyEvent.Add(&(InAnimMontage->GetNotifies()[index]));
		}

		// 해당 섹션안에 있는 스킬 노티파이만 추린다.
		TArray< USkillNotify* > SkillNotifys;
		//TArray< UCollisionSkillNotify* > CollisionSkillNotifys;
		for (int32 index = 0; index < SkillNotifyEvent.Num(); ++index)
		{
			SkillNotifys.Add(SkillNotifyEvent[index]->SkillNotify);
		}

		return SkillNotifys;
	}
}