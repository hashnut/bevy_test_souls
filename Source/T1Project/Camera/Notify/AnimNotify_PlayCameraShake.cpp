// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimNotify_PlayCameraShake.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Animation/AnimNotifies/AnimNotify.h"
#include "Kismet/GameplayStatics.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Option/GsGameUserSettings.h"

/////////////////////////////////////////////////////
// UAnimNotify_PlayCameraShake

UAnimNotify_PlayCameraShake::UAnimNotify_PlayCameraShake()
	: Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 051, 153, 255);
#endif // WITH_EDITORONLY_DATA
}


FString UAnimNotify_PlayCameraShake::GetNotifyName_Implementation() const
{
	if (CameraShake)
	{
		return CameraShake->GetName();
	}
	else
	{
		return Super::GetNotifyName_Implementation();
	}
}

void UAnimNotify_PlayCameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	bool isUseCameraShake = true;
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		const int32 isUseCameraShakeInt =
			gameUserSettings->GetOutputSetting(EGsOptionOutput::IS_USE_CAMERA_SHAKE);

		isUseCameraShake = (bool)isUseCameraShakeInt;
	}
	// 옵션 꺼져있으면 처리 안함
	if (isUseCameraShake == false)
	{
		return;
	}

	// 일단 내 캐릭터에 해당되는 액터 타입만 적용
	if (CameraShake && Cast<AGsCharacterLocalPlayer>(MeshComp->GetOwner()))
	{
		if(UWorld* world = MeshComp->GetWorld())
		{
			UGameplayStatics::PlayWorldCameraShake(world, CameraShake, MeshComp->GetComponentLocation() + TargetOffset, InnerRadius, OuterRadius, Falloff, bOrientShakeTowardsEpicenter);
		}
	}
}