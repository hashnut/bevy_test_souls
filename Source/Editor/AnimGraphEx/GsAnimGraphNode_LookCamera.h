#pragma once
#include "CoreMinimal.h"
#include "CoreUObject/Public/UObject/ObjectMacros.h"
#include "Engine/Public/EdGraph/EdGraphNodeUtils.h"
#include "Animation/AnimNode/GsAnimNode_LookCamera.h"
#include "Editor/AnimGraph/Classes/AnimGraphNode_SkeletalControlBase.h"
#include "GsAnimGraphNode_LookCamera.generated.h"


class FPrimitiveDrawInterface;
class USkeletalMeshComponent;

UCLASS(MinimalAPI, meta = (Keywords = "Look Camera, Follow Camera, Trace Camera, Track Camera"))
class UGsAnimGraphNode_LookCamera : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = Settings)
	FGsAnimNode_LookCamera Node;

public:
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	// End of UEdGraphNode interface

	// UAnimGraphNode_SkeletalControlBase interface
	virtual void Draw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* SkelMeshComp) const override;
	virtual void GetOnScreenDebugInfo(TArray<FText>& DebugInfo, FAnimNode_Base* RuntimeAnimNode, USkeletalMeshComponent* PreviewSkelMeshComp) const override;
	// End of UAnimGraphNode_SkeletalControlBase interface

protected:
	// UAnimGraphNode_SkeletalControlBase interface
	virtual FText GetControllerDescription() const override;
	virtual const FAnimNode_SkeletalControlBase* GetNode() const override { return &Node; }
	// End of UAnimGraphNode_SkeletalControlBase interface

	// UObject interface
	virtual void Serialize(FArchive& Ar) override;
	// End of UObject interface
private:
	/** Constructing FText strings can be costly, so we cache the node's title */
	FNodeTitleTextTable CachedNodeTitles;
};
