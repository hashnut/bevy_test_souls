// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUSEMaterialUseFullPrecisionToggle.h"
#include "EditorUtilityLibrary.h"
#include "AssetRegistryModule.h"
#include "Editor/UnrealEd/Public/FileHelpers.h"

void UGsUSEMaterialUseFullPrecisionToggle::UseFullPrecisionOn()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> MaterialAssetData;
	const UClass* Class = UMaterial::StaticClass();
	AssetRegistryModule.Get().GetAssetsByClass(Class->GetFName(), MaterialAssetData);

	for (int32 AssetIdx = 0; AssetIdx < MaterialAssetData.Num(); ++AssetIdx)
	{
		const FAssetData& AssetData = MaterialAssetData[AssetIdx];
		UMaterial* Material = Cast<UMaterial>(AssetData.GetAsset());
		if (Material != nullptr)
		{
			Material->bUseFullPrecision = true;

			Material->MarkPackageDirty();
			FEditorFileUtils::SaveDirtyPackage(Material->GetOutermost(), false, true, true, false, false, false);
		}
	}
}

void UGsUSEMaterialUseFullPrecisionToggle::UseFullPrecisionOff()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> MaterialAssetData;
	const UClass* Class = UMaterial::StaticClass();
	AssetRegistryModule.Get().GetAssetsByClass(Class->GetFName(), MaterialAssetData);

	for (int32 AssetIdx = 0; AssetIdx < MaterialAssetData.Num(); ++AssetIdx)
	{
		const FAssetData& AssetData = MaterialAssetData[AssetIdx];
		UMaterial* Material = Cast<UMaterial>(AssetData.GetAsset());
		if (Material != nullptr)
		{
			Material->bUseFullPrecision = false;

			Material->MarkPackageDirty();
			FEditorFileUtils::SaveDirtyPackage(Material->GetOutermost(), false, true, true, false, false, false);
		}
	}
}
