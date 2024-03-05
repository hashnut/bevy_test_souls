// Copyright Epic Games, Inc. All Rights Reserved.

#include "GsAnimNode_LookCamera.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowLobby.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "GameFlow/Stage/StageLobby/GsStageManagerLobby.h"
#include "GameFlow/Stage/StageLobby/GsStageLobbyBase.h"

#include "ActorEx/GsCharacterLobbyPlayer.h"

#include "Animation/GsLobbyAnimInstanceState.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Engine/Public/SceneManagement.h"
#include "Engine/Public/Animation/AnimInstanceProxy.h"
#include "Engine/Public/Animation/AnimTrace.h"
#include "AnimationCore/Public/AnimationCoreLibrary.h"
#include "Engine/Engine.h"
#include "Engine/Classes/Engine/SkeletalMeshSocket.h"
#include "Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Classes/Curves/CurveFloat.h"
#include "Core/Public/Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "GameFlow/GsGameFlow.h"

static const FVector DefaultLookAtAxis(0.f, 1.f, 0.f);
static const FVector DefaultLookUpAxis(1.f, 0.f, 0.f);

/////////////////////////////////////////////////////
// FAnimNode_LookAt

FGsAnimNode_LookCamera::FGsAnimNode_LookCamera()
	: InterpolationType(EInterpolationBlend::Linear)
	, InterpolationTime(0.f)
	, InterpolationTriggerThreashold(0.f)
	, CurrentLookAtLocation(ForceInitToZero)
	, AccumulatedInterpoolationTime(0.f)
{
}

void FGsAnimNode_LookCamera::GatherDebugData(FNodeDebugData& DebugData)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(GatherDebugData)
	FString DebugLine = DebugData.GetNodeName(this);

	DebugLine += "(";
	AddDebugNodeData(DebugLine);

	DebugLine += FString::Printf(TEXT(" Bone: %s)"), 
		*BoneToModify.BoneName.ToString());
	

	DebugData.AddDebugItem(DebugLine);

	ComponentPose.GatherDebugData(DebugData);
}

float FGsAnimNode_LookCamera::AlphaToBlendType(float InAlpha, EInterpolationBlend::Type BlendType)
{
	switch (BlendType)
	{
	case EInterpolationBlend::Sinusoidal: return FMath::Clamp<float>((FMath::Sin(InAlpha * PI - HALF_PI) + 1.f) / 2.f, 0.f, 1.f);
	case EInterpolationBlend::Cubic: return FMath::Clamp<float>(FMath::CubicInterp<float>(0.f, 0.f, 1.f, 0.f, InAlpha), 0.f, 1.f);
	case EInterpolationBlend::EaseInOutExponent2: return FMath::Clamp<float>(FMath::InterpEaseInOut<float>(0.f, 1.f, InAlpha, 2), 0.f, 1.f);
	case EInterpolationBlend::EaseInOutExponent3: return FMath::Clamp<float>(FMath::InterpEaseInOut<float>(0.f, 1.f, InAlpha, 3), 0.f, 1.f);
	case EInterpolationBlend::EaseInOutExponent4: return FMath::Clamp<float>(FMath::InterpEaseInOut<float>(0.f, 1.f, InAlpha, 4), 0.f, 1.f);
	case EInterpolationBlend::EaseInOutExponent5: return FMath::Clamp<float>(FMath::InterpEaseInOut<float>(0.f, 1.f, InAlpha, 5), 0.f, 1.f);
	}

	return InAlpha;
}

