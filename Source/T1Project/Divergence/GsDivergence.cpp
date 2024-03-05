#include "GsDivergence.h"

#include "Management/ScopeGlobal/GsPatchManager.h"
#include "GsHttpCall.h"
#include "GsDivergenceFunc.h"

#include "Management/ScopeGlobal/GsPhoneInfoManager.h"

#include "T1Project.h"

#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Misc/FileHelper.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include "Management/GsMessageHolder.h"

#include "MOSPhoneInfoBase.h"

#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/GsMessageStage.h"

#define LOG_DIVERGENCE


FGsDivergence::~FGsDivergence()
{
	if (_httpCall != nullptr)
	{
		delete _httpCall;
		_httpCall = nullptr;
	}
}
// 초기화
void FGsDivergence::Initialize()
{
#ifdef LOG_DIVERGENCE
	GSLOG(Log, TEXT("[FGsDivergence] Initialize begin"));
#endif

	/*ProjectDir: FApp::GetProjectName()을 보고 현재 프로젝트의 기본 디렉토리를 반환합니다.
	static FString FPaths::ProjectDir()
	결과 : .. / .. / .. / .. / .. / .. / .. / .. / Users / (사용자명) / Desktop / General / Project / Prototype / (언리얼 프로젝트명) /
	위 경로는 제 언리얼 프로젝트가 설치되어 있는 경로를 의미합니다.절대 경로는 따로 생략하도록 하겠습니다.
	https://minusi.tistory.com/entry/%EC%96%B8%EB%A6%AC%EC%96%BC-%EA%B2%BD%EB%A1%9C-%ED%95%A8%EC%88%98Unreal-FPaths
	*/
	// cdl url 정보 저장
	_strSaveData = FPaths::ProjectDir() + "info/server.config";
	
	_httpCall = new FGsHttpCall();

	_httpCall->_funcResultContents = [this](const FString & In_text)
	{
		OnHTTPCallResult(In_text);
	};
	// 로드 시작
	StartLoadDivergence();
}

// 다이버전스 로드 시작
void FGsDivergence::StartLoadDivergence()
{
	StartDivergence();
}

// 분기 시작
void FGsDivergence::StartDivergence()
{
#ifdef LOG_DIVERGENCE
	GSLOG(Log, TEXT("[FGsDivergence] StartDivergence begin"));
#endif
	// 이미 초기화 되었으면 할필요 없음
	//if (true == GDivergenceVersion()->IsInit())
	{
		FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_HIVE_DIVERGENCE);
		GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);

		return;
	}

	// 최초 실행 시 1번만 서버 분기 실행
	if (IsFirstStartElseLoadCdnUrl())
	{
		// cdn 주소 php 호출로 얻어오기
		ExecuteMccCheck();
	}
	else
	{
		// cdn 주소 파일에서 얻어오기(전에 저장해놓은것)
		StartGVSProcess();
	}
}
// 처음인가?
bool FGsDivergence::IsFirstStartElseLoadCdnUrl()
{
#ifdef LOG_DIVERGENCE
	GSLOG(Log, TEXT("[FGsDivergence] IsFirstStartElseLoadCdnUrl begin"));
#endif
	// info 폴더 경로
	FString infoFolder = FPaths::ProjectDir() + _strInfoFolder;

	IPlatformFile& platformFile =
		FPlatformFileManager::Get().GetPlatformFile();

	// directory 없으면 
	if (false == platformFile.DirectoryExists(*infoFolder))
	{
#ifdef LOG_DIVERGENCE
		GSLOG(Log, TEXT("[FGsDivergence] DirectoryExists(%s) == false"), *infoFolder);
#endif
		return true;
	}

	// 파일이 없으면
	if (false == platformFile.FileExists(*_strSaveData))
	{
#ifdef LOG_DIVERGENCE
		GSLOG(Log, TEXT("[FGsDivergence] FileExists(%s) == false"), *_strSaveData);
#endif
		return true;
	}

	LoadCdnURL();

	FString versionText = "";// FGsDivergenceFunc::GetVersion();

	if (true == _strConfigureVersion.IsEmpty() || 
		_strConfigureVersion.Equals(versionText) == false)
	{
		return true;
	}
	return false;
}

// cdn url 로드
void FGsDivergence::LoadCdnURL()
{
#ifdef LOG_DIVERGENCE
	GSLOG(Log, TEXT("[FGsDivergence] LoadCdnURL begin"));
#endif
	FString fileData = "";
	FFileHelper::LoadFileToString(fileData, *_strSaveData);

	// size가 0이 아니면
	if (false == fileData.IsEmpty())
	{
		TArray<FString> list;
		fileData.ParseIntoArray(list, TEXT(":"), true);

		if (list.Num() >= 2)
		{
			// 이전 저장된 정보때문에 분기
			if (list.Num() == 2)
			{
				_strCdnURL = list[0];
				_strRegion = list[1];
			}
			else
			{
				_strCdnURL = list[0];
				_strRegion = list[1];
				_strCloudURL = list[2];
				_strConfigureVersion = "";

				if (list.Num() >= 4)
				{
					_strConfigureVersion = list[3];
				}

				// RB에서는 서비스 지역인지 체크 있음(중국이면 안함 이런거)
			}
		}
	}
#ifdef LOG_DIVERGENCE
	FString config = FString::Format(TEXT("{0}: {1}"), { _strCloudURL, _strRegion });
	GSLOG(Log, TEXT("[LoadConfig] %s: %s"), *_strSaveData, *config);
#endif
}

