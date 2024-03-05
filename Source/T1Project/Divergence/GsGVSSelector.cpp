// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGVSSelector.h"
#include "GsRelatedToPatchType.h"
#include "GsGVSPlatform.h"
#include "GsGVSInfo.h"
#include "GsDivergenceFunc.h"

#include "T1Project.h"

#define LOG_DIVERGENCE

FGsGVSSelector::FGsGVSSelector()
{
}

FGsGVSSelector::~FGsGVSSelector()
{
}
// 클라이언트 버전 로드
void FGsGVSSelector::LoadClientVersion()
{
	_clientVersion = FGsDivergenceFunc::GetVersion();

#ifdef LOG_DIVERGENCE
	GSLOG(Log, TEXT("LoadClientVersion _clientVersion: %s"), *_clientVersion);
#endif
}

void FGsGVSSelector::DoSelect(FGsGVSPlatform* In_platform)
{
	// 버전정보 없으면 처리하지 않는다
	if (_clientVersion.IsEmpty())
	{
		GSLOG(Error, TEXT("클라이언트 버전정보를 참고하지 못했음: %s"), *_clientVersion);
		return;
	}

	// 일단 GVS에 있는 플랫폼 파일을 준다. 클라이언트 버전 체크는 안한다
	_selectedInfo = In_platform->GetTypeGVS(EGsServiceType::dev);
	_hasSelect = true;

//	// 버전이 같으면
//	if (true == In_platform->EqualLiveVersion(_clientVersion))
//	{
//#ifdef LOG_DIVERGENCE
//		GSLOG(Log, TEXT("버전이 동일할때 해야 하는일"));
//#endif
//		// 리소스 업데이트 후 라이브서버로 접속
//		_selectedInfo = In_platform->GetTypeGVS(EGsServiceType::live);
//		// 선택 완료
//		_hasSelect = true;
//	}
//	else
//	{
//		// 클라이언트 버전코드가 gvs에 명시된 live 버전보다 낮은경우
//		if (In_platform->ClientVersionLessLiveVersion(_clientVersion))
//		{
//#ifdef LOG_DIVERGENCE
//			GSLOG(Log, TEXT("클라이언트 버전코드가 gvs에 명시된 live 버전보다 낮은경우"));
//#endif
//			// 선택 업데이트 경우 리소스 업데이트 후 server_url 접속
//			// 강제 업데이트인 경우 Hive initialize 과정에서 마켓 업데이트 링크로 이동
//			// 선택, 강제 업데이트 여부는 Hive세팅 방향에 따라 달라지므로 Hive플랫폼 팀과 협의 필요
//			_hasSelect = false;
//			_isNeedClientUpdate = true;
//		}
//		// 클라이언트 버전 코드가 gvs에 명시된 live 버전보다 높을 경우
//		else
//		{
//#ifdef LOG_DIVERGENCE
//			GSLOG(Log, TEXT("클라이언트 버전코드가 gvs에 명시된 live 버전보다 높은경우"));
//#endif
//			// live 환경이 아닌 다른환경(ex: submission, qa등에 일치하는 버전코드를 찾아서 접속
//			// 다른 환경에도 버전코드가 존재하지 않는 경우에 에러 처리
//			_selectedInfo = In_platform->GetMatchedGVS(_clientVersion);
//			if (nullptr != _selectedInfo)
//			{
//				_hasSelect = true;
//			}
//			else
//			{
//				_hasSelect = false;
//#ifdef LOG_DIVERGENCE
//				GSLOG(Log, TEXT("클라이언트 버전에 맞는 GVS파일을 찾지 못함"));
//#endif
//			}
//		}
//	}

	if (true == _hasSelect)
	{
#if PLATFORM_IOS
		GSLOG(Log, TEXT("Hive: Configuration.setServerId : %s"), *_selectedInfo->server_id);
#endif
#if PLATFORM_ANDROID
		GSLOG(Log, TEXT("Hive: Configuration.setServerId : %s"), *_selectedInfo->server_id);
#endif

	}

	
}