void FGsAnimNode_LookCamera::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateSkeletalControl_AnyThread)
	check(OutBoneTransforms.Num() == 0);
	

	if (_currentStageMode != FGsStageMode::Lobby::CHARACTER_CUSTOMIZING)
	{
		return;
	}
	if (_lobbyChar == nullptr)
	{
		return;
	}

	APlayerController* playerController = Cast<APlayerController>(_lobbyChar->Controller);

	if (playerController == nullptr)
	{
		return;
	}
	FVector camLoc = FVector::ZeroVector;
	FRotator rot = FRotator::ZeroRotator;

	playerController->GetPlayerViewPoint(camLoc, rot);

	// not ready camera
	if (camLoc.Equals(FVector::ZeroVector))
	{
		return;
	}

	const FBoneContainer& BoneContainer = Output.Pose.GetPose().GetBoneContainer();
	const FCompactPoseBoneIndex ModifyBoneIndex = BoneToModify.GetCompactPoseIndex(BoneContainer);
	FTransform ComponentBoneTransform = Output.Pose.GetComponentSpaceTransform(ModifyBoneIndex);

	// additional modify bones
	TArray<FCompactPoseBoneIndex> arrayAdditionalModifyBoneIndexs;
	TArray<FTransform> arrayAdditionalComponentBoneTransforms;
	for (FGsBoneReferenceInfo& iterModify : arrayAdditionalBoneToModifys)
	{
		FCompactPoseBoneIndex additionalModifyBoneIndex = iterModify.BoneToModify.GetCompactPoseIndex(BoneContainer);
		arrayAdditionalModifyBoneIndexs.Add(additionalModifyBoneIndex);
		arrayAdditionalComponentBoneTransforms.Add(Output.Pose.GetComponentSpaceTransform(additionalModifyBoneIndex));
	}

	EGsLookCameraCheckResult checkResult = CheckLookAt(_lobbyChar, ComponentBoneTransform);

	USkeletalMeshComponent* meshTargetHead = _lobbyChar->GetPartsSkeletalComponentBODY();

	if (meshTargetHead == nullptr)
	{
		return;
	}

	FTransform LocalToWorld = meshTargetHead->GetComponentTransform();

	// head calc
	FTransform ComponentWorldTransform = ComponentBoneTransform * LocalToWorld;
	FVector bonePos = ComponentWorldTransform.GetLocation();

	if (checkResult == EGsLookCameraCheckResult::GotoBaseRot)
	{
		if (IsDiffAniRot == true)
		{
			EndLookAt(arrayAdditionalComponentBoneTransforms, ComponentBoneTransform);
		}
	}
	else if (checkResult == EGsLookCameraCheckResult::HoldLookAt)
	{
		TArray<FQuat> arrayTargetQuat;
		GetLookAtRot(arrayAdditionalComponentBoneTransforms,
			_lobbyChar, bonePos, arrayTargetQuat, true);

		TArray<FQuat> arrayCurrent = CurrentQuat;
		if (arrayCurrent.Num() == 0)
		{
			for (FTransform& iterTr : arrayAdditionalComponentBoneTransforms)
			{
				arrayCurrent.Add(iterTr.GetRotation());
			}
			arrayCurrent.Add(ComponentBoneTransform.GetRotation());
		}

		// set current hold
		StartLookAt(arrayTargetQuat, arrayCurrent);

		IsBackToNormal = false;
		IsDiffAniRot = true;
	}
	else if(checkResult == EGsLookCameraCheckResult::CanLookAt)
	{


		TArray<FQuat> arrayTargetQuat;
		GetLookAtRot(arrayAdditionalComponentBoneTransforms,
			_lobbyChar, bonePos, arrayTargetQuat);

		TArray<FQuat> arrayCurrent = CurrentQuat;
		if (arrayCurrent.Num() == 0)
		{
			for (FTransform& iterTr : arrayAdditionalComponentBoneTransforms)
			{
				arrayCurrent.Add(iterTr.GetRotation());
			}
			arrayCurrent.Add(ComponentBoneTransform.GetRotation());
		}

		StartLookAt(arrayTargetQuat, arrayCurrent);

		IsBackToNormal = false;
		IsDiffAniRot = true;
	}

	UpdateLookAt(arrayAdditionalModifyBoneIndexs, arrayAdditionalComponentBoneTransforms,
		ModifyBoneIndex, ComponentBoneTransform, OutBoneTransforms);

	CurrentLookAtLocation = camLoc;

#if !UE_BUILD_SHIPPING
	CachedOriginalTransform = ComponentBoneTransform;
	//CachedTargetCoordinate = TargetTransform;

	CachedCurrentLookAtLocation = CurrentLookAtLocation;
