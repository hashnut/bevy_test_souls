#include "PSOTools.h"

#include "EngineMinimal.h"
#include "AssetRegistryModule.h"
#include "Engine/Engine.h"

namespace PSOTools
{
	TArray<UParticleSystem*> ParticleSystemlist;

	void AutoPSOGetheringPSOData()
	{
		const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));

		const FString rootPaths = FString(TEXT("/Game/VFX/"));
		TArray<FString> outSubPaths;
		AssetRegistryModule.Get().GetSubPaths(FPaths::GetPath(rootPaths), outSubPaths, true);

		TArray<FAssetData> AllAssets;
		FARFilter Filter;
		for (int32 PathIdx = 0; PathIdx < outSubPaths.Num(); ++PathIdx)
		{
			Filter.PackagePaths.Add(*outSubPaths[PathIdx]);
		}

		ParticleSystemlist.Empty();
		AssetRegistryModule.Get().GetAssets(Filter, AllAssets);
		for (int32 AssetIdx = 0; AssetIdx < AllAssets.Num(); ++AssetIdx)
		{
			const FAssetData& AssetData = AllAssets[AssetIdx];
			UParticleSystem* ParticleSystem = Cast<UParticleSystem>(AssetData.GetAsset());
			if (ParticleSystem != nullptr)
			{				
				ParticleSystemlist.Add(ParticleSystem);
			}
		}
	}

	void LoadImmediatelyAndRun(UWorld* InWorld)
	{
		if (InWorld != nullptr)
		{
			for (int32 i = 0; i < ParticleSystemlist.Num(); ++i)
			{
				UParticleSystem* ParticleSystem = ParticleSystemlist[i];
				if (ParticleSystem)
				{
					UGameplayStatics::SpawnEmitterAtLocation(InWorld, ParticleSystem, FTransform::Identity, true, EPSCPoolMethod::AutoRelease);
				}
			}
		}
	}

	void Save()
	{

	}
}