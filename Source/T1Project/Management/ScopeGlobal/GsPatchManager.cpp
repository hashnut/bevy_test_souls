#include "GsPatchManager.h"

#include "Runtime/Online/HTTP/Public/Http.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Runtime/PakFile/Public/IPlatformFilePak.h"
#include "Runtime/Online/BuildPatchServices/Public/Interfaces/IBuildPatchServicesModule.h"
#include "JsonObjectConverter.h"

#include "Management/ScopeGlobal/GsDivergenceManager.h"
#include "Management/GsMessageHolder.h"

#include "EngineSettings/Classes/GeneralProjectSettings.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "FileManager.h"
#include "PlatformFilemanager.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/GsMessageStage.h"

#define LOCTEXT_NAMESPACE "GsPatcheManager"
DEFINE_LOG_CATEGORY_STATIC(LogPatch, Log, All);

static const FString PATCH_INFO_FILE = TEXT("Patch.info");

template<>
UGsPatchManager* GsPatchManagerSingle::_instance = nullptr;

UGsPatchManager::~UGsPatchManager()
{
	UE_LOG(LogPatch, Log, TEXT("UGsPatchManager::~UGsPatchManager()"));
}

void UGsPatchManager::Initialize()
{
	UGsPatchManager::InitInstance(this);

#if PLATFORM_WINDOWS
	// Patch.info파일이 없으면 일단 Default로 
	if (false == GPatch()->LadoPatchInfoFile())
	{
		bDefaultPatchPath = true;
	}
	else
	{
		bDefaultPatchPath = (PatchInfo.DefaultPatchPath.Compare(TEXT("true")) == 0) ? true : false;
	}

	ChangePatchPath(bDefaultPatchPath);
#else
	PatchInstallPath = FPaths::ProjectPersistentDownloadDir() / TEXT("../Content/Patch/");
	DLCInstallPath = FPaths::ProjectPersistentDownloadDir() / TEXT("../Content/DLC/");
	PatchStagingPath = FPaths::ProjectPersistentDownloadDir() / TEXT("PatchStaging");
#endif
}

void UGsPatchManager::ChangePatchPath(bool DefaultPatchPath/*ProjectContentDir*/)
{
	bDefaultPatchPath = DefaultPatchPath;
	if (DefaultPatchPath)
	{
		PatchInstallPath = FPaths::ProjectContentDir() / TEXT("Patch");
		DLCInstallPath = FPaths::ProjectContentDir() / TEXT("DLC");
		PatchStagingPath = FPaths::ProjectContentDir() / TEXT("PatchStaging");
	}
	else
	{
		PatchInstallPath = FPaths::ProjectPersistentDownloadDir() / TEXT("../../Content/Patch/");
		DLCInstallPath = FPaths::ProjectPersistentDownloadDir() / TEXT("../../Content/DLC/");
		PatchStagingPath = FPaths::ProjectPersistentDownloadDir() / TEXT("PatchStaging");
	}
}

bool UGsPatchManager::PatchFileCleanUp()
{
	// 그전에 설치된 폴더에 패치 데이터가 있는지 체크
	FString OldPatchInstallPath;
	FString OldDLCInstallPath;
	FString OldPatchStagingPath;

	if (bDefaultPatchPath)
	{	
		OldPatchInstallPath = FPaths::ProjectPersistentDownloadDir() / TEXT("../../Content/Patch/");
		OldDLCInstallPath = FPaths::ProjectPersistentDownloadDir() / TEXT("../../Content/DLC/");
		OldPatchStagingPath = FPaths::ProjectPersistentDownloadDir() / TEXT("PatchStaging");
	}
	else
	{		
		OldPatchInstallPath = FPaths::ProjectContentDir() / TEXT("Patch");
		OldDLCInstallPath = FPaths::ProjectContentDir() / TEXT("DLC");
		OldPatchStagingPath = FPaths::ProjectContentDir() / TEXT("PatchStaging");
	}

	// 다른 폴더에 있는지 체크, 있으면 바뀐 폴더로 이동(복사 덮어쓰기 후 기존 폴더는 삭제
	// 해당 작업이 모두 완료 되어야만 성공
	// 일단 소스 폴더가 없어도 실패를 반환하니깐 폴더 체크는 먼저 하자 원본 폴더가 없으면 그냥 안하면 된다.

	bool bPatchFileCleanUp = true;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (IFileManager::Get().DirectoryExists(*OldPatchInstallPath))
	{
		bPatchFileCleanUp = PlatformFile.CopyDirectoryTree(*PatchInstallPath, *OldPatchInstallPath, true);
		if (bPatchFileCleanUp == false)
		{
			return bPatchFileCleanUp;
		}

		PlatformFile.DeleteDirectoryRecursively(*OldPatchInstallPath);
	}

	if (IFileManager::Get().DirectoryExists(*OldPatchStagingPath))
	{
		bPatchFileCleanUp = PlatformFile.CopyDirectoryTree(*PatchStagingPath, *OldPatchStagingPath, true);
		if (bPatchFileCleanUp == false)
		{
			return bPatchFileCleanUp;
		}

		PlatformFile.DeleteDirectoryRecursively(*OldPatchStagingPath);
	}

	// DLC는 배포가 안되었다. 이번에는 없음
	if (IFileManager::Get().DirectoryExists(*OldDLCInstallPath))
	{
		bPatchFileCleanUp = PlatformFile.CopyDirectoryTree(*DLCInstallPath, *OldDLCInstallPath, true);
		if (bPatchFileCleanUp == false)
		{
			return bPatchFileCleanUp;
		}

		PlatformFile.DeleteDirectoryRecursively(*OldDLCInstallPath);
	}
	
	SavePatchInfoFile();

	return bPatchFileCleanUp;
}

void UGsPatchManager::Finalize()
{
	UGsPatchManager::RemoveInstance();

	IBuildPatchServicesModule* BuildPatchServices = GetBuildPatchServices();
	if (BuildPatchServices)
		BuildPatchServices->CancelAllInstallers(true);
}

void UGsPatchManager::CancelInstall()
{
	if (Installer.IsValid())
	{
		Installer->CancelInstall();
		Installer.Reset();
	}
}

void UGsPatchManager::Update(float inTick)
{
}

float UGsPatchManager::GetDownloadSize()
{
	double DonwloadSize = 0.0;

	if (RemoteManifest.IsValid())
	{
		if (InstalledManifest.IsValid())
		{
			TSet<FString> Tags;
			DonwloadSize = (double)RemoteManifest->GetDeltaDownloadSize(Tags, InstalledManifest.ToSharedRef());
		}
		else
		{
			DonwloadSize = (double)RemoteManifest->GetDownloadSize();
		}
	}

	double DonwloadSizeMB = FUnitConversion::Convert(DonwloadSize, EUnit::Bytes, EUnit::Megabytes);
	return (float)DonwloadSizeMB;
}

