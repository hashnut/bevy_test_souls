// Fill out your copyright notice in the Description page of Project Settings.


#include "GsDivergenceManager.h"

#include "Runtime/Online/HTTP/Public/Http.h"
#include "JsonObjectConverter.h"
#include "Management/GsMessageHolder.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Management/ScopeGlobal/GsPatchManager.h"

#if PLATFORM_ANDROID
	#include "Android/AndroidRuntimeSettings/Classes/AndroidRuntimeSettings.h"
#elif PLATFORM_IOS
	#include "IOS/IOSRuntimeSettings/Classes/IOSRuntimeSettings.h"
#elif WITH_EDITOR || PLATFORM_WINDOWS
	#include "EngineSettings/Classes/GeneralProjectSettings.h"
#endif
#include "../Message/GsMessageSystem.h"
#include "ConfigCacheIni.h"


#define LOCTEXT_NAMESPACE "GsDivergenceManager"
DEFINE_LOG_CATEGORY_STATIC(LogDivergence, Log, All);

static const FString SELETED_DIVERGENCE = TEXT("SeletedDivergence.gvs");
static const FString DIVERGENCE_INFO = TEXT("DivergenceInfo.info");

template<>
UGsDivergenceManager* GsDivergenceManagerSingle::_instance = nullptr;

void UGsDivergenceManager::Initialize()
{
	UGsDivergenceManager::InitInstance(this);
}

void UGsDivergenceManager::Finalize()
{
	UGsDivergenceManager::RemoveInstance();

	// cancel any pending divergence request
	if (DivergenceRequest.IsValid())
	{
		DivergenceRequest->CancelRequest();
		DivergenceRequest.Reset();
	}
}

void UGsDivergenceManager::TryLoadDivergenceFile(int TryNumber)
{
	// get base url from defaultgame.ini
	FString DivergenceCdnUrl;
	FString ConfigSectionName = FString::Printf(TEXT("/Script/DivergenceFileUrl"));
	GConfig->GetString(*ConfigSectionName, TEXT("CdnUrl"), DivergenceCdnUrl, GGameIni);
	UE_LOG(LogDivergence, Log, TEXT("Loading build divergence (attempt #%d) from %s"), TryNumber + 1, *DivergenceCdnUrl);

	DivergenceRequest = FHttpModule::Get().CreateRequest();
	DivergenceRequest->SetURL(*DivergenceCdnUrl);
	DivergenceRequest->SetVerb(TEXT("GET"));
	DivergenceRequest->OnProcessRequestComplete().BindLambda([this, TryNumber](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess) {
		if (bSuccess && HttpResponse.IsValid())
		{
			const int32 HttpStatus = HttpResponse->GetResponseCode();
			if (EHttpResponseCodes::IsOk(HttpStatus))
			{
				// Parsing
				if (ParsingAndSelectDivergence(HttpResponse->GetContentAsString()))
				{
					UE_LOG(LogDivergence, Log, TEXT("Succeeded to parsing and Select divergence to '%s'"), *HttpResponse->GetContentAsString());					

					GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_DIVERGENCE_TRUE, nullptr);

					return;
				}
				else
				{
					UE_LOG(LogDivergence, Log, TEXT("Failed to parsing and select divergence to '%s'"), *HttpResponse->GetContentAsString());
					GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_DIVERGENCE_FALSE, nullptr);
				}				
			}
			else
			{
				UE_LOG(LogDivergence, Log, TEXT("HTTP %d while loading divergence from '%s'"), HttpStatus, *HttpRequest->GetURL());
				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_DIVERGENCE_FALSE, nullptr);
			}
		}
		else
		{
			UE_LOG(LogDivergence, Log, TEXT("HTTP connection issue while loading divergence '%s'"), *HttpRequest->GetURL());
			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_DIVERGENCE_FALSE, nullptr);
		}

		// try to load it				
		if (!this->IsA<UGsDivergenceManager>())
		{
			UE_LOG(LogDivergence, Log, TEXT("UGsDivergenceManager was destroyed while loading divergence '%s'"), *HttpRequest->GetURL());
			return;
		}
		this->DivergenceRequest.Reset();
		});
	DivergenceRequest->ProcessRequest();
}

