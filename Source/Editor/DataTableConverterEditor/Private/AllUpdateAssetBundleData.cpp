#if WITH_DEV_AUTOMATION_TESTS

#include "AutomationTest.h"
#include "AssetRegistryModule.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Engine/Classes/Engine/PrimaryAssetLabel.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"
#include "Editor/UnrealEd/Public/FileHelpers.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AllUpdateAssetBundleData, TEXT("Raon.EditorPostBuild.9 All Update Asset Bundle Data")
	, EAutomationTestFlags::EditorContext | EAutomationTestFlags::MediumPriorityAndAbove | EAutomationTestFlags::EngineFilter)


bool AllUpdateAssetBundleData::RunTest(const FString& Parameters)
{
	UE_LOG(LogTemp, Log, TEXT("PrimaryAssetLabel AllUpdateAssetBundleData Call"));

	static const FName AssetRegistryName(TEXT("AssetRegistry"));
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryName);

	TArray<FAssetData> AssetList;
	AssetRegistryModule.Get().GetAssetsByClass(TEXT("PrimaryAssetLabel"), AssetList, true);

	for (int i = 0; i < AssetList.Num(); ++i)
	{
		if (0 <= AssetList[i].PackageName.ToString().Find(TEXT("launcher")))
		{
			// 경로에 Launcher 있는 경우 스킵.
			UE_LOG(LogTemp, Log, TEXT("Skip Asset: %s"), *AssetList[i].PackageName.ToString());
			continue;
		}

		UE_LOG(LogTemp, Log, TEXT("Resaving Asset: %s"), *AssetList[i].PackageName.ToString());

		UPrimaryAssetLabel* PrimaryAssetLabelObj = Cast<UPrimaryAssetLabel>(AssetList[i].GetAsset());
		
		if (PrimaryAssetLabelObj != nullptr)
		{
			//PrimaryAssetLabelObj->Modify();
			PrimaryAssetLabelObj->MarkPackageDirty();
			PrimaryAssetLabelObj->UpdateAssetBundleData();
			FEditorFileUtils::SaveDirtyPackage(PrimaryAssetLabelObj->GetOutermost(), true, false, true, true, false, false);
			//PrimaryAssetLabelObj->GetOutermost()->SavePackage(nullptr, nullptr,)
			
			UE_LOG(LogTemp, Log, TEXT("PrimaryAssetLabelObj->UpdateAssetBundleData() ok"));
		}
	}

	return true;
}

#endif
