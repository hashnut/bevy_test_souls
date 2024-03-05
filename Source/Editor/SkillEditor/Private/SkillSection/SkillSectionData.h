// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkillMetaData.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"

#include "SkillSectionData.generated.h"


class USkillEditorDataAsset;

UCLASS()
class USkillSectionData : public USkillMetaData
{
	GENERATED_BODY()

private:
	int32 drawIndex;	
	
public:
	UPROPERTY(EditAnywhere, Category = "SkillSection")
	FGsSchemaSkillSet SkillSetData;

	UPROPERTY(EditAnywhere, Category = "SkillSection")
	bool CollisionDraw;
		
	float StartTime;
	float EndTime;

	USkillEditorDataAsset* SkillEditorDataAsset;

	void SetAniPath(const FString& InAniPath)
	{
		SkillSetData.aniPath = InAniPath;
	}

	void DrawSet();

	virtual void NotifiesChanged(USkillEditorDataAsset* InSkillEditorDataAsset, int32 InSectionIndex) override;
	virtual void CopyData(USkillMetaData* InSkillMetaData) override;
	virtual void SetData(USkillEditorDataAsset* InSkillEditorDataAsset, int32 InSectionIndex) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void SetRootmotionData(UAnimMontage* InMontage);

protected:
	void ExtractRootMotion(UAnimMontage* InMontage, float InOffset, const FTransform& InTransform,
		struct OUT FGsSchemaRootmotionSet* InRootmotionSetData);
#endif
};
