#include "SkillEditorDataAsset.h"
#include "SkillSection/SkillSectionData.h"
#include "SkillSection/CommonSectionData.h"
#include "SkillSection/ReactionSectionData.h"
#include "Runtime/CoreUObject/Public/Misc/PackageName.h"
#include "UObject/UnrealType.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Misc/MessageDialog.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SkillEditorViewportClient.h"
#include "Animation/SkillMetaData.h"

#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "SkillNotify/HitSkillNotify.h"
#include "SkillNotify/ProjectileSkillNotify.h"

#include "GsTableManagerEditor.h"
#include "DataTableConverter/GsDataTableConvertBase.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/Client/GsSchemaClientSkillLevelInfo.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"
#include "../SKillNotify/GuideEffectSkillNotify.h"
#include "Editor/UnrealEd/Public/FileHelpers.h"

#include "Animation/AnimNotify/AnimNotifyState_SkeletalMesh.h"
#include "Animation/AnimNotify/AnimNotifyState_StaticMesh.h"


USkillEditorDataAsset::USkillEditorDataAsset()
{	
	// default 값
	BeforeActionCategory = ActionCategory = ActionCategory::SKILL;
	BeforeSkillCategory = SkillCategory = SkillCategory::NORMAL;
	BeforeCommonCategory = CommonCategory = CommonActionType::SPAWN;
	BeforeReactionCategory = ReactionCategory = EReactionType::NONE;

	AutoTotalRuntimeSetting = true;
	//UseSkillNotifyCollision = false;

	SkillCategoryShow();
	CommonCategoryHide();
	ReactionCategoryHide();

	AnimMontageShow();
	AnimSequenceHide();
}

#if WITH_EDITORONLY_DATA
void USkillEditorDataAsset::CommonCategoryHide()
{
	if (UClass* Class = GetClass())
	{
		if (FProperty* Property = Class->FindPropertyByName(TEXT("CommonCategory")))
		{
			Property->PropertyFlags = CPF_None;
		}
	}
}

void USkillEditorDataAsset::CommonCategoryShow()
{
	if (UClass* Class = GetClass())
	{
		if (FProperty* Property = Class->FindPropertyByName(TEXT("CommonCategory")))
		{
			Property->PropertyFlags = CPF_Edit | CPF_ZeroConstructor | CPF_IsPlainOldData | CPF_NoDestructor | CPF_HasGetValueTypeHash | CPF_NativeAccessSpecifierPublic;
		}
	}
}

void USkillEditorDataAsset::ReactionCategoryHide()
{
	if (UClass* Class = GetClass())
	{
		if (FProperty* Property = Class->FindPropertyByName(TEXT("ReactionCategory")))
		{
			Property->PropertyFlags = CPF_None;
		}
	}
}

void USkillEditorDataAsset::ReactionCategoryShow()
{
	if (UClass* Class = GetClass())
	{
		if (FProperty* Property = Class->FindPropertyByName(TEXT("ReactionCategory")))
		{
			Property->PropertyFlags = CPF_Edit | CPF_ZeroConstructor | CPF_IsPlainOldData | CPF_NoDestructor | CPF_HasGetValueTypeHash | CPF_NativeAccessSpecifierPublic;
		}
	}
}

void USkillEditorDataAsset::SkillCategoryHide()
{
	if (UClass* Class = GetClass())
	{
		if (FProperty* Property = Class->FindPropertyByName(TEXT("SkillCategory")))
		{
			Property->PropertyFlags = CPF_None;
		}
	}
}

void USkillEditorDataAsset::SkillCategoryShow()
{
	if (UClass* Class = GetClass())
	{
		if (FProperty* Property = Class->FindPropertyByName(TEXT("SkillCategory")))
		{
			Property->PropertyFlags = CPF_Edit | CPF_ZeroConstructor | CPF_IsPlainOldData | CPF_NoDestructor | CPF_HasGetValueTypeHash | CPF_NativeAccessSpecifierPublic;
		}
	}
}

void USkillEditorDataAsset::AnimMontageHide()
{
	if (UClass* Class = GetClass())
	{
		if (FProperty* Property = Class->FindPropertyByName(TEXT("AnimMontage")))
		{
			Property->PropertyFlags = CPF_None;
		}
	}
}

void USkillEditorDataAsset::AnimMontageShow()
{
	if (UClass* Class = GetClass())
	{
		if (FProperty* Property = Class->FindPropertyByName(TEXT("AnimMontage")))
		{
			Property->PropertyFlags = CPF_Edit | CPF_ZeroConstructor | CPF_IsPlainOldData | CPF_NoDestructor | CPF_HasGetValueTypeHash | CPF_NativeAccessSpecifierPublic;
		}
	}
}

void USkillEditorDataAsset::AnimSequenceHide()
{
	if (UClass* Class = GetClass())
	{
		if (FProperty* Property = Class->FindPropertyByName(TEXT("AnimSequence")))
		{
			Property->PropertyFlags = CPF_None;
		}
	}
}

void USkillEditorDataAsset::AnimSequenceShow()
{
	if (UClass* Class = GetClass())
	{
		if (FProperty* Property = Class->FindPropertyByName(TEXT("AnimSequence")))
		{
			Property->PropertyFlags = CPF_Edit | CPF_ZeroConstructor | CPF_IsPlainOldData | CPF_NoDestructor | CPF_HasGetValueTypeHash | CPF_NativeAccessSpecifierPublic;
		}
	}
}

void USkillEditorDataAsset::ClearTable()
{
	UGsTableManagerEditor& Dman = UGsTableManagerEditor::GetInstance();
	if (UGsTable* SkillSetTable = Dman.GetTable(FGsSchemaSkillSet::StaticStruct()))
	{
		SkillSetTable->Empty();
	}
	if (UGsTable* skillNotifySetTable = Dman.GetTable(FGsSchemaSkillNotifySet::StaticStruct()))
	{
		skillNotifySetTable->Empty();
	}
}

void USkillEditorDataAsset::AddSkillsetDataTable(UGsTable* InSkillSetTable, USkillSectionData* InSkillSectionData)
{
	// Origin SkillNotifySet 테이블 얻기
	UGsDataTable* skillNotifySetTable = nullptr;
	const FString path(TEXT("/Game/DataCenter/DataTable/Skill/DTGsSkillNotifySet.DTGsSkillNotifySet"));
	if (!FGsDataTableConvertBase::LoadDataTable(path, skillNotifySetTable))
	{
		return;
	}
	const FText& assetName = FText::FromString(InSkillSectionData->SkillSetData.nameText);

	// skillSetData.notifyInfoList.skillNotifyId 내부 DataTable정보를 변경하기 위해
	// FGsSchemaSkillSet 데이터 복사
	FGsSchemaSkillSet skillSetData = InSkillSectionData->SkillSetData;
	// 컬럼별 유효성 체크
	if (skillSetData.skillTargetMethod != SkillTargetMethod::NON_TARGET && skillSetData.nontargetApplyCollisionScale)
	{
		FText failedMessage = FText::Format(
			NSLOCTEXT("SkillEditor", "Failed Export", "nontargetApplyCollisionScale값이 활성화 되어있지만 SkillTargetMethod 타입이 맞지 않습니다! {0}"), assetName);
		FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
		return;
	}

	// Collision 체크
	for (const FGsSchemaSkillCollision& el : skillSetData.collisionInfoList)
	{
		if (false == CheckSkillCollision(el))
		{
			FText failedMessage = FText::Format(
				NSLOCTEXT("SkillEditor", "Failed Export", "skillSetData.collisionInfoList값이 SkillCollisionType과 맞지 않습니다! {0}"), assetName);
			FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
			return;
		}
	}

	// 클라이언트용 NotifySet는 notifyInfoList로 이관 한다.
	if (skillSetData.clientNotifyInfoList.Num() > 0)
	{
		for (int i = 0; i < skillSetData.clientNotifyInfoList.Num(); ++i)
		{
			skillSetData.notifyInfoList.Add(skillSetData.clientNotifyInfoList[i]);
		}

		skillSetData.notifyInfoList.Sort([](const FGsSchemaSkillNotify& el1, const FGsSchemaSkillNotify& el2)
			{
				return el1.offset < el2.offset;
			});
	}

	for (int i = 0; i < skillSetData.notifyInfoList.Num(); ++i)
	{
		const FName& rowName = skillSetData.notifyInfoList[i].skillNotifyId.RowName;		
		// 원본 SkillNotifySet에서 검색
		const FGsSchemaSkillNotifySet* outTable = 
			skillNotifySetTable->FindRow<FGsSchemaSkillNotifySet>(rowName, skillNotifySetTable->GetName());
		// 없다면 원본 SkillNotifySet에 추가 한다.
		if (nullptr == outTable)
		{
			// GetRow() 함수는 원본 테이블 정보에서 읽어 온다. 
			// const FGsSchemaSkillNotifySet* skillNotifySet = skillSetData.notifyInfoList[i].skillNotifyId.GetRow();
			const UGsDataTable* currDataTable = skillSetData.notifyInfoList[i].skillNotifyId.DataTable;
			if (nullptr == currDataTable)
			{
				FText failedMessage = FText::Format(
					NSLOCTEXT("SkillEditor", "Failed Export", "skillSetData->notifyInfoList DataTable is empty : {0}"), assetName);
				FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
				continue;
			}

			const FGsSchemaSkillNotifySet* skillNotifySet = currDataTable->FindRow<FGsSchemaSkillNotifySet>(
				rowName, currDataTable->GetName());
			if (nullptr == skillNotifySet)
			{
				// skillNotify RowPick이 안되어 있는게 있을수 있다.
				FText failedMessage = FText::Format(
					NSLOCTEXT("SkillEditor", "Failed Export", "SkillNotifySet not found RowName : {0} {1}"), assetName,
					FText::FromName(rowName));
				FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
				continue;
			}
			
			// Collision 체크			
			for (const FGsSchemaSkillCollisionRow& el : skillNotifySet->collisionInfoList)
			{
				const FGsSchemaSkillCollision* skillCollision = el.GetRow();
				if (nullptr == skillCollision)
				{
					continue;
				}
				if (false == CheckSkillCollision(*skillCollision))
				{
					FText failedMessage = FText::Format(
						NSLOCTEXT("SkillEditor", "Failed Export", "skillNotifySet.collisionInfoList값이 SkillCollisionType과 맞지 않습니다! {0}, {1}"), assetName,
						FText::FromName(rowName));
					FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
					return;
				}
			}

			skillNotifySetTable->AddRow(rowName, *skillNotifySet);
			outTable = skillNotifySetTable->FindRow<FGsSchemaSkillNotifySet>(rowName, skillNotifySetTable->GetName());
		}

		// SkillNotifySet DataTable 경로 수정
		skillSetData.notifyInfoList[i].skillNotifyId.DataTable = skillNotifySetTable;
		skillSetData.notifyInfoList[i].skillNotifyId.RowName = rowName;
	}

	// 클라이언트용 NotifySet도 처리
// 	for (int i = 0; i < skillSetData.clientNotifyInfoList.Num(); ++i)
// 	{
// 		const FName& rowName = skillSetData.clientNotifyInfoList[i].skillNotifyId.RowName;
// 		const FGsSchemaSkillNotifySet* outTable =
// 			skillNotifySetTable->FindRow<FGsSchemaSkillNotifySet>(rowName, skillNotifySetTable->GetName());
// 		if (nullptr == outTable)
// 		{
// 			//const FGsSchemaSkillNotifySet* skillNotifySet = skillSetData.clientNotifyInfoList[i].skillNotifyId.GetRow();
// 			const UGsDataTable* currDataTable = skillSetData.clientNotifyInfoList[i].skillNotifyId.DataTable;
// 			if (nullptr == currDataTable)
// 			{
// 				FText failedMessage = FText::Format(
// 					NSLOCTEXT("SkillEditor", "Failed Export", "skillSetData->clientNotifyInfoList DataTable is empty : {0}"), assetName);
// 				FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
// 				continue;
// 			}
// 
// 			const FGsSchemaSkillNotifySet* skillNotifySet = currDataTable->FindRow<FGsSchemaSkillNotifySet>(
// 				rowName, currDataTable->GetName());
// 			if (nullptr == skillNotifySet)
// 			{
// 				// skillNotify RowPick이 안되어 있는게 있을수 있다.
// 				FText failedMessage = FText::Format(
// 					NSLOCTEXT("SkillEditor", "Failed Export", "SkillNotifySet not found RowName : {0} {1}"), assetName,
// 					FText::FromName(rowName));
// 				FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
// 				continue;
// 			}
// 
// 			skillNotifySetTable->AddRow(rowName, *skillNotifySet);
// 			outTable = skillNotifySetTable->FindRow<FGsSchemaSkillNotifySet>(rowName, skillNotifySetTable->GetName());
// 		}
// 
// 		// SkillNotifySet DataTable 경로 수정
// 		skillSetData.clientNotifyInfoList[i].skillNotifyId.DataTable = skillNotifySetTable;
// 		skillSetData.clientNotifyInfoList[i].skillNotifyId.RowName = rowName;
// 	}
// 	
	// SkillSet 테이블에 추가
	FGsSchemaSkillSet* outSkillSet = nullptr;
	InSkillSetTable->AddRow(skillSetData, outSkillSet);

	// 클라이언트 사용 SkillLevel 필요정보 추가
	AddSkillLevelSetDataTable(outSkillSet);
}