void FGsDivergence::ExecuteMccCheck()
{
//#ifdef LOG_DIVERGENCE
//	GSLOG(Log, TEXT("[FGsDivergence] ExecuteMccCheck begin"));
//#endif
//	FMOSPhoneInfoBase* phoneInfo = GPhoneInfo()->GetPhoneInfo();
//	if (nullptr == phoneInfo)
//	{
//		return;
//	}
//	
//	FString strMnc = phoneInfo->GetStrMNC();
//	FString strMcc = phoneInfo->GetStrMCC();
//
//#ifdef LOG_DIVERGENCE
//	GSLOG(Log, TEXT("[FGsDivergence] strMnc: %s, strMcc: %s"), *strMnc, *strMcc);
//#endif
//
//#if WITH_EDITOR
//	// PC는 -1이라서 임의의 값 입력
//	strMnc = "05";
//	strMcc = "202";
//#endif
//
//	FString strLocalName = FInternationalization::Get().GetCurrentLocale()->GetName();
//	FString sendUrl = FString::Printf(
//		//*_strUrlFormat, 
//		TEXT("%s?searchgid=%s&searchmcc=%s&searchmnc=%s&searchlge=%s"),
//		*_strTargetUrl, *_strWorldOfZenoniaGameID,
//		*strMnc, *strMcc, *strLocalName);
//
//#ifdef LOG_DIVERGENCE
//	GSLOG(Log, TEXT("[FGsDivergence] sendUrl: %s"), *sendUrl);
//#endif
//	_httpCall->StartHttpCall(sendUrl);
}

// http 호출 결과 
void FGsDivergence::OnHTTPCallResult(const FString& In_text)
{
#ifdef LOG_DIVERGENCE
	GSLOG(Log, TEXT("[FGsDivergence] OnHTTPCallResult In_text :%s"), *In_text);
#endif

	int resultVal = FCString::Atoi(*GetTagValue(In_text, _strTagResultVal));
	// 성공
	if (resultVal == 1)
	{
		_strRegion = GetTagValue(In_text, _strTagRegion);
		_strCdnURL = GetTagValue(In_text, _strTagCdnURL);
		_strCloudURL = GetTagValue(In_text, _strTagCloudURL);

		// info 폴더 경로
		FString infoFolder = FPaths::ProjectDir() + _strInfoFolder;

		IPlatformFile& platformFile =
			FPlatformFileManager::Get().GetPlatformFile();

		// directory 없으면 
		platformFile.CreateDirectory(*infoFolder);

		// 파일이 있다면
		if (true == platformFile.FileExists(*_strSaveData))
		{
			// 삭제
			platformFile.DeleteFile(*_strSaveData);
		}
		
		FString versionText = "";//FGsDivergenceFunc::GetVersion();

		FString saveText = FString::Printf(
			TEXT("%s:%s:%s:%s"), *_strCdnURL, *_strRegion, *_strCloudURL,
			*versionText);
		// 저장
		FFileHelper::SaveStringToFile(saveText, *_strSaveData);

#ifdef LOG_DIVERGENCE
		GSLOG(Log, TEXT("[SaveConfig] %s: %s"), *_strSaveData, *saveText );
#endif
	}
	// 실패
	else
	{
#ifdef LOG_DIVERGENCE
		GSLOG(Log, TEXT("[CDN_URL] Result Not 1 downloading: %s"), *In_text);
#endif
	}
	// gvs 처리 시작
	StartGVSProcess();
}
// gvs 처리
void FGsDivergence::StartGVSProcess()
{
	// 다운로드에 쓰임
	//GDivergenceVersion()->SetCdnUrl(_strCdnURL);

	// 기존 gvs 파일은 지워준다
	FString filePath = FPaths::ProjectDir() + _strVersionGvs;

	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();

	// 파일이 있다면
	if (true == platformFile.FileExists(*filePath))
	{
		// 삭제
		platformFile.DeleteFile(*filePath);
	}

	FString gvsPath = FString::Printf(TEXT("https://%s%s"), *_strCdnURL, *_strVersionGvs);
#ifdef LOG_DIVERGENCE
	GSLOG(Log, TEXT("[VERSION] GvsPath %s"), *gvsPath);
#endif

	// info 폴더 경로
	//FString infoFolder = FPaths::ProjectDir() + _strInfoFolder;
	//FString infoFolder = "../../../" + _strInfoFolder;
	//FString infoFolder = FPaths::ProjectDir();
	FString infoFolder = "T1Project/Config";
	//GDivergenceVersion()->LoadGVS(gvsPath, infoFolder);
}
// xml 형식의 태그로 값찾아오기
FString FGsDivergence::GetTagValue(const FString& In_contentsText, const FString& In_tag)
{
	// 시작태그 위치 찾기
	int startIndex = In_contentsText.Find(In_tag);
	if (startIndex == -1)
	{
		return "";
	}
	// 끝 태그값 앞이 본문 값 끝 인덱스
	int endIndex = In_contentsText.Find("</",  ESearchCase::Type::IgnoreCase,
		ESearchDir::FromStart, startIndex);
	// 시작 태그값 지나서 본문 값의 시작 지점
	startIndex = (startIndex + In_tag.Len()) + 1;
	// 본문 값 길이 구하기
	int findValueLength = endIndex - startIndex;
	return In_contentsText.Mid(startIndex, findValueLength);
}