float UGsPatchManager::GetRequiredDiskSpace()
{
	if (RemoteManifest.IsValid())
	{
		double BuildSize = (double)RemoteManifest->GetBuildSize();
		double BuildSizeMB = FUnitConversion::Convert(BuildSize, EUnit::Bytes, EUnit::Megabytes);
		return (float)BuildSizeMB;
	}

	return 0.0f;
}

float UGsPatchManager::GetTotalDownloadedSize()
{
	if (Installer.IsValid())
	{
		double SizeBytes = (double)Installer->GetTotalDownloaded();
		double SizeMB = FUnitConversion::Convert(SizeBytes, EUnit::Bytes, EUnit::Megabytes);
		return (float)SizeMB;
	}
	return 0.0f;
}

float UGsPatchManager::GetTotalDownloadRequiredSize()
{
	if (Installer.IsValid())
	{
		double SizeBytes = (double)Installer->GetTotalDownloadRequired();
		double SizeMB = FUnitConversion::Convert(SizeBytes, EUnit::Bytes, EUnit::Megabytes);
		return (float)SizeMB;
	}
	return 0.0f;
}

float UGsPatchManager::GetDownloadSpeed()
{
	if (Installer.IsValid())
	{
		double SpeedBytes = (double)Installer->GetDownloadSpeed();
		double SpeedMB = FUnitConversion::Convert(SpeedBytes, EUnit::Bytes, EUnit::Megabytes);
		return (float)SpeedMB;
	}
	return 0.0f;
}

float UGsPatchManager::GetInstallProgress()
{
	if (Installer.IsValid())
	{
		return Installer->GetUpdateProgress();
	}
	return 0.0f;
}

bool UGsPatchManager::GetDiskFreeSpace(OUT float& outDiskFreeSpaceMB)
{
	uint64 TotalDiskSpace = 0;
	uint64 TotalDiskFreeSpace = 0;
	if (FPlatformMisc::GetDiskTotalAndFreeSpace(PatchInstallPath, TotalDiskSpace, TotalDiskFreeSpace))
	{
		double DiskFreeSpace = (double)TotalDiskFreeSpace;
		double DiskFreeSpaceMB = FUnitConversion::Convert(DiskFreeSpace, EUnit::Bytes, EUnit::Megabytes);
		outDiskFreeSpaceMB = (float)DiskFreeSpaceMB;

		return true;
	}
	return false;
}

BuildPatchServices::EBuildPatchState UGsPatchManager::GetState()
{
	if (Installer.IsValid())
	{		
		return Installer->GetState();
	}

	return BuildPatchServices::EBuildPatchState::Initializing;
}

FString UGsPatchManager::GetStateString()
{
	if (Installer.IsValid())
	{
		return BuildPatchServices::StateToString(Installer->GetState());
	}

	return FString::Printf(TEXT("Patch Installer is Not Valid"));
}


bool UGsPatchManager::WriteStringAsUtf8TextFile(const FString& FileText, const FString& FilePath)
{
	// convert to UTF8
	FTCHARToUTF8 FileUtf8(*FileText);

	// open the file for writing
	bool bSuccess = false;
	IFileHandle* File = IPlatformFile::GetPlatformPhysical().OpenWrite(*FilePath);
	if (File != nullptr)
	{
		// write to the file
		if (File->Write(reinterpret_cast<const uint8*>(FileUtf8.Get()), FileUtf8.Length()))
		{
			UE_LOG(LogPatch, Log, TEXT("Wrote to %s"), *FilePath);
			bSuccess = true;
		}
		else
		{
			UE_LOG(LogPatch, Log, TEXT("Write error writing to %s"), *FilePath);
		}

		// close the file
		delete File;
	}
	else
	{
		UE_LOG(LogPatch, Log, TEXT("Unable open %s for writing."), *FilePath);
	}
	return bSuccess;
}

void UGsPatchManager::SavePatchInfoFile()
{
	UE_LOG(LogPatch, Log, TEXT("call SavePatchInfoFile()"));

	FString SaveDir = FPaths::ProjectPersistentDownloadDir() / TEXT("../Information/");
	IFileManager& FileManager = IFileManager::Get();
	if (!FileManager.MakeDirectory(*SaveDir, true))
	{
		UE_LOG(LogPatch, Log, TEXT("Unable to create cache folder at '%s'"), *SaveDir);
	}

	FString FilePath = SaveDir / PATCH_INFO_FILE;

	PatchInfo.DefaultPatchPath = bDefaultPatchPath == true ? TEXT("true") : TEXT("false");

	FString PatchInfoJson;
	if (FJsonObjectConverter::UStructToJsonObjectString(PatchInfo, PatchInfoJson))
	{
		if (!WriteStringAsUtf8TextFile(PatchInfoJson, FilePath))
		{
			UE_LOG(LogPatch, Log, TEXT("PatchInfo file Saved report to %s"), *FilePath);
		}
	}
}

bool UGsPatchManager::LadoPatchInfoFile()
{
	UE_LOG(LogPatch, Log, TEXT("call LadoPatchInfoFile()"));	

	TArray<FString> FindFile;
	FString InformationFolder = FPaths::ProjectPersistentDownloadDir() / TEXT("../Information/");
	FString FilePath = InformationFolder / PATCH_INFO_FILE;

	UE_LOG(LogPatch, Log, TEXT("Find Patch.info to '%s'"), *FilePath);
	IFileManager::Get().FindFiles(FindFile, *InformationFolder, TEXT("*.info"));

	if (FindFile.Num() > 0)
	{
		IFileHandle* PatchInfoFile = IPlatformFile::GetPlatformPhysical().OpenRead(*FilePath);
		if (PatchInfoFile != nullptr)
		{
			int64 FileSize = PatchInfoFile->Size();
			char* FileBuffer = new char[FileSize + 8]; // little extra since we're forcing null term in places outside bounds of a field
			if (PatchInfoFile->Read((uint8*)FileBuffer, FileSize))
			{
				FileBuffer[FileSize] = '\0';
				FString Temp(FileBuffer);
				if (FJsonObjectConverter::JsonObjectStringToUStruct(Temp, &PatchInfo, 0, 0))
				{
					// close the file
					delete PatchInfoFile;					

					return true;
				}				
			}

			// close the file
			delete PatchInfoFile;
		}
	}
	// 버전이 같지 않거나, 해당 파일이 없을때는(최초설치) 패치를 받아야 된다.
	UE_LOG(LogPatch, Log, TEXT("Not Find Patch.info"));
	return false;
}

