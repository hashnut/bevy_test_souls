#include "GsGVSInfo.h"

#include "Math/NumericLimits.h"
#include "Kismet/GameplayStatics.h"
#include "T1Project.h"

// 로그 출력
void FGsGVSInfo::PrintLog()
{
	GSLOG(Warning,
		TEXT("platform: %s, chunk_num : %s, cdn_suburl: %s, service_type : %s, server_id: %s, auth_agent_port: %s "),
		*platform, *chunk_num, *cdn_suburl, *service_type, *server_id, *auth_agent_port);

	for (auto& iter : version)
	{
		GSLOG(Warning, TEXT("version: %s"), *iter);
	}

	for (auto& iter : patch_version)
	{
		GSLOG(Warning, TEXT("patch_version: %s"), *iter);
	}

	for (auto& iter : auth_agent)
	{
		GSLOG(Warning, TEXT("auth_agent: %s"), *iter);
	}
}

bool FGsGVSInfo::IsCurrentPlatform()
{
	FString devicePlatformName = UGameplayStatics::GetPlatformName();

	if (devicePlatformName.Compare(TEXT("Windows")) == 0)
	{
		if (platform.Compare(TEXT("Windows")) == 0)
		{
			return true;
		}
	}
	else if (devicePlatformName.Compare(TEXT("Android")) == 0)
	{
		if (platform.Compare(TEXT("Android")) == 0)
		{
			return true;
		}
	}
	else if (devicePlatformName.Compare(TEXT("IOS")) == 0)
	{		
		if (platform.Compare(TEXT("IOS")) == 0)
		{
			return true;
		}
	}

	return false;
}

// 서비스 타입이 동일한지?
bool FGsGVSInfo::IsSameServiceType(const FString& In_str)
{
	// 같으면 true, 다르면 false
	return (service_type.Compare(In_str) == 0) ? true : false;
}

// 버전 정보 파일을 가지고 있는지 검사
bool FGsGVSInfo::IsIncludeVersion(const FString& In_versetText)
{
	for (auto& iter : version)
	{
		// 같은지?
		if (iter.Compare(In_versetText) == 0)
		{
			return true;
		}
	}

	// 해당 버전이 없다
	return false;
}

// 문서정보에 의거 Version 비교시 version element는 어레이로 구성되기에
// 그중 가장 높은 버전과 비교한다
bool FGsGVSInfo::IsLessInVersion(float In_version)
{
	GSLOG(Log, TEXT("In_version %f"), In_version);

	float maxVer = TNumericLimits<float>::Min();
	for (auto& iter : version)
	{
		GSLOG(Log, TEXT("Target %s"), *iter);
		float parsingVer = FCString::Atof(*iter);
		if (maxVer < parsingVer)
		{
			maxVer = parsingVer;
		}
	}

	return maxVer < In_version;
}