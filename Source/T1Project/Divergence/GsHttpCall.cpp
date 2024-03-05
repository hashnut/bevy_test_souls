#include "GsHttpCall.h"

#include "Json/Public/Dom/JsonObject.h"
#include "Json/Public/Serialization/JsonReader.h"
#include "Json/Public/Serialization/JsonSerializer.h"
#include "Management/GsMessageHolder.h"
#include "T1Project.h"

#define LOG_DIVERGENCE

FGsHttpCall::FGsHttpCall()
{
	_http = &FHttpModule::Get();
}

// 호출 시작
void FGsHttpCall::StartHttpCall(const FString& In_url)
{
	FHttpRequestRef request = _http->CreateRequest();
	FString outputString;
	request->OnProcessRequestComplete().BindRaw(this, &FGsHttpCall::OnResponseReceived);
	request->SetURL(In_url);
	request->SetVerb("Get");
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));

	request->ProcessRequest();
}

/*again this function to call when the get request processes sucessfully*/
void FGsHttpCall::OnResponseReceived(FHttpRequestPtr In_request, FHttpResponsePtr In_response,	bool In_wasSuccessful)
{
	if (nullptr != _funcResultContents)
	{
		if (In_response != nullptr)
		{
			_funcResultContents(In_response->GetContentAsString());
		}
		// 실패시
		else
		{
			_funcResultContents(TEXT(""));
		}
	}
}