// 무조건 한번은 호출되는 함수, 초기화도 여기서 해주면 된다. 한번은 무조건 한다는 가정하에
void UGsPatchManager::CheckDownloadRequest(int InAssetChunkID, int InVoiceChunkID)
{
	UE_LOG(LogPatch, Log, TEXT("call CheckDownloadRequest(InAssetChunkID : %d, InVoiceChunkID : %d) begin..."), InAssetChunkID, InVoiceChunkID);
	UE_LOG(LogPatch, Log, TEXT("...................................."));
	
	FString ServiceDir = GDivergence()->GetService();
	if (false == GDivergence()->GetServiceDirOverride().IsEmpty())
		ServiceDir = GDivergence()->GetServiceDirOverride();

	UE_LOG(LogPatch, Log, TEXT("ServiceDir : %s"), *ServiceDir);

	ManifestListFileURL = GDivergence()->GetCdnUrl() / ServiceDir / GetCloudManifestListFolder() / TEXT("CloudDir") / GetCloudManifestListFileName();
	CloudDirURL = GDivergence()->GetCdnUrl() / ServiceDir / GetCloudManifestListFolder() / TEXT("CloudDir");

	// 실행시 필요한 로컬라이징 애셋 id
	AssetChunkID = InAssetChunkID;
	VoiceChunkID = InVoiceChunkID;

	// PatchingProcessOn - 옵션이 켜져 있으면 기존대로 프로세스를 타면 된다.
	if (true == GDivergence()->PatchingProcessOn())
	{
		MakeRequestManifestNames();
		StartTotalDownloadSizeCalc();
	}
	// Off되어 있으면 패치가 되있다고 할필요 없다고 반영(주의:Chunk1은 한번은 받아 있어야 정상 인게임 진입이 된다)
	else
	{
		// BSAM_PATCH_DATA_REPAIR
		bool Download = false;
		bool Mount = LocalMount();

		if (OnRequestCheckDownloadComplete().IsBound())
			OnRequestCheckDownloadComplete().ExecuteIfBound(Download, Mount, TotalDownloadSize);
	}
}

bool UGsPatchManager::ParsingManifestList(const FString& ManifestString)
{
	if (false == FJsonObjectConverter::JsonObjectStringToUStruct(ManifestString, &ManifestList, 0, 0))
	{
		UE_LOG(LogPatch, Log, TEXT("Failed to parsing patch.manifestlist file to '%s'"), *ManifestString);
		return false;
	}

	return true;
}

// 적용할 패치 manifest 파일들 이름을 Divergence를 참조 조합 한다.
void UGsPatchManager::MakeRequestManifestNames()
{
	RequestManifestNames.Empty();

	FString ProjectName;
	const UGeneralProjectSettings* Settings = GetDefault<UGeneralProjectSettings>();
	if (Settings)
	{
		ProjectName = Settings->ProjectName;
	}

	FString localManifestToFilePath = PatchInstallPath;
	FString remoteManifestURL = CloudDirURL / FString::Printf(TEXT("%s_pakchunk1%s.manifest"), *ProjectName, *(GDivergence()->GetPatchVersion()));
	FString localManifestToFileName = FString::Printf(TEXT("%s_pakchunk1.manifest"), *ProjectName);
	RequestManifestNames.Add(FGsRequestManifestInfo(remoteManifestURL, localManifestToFileName, localManifestToFilePath));

	remoteManifestURL = CloudDirURL / FString::Printf(TEXT("%s_pakchunk%d%s.manifest"), *ProjectName, AssetChunkID, *(GDivergence()->GetPatchVersion()));
	localManifestToFileName = FString::Printf(TEXT("%s_pakchunk%d.manifest"), *ProjectName, AssetChunkID);
	RequestManifestNames.Add(FGsRequestManifestInfo(remoteManifestURL, localManifestToFileName, localManifestToFilePath));

	remoteManifestURL = CloudDirURL / FString::Printf(TEXT("%s_pakchunk%d%s.manifest"), *ProjectName, VoiceChunkID, *(GDivergence()->GetPatchVersion()));
	localManifestToFileName = FString::Printf(TEXT("%s_pakchunk%d.manifest"), *ProjectName, VoiceChunkID);
	RequestManifestNames.Add(FGsRequestManifestInfo(remoteManifestURL, localManifestToFileName, localManifestToFilePath));

	if (GDivergence()->GetDLCVersion().Len() > 0)
	{
		localManifestToFilePath = DLCInstallPath;

		remoteManifestURL = CloudDirURL / FString::Printf(TEXT("%s_pakchunk1%s.manifest"), *ProjectName, *(GDivergence()->GetDLCVersion()));
		localManifestToFileName = FString::Printf(TEXT("%s_pakchunk1_DLC.manifest"), *ProjectName);
		RequestManifestNames.Add(FGsRequestManifestInfo(remoteManifestURL, localManifestToFileName, localManifestToFilePath));

		remoteManifestURL = CloudDirURL / FString::Printf(TEXT("%s_pakchunk%d%s.manifest"), *ProjectName, AssetChunkID, *(GDivergence()->GetDLCVersion()));
		localManifestToFileName = FString::Printf(TEXT("%s_pakchunk%d_DLC.manifest"), *ProjectName, AssetChunkID);
		RequestManifestNames.Add(FGsRequestManifestInfo(remoteManifestURL, localManifestToFileName, localManifestToFilePath));

		remoteManifestURL = CloudDirURL / FString::Printf(TEXT("%s_pakchunk%d%s.manifest"), *ProjectName, VoiceChunkID, *(GDivergence()->GetDLCVersion()));
		localManifestToFileName = FString::Printf(TEXT("%s_pakchunk%d_DLC.manifest"), *ProjectName, VoiceChunkID);
		RequestManifestNames.Add(FGsRequestManifestInfo(remoteManifestURL, localManifestToFileName, localManifestToFilePath));
	}
}

IBuildManifestPtr UGsPatchManager::GetInstalledManifest(const FString& FindLocalManifestFilePath, const FString& FindLocalManifestFileName)
{
	IBuildManifestPtr InstallManifest = nullptr;

	TArray<FString> InstalledManifestNames;
	IFileManager::Get().FindFilesRecursive(InstalledManifestNames, *FindLocalManifestFilePath, *FindLocalManifestFileName, true, false);
	if (InstalledManifestNames.Num() > 0)
	{
		FString InstalledManifestName = InstalledManifestNames[0];
		IBuildPatchServicesModule* BuildPatchServices = GetBuildPatchServices();
		InstallManifest = BuildPatchServices->LoadManifestFromFile(InstalledManifestName);

		UE_LOG(LogPatch, Log, TEXT("GetInstalledManifest() LoadManifestFromFile to '%s'"), *InstalledManifestName);
	}
	
	return InstallManifest;
}

