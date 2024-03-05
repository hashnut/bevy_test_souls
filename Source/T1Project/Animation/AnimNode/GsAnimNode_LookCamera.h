#pragma once
#include "CoreMinimal.h"
#include "CoreUObject/Public/UObject/ObjectMacros.h"
#include "AnimationCore/Public/BoneIndices.h"
#include "Engine/Public/BoneContainer.h"
#include "Engine/Public/BonePose.h"
#include "AnimGraphRuntime/Public/BoneControllers/AnimNode_SkeletalControlBase.h"
#include "AnimGraphRuntime/Public/BoneControllers/AnimNode_LookAt.h"
#include "AnimationCore/Public/CommonAnimTypes.h"

#include "GameFlow/Stage/GsStageMode.h"

#include "GsAnimNode_LookCamera.generated.h"

class AGsCharacterLobbyPlayer;
class AGsCharacterPlayer;

USTRUCT()
struct FGsBoneReferenceInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = BoneReferenceInfo)
	FBoneReference BoneToModify;

	UPROPERTY(EditAnywhere, Category = BoneReferenceInfo)
	float ModifyWeight;
};

enum class EGsLookCameraCheckResult
{
	CanLookAt,			// follow camera
	HoldLookAt,			// last camera rotation keep
	GotoBaseRot,		// go to base rotation
};

/**
 *	Simple controller that make a bone to look at the point or another bone
 */
USTRUCT(BlueprintInternalUseOnly)
struct T1PROJECT_API  FGsAnimNode_LookCamera : public FAnimNode_SkeletalControlBase
{
	GENERATED_USTRUCT_BODY()

	/** Name of bone to control. This is the main bone chain to modify from. **/
	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference BoneToModify;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	TArray<FGsBoneReferenceInfo> arrayAdditionalBoneToModifys;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference LookCenterBone;
	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	float ClampDegree;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	float HoldDegree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
	TEnumAsByte<EInterpolationBlend::Type>	InterpolationType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
	float	InterpolationTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
	float	InterpolationTriggerThreashold;

private:
	AGsCharacterLobbyPlayer* _lobbyChar;

	FGsStageMode::Lobby _currentStageMode = FGsStageMode::Lobby::LOBBY_MAX;
public:
	// in the future, it would be nice to have more options, -i.e. lag, interpolation speed
	FGsAnimNode_LookCamera();

	// FAnimNode_Base interface
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	virtual void UpdateInternal(const FAnimationUpdateContext& Context) override;
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	// End of FAnimNode_Base interface

	// FAnimNode_SkeletalControlBase interface
	virtual void EvaluateComponentSpaceInternal(FComponentSpacePoseContext& Context) override;
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface

	//FVector GetCachedTargetLocation() const { return 	CachedCurrentTargetLocation; }

#if WITH_EDITOR
	void ConditionalDebugDraw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* MeshComp) const;
#endif // WITH_EDITOR

private:
	// FAnimNode_SkeletalControlBase interface
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface

	/** Turn a linear interpolated alpha into the corresponding AlphaBlendType */
	static float AlphaToBlendType(float InAlpha, EInterpolationBlend::Type BlendType);

	// logic func
private:
	EGsLookCameraCheckResult CheckLookAt(AGsCharacterLobbyPlayer* In_lobbyChar, FTransform& In_componentBoneTransform);

	void StartLookAt(TArray<FQuat> In_targetQuat, TArray<FQuat> In_currentQuat);

	void EndLookAt(TArray<FTransform>& In_arrayAdditionalComponentBoneTransforms, 
		FTransform& In_headTransform);

	void GetLookAtRot(
		TArray<FTransform>& In_arrayAdditionalComponentBoneTransforms,
		AGsCharacterPlayer* In_charPlayer,
		FVector& In_headBonePos,
		TArray<FQuat>& Out_targetQuat, 
		bool In_isHold = false);

	void UpdateLookAt(TArray<FCompactPoseBoneIndex>& In_additionalModifyBoneIndexs,
		TArray<FTransform>& In_AdditionalMComponentBoneTransforms, 
		const FCompactPoseBoneIndex In_headBoneIndex,
		FTransform& In_headBoneTr,
		TArray<FBoneTransform>& Out_BoneTransforms);

	void ReachLookAtTarget();

private:
	/** Debug transient data */
	FVector CurrentLookAtLocation;

	/** Current Alpha */
	float AccumulatedInterpoolationTime;

	// parent -> child
	TArray<FQuat> ArrayQuatFrom;
	TArray<FQuat> ArrayQuatTo;

	TArray<FQuat> CurrentQuat;

	bool IsDiffAniRot = false;
	bool IsBackToNormal = false;
	bool IsBoneRotChanged = false;
	bool IsLookAtRotChanging = false;

	float ProgressLookAtRate = 0.0f;

	float DeltaTime = 0.0f;

	float LookAtRotSpeed = 0.0f;
	float LookAtFinishDiffVal = 0.0f;

#if !UE_BUILD_SHIPPING
	/** Debug draw cached data */
	FTransform CachedOriginalTransform;
	FTransform CachedLookAtTransform;
	//FTransform CachedTargetCoordinate;
	FVector CachedPreviousTargetLocation;
	FVector CachedCurrentLookAtLocation;
#endif // UE_BUILD_SHIPPING
};