void USkillEditorDataAsset::AddSkillLevelSetDataTable(OUT FGsSchemaSkillSet* OutSkillSetData)
{
	UGsTableManagerEditor& Dman = UGsTableManagerEditor::GetInstance();

	// SkillLevelSet 데이터 불러오기
	UGsTableSkillLevelSet* skillLevelSetTable = Cast<UGsTableSkillLevelSet>(Dman.GetTable(FGsSchemaSkillLevelSet::StaticStruct()));
	if (nullptr == skillLevelSetTable)
	{
		return;
	}

	// 해당 스킬 LevelSet 얻기
	const FGsSchemaSkillLevelSet* skillLevelSet = nullptr;
	skillLevelSetTable->FindRowById(OutSkillSetData->id, skillLevelSet);
	if (nullptr == skillLevelSet)
	{
		return;
	}
	
	// 원본 SkillNotifySet 불러오기
	// DTGsSkillNotifySet인지 확인 할것
	UGsTableSkillNotifySet* skillNotifySetTable = Cast<UGsTableSkillNotifySet>(Dman.GetTable(FGsSchemaSkillNotifySet::StaticStruct()));
	if (nullptr == skillNotifySetTable)
	{
		return;
	}

	// AbnormalitySet 데이터 불러오기
	UGsTableAbnormalitySet* abnormalitySetTable = Cast<UGsTableAbnormalitySet>(Dman.GetTable(FGsSchemaAbnormalitySet::StaticStruct()));
	if (nullptr == abnormalitySetTable)
	{
		return;
	}	

	OutSkillSetData->clientLevelInfo.Empty();

	// 최대 레벨을 확인
	// 최대 레벨을 가장 마지막에 설정하지 않았을경우는 순환검색해야 할수 있다.
	int maxLevel = skillLevelSet->levelInfoList.Last().level;
	for (int i = 0; i <= maxLevel; ++i)
	{
		const FGsSchemaSkillLevelInfo* findLevelInfo = skillLevelSet->levelInfoList.FindByPredicate([i](
			const FGsSchemaSkillLevelInfo& el)
			{
				return el.level == i;
			});

		if (findLevelInfo)
		{			
			FGsSchemaSkillNotify newNotify;

			// 레벨셋 노티파이정보와 갯수가 다른 오류 확인
			if (OutSkillSetData->notifyInfoList.Num() != findLevelInfo->notifyInfoList.Num())
			{
				FString strSkillId = FString::FromInt(OutSkillSetData->id);
				FText failedMessage = FText::Format(
					NSLOCTEXT("SkillEditor", "Failed Export", "SkillLevelSet Notify Count not Sync!! SkillSetid {0}"),
					FText::FromString(strSkillId));
				FMessageDialog::Open(EAppMsgType::Ok, failedMessage);

				// 출력창에 히스토리 남기기
				UE_LOG(LogTemp, Error, TEXT("[SkillEditor] SkillLevelSet Notify Count not Sync!! SkillSetid %d"), OutSkillSetData->id);

				return;
			}

			for (int j = 0; j < OutSkillSetData->notifyInfoList.Num(); ++j)
			{
				FGsSchemaClientSkillLevelNotifyInfo newNotifyInfo;
				newNotifyInfo.offset = OutSkillSetData->notifyInfoList[j].offset;

				// 레벨셋에 있는 노티파이 정보 확인				
				int32 notifyid = findLevelInfo->notifyInfoList[j];
				if (notifyid == 0)
				{
					newNotifyInfo.skillNotify = OutSkillSetData->notifyInfoList[j].skillNotifyId;
				}
				else
				{
					const FGsSchemaSkillNotifySet* skillNotifySet = nullptr;
					skillNotifySetTable->FindRowById(notifyid, skillNotifySet);
					if (skillNotifySet)
					{
						newNotifyInfo.skillNotify.DataTable = skillNotifySetTable->GetCachedDataTable();
						newNotifyInfo.skillNotify.RowName = skillNotifySet->GetRowName();
					}
					else
					{
						// 레벨셋에만 존재하는 SkillNotifyset 정보인듯
						FString strSkillLevelId = FString::FromInt(skillLevelSet->skillId);
						FText failedMessage = FText::Format(
							NSLOCTEXT("SkillEditor", "Failed Export", "SkillLevelSet Notifyid not found!! SkillLevelSetid {0}"),
							FText::FromString(strSkillLevelId));
						FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
						continue;
					}					
				}

				// Collision 체크
				const FGsSchemaSkillNotifySet* skillNotifySet = newNotifyInfo.skillNotify.GetRow();
				for (const FGsSchemaSkillCollisionRow& el : skillNotifySet->collisionInfoList)
				{
					const FGsSchemaSkillCollision* skillCollision = el.GetRow();
					if (nullptr == skillCollision)
					{
						continue;
					}
					if (false == CheckSkillCollision(*skillCollision))
					{
						FString strskillLevelSetId = FString::FromInt(skillLevelSet->skillId);
						FString strSkillNotifySetId = FString::FromInt(skillNotifySet->id);

						FText failedMessage = FText::Format(
							NSLOCTEXT("SkillEditor", "Failed Export", "skillNotifySet.collisionInfoList값이 SkillCollisionType과 맞지 않습니다! SkillLevelSetId {0}, skillNotifySetId{1}"),
							FText::FromString(strskillLevelSetId),
							FText::FromString(strSkillNotifySetId));
						FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
						return;
					}
				}
			}
		}
	}
}

bool USkillEditorDataAsset::CheckSkillCollision(const FGsSchemaSkillCollision& InSkillCollision)
{
	if (InSkillCollision.type == SkillCollisionType::NONE || InSkillCollision.type == SkillCollisionType::MAX)
	{
		return InSkillCollision.angle == 0 && InSkillCollision.angleOffset == 0 && InSkillCollision.height == 0
			&& InSkillCollision.width == 0;
	}
	else if (InSkillCollision.type == SkillCollisionType::RECT)
	{
		return InSkillCollision.width != 0 && InSkillCollision.height != 0;
	}
	else if(InSkillCollision.type == SkillCollisionType::FAN)
	{
		return InSkillCollision.angle != 0 && InSkillCollision.maxRadius != 0;
	}
	else if(InSkillCollision.type == SkillCollisionType::CIRCLE)
	{
		return InSkillCollision.maxRadius != 0;
	}
	return false;
}
#endif

void USkillEditorDataAsset::CreateAnimSequenceSkill()
{
	if (AnimSequence)
	{
		UAnimSequenceSkill* NewAnimSequenceSkill = NewObject<UAnimSequenceSkill>(this);
		AnimSequenceSkill = NewAnimSequenceSkill;
		AnimSequenceSkill->SkillEditorMode = true;
		NewAnimSequenceSkill->CreateAnimationAndNotifyInit(AnimSequence->GetSkeleton(), AnimSequence);		
		
		// 스킬 노티파이를 추가하기.
		for (int32 i = 0; i < SkillNotifies.Num(); ++i)
		{
			if (SkillNotifies[i].SkillNotify != nullptr)
			{
				int32 addIndex = AnimSequenceSkill->GetNotifies().AddDefaulted();
				AnimSequenceSkill->GetNotifies()[addIndex] = SkillNotifies[i];

				USkillNotify* DuplicateSkillNotify = DuplicateObject<USkillNotify>(SkillNotifies[i].SkillNotify, this);
				if (DuplicateSkillNotify)
				{
					AnimSequenceSkill->GetNotifies()[addIndex].SkillNotify = DuplicateSkillNotify;
				}
			}
		}

		// 기존 원본에 있던 노티파이를 추가하기.
		for (int32 i = 0; i < AnimSequence->GetNotifies().Num(); ++i)
		{
			if (AnimSequence->GetNotifies()[i].Notify != nullptr)
			{
				int32 addIndex = AnimSequenceSkill->GetNotifies().AddDefaulted();
				AnimSequenceSkill->GetNotifies()[addIndex] = AnimSequence->GetNotifies()[i];

				UAnimNotify* DuplicateNotify = DuplicateObject<UAnimNotify>(AnimSequence->GetNotifies()[i].Notify, this);

				if (DuplicateNotify)
				{
					AnimSequenceSkill->GetNotifies()[addIndex].Notify = DuplicateNotify;
				}
			}
		}

		// 추가 적으로 원본 노티파이 스테이트도 추가 요청옴(https://jira.com2us.com/jira/browse/CHR-22680)
		for (int32 i = 0; i < AnimSequence->GetNotifies().Num(); ++i)
		{
			if (AnimSequence->GetNotifies()[i].NotifyStateClass != nullptr)
			{
				if (false == AnimSequence->GetNotifies()[i].NotifyStateClass->IsA<UAnimNotifyState_SkeletalMesh>()
					&& false == AnimSequence->GetNotifies()[i].NotifyStateClass->IsA<UAnimNotifyState_StaticMesh>())
					continue;

				int32 addIndex = AnimSequenceSkill->GetNotifies().AddDefaulted();
				AnimSequenceSkill->GetNotifies()[addIndex] = AnimSequence->GetNotifies()[i];

				UAnimNotifyState* DuplicateNotifyStateClass = DuplicateObject<UAnimNotifyState>(AnimSequence->GetNotifies()[i].NotifyStateClass, this);

				if (DuplicateNotifyStateClass)
				{
					AnimSequenceSkill->GetNotifies()[addIndex].NotifyStateClass = DuplicateNotifyStateClass;
				}
			}
		}

		BeforeAnimSequence = AnimSequence;
	}
	else
	{
		BeforeAnimSequence = nullptr;
	}
}

