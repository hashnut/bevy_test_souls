// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkillMetaData.h"
#include "DataSchema/Skill/GsSchemaSkillReaction.h"

#include "ReactionSectionData.generated.h"


class USkillEditorDataAsset;

UCLASS()
class UReactionSectionData : public USkillMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "SkillSection")
	FGsSchemaSkillReaction  SkillSetData;

	void SetAniPath(const FString& InAniPath)
	{
		SkillSetData.aniPath = InAniPath;
	}

	float StartTime;
	float EndTime;

	USkillEditorDataAsset* SkillEditorDataAsset;

	virtual void NotifiesChanged(USkillEditorDataAsset* InSkillEditorDataAsset, int32 InSectionIndex) override;
	virtual void CopyData(USkillMetaData* InSkillMetaData) override;
	virtual void SetData(USkillEditorDataAsset* InSkillEditorDataAsset, int32 InSectionIndex) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void SetRootmotionData(UAnimMontage* InMontage);

protected:	
	void ExtractRootMotion(UAnimMontage* InMontage, float InOffset, const FTransform& InTransform,
		struct OUT FGsSchemaRootmotionSet* InRootmotionSetData);
	// return : √÷¡æ offset
	float AppendExtractRootMotion(UAnimMontage* InMontage, float InTotalLen, const FTransform& InTransform,
		struct OUT FGsSchemaRootmotionSet* InRootmotionSetData);

	void AddRootmotionSet(const FVector& InMoveDir, const FVector& InObjDir, float InOffset, struct OUT FGsSchemaRootmotionSet* InRootmotionSetData);
#endif
};
