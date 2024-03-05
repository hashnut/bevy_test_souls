// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimNotify_PlayMeshDissolveShader.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Animation/AnimNotifies/AnimNotify.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponentEx/EGsMeshShaderEffect.h"
#include "T1Project/ActorComponentEx/GsMeshShaderControlComponent.h"
#include "T1Project/ActorEx/GsCharacterBase.h"

/////////////////////////////////////////////////////
UAnimNotify_PlayMeshDissolveShader::UAnimNotify_PlayMeshDissolveShader()
	: Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 051, 153, 255);
#endif // WITH_EDITORONLY_DATA
}


FString UAnimNotify_PlayMeshDissolveShader::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_PlayMeshDissolveShader::Notify(USkeletalMeshComponent* in_meshComp, UAnimSequenceBase* Animation)
{
	if (in_meshComp)
	{		
		if (IGsMeshShaderInterface* meshShader = Cast<IGsMeshShaderInterface>(in_meshComp->GetOwner()))
		{
			if (_curveData)
			{
				if (UGsMeshShaderControlComponent* meshShaderComponent = meshShader->GetMeshShaderComponent())
				{
					meshShaderComponent->Add(EGsMeshShaderEffect::Dissolve, _curveData, false);
					meshShaderComponent->Start();
				}
			}
			else
			{
				meshShader->StartMeshShader(EGsMeshShaderEffect::Dissolve, false);
			}
		}
	}
}