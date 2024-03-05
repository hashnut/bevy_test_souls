#pragma once
#include "CoreMinimal.h"
#include "GsBannedWordInfo.h"
#include "Tickable.h"
#include "Online/HTTP/Public/Http.h"
#include "GsBannedWordProcess.generated.h"

// Web의 Post Response Byte Data -> Json 하기 위한 구조체들 USTRUCT
USTRUCT()
struct FBanndedWordChange
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString action;

	UPROPERTY()
	FString word;

	UPROPERTY()
	FString lang;

	UPROPERTY()
	FString country;

	UPROPERTY()
	FString usage;
};

USTRUCT()
struct FBannedWordBundleInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	uint32 revision;

	UPROPERTY()
	FString modDate;

	UPROPERTY()
	int resultType;

	UPROPERTY()
	FString url;

	UPROPERTY()
	uint32 fileSize;

	UPROPERTY()
	FString md5;

	UPROPERTY()
	TArray<FBanndedWordChange> change;
};

USTRUCT()
struct FBannedWordResultInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int resultCode;

	UPROPERTY()
	FString resultMessage;

	UPROPERTY()
	FBannedWordBundleInfo bundleInfo;
};

class IHttpRequest;
class FGsBannedWordFile;

/*
 * 금칙어 번들 파일을 처리하기 위한 제반 절차를 진행한다.
 * Tick 이 필요하여 FTickableGameObject를 상속 받음 
 * FTickableGameObject Tick 이용중, web 람다바인딩 사용하면 tick의 충돌에 의해 다른 this가 전달되는 경우가 있어, web response시 object binding 사용한다.
 * IsTemplate() == false 일때만 Tickerable 사용해도 역시 발생한다.
 * DiversenceManager가 처리되고 난 뒤에 처리한다. - live/test 구분
 *		1. local info 파일 또는 저장되어 있는 부분 읽기 - releaseDate, revision 정보
 *		2. url POST로 info 요청 - releaseDate, revision, md5sum, fileSize, changedInfo ->json
 *		3.  if (changed변화) load client table + changed 적용
 *			else if(file을 다운로드 필요) info의 url 사용하여 호출 임시 파일로 저장
 *				serverMd5 와 downfile 의 data로 checkSum check 후 일치하면 저장 load client table
 *			else 변화도 file Download 도 필요없음 load client table
 *		4. Load Table 절차
 *			1. 정해진 key값으로 description -{ 0x38, 0x33, 0x31, 0x38, 0x37, 0x34, 0x39, 0x30, 0x36, 0x36, 0x33, 0x38, 0x38, 0x34, 0x39, 0x30 };
 *			2. uncompress
 *			3. 테이블 로드
*/

UCLASS()
class T1PROJECT_API UGsBannedWordProcess  : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FBannedWordProcessCompleteDelegate, TArray<BannedWordFileRowData>&);

	struct FGsBannedWordBundleInfo
	{
		FString _serverUrl = TEXT("https://staging-api-gspus.qpyou.cn/gateway.php");		//info 요청
		FString _appId = TEXT("com.gamevil.worldofzenonia");
		FString _gameName = TEXT("WorldOfZenonia");
		FString _bundlePath = TEXT("BannedWord");		// bin file download
		FString _bundleInfoPath = TEXT("BannedWord");	// local info path

		FString _bundleURL;			// bin file url
		FString _releaseDate;
		FString _md5Hash;
		uint32  _revision{ 0 };
		uint32  _fileSize{ 0 };

		bool _isExistBundle{ false };

		TArray<BannedWordFileRowData> _changeList;
		void Reset()
		{
			//_serverUrl.Reset();
			//_appId.Reset();
			//_gameName.Reset();
			//_bundlePath.Reset();
			//_bundleInfoPath.Reset();

			//_bundleURL.Reset();
			//_releaseDate.Reset();
			//_md5Hash.Reset();
			_bundleURL = TEXT("");
			_releaseDate = TEXT("");
			_md5Hash = TEXT("");

			_changeList.Reset();
		}
	};

	enum class ProcessState : uint8 { None, Init, ReadInfo, GetInfo, ReqDownload, DownloadBundle, CheckBundle, FInished, Unknown = 255 };

protected:
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> _requestInfoHttp{ nullptr };
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> _requestDownloadHttp{ nullptr };

	FGsBannedWordBundleInfo _bundleInfo;
	TSharedPtr<FGsBannedWordFile> _bannedWordFile{ nullptr };

	FBannedWordProcessCompleteDelegate  ProcessCompleteDelegate;

private:
	ProcessState _curProcess{ ProcessState::None };

	ProcessState _errorProcess{ ProcessState::None };
	int _errorType = 0;

	bool _validProces{ false };

public:
	const FGsBannedWordBundleInfo& GetBunndleInfo() const { return _bundleInfo; }
	FBannedWordProcessCompleteDelegate& OnProcessComplete();

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickableInEditor() const override { return true; }

	void SetBannedWordProcInfo(FString banWordServerUrl, FString appId, FString gameName, FString bundlePath = TEXT("BannedWord"), FString bundleInfoPath = TEXT("BannedWord"));
	void ResetProcInfo();
	void StartProcess();
	void EndProcess();
	
	bool UpdateProcess(float DeltaTime, bool isReset = false);

	bool IsProcessError(OUT ProcessState& errorState);
	int	 GetErrorCode() { return _errorType; }

protected:
	void LoadBannedWordInfo(); //_curWordFileInfo
	void SaveBannedWordInfo();
	void SaveDownloadedBundleFile(const TArray<uint8>& downloadFileData, int32 size);

	void RequestBannedWordInfo(); //server로
	void RequestDownloadBannedWordFile(); //download

protected:
	void OnHttpResponseBundleInfo(FHttpRequestPtr httpRequest, FHttpResponsePtr httpResponse, bool isSuccess);
	void OnHttpResponseBundleDownload(FHttpRequestPtr httpRequest, FHttpResponsePtr httpResponse, bool isSuccess);

private:
	void SetState(ProcessState state) { _curProcess = state; }
	void SetErrorState(ProcessState state) { _errorProcess = state; }
	void OnChangedProcess(ProcessState state);

	FBannedWordBundleInfo ParseJsonStringToInfo(FString parseText);
};

