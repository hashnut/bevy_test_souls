#include "GsCameraLookAtHandlerLobby.h"

#include "Animation/GsLobbyAnimInstanceState.h"
#include "Animation/GsAnimationDefine.h"

#include "Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Animation/AnimInstance.h"
#include "Engine/Classes/Animation/AnimSequence.h"
#include "Engine/Classes/Animation/AnimationPoseData.h"
#include "Engine/Classes/Animation/AnimationAsset.h"
#include "Engine/Public/BonePose.h"
#include "Engine/Public/BoneContainer.h"
#include "Engine/Public/Animation/AnimInstanceProxy.h"
#include "ActorEx/GsCharacterPlayer.h"

void FGsCameraLookAtHandlerLobby::SetBoneRotation(const FQuat& In_quat)
{
	if (_char == nullptr)
	{
		GSLOG(Error, TEXT("_char == nullptr"));
		return;
	}

	USkeletalMeshComponent* skeletalComponent = _char->GetMesh();
	if (skeletalComponent == nullptr)
	{
		return;
	}

	UAnimInstance* AnimInstance = skeletalComponent->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}

	UGsLobbyAnimInstanceState* lobbyAnimInstance = Cast<UGsLobbyAnimInstanceState>(AnimInstance);

	if (lobbyAnimInstance == nullptr)
	{
		return;
	}


	FCSPose<FCompactPose>	Pose;

	FAnimInstanceProxy* animInstanceProxy = lobbyAnimInstance->GetAnimInstanceProxy();
	
	const FBoneContainer& RequiredBone = animInstanceProxy->GetRequiredBones();
	Pose.InitPose(&RequiredBone);

	const FBoneContainer& BoneContainer = Pose.GetPose().GetBoneContainer();

	FBoneReference BoneToModify;
	BoneToModify.BoneName = _headBoneName;
	BoneToModify.Initialize(BoneContainer);

	
	const FCompactPoseBoneIndex ModifyBoneIndex = BoneToModify.GetCompactPoseIndex(BoneContainer);
	FTransform posComponentBoneTransform = Pose.GetComponentSpaceTransform(ModifyBoneIndex);

	posComponentBoneTransform.SetRotation(In_quat);
	TArray<FBoneTransform> OutBoneTransforms;
	// Set New Transform 
	OutBoneTransforms.Add(FBoneTransform(ModifyBoneIndex, posComponentBoneTransform));

	Pose.LocalBlendCSBoneTransforms(OutBoneTransforms, 1.0f);
}