void USkillEditorDataAsset::CreateAnimSequenceSkillSection()
{
	if (AnimSequenceSkill)
	{
		FSkillEditorViewportClient::DrawManager.ClearDrawData();
		ListSkillSetData.Reset();
		ListSkillSetData.Add(NewObject<USkillSectionData>(this));
	}
}

void USkillEditorDataAsset::CreateAnimMontageSkill()
{
	if (AnimMontage)
	{
		UAnimMontageSkill* NewAnimMontageSkill = NewObject<UAnimMontageSkill>(this);
		AnimMontageSkill = NewAnimMontageSkill;
		AnimMontageSkill->SkillEditorMode = true;
		AnimMontageSkill->CreateMontageAndNotifyInit(AnimMontage->GetSkeleton(), AnimMontage);		

		// 스킬 노티파이를 추가하기.
		for (int32 i = 0; i < SkillNotifies.Num(); ++i)
		{
			if (SkillNotifies[i].SkillNotify != nullptr)
			{
				int32 addIndex = AnimMontageSkill->GetNotifies().AddDefaulted();
				AnimMontageSkill->GetNotifies()[addIndex] = SkillNotifies[i];

				USkillNotify* DuplicateSkillNotify = DuplicateObject<USkillNotify>(SkillNotifies[i].SkillNotify, this);
				if (DuplicateSkillNotify)
				{
					AnimMontageSkill->GetNotifies()[addIndex].SkillNotify = DuplicateSkillNotify;
				}
			}
		}

		// 기존 원본에 있던 노티파이를 추가하기.
		for (int32 i = 0; i < AnimMontage->GetNotifies().Num(); ++i)
		{
			if (AnimMontage->GetNotifies()[i].Notify != nullptr)
			{
				int32 addIndex = AnimMontageSkill->GetNotifies().AddDefaulted();
				AnimMontageSkill->GetNotifies()[addIndex] = AnimMontage->GetNotifies()[i];

				UAnimNotify* DuplicateNotify = DuplicateObject<UAnimNotify>(AnimMontage->GetNotifies()[i].Notify, this);

				if (DuplicateNotify)
				{
					AnimMontageSkill->GetNotifies()[addIndex].Notify = DuplicateNotify;
				}
			}
		}

		// 추가 적으로 원본 노티파이 스테이트도 추가 요청옴(https://jira.com2us.com/jira/browse/CHR-22680)
		for (int32 i = 0; i < AnimMontage->GetNotifies().Num(); ++i)
		{
			if (AnimMontage->GetNotifies()[i].NotifyStateClass != nullptr)
			{
				if (false == AnimMontage->GetNotifies()[i].NotifyStateClass->IsA<UAnimNotifyState_SkeletalMesh>()
					&& false == AnimMontage->GetNotifies()[i].NotifyStateClass->IsA<UAnimNotifyState_StaticMesh>())
					continue;

				int32 addIndex = AnimMontageSkill->GetNotifies().AddDefaulted();
				AnimMontageSkill->GetNotifies()[addIndex] = AnimMontage->GetNotifies()[i];

				UAnimNotifyState* DuplicateNotifyStateClass = DuplicateObject<UAnimNotifyState>(AnimMontage->GetNotifies()[i].NotifyStateClass, this);

				if (DuplicateNotifyStateClass)
				{
					AnimMontageSkill->GetNotifies()[addIndex].NotifyStateClass = DuplicateNotifyStateClass;
				}
			}
		}

		BeforeAnimMontage = AnimMontage;
	}
	else
	{
		BeforeAnimMontage = nullptr;
	}
}

void USkillEditorDataAsset::CreateAnimMontageSkillSection()
{
	if (AnimMontageSkill)
	{
		FSkillEditorViewportClient::DrawManager.ClearDrawData();
		ListSkillSetData.Reset();
		for (int32 SectionIndex = 0; SectionIndex < AnimMontageSkill->GetCompositeSections().Num(); ++SectionIndex)
		{
			if (ActionCategory == ActionCategory::SKILL)
			{
				ListSkillSetData.Add(NewObject<USkillSectionData>(this));
			}
			else
			if (ActionCategory == ActionCategory::COMMON)
			{
				ListSkillSetData.Add(NewObject<UCommonSectionData>(this));
			}
			else
			if (ActionCategory == ActionCategory::REACTION)
			{
				ListSkillSetData.Add(NewObject<UReactionSectionData>(this));
			}
		}
	}
}

void USkillEditorDataAsset::SkillSectionDataNextIdSet()
{
	if (ActionCategory == ActionCategory::SKILL)
	{
		for (int32 SectionIndex = 0; SectionIndex < ListSkillSetData.Num(); ++SectionIndex)
		{
			int nextSectionIndex = SectionIndex + 1;
			if (nextSectionIndex == ListSkillSetData.Num())
				nextSectionIndex = 0;

			USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
			USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);

			USkillMetaData*& NextSkillMetaData = ListSkillSetData[nextSectionIndex];
			USkillSectionData* NextSkillSectionData = Cast< USkillSectionData >(NextSkillMetaData);

			SkillSectionData->SkillSetData.sequenceInfo.comboInfo.nextComboSkillId = NextSkillSectionData->SkillSetData.id;
			if (AnimMontageSkill)
				SkillSectionData->SkillSetData.sequenceInfo.comboInfo.sectionName = AnimMontageSkill->GetAnimCompositeSection(SectionIndex).SectionName;
		}
	}
}

void USkillEditorDataAsset::SetSkillUesNotifyCollisionDraw(int32 InSectionIndex, bool InUseSkillNotifyCollision)
{
	for (int32 i = 0; i < SkillNotifies.Num(); ++i)
	{
		if (InSectionIndex == SkillNotifies[i].GetSegmentIndex())
		{
			UHitSkillNotify* HitSkillNotify = Cast< UHitSkillNotify>(SkillNotifies[i].SkillNotify);
			UProjectileSkillNotify* ProjectileSkillNotify = Cast<UProjectileSkillNotify>(SkillNotifies[i].SkillNotify);

			if (HitSkillNotify)
			{
				HitSkillNotify->UseSkillNotifyCollision = InUseSkillNotifyCollision;
				HitSkillNotify->SectionIndex = InSectionIndex;
				USkillMetaData*& SkillMetaData = ListSkillSetData[InSectionIndex];
				HitSkillNotify->SkillSectionData = Cast< USkillSectionData >(SkillMetaData);
			}
			else if (ProjectileSkillNotify)
			{
				ProjectileSkillNotify->UseSkillNotifyCollision = InUseSkillNotifyCollision;
				ProjectileSkillNotify->SectionIndex = InSectionIndex;
				USkillMetaData*& SkillMetaData = ListSkillSetData[InSectionIndex];
				ProjectileSkillNotify->SkillSectionData = Cast< USkillSectionData >(SkillMetaData);
			}
		}
		
	}
}