void UGsPatchManager::DownloadSizeCalc()
{
	DownloadSizeCalcHttpRequest = FHttpModule::Get().CreateRequest();
	// http연결 제한시간 설정인데 엔진(Engine.ini) 기본은 300초(5분으로 셋팅)로 되어 있다. 패치 혹은 체크시 해당 대기 시간은 너무 길기에 20초로 변경(엔진에서 안바꾸는 이유는 전체바꾸면 문제가 발생할 요지가 있어서)
	// 안덕규님 추천으로 좋은 방안으로 채택 적용함
	DownloadSizeCalcHttpRequest->SetTimeout(20.0f);
	DownloadSizeCalcHttpRequest->SetURL(*RequestManifestNameURLList[0].RemoteManifestURL);
	DownloadSizeCalcHttpRequest->SetVerb(TEXT("GET"));
	DownloadSizeCalcHttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess) {
		if (bSuccess && HttpResponse.IsValid())
		{
			const int32 HttpStatus = HttpResponse->GetResponseCode();
			if (EHttpResponseCodes::IsOk(HttpStatus))
			{
				IBuildPatchServicesModule* BuildPatchServices = GetBuildPatchServices();

				// 설치된 매니페스트(없을수도 있음)
				InstalledManifest = GetInstalledManifest(RequestManifestNameURLList[0].LocalManifestToFilePath, RequestManifestNameURLList[0].LocalManifestToFileName);
				// 설치할 매니페스트
				RemoteManifest = BuildPatchServices->MakeManifestFromData(HttpResponse->GetContent());

				TotalDownloadSize += GetDownloadSize();

				RequestManifestNameURLList.RemoveAt(0);				

				if (RequestManifestNameURLList.Num() > 0)
				{
					DownloadSizeCalc();
				}
				else
				{
					// BSAM_PATCH_DATA_REPAIR
					bool Download = false;
					bool Mount = false;
					if (TotalDownloadSize > FLT_EPSILON) Download = true; // 패치 받아야된다.
					else Mount = LocalMount();

					if (OnRequestCheckDownloadComplete().IsBound())
						OnRequestCheckDownloadComplete().ExecuteIfBound(Download, Mount, TotalDownloadSize);
				}
			}
			else
			{
				UE_LOG(LogPatch, Log, TEXT("HTTP %d while loading RemoteManifestURL from '%s'"), HttpStatus, *HttpRequest->GetURL());

				FString msg = FString::Format(TEXT("HTTP {0} while loading RemoteManifestURL from {1}"), { HttpStatus, *HttpRequest->GetURL() });
				FGsUIHelper::PopupSystemMsg(FText::FromString(msg));
			}
		}
		else
		{
			UE_LOG(LogPatch, Log, TEXT("HTTP connection issue while loading RemoteManifestURL '%s'"), *HttpRequest->GetURL());

			FString msg = FString::Format(TEXT("HTTP connection issue while loading RemoteManifestURL {0}"), { *HttpRequest->GetURL() });
			FGsUIHelper::PopupSystemMsg(FText::FromString(msg));

		}});
	DownloadSizeCalcHttpRequest->ProcessRequest();
}