FString UGsDivergenceManager::GetProjectVersion()
{
	FString ProjectVersionText;
#if PLATFORM_ANDROID
	const UAndroidRuntimeSettings* Settings = GetDefault<UAndroidRuntimeSettings>();
	if (Settings)
	{
		ProjectVersionText = Settings->VersionDisplayName;
	}
#elif PLATFORM_IOS
	const UIOSRuntimeSettings* Settings = GetDefault<UIOSRuntimeSettings>();
	if (Settings)
	{
		ProjectVersionText = Settings->VersionInfo;
	}
#elif WITH_EDITOR || PLATFORM_WINDOWS
	const UGeneralProjectSettings* Settings = GetDefault<UGeneralProjectSettings>();
	if (Settings)
	{
		ProjectVersionText = Settings->ProjectVersion;
	}
#endif
	return ProjectVersionText;
}

bool UGsDivergenceManager::WriteStringAsUtf8TextFile(const FString& FileText, const FString& FilePath)
{
	// convert to UTF8
	FTCHARToUTF8 DivergenceFileUtf8(*FileText);

	// open the file for writing
	bool bSuccess = false;
	IFileHandle* DivergenceFile = IPlatformFile::GetPlatformPhysical().OpenWrite(*FilePath);
	if (DivergenceFile != nullptr)
	{
		// write to the file
		if (DivergenceFile->Write(reinterpret_cast<const uint8*>(DivergenceFileUtf8.Get()), DivergenceFileUtf8.Length()))
		{
			UE_LOG(LogDivergence, Log, TEXT("Wrote to %s"), *FilePath);
			bSuccess = true;
		}
		else
		{
			UE_LOG(LogDivergence, Log, TEXT("Write error writing to %s"), *FilePath);
		}

		// close the file
		delete DivergenceFile;
	}
	else
	{
		UE_LOG(LogDivergence, Log, TEXT("Unable open %s for writing."), *FilePath);
	}
	return bSuccess;
}

bool UGsDivergenceManager::IsResetData()
{ 
	return (SelectedDivergence.ResetData.Compare(TEXT("true")) == 0) ? true : false;
}

bool UGsDivergenceManager::PatchingProcessOn()
{
	return (SelectedDivergence.Patching.Compare(TEXT("on")) == 0) ? true : false;
}

bool UGsDivergenceManager::IsFindDivergenceInfoFile()
{
	UE_LOG(LogDivergence, Log, TEXT("call IsFindDivergenceInfoFile()"));

	TArray<FString> FindFile;
	FString InformationFolder = FPaths::ProjectPersistentDownloadDir() / TEXT("../Information/");

	UE_LOG(LogDivergence, Log, TEXT("Find PatchManifestFiles Folder FPaths::ProjectPersistentDownloadDir() / ../Information/ to '%s'"), *InformationFolder);
	IFileManager::Get().FindFiles(FindFile, *InformationFolder, TEXT("*.info"));

	if (FindFile.Num() > 0)
	{
		return true;
	}

	return false;
}

void UGsDivergenceManager::SaveDivergenceInfoFile()
{
	UE_LOG(LogDivergence, Log, TEXT("call SaveDivergenceInfoFile()"));

	FString SaveDir = FPaths::ProjectPersistentDownloadDir() / TEXT("../Information/");
	IFileManager& FileManager = IFileManager::Get();
	if (!FileManager.MakeDirectory(*SaveDir, true))
	{
		UE_LOG(LogDivergence, Log, TEXT("Unable to create cache folder at '%s'"), *SaveDir);
	}

	FString FilePath = SaveDir / DIVERGENCE_INFO;

	DivergenceInfo.ResetDataVersion = SelectedDivergence.PatchVersion;

	FString DivergenceInfoJson;
	if (FJsonObjectConverter::UStructToJsonObjectString(DivergenceInfo, DivergenceInfoJson))
	{
		if (!WriteStringAsUtf8TextFile(DivergenceInfoJson, FilePath))
		{
			UE_LOG(LogDivergence, Log, TEXT("SeletedDivergence file Saved report to %s"), *FilePath);
		}
	}
}