#endif



#if !UE_BUILD_SHIPPING
	CachedLookAtTransform = ComponentBoneTransform;
#endif

	TRACE_ANIM_NODE_VALUE(Output, TEXT("Bone"), BoneToModify.BoneName);

#ifdef TEST_DRAW
	FVector lookTargetPos = bonePos + lookRotVec * 100.0f;

	DrawDebugLine(lobbyCharManager->GetWorld(), bonePos, lookTargetPos, FColor::Red, false, -1.0f);
#endif
}
EGsLookCameraCheckResult FGsAnimNode_LookCamera::CheckLookAt(AGsCharacterLobbyPlayer* In_lobbyChar, FTransform& In_componentBoneTransform)
{
	EGsLookCameraCheckResult result = EGsLookCameraCheckResult::GotoBaseRot;
	APlayerController* playerController = Cast<APlayerController>(In_lobbyChar->Controller);

	if (playerController == nullptr)
	{
		return result;
	}
	FVector camLoc = FVector::ZeroVector;
	FRotator rot = FRotator::ZeroRotator;

	playerController->GetPlayerViewPoint(camLoc, rot);

	USkeletalMeshComponent* meshTargetHead = In_lobbyChar->GetPartsSkeletalComponentBODY();

	if (meshTargetHead == nullptr)
	{
		return result;
	}

	FVector lookCenterBoneY =
		meshTargetHead->GetBoneAxis(LookCenterBone.BoneName, EAxis::Y);

	FVector backCenterBoneY = lookCenterBoneY * -1.0f;

	FTransform LocalToWorld = meshTargetHead->GetComponentTransform();

	// head calc
	FTransform ComponentWorldTransform = In_componentBoneTransform * LocalToWorld;
	FVector bonePos = ComponentWorldTransform.GetLocation();

	FVector camLookRotVec = camLoc - bonePos;

	camLookRotVec.Normalize();


	
	if (ClampDegree <= ZERO_ANIMWEIGHT_THRESH ||
		HoldDegree <= ZERO_ANIMWEIGHT_THRESH)
	{
		return result;
	}


	float clampInRadians = FMath::DegreesToRadians(FMath::Min(ClampDegree, 180.f));
	float holdInRadians = FMath::DegreesToRadians(FMath::Min(HoldDegree, 180.f));
	float DiffAngle = FMath::Acos(FVector::DotProduct(backCenterBoneY, camLookRotVec));

	// too big rotation
	if (DiffAngle > holdInRadians)
	{
		result = EGsLookCameraCheckResult::GotoBaseRot;
	}
	// middle rotation
	else if (DiffAngle <= holdInRadians && DiffAngle > clampInRadians)
	{
		result = EGsLookCameraCheckResult::HoldLookAt;
	}
	else if(DiffAngle <= clampInRadians)
	{
		result = EGsLookCameraCheckResult::CanLookAt;
	}

	return result;
}

void FGsAnimNode_LookCamera::StartLookAt(TArray<FQuat> In_targetQuat,  TArray<FQuat> In_currentQuat)
{
	if (IsLookAtRotChanging == true)
	{
		FQuat newTargetHeadQuat = In_targetQuat.Top();
		FQuat oldTargetHeadQuat = ArrayQuatTo.Top();

		if (true == newTargetHeadQuat.Equals(oldTargetHeadQuat))
		{
			return;
		}
	}

	ArrayQuatTo = In_targetQuat;
	ArrayQuatFrom = In_currentQuat;

	IsLookAtRotChanging = true;
	ProgressLookAtRate = 0.0f;
}

