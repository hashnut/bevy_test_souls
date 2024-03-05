// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkillMetaData.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"

#include "CommonSectionData.generated.h"


class USkillEditorDataAsset;

UCLASS()
class UCommonSectionData : public USkillMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "SkillSection")
	FGsSchemaSkillCommon  SkillSetData;

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
#endif
};
