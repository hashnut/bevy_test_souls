#pragma once

#include "GsSystemMessageHandlerInterface.h"
#include "../GsMessageSystem.h"
#include "../MessageParam/GsMessageParam.h"
#include "Runtime/Online/HTTP/Public/Interfaces/IHttpRequest.h"


using FGsSystemParamMessage = TGsMessageDelegateList<MSystemParam, MessageSystem>;

class T1PROJECT_API FGsSystemParamMessageHandlerGlobal :
	public IGsMessageHandler, FGsSystemParamMessage
{
public:
	static FString _smokeSignalUrl;
	static FString _smokeSignalFirst;
	static FString _smokeSignalSecond;
	static FString _smokeSignalThird;
	static int32 _signalNum;

public:
	FGsSystemParamMessageHandlerGlobal() = default;
	virtual ~FGsSystemParamMessageHandlerGlobal();

public:
	virtual void InitializeMessage() override;
	virtual void FinalizeMessage() override;

protected:
	// 하이브 인증 성공
	void OnHiveAuthTrue(const IGsMessageParam*);

public:
	void OnEndSequencePlayer(const IGsMessageParam*);

	void OnBeforeChangeLevel(const IGsMessageParam* InParam);

	/** 특정 개발 메세지 송신 초기화 */
	void OnInitSmokeSignal(const IGsMessageParam* InParam);

	/** 특정 개발 메세지 송신 */
	void OnSendSmokeSignal(const IGsMessageParam* InParam);

private:
	void OnSmokeUrlRequestComplete(FHttpRequestPtr InHttpRequest, FHttpResponsePtr InHttpResponse, bool InbSucceeded);
};