void FGsAnimNode_LookCamera::GetLookAtRot(
	TArray<FTransform>& In_arrayAdditionalComponentBoneTransforms, 
	AGsCharacterPlayer* In_charPlayer,
	FVector& In_headBonePos,
	TArray<FQuat>& Out_targetQuat,
	bool In_isHold)
{

	if (In_charPlayer == nullptr)
	{
		return;
	}

	USkeletalMeshComponent* meshTargetHead = In_charPlayer->GetPartsSkeletalComponentBODY();

	if (meshTargetHead == nullptr)
	{
		return;
	}

	FVector lookCenterBoneY =
		meshTargetHead->GetBoneAxis(LookCenterBone.BoneName, EAxis::Y);

	FVector backCenterBoneY = lookCenterBoneY * -1.0f;

	float clampInRadians = FMath::DegreesToRadians(FMath::Min(ClampDegree, 180.f));

	FVector upVec = In_charPlayer->GetActorUpVector();
	FTransform LocalToWorld = meshTargetHead->GetComponentTransform();
	//other bone calc
	for (int i = 0; i < In_arrayAdditionalComponentBoneTransforms.Num(); ++i)
	{
		FTransform trIter = In_arrayAdditionalComponentBoneTransforms[i];
		FTransform addComponentWorldTransform = trIter * LocalToWorld;
		FVector addBonePos = addComponentWorldTransform.GetLocation();
		FVector addLookRotVec = CurrentLookAtLocation - addBonePos;

		addLookRotVec.Normalize();
		if (In_isHold == true)
		{
			float DiffAngle = FMath::Acos(FVector::DotProduct(backCenterBoneY, addLookRotVec));

			FVector DeltaTarget = addLookRotVec - backCenterBoneY;
			// clamp delta target to within the ratio
			DeltaTarget *= (clampInRadians / DiffAngle);
			// set new target
			addLookRotVec = backCenterBoneY + DeltaTarget;
			addLookRotVec.Normalize();
		}

		FRotator addLookRot = UKismetMathLibrary::MakeRotFromXZ(addLookRotVec, upVec);

		if (arrayAdditionalBoneToModifys.Num() <= i)
		{
			return;
		}

		FGsBoneReferenceInfo info = arrayAdditionalBoneToModifys[i];

		FQuat addRelativeQuat = meshTargetHead->GetRelativeRotationFromWorld(addLookRot.Quaternion());

		FRotator addRelativeRot = addRelativeQuat.Rotator();

		FRotator addModifyRot = FRotator(90.0f - addRelativeRot.Pitch, addRelativeRot.Yaw - 90.0f, addRelativeRot.Roll + 90.0f);

		FQuat nowQuat = trIter.GetRotation();

		FQuat newQuat = FQuat::FastLerp(nowQuat, addModifyRot.Quaternion(), info.ModifyWeight);

		newQuat.Normalize();

		Out_targetQuat.Add(newQuat);
	}

	FVector targetLookRotVec = CurrentLookAtLocation - In_headBonePos;
	targetLookRotVec.Normalize();
	if (In_isHold == true)
	{
		float DiffAngle = FMath::Acos(FVector::DotProduct(backCenterBoneY, targetLookRotVec));

		FVector DeltaTarget = targetLookRotVec - backCenterBoneY;
		// clamp delta target to within the ratio
		DeltaTarget *= (clampInRadians / DiffAngle);
		// set new target
		targetLookRotVec = backCenterBoneY + DeltaTarget;
		targetLookRotVec.Normalize();
	}

	FRotator lookRot = UKismetMathLibrary::MakeRotFromXZ(targetLookRotVec, upVec);

	FQuat relativeQuat = meshTargetHead->GetRelativeRotationFromWorld(lookRot.Quaternion());

	FRotator relativeRot = relativeQuat.Rotator();

	FRotator modifyRot = FRotator(90.0f - relativeRot.Pitch, relativeRot.Yaw - 90.0f, relativeRot.Roll + 90.0f);

	FQuat modifyQuat = modifyRot.Quaternion();

	modifyQuat.Normalize();

	Out_targetQuat.Add(modifyQuat);
}

