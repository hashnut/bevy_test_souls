// Copyright Epic Games, Inc. All Rights Reserved.

#include "GsAnimGraphNode_LookCamera.h"
#include "Engine/Classes/Animation/AnimInstance.h"
#include "Core/Public/UObject/AnimPhysObjectVersion.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "CoreUObject/Public/UObject/UObjectGlobals.h"

/////////////////////////////////////////////////////
// UAnimGraphNode_LookAt

#define LOCTEXT_NAMESPACE "AnimGraph_LookCamera"

UGsAnimGraphNode_LookCamera::UGsAnimGraphNode_LookCamera(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UGsAnimGraphNode_LookCamera::GetControllerDescription() const
{
	return LOCTEXT("LookCameraNode", "Look Camera");
}

FText UGsAnimGraphNode_LookCamera::GetTooltipText() const
{
	return LOCTEXT("AnimGraphNode_LookCamera_Tooltip", "This node allow a bone to trace or follow camera");
}

FText UGsAnimGraphNode_LookCamera::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if ((TitleType == ENodeTitleType::ListView || TitleType == ENodeTitleType::MenuTitle) && (Node.BoneToModify.BoneName == NAME_None))
	{
		return GetControllerDescription();
	}
	// @TODO: the bone can be altered in the property editor, so we have to 
	//        choose to mark this dirty when that happens for this to properly work
	else //if (!CachedNodeTitles.IsTitleCached(TitleType, this))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("ControllerDescription"), GetControllerDescription());
		Args.Add(TEXT("BoneName"), FText::FromName(Node.BoneToModify.BoneName));

		// FText::Format() is slow, so we cache this to save on performance
		if (TitleType == ENodeTitleType::ListView || TitleType == ENodeTitleType::MenuTitle)
		{
			CachedNodeTitles.SetCachedTitle(TitleType, FText::Format(LOCTEXT("AnimGraphNode_LookCamera_ListTitle", "{ControllerDescription} - Bone: {BoneName}"), Args), this);
		}
		else
		{
			CachedNodeTitles.SetCachedTitle(TitleType, FText::Format(LOCTEXT("AnimGraphNode_LookCamera_Title", "{ControllerDescription}\nBone: {BoneName}"), Args), this);
		}
	}
	return CachedNodeTitles[TitleType];
}

void UGsAnimGraphNode_LookCamera::Draw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* SkelMeshComp) const
{
	if (SkelMeshComp)
	{
		if (FGsAnimNode_LookCamera* ActiveNode = GetActiveInstanceNode<FGsAnimNode_LookCamera>(SkelMeshComp->GetAnimInstance()))
		{
			ActiveNode->ConditionalDebugDraw(PDI, SkelMeshComp);
		}
	}
}


void UGsAnimGraphNode_LookCamera::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FAnimPhysObjectVersion::GUID);

	auto GetAlignVector = [](EAxisOption::Type AxisOption, const FVector& CustomAxis) -> FVector
	{
		switch (AxisOption)
		{
		case EAxisOption::X:
			return FTransform::Identity.GetUnitAxis(EAxis::X);
		case EAxisOption::X_Neg:
			return -FTransform::Identity.GetUnitAxis(EAxis::X);
		case EAxisOption::Y:
			return FTransform::Identity.GetUnitAxis(EAxis::Y);
		case EAxisOption::Y_Neg:
			return -FTransform::Identity.GetUnitAxis(EAxis::Y);
		case EAxisOption::Z:
			return FTransform::Identity.GetUnitAxis(EAxis::Z);
		case EAxisOption::Z_Neg:
			return -FTransform::Identity.GetUnitAxis(EAxis::Z);
		case EAxisOption::Custom:
			return CustomAxis;
		}

		return FVector(1.f, 0.f, 0.f);
	};
	
}

void UGsAnimGraphNode_LookCamera::GetOnScreenDebugInfo(TArray<FText>& DebugInfo, FAnimNode_Base* RuntimeAnimNode, USkeletalMeshComponent* PreviewSkelMeshComp) const
{
	if (RuntimeAnimNode)
	{
		const FGsAnimNode_LookCamera* LookatRuntimeNode = static_cast<FGsAnimNode_LookCamera*>(RuntimeAnimNode);
		DebugInfo.Add(FText::Format(LOCTEXT("DebugOnScreenBoneName", "Anim Look At (Source:{0})"), FText::FromName(LookatRuntimeNode->BoneToModify.BoneName)));

		

		//DebugInfo.Add(FText::Format(LOCTEXT("DebugOnScreenTargetLocation", "	TargetLocation: {0}"), FText::FromString(LookatRuntimeNode->GetCachedTargetLocation().ToString())));
	}
}

#undef LOCTEXT_NAMESPACE