bool UGsPatchManager::LocalMount()//int32 PakOrder, const FString& MountPoint, const FString& PakPath, const FString& PakName)
{
	/*
	* Pak파일은 크게 두개가 있다.
	* Chunk1의 main인 pakchunk1-platform.pak
	* 그리고 리소스만 패치한 pakchunk1-platform_P.pak
	* 이두가지만 mount 하면된다. Pak의 업데이트, 생성, 버전 관리등은 다른 함수에서 처리한다.
	*/

	// FPakPlatformFile 은 editor일때는 null, eidtor에서  test용으로 생성하지만 그냥 두어도 상관없다.
	FPakPlatformFile* PakFileMgr = (FPakPlatformFile*)(FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile")));
	if (PakFileMgr == nullptr)
	{
		FPakPlatformFile* PlatformFile = new FPakPlatformFile;
		if (!PlatformFile->Initialize(&FPlatformFileManager::Get().GetPlatformFile(), TEXT("")))
		{
			return false;
		}

		PlatformFile->InitializeNewAsyncIO();
		FPlatformFileManager::Get().SetPlatformFile(*PlatformFile);

		PakFileMgr = (FPakPlatformFile*)(FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile")));
		if (PakFileMgr == nullptr)
			return false;
	}

	IPlatformFile* LowerLevelPlatformFile = &FPlatformFileManager::Get().GetPlatformFile();
	if (nullptr != LowerLevelPlatformFile)
	{
		uint32 PakOrder = 10;
		TArray<FString> FindPakFiles;
		IFileManager::Get().FindFilesRecursive(FindPakFiles, *PatchInstallPath, TEXT("*.pak"), true, false);		
		for (FString PakFileName : FindPakFiles)
		{
			FPakFile PakFile(LowerLevelPlatformFile, *PakFileName, false);
			const TCHAR* MountPoint = PakFile.GetMountPoint().GetCharArray().GetData();
			if (MountPoint != nullptr)
			{
				if (PakFileMgr->Mount(*PakFileName, ++PakOrder, MountPoint))
				{
					UE_LOG(LogPatch, Log, TEXT("Mounted = %s, Order = %d, MountPoint = %s"), *PakFileName, PakOrder, !MountPoint ? TEXT("(null)") : MountPoint);
				}
				else
				{
					UE_LOG(LogPatch, Log, TEXT("Failed to main mount pak = %s"), *PakFileName);
					return false;
				}
			}
			else
			{
				UE_LOG(LogPatch, Log, TEXT("Failed to MountPoint is nullptr"));
			}
		}

		PakOrder = 100;
		FindPakFiles.Empty();
		IFileManager::Get().FindFilesRecursive(FindPakFiles, *DLCInstallPath, TEXT("*.pak"), true, false);
		for (FString PakFileName : FindPakFiles)
		{
			FPakFile PakFile(LowerLevelPlatformFile, *PakFileName, false);
			const TCHAR* MountPoint = PakFile.GetMountPoint().GetCharArray().GetData();
			if (MountPoint != nullptr)
			{
				if (PakFileMgr->Mount(*PakFileName, ++PakOrder, MountPoint))
				{
					UE_LOG(LogPatch, Log, TEXT("Mounted = %s, Order = %d, MountPoint = %s"), *PakFileName, PakOrder, !MountPoint ? TEXT("(null)") : MountPoint);
				}
				else
				{
					UE_LOG(LogPatch, Log, TEXT("Failed to main mount pak = %s"), *PakFileName);
					return false;
				}
			}
			else
			{
				UE_LOG(LogPatch, Log, TEXT("Failed to MountPoint is nullptr"));
			}
		}
	}
	else
	{
		UE_LOG(LogPatch, Log, TEXT("Failed to LowerLevelPlatformFile is nullptr"));
	}

	return true;
}

void UGsPatchManager::StartTotalDownloadSizeCalc()
{
	// load the divergence from the root CDN
	StartTotalDownloadSizeCalcHttpRequest = FHttpModule::Get().CreateRequest();
	StartTotalDownloadSizeCalcHttpRequest->SetTimeout(20.0f);
	StartTotalDownloadSizeCalcHttpRequest->SetURL(*ManifestListFileURL);
	StartTotalDownloadSizeCalcHttpRequest->SetVerb(TEXT("GET"));
	StartTotalDownloadSizeCalcHttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess) {
		if (bSuccess && HttpResponse.IsValid())
		{
			const int32 HttpStatus = HttpResponse->GetResponseCode();
			if (EHttpResponseCodes::IsOk(HttpStatus))
			{
				// Parsing
				if (ParsingManifestList(HttpResponse->GetContentAsString()))
				{
					UE_LOG(LogPatch, Log, TEXT("Succeeded to parsing ManifestListCdnUrl to '%s'"), *HttpResponse->GetContentAsString());

					// 요청할 CDN Cloud Manifest files이름에 URL경로를 합치기, 해당 배열이 재귀용법에 종로 트리거도 된다.
					RequestManifestNameURLList.Empty();
					RequestManifestNameURLList = RequestManifestNames;
					TotalDownloadSize = .0f;

					DownloadSizeCalc();										
				}
				else
				{
					UE_LOG(LogPatch, Log, TEXT("Failed to parsing ManifestListCdnUrl to '%s'"), *HttpResponse->GetContentAsString());

					FString msg = FString::Format(TEXT("Failed to parsing ManifestListCdnUrl to {0}"), { *HttpResponse->GetContentAsString() });
					FGsUIHelper::PopupSystemMsg(FText::FromString(msg));
				}
			}
			else
			{
				UE_LOG(LogPatch, Log, TEXT("HTTP %d while loading ManifestListCdnUrl from '%s'"), HttpStatus, *HttpRequest->GetURL());

				FString msg = FString::Format(TEXT("HTTP %d while loading ManifestListCdnUrl from {0}"), { HttpStatus , *HttpRequest->GetURL() });
				FGsUIHelper::PopupSystemMsg(FText::FromString(msg));
			}
		}
		else
		{
			UE_LOG(LogPatch, Log, TEXT("HTTP connection issue while loading ManifestListCdnUrl '%s'"), *HttpRequest->GetURL());

			FString msg = FString::Format(TEXT("HTTP connection issue while loading ManifestListCdnUrl {0}"), { *HttpRequest->GetURL() });
			FGsUIHelper::PopupSystemMsg(FText::FromString(msg));
		}

		// try to load it				
		if (!this->IsA<UGsPatchManager>())
		{
			UE_LOG(LogPatch, Log, TEXT("UGsPatchManager was destroyed while loading ManifestListCdnUrl '%s'"), *HttpRequest->GetURL());

			FString msg = FString::Format(TEXT("UGsPatchManager was destroyed while loading ManifestListCdnUrl {0}"), { *HttpRequest->GetURL() });
			FGsUIHelper::PopupSystemMsg(FText::FromString(msg));

			return;
		}
		});
	StartTotalDownloadSizeCalcHttpRequest->ProcessRequest();
}

void UGsPatchManager::PatchProcessRequest()
{
	UE_LOG(LogPatch, Log, TEXT("call PatchProcessRequest() begin..."));
	UE_LOG(LogPatch, Log, TEXT("...................................."));

	RequestManifestNameURLList.Empty();
	RequestManifestNameURLList = RequestManifestNames;

	StartPatch();
}

void UGsPatchManager::RepairProcessRequest(int InAssetChunkID, int InVoiceChunkID)
{
	UE_LOG(LogPatch, Log, TEXT("call RepairProcessRequest() begin..."));
	UE_LOG(LogPatch, Log, TEXT("...................................."));

	FString ServiceDir = GDivergence()->GetService();
	if (false == GDivergence()->GetServiceDirOverride().IsEmpty())
		ServiceDir = GDivergence()->GetServiceDirOverride();

	UE_LOG(LogPatch, Log, TEXT("ServiceDir : %s"), *ServiceDir);

	ManifestListFileURL = GDivergence()->GetCdnUrl() / ServiceDir / GetCloudManifestListFolder() / TEXT("CloudDir") / GetCloudManifestListFileName();
	CloudDirURL = GDivergence()->GetCdnUrl() / ServiceDir / GetCloudManifestListFolder() / TEXT("CloudDir");

	// 실행시 필요한 로컬라이징 애셋 id
	AssetChunkID = InAssetChunkID;
	VoiceChunkID = InVoiceChunkID;

	MakeRequestManifestNames();

	RequestManifestNameURLList.Empty();
	RequestManifestNameURLList = RequestManifestNames;

	StartRepair();
}

void UGsPatchManager::RepairComplete(const IBuildInstallerRef& InstallerRef)
{
	if (InstallerRef->CompletedSuccessfully())
	{
		IBuildPatchServicesModule* BuildPatchServices = GetBuildPatchServices();
		FString ManifestFullFilename = RequestManifestNameURLList[0].LocalManifestToFilePath / RequestManifestNameURLList[0].LocalManifestToFileName;
		BuildPatchServices->SaveManifestToFile(ManifestFullFilename, RemoteManifest.ToSharedRef());

		RequestManifestNameURLList.RemoveAt(0);

		if (RequestManifestNameURLList.Num() > 0)
		{
			Repairing();
		}
		else
		{
			// BSAM_PATCH_DATA_REPAIR
			if(LocalMount())
				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::REPAIR_TRUE, nullptr);
			else
				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::REPAIR_FALSE, nullptr);
		}
	}
	else
	{
		UE_LOG(LogPatch, Log, TEXT("Repair Error '%s'"), *InstallerRef->GetErrorCode());

		BuildPatchRepairError = InstallerRef->GetErrorType();
		PatchRepairFail = true;

		GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::REPAIR_FALSE, nullptr);
	}
}