void USkillEditorDataAsset::SkillSectionDataSet()
{
	FSkillEditorViewportClient::DrawManager.ClearDrawData();
	FSkillEditorViewportClient::NotifyDrawManager.ClearDrawData();	

	for (int32 SectionIndex = 0; SectionIndex < ListSkillSetData.Num(); ++SectionIndex)
	{
		ListSkillSetData[SectionIndex]->SetData(this, SectionIndex);
	}

	if (ActionCategory == ActionCategory::COMMON)
	{
		float AllTotalRuntime = .0f;
		for (int32 SectionIndex = 0; SectionIndex < ListSkillSetData.Num(); ++SectionIndex)
		{
			USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
			UCommonSectionData* CommonSectionData = Cast< UCommonSectionData >(SkillMetaData);

			if (false == IsDivide())
				AllTotalRuntime = CommonSectionData->SkillSetData.totalRuntime;
		}

		if (false == IsDivide())
		{
			for (int32 SectionIndex = 0; SectionIndex < ListSkillSetData.Num(); ++SectionIndex)
			{
				USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
				UCommonSectionData* CommonSectionData = Cast< UCommonSectionData >(SkillMetaData);
				if (CommonSectionData)
				{
					CommonSectionData->SkillSetData.totalRuntime = AllTotalRuntime;
				}
			}
		}
	}
	else if (ActionCategory == ActionCategory::SKILL)
	{
		// Divide를 안하면, TotalRuntime은 색션 구분에 상관없이 총길이값을 설정 (모든 색션의 길이를 합한값)
		float AllTotalRuntime = .0f;
		for (int32 SectionIndex = 0; SectionIndex < ListSkillSetData.Num(); ++SectionIndex)
		{
			int nextSectionIndex = SectionIndex + 1;
			if (nextSectionIndex == ListSkillSetData.Num())
				nextSectionIndex = 0;

			USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
			USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);

			USkillMetaData*& NextSkillMetaData = ListSkillSetData[nextSectionIndex];
			USkillSectionData* NextSkillSectionData = Cast< USkillSectionData >(NextSkillMetaData);

			// 체인 스킬은 수동 입력
			if (SkillSectionData->SkillSetData.skillSequenceType != SkillSequenceType::CHAIN)
			{
				SkillSectionData->SkillSetData.sequenceInfo.comboInfo.nextComboSkillId = NextSkillSectionData->SkillSetData.id;
			}
			if (AnimMontageSkill)
			{
				if (AnimMontageSkill->IsValidSectionName(SkillSectionData->SkillSetData.sequenceInfo.comboInfo.sectionName))
				{
					SkillSectionData->SkillSetData.sequenceInfo.comboInfo.sectionName = AnimMontageSkill->GetAnimCompositeSection(SectionIndex).SectionName;
				}
			}

			if (false == IsDivide())
				AllTotalRuntime = SkillSectionData->SkillSetData.totalRuntime;
		}
		
		if (false == IsDivide())
		{
			for (int32 SectionIndex = 0; SectionIndex < ListSkillSetData.Num(); ++SectionIndex)
			{
				USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
				USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);
				if (SkillSectionData)
				{
					SkillSectionData->SkillSetData.totalRuntime = AllTotalRuntime;
				}
			}
		}

		// RateScale 계산 적용
		RateScale = 1.0f;
		if (AnimMontage)
		{
			AnimMontageSkill->RateScale = AnimMontage->RateScale;
			RateScale = 1.0f / AnimMontage->RateScale;
		}
		else if (AnimSequence)
		{
			AnimSequenceSkill->RateScale = AnimSequence->RateScale;
			RateScale = 1.0f / AnimSequence->RateScale;
		}

		for (int32 i = 0; i < ListSkillSetData.Num(); ++i)
		{
			USkillSectionData* SkillSectionData = Cast< USkillSectionData >(ListSkillSetData[i]);
			if (SkillSectionData)
			{
				SkillSectionData->SkillSetData.totalRuntime *= RateScale;
				for (int32 j = 0; j < SkillSectionData->SkillSetData.notifyInfoList.Num(); ++j)
					SkillSectionData->SkillSetData.notifyInfoList[j].offset *= RateScale;

				for (int32 k = 0; k < SkillSectionData->SkillSetData.clientNotifyInfoList.Num(); ++k)
					SkillSectionData->SkillSetData.clientNotifyInfoList[k].offset *= RateScale;
			}

			UCommonSectionData* CommonSectionData = Cast< UCommonSectionData >(ListSkillSetData[i]);
			if (CommonSectionData)
			{
				CommonSectionData->SkillSetData.totalRuntime *= RateScale;
			}

			UReactionSectionData* ReactionSectionData = Cast< UReactionSectionData >(ListSkillSetData[i]);
			if (ReactionSectionData)
			{
				ReactionSectionData->SkillSetData.totalRuntime *= RateScale;
			}
		}

		// Draw 셋팅
		FSkillEditorViewportClient::DrawManager.AddDrawDataList(ListSkillSetData.Num());
		FSkillEditorViewportClient::NotifyDrawManager.AddDrawDataList(ListSkillSetData.Num());

		for (int32 i = 0; i < ListSkillSetData.Num(); ++i)
		{
			USkillSectionData* SkillSectionData = Cast< USkillSectionData >(ListSkillSetData[i]);
			if (SkillSectionData)
			{
				if (false == SkillSectionData->SkillSetData.useSkillNotifyCollision)
				{
					for (int32 j = 0; j < SkillSectionData->SkillSetData.collisionInfoList.Num(); ++j)
					{
						DrawTools::FDrawData DrawData;
						DrawData.DrawType = DrawTools::EDrawType::None;
						if (SkillSectionData->SkillSetData.collisionInfoList[j].type == SkillCollisionType::CIRCLE)
							DrawData.DrawType = DrawTools::EDrawType::Circle;
						else if (SkillSectionData->SkillSetData.collisionInfoList[j].type == SkillCollisionType::FAN)
							DrawData.DrawType = DrawTools::EDrawType::Cone;
						else if (SkillSectionData->SkillSetData.collisionInfoList[j].type == SkillCollisionType::RECT)
							DrawData.DrawType = DrawTools::EDrawType::Rect;

						float EndTime = SkillSectionData->EndTime;

						DrawData.TM = FTransform::Identity;
						DrawData.DegAngle = SkillSectionData->SkillSetData.collisionInfoList[j].angle;
						DrawData.LifeTime = EndTime - SkillSectionData->StartTime;
						DrawData.StartTime = SkillSectionData->StartTime;
						DrawData.EndTime = EndTime;
						DrawData.Radius = SkillSectionData->SkillSetData.collisionInfoList[j].maxRadius;
						DrawData.Width = SkillSectionData->SkillSetData.collisionInfoList[j].width;
						DrawData.Height = SkillSectionData->SkillSetData.collisionInfoList[j].height;
						DrawData.Color = FColor(192, 255, 99, 128); //NotifyColor;
						DrawData.FillMode = true;
						DrawData.PosOffset = SkillSectionData->SkillSetData.collisionInfoList[j].posOffset;
						DrawData.OffsetX = SkillSectionData->SkillSetData.collisionInfoList[j].offsetX;
						DrawData.OffsetY = SkillSectionData->SkillSetData.collisionInfoList[j].offsetY;
						DrawData.AngleOffset = SkillSectionData->SkillSetData.collisionInfoList[j].angleOffset;
						DrawData.AngleOffset2 = SkillSectionData->SkillSetData.collisionInfoList[j].angleOffset2;
						DrawData.CollisionDraw = SkillSectionData->CollisionDraw;
						DrawData.MaxRadius = SkillSectionData->SkillSetData.collisionInfoList[j].maxRadius;
						DrawData.MinRadius = SkillSectionData->SkillSetData.collisionInfoList[j].minRadius;

						FSkillEditorViewportClient::DrawManager.PushDrawData(i, DrawData);
					}
				}				
			}
		}
	}
}

void USkillEditorDataAsset::Selected()
{
	//Modify();

	BeforeActionCategory = ActionCategory;
	BeforeSkillCategory = SkillCategory;
	BeforeCommonCategory = CommonCategory;
	BeforeReactionCategory = ReactionCategory;

	if (AnimMontage == nullptr && AnimSequence == nullptr)
		return;

	if (ActionCategory::SKILL == ActionCategory)
	{
		SkillCategoryShow();
		CommonCategoryHide();
		ReactionCategoryHide();

		SkillCategorySet SkillCategorySetResult = SharedFunc::GetSkillCategorySet(SkillCategory);
		if (SkillCategorySetResult == SkillCategorySet::BUFF || SkillCategorySetResult == SkillCategorySet::PASSIVE)
		{
			AnimMontageHide();
			AnimSequenceShow();

			AnimMontage = nullptr;
			AnimMontageSkill = nullptr;

			CreateAnimSequenceSkill();
			SkillSectionDataSet();
		}
		else
		{
			AnimMontageShow();
			AnimSequenceHide();

			AnimSequence = nullptr;
			AnimSequenceSkill = nullptr;

			CreateAnimMontageSkill();
			SkillSectionDataSet();
		}
	}
	else
	if (ActionCategory::REACTION == ActionCategory)
	{
		ReactionCategoryShow();
		SkillCategoryHide();
		CommonCategoryHide();

		AnimMontageShow();
		AnimSequenceHide();

		AnimSequence = nullptr;
		AnimSequenceSkill = nullptr;

		CreateAnimMontageSkill();
		SkillSectionDataSet();
	}
	else
	if (ActionCategory::COMMON == ActionCategory)
	{
		CommonCategoryShow();
		SkillCategoryHide();
		ReactionCategoryHide();

		AnimMontageShow();
		AnimSequenceHide();

		AnimSequence = nullptr;
		AnimSequenceSkill = nullptr;

		CreateAnimMontageSkill();
		SkillSectionDataSet();
	}
}

void USkillEditorDataAsset::Pate(USkillEditorDataAsset* InSource)
{
	if (InSource == nullptr)
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Pate", "복사할 소스가 없습니다."));
		return;
	}

	if (ActionCategory != InSource->ActionCategory)
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Pate", "액션 카테고리가 틀립니다."));
		return;
	}

	if (SkillCategory != InSource->SkillCategory)
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Pate", "스킬 카테고리가 틀립니다."));
		return;
	}

	if (CommonCategory != InSource->CommonCategory)
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Pate", "커먼 카테고리가 틀립니다."));
		return;
	}

	if (ReactionCategory != InSource->ReactionCategory)
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Pate", "리액션 카테고리가 틀립니다."));
		return;
	}

	if (AnimMontage == nullptr && AnimSequence == nullptr)
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Pate", "AnimMontage 혹은 AnimSequence가 비어있습니다.(둘중하나는 반드시 있어야 됩니다."));
		return;
	}

	if (false == (ListSkillSetData.Num() >= InSource->ListSkillSetData.Num()))
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Pate", "원본 ListSkillSetData 수보다 복사할 ListSkillSetData의 수가 작습니다.(같거나 커야됩니다)"));
		return;
	}

	Modify();

	for (int32 i = 0; i < InSource->ListSkillSetData.Num(); ++i)
	{
		if (ActionCategory == ActionCategory::SKILL)
		{
			USkillMetaData*& SkillMetaData = InSource->ListSkillSetData[i];
			ListSkillSetData[i]->CopyData(SkillMetaData);
		}
		else
		if (ActionCategory == ActionCategory::COMMON)
		{
			USkillMetaData*& SkillMetaData = InSource->ListSkillSetData[i];
			ListSkillSetData[i]->CopyData(SkillMetaData);
		}
		else
		if (ActionCategory == ActionCategory::REACTION)
		{
			USkillMetaData*& SkillMetaData = InSource->ListSkillSetData[i];
			ListSkillSetData[i]->CopyData(SkillMetaData);
		}
	}

	if (AnimMontageSkill)
	{
		UAnimMontageSkill* DuplicateAnimMontageSkill = DuplicateObject<UAnimMontageSkill>(InSource->AnimMontageSkill, this);
		AnimMontageSkill = DuplicateAnimMontageSkill;

		if (AnimMontageSkill)
		{
			for (int32 i = 0; i < AnimMontageSkill->GetNotifies().Num(); ++i)
			{
				// AnimNotify들은 기존에 것들이 레퍼런싱 되어 있기에, 해당을 끊꼬 새로 복제해준다.
				if (AnimMontageSkill->GetNotifies()[i].Notify != nullptr)
				{
					UAnimNotify* DuplicateNotify = DuplicateObject<UAnimNotify>(AnimMontageSkill->GetNotifies()[i].Notify, this);
					AnimMontageSkill->GetNotifies()[i].Notify = nullptr;

					AnimMontageSkill->GetNotifies()[i].Notify = DuplicateNotify;
				}
			}
		}
	}

	if (AnimSequenceSkill)
	{
		UAnimSequenceSkill* DuplicateAnimSequenceSkill = DuplicateObject<UAnimSequenceSkill>(InSource->AnimSequenceSkill, this);
		AnimSequenceSkill = DuplicateAnimSequenceSkill;

		if (AnimSequenceSkill)
		{
			for (int32 i = 0; i < AnimSequenceSkill->GetNotifies().Num(); ++i)
			{
				// AnimNotify들은 기존에 것들이 레퍼런싱 되어 있기에, 해당을 끊꼬 새로 복제해준다.
				if (AnimSequenceSkill->GetNotifies()[i].Notify != nullptr)
				{
					UAnimNotify* DuplicateNotify = DuplicateObject<UAnimNotify>(AnimSequenceSkill->GetNotifies()[i].Notify, this);
					AnimSequenceSkill->GetNotifies()[i].Notify = nullptr;

					AnimSequenceSkill->GetNotifies()[i].Notify = DuplicateNotify;
				}
			}
		}
	}
}

