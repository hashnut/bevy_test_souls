#include "GsBannedWordProcess.h"
#include "FileManager.h"
#include "JsonObjectWrapper.h"
#include "JsonObjectConverter.h"
#include "GsBannedWordFile.h"
#include "JsonUtilities/Public/JsonDomBuilder.h"
#include "T1Project.h"

#define LOCTEXT_NAMESPACE "GsBannedWordProcesse"
DEFINE_LOG_CATEGORY_STATIC(LogBannedWord, Log, All);

void UGsBannedWordProcess::Tick(float DeltaTime)
{
	if (UpdateProcess(DeltaTime))
	{
		EndProcess();
	}
}

bool UGsBannedWordProcess::IsTickable() const
{
	return IsTemplate(RF_ArchetypeObject) == false; //IsValid(this) && this->_validProces; //
}

TStatId UGsBannedWordProcess::GetStatId() const
{
	return TStatId();
}

UGsBannedWordProcess::FBannedWordProcessCompleteDelegate& UGsBannedWordProcess::OnProcessComplete()
{
	UE_LOG(LogBannedWord, VeryVerbose, TEXT("UGsBannedWordProcess:OnProcessComplete()"));
	return ProcessCompleteDelegate;
}

void UGsBannedWordProcess::SetBannedWordProcInfo(FString banWordServerUrl, FString appId, FString gameName, FString bundlePath /*= TEXT("BannedWord")*/, FString bundleInfoPath /*= TEXT("BannedWord")*/)
{
	FString downloadPath = FPaths::ProjectPersistentDownloadDir();

	SetState(ProcessState::Init);
	SetErrorState(ProcessState::Init);

	_errorType = 0;

	_bundleInfo.Reset();
	_bundleInfo._serverUrl = banWordServerUrl;
	_bundleInfo._appId = appId;
	_bundleInfo._gameName = gameName;
	_bundleInfo._bundlePath = downloadPath / bundlePath;
	_bundleInfo._bundleInfoPath = downloadPath / bundleInfoPath;
}

void UGsBannedWordProcess::ResetProcInfo()
{
	if (_requestInfoHttp)
		_requestInfoHttp = nullptr;

	if (_requestDownloadHttp)
		_requestDownloadHttp = nullptr;

	if (_bannedWordFile)
		_bannedWordFile = nullptr;

	SetState(ProcessState::None);
	UpdateProcess(0.f, true);
}

void UGsBannedWordProcess::StartProcess()
{
	if (_curProcess != ProcessState::Init)
		return;

	_validProces = true;
	UpdateProcess(0.f);
	LoadBannedWordInfo();
}

void UGsBannedWordProcess::EndProcess()
{
	if(_requestInfoHttp)
		_requestInfoHttp = nullptr;

	if (_requestDownloadHttp)
		_requestDownloadHttp = nullptr;

	if(_bannedWordFile)
		_bannedWordFile = nullptr;

	ProcessCompleteDelegate.ExecuteIfBound(_bundleInfo._changeList);

	SetState(ProcessState::Init);
}

bool UGsBannedWordProcess::UpdateProcess(float DeltaTime, bool isReset)
{
	static ProcessState changeState = ProcessState::None;
	if (isReset)
	{
		changeState = ProcessState::None;
		return false;
	}

	if (changeState != _curProcess)
	{
		changeState = _curProcess;
		OnChangedProcess(_curProcess);

		return changeState == ProcessState::FInished;
	}
	return false;
}

void UGsBannedWordProcess::OnChangedProcess(ProcessState state)
{
	if (state == ProcessState::ReadInfo)
	{
		RequestBannedWordInfo();
	}
	else if (state == ProcessState::ReqDownload)
	{
		RequestDownloadBannedWordFile();
	}
}

