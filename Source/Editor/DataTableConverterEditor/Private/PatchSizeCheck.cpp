#if WITH_DEV_AUTOMATION_TESTS


#include "AutomationTest.h"
#include "Runtime/Online/BuildPatchServices/Public/Interfaces/IBuildManifest.h"
#include "Runtime/Online/BuildPatchServices/Public/Interfaces/IBuildInstaller.h"
#include "Runtime/Online/BuildPatchServices/Public/Interfaces/IBuildPatchServicesModule.h"
#include "FileManager.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(PatchSizeCheck, TEXT("Raon.PatchSizeCheck.0 Patch Size Check")
	, EAutomationTestFlags::EditorContext | EAutomationTestFlags::MediumPriorityAndAbove | EAutomationTestFlags::EngineFilter)


bool PatchSizeCheck::RunTest(const FString& Parameters)
{
	UE_LOG(LogTemp, Log, TEXT("PatchSizeCheck Call"));

	FString BasePatchVersion;// = TEXT("1.0");
	FString CurrentPatchVersion;// = TEXT("1.1");
	FString AssetChunkIDStr;
	FString VoiceChunkIDStr;
	FString TargetPath;
	if (FParse::Value(FCommandLine::Get(), TEXT("BasePatchVersion="), BasePatchVersion))
	{
		UE_LOG(LogTemp, Log, TEXT("BasePatchVersion to '%s'"), *BasePatchVersion);
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("CurrentPatchVersion="), CurrentPatchVersion))
	{
		UE_LOG(LogTemp, Log, TEXT("CurrentPatchVersion to '%s'"), *CurrentPatchVersion);
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("AssetChunkID="), AssetChunkIDStr))
	{
		UE_LOG(LogTemp, Log, TEXT("AssetChunkIDStr to '%s'"), *AssetChunkIDStr);
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("VoiceChunkID="), VoiceChunkIDStr))
	{
		UE_LOG(LogTemp, Log, TEXT("VoiceChunkIDStr to '%s'"), *VoiceChunkIDStr);
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("BasePatchVersion="), BasePatchVersion))
	{
		UE_LOG(LogTemp, Log, TEXT("BasePatchVersion to '%s'"), *BasePatchVersion);
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("TargetPath="), TargetPath))
	{
		UE_LOG(LogTemp, Log, TEXT("TargetPath to '%s'"), *TargetPath);
	}

	int AssetChunkID = FCString::Atoi(*AssetChunkIDStr);
	int VoiceChunkID = FCString::Atoi(*VoiceChunkIDStr);

	UE_LOG(LogTemp, Log, TEXT("AssetChunkID to '%d'"), AssetChunkID);
	UE_LOG(LogTemp, Log, TEXT("VoiceChunkID to '%d'"), VoiceChunkID);

	static IBuildPatchServicesModule* BuildPatchServices = nullptr;
	if (BuildPatchServices == nullptr)
		BuildPatchServices = &FModuleManager::LoadModuleChecked<IBuildPatchServicesModule>(TEXT("BuildPatchServices"));

	if (BuildPatchServices == nullptr) return false;

	FString PlatforFolderName = TEXT("None");
	FString PlatformName = TargetPath;// UGameplayStatics::GetPlatformName();
	if (PlatformName.Compare(TEXT("Android")) == 0)  PlatforFolderName = TEXT("Android_ASTC");
	else if (PlatformName.Compare(TEXT("iOS")) == 0) PlatforFolderName = TEXT("IOS");
	else if (PlatformName.Compare(TEXT("Win64")) == 0) PlatforFolderName = TEXT("WindowsNoEditor");

	UE_LOG(LogTemp, Log, TEXT("Platform to '%s'"), *PlatformName);

	float TotalDonwloadSizeMB = .0f;
	float DataDonwloadSizeMB = .0f;
	float LocalAssetDonwloadSizeMB = .0f;
	float LocalVoiceDonwloadSizeMB = .0f;

	// A 패치 ~ B 패치 간에 패치용량 체크
	IBuildManifestPtr BaseManifest; // A
	IBuildManifestPtr CurrentManifest; // B	

	FString BaseManifestFileName = FString::Printf(TEXT("Zenonia_pakchunk1%s.manifest"), *BasePatchVersion);
	FString CurrentManifestFileName = FString::Printf(TEXT("Zenonia_pakchunk1%s.manifest"), *CurrentPatchVersion);

	FString BasePatchVersionFile = TEXT("../../../../_chunk") / PlatforFolderName / TEXT("CloudDir") / BaseManifestFileName;
	FString CurrentPatchVersionFile = TEXT("../../../../_chunk") / PlatforFolderName / TEXT("CloudDir") / CurrentManifestFileName;

	UE_LOG(LogTemp, Log, TEXT("BasePatchVersionFile to '%s'"), *BasePatchVersionFile);
	UE_LOG(LogTemp, Log, TEXT("CurrentPatchVersionFile to '%s'"), *CurrentPatchVersionFile);

	BaseManifest = BuildPatchServices->LoadManifestFromFile(BasePatchVersionFile);
	CurrentManifest = BuildPatchServices->LoadManifestFromFile(CurrentPatchVersionFile);

	double DonwloadSize = 0.0;

	if (CurrentManifest.IsValid())
	{
		if (BaseManifest.IsValid())
		{
			TSet<FString> Tags;
			DonwloadSize = (double)CurrentManifest->GetDeltaDownloadSize(Tags, BaseManifest.ToSharedRef());
		}
		else
		{
			DonwloadSize = (double)CurrentManifest->GetDownloadSize();
		}
	}

	DataDonwloadSizeMB = (float)FUnitConversion::Convert(DonwloadSize, EUnit::Bytes, EUnit::Megabytes);
	UE_LOG(LogTemp, Log, TEXT("Data Patch DonwloadSizeMB to %.2f"), DataDonwloadSizeMB);

	// 로컬라이징 관련 패치 용량 체크(아래는 기본 한국어 ID)

	// Asset
	FString BaseAssetManifestFileName = FString::Printf(TEXT("Zenonia_pakchunk%d%s.manifest"), AssetChunkID, *BasePatchVersion);
	FString CurrentAssetManifestFileName = FString::Printf(TEXT("Zenonia_pakchunk%d%s.manifest"), AssetChunkID, *CurrentPatchVersion);

	FString BaseAssetPatchVersionFile = TEXT("../../../../_chunk") / PlatforFolderName / TEXT("CloudDir") / BaseAssetManifestFileName;
	FString CurrentAssetPatchVersionFile = TEXT("../../../../_chunk") / PlatforFolderName / TEXT("CloudDir") / CurrentAssetManifestFileName;

	UE_LOG(LogTemp, Log, TEXT("BaseAssetPatchVersionFile to '%s'"), *BaseAssetPatchVersionFile);
	UE_LOG(LogTemp, Log, TEXT("CurrentAssetPatchVersionFile to '%s'"), *CurrentAssetPatchVersionFile);

	BaseManifest = BuildPatchServices->LoadManifestFromFile(BaseAssetPatchVersionFile);
	CurrentManifest = BuildPatchServices->LoadManifestFromFile(CurrentAssetPatchVersionFile);

	DonwloadSize = 0.0;

	if (CurrentManifest.IsValid())
	{
		if (BaseManifest.IsValid())
		{
			TSet<FString> Tags;
			DonwloadSize = (double)CurrentManifest->GetDeltaDownloadSize(Tags, BaseManifest.ToSharedRef());
		}
		else
		{
			DonwloadSize = (double)CurrentManifest->GetDownloadSize();
		}
	}
	
	LocalAssetDonwloadSizeMB = (float)FUnitConversion::Convert(DonwloadSize, EUnit::Bytes, EUnit::Megabytes);
	UE_LOG(LogTemp, Log, TEXT("Asset Patch DonwloadSizeMB to %.2f"), LocalAssetDonwloadSizeMB);

	// Voice
	FString BaseVoiceManifestFileName = FString::Printf(TEXT("Zenonia_pakchunk%d%s.manifest"), VoiceChunkID, *BasePatchVersion);
	FString CurrentVoiceManifestFileName = FString::Printf(TEXT("Zenonia_pakchunk%d%s.manifest"), VoiceChunkID, *CurrentPatchVersion);

	FString BaseVoicePatchVersionFile = TEXT("../../../../_chunk") / PlatforFolderName / TEXT("CloudDir") / BaseVoiceManifestFileName;
	FString CurrentVoicePatchVersionFile = TEXT("../../../../_chunk") / PlatforFolderName / TEXT("CloudDir") / CurrentVoiceManifestFileName;

	UE_LOG(LogTemp, Log, TEXT("BaseVoicePatchVersionFile to '%s'"), *BaseVoicePatchVersionFile);
	UE_LOG(LogTemp, Log, TEXT("CurrentVoicePatchVersionFile to '%s'"), *CurrentVoicePatchVersionFile);

	BaseManifest = BuildPatchServices->LoadManifestFromFile(BaseVoicePatchVersionFile);
	CurrentManifest = BuildPatchServices->LoadManifestFromFile(CurrentVoicePatchVersionFile);

	DonwloadSize = 0.0;

	if (CurrentManifest.IsValid())
	{
		if (BaseManifest.IsValid())
		{
			TSet<FString> Tags;
			DonwloadSize = (double)CurrentManifest->GetDeltaDownloadSize(Tags, BaseManifest.ToSharedRef());
		}
		else
		{
			DonwloadSize = (double)CurrentManifest->GetDownloadSize();
		}
	}

	LocalVoiceDonwloadSizeMB = (float)FUnitConversion::Convert(DonwloadSize, EUnit::Bytes, EUnit::Megabytes);
	UE_LOG(LogTemp, Log, TEXT("Voice Patch DonwloadSizeMB to %.2f"), LocalVoiceDonwloadSizeMB);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TotalDonwloadSizeMB = DataDonwloadSizeMB + LocalAssetDonwloadSizeMB + LocalVoiceDonwloadSizeMB;
	UE_LOG(LogTemp, Log, TEXT("TotalDonwloadSizeMB to %f"), TotalDonwloadSizeMB);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Save file 젠킨스 결과 출력용

	UE_LOG(LogTemp, Log, TEXT("call PatchSizeCheck Save"));

	FString FileText, FilePath;

	FileText = FString::Printf(TEXT("BasePatchVersion : %s\nCurrentPatchVersion : %s\nTotalDonwloadSizeMB : %f"), *BasePatchVersion, *CurrentPatchVersion, TotalDonwloadSizeMB);
	//FilePath = FPaths::AutomationDir() / TEXT("PatchSizeCheck.info");

	FString SaveDir = FPaths::ProjectPersistentDownloadDir() / TEXT("../Information") / TEXT("PatchSizeCheck.info");
	
	FTCHARToUTF8 FileUtf8(*FileText);
	IFileHandle* File = IPlatformFile::GetPlatformPhysical().OpenWrite(*SaveDir);
	if (File != nullptr)
	{
		// write to the file
		if (File->Write(reinterpret_cast<const uint8*>(FileUtf8.Get()), FileUtf8.Length()))
			UE_LOG(LogTemp, Log, TEXT("Wrote to %s"), *SaveDir);

		// close the file
		delete File;
	}

	return true;
}

#endif
