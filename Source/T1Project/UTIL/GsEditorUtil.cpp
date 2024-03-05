// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEditorUtil.h"
#if WITH_EDITOR
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/LevelEditorViewport.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"

#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/User/GsSchemaUserReactionData.h"
#endif

#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "Management/ScopeGlobal/GsLevelManager.h"

#if WITH_EDITOR
FRotator UGsEditorUtil::GetFaceRotationForEditorCamera(const FVector& in_actor)
{
	FLevelEditorViewportClient* Client = (FLevelEditorViewportClient*)(GEditor->GetActiveViewport()->GetClient());

	if (Client)
	{
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(in_actor, Client->GetViewLocation());

		return rot;
	}

	return FRotator::ZeroRotator;
}

bool UGsSkillEditorUtil::IsSectionDivide(const FGsSchemaSkillCommon* commmonData)
{
	if (commmonData)
	{
		switch (commmonData->category)
		{
		case CommonActionType::TOUCHPROP:
			return true;
		default:
			break;
		}
	}
	return false;
}

bool UGsSkillEditorUtil::IsSectionDivide(const FGsSchemaSkillSet* skillData)
{
	return true;
}

bool UGsSkillEditorUtil::IsSectionDivide(const FGsSchemaSkillReaction* reactionData)
{
	return true;
}
#endif