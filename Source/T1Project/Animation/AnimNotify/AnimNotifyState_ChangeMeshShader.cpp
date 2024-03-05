// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_ChangeMeshShader.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Animation/AnimSequenceBase.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"

UAnimNotifyState_ChangeMeshShader::UAnimNotifyState_ChangeMeshShader(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

FString UAnimNotifyState_ChangeMeshShader::GetNotifyName_Implementation() const
{	
	return Super::GetNotifyName_Implementation();
}

void UAnimNotifyState_ChangeMeshShader::NotifyBegin(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_TotalDuration)
{
	CurrentTime = 0;

	Received_NotifyBegin(in_MeshComp, in_Animation, in_TotalDuration);
}

void UAnimNotifyState_ChangeMeshShader::NotifyTick(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_FrameDeltaTime)
{
	if (DissolveCurve)
	{
		if (in_MeshComp)
		{
			CurrentTime += in_FrameDeltaTime;
			float value = DissolveCurve->GetFloatValue(CurrentTime);
			in_MeshComp->SetScalarParameterValueOnMaterials(ParameterName, value);
		}
	}

	Received_NotifyTick(in_MeshComp, in_Animation, in_FrameDeltaTime);
}

void UAnimNotifyState_ChangeMeshShader::NotifyEnd(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation)
{
	Received_NotifyEnd(in_MeshComp, in_Animation);
}