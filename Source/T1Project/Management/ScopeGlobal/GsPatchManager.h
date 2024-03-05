#pragma once

#include "CoreMinimal.h"
#include "Classes/GsSingleton.h"
#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "Interfaces/IBuildInstaller.h"
#include "Interfaces/IBuildManifest.h"
#include "Interfaces/IBuildPatchServicesModule.h"

#include "GsPatchManager.generated.h"

USTRUCT()
struct FGsPatchManifestFile
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString filename;

	UPROPERTY()
	FString uniqueFilename;

	UPROPERTY()
	FString length;

	UPROPERTY()
	FString URL;

	UPROPERTY()
	FString hash;

	UPROPERTY()
	FString hash256;
};

USTRUCT()
struct FGsManifestList
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString ClientVersion;

	UPROPERTY()
	FString PatchVersion;

	UPROPERTY()
	FString BuildUrl;

	UPROPERTY()
	TArray<FGsPatchManifestFile> files;
};

USTRUCT()
struct FPatchInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString DefaultPatchPath; // 패치 폴더가 Default로 배포된 프로그램이 설치된 곳인지(Windows만)
};


class IHttpRequest;

DECLARE_DELEGATE_ThreeParams(FRequestCheckDownloadDelegate, bool /*bCheckDownload*/, bool /*bMounted*/, float /*fDownloadSize*/);
DECLARE_DELEGATE_OneParam(FRequestPatchProcessDelegate, bool /*bWasSuccessful*/);

UCLASS()
class T1PROJECT_API UGsPatchManager :
	public UObject,
	public TGsSingleton<UGsPatchManager>,
	public IGsManager
{
	GENERATED_BODY()
public:
	enum class ECulture
	{
		ko,
		en,
	};

	class FGsRequestManifestInfo
	{
	public:
		FString RemoteManifestURL; // Cloud상에 업로드된 적용할 경로포함 Manifest 이름(실제 주소)
		FString LocalManifestToFileName; // 로컬에 저장,로드할때 사용할 이름(ex pakchunk1.manifest)
		FString LocalManifestToFilePath; // 로컬에 저장,로드할때 사용할 경로

		FGsRequestManifestInfo(FString& In_RemoteManifestURL
			, FString& In_LocalManifestToFileName
			, FString& In_LocalManifestToFilePath)
		{
			RemoteManifestURL = In_RemoteManifestURL;
			LocalManifestToFileName = In_LocalManifestToFileName;
			LocalManifestToFilePath = In_LocalManifestToFilePath;
		}
	};

public:
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update(float inTick) override;
	virtual ~UGsPatchManager();
	
	float GetDownloadSize();
	float GetRequiredDiskSpace();
	float GetTotalDownloadedSize();
	float GetTotalDownloadRequiredSize();
	float GetDownloadSpeed();
	float GetInstallProgress();
	bool GetDiskFreeSpace(OUT float& outDiskFreeSpaceMB);

	BuildPatchServices::EBuildPatchState GetState();
	FString GetStateString();
	EBuildPatchInstallError GetInstallFailError();
	EBuildPatchInstallError GetRepairFailError();
	bool IsInstallFail(); // 인스톨 설치 에러 인지 그외 에러인지 체크하기 위한 함수
	bool IsRepairFail(); // Repair 에러 인지 그외 에러인지 체크하기 위한 함수
	bool IsCheckPatchInfoFile(); // 한번만(제설치시 초기화) 패치 경로를 선택하는데 선택 했는지 체크 하는 함수
	void ChangePatchPath(bool DefaultPatchPath);
	bool PatchFileCleanUp();

	void CheckDownloadRequest(int InAssetChunkID = -1, int InVoiceChunkID = -1); // 다운로드가 필요한가?
	void PatchProcessRequest(); // CheckDownloadRequest() 함수를 호출해서 정보를 셋팅해야 유효한 함수 호출이된다.	
	void RepairProcessRequest(int InAssetChunkID = -1, int InVoiceChunkID = -1);

	void CancelInstall();

	void DeletePatchStagingDataDirectory();
	void DeletePatchDataDirectory();

	FRequestCheckDownloadDelegate& OnRequestCheckDownloadComplete();
	FRequestPatchProcessDelegate& OnRequestPatchProcessComplate();

private:
	void StartTotalDownloadSizeCalc();
	bool ParsingManifestList(const FString& ManifestString);	
	void MakeRequestManifestNames();
	void DownloadSizeCalc();

	IBuildManifestPtr GetInstalledManifest(const FString& FindLocalManifestFilePath, const FString& FindLocalManifestFileName);

	void StartPatch(int TryNumber = 0);
	void Patching();
	void InstallComplete(const IBuildInstallerRef& InstallerRef);
	
	void StartRepair(int TryNumber = 0);
	void Repairing();
	void TryLoadManifestFileRepair(int TryNumber);
	void RepairComplete(const IBuildInstallerRef& InstallerRef);

	IBuildPatchServicesModule* GetBuildPatchServices();
	void TryLoadManifestFile(int TryNumber);

	bool LocalMount();	
	bool LocalUnmount();

	FString GetCloudManifestListFileName();
	FString GetCloudManifestListFolder();

	bool LadoPatchInfoFile();
	void SavePatchInfoFile();
	bool WriteStringAsUtf8TextFile(const FString& FileText, const FString& FilePath);

private:
	FString PatchInstallPath;
	FString DLCInstallPath;
	FString PatchStagingPath;

	FString ManifestListFileURL;
	FString CloudDirURL;

	FGsManifestList ManifestList; // CDN Cloud에 업로드(적용)되어 있는 매니페스트 파일 리스트

	TArray<FString> FindLocalManifestList; // 로컬에 있는 매니페스트 파일 리스트

	TArray<FGsRequestManifestInfo> RequestManifestNames; // 패치 요청할 매니페스트 파일이름
	TArray<FGsRequestManifestInfo> RequestManifestNameURLList; // 패치 요청할 CDN URL 패스를 포함한 매니페스트 파일이름, 요청할때마다 셋팅해줘야된다.

	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> DownloadSizeCalcHttpRequest;
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> StartTotalDownloadSizeCalcHttpRequest;
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> PatchingHttpRequest;
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> TryLoadManifestFileHttpRequest;

	FString RemoteManifestURL; // User specified URL from where manifest can be downloaded

	float TotalDownloadSize;
	
	// Content installer, only valid during installation
	IBuildInstallerPtr Installer;

	// Currently installed manifest
	IBuildManifestPtr InstalledManifest;

	// Manifest downloaded from a cloud
	IBuildManifestPtr RemoteManifest;

	FRequestCheckDownloadDelegate RequestCheckDownloadDelegate;
	FRequestPatchProcessDelegate RequestPatchProcessDelegate;

	int AssetChunkID;
	int VoiceChunkID;

	FPatchInfo PatchInfo;

	EBuildPatchInstallError BuildPatchInstallError;
	bool PatchInstallFail;
	bool bDefaultPatchPath;

	EBuildPatchInstallError BuildPatchRepairError;
	bool PatchRepairFail;
};

using GsPatchManagerSingle = TGsSingleton<UGsPatchManager>;
#define GPatch() GsPatchManagerSingle::Instance()