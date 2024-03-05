#include "GsAnimation.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Classes/Animation/AnimInstance.h"
#include "Classes/Animation/AnimMontage.h"

#include "Engine/Public/Animation/AnimNotifyQueue.h"
#include "Engine/Classes/Animation/AnimSingleNodeInstance.h"
#include "Engine/Classes/Animation/AnimSequence.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/GameFramework/Character.h"

#include "Engine/Engine.h"

#include "T1Project.h"

// 0.0 ~ 1.0 값으로 위치 세팅
void FGsAnimationUtil::ProgressAnimMontage(UAnimInstance* In_animInstance,
	UAnimMontage* In_animMontage, FName In_sectionName, float In_progressVal,
	float In_preProgressVal,
	bool In_bFireNotifies)
{
	if (In_animInstance == nullptr ||
		In_animMontage == nullptr)
	{
		return;
	}

	// 구간 위치
	const int32 nowSectionID = In_animMontage->GetSectionIndex(In_sectionName);
	if (In_animMontage->IsValidSectionIndex(nowSectionID) == false)
	{
		return;
	}

	FCompositeSection & CurSection = In_animMontage->GetAnimCompositeSection(nowSectionID);

	// 섹션 시작점
	float startPosition = CurSection.GetTime();

	//// 끝에 빼는건 완전 끝이면 다음 시작이라서 그런가
	float sectionSize = In_animMontage->GetSectionLength(nowSectionID) - KINDA_SMALL_NUMBER;
	float nowPosition = startPosition + sectionSize * In_progressVal;

	float oldPosition = startPosition + sectionSize * In_preProgressVal;

	In_animInstance->Montage_SetPosition(In_animMontage, nowPosition);

	// 정방향 진행만 처리
	if (In_bFireNotifies && (nowPosition > oldPosition))
	{
		UAnimSingleNodeInstance* singleNodeInstance = Cast<UAnimSingleNodeInstance>(In_animInstance);
		if (singleNodeInstance == nullptr)
		{
			return;
		}
		singleNodeInstance->SetPositionWithPreviousTime(nowPosition, oldPosition, true);
	}
}

FTransform FGsAnimationUtil::GetBoneTransformFromMontage(
	const ACharacter* In_char,
	const USkeletalMeshComponent* MeshComponent, const UAnimMontage* anim_montage, FName target_bone_name, float Time)
{
	FTransform Transform = FTransform::Identity;
	if (In_char == nullptr ||
		anim_montage == nullptr)
	{
		return Transform;
	}

	UAnimSequence* AnimSequence = Cast<UAnimSequence>(anim_montage->SlotAnimTracks[0].AnimTrack.AnimSegments[0].AnimReference);
	Transform = GetBoneTransformFromSequence(In_char, MeshComponent, AnimSequence, target_bone_name, Time);

	return Transform;
}

FTransform FGsAnimationUtil::GetBoneTransformFromSequence(
	const ACharacter* In_char,
	const USkeletalMeshComponent* MeshComponent, const UAnimSequence* anim_sequence, FName target_bone_name, float Time)
{
	if (In_char == nullptr ||
		!anim_sequence || !MeshComponent || !MeshComponent->SkeletalMesh)
		return FTransform::Identity;

	FName bone_name = target_bone_name;
	FTransform global_transform = FTransform::Identity;

	do
	{
		const int bone_index = MeshComponent->GetBoneIndex(bone_name);
#if BSAM_TEMP
		int32 BoneTrackIndex = MeshComponent->SkeletalMesh->GetSkeleton()->GetRawAnimationTrackIndex(bone_index, anim_sequence);
#else
		int32 BoneTrackIndex = MeshComponent->SkeletalMesh->Skeleton->GetRawAnimationTrackIndex(bone_index, anim_sequence);
#endif

		FTransform bone_transform;
		anim_sequence->GetBoneTransform(bone_transform, BoneTrackIndex, Time, true);

		global_transform *= bone_transform;

		if (FGsGameDataManager* dataManager = GGameData())
		{
			if (true == dataManager->GetIsShowTestCamBoneLog())
			{
#ifdef TEST_LOG
				FVector globalLocation = global_transform.GetLocation();
				GSLOG(Error, TEXT("[ShowTestCamBoneLog] bone_name: %s,  globalLocation.Z: %f"), *bone_name.ToString(), globalLocation.Z);
#endif
			}
		}

		bone_name = MeshComponent->GetParentBone(bone_name);

	} while (bone_name != NAME_None);

	global_transform *= MeshComponent->GetRelativeTransform();
	global_transform *= In_char->GetTransform();

	return global_transform;
}