void UGsPatchManager::InstallComplete(const IBuildInstallerRef& InstallerRef)
{	
	if (InstallerRef->CompletedSuccessfully())
	{
		IBuildPatchServicesModule* BuildPatchServices = GetBuildPatchServices();
		FString ManifestFullFilename = RequestManifestNameURLList[0].LocalManifestToFilePath / RequestManifestNameURLList[0].LocalManifestToFileName;
		BuildPatchServices->SaveManifestToFile(ManifestFullFilename, RemoteManifest.ToSharedRef());

		RequestManifestNameURLList.RemoveAt(0);

		if (RequestManifestNameURLList.Num() > 0)
		{
			Patching();
		}
		else
		{
			// BSAM_PATCH_DATA_REPAIR
			if (LocalMount())
				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::PATCH_TRUE, nullptr);
			else
				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::PATCH_FALSE, nullptr);
		}
	}
	else
	{
		UE_LOG(LogPatch, Log, TEXT("Install Error '%s'"), *InstallerRef->GetErrorCode());

		BuildPatchInstallError = InstallerRef->GetErrorType();
		PatchInstallFail = true;

		GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::PATCH_FALSE, nullptr);
	}
}

EBuildPatchInstallError UGsPatchManager::GetInstallFailError()
{
	return BuildPatchInstallError;
}

EBuildPatchInstallError UGsPatchManager::GetRepairFailError()
{
	return BuildPatchRepairError;
}

bool UGsPatchManager::IsInstallFail() // 인스톨 설치 에러 인지 그외 에러인지 체크하기 위한 함수
{
	return PatchInstallFail;
}

bool UGsPatchManager::IsRepairFail() // Repair 에러 인지 그외 에러인지 체크하기 위한 함수
{
	return PatchRepairFail;
}

bool UGsPatchManager::IsCheckPatchInfoFile()
{
	TArray<FString> FindFile;
	FString InformationFolder = FPaths::ProjectPersistentDownloadDir() / TEXT("../Information/");
	FString FilePath = InformationFolder / PATCH_INFO_FILE;
	IFileManager::Get().FindFiles(FindFile, *InformationFolder, TEXT("*.info"));

	if (FindFile.Num() > 0)		
		return true;

	return false;
}

void UGsPatchManager::Repairing()
{
	PatchingHttpRequest = FHttpModule::Get().CreateRequest();
	PatchingHttpRequest->SetTimeout(20.0f);
	PatchingHttpRequest->SetURL(*RequestManifestNameURLList[0].RemoteManifestURL);
	PatchingHttpRequest->SetVerb(TEXT("GET"));
	PatchingHttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess) {
		if (bSuccess && HttpResponse.IsValid())
		{
			const int32 HttpStatus = HttpResponse->GetResponseCode();
			if (EHttpResponseCodes::IsOk(HttpStatus))
			{
				IBuildPatchServicesModule* BuildPatchServices = GetBuildPatchServices();
				RemoteManifest = BuildPatchServices->MakeManifestFromData(HttpResponse->GetContent());

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				TArray<BuildPatchServices::FInstallerAction> InstallerActions;
				InstallerActions.Add(BuildPatchServices::FInstallerAction::MakeRepair(RemoteManifest.ToSharedRef(), TSet<FString>()));

				BuildPatchServices::FBuildInstallerConfiguration Configuration(MoveTemp(InstallerActions));
				Configuration.StagingDirectory = PatchStagingPath;
				Configuration.CloudDirectories = { CloudDirURL };
				Configuration.InstallDirectory = RequestManifestNameURLList[0].LocalManifestToFilePath;
				// add
				Configuration.DeltaPolicy = BuildPatchServices::EDeltaPolicy::TryFetchContinueWithout;

				Installer = BuildPatchServices->CreateBuildInstaller(Configuration, FBuildPatchInstallerDelegate::CreateUObject(this, &UGsPatchManager::RepairComplete));
				Installer->StartInstallation();

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
			else
			{
				UE_LOG(LogPatch, Log, TEXT("HTTP %d while loading RemoteManifestURL from '%s'"), HttpStatus, *HttpRequest->GetURL());

				FString msg = FString::Format(TEXT("HTTP {0} while loading RemoteManifestURL from {1}"), { HttpStatus , *HttpRequest->GetURL() });
				FGsUIHelper::PopupSystemMsg(FText::FromString(msg));
			}
		}
		else
		{
			UE_LOG(LogPatch, Log, TEXT("HTTP connection issue while loading RemoteManifestURL '%s'"), *HttpRequest->GetURL());

			FString msg = FString::Format(TEXT("HTTP connection issue while loading RemoteManifestURL {0}"), { *HttpRequest->GetURL() });
			FGsUIHelper::PopupSystemMsg(FText::FromString(msg));
		}});
	PatchingHttpRequest->ProcessRequest();
}

void UGsPatchManager::Patching()
{
	PatchingHttpRequest = FHttpModule::Get().CreateRequest();
	PatchingHttpRequest->SetTimeout(20.0f);
	PatchingHttpRequest->SetURL(*RequestManifestNameURLList[0].RemoteManifestURL);
	PatchingHttpRequest->SetVerb(TEXT("GET"));
	PatchingHttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess) {
		if (bSuccess && HttpResponse.IsValid())
		{
			const int32 HttpStatus = HttpResponse->GetResponseCode();
			if (EHttpResponseCodes::IsOk(HttpStatus))			
			{
				// 이미 설치된 Manifest파일
				InstalledManifest = GetInstalledManifest(RequestManifestNameURLList[0].LocalManifestToFilePath, RequestManifestNameURLList[0].LocalManifestToFileName);

				// 설치할 Manifest파일
				IBuildPatchServicesModule* BuildPatchServices = GetBuildPatchServices();
				RemoteManifest = BuildPatchServices->MakeManifestFromData(HttpResponse->GetContent());

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				TArray<BuildPatchServices::FInstallerAction> InstallerActions;
				InstallerActions.Add(BuildPatchServices::FInstallerAction::MakeInstallOrUpdate(InstalledManifest, RemoteManifest.ToSharedRef(), TSet<FString>()));

				BuildPatchServices::FBuildInstallerConfiguration Configuration(MoveTemp(InstallerActions));
				Configuration.StagingDirectory = PatchStagingPath;
				Configuration.CloudDirectories = { CloudDirURL };
				Configuration.InstallDirectory = RequestManifestNameURLList[0].LocalManifestToFilePath;
				// add
				Configuration.DeltaPolicy = BuildPatchServices::EDeltaPolicy::TryFetchContinueWithout;

				Installer = BuildPatchServices->CreateBuildInstaller(Configuration, FBuildPatchInstallerDelegate::CreateUObject(this, &UGsPatchManager::InstallComplete));
				Installer->StartInstallation();

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
			else
			{
				UE_LOG(LogPatch, Log, TEXT("HTTP %d while loading RemoteManifestURL from '%s'"), HttpStatus, *HttpRequest->GetURL());

				FString msg = FString::Format(TEXT("HTTP {0} while loading RemoteManifestURL from {1}"), { HttpStatus , *HttpRequest->GetURL() });
				FGsUIHelper::PopupSystemMsg(FText::FromString(msg));
			}
		}
		else
		{
			UE_LOG(LogPatch, Log, TEXT("HTTP connection issue while loading RemoteManifestURL '%s'"), *HttpRequest->GetURL());

			FString msg = FString::Format(TEXT("HTTP connection issue while loading RemoteManifestURL {0}"), { *HttpRequest->GetURL() });
			FGsUIHelper::PopupSystemMsg(FText::FromString(msg));
		}});
	PatchingHttpRequest->ProcessRequest();
}