void UGsBannedWordProcess::LoadBannedWordInfo()
{
	GSLOG(Log, TEXT("== LoadBannedWordInfo =="));

	auto& manager = IFileManager::Get();
	if (!manager.DirectoryExists(*(_bundleInfo._bundleInfoPath)))
	{
		manager.MakeDirectory(*(_bundleInfo._bundleInfoPath), true);
	}

	FString jsonFormText;
	FString filePath = _bundleInfo._bundleInfoPath / BANNEDWORD_INFOFILE;
	FString bundlePath = _bundleInfo._bundlePath / BANNEDWORD_BUNDDLEFILE;
	if (manager.FileExists(*filePath))
	{
		if (FArchive* arReader = manager.CreateFileReader(*filePath))
		{
			*arReader << jsonFormText;
			arReader->Close();
		}
	}
	else
	{
		GSLOG(Log, TEXT("== LoadBannedWordInfo : file %s is not exist. bundle %s =="), *filePath, *bundlePath);
	}

	FBannedWordBundleInfo info = ParseJsonStringToInfo(jsonFormText);
	_bundleInfo._releaseDate = info.modDate;
	_bundleInfo._md5Hash = info.md5;
	_bundleInfo._bundleURL = info.url;
	_bundleInfo._revision = info.revision;
	_bundleInfo._isExistBundle = manager.FileExists(*bundlePath);

	SetState(ProcessState::ReadInfo);
	SetErrorState(ProcessState::ReadInfo);
}

void UGsBannedWordProcess::SaveBannedWordInfo()
{
	GSLOG(Log, TEXT("== SaveBannedWordInfo =="));
	auto& manager = IFileManager::Get();
	if (!manager.DirectoryExists(*(_bundleInfo._bundleInfoPath)))
	{
		manager.MakeDirectory(*(_bundleInfo._bundleInfoPath), true);
	}

	FBannedWordBundleInfo info = FBannedWordBundleInfo{ _bundleInfo._revision, _bundleInfo._releaseDate, 0, TEXT(""), 0, _bundleInfo._md5Hash};
	
	bool saveReturn = false;
	FString jsonText;
	FString filePath = _bundleInfo._bundleInfoPath / BANNEDWORD_INFOFILE;
	if (FJsonObjectConverter::UStructToJsonObjectString(info, jsonText))
	{
		if (FArchive* arWriter = manager.CreateFileWriter(*filePath, FILEWRITE_None))
		{
			*arWriter << jsonText;
			saveReturn = arWriter->Close();
		}
	}

	GSLOG(Log, TEXT("== SaveBannedWordInfo : ArchieveClose[%d] file:%s %s"), saveReturn, *filePath, *jsonText);
}

void UGsBannedWordProcess::SaveDownloadedBundleFile(const TArray<uint8>& downloadFileData, int32 size)
{
	SetState(ProcessState::CheckBundle);

	_bannedWordFile = MakeShared<FGsBannedWordFile>();
	_bannedWordFile->OpenBannedWordFileWithData(_bundleInfo._bundlePath / BANNEDWORD_BUNDDLEFILE, downloadFileData, size);

	if (_bannedWordFile->IsSameMd5CheckSum(_bundleInfo._md5Hash))
	{
		_bannedWordFile->SaveBannedWordFile(true);
	}

	_bannedWordFile->CloseBannedWordFile();
	_bannedWordFile = nullptr;

	_bundleInfo._isExistBundle = true;

	_errorType = 0;
}

void UGsBannedWordProcess::RequestBannedWordInfo()
{
	_requestInfoHttp = FHttpModule::Get().CreateRequest();

	FJsonDomBuilder::FObject jsonObject;
	FString req = TEXT("RequestWordfilterGetBundleInfo");
	jsonObject.Set(TEXT("type"), req);
	jsonObject.Set(TEXT("appId"), _bundleInfo._appId);
	jsonObject.Set(TEXT("game"), _bundleInfo._gameName);
	jsonObject.Set(TEXT("revision"), _bundleInfo._revision);

	auto strJson = jsonObject.ToString();

	_requestInfoHttp->SetURL(_bundleInfo._serverUrl);
	_requestInfoHttp->SetVerb(TEXT("POST"));
	_requestInfoHttp->SetContentAsString(strJson);
	_requestInfoHttp->AppendToHeader(TEXT("Content-Type"), TEXT("application/json"));

	_requestInfoHttp->OnProcessRequestComplete().BindUObject(this, &UGsBannedWordProcess::OnHttpResponseBundleInfo);
	_requestInfoHttp->ProcessRequest();

	GSLOG(Log, TEXT("== RequestBannedWordInfo : RequestWordfilterGetBundleInfo, %s"), *strJson);

	SetState(ProcessState::GetInfo);
	SetErrorState(ProcessState::GetInfo);
}