bool UGsDivergenceManager::ParsingAndLoadDivergenceInfoFile()
{
	UE_LOG(LogDivergence, Log, TEXT("call ParsingAndLoadDivergenceInfoFile()"));

	TArray<FString> FindFile;
	FString InformationFolder = FPaths::ProjectPersistentDownloadDir() / TEXT("../Information/");

	UE_LOG(LogDivergence, Log, TEXT("Find PatchManifestFiles Folder FPaths::ProjectPersistentDownloadDir() / ../Information/ to '%s'"), *InformationFolder);
	IFileManager::Get().FindFiles(FindFile, *InformationFolder, TEXT("*.info"));

	if (FindFile.Num() > 0)
	{
		FString FullPath = InformationFolder / TEXT("DivergenceInfo.info");
		IFileHandle* InfomationFile = IPlatformFile::GetPlatformPhysical().OpenRead(*FullPath);
		if (InfomationFile != nullptr)
		{
			int64 FileSize = InfomationFile->Size();
			char* FileBuffer = new char[FileSize + 8]; // little extra since we're forcing null term in places outside bounds of a field
			if (InfomationFile->Read((uint8*)FileBuffer, FileSize))
			{
				FileBuffer[FileSize] = '\0';
				FString Temp(FileBuffer);
				if (FJsonObjectConverter::JsonObjectStringToUStruct(Temp, &DivergenceInfo, 0, 0))
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool UGsDivergenceManager::ParsingAndSelectDivergence(const FString& DivergenceString)
{
	TArray<FDivergence> DivergenceList;
	if (FJsonObjectConverter::JsonArrayStringToUStruct(DivergenceString, &DivergenceList, 0, 0))
	{
		/*
		* 1. Platform 찾기
		* 2. APK(IPA) Version == ClientVersion 찾기
		* 3. 찾은 정보를 해당 기기에 저장하기
		*/

		FString DevicePlatformName = UGameplayStatics::GetPlatformName();
		FString ProjectVersion = GetProjectVersion();
		UE_LOG(LogDivergence, Log, TEXT("DevicePlatformName : %s, ProjectVersion : %s"), *DevicePlatformName, *ProjectVersion);

		FDivergence* FindDivergence = nullptr;
		for (FDivergence Divergence : DivergenceList)
		{
			if (Divergence.Platform == DevicePlatformName && Divergence.ClientVersion == ProjectVersion)
			{
				SelectedDivergence = Divergence;
				ClientVersionFloat = FCString::Atof(*(SelectedDivergence.ClientVersion.TrimQuotes()));

#if UE_BUILD_SHIPPING
				// rest data
				if (IsResetData() == true)
				{
					UE_LOG(LogDivergence, Log, TEXT("IsResetData() == true"));

					GPatch()->DeletePatchDataDirectory();
					//SaveDivergenceInfoFile();
				}				
#else
				FString SelectedDivergenceJson;
				if (FJsonObjectConverter::UStructToJsonObjectString(SelectedDivergence, SelectedDivergenceJson))
				{
					FString SaveDir = FPaths::ProjectPersistentDownloadDir() / TEXT("../Information/");
					IFileManager& FileManager = IFileManager::Get();
					if (!FileManager.MakeDirectory(*SaveDir, true))
					{
						UE_LOG(LogDivergence, Log, TEXT("Unable to create cache folder at '%s'"), *SaveDir);
					}

					FString FilePath = SaveDir / SELETED_DIVERGENCE;

					if (!WriteStringAsUtf8TextFile(SelectedDivergenceJson, FilePath))
					{
						UE_LOG(LogDivergence, Log, TEXT("SeletedDivergence file Saved report to %s"), *FilePath);
					}

					// rest data
					if (IsResetData() == true)
					{
						UE_LOG(LogDivergence, Log, TEXT("IsResetData() == true"));

						GPatch()->DeletePatchDataDirectory();
						SaveDivergenceInfoFile();
					}
				}
				else
				{
					UE_LOG(LogDivergence, Log, TEXT("Failed to UStruct To Json Object String"));
				}
#endif
				return true;
			}
		}
	}

	return false;
}

const bool UGsDivergenceManager::IsUseIAP()
{
#if WITH_EDITOR
	return true;
#else
	return (SelectedDivergence.UseIAP.Compare(TEXT("true")) == 0) ? true : false;
#endif
}

#undef LOCTEXT_NAMESPACE