void UGsPatchManager::TryLoadManifestFile(int TryNumber)
{
	// load the divergence from the root CDN
	TryLoadManifestFileHttpRequest = FHttpModule::Get().CreateRequest();
	TryLoadManifestFileHttpRequest->SetTimeout(20.0f);
	TryLoadManifestFileHttpRequest->SetURL(*RequestManifestNameURLList[0].RemoteManifestURL);
	TryLoadManifestFileHttpRequest->SetVerb(TEXT("GET"));
	TryLoadManifestFileHttpRequest->OnProcessRequestComplete().BindLambda([this, TryNumber](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess) {
		if (HttpRequest.IsValid())
		{
			const EHttpRequestStatus::Type HttpStatus = HttpRequest->GetStatus();
			if (EHttpRequestStatus::Type::Succeeded == HttpStatus)
			{
				UE_LOG(LogPatch, Log, TEXT("HttpRequest to Finished and was successful"));
			}
			else
			{
				UE_LOG(LogPatch, Log, TEXT("HttpRequest to Failed and %s"), EHttpRequestStatus::ToString(HttpStatus));

				FString msg = FString::Format(TEXT("HttpRequest to Failed and {0}"), { EHttpRequestStatus::ToString(HttpStatus) });
				FGsUIHelper::PopupSystemMsg(FText::FromString(msg));

				PatchInstallFail = false;
				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::PATCH_FALSE, nullptr);
				return;
			}
		}
		else
		{
			UE_LOG(LogPatch, Log, TEXT("HttpRequest to nullptr"));

			FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("HttpRequest to nullptr")));

			PatchInstallFail = false;
			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::PATCH_FALSE, nullptr);
			return;
		}

		if (bSuccess && HttpResponse.IsValid())
		{
			const int32 HttpStatus = HttpResponse->GetResponseCode();
			if (EHttpResponseCodes::IsOk(HttpStatus))
			{
				UE_LOG(LogPatch, Log, TEXT("Succeeded to parsing manifestlist to '%s'"), *HttpResponse->GetContentAsString());

				// 요청할 CDN Cloud Manifest files이름에 URL경로를 합치기, 해당 배열이 재귀용법에 종로 트리거도 된다.
				RequestManifestNameURLList.Empty();
				RequestManifestNameURLList = RequestManifestNames;

				Patching();				
			}
			else
			{
				UE_LOG(LogPatch, Log, TEXT("HTTP %d while loading manifestlist from '%s'"), HttpStatus, *HttpRequest->GetURL());

				FString msg = FString::Format(TEXT("HTTP {0} while loading manifestlist from {1}"), { HttpStatus, *HttpRequest->GetURL() });
				FGsUIHelper::PopupSystemMsg(FText::FromString(msg));

				PatchInstallFail = false;
				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::PATCH_FALSE, nullptr);
				return;
			}
		}
		else
		{
			UE_LOG(LogPatch, Log, TEXT("HTTP connection issue while loading manifestlist '%s'"), *HttpRequest->GetURL());

			FString msg = FString::Format(TEXT("HTTP connection issue while loading manifestlist {0}"), { *HttpRequest->GetURL() });
			FGsUIHelper::PopupSystemMsg(FText::FromString(msg));

			PatchInstallFail = false;
			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::PATCH_FALSE, nullptr);
			return;
		}
		});
	if (!TryLoadManifestFileHttpRequest->ProcessRequest())
	{
		// 시작을 못했으면
		UE_LOG(LogPatch, Log, TEXT("HttpRequest to Start Failed"));

		FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("HttpRequest to Start Failed")));

		PatchInstallFail = false;
		GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::PATCH_FALSE, nullptr);
	}
}

void UGsPatchManager::TryLoadManifestFileRepair(int TryNumber)
{
	// load the divergence from the root CDN
	TryLoadManifestFileHttpRequest = FHttpModule::Get().CreateRequest();
	TryLoadManifestFileHttpRequest->SetTimeout(20.0f);
	TryLoadManifestFileHttpRequest->SetURL(*RequestManifestNameURLList[0].RemoteManifestURL);
	TryLoadManifestFileHttpRequest->SetVerb(TEXT("GET"));
	TryLoadManifestFileHttpRequest->OnProcessRequestComplete().BindLambda([this, TryNumber](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess) {
		if (bSuccess && HttpResponse.IsValid())
		{
			const int32 HttpStatus = HttpResponse->GetResponseCode();
			if (EHttpResponseCodes::IsOk(HttpStatus))
			{
				UE_LOG(LogPatch, Log, TEXT("Succeeded to parsing manifestlist to '%s'"), *HttpResponse->GetContentAsString());

				// 요청할 CDN Cloud Manifest files이름에 URL경로를 합치기, 해당 배열이 재귀용법에 종로 트리거도 된다.
				RequestManifestNameURLList.Empty();
				RequestManifestNameURLList = RequestManifestNames;

				Repairing();
			}
			else
			{
				UE_LOG(LogPatch, Log, TEXT("HTTP %d while loading manifestlist from '%s'"), HttpStatus, *HttpRequest->GetURL());

				FString msg = FString::Format(TEXT("HTTP {0} while loading manifestlist from {1}"), { HttpStatus, *HttpRequest->GetURL() });
				FGsUIHelper::PopupSystemMsg(FText::FromString(msg));

				PatchInstallFail = false;
				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::REPAIR_FALSE, nullptr);
				return;
			}
		}
		else
		{
			UE_LOG(LogPatch, Log, TEXT("HTTP connection issue while loading manifestlist '%s'"), *HttpRequest->GetURL());

			FString msg = FString::Format(TEXT("HTTP connection issue while loading manifestlist {0}"), { *HttpRequest->GetURL() });
			FGsUIHelper::PopupSystemMsg(FText::FromString(msg));

			PatchInstallFail = false;
			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::REPAIR_FALSE, nullptr);
			return;
		}
	});
	if (!TryLoadManifestFileHttpRequest->ProcessRequest())
	{
		// 시작을 못했으면
		UE_LOG(LogPatch, Log, TEXT("HttpRequest to Start Failed"));
		FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("HttpRequest to Start Failed")));		

		PatchInstallFail = false;
		GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::REPAIR_FALSE, nullptr);
	}
}

