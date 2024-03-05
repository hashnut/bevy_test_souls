#pragma once
#include "CoreMinimal.h"
#include "Runtime/Online/HTTP/Public/Http.h"
/*
 HTTP 호출, 참조(https://wiki.unrealengine.com/UE4.10_How_To_Make_HTTP_GET_Request_in_C%2B%2B)
*/

class FHttpModule;

class FGsHttpCall
{
private:
	FHttpModule* _http;

public:
	// 결과텍스트 콜백
	TFunction<void(const FString & In_text)> _funcResultContents;


public:
	// 생성자
	FGsHttpCall();
	// 소멸자
	virtual ~FGsHttpCall() {}

	// 로직함수
public:
	// 호출 시작
	void StartHttpCall(const FString& In_url);
	/*again this function to call when the get request processes sucessfully*/
	// 서버 응답
	void OnResponseReceived(FHttpRequestPtr In_request, FHttpResponsePtr In_response, bool In_wasSuccessful);
};