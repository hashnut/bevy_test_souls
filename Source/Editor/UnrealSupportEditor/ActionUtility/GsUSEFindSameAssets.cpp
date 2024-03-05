// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUSEFindSameAssets.h"
#include "EditorUtilityLibrary.h"
#include "AssetRegistryModule.h"
#include "Editor/UnrealEd/Public/FileHelpers.h"

/*
1. 모든 파일 이름 리스트화
2. 검색을 통해 동일한 이름 리스트화
3. 동일한 이름들 용량(바이너리 체크, 동일한 파일인지 이름만 같은건지)
4. 출력창에 출력 및 파일로 저장하기
*/
void UGsUSEFindSameAssets::FindSameAssets()
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));

	const FString rootPaths = FString(TEXT("/Game/"));
	TArray<FString> outSubPaths;
	AssetRegistryModule.Get().GetSubPaths(FPaths::GetPath(rootPaths), outSubPaths, true);

	TArray<FAssetData> AllAssets;
	FARFilter Filter;
	for (int32 PathIdx = 0; PathIdx < outSubPaths.Num(); ++PathIdx)
	{
		Filter.PackagePaths.Add(*outSubPaths[PathIdx]);
	}

	TArray<FName> AssetNames;
	AssetRegistryModule.Get().GetAssets(Filter, AllAssets);
	for (int32 AssetIdx = 0; AssetIdx < AllAssets.Num(); ++AssetIdx)
		AssetNames.Add(AllAssets[AssetIdx].AssetName);

	TMap<FName, TArray<int32>> ResultAssetNames;
	TArray<int32> SkipIdxs; // 이미 추가한것은 검색 및 추가에서 제외
	TArray<int32> AddIdxs;
	for (int32 AssetIdx = 0; AssetIdx < AllAssets.Num(); ++AssetIdx)
	{
		if (SkipIdxs.Contains(AssetIdx))
			continue;

		AddIdxs.Empty();
		for (int32 AssetNameIdx = 0; AssetNameIdx < AssetNames.Num(); ++AssetNameIdx)
		{
			if (SkipIdxs.Contains(AssetNameIdx))
				continue;
			
			if (AssetIdx != AssetNameIdx 
				&& 0 == AllAssets[AssetIdx].AssetName.Compare(AssetNames[AssetNameIdx]))
			{
				SkipIdxs.Add(AssetNameIdx);				
				AddIdxs.Add(AssetNameIdx);
			}
		}

		if (AddIdxs.Num() <= 0)
			continue;

		AddIdxs.Add(AssetIdx);
		if (!ResultAssetNames.Contains(AllAssets[AssetIdx].AssetName))
		{
			ResultAssetNames.Add(AllAssets[AssetIdx].AssetName, AddIdxs);
		}
		ResultAssetNames[AllAssets[AssetIdx].AssetName] = AddIdxs;
		UE_LOG(LogTemp, Log, TEXT("//////////////////////////////////////////////////////////////////"));
		
		UE_LOG(LogTemp, Log, TEXT("AssetName : %s"), *AllAssets[AssetIdx].AssetName.ToString());

		UE_LOG(LogTemp, Log, TEXT("//////////////////////////////////////////////////////////////////"));
	}
}