void UGsPatchManager::StartPatch(int TryNumber)
{
	// fast path the first try
	if (TryNumber <= 0)
	{
		// load it
		TryLoadManifestFile(TryNumber);
		return;
	}

	// 한번에 성공했으면 여기까지 오지 않는다, 여기왔다는건 처음 시도가 실패해서 성공할때까지 지속적으로 요청할 것이라는 것
	// compute delay before re-starting load
	float SecondsToDelay = TryNumber * 5.0f;
	if (SecondsToDelay > 60)
		SecondsToDelay = 60;

	UE_LOG(LogPatch, Log, TEXT("Will re-attempt manifest load in %f seconds"), SecondsToDelay);
	FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this, TryNumber](float Unused) {
		if (this->IsA<UGsPatchManager>())
		{
			this->TryLoadManifestFile(TryNumber);
		}
		return false;
		}), SecondsToDelay);
	return;
}

void UGsPatchManager::StartRepair(int TryNumber)
{
	// fast path the first try
	if (TryNumber <= 0)
	{
		// load it
		TryLoadManifestFileRepair(TryNumber);
		return;
	}

	// 한번에 성공했으면 여기까지 오지 않는다, 여기왔다는건 처음 시도가 실패해서 성공할때까지 지속적으로 요청할 것이라는 것
	// compute delay before re-starting load
	float SecondsToDelay = TryNumber * 5.0f;
	if (SecondsToDelay > 60)
		SecondsToDelay = 60;

	UE_LOG(LogPatch, Log, TEXT("Will re-attempt manifest load in %f seconds"), SecondsToDelay);
	FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this, TryNumber](float Unused) {
		if (this->IsA<UGsPatchManager>())
		{
			this->TryLoadManifestFileRepair(TryNumber);
		}
		return false;
	}), SecondsToDelay);
	return;
}

IBuildPatchServicesModule* UGsPatchManager::GetBuildPatchServices()
{
	static IBuildPatchServicesModule* BuildPatchServices = nullptr;
	if (BuildPatchServices == nullptr)
	{
		BuildPatchServices = &FModuleManager::LoadModuleChecked<IBuildPatchServicesModule>(TEXT("BuildPatchServices"));
	}
	return BuildPatchServices;
}

FRequestCheckDownloadDelegate& UGsPatchManager::OnRequestCheckDownloadComplete()
{
	UE_LOG(LogPatch, Log, TEXT("UGsPatchManager::OnRequestCheckDownloadComplete()"));
	return RequestCheckDownloadDelegate;
}

FRequestPatchProcessDelegate& UGsPatchManager::OnRequestPatchProcessComplate()
{
	UE_LOG(LogPatch, Log, TEXT("UGsPatchManager::OnRequestPatchProcessComplate()"));
	return RequestPatchProcessDelegate;
}

bool UGsPatchManager::LocalUnmount()
{
	// FPakPlatformFile 은 editor일때는 null, eidtor에서  test용으로 생성하지만 그냥 두어도 상관없다.
	FPakPlatformFile* PakFileMgr = (FPakPlatformFile*)(FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile")));
	if (PakFileMgr == nullptr)
	{
		// 애초에 PakFileMgr가 널이면 mount도 안했다는 말 return 처리.
		return false;
	}

	TArray<FString> FindPakFiles;
	IFileManager::Get().FindFilesRecursive(FindPakFiles, *PatchInstallPath, TEXT("*.pak"), true, false);
	for (FString PakFileName : FindPakFiles)
	{
		if (PakFileMgr->Unmount(*PakFileName))
		{
			UE_LOG(LogPatch, Log, TEXT("Unmounted = %s"), *PakFileName);
		}
		else
		{
			UE_LOG(LogPatch, Log, TEXT("Failed to main unmount pak = %s"), *PakFileName);
		}
	}

	FindPakFiles.Empty();
	IFileManager::Get().FindFilesRecursive(FindPakFiles, *DLCInstallPath, TEXT("*.pak"), true, false);
	for (FString PakFileName : FindPakFiles)
	{
		if (PakFileMgr->Unmount(*PakFileName))
		{
			UE_LOG(LogPatch, Log, TEXT("Unmounted = %s"), *PakFileName);
		}
		else
		{
			UE_LOG(LogPatch, Log, TEXT("Failed to main unmount pak = %s"), *PakFileName);
		}
	}

	return true;
}

void UGsPatchManager::DeletePatchStagingDataDirectory()
{
	if (true == IFileManager::Get().DirectoryExists(*PatchStagingPath))
	{
		IFileManager::Get().DeleteDirectory(*PatchStagingPath, true, true);
	}	
}

void UGsPatchManager::DeletePatchDataDirectory()
{
	if (true == IFileManager::Get().DirectoryExists(*PatchInstallPath))
	{
		IFileManager::Get().DeleteDirectory(*PatchInstallPath, true, true);
	}

	if (true == IFileManager::Get().DirectoryExists(*DLCInstallPath))
	{
		IFileManager::Get().DeleteDirectory(*DLCInstallPath, true, true);
	}
}

FString UGsPatchManager::GetCloudManifestListFileName()
{
	FString ManifestListFileName;
	FString FlatformName = GDivergence()->GetPlatform();	
	if (FlatformName.Compare(TEXT("Android")) == 0)  ManifestListFileName = TEXT("android_astc.manifestlist");
	else if (FlatformName.Compare(TEXT("IOS")) == 0) ManifestListFileName = TEXT("ios.manifestlist");
	else if (FlatformName.Compare(TEXT("Windows")) == 0) ManifestListFileName = TEXT("windowsnoeditor.manifestlist");

	return ManifestListFileName;
}

FString UGsPatchManager::GetCloudManifestListFolder()
{
	FString ManifestListFolder;
	FString FlatformName = GDivergence()->GetPlatform();	
	if (FlatformName.Compare(TEXT("Android")) == 0)  ManifestListFolder = TEXT("Android_ASTC");
	else if (FlatformName.Compare(TEXT("IOS")) == 0) ManifestListFolder = TEXT("IOS");
	else if (FlatformName.Compare(TEXT("Windows")) == 0) ManifestListFolder = TEXT("WindowsNoEditor");

	return ManifestListFolder;
}

#undef LOCTEXT_NAMESPACE