void USkillEditorDataAsset::NotifiesChangedEx(const TArray<UObject*>& InObjects)
{
	//Modify();

	if (AnimSequenceSkill)
	{
		SkillNotifies.Empty();
		for (int32 index = 0; index < InObjects.Num(); ++index)
		{
			USkillNotify* SelectedSkillNotify = Cast<USkillNotify>(InObjects[index]);
			for (int32 indexNotifie = 0; indexNotifie < AnimSequenceSkill->GetNotifies().Num(); ++indexNotifie)
			{
				USkillNotify* SkillNotify = Cast<USkillNotify>(AnimSequenceSkill->GetNotifies()[indexNotifie].SkillNotify);
				if (SkillNotify == nullptr)
					continue;

				if (SkillNotify == SelectedSkillNotify)
				{
					SkillNotifies.Add(AnimSequenceSkill->GetNotifies()[indexNotifie]);
				}
				else
				{
					USkillNotify* DuplicateSkillNotify = DuplicateObject<USkillNotify>(AnimSequenceSkill->GetNotifies()[indexNotifie].SkillNotify, this);
					AnimSequenceSkill->GetNotifies()[indexNotifie].SkillNotify = DuplicateSkillNotify;
					//AnimSequenceSkill->GetNotifies()[indexNotifie].SkillNotify->OnChangeSkillNotify = OnNotifyIdChanged;
					SkillNotifies.Add(AnimSequenceSkill->GetNotifies()[indexNotifie]);
				}
			}
		}

		if (InObjects.Num() <= 0)
		{
			if (AnimSequenceSkill)
			{
				SkillNotifies.Empty();
				for (int32 index = 0; index < AnimSequenceSkill->GetNotifies().Num(); ++index)
				{
					if (AnimSequenceSkill->GetNotifies()[index].SkillNotify != nullptr)
					{
						USkillNotify* DuplicateSkillNotify = DuplicateObject<USkillNotify>(AnimSequenceSkill->GetNotifies()[index].SkillNotify, this);
						AnimSequenceSkill->GetNotifies()[index].SkillNotify = DuplicateSkillNotify;
						//AnimSequenceSkill->GetNotifies()[index].SkillNotify->OnChangeSkillNotify = OnNotifyIdChanged;
						SkillNotifies.Add(AnimSequenceSkill->GetNotifies()[index]);
					}
				}
			}
		}
	}
	else
	if (AnimMontageSkill)
	{
		SkillNotifies.Empty();
		for (int32 index = 0; index < InObjects.Num(); ++index)
		{
			USkillNotify* SelectedSkillNotify = Cast<USkillNotify>(InObjects[index]);
			for (int32 indexNotifie = 0; indexNotifie < AnimMontageSkill->GetNotifies().Num(); ++indexNotifie)
			{
				USkillNotify* SkillNotify = Cast<USkillNotify>(AnimMontageSkill->GetNotifies()[indexNotifie].SkillNotify);
				if (SkillNotify == nullptr)
					continue;

				if (SkillNotify == SelectedSkillNotify)
				{
					SkillNotifies.Add(AnimMontageSkill->GetNotifies()[indexNotifie]);
				}
				else
				{
					USkillNotify* DuplicateSkillNotify = DuplicateObject<USkillNotify>(AnimMontageSkill->GetNotifies()[indexNotifie].SkillNotify, this);
					if (DuplicateSkillNotify != nullptr)
					{
						AnimMontageSkill->GetNotifies()[indexNotifie].SkillNotify = DuplicateSkillNotify;
						//AnimMontageSkill->GetNotifies()[indexNotifie].SkillNotify->OnChangeSkillNotify = OnNotifyIdChanged;

						SkillNotifies.Add(AnimMontageSkill->GetNotifies()[indexNotifie]);
					}
				}
			}
		}

		if (InObjects.Num() <= 0)
		{
			if (AnimMontageSkill)
			{
				SkillNotifies.Empty();
				for (int32 index = 0; index < AnimMontageSkill->GetNotifies().Num(); ++index)
				{
					if (AnimMontageSkill->GetNotifies()[index].SkillNotify != nullptr)
					{
						USkillNotify* DuplicateSkillNotify = DuplicateObject<USkillNotify>(AnimMontageSkill->GetNotifies()[index].SkillNotify, this);
						if (DuplicateSkillNotify != nullptr)
						{
							AnimMontageSkill->GetNotifies()[index].SkillNotify = DuplicateSkillNotify;
							//AnimMontageSkill->GetNotifies()[index].SkillNotify->OnChangeSkillNotify = OnNotifyIdChanged;

							SkillNotifies.Add(AnimMontageSkill->GetNotifies()[index]);
						}
					}
				}
			}
		}
	}

	for (int32 SectionIndex = 0; SectionIndex < ListSkillSetData.Num(); ++SectionIndex)
	{
		ListSkillSetData[SectionIndex]->NotifiesChanged(this, SectionIndex);
	}
}

//void USkillEditorDataAsset::NotifiesChanged()
//{	
//	Modify();
//
//	if (AnimSequenceSkill)
//	{
//		SkillNotifies.Empty();
//		for (int32 index = 0; index < AnimSequenceSkill->GetNotifies().Num(); ++index)
//		{		
//			SkillNotifies.Add(AnimSequenceSkill->GetNotifies()[index]);
//		}
//	}
//	else
//	if (AnimMontageSkill)
//	{
//		SkillNotifies.Empty();
//		for (int32 index = 0; index < AnimMontageSkill->GetSkillNotifies().Num(); ++index)
//		{			
//			SkillNotifies.Add(AnimMontageSkill->GetSkillNotifies()[index]);
//		}
//	}
//
//	for (int32 SectionIndex = 0; SectionIndex < ListSkillSetData.Num(); ++SectionIndex)
//	{
//		ListSkillSetData[SectionIndex]->NotifiesChanged(this, SectionIndex);
//	}
//}