void FGsAnimNode_LookCamera::EndLookAt(TArray<FTransform>& In_arrayAdditionalComponentBoneTransforms,
	FTransform& In_headTransform)
{
	if (false == IsBoneRotChanged)
	{
		return;
	}

	IsBackToNormal = true;

	TArray<FQuat> arrayQuat;
	for (FTransform& trIter: In_arrayAdditionalComponentBoneTransforms)
	{
		arrayQuat.Add(trIter.GetRotation());
	}
	arrayQuat.Add(In_headTransform.GetRotation());

	TArray<FQuat> arrayCurrent = CurrentQuat;
	if (arrayCurrent.Num() == 0)
	{
		arrayCurrent = arrayQuat;
	}

	StartLookAt(arrayQuat, arrayCurrent);
}
void FGsAnimNode_LookCamera::UpdateLookAt(TArray<FCompactPoseBoneIndex>& In_additionalModifyBoneIndexs,
	TArray<FTransform>& In_AdditionalMComponentBoneTransforms,
	const FCompactPoseBoneIndex In_headBoneIndex,
	FTransform& In_headBoneTr,
	TArray<FBoneTransform>& Out_BoneTransforms)
{
	if (false == IsLookAtRotChanging)
	{
		return;
	}

	float addRate = DeltaTime* LookAtRotSpeed;

	ProgressLookAtRate = FMath::Clamp<float>(ProgressLookAtRate + addRate, 0.f, 1.f);

	UCurveFloat* curve = GData()->GetCustomizingLookAtCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(ProgressLookAtRate), 0.f, 1.f);
	}

	if (CurrentQuat.Num() != 0)
	{
		CurrentQuat.Empty();
	}
	for (int i=0; i < ArrayQuatFrom.Num(); ++i)
	{
		FQuat fromQuat = ArrayQuatFrom[i];
		FQuat toQuat = ArrayQuatTo[i];

		FQuat newQuat = FMath::Lerp(fromQuat, toQuat, curveVal);

		CurrentQuat.Add(newQuat);
	}

	for (int i=0; i< In_AdditionalMComponentBoneTransforms.Num(); ++i)
	{
		FTransform trIter = In_AdditionalMComponentBoneTransforms[i];

		FQuat newQuat = CurrentQuat[i];
		trIter.SetRotation(newQuat);

		Out_BoneTransforms.Add(FBoneTransform(In_additionalModifyBoneIndexs[i], trIter));
	}

	FQuat currentHeadQuat = CurrentQuat.Top();
	In_headBoneTr.SetRotation(currentHeadQuat);
	Out_BoneTransforms.Add(FBoneTransform(In_headBoneIndex, In_headBoneTr));

	IsBoneRotChanged = true;

	FQuat headQuatTo =  ArrayQuatTo.Top();
	if (headQuatTo.Equals(currentHeadQuat, LookAtFinishDiffVal) == true)
	{
		ReachLookAtTarget();
	}
}
void FGsAnimNode_LookCamera::ReachLookAtTarget()
{
	IsLookAtRotChanging = false;

	if (IsBackToNormal == true)
	{
		IsBackToNormal = false;
		IsDiffAniRot = false;
		IsBoneRotChanged = false;
	}

}
void FGsAnimNode_LookCamera::EvaluateComponentSpaceInternal(FComponentSpacePoseContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateComponentSpaceInternal)
	Super::EvaluateComponentSpaceInternal(Context);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	/*if (bEnableDebug)
	{
		const FTransform LocalToWorld = Context.AnimInstanceProxy->GetSkelMeshCompLocalToWorld();
		FVector TargetWorldLoc = LocalToWorld.TransformPosition(CachedCurrentTargetLocation);
		FVector SourceWorldLoc = LocalToWorld.TransformPosition(CachedComponentBoneLocation);

		Context.AnimInstanceProxy->AnimDrawDebugLine(SourceWorldLoc, TargetWorldLoc, FColor::Green);
	}*/

#endif // !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
}

bool FGsAnimNode_LookCamera::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	// if both bones are valid
	return (BoneToModify.IsValidToEvaluate(RequiredBones) );
}