void UGsBannedWordProcess::RequestDownloadBannedWordFile()
{
	_requestDownloadHttp = FHttpModule::Get().CreateRequest();
	_requestDownloadHttp->SetURL(_bundleInfo._bundleURL);
	_requestDownloadHttp->OnProcessRequestComplete().BindUObject(this, &UGsBannedWordProcess::OnHttpResponseBundleDownload);
	_requestDownloadHttp->ProcessRequest();

	SetState(ProcessState::DownloadBundle);
	SetErrorState(ProcessState::DownloadBundle);
}

bool UGsBannedWordProcess::IsProcessError(OUT ProcessState& errorState)
{
	if (_errorType == -1)
	{
		errorState = ProcessState::Unknown;
	}
	else
	{
		errorState = _errorProcess;
	}
	return (_errorType != 0 || _bundleInfo._isExistBundle == false);
}

void UGsBannedWordProcess::OnHttpResponseBundleInfo(FHttpRequestPtr httpRequest, FHttpResponsePtr httpResponse, bool isSuccess)
{
	GSLOG(Log, TEXT("== OnHttpResponseBundleInfo : ResponseWordfilterGetBundleInfo, isSuccess[%d]"), isSuccess);

	bool download = false;
	if (isSuccess && httpResponse.IsValid() && EHttpResponseCodes::IsOk(httpResponse->GetResponseCode()))
	{
		auto responseString = httpResponse->GetContentAsString();

		FBannedWordResultInfo result;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(responseString, &result, 0, 0) && result.resultCode == 100)
		{
			_bundleInfo._revision = result.bundleInfo.revision;
			_bundleInfo._releaseDate = result.bundleInfo.modDate;
			_bundleInfo._md5Hash = result.bundleInfo.md5;
			_bundleInfo._bundleURL = result.bundleInfo.url;
			_bundleInfo._fileSize = result.bundleInfo.fileSize;
			_bundleInfo._changeList.Reset();

			SaveBannedWordInfo();

			if (result.bundleInfo.resultType == 0) //FJsonObjectConverter::JsonArrayStringToUStruct(result.bundleInfo.change, &outList, 0, 0)) //changedÀû¿ë
			{
				using ACT = BannedWordFileRowData::ACT;
				for (auto e : result.bundleInfo.change)
				{
					_bundleInfo._changeList.Add(BannedWordFileRowData{ e.word, e.lang, e.country, e.usage, e.action == TEXT("on") ? ACT::On : ACT::Off });
				}
			}

			download = (result.bundleInfo.resultType != 0 || !_bundleInfo._isExistBundle);
		}
	}
	else
	{
		_errorType = httpResponse.IsValid() ? httpResponse->GetResponseCode() : -1;
	}
	_requestInfoHttp = nullptr;
	SetState(download? ProcessState::ReqDownload : ProcessState::FInished);
}

void UGsBannedWordProcess::OnHttpResponseBundleDownload(FHttpRequestPtr httpRequest, FHttpResponsePtr httpResponse, bool isSuccess)
{
	if (isSuccess && httpResponse.IsValid() && EHttpResponseCodes::IsOk(httpResponse->GetResponseCode()))
	{
		SaveDownloadedBundleFile(httpResponse->GetContent(), httpResponse->GetContentLength());
	}
	else
	{
		_errorType = httpResponse.IsValid() ? httpResponse->GetResponseCode() : -1;
	}

	_requestDownloadHttp = nullptr;
	SetState(ProcessState::FInished);
	SetErrorState(ProcessState::FInished);
}

FBannedWordBundleInfo UGsBannedWordProcess::ParseJsonStringToInfo(FString parseText)
{
	if (!parseText.IsEmpty())
	{
		FBannedWordBundleInfo info;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(parseText, &info, 0, 0))
		{
			return info;
		}
	}
	
	return FBannedWordBundleInfo{0, TEXT(""), 0, TEXT(""), 0, TEXT("")};
}

#undef LOCTEXT_NAMESPACE