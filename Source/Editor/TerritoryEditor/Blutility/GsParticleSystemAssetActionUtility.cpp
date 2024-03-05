// Fill out your copyright notice in the Description page of Project Settings.

#define  SAVE_PARTICLE false

#include "GsParticleSystemAssetActionUtility.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleLODLevel.h"
#include "EditorUtilityLibrary.h"

const int32 INVALID_INDEX = -1;

void UGsParticleSystemAssetActionUtility::ModifySignificantLevelByLOD()
{
	TArray<UObject*> selectedParticleArray = UEditorUtilityLibrary::GetSelectedAssets();
	TArray<UPackage*> packagesToSave;
	UParticleSystem* particle = nullptr;

	for (UObject* selectedParticle : selectedParticleArray)
	{
		if (false == selectedParticle->IsA(UParticleSystem::StaticClass()))
		{
			continue;
		}

		particle = Cast<UParticleSystem>(selectedParticle);
		if (nullptr == particle)
		{
			continue;
		}

		UE_LOG(LogTemp, Log, TEXT("========================================="));
		UE_LOG(LogTemp, Log, TEXT("Modify Particle significant - name : %s"), *selectedParticle->GetPackage()->GetFName().ToString());

		ModifySignificantLevelByLOD_Internal(particle);

		UE_LOG(LogTemp, Log, TEXT("========================================="));

#if SAVE_PARTICLE
		packagesToSave.Add(particle);		
#else
		particle->MarkPackageDirty();
#endif
	}

#if SAVE_PARTICLE
	if (particle)
	{
		FEditorFileUtils::PromptForCheckoutAndSave(packagesToSave, true, false);
	}
#endif 
}

void UGsParticleSystemAssetActionUtility::ModifySignificantLevelByLOD_Internal(UParticleSystem* inParticleSystem)
{	
	int32 maxLODLevel = INVALID_INDEX;
	TArray<int32> lodLevelArray;
	int32 num = inParticleSystem->Emitters.Num();
	for (int32 i = 0; i < num; ++i)
	{
		lodLevelArray.Add(INVALID_INDEX);
	}

	//get max level 
	int32 lodLevel = INVALID_INDEX;
	for (int32 emitterIndex = 0; emitterIndex < num; ++emitterIndex)
	{
		TryGetLastLOD(lodLevel, inParticleSystem->Emitters[emitterIndex]);
		lodLevelArray[emitterIndex] = lodLevel;

		UE_LOG(LogTemp, Log, TEXT("ParticleSystemAssetAction - emitter - lod level : %d"), lodLevel);

		if (maxLODLevel < lodLevel)
		{
			maxLODLevel = lodLevel;
		}
	}

	for (int32 emitterIndex = 0; emitterIndex < num; ++emitterIndex)
	{
		SetSignificant(inParticleSystem->Emitters[emitterIndex], maxLODLevel, lodLevelArray[emitterIndex]);
	}
}

bool UGsParticleSystemAssetActionUtility::TryGetLastLOD(OUT int32& out_LOD_level, const class UParticleEmitter* inEmitter)
{
	out_LOD_level = 0;
	int32 num = inEmitter->LODLevels.Num();
	if (0 == num)
	{		
		return false;
	}

	int32 maxLodLevel = -1;
	UParticleLODLevel* lodLevel = nullptr;
	for (int32 lodLevelIndex = 0; lodLevelIndex < num; ++lodLevelIndex)
	{
		lodLevel = inEmitter->LODLevels[lodLevelIndex];
		if (maxLodLevel < lodLevel->Level
		&& lodLevel->bEnabled)
		{
			maxLodLevel = lodLevel->Level;
		}
	}	

	out_LOD_level = maxLodLevel;

	return true;
}

void UGsParticleSystemAssetActionUtility::SetSignificant(class UParticleEmitter* inEmitter, int32 inMaxLevel, int32 inLodLevel)
{
	if (inMaxLevel == inLodLevel)
	{
		inEmitter->SignificanceLevel = EParticleSignificanceLevel::Critical;
	}
	else if (0 == inLodLevel)
	{
		inEmitter->SignificanceLevel = EParticleSignificanceLevel::Low;
	}
	else
	{
		if (inLodLevel > (int32)(inMaxLevel / 2))
		{
			inEmitter->SignificanceLevel = EParticleSignificanceLevel::High;
		}
		else
		{
			inEmitter->SignificanceLevel = EParticleSignificanceLevel::Medium;
		}
	}	
}