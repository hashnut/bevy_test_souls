#pragma once

#include "CoreMinimal.h"
/*
서버 분기(로열블러드 참조)
*/

class FGsDivergenceVersion;
class FGsHttpCall;

class FGsDivergence
{	
private:
	// cdn url 저장할 파일 경로
	FString _strSaveData;
	// 정보 얻어올 url 
	const FString _strTargetUrl;
	// url 포맷
	const FString _strUrlFormat;
	// http 요청용 월드 오브 제노니아 게임 id
	const FString _strWorldOfZenoniaGameID;
	// 태그
private:
	// 결과 확인 태그
	const FString _strTagResultVal;
	// 클라우드 url 태그
	const FString _strTagCloudURL;
	// cdn url 태그
	const FString _strTagCdnURL;
	// 지역 태그
	const FString _strTagRegion;

private:
	// 실제 버전 정보를 담고 있는 파일 경로(CDN + 파일 이름)
	const FString _strVersionGvs;
	// info 폴더
	const FString _strInfoFolder;

	// 결과
private:
	// cdn의 url
	FString _strCdnURL;
	// 지역
	FString _strRegion;
	// cloud url
	FString _strCloudURL;
	// 버전
	FString _strConfigureVersion;
	// http 호출
	FGsHttpCall* _httpCall = nullptr;

	// 생성자
public:
	FGsDivergence() :
		_strTargetUrl("https://cds.gamevil.com/mcc/mcc_check_t3.php"),
		_strUrlFormat("%s?searchgid=%s&searchmcc=%s&searchmnc=%s&searchlge=%s"),
		_strWorldOfZenoniaGameID("15031"),
		_strTagResultVal("result_value"),
		_strTagCloudURL("cloud_url"),
		_strTagCdnURL("cdn_url"),
		_strTagRegion("region"),
		_strVersionGvs("info/version_WZ.gvs"),
		_strInfoFolder("info")
	{}
	virtual ~FGsDivergence();

	// 이벤트
public:
	// http 호출 결과 
	void OnHTTPCallResult(const FString& In_text);

	// 로직 함수
public:
	// 초기화
	void Initialize();
	// 분기 시작
	void StartDivergence();
	// 다이버전스 로드 시작
	void StartLoadDivergence();
	// 처음인가? 
	bool IsFirstStartElseLoadCdnUrl();
	// cdn url 로드
	void LoadCdnURL();
	// mcc 체크 실행
	void ExecuteMccCheck();
	// gvs 처리
	void StartGVSProcess();
	// xml 형식의 태그로 값찾아오기
	FString GetTagValue(const FString& In_contentsText, const FString& In_tag);
};