void USkillEditorDataAsset::Convert()
{
	// 2021/02/03 - collisionInfo -> collisionInfoList 로변경 기존것을 List 0인덱스로 Copy
	/*if (ActionCategory == ActionCategory::SKILL
		&& !(SkillCategory == SkillCategory::BUFF)
		&& !(SkillCategory == SkillCategory::PASSIVE)
		&& ListSkillSetData.Num() > 0)*/
		// 일회용 작업 : 사본 SkillnotifySet0로드
	UGsDataTable* tempSkillNotifySet0 = nullptr;
	const FString path(TEXT("/Game/DataCenter/DataTable/Skill/DTGsSkillNotifySet/DTGsSkillNotifySet0.DTGsSkillNotifySet0"));
	FGsDataTableConvertBase::LoadDataTable(path, tempSkillNotifySet0);
	if (nullptr == tempSkillNotifySet0)
	{
		return;
	}
	//
	for (int32 index = 0; index < SkillNotifies.Num(); ++index)
	{
		if (UHitSkillNotify* hitSkillNotify = Cast< UHitSkillNotify>(SkillNotifies[index].SkillNotify))
		{
			hitSkillNotify->SkillNotifySet.DataTable = tempSkillNotifySet0;
		}
		else if (UGuideEffectSkillNotify* guideSkillNotify = Cast< UGuideEffectSkillNotify>(SkillNotifies[index].SkillNotify))
		{
			guideSkillNotify->SkillNotifySet.DataTable = tempSkillNotifySet0;
		}
		else if (UProjectileSkillNotify* projectileNotify = Cast< UProjectileSkillNotify>(SkillNotifies[index].SkillNotify))
		{
			projectileNotify->SkillNotifySet.DataTable = tempSkillNotifySet0;
		}
	}	

	// USkillSectionData 설정 변경 및 콤보 데이터 설정
	for (int32 i = 0; i < ListSkillSetData.Num(); ++i)
	{
		USkillMetaData*& SkillMetaData = ListSkillSetData[i];
		USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);
		if (SkillSectionData)
		{
			for (int j = 0; j < SkillSectionData->SkillSetData.notifyInfoList.Num(); ++j)
			{
				SkillSectionData->SkillSetData.notifyInfoList[j].skillNotifyId.DataTable = tempSkillNotifySet0;
			}
			for (int j = 0; j < SkillSectionData->SkillSetData.clientNotifyInfoList.Num(); ++j)
			{
				SkillSectionData->SkillSetData.clientNotifyInfoList[j].skillNotifyId.DataTable = tempSkillNotifySet0;
			}
			
			int nextSectionIndex = i + 1;
			if (nextSectionIndex == ListSkillSetData.Num())
				nextSectionIndex = 0;

			USkillMetaData*& NextSkillMetaData = ListSkillSetData[nextSectionIndex];
			USkillSectionData* NextSkillSectionData = Cast< USkillSectionData >(NextSkillMetaData);
			// 체인 스킬은 수동 입력
			if (SkillSectionData->SkillSetData.skillSequenceType != SkillSequenceType::CHAIN)
			{
				SkillSectionData->SkillSetData.sequenceInfo.comboInfo.nextComboSkillId = NextSkillSectionData->SkillSetData.id;
			}
			if (AnimMontageSkill)
				SkillSectionData->SkillSetData.sequenceInfo.comboInfo.sectionName = AnimMontageSkill->GetAnimCompositeSection(i).SectionName;
		}
	}

	FEditorFileUtils::SaveDirtyPackage(GetOutermost(), true, true, true, false, false, false);

	return;
	if(ListSkillSetData.Num() > 0)
	{
		Modify();

		for (int32 i = 0; i < ListSkillSetData.Num(); ++i)
		{
			USkillMetaData*& SkillMetaData = ListSkillSetData[i];
			USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);
			if (SkillSectionData)
			{
				SkillSectionData->SkillSetData.collisionInfoList.Reset();
				SkillSectionData->SkillSetData.collisionInfoList.AddDefaulted();
			}
		}
	}

	return;

	// 2020/11/12 - Combo에서 스킬셋(SkillSet(SkillMeta) UObject가 첫번째(index 0)와 마지막 인덱스가 같다. 마지막 인덱스로 0도 셋팅되어 있다. 다시 생성시에는 이상이 없고
	//              컨버팅 후 저장중에 생긴 문제로 보인다. 그래서 콤보만 찾아서 다시 생성 셋팅한다.
	if (ActionCategory == ActionCategory::SKILL && SkillCategory == SkillCategory::COMBO && ListSkillSetData.Num() > 0)
	{
		Modify();

		// 0번만 새로 만든다.
		ListSkillSetData[0] = NewObject<USkillSectionData>(this);
			
		// Table에서 가져와 셋팅해준다.
		UGsTableManagerEditor& Dman = UGsTableManagerEditor::GetInstance();
		USkillMetaData*& SkillMetaData = ListSkillSetData[0];
		USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);

		const FGsSchemaSkillSet* row = nullptr;
		UGsTable* SkillSetTable = Dman.GetTable(FGsSchemaSkillSet::StaticStruct());			

		// 마지막 id를 기반으로 테이블상 id 찾기
		int32 lastId = ListSkillSetData.Num() - 1;
		USkillMetaData*& LastSkillMetaData = ListSkillSetData[lastId];
		USkillSectionData* LastSkillSectionData = Cast< USkillSectionData >(LastSkillMetaData);

		int32 tableLastId = LastSkillSectionData == nullptr ? 0 : LastSkillSectionData->SkillSetData.id;
		int32 startId = tableLastId - (ListSkillSetData.Num() - 1);

		if (SkillSectionData)
		{				
			if (SkillSetTable->FindRow(FName(*FString::FromInt(startId)), row))
			{
				SkillSectionData->SkillSetData = *row;
			}
		}
	}

	return;

	// 2020/11/10 - Common, Reaction Category Converting - 추가 컨버팅, 실제 테이블 데이터와 스킬데이터애셋과 틀림
	UGsTableManagerEditor& Dman = UGsTableManagerEditor::GetInstance();	
	if (ListSkillSetData.Num() > 0)
	{
		USkillMetaData*& SkillMetaData = ListSkillSetData[0];
		USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);
		UReactionSectionData* ReactionSectionData = Cast< UReactionSectionData >(SkillMetaData);
		UCommonSectionData* CommonSectionData = Cast< UCommonSectionData >(SkillMetaData);
		if (SkillSectionData)
		{
			const FGsSchemaSkillSet* row = nullptr;
			UGsTable* SkillSetTable = Dman.GetTable(FGsSchemaSkillSet::StaticStruct());
			if (SkillSetTable)
			{
				if (SkillSetTable->FindRow(FName(*FString::FromInt(SkillSectionData->SkillSetData.id)), row))
				{
					SkillCategory = row->category;
					SkillSectionData->SkillSetData.category = row->category;
					for (int32 i = 1; i < ListSkillSetData.Num(); ++i)
					{
						SkillMetaData = ListSkillSetData[i];
						SkillSectionData = Cast< USkillSectionData >(SkillMetaData);
						if (SkillSectionData)
							SkillSectionData->SkillSetData.category = row->category;
					}
				}
			}
		}
		else
		if (ReactionSectionData)
		{
			const FGsSchemaSkillReaction* row = nullptr;
			UGsTable* SkillRecationTable = Dman.GetTable(FGsSchemaSkillReaction::StaticStruct());
			if (SkillRecationTable)
			{
				if (SkillRecationTable->FindRow(FName(*FString::FromInt(ReactionSectionData->SkillSetData.id)), row))
				{
					ReactionCategory = row->category;
					ReactionSectionData->SkillSetData.category = row->category;
					for (int32 i = 1; i < ListSkillSetData.Num(); ++i)
					{
						SkillMetaData = ListSkillSetData[i];
						ReactionSectionData = Cast< UReactionSectionData >(SkillMetaData);
						if (ReactionSectionData)
							ReactionSectionData->SkillSetData.category = row->category;
					}
				}
			}
		}
		else
		if (CommonSectionData)
		{
			const FGsSchemaSkillCommon* row = nullptr;
			UGsTable* SkillCommonTable = Dman.GetTable(FGsSchemaSkillCommon::StaticStruct());
			if (SkillCommonTable)
			{
				if (SkillCommonTable->FindRow(FName(*FString::FromInt(CommonSectionData->SkillSetData.id)), row))
				{
					CommonCategory = row->category;
					CommonSectionData->SkillSetData.category = row->category;
					for (int32 i = 1; i < ListSkillSetData.Num(); ++i)
					{
						SkillMetaData = ListSkillSetData[i];
						CommonSectionData = Cast< UCommonSectionData >(SkillMetaData);
						if (CommonSectionData)
							CommonSectionData->SkillSetData.category = row->category;
					}
				}
			}
		}
	}
	
	return;
	
	// 2020/11/10 - Common, Reaction Category Converting
	if (ListSkillSetData.Num() > 0)
	{		
		USkillMetaData*& SkillMetaData = ListSkillSetData[0];
		USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);
		UReactionSectionData* ReactionSectionData = Cast< UReactionSectionData >(SkillMetaData);
		UCommonSectionData* CommonSectionData = Cast< UCommonSectionData >(SkillMetaData);
		if (SkillSectionData)
		{
			SkillCategory = SkillSectionData->SkillSetData.category;
		}
		else
		if (ReactionSectionData)
		{
			ReactionCategory = ReactionSectionData->SkillSetData.category;
		}
		else
		if (CommonSectionData)
		{
			CommonCategory = CommonSectionData->SkillSetData.category;
		}
	}

	return;

	// json에서 특정 정보 가져오기(기존 Montage에서 사라졌음)
	{
		FString rootPaths = FPaths::RootDir();
		FString engineFolder = TEXT("UnrealEngine-4.24.3");
		uint32 rootSize = rootPaths.GetCharArray().Num();
		uint32 engineFolderSize = engineFolder.GetCharArray().Num();

		rootPaths.RemoveAt(rootSize - engineFolderSize, engineFolderSize);
		const FString skillSetFile = rootPaths / TEXT("Release") / TEXT("Data") / TEXT("Design") / TEXT("Skill") / TEXT("SkillSet.json");

		FString JsonText;
		if (FFileHelper::LoadFileToString(JsonText, *skillSetFile))
		{
			SkillSetJson.Reset();

			TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonText);
			TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());
			if (FJsonSerializer::Deserialize(JsonReader, jsonObj))
			{
				TSharedPtr<FJsonObject> skillSetJSON = jsonObj->GetObjectField(TEXT("skillSet"));
				const TArray<TSharedPtr<FJsonValue>> SKillSetList = skillSetJSON->GetArrayField(TEXT("SKillSetList"));
				SkillSetJson.AddDefaulted(SKillSetList.Num());
				for (int32 i = 0; i < SKillSetList.Num(); ++i)
				{
					TSharedPtr<FJsonObject> SKillSet = SKillSetList[i]->AsObject();
					if (SKillSet)
					{
						SkillSetJson[i].id = SKillSet->GetIntegerField(TEXT("id"));
						const TArray<TSharedPtr<FJsonValue>> notifyInfoList = SKillSet->GetArrayField(TEXT("notifyInfoList"));
						SkillSetJson[i].notifyInfoList.AddDefaulted(notifyInfoList.Num());
						for (int32 j = 0; j < notifyInfoList.Num(); ++j)
						{
							SkillSetJson[i].notifyInfoList[j] = notifyInfoList[j]->AsObject()->GetIntegerField(TEXT("skillNotifyId"));
						}
					}
				}				
			}
		}
	}

	if (AnimMontage)
	{
		// Notifies Copy
		TArray< FAnimNotifyEvent* > SkillNotifyEvent;
		for (int32 index = 0; index < AnimMontage->GetNotifies().Num(); ++index)
		{
			if (AnimMontage->GetNotifies()[index].SkillNotify != nullptr)
				SkillNotifyEvent.Add(&(AnimMontage->GetNotifies()[index]));
		}

		SkillNotifies.Empty();
		for (int32 index = 0; index < SkillNotifyEvent.Num(); ++index)
		{
			FAnimNotifyEvent CopyData = *SkillNotifyEvent[index];

			USkillNotify* DuplicateSkillNotify = DuplicateObject<USkillNotify>(CopyData.SkillNotify, this);
			CopyData.SkillNotify = DuplicateSkillNotify;
			SkillNotifies.Add(CopyData);
		}

		// 섹션이 여러개일수 있으니 일단 SkillSectionData의 id와 notify 를 모은다.
		TArray<int32> tempNotifyId;
		for (int32 SectionIndex = 0; SectionIndex < AnimMontage->GetCompositeSections().Num(); ++SectionIndex)
		{
			USkillMetaData*& SkillMetaData = AnimMontage->GetAnimCompositeSection(SectionIndex).GetSkillSetMetaData();
			USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);

			if (SkillSectionData)
			{
				for (int32 i = 0; i < SkillSetJson.Num(); ++i)
				{
					if (SkillSetJson[i].id == SkillSectionData->SkillSetData.id)
					{
						for (int32 j = 0; j < SkillSetJson[i].notifyInfoList.Num(); ++j)
						{
							tempNotifyId.Add(SkillSetJson[i].notifyInfoList[j]);
						}
					}
				}
			}
		}

		for (int32 index = 0, indexTemp=0; index < SkillNotifies.Num() && indexTemp < tempNotifyId.Num(); ++index)
		{
			//if (index < tempNotifyId.Num())
			{
				UHitSkillNotify* HitSkillNotify = Cast< UHitSkillNotify>(SkillNotifies[index].SkillNotify);
				UProjectileSkillNotify* ProjectileSkillNotify = Cast< UProjectileSkillNotify >(SkillNotifies[index].SkillNotify);
				if (HitSkillNotify)
				{					
					HitSkillNotify->SkillNotifySet.RowName = FName(*FString::FromInt(tempNotifyId[indexTemp]));
					++indexTemp;
				}
				else
				if (ProjectileSkillNotify)
				{
					ProjectileSkillNotify->SkillNotifySet.RowName = FName(*FString::FromInt(tempNotifyId[indexTemp]));
					++indexTemp;
				}
			}
		}

		//int32 j = 0;
		//int32 allNotifyNum = 0;
		//for (int32 SectionIndex = 0; SectionIndex < AnimMontage->GetCompositeSections().Num(); ++SectionIndex)
		//{
		//	USkillMetaData*& SkillMetaData = AnimMontage->GetAnimCompositeSection(SectionIndex).GetSkillSetMetaData();
		//	USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);
		//	
		//	if (SkillSectionData)
		//	{
		//		for (int32 i = 0; i < SkillSetJson.Num(); ++i)
		//		{
		//			// json 에도 같은게 잇다
		//			if (SkillSetJson[i].id == SkillSectionData->SkillSetData.id)
		//			{
		//				allNotifyNum += SkillSetJson[i].notifyInfoList.Num();
		//				for (; j < SkillNotifies.Num() && j < allNotifyNum;)
		//				{
		//					//if (j < allNotifyNum)
		//					{
		//						UHitSkillNotify* HitSkillNotify = Cast< UHitSkillNotify>(SkillNotifies[j].SkillNotify);
		//						UProjectileSkillNotify* ProjectileSkillNotify = Cast< UProjectileSkillNotify >(SkillNotifies[j].SkillNotify);
		//						if (HitSkillNotify)
		//						{
		//							HitSkillNotify->SkillNotifySet.RowName = FName(*FString::FromInt(SkillSetJson[i].notifyInfoList[j]));
		//						}
		//						else
		//						if (ProjectileSkillNotify)
		//						{
		//							ProjectileSkillNotify->SkillNotifySet.RowName = FName(*FString::FromInt(SkillSetJson[i].notifyInfoList[j]));									
		//						}

		//						++j;
		//					}
		//				}

		//				break;
		//			}
		//		}
		//	}
		//}

		CreateAnimMontageSkill();
		CreateAnimMontageSkillSection();
		SkillSectionDataSet();

		// Section Copy
		for (int32 SectionIndex = 0; SectionIndex < AnimMontage->GetCompositeSections().Num(); ++SectionIndex)
		{
			USkillMetaData*& SkillMetaData = AnimMontage->GetAnimCompositeSection(SectionIndex).GetSkillSetMetaData();
			ListSkillSetData[SectionIndex]->CopyData(SkillMetaData);
		}		
	}

	SkillSetJson.Reset();
}

bool USkillEditorDataAsset::IsSectionDataValid()
{
	FText assetName = FText::FromString(GetName());
	if (ListSkillSetData.Num() <= 0)
	{
		FText failedMessage = FText::Format(
			NSLOCTEXT("SkillEditor", "Failed Export", "ListSkillSetData is empty : {0}"), assetName);
		FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
		return false;
	}

	UGsTableManagerEditor& Dman = UGsTableManagerEditor::GetInstance();

	int32 MaxNum = ListSkillSetData.Num();
	if (IsDivide() == false)
		MaxNum = 1;

	for (int32 SectionIndex = 0; SectionIndex < MaxNum; ++SectionIndex)
	{
		if (this->ActionCategory == ActionCategory::SKILL)
		{
			USkillSectionData* SkillSectionData = Cast<USkillSectionData>(ListSkillSetData[SectionIndex]);
			if (SkillSectionData == nullptr)
			{
				FText failedMessage = FText::Format(
					NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::SKILL, USkillSectionData is nullptr. : {0}"), assetName);
				FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
				return false;
			}

			if (SkillSectionData->SkillSetData.id == 0)
			{
				FText failedMessage = FText::Format(
					NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::SKILL, SkillSetData.id == 0 : {0}"), assetName);
				FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
				return false;
			}

			// 중복 체크			
			/*const FGsSchemaSkillSet* row = nullptr;
			UGsTable* SkillSetTable = Dman.GetTable(FGsSchemaSkillSet::StaticStruct());
			if (SkillSetTable == nullptr)
			{
				FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "SkillSetTable is nullptr"));
				return false;
			}

			if (SkillSetTable->FindRow(FName(*FString::FromInt(SkillSectionData->SkillSetData.id)), row))
			{
				FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::SKILL, The SkillSetData.id is a duplicate."));
				return false;
			}*/

			/*for (int32 i = 0; i < SkillSectionData->SkillSetData.levelInfoList.Num(); ++i)
			{
				if (nullptr == SkillSectionData->SkillSetData.levelInfoList[i].skillLevelId.GetRow())
				{
					FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::SKILL, SkillSetData::levelInfoList::skillLevelId 가 nullptr이다."));
					return false;
				}
			}*/

			/*for (int32 i = 0; i < SkillSectionData->SkillSetData.notifyInfoList.Num(); ++i)
			{
				if (nullptr == SkillSectionData->SkillSetData.notifyInfoList[i].skillNotifyId.GetRow())
				{
					FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::SKILL, SkillSetData::notifyInfoList::skillNotifyId is nullptr."));
					return false;
				}
			}

			for (int32 i = 0; i < SkillSectionData->SkillSetData.clientNotifyInfoList.Num(); ++i)
			{
				if (nullptr == SkillSectionData->SkillSetData.clientNotifyInfoList[i].skillNotifyId.GetRow())
				{
					FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::SKILL, SkillSetData::clientNotifyInfoList::skillNotifyId is nullptr."));
					return false;
				}
			}*/

			/*for (int32 i = 0; i < SkillSectionData->SkillSetData.passivityInfoList.Num(); ++i)
			{
				if (nullptr == SkillSectionData->SkillSetData.passivityInfoList[i].GetRow())
				{
					FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::SKILL, SkillSetData::passivityInfoList 가 nullptr이다."));
					return false;
				}
			}*/
		}
		else
		if (this->ActionCategory == ActionCategory::REACTION)
		{
			UReactionSectionData* SkillSectionData = Cast<UReactionSectionData>(ListSkillSetData[SectionIndex]);
			if (SkillSectionData == nullptr)
			{
				FText failedMessage = FText::Format(
					NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::REACTION, UReactionSectionData is nullptr : {0}"), assetName);
				FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
				return false;
			}

			if (SkillSectionData->SkillSetData.id == 0)
			{
				FText failedMessage = FText::Format(
					NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::REACTION, SkillSetData.id == 0 : {0}"), assetName);
				FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
				return false;
			}

			// 중복 체크
			/*const FGsSchemaSkillReaction* row = nullptr;
			UGsTable* SkillReactionTable = Dman.GetTable(FGsSchemaSkillReaction::StaticStruct());
			if (SkillReactionTable == nullptr)
			{
				FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "SkillReactionTable is nullptr"));
				return false;
			}

			if (SkillReactionTable->FindRow(FName(*FString::FromInt(SkillSectionData->SkillSetData.id)), row))
			{
				FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::SKILL, The SkillSetData.id is a duplicate."));
				return false;
			}*/

			/*for (int32 i = 0; i < SkillSectionData->SkillSetData.notifyInfoList.Num(); ++i)
			{
				if (nullptr == SkillSectionData->SkillSetData.notifyInfoList[i].skillNotifyId.GetRow())
				{
					FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::SKILL, SkillSetData::notifyInfoList::skillNotifyId is nullptr."));
					return false;
				}
			}*/
		}
		else
		if (this->ActionCategory == ActionCategory::COMMON)
		{
			UCommonSectionData* SkillSectionData = Cast<UCommonSectionData>(ListSkillSetData[SectionIndex]);
			if (SkillSectionData == nullptr)
			{
				FText failedMessage = FText::Format(
					NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::COMMON, UCommonSectionData is nullptr. : {0}"), assetName);
				FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
				return false;
			}

			if (SkillSectionData->SkillSetData.id == 0)
			{
				FText failedMessage = FText::Format(
					NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::COMMON, SkillSetData.id == 0 : {0}"), assetName);
				FMessageDialog::Open(EAppMsgType::Ok, failedMessage);
				return false;
			}

			// 중복 체크
			/*const FGsSchemaSkillCommon* row = nullptr;
			UGsTable* SkillCommonTable = Dman.GetTable(FGsSchemaSkillCommon::StaticStruct());
			if (SkillCommonTable == nullptr)
			{
				FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "SkillCommonTable is nullptr"));
				return false;
			}

			if (SkillCommonTable->FindRow(FName(*FString::FromInt(SkillSectionData->SkillSetData.id)), row))
			{
				FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::SKILL, The SkillSetData.id is a duplicate."));
				return false;
			}*/

			/*for (int32 i = 0; i < SkillSectionData->SkillSetData.notifyInfoList.Num(); ++i)
			{
				if (nullptr == SkillSectionData->SkillSetData.notifyInfoList[i].skillNotifyId.GetRow())
				{
					FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed Export", "ActionCategory::SKILL, SkillSetData::notifyInfoList::skillNotifyId is nullptr."));
					return false;
				}
			}*/
		}
	}

	return true;
}

void USkillEditorDataAsset::ChangeSectionCategory(int32 InChangedSkillCategory)
{
	// 단일로 하기로 했다면 가테고리도 하나로 통일시킨다.
	if (IsDivide() == false)
	{
		for (int32 SectionIndex = 0; SectionIndex < ListSkillSetData.Num(); ++SectionIndex)
		{
			if (this->ActionCategory == ActionCategory::SKILL)
			{
				USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
				USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);

				SkillSectionData->SkillSetData.category = static_cast<enum::SkillCategory>(InChangedSkillCategory);
			}
			else
			if (this->ActionCategory == ActionCategory::REACTION)
			{
				USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
				UReactionSectionData* ReactionSectionData = Cast< UReactionSectionData >(SkillMetaData);

				ReactionSectionData->SkillSetData.category = static_cast<enum::EReactionType>(InChangedSkillCategory);
			}
			else
			if (this->ActionCategory == ActionCategory::COMMON)
			{
				USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
				UCommonSectionData* CommonSectionData = Cast< UCommonSectionData >(SkillMetaData);

				CommonSectionData->SkillSetData.category = static_cast<enum::CommonActionType>(InChangedSkillCategory);
			}
		}
	}

	OnChangeSectionCategoryEdited.ExecuteIfBound();
}

bool USkillEditorDataAsset::IsDivide()
{
	// 어떤섹션의 가테고리를 바꿔서 분할을 On/Off시킨지 모르니깐 bool 누적으로 전체 체크해야된다
	// true가 단하나라도 있으면 분할중지! 단일 섹션이라는 뜻이다.

	bool isDivide = true;
	for (int32 SectionIndex = 0; SectionIndex < ListSkillSetData.Num(); ++SectionIndex)
	{
		if (this->ActionCategory == ActionCategory::SKILL)
		{
			USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
			USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);

			if (SkillSectionData)
			{
				switch (SkillSectionData->SkillSetData.category)
				{
				case SkillCategory::CAST:
					isDivide = false;
					break;
				}
			}
		}
		else
		if (this->ActionCategory == ActionCategory::REACTION ||
			this->ActionCategory == ActionCategory::COMMON)
		{
			isDivide = false;
		}
	}

	return isDivide;
}

bool USkillEditorDataAsset::Export()
{
	// 검증 SkillSet id 가 0 이거나 SkillNotify id 가 0일때 경고 팝업을 띄운다 그리고 Export 실패한다.
	if (false == IsSectionDataValid())
		return false;

	const FString& AssetName = FPackageName::GetShortName(*this->GetOutermost()->GetPathName());
	UGsTableManagerEditor& Dman = UGsTableManagerEditor::GetInstance();
	UGsTable* SkillSetTable = Dman.GetTable(FGsSchemaSkillSet::StaticStruct());

	if(IsDivide())
	{
		for (int32 SectionIndex = 0; SectionIndex < ListSkillSetData.Num(); ++SectionIndex)
		{
			if (this->ActionCategory == ActionCategory::SKILL && SkillSetTable != nullptr)
			{				
				USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
				USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);

				SkillSectionData->SkillSetData.RowName = FName(*FString::FromInt(SkillSectionData->SkillSetData.id));
				SkillSectionData->SkillSetData.nameText = AssetName;
				AddSkillsetDataTable(SkillSetTable, SkillSectionData);
			}
			else
			if (this->ActionCategory == ActionCategory::REACTION)
			{
				USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
				UReactionSectionData* SkillSectionData = Cast< UReactionSectionData >(SkillMetaData);

				UGsTable* Table = Dman.GetTable(FGsSchemaSkillReaction::StaticStruct());
				//Table->Empty();

				SkillSectionData->SkillSetData.RowName = FName(*FString::FromInt(SkillSectionData->SkillSetData.id));
				SkillSectionData->SkillSetData.nameText = AssetName;
				Table->AddRow(SkillSectionData->SkillSetData);
			}
			else
			if (this->ActionCategory == ActionCategory::COMMON)
			{
				USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
				UCommonSectionData* SkillSectionData = Cast< UCommonSectionData >(SkillMetaData);

				UGsTable* Table = Dman.GetTable(FGsSchemaSkillCommon::StaticStruct());
				//Table->Empty();

				SkillSectionData->SkillSetData.RowName = FName(*FString::FromInt(SkillSectionData->SkillSetData.id));
				SkillSectionData->SkillSetData.nameText = AssetName;
				Table->AddRow(SkillSectionData->SkillSetData);
			}
		}
	}
	else
	{
		int32 SectionIndex = 0;
		if (this->ActionCategory == ActionCategory::SKILL && SkillSetTable != nullptr)
		{
			USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
			USkillSectionData* SkillSectionData = Cast< USkillSectionData >(SkillMetaData);

			SkillSectionData->SkillSetData.RowName = FName(*FString::FromInt(SkillSectionData->SkillSetData.id));
			SkillSectionData->SkillSetData.nameText = AssetName;
			AddSkillsetDataTable(SkillSetTable, SkillSectionData);
		}
		else
		if (this->ActionCategory == ActionCategory::REACTION)
		{
			USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
			UReactionSectionData* SkillSectionData = Cast< UReactionSectionData >(SkillMetaData);

			UGsTable* Table = Dman.GetTable(FGsSchemaSkillReaction::StaticStruct());
			//Table->Empty();

			SkillSectionData->SkillSetData.RowName = FName(*FString::FromInt(SkillSectionData->SkillSetData.id));
			SkillSectionData->SkillSetData.nameText = AssetName;
			Table->AddRow(SkillSectionData->SkillSetData);
		}
		else
		if (this->ActionCategory == ActionCategory::COMMON)
		{
			USkillMetaData*& SkillMetaData = ListSkillSetData[SectionIndex];
			UCommonSectionData* SkillSectionData = Cast< UCommonSectionData >(SkillMetaData);

			UGsTable* Table = Dman.GetTable(FGsSchemaSkillCommon::StaticStruct());
			//Table->Empty();

			SkillSectionData->SkillSetData.RowName = FName(*FString::FromInt(SkillSectionData->SkillSetData.id));
			SkillSectionData->SkillSetData.nameText = AssetName;
			Table->AddRow(SkillSectionData->SkillSetData);
		}
	}

	return true;
}

void USkillEditorDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{	
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName = (PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SpawnBPData Event

		if (PropertyName == TEXT("SpawnBPData") || PropertyName == TEXT("GsCharacterBase"))
		{
			if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove)
			{
				OnSpawnBPDataGsCharacterBaseRemoveUpdate.ExecuteIfBound();
			}
			else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear)
			{
				OnSpawnBPDataGsCharacterBaseRemoveAllUpdate.ExecuteIfBound();
			}
			else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet)
			{
				OnSpawnBPDataGsCharacterBaseSettingUpdate.ExecuteIfBound();
			}
			else if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
			{
			}
		}

		if (PropertyName == TEXT("TM"))
		{
			OnSpawnTransformUpdate.ExecuteIfBound();
		}

		if (PropertyName == TEXT("SpawnBPDataRendering"))
		{
			OnSpawnBPDataRendering.ExecuteIfBound();
		}

		if (PropertyName == TEXT("ActorScale"))
		{
			OnActorScaleUpdate.ExecuteIfBound();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// 기능이 중복되지만 ActionCategory와 SkillCategory를 분리한다. 중복되는것도 안되는것도 있다...이 무슨...
		if (PropertyName == TEXT("AutoTotalRuntimeSetting"))
		{
			SkillSectionDataSet();
		}
		/*else if (PropertyName == TEXT("UseSkillNotifyCollision"))
		{
			RefreshDrawData();
		}*/
		else if (PropertyName == TEXT("ActionCategory"))
		{
			const EAppReturnType::Type Choice = FMessageDialog::Open(EAppMsgType::YesNo, NSLOCTEXT("SkillEditor", "ActionCategory", "If you change ActionCategory, SectionData data is changed, so SkillData is reset or initialized. Do you want to proceed ?"));
			if (Choice == EAppReturnType::Yes)
			{
				BeforeActionCategory = ActionCategory;
				BeforeSkillCategory = SkillCategory;
				BeforeCommonCategory = CommonCategory;
				BeforeReactionCategory = ReactionCategory;

				if (ActionCategory::SKILL == ActionCategory)
				{
					SkillCategoryShow();
					CommonCategoryHide();
					ReactionCategoryHide();

					SkillCategorySet SkillCategorySetResult = SharedFunc::GetSkillCategorySet(SkillCategory);
					if (SkillCategorySetResult == SkillCategorySet::BUFF || SkillCategorySetResult == SkillCategorySet::PASSIVE)
					{
						AnimMontageHide();
						AnimSequenceShow();

						AnimMontage = nullptr;
						AnimMontageSkill = nullptr;
						SkillNotifies.Empty();
						
						CreateAnimSequenceSkill();
						CreateAnimSequenceSkillSection();
						SkillSectionDataSet();
					}
					else
					{
						AnimMontageShow();
						AnimSequenceHide();

						AnimSequence = nullptr;
						AnimSequenceSkill = nullptr;
						SkillNotifies.Empty();

						CreateAnimMontageSkill();
						CreateAnimMontageSkillSection();
						SkillSectionDataSet();
					}
				}
				else
				if (ActionCategory::REACTION == ActionCategory)
				{
					ReactionCategoryShow();
					SkillCategoryHide();
					CommonCategoryHide();

					AnimMontageShow();
					AnimSequenceHide();

					AnimSequence = nullptr;
					AnimSequenceSkill = nullptr;
					SkillNotifies.Empty();

					CreateAnimMontageSkill();
					CreateAnimMontageSkillSection();
					SkillSectionDataSet();
				}
				else
				if (ActionCategory::COMMON == ActionCategory)
				{
					CommonCategoryShow();
					SkillCategoryHide();
					ReactionCategoryHide();

					AnimMontageShow();
					AnimSequenceHide();

					AnimSequence = nullptr;
					AnimSequenceSkill = nullptr;
					SkillNotifies.Empty();

					CreateAnimMontageSkill();
					CreateAnimMontageSkillSection();
					SkillSectionDataSet();
				}

				OnChangeEdited.ExecuteIfBound();
			}
			else
			{
				ActionCategory = BeforeActionCategory;
				SkillCategory = BeforeSkillCategory;
				CommonCategory = BeforeCommonCategory;
				ReactionCategory = BeforeReactionCategory;
			}
		}
		else if (PropertyName == TEXT("SkillCategory"))
		{
			// 이게 온다는건 일단 ActionCategory가 Skill이라는 것
			const EAppReturnType::Type Choice = FMessageDialog::Open(EAppMsgType::YesNo, NSLOCTEXT("SkillEditor", "SkillCategory", "If you change SkillCategory, SectionData data is changed, so SkillData is reset or initialized. Do you want to proceed ?"));
			if (Choice == EAppReturnType::Yes)
			{
				BeforeActionCategory = ActionCategory;
				BeforeSkillCategory = SkillCategory;

				if (ActionCategory::SKILL == ActionCategory)
				{
					SkillCategoryShow();
					CommonCategoryHide();
					ReactionCategoryHide();

					SkillCategorySet SkillCategorySetResult = SharedFunc::GetSkillCategorySet(SkillCategory);
					if (SkillCategorySetResult == SkillCategorySet::BUFF || SkillCategorySetResult == SkillCategorySet::PASSIVE)
					{
						AnimMontageHide();
						AnimSequenceShow();

						AnimMontage = nullptr;
						AnimMontageSkill = nullptr;
						SkillNotifies.Empty();

						CreateAnimSequenceSkill();
						CreateAnimSequenceSkillSection();
						SkillSectionDataSet();
					}
					else
					{
						AnimMontageShow();
						AnimSequenceHide();

						AnimSequence = nullptr;
						AnimSequenceSkill = nullptr;
						SkillNotifies.Empty();

						CreateAnimMontageSkill();
						CreateAnimMontageSkillSection();
						SkillSectionDataSet();
					}
				}

				OnChangeEdited.ExecuteIfBound();
			}
			else
			{
				ActionCategory = BeforeActionCategory;
				SkillCategory = BeforeSkillCategory;
			}
		}
		else if (PropertyName == TEXT("CommonCategory"))
		{
			// 이게 온다는건 일단 ActionCategory가 Skill이라는 것
			const EAppReturnType::Type Choice = FMessageDialog::Open(EAppMsgType::YesNo, NSLOCTEXT("SkillEditor", "CommonCategory", "If you change CommonCategory, SectionData data is changed, so SkillData is reset or initialized. Do you want to proceed ?"));
			if (Choice == EAppReturnType::Yes)
			{
				BeforeActionCategory = ActionCategory;
				BeforeCommonCategory = CommonCategory;

				if (ActionCategory::COMMON == ActionCategory)
				{
					CommonCategoryShow();
					SkillCategoryHide();					
					ReactionCategoryHide();

					AnimMontageShow();
					AnimSequenceHide();

					AnimSequence = nullptr;
					AnimSequenceSkill = nullptr;
					SkillNotifies.Empty();

					CreateAnimMontageSkill();
					CreateAnimMontageSkillSection();
					SkillSectionDataSet();
				}

				OnChangeEdited.ExecuteIfBound();
			}
			else
			{
				ActionCategory = BeforeActionCategory;
				CommonCategory = BeforeCommonCategory;
			}
		}
		else if (PropertyName == TEXT("ReactionCategory"))
		{
			// 이게 온다는건 일단 ActionCategory가 Skill이라는 것
			const EAppReturnType::Type Choice = FMessageDialog::Open(EAppMsgType::YesNo, NSLOCTEXT("SkillEditor", "ReactionCategory", "If you change ReactionCategory, SectionData data is changed, so SkillData is reset or initialized. Do you want to proceed ?"));
			if (Choice == EAppReturnType::Yes)
			{
				BeforeActionCategory = ActionCategory;
				BeforeReactionCategory = ReactionCategory;

				if (ActionCategory::REACTION == ActionCategory)
				{
					ReactionCategoryShow();
					CommonCategoryHide();					
					SkillCategoryHide();

					AnimMontageShow();
					AnimSequenceHide();

					AnimSequence = nullptr;
					AnimSequenceSkill = nullptr;
					SkillNotifies.Empty();

					CreateAnimMontageSkill();
					CreateAnimMontageSkillSection();
					SkillSectionDataSet();
				}

				OnChangeEdited.ExecuteIfBound();
			}
			else
			{
				ActionCategory = BeforeActionCategory;
				ReactionCategory = BeforeReactionCategory;
			}
		}
		else if (PropertyName == TEXT("AnimMontage"))
		{
			// 기존에 nullptr이었으면 출력할 필요가 없다.
			if (BeforeAnimMontage == nullptr)
			{
				SkillNotifies.Empty();

				CreateAnimMontageSkill();
				CreateAnimMontageSkillSection();
				SkillSectionDataSet();

				OnChangeEdited.ExecuteIfBound();
			}
			else
			{
				const EAppReturnType::Type Choice = FMessageDialog::Open(EAppMsgType::YesNo, NSLOCTEXT("SkillEditor", "AnimMontage", "If you change AnimMontage, SectionData data is changed, so SkillData is reset or initialized. Do you want to proceed ?"));
				if (Choice == EAppReturnType::Yes)
				{
					SkillNotifies.Empty();

					CreateAnimMontageSkill();
					CreateAnimMontageSkillSection();
					SkillSectionDataSet();

					OnChangeEdited.ExecuteIfBound();
				}
				else
				{
					AnimMontage = BeforeAnimMontage;
				}
			}
		}
		else if (PropertyName == TEXT("AnimSequence"))
		{
			if (BeforeAnimSequence == nullptr)
			{
				SkillNotifies.Empty();

				CreateAnimSequenceSkill();
				CreateAnimSequenceSkillSection();
				SkillSectionDataSet();

				OnChangeEdited.ExecuteIfBound();
			}
			else
			{
				const EAppReturnType::Type Choice = FMessageDialog::Open(EAppMsgType::YesNo, NSLOCTEXT("SkillEditor", "AnimSequence", "If you change AnimSequence, SectionData data is changed, so SkillData is reset or initialized. Do you want to proceed ?"));
				if (Choice == EAppReturnType::Yes)
				{
					SkillNotifies.Empty();

					CreateAnimSequenceSkill();
					CreateAnimSequenceSkillSection();
					SkillSectionDataSet();

					OnChangeEdited.ExecuteIfBound();
				}
				else
				{
					AnimSequence = BeforeAnimSequence;
				}
			}
		}
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#if WITH_EDITORONLY_DATA
void USkillEditorDataAsset::Serialize(FStructuredArchive::FRecord Record)
{	
	Super::Serialize(Record);
}
#endif

void USkillEditorDataAsset::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}

void USkillEditorDataAsset::PostLoad()
{
	Super::PostLoad();
}

void USkillEditorDataAsset::PostInterpChange(FProperty* PropertyThatChanged)
{
	Super::PostInterpChange(PropertyThatChanged);
}

void USkillEditorDataAsset::PreEditChange(FProperty* PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);
}

void USkillEditorDataAsset::PreEditChange(class FEditPropertyChain& PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);
}