#pragma once
#include "CoreMinimal.h"


class UAnimInstance;
class UAnimMontage;
class USkeletalMeshComponent;
class UAnimSequence;
class ACharacter;

// 애니 관련 util 함수
class T1PROJECT_API FGsAnimationUtil
{
public:
	// 0.0 ~ 1.0 값으로 위치 세팅
	static void ProgressAnimMontage(UAnimInstance* In_animInstance, 
		UAnimMontage* In_animMontage, FName In_sectionName, float In_progressVal,
		float In_preProgressVal,
		bool In_bFireNotifies = true);

	// https://forums.unrealengine.com/t/how-to-get-a-bone-location-for-the-first-frame-of-an-animmontage/18818/12
	static FTransform GetBoneTransformFromMontage(
		const ACharacter* In_char,
		const USkeletalMeshComponent* MeshComponent, const UAnimMontage* anim_montage, FName target_bone_name, float Time);
	static FTransform GetBoneTransformFromSequence(
		const ACharacter* In_char,
		const USkeletalMeshComponent* MeshComponent, const UAnimSequence* anim_sequence, FName target_bone_name, float Time);
};