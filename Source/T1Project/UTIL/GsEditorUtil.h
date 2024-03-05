// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#if WITH_EDITOR
#include "Editor/WorldBrowser/Public/WorldBrowserModule.h"
#include "Editor/WorldBrowser/Private/LevelCollectionModel.h"
#include "Editor/WorldBrowser/Private/LevelModel.h"
#endif

#include "GsEditorUtil.generated.h"

/**
 * 
 */
UENUM()
enum class LEVEL_WORK_TYPE
{
	Art,
	LevelDesign,
	Server,
	All
};

UCLASS()
class T1PROJECT_API UGsEditorUtil : public UObject
{
	GENERATED_BODY()
	
#if WITH_EDITOR
	//Camera
public:	
	static FRotator GetFaceRotationForEditorCamera(const FVector& in_actor);
	
#endif
};

UCLASS()
class T1PROJECT_API UGsSkillEditorUtil : public UObject
{
	GENERATED_BODY()

#if WITH_EDITOR
public:
	static bool IsSectionDivide(const struct FGsSchemaSkillCommon* commmonData);
	static bool IsSectionDivide(const struct FGsSchemaSkillSet* skillData);
	static bool IsSectionDivide(const struct FGsSchemaSkillReaction* reactionData);
#endif
};