#if WITH_EDITOR
// can't use World Draw functions because this is called from Render of viewport, AFTER ticking component, 
// which means LineBatcher already has ticked, so it won't render anymore
// to use World Draw functions, we have to call this from tick of actor
void FGsAnimNode_LookCamera::ConditionalDebugDraw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* MeshComp) const
{

	// did not apply any of LocaltoWorld
	if (PDI && MeshComp)
	{
		FTransform LocalToWorld = MeshComp->GetComponentTransform();
		FTransform ComponentTransform = CachedOriginalTransform * LocalToWorld;
		FTransform LookAtTransform = CachedLookAtTransform * LocalToWorld;
		//FTransform TargetTrasnform = CachedTargetCoordinate * LocalToWorld;
		FVector BoneLocation = LookAtTransform.GetLocation();

		// current look at location (can be clamped or interpolating)
		DrawDashedLine(PDI, BoneLocation, LocalToWorld.TransformPosition(CachedCurrentLookAtLocation), FColor::Yellow, 5.f, SDPG_World);
		DrawWireStar(PDI, CachedCurrentLookAtLocation, 5.0f, FColor::Yellow, SDPG_World);	

		// draw directional  - lookat and look up
		DrawCoordinateSystem(PDI, BoneLocation, LookAtTransform.GetRotation().Rotator(), 20.f, SDPG_Foreground);
		//DrawCoordinateSystem(PDI, TargetTrasnform.GetLocation(), TargetTrasnform.GetRotation().Rotator(), 20.f, SDPG_Foreground);
	}
}
#endif // WITH_EDITOR

void FGsAnimNode_LookCamera::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(InitializeBoneReferences)
	BoneToModify.Initialize(RequiredBones);

	for (FGsBoneReferenceInfo& iterModify : arrayAdditionalBoneToModifys)
	{
		iterModify.BoneToModify.Initialize(RequiredBones);
	}

	UGsDataContainManager* dataManager = GData();
	if (dataManager == nullptr)
	{
		return;
	}

	
	LookAtRotSpeed = dataManager->GetGlobalData()->LookAtRotSpeed;
	LookAtFinishDiffVal = dataManager->GetGlobalData()->LookAtFinishDiffVal;
}

void FGsAnimNode_LookCamera::UpdateInternal(const FAnimationUpdateContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(UpdateInternal)
	FAnimNode_SkeletalControlBase::UpdateInternal(Context);

	AccumulatedInterpoolationTime = FMath::Clamp(AccumulatedInterpoolationTime + Context.GetDeltaTime(), 0.f, InterpolationTime);
	DeltaTime = Context.GetDeltaTime();
}

void FGsAnimNode_LookCamera::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Initialize_AnyThread)
	FAnimNode_SkeletalControlBase::Initialize_AnyThread(Context);


	// initialize
#ifdef INIT_NODE_DATA_IN_NODE_CLASS
	FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow();
	if (nullptr == lobbyFlow)
	{
		return;
	}

	FGsStageManagerLobby* stageManagerLobby = lobbyFlow->GetStageManagerLobby();
	if (nullptr == stageManagerLobby)
	{
		return;
	}

	_currentStageMode = stageManagerLobby->GetCurrentStageMode();

	if (_currentStageMode != FGsStageMode::Lobby::CHARACTER_CUSTOMIZING)
	{
		return;
	}

	AGsCharacterPlayer* charPlayer = stageManagerLobby->GetCurrentState()->GetCharacter();
	if (charPlayer == nullptr)
	{
		return;
	}

	_lobbyChar = Cast<AGsCharacterLobbyPlayer>(charPlayer);
#endif
	FAnimInstanceProxy* proxy = Context.AnimInstanceProxy;
	if (nullptr == proxy)
	{
		return;
	}

	FGsLobbyAnimInstanceStateProxy* lobbyAnimInstanceProxy = static_cast<FGsLobbyAnimInstanceStateProxy*>(proxy);
	if (nullptr == lobbyAnimInstanceProxy)
	{
		return;
	}

	_currentStageMode = lobbyAnimInstanceProxy->_currentStageMode;
	if (_currentStageMode != FGsStageMode::Lobby::CHARACTER_CUSTOMIZING)
	{
		_lobbyChar = nullptr;
	}
	else
	{
		_lobbyChar = lobbyAnimInstanceProxy->_lobbyChar;
	}

}
