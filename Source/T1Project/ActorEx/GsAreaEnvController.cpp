// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAreaEnvController.h"
#include "Classes/Camera/CameraComponent.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Sound/GsSoundPlayer.h"


// Sets default values
AGsAreaEnvController::AGsAreaEnvController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGsAreaEnvController::BeginDestroy()
{
	Super::BeginDestroy();
}

void AGsAreaEnvController::OnChangeTimeline(EGsEnvTimeline InTimeline)
{
	if (InTimeline == EGsEnvTimeline::DAY ||
		InTimeline == EGsEnvTimeline::NIGHT)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			float ratio = static_cast<float>(InTimeline) / (static_cast<float>(EGsEnvTimeline::Max));
			soundPlayer->SetAudioParameter(EGsAudioParameterType::EnvTimeline, ratio);
		}
	}
}

UParticleSystemComponent* AGsAreaEnvController::GetLocalPlayerParticle()
{
	if (UCameraComponent* camera = GetLocalPlayerCamera())
	{
		const TArray<USceneComponent*>& attachedChildren = camera->GetAttachChildren();
		for (USceneComponent* iter : attachedChildren)
		{
			if (iter->IsA<UParticleSystemComponent>())
			{
				return Cast<UParticleSystemComponent>(iter);
			}
		}
	}

	return nullptr;
}

UCameraComponent* AGsAreaEnvController::GetLocalPlayerCamera()
{
	if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
	{
		UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(gameObjectMgr->FindObject(EGsGameObjectType::LocalPlayer));
		if (localPlayer)
		{
			if (AGsCharacterLocalPlayer* character = Cast<AGsCharacterLocalPlayer>(localPlayer->GetActor()))
			{
				return character->GetFollowCamera();
			}
		}
	}	

	